#ifndef _ROS_wiimote_State_h
#define _ROS_wiimote_State_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Vector3.h"
#include "wiimote/IrSourceInfo.h"
#include "ros/time.h"

namespace wiimote
{

  class State : public ros::Msg
  {
    public:
      std_msgs::Header header;
      geometry_msgs::Vector3 angular_velocity_zeroed;
      geometry_msgs::Vector3 angular_velocity_raw;
      float angular_velocity_covariance[9];
      geometry_msgs::Vector3 linear_acceleration_zeroed;
      geometry_msgs::Vector3 linear_acceleration_raw;
      float linear_acceleration_covariance[9];
      geometry_msgs::Vector3 nunchuk_acceleration_zeroed;
      geometry_msgs::Vector3 nunchuk_acceleration_raw;
      float nunchuk_joystick_zeroed[2];
      float nunchuk_joystick_raw[2];
      bool buttons[11];
      bool nunchuk_buttons[2];
      bool LEDs[4];
      bool rumble;
      uint8_t ir_tracking_length;
      wiimote::IrSourceInfo st_ir_tracking;
      wiimote::IrSourceInfo * ir_tracking;
      float raw_battery;
      float percent_battery;
      ros::Time zeroing_time;
      uint64_t errors;
      enum { INVALID =  -1 };
      enum { INVALID_FLOAT =  -1.0 };
      enum { MSG_BTN_1 =  0 };
      enum { MSG_BTN_2 =  1 };
      enum { MSG_BTN_A =  2 };
      enum { MSG_BTN_B =  3 };
      enum { MSG_BTN_PLUS =  4 };
      enum { MSG_BTN_MINUS =  5 };
      enum { MSG_BTN_LEFT =  6 };
      enum { MSG_BTN_RIGHT =  7 };
      enum { MSG_BTN_UP =  8 };
      enum { MSG_BTN_DOWN =  9 };
      enum { MSG_BTN_HOME =  10 };
      enum { MSG_BTN_Z =  0 };
      enum { MSG_BTN_C =  1 };
      enum { MSG_CLASSIC_BTN_X =  0 };
      enum { MSG_CLASSIC_BTN_Y =  1 };
      enum { MSG_CLASSIC_BTN_A =  2 };
      enum { MSG_CLASSIC_BTN_B =  3 };
      enum { MSG_CLASSIC_BTN_PLUS =  4 };
      enum { MSG_CLASSIC_BTN_MINUS =  5 };
      enum { MSG_CLASSIC_BTN_LEFT =  6 };
      enum { MSG_CLASSIC_BTN_RIGHT =  7 };
      enum { MSG_CLASSIC_BTN_UP =  8 };
      enum { MSG_CLASSIC_BTN_DOWN =  9 };
      enum { MSG_CLASSIC_BTN_HOME =  10 };
      enum { MSG_CLASSIC_BTN_L =  11 };
      enum { MSG_CLASSIC_BTN_R =  12 };
      enum { MSG_CLASSIC_BTN_ZL =  13 };
      enum { MSG_CLASSIC_BTN_ZR =  14 };

    State():
      header(),
      angular_velocity_zeroed(),
      angular_velocity_raw(),
