/* 
 * rosserial Up/Down count example for PSoC4 "RosOnAStick"
 * 
 * reads a differential pulse rate signal (e.g. HP interferometer)
 *   using a PSoC hardware counter
 *
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Empty.h>
#include "UpDownCount.h"

extern ros::NodeHandle nh;

namespace UpDownCount {

std_msgs::Int32 updown_msg;
ros::Publisher p("updown", &updown_msg);

void resetCb( const std_msgs::Empty& msg){
  UpDown_WriteCounter(0);
}

ros::Subscriber<std_msgs::Empty> sub("updown_reset", &resetCb );

uint32_t next_report_time;
const uint32_t kReportIntervalMs = 200;

void setup()
{ 
  nh.advertise(p);
  nh.subscribe(sub);
  next_report_time = SysTimer::millis();
  UpDown_Start();
}

void loop()
{
  if ((int32_t)(SysTimer::millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;
    updown_msg.data = UpDown_ReadCounter();
    p.publish(&updown_msg);
  }
}

} // namespace UpDownCount
