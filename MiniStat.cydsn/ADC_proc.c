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
int32 accum[ADC_TOTAL_CHANNELS_NUM];
int16 accum_count;
#define NUM_ACCUMS 100

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
        for (chan=0; chan<ADC_TOTAL_CHANNELS_NUM; ++chan)
        {
            /* save accumulated reading if ready*/
            if(accum_count==0) {
                adc_result[chan] = accum[chan];
                accum[chan] = 0;
            }
            /* Read conversion result */
            accum[chan] += ADC_GetResult16(chan);
        }    
    }    

    /* Clear handled interrupt */
    ADC_SAR_INTR_REG = intr_status;
}

void adc_setup() {
  /* Init and start sequencing SAR ADC */
  ADC_Start();
  ADC_StartConvert();
  /* Enable interrupt and set interrupt handler to local routine */
  ADC_IRQ_StartEx(ADC_ISR_LOC);
}
