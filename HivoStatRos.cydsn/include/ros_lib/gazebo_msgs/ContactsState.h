#ifndef _ROS_gazebo_msgs_ContactsState_h
#define _ROS_gazebo_msgs_ContactsState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "gazebo_msgs/ContactState.h"

namespace gazebo_msgs
{

  class ContactsState : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint8_t states_length;
      gazebo_msgs::ContactState st_states;
      gazebo_msgs::ContactState * states;

    ContactsState():
      header(),
      states_length(0), states(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset++) = states_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < states_length; i++){
      offset += this->states[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->header.deserialize(inbuffer + offset);
      uint8_t states_lengthT = *(inbuffer + offset++);
      if(states_lengthT < states_length)
        states_length = states_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < states_lengthT; i++){
      offset += this->st_states.deserialize(inbuffer + offset);
      if (i < states_length) {
          memcpy( &(this->states[i]), &(this->st_states), sizeof(gazebo_msgs::ContactState));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "gazebo_msgs/ContactsState"; };
    const char * getMD5(){ return "acbcb1601a8e525bf72509f18e6f668d"; };

  };

}
#endif