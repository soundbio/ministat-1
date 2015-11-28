/* 
 * Button Example for Rosserial on PSoC4
 *  demo board SW1 button is on P0.7
 */

#include <ros.h>
#include <std_msgs/Bool.h>
#include "button_example.h"
// this defn from Cypress-generated Port).c:
#define SetP4PinDriveMode(shift, mode)  \
    do { \
        Port0_PC =   (Port0_PC & \
                                (uint32)(~(uint32)(Port0_DRIVE_MODE_IND_MASK << (Port0_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (Port0_DRIVE_MODE_BITS * (shift))); \
    } while (0)


extern ros::NodeHandle  nh;

namespace Button {

std_msgs::Bool pushed_msg;
ros::Publisher pub_button("pushed", &pushed_msg);

bool last_reading;
long last_debounce_time=0;
long debounce_delay=50;
bool published = true;

void setup()
{
  nh.advertise(pub_button);
  
  //Enable the pullup resistor on the button
  SetP4PinDriveMode(Port0__7__SHIFT, Port0_DM_RES_UP);
  
  //The button is a normally open button
  last_reading = !( (Port0_Read()>>Port0__7__SHIFT) & 0x01 );
 
}

void loop()
{
  
  bool reading = !( (Port0_Read()>>Port0__7__SHIFT) & 0x01 );
  
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
      nh.loginfo("button pushed");
    }
  }

  last_reading = reading;
}

} //namespace
