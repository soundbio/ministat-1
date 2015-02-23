/* 
 * Capsense Button Example for PSoC4 "RosOnAStick"
 *  demo board capsense button0 is on P3.1
 *
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include <ros.h>
#include <std_msgs/Bool.h>
#include "capsense_button.h"

extern ros::NodeHandle  nh;

namespace Capsense {

std_msgs::Bool pushed_msg;
ros::Publisher pub_button("capsense", &pushed_msg);

bool last_reading;
long last_debounce_time=0;
long debounce_delay=50;
bool published = true;

void setup()
{
  nh.advertise(pub_button);
  
  CapSense_Start();
  CapSense_InitializeAllBaselines();
  
  last_reading = 0;
  
 
}

void loop()
{
  CapSense_UpdateEnabledBaselines();
  if (CapSense_IsBusy()) {
    return;
  }
  bool reading = CapSense_CheckIsWidgetActive(CapSense_BUTTON0__BTN);
  
  if (last_reading!= reading){
      last_debounce_time = SysTimer::millis();
      published = false;
  }
  
  //if the button value has not changed for the debounce delay, we know its stable
  if ( !published && ((int32_t)SysTimer::millis() - last_debounce_time)  > debounce_delay) {
    pushed_msg.data = reading;
    pub_button.publish(&pushed_msg);
    published = true;
    if (reading) {
      nh.loginfo("capsense button pushed");
    }
  }

  last_reading = reading;
  CapSense_ScanEnabledWidgets();
  return;
}
} // namespace Capsense
