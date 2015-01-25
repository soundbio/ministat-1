/* 
 * HiveBio Ministat turbidostat system
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
#include <project.h>
#include "SysTimer.h"
#include "isnprintf.h"
#include "ADC_proc.h"

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
    CyGlobalIntEnable; // Enable interrupts
    
    uint32 report_interval_ms = 100;
    uint32 next_report_time = millis() + report_interval_ms;
    uint32 toggle_interval_ms = 20;
    uint32 next_toggle_time = millis() + toggle_interval_ms;
    uint32 reboot_timer = millis();
    uint8 reboot_count =0;
    uint32 photo_bg = 0, photo_fg = 0;
    uint32 led_volt_bg = 0, led_volt_fg = 0;
    uint32 led_sense_bg = 0, led_sense_fg = 0;

    for(;;)
    {
        // alternate foreground (LED on) and backgroung (LED off) measurement
        if ((int32)(millis()-next_toggle_time) >= 0) {
            next_toggle_time += toggle_interval_ms;
            if (detector_phase == LED_on) {
                detector_phase = collect_on;
            } else if (detector_phase == collect_on) {
                photo_fg = adc_result[0]; // photodiode signal
                led_sense_fg = adc_result[1]; // LED current
                led_volt_fg = adc_result[2]; // LED anode voltage (ref to gnd)
                LED_driver_Stop();
                detector_phase = LED_off;
            } else if (detector_phase == LED_off) {
                detector_phase = collect_off;
            } else {
                photo_bg = adc_result[0];
                led_sense_bg = adc_result[1];
                led_volt_bg = adc_result[2];
                LED_driver_Enable();
                detector_phase = LED_on;
            }
        }
        // periodically report results
        if ((int32)(millis()-next_report_time) >= 0) {
            next_report_time += report_interval_ms;
            // take (foreground - background)
            uint8* byteptr;
            uint32 photoval = photo_fg - photo_bg;
            uint32 ledsenseval = led_sense_fg - led_sense_bg;
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
            byteptr = (uint8*)(&ledsenseval);
            count = isnprintf(buf, 30, "S%c%c%c%c", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&ledvoltval);
            count = isnprintf(buf, 30, "V%c%c%c%c", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            byteptr = (uint8*)(&adjusted_photoval);
            count = isnprintf(buf, 30, "J%c%c%c%c\r\n", 
              *byteptr, *(byteptr+1), *(byteptr+2), *(byteptr+3)); 
            Host_UART_SpiUartPutArray((uint8*)buf, count);
        }
            
        // watch for pushbutton held down 1-2 sec (reboot request)
        if ((int32)(millis()-reboot_timer) >= 0) {
            if(Button_Read()) {
                reboot_count = 0;
            } else {
                if(++reboot_count > 1) {
                    Bootloadable_Load();
                }
            }
            reboot_timer += 1000;
        }
    }
}
