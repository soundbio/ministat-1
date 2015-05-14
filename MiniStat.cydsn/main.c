/* 
 * HiveBio Ministat turbidostat system
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
#include <project.h>
#include <math.h>
#include "SysTimer.h"
#include "isnprintf.h"
#include "ADC_proc.h"
#include "Thermistor_linearize.h"

// LED thermal drift correction
#define NOM_LEDVOLT 2970000L
#define OFFSET_LEDVOLT (1./3. * 32768 * 100) // LED cathode is at Vref
#define LED_CORR_FACTOR (6.2)  // % change in light per 1% change in LED voltage

enum { LED_on, collect_on, LED_off, collect_off } detector_phase = LED_on;

int main()
{
    systimer_setup(); // sets up millisec() hardware timer
    Host_UART_Start(); // serial port
    Transimpedance_Start(); // photodiode op amp
    LED_driver_Start(); // LED driver op amp
    adc_setup(); // A to D converter
    STIR_PWM_Start();
    PUMP_PWM_Start();
    CyGlobalIntEnable; // Enable interrupts
    
    uint32 report_interval_ms = 100;
    uint32 next_report_time = millis() + report_interval_ms;
    uint32 RX8_reports_remaining = 0;
    uint32 toggle_interval_ms = 20;
    uint32 next_toggle_time = millis() + toggle_interval_ms;
    uint32 reboot_timer = millis();
    uint8 reboot_count =0;
    uint32 photo_bg = 0, photo_fg = 0;
    uint32 led_volt_bg = 0, led_volt_fg = 0;
    uint16 pump_run_pwm = 0;
    uint16 pump_steps_target = 0;
    int32 input_param = 0;

    for(;;)
    {
        // alternate foreground (LED on) and backgroung (LED off) measurement
        if ((int32)(millis()-next_toggle_time) >= 0) {
            next_toggle_time += toggle_interval_ms;
            if (detector_phase == LED_on) {
                detector_phase = collect_on;
            } else if (detector_phase == collect_on) {
                photo_fg = adc_result[adc_chan_photosensor]; // photodiode signal
                led_volt_fg = adc_result[adc_chan_led_voltage]; // LED anode voltage (ref to gnd)
                LED_driver_Stop();
                detector_phase = LED_off;
            } else if (detector_phase == LED_off) {
                detector_phase = collect_off;
            } else {
                photo_bg = adc_result[adc_chan_photosensor];
                led_volt_bg = adc_result[adc_chan_led_voltage];
                LED_driver_Enable();
                detector_phase = LED_on;
            }
        }
        // periodically report results to Cypress Bridge Control Panel (RX8)
        if (RX8_reports_remaining > 0 && (int32)(millis()-next_report_time) >= 0) {
            next_report_time += report_interval_ms;
            RX8_reports_remaining -= 1;
            // take (foreground - background)
            uint8* byteptr;
            uint32 photoval = photo_fg - photo_bg;
            uint32 ledvoltval = led_volt_fg - led_volt_bg;
            // apply LED thermal drift correction
            float delta_ledvolt = (int32)(ledvoltval - NOM_LEDVOLT)/(NOM_LEDVOLT - OFFSET_LEDVOLT);
            uint32 adjusted_photoval = photoval / (1.0 + delta_ledvolt*LED_CORR_FACTOR);
            // output values on serial port; hex then binary
            char buf[30];
            byteptr = (uint8*)(&photoval);
            int32 count = isnprintf(buf, 30, "0x%x R%c%c%c%c", photoval,
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&(adc_result[adc_chan_pump_sensor]));
            count = isnprintf(buf, 30, "S%c%c%c%c", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&ledvoltval);
            count = isnprintf(buf, 30, "V%c%c%c%c", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&adjusted_photoval);
            count = isnprintf(buf, 30, "J%c%c%c%c", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&adc_pump_state.count);
            count = isnprintf(buf, 30, "P%c%c", 
              *byteptr, *(byteptr+1)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&(adc_result[adc_chan_thermistor]));
            count = isnprintf(buf, 30, "T%c%c%c%c", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            float volt_ratio = adc_result[adc_chan_thermistor]/pow(2.0, 15);
            const float thermistor_factor = 0.989; // ratio of divider top resistor to thermistor 25C value 
            float thermistor_temp = ThermistorTempC(thermistor_factor*volt_ratio/(1.0-volt_ratio), 25.0, 3380.);            byteptr = (uint8*)(&(adc_result[adc_chan_thermistor]));
            byteptr = (uint8*)(&(thermistor_temp));
            count = isnprintf(buf, 30, "C%c%c%c%c\r\n", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);

        }
        P1_6_Write(RX8_reports_remaining>0); // Blue LED on in RX8 mode
        
        if((int16)(adc_pump_state.count-pump_steps_target)>=0) {
            PUMP_PWM_WriteCompare(0);
        }
        
        // watch for serial input
        // simple serial command protocol, single-letter commands:
        //    [<numeric parameter>]<command letter>
        // e.g. '150s' sets stir speed to 150
        //
        if (Host_UART_SpiUartGetRxBufferSize() != 0) {
            char in_char = (char)(Host_UART_SpiUartReadRxData() & 0xFF);
            if(RX8_reports_remaining==0) { // echo
                Host_UART_UartPutChar(in_char); // echo
                if (in_char=='\r') {
                    Host_UART_UartPutChar('\n');
                }
            }
            if (in_char>='0' && in_char<='9') {
                // collect a numerical parameter value
                input_param = input_param*10 + (in_char-'0');
            } else {
                // dispatch
                switch (in_char) {
                    case 'R':
                        // toggle RX8 (Blue LED = talking RX8)
                        RX8_reports_remaining = input_param;
                        if (RX8_reports_remaining > 0) {
                            next_report_time = millis()+10000; // 10 sec startup delay
                        }
                        break;
                    case 'P':
                        // run pump for n steps (6 steps per rev)
                        pump_steps_target = adc_pump_state.count + input_param;
                        PUMP_PWM_WriteCompare(pump_run_pwm);
                        break;
                    case 'p':
                        // Set pump running pwm level
                        pump_run_pwm = input_param & 0x3FF;
                        break;
                    case 's':
                        // Set stir motor pwm level
                        STIR_PWM_WriteCompare(input_param & 0x3FF);
                    default:
                        break;
                }
                input_param = 0;
            }
        }
        // clear serial RX error flags if any
        if (Host_UART_CHECK_INTR_RX(Host_UART_INTR_RX_ERR)) {
            Host_UART_ClearRxInterruptSource(Host_UART_INTR_RX_ERR);
        }

        // watch for pushbutton
        //   Stop RX8 transmission
        //   Held down 1-2 sec = reboot request
        if ((int32)(millis()-reboot_timer) >= 0) {
            if(Button_Read()) {
                reboot_count = 0;
            } else {
                RX8_reports_remaining = 0;
                if(++reboot_count > 1) {
                    Bootloadable_Load();
                }
            }
            reboot_timer += 1000;
        }
    }
}
