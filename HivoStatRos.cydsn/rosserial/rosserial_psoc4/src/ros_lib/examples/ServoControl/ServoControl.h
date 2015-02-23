/* include file for rosserial ADC example on PSoC4 with interrupt-driven conversion
 *
 */

#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

extern "C" {
#include "project.h"
}

namespace ServoControl {
   
extern void setup();
extern void loop();


} // namespace ServoControl
#endif
