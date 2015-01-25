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
            // take (foreground - background); split to 4 bytes for output
            uint32 photoval = photo_fg - photo_bg;
            uint32 photoval_bytes[4] = { (photoval & 0xff000000) >> 24, (photoval & 0x00ff0000) >> 16,
                                         (photoval & 0x0000ff00) >> 8, photoval & 0x000000ff };
            uint32 ledsenseval = led_sense_fg - led_sense_bg;
            uint32 ledsenseval_bytes[4] = { (ledsenseval & 0xff000000) >> 24, (ledsenseval & 0x00ff0000) >> 16,
                                         (ledsenseval & 0x0000ff00) >> 8, ledsenseval & 0x000000ff };
            uint32 ledvoltval = led_volt_fg - led_volt_bg;
            uint32 ledvoltval_bytes[4] = { (ledvoltval & 0xff000000) >> 24, (ledvoltval & 0x00ff0000) >> 16,
                                         (ledvoltval & 0x0000ff00) >> 8, ledvoltval & 0x000000ff };
            // output values on serial port; hex then binary
            char buf[30];
            int32 count = isnprintf(buf, 30, "0x%x R%c%c%c%c", photoval,
              photoval_bytes[0], photoval_bytes[1], photoval_bytes[2], photoval_bytes[3]);
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            count = isnprintf(buf, 30, "S%c%c%c%c", 
              ledsenseval_bytes[0], ledsenseval_bytes[1], ledsenseval_bytes[2], ledsenseval_bytes[3]);
            Host_UART_SpiUartPutArray((uint8*)buf, count);
            count = isnprintf(buf, 30, "V%c%c%c%c\r\n", 
              ledvoltval_bytes[0], ledvoltval_bytes[1], ledvoltval_bytes[2], ledvoltval_bytes[3]);
            Host_UART_SpiUartPutArray((uint8*)buf, count);
        }
            
        // watch for pushbutton held down 1-2 sec (reboot request)
        if ((int32)(millis()-reboot_timer) >= 0) {
            if(Button_Read()) {
                reboot_count = 0;
            } else {
                if(++reboot_count > 2) {
                    Bootloadable_Load();
                }
            }
            reboot_timer += 1000;
        }
    }
}
