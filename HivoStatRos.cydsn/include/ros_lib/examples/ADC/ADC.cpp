/* 
 * rosserial ADC Example
 * 
 * This is a poor man's Oscilloscope.  It does not have the sampling 
 * rate or accuracy of a commerical scope, but it is great to get
 * an analog value into ROS in a pinch.
 */

#include <ros.h>
#include <rosserial_psoc4/Adc.h>
#include "ADC_psoc4.h"

extern ros::NodeHandle nh;

namespace ADC {

rosserial_psoc4::Adc adc_msg;
ros::Publisher p("adc", &adc_msg);

uint32_t next_report_time;
const uint32_t kReportIntervalMs = 250;

static volatile int16 result[ADC_SAR_SEQ_TOTAL_CHANNELS_NUM];
CY_ISR(ADC_SAR_SEQ_ISR_LOC)
{
    uint32 intr_status;

    /* Read interrupt status registers */
    intr_status = ADC_SAR_SEQ_SAR_INTR_MASKED_REG;
    /* Check for End of Scan interrupt */
    if((intr_status & ADC_SAR_SEQ_EOS_MASK) != 0u)
    {
        unsigned int chan;
        for (chan=0; chan<ADC_SAR_SEQ_TOTAL_CHANNELS_NUM; ++chan)
        {
            /* Read conversion result */
            result[chan] = ADC_SAR_SEQ_GetResult16(chan);

        }    
    }    

    /* Clear handled interrupt */
    ADC_SAR_SEQ_SAR_INTR_REG = intr_status;
}

void adc_setup() {
  /* Init and start sequencing SAR ADC */
  ADC_SAR_SEQ_Start();
  ADC_SAR_SEQ_StartConvert();
  /* Enable interrupt and set interrupt handler to local routine */
  ADC_SAR_SEQ_IRQ_StartEx(ADC_SAR_SEQ_ISR_LOC);
}

int analogRead(int pin) {
  return result[pin];
}

void setup()
{ 
  nh.advertise(p);
  next_report_time = SysTimer::millis();
  adc_setup();
}

//We sum the analog reading to elminate some of the noise
int averageAnalog(int pin){
  int v=0;
  for(int i=0; i<4; i++) v += analogRead(pin);
  if(v<0) v = 0;
  return v;
}

void loop()
{
  if ((int32_t)(SysTimer::millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;

    adc_msg.adc0 = averageAnalog(0);
    adc_msg.adc1 = averageAnalog(1);
   
    p.publish(&adc_msg);
  }
}

} // namespace ADC