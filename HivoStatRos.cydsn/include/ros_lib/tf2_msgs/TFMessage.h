#ifndef _ROS_tf2_msgs_TFMessage_h
#define _ROS_tf2_msgs_TFMessage_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/TransformStamped.h"

namespace tf2_msgs
{

  class TFMessage : public ros::Msg
  {
    public:
      uint8_t transforms_length;
      geometry_msgs::TransformStamped st_transforms;
      geometry_msgs::TransformStamped * transforms;

    TFMessage():
      transforms_length(0), transforms(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = transforms_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < transforms_length; i++){
      offset += this->transforms[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t transforms_lengthT = *(inbuffer + offset++);
      if(transforms_lengthT < transforms_length)
        transforms_length = transforms_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < transforms_lengthT; i++){
      offset += this->st_transforms.deserialize(inbuffer + offset);
      if (i < transforms_length) {
          memcpy( &(this->transforms[i]), &(this->st_transforms), sizeof(geometry_msgs::TransformStamped));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "tf2_msgs/TFMessage"; };
    const char * getMD5(){ return "94810edda583a504dfda3829e70d7eec"; };

  };

}
#endif