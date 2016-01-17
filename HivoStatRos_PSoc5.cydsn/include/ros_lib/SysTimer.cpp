/* 
 * system millisec timer for HiveBio Ministat PSoC5
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
extern "C" {
#include "project.h"
}

volatile uint32_t millis_;

CY_ISR(msec_Interrupt) {
  ++millis_;
  Millisec_ReadStatusRegister();
};

void systimer_setup() {
  isr_msec_StartEx(msec_Interrupt);
  millis_ = 0;
  Millisec_Start();
};

uint32_t millis() { return millis_; };
