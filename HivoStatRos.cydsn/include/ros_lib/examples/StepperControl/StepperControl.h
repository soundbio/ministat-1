/* include file for rosserial Stepper example on PSoC4 
 *
 */

#ifndef STEPPER_CONTROL_H
#define STEPPER_CONTROL_H

extern "C" {
#include "project.h"
}

namespace StepperControl {
   
extern void setup();
extern void loop();


} // namespace StepperControl
#endif
