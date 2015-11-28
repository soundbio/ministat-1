/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and Cypress PSoC4 "RosOnAStick"
 * 
 */


#include <ros.h>
#include <std_msgs/UInt16.h>
#include "ServoControl.h"

extern ros::NodeHandle  nh;

namespace ServoControl {

void servo_us_cb( const std_msgs::UInt16& cmd_msg){
  // set servo pulse width in microseconds
  uint16_t width = cmd_msg.data;
  if (width<500) { width = 500; }
  if (width>2000) { width = 2000; }
  PWM_1_WriteCompare(width); 
}


ros::Subscriber<std_msgs::UInt16> sub("servo_us", servo_us_cb);

void setup(){
  nh.subscribe(sub);
  
  PWM_1_Start();
}

void loop(){
}

} // namespace
