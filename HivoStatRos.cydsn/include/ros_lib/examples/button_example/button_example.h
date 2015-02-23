/* include file for rosserial ADC example on PSoC4 with interrupt-driven conversion
 *
 */

#ifndef BUTTON_H
#define BUTTON_H

extern "C" {
#include "project.h"
}

namespace Button {
   
extern void setup();
extern void loop();


} // namespace Button
#endif
