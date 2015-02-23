#ifndef _ROS_SERVICE_GetLoggers_h
#define _ROS_SERVICE_GetLoggers_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "roscpp/Logger.h"

namespace roscpp
{

static const char GETLOGGERS[] = "roscpp/GetLoggers";

  class GetLoggersRequest : public ros::Msg
  {
    public:

    GetLoggersRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
#endif
     return offset;
    }

    const char * getType(){ return GETLOGGERS; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class GetLoggersResponse : public ros::Msg
  {
    public:
      uint8_t loggers_length;
      roscpp::Logger st_loggers;
      roscpp::Logger * loggers;

    GetLoggersResponse():
      loggers_length(0), loggers(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = loggers_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < loggers_length; i++){
      offset += this->loggers[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t loggers_lengthT = *(inbuffer + offset++);
      if(loggers_lengthT < loggers_length)
        loggers_length = loggers_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < loggers_lengthT; i++){
      offset += this->st_loggers.deserialize(inbuffer + offset);
      if (i < loggers_length) {
          memcpy( &(this->loggers[i]), &(this->st_loggers), sizeof(roscpp::Logger));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return GETLOGGERS; };
    const char * getMD5(){ return "32e97e85527d4678a8f9279894bb64b0"; };

  };

  class GetLoggers {
    public:
    typedef GetLoggersRequest Request;
    typedef GetLoggersResponse Response;
  };

}
#endif
