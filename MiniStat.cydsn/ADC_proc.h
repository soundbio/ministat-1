/* 
 * Analog to Digital conversion for HiveBio Ministat
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */


#ifndef ADC_PROC_H
#define ADC_PROC_H
#include <project.h>
    
extern volatile int32 adc_result[];
extern struct PumpState
{
    int16 thresh;
    int16 hyst;
    uint16 count;
} adc_pump_state;

enum {
    adc_chan_photosensor,
    adc_chan_led_voltage,
    ADC_NUM_SYNC_DETECT_CHANS,
    adc_chan_pump_sensor = ADC_NUM_SYNC_DETECT_CHANS
};

void adc_setup();

#endif
