/* include file for rosserial ADC example on PSoC4 with interrupt-driven conversion
 *
 */

#ifndef ADC_PSOC4_H
#define ADC_PSOC4_H

extern "C" {
#include "project.h"
}

namespace ADC {
   
extern void setup();
extern void loop();


} // namespace ADC
#endif
