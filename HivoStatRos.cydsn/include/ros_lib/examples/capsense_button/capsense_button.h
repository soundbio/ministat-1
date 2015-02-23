/* include file for rosserial capsense button on PSoC4 
 *
 */

#ifndef CAPSENSE_BUTTON_H
#define CAPSENSE_BUTTON_H

extern "C" {
#include "project.h"
}

namespace Capsense {
   
extern void setup();
extern void loop();


} // namespace Capsense
#endif
