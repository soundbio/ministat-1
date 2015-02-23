/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <std_msgs/String.h>
extern ros::NodeHandle  nh;

namespace HelloWorld {

std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

char hello[13] = "hello world!";
uint32_t next_time;

void setup()
{
  nh.advertise(chatter);
  next_time = SysTimer::millis();
}

void loop()
{
  if ((int32_t)(SysTimer::millis()-next_time)>0) {
    next_time += 1000; // delay 1 sec
    str_msg.data = hello;
    chatter.publish( &str_msg );
  }
}

} // namespace