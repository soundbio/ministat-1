/* 
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */
 
extern "C" {
  #include "project.h"
}

#include <ros.h>
#include <std_msgs/Empty.h>
extern ros::NodeHandle  nh;

namespace Blink {

uint8_t ledOn;
void messageCb( const std_msgs::Empty& toggle_msg){
  ledOn = 1-ledOn;
}

ros::Subscriber<std_msgs::Empty> sub("toggle_led", &messageCb );

void setup()
{
  ledOn = 0;
  P1_6_Write(ledOn);
  nh.subscribe(sub);
}

void loop()
{  
  P1_6_Write(ledOn);
}

} // namespace Blink
