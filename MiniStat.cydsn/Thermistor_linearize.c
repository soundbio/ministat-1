/* 
 * Thermistor linearization for HiveBio Ministat
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
#include <project.h>
#include "Thermistor_linearize.h"
#include <math.h>

float ThermistorTempC(float resistance_ratio, float T0_C, float B)
{
    float tf = log(resistance_ratio)/B;
    float T0_K = T0_C+zeroC;
    return T0_K/(1+T0_K*tf) - zeroC;
}