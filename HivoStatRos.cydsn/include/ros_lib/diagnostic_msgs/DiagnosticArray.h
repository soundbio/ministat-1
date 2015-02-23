#ifndef _ROS_diagnostic_msgs_DiagnosticArray_h
#define _ROS_diagnostic_msgs_DiagnosticArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "diagnostic_msgs/DiagnosticStatus.h"

namespace diagnostic_msgs
{

  class DiagnosticArray : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint8_t status_length;
      diagnostic_msgs::DiagnosticStatus st_status;
      diagnostic_msgs::DiagnosticStatus * status;

    DiagnosticArray():
      header(),
      status_length(0), status(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset++) = status_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < status_length; i++){
      offset += this->status[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->header.deserialize(inbuffer + offset);
      uint8_t status_lengthT = *(inbuffer + offset++);
      if(status_lengthT < status_length)
        status_length = status_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < status_lengthT; i++){
      offset += this->st_status.deserialize(inbuffer + offset);
      if (i < status_length) {
          memcpy( &(this->status[i]), &(this->st_status), sizeof(diagnostic_msgs::DiagnosticStatus));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "diagnostic_msgs/DiagnosticArray"; };
    const char * getMD5(){ return "3cfbeff055e708a24c3d946a5c8139cd"; };

  };

}
#endif