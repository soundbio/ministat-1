/* 
 * system millisec timer for HiveBio Ministat
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
#include <project.h>
#define SYSTICK_INTERRUPT_VECTOR_NUMBER 15u /* Cortex-M0 hard vector */
#define CLOCK_FREQ     CYDEV_BCLK__SYSCLK__HZ

uint32 millis_;

CY_ISR(SysTick_ISR) {
  ++millis_;
};

void systimer_setup() {
  CyIntSetSysVector(SYSTICK_INTERRUPT_VECTOR_NUMBER, SysTick_ISR);
  SysTick_Config(CLOCK_FREQ / 1000);
  millis_ = 0;
};

uint32 millis() { return millis_; };

