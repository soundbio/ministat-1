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

void adc_setup();

#endif
