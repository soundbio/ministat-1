/* Clock initialization for PSoc4 "RosOnAStick"
 *
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
 
#include "ros.h"
#include "SysTimer.h"
extern "C" {
  #include "project.h"
}

void init(void)
{
  SysTimer::init();
  SysTick_Config(CLOCK_FREQ / INTERRUPT_FREQ); /* defined in auto-generated core_cm0.h */

  CyGlobalIntEnable;
}
