/* 
 * Thermistor linearization for HiveBio Ministat
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */


#ifndef THERM_LIN_H
#define THERM_LIN_H
#include <project.h>
    
#define zeroC (273.15)
    
float ThermistorTempC(float resistance_ratio, float T0, float B);

#endif
