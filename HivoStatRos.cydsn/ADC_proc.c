/* 
 * Analog to Digital conversion for HiveBio Ministat
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
#include <project.h>
#include "ADC_proc.h"

volatile int32 adc_result[ADC_TOTAL_CHANNELS_NUM];
int32 accum[ADC_NUM_SYNC_DETECT_CHANS];
int16 accum_count;
#define NUM_ACCUMS 100
struct PumpState adc_pump_state;

CY_ISR(ADC_ISR_LOC)
{
    uint32 intr_status;

    /* Read interrupt status registers */
    intr_status = ADC_SAR_INTR_MASKED_REG;
    /* Check for End of Scan interrupt */
    if((intr_status & ADC_EOS_MASK) != 0u)
    {
        if(++accum_count >= NUM_ACCUMS) {
            accum_count = 0;
        }
        unsigned int chan;
        for (chan=0; chan<ADC_NUM_SYNC_DETECT_CHANS; ++chan)
        {
            /* save accumulated reading if ready*/
            if(accum_count==0) {
                adc_result[chan] = accum[chan];
                accum[chan] = 0;
            }
            /* Read conversion result */
            accum[chan] += ADC_GetResult16(chan);
        } 
        // count pump sensor transitions
        adc_result[adc_chan_pump_sensor] = ADC_GetResult16(adc_chan_pump_sensor);
        if (adc_pump_state.count&0x01)
        { // sensor state high
            if(ADC_GetResult16(adc_chan_pump_sensor)<(adc_pump_state.thresh-adc_pump_state.hyst))
            {
                adc_pump_state.count++;
            }
        } else
        { // sensor state low
            if(ADC_GetResult16(adc_chan_pump_sensor)>adc_pump_state.thresh)
            {
                adc_pump_state.count |= 1;
            }
        }
    }    

    /* Clear handled interrupt */
    ADC_SAR_INTR_REG = intr_status;
}

void adc_setup() {
    // initialize pump sensor state
    const int16 kThresh = 25000;
    const int16 kHyst = kThresh/5;
    adc_pump_state.thresh = kThresh;
    adc_pump_state.hyst = kHyst;
    adc_pump_state.count = 0;
    /* Init and start sequencing SAR ADC */
    ADC_Start();
    ADC_StartConvert();
    /* Enable interrupt and set interrupt handler to local routine */
    ADC_IRQ_StartEx(ADC_ISR_LOC);
}
