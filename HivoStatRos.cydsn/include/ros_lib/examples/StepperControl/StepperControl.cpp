/* 
 * rosserial Subscriber Example
 * Move stepper motor PSoC4 "RosOnAStick"
 *
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
 
#include <ros.h>
#include <std_msgs/Int16.h>
#include "StepperControl.h"

extern ros::NodeHandle  nh;

namespace StepperControl {

const uint8_t msec_per_step = 10;
const uint8_t PRESET = 0x04;
const uint8_t STEP_UP = 0x06;
const uint8_t STEP_DN = 0x07;

int16_t steps_remaining = 0;
uint32_t next_time;

void messageCb( const std_msgs::Int16& step_msg){
  steps_remaining = step_msg.data;
}

ros::Subscriber<std_msgs::Int16> sub("stepper_move", &messageCb );

void setup()
{
  nh.subscribe(sub);
  next_time = SysTimer::millis();
  Stepper_Control_Write(PRESET);
}

void loop()
{
  if ((int32_t)(SysTimer::millis()-next_time)>0) {
    next_time += msec_per_step;
    if (steps_remaining > 0) {
      Stepper_Control_Write(STEP_UP);
      steps_remaining--;
    } else if (steps_remaining < 0) {
      Stepper_Control_Write(STEP_DN);
      steps_remaining++;
    }
  }
}

} // namespace StepperControl
