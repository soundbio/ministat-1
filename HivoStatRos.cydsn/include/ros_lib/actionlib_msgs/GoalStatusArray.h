#ifndef _ROS_actionlib_msgs_GoalStatusArray_h
#define _ROS_actionlib_msgs_GoalStatusArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"

namespace actionlib_msgs
{

  class GoalStatusArray : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint8_t status_list_length;
      actionlib_msgs::GoalStatus st_status_list;
      actionlib_msgs::GoalStatus * status_list;

    GoalStatusArray():
      header(),
      status_list_length(0), status_list(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset++) = status_list_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < status_list_length; i++){
      offset += this->status_list[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->header.deserialize(inbuffer + offset);
      uint8_t status_list_lengthT = *(inbuffer + offset++);
      if(status_list_lengthT < status_list_length)
        status_list_length = status_list_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < status_list_lengthT; i++){
      offset += this->st_status_list.deserialize(inbuffer + offset);
      if (i < status_list_length) {
          memcpy( &(this->status_list[i]), &(this->st_status_list), sizeof(actionlib_msgs::GoalStatus));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "actionlib_msgs/GoalStatusArray"; };
    const char * getMD5(){ return "8b2b82f13216d0a8ea88bd3af735e619"; };

  };

}
#endif