#ifndef _ROS_std_msgs_Float64MultiArray_h
#define _ROS_std_msgs_Float64MultiArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/MultiArrayLayout.h"

namespace std_msgs
{

  class Float64MultiArray : public ros::Msg
  {
    public:
      std_msgs::MultiArrayLayout layout;
      uint8_t data_length;
      float st_data;
      float * data;

    Float64MultiArray():
      layout(),
      data_length(0), data(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->layout.serialize(outbuffer + offset);
      *(outbuffer + offset++) = data_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < data_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->data[i]);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->layout.deserialize(inbuffer + offset);
      uint8_t data_lengthT = *(inbuffer + offset++);
      if(data_lengthT < data_length)
        data_length = data_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < data_lengthT; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_data));
      if (i < data_length) {
          memcpy( &(this->data[i]), &(this->st_data), sizeof(float));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "std_msgs/Float64MultiArray"; };
    const char * getMD5(){ return "4b7d974086d4060e7db4613a7e6c3ba4"; };

  };

}
#endif