#ifndef _ROS_SERVICE_ServiceResponseDetails_h
#define _ROS_SERVICE_ServiceResponseDetails_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "rosapi/TypeDef.h"

namespace rosapi
{

static const char SERVICERESPONSEDETAILS[] = "rosapi/ServiceResponseDetails";

  class ServiceResponseDetailsRequest : public ros::Msg
  {
    public:
      const char* type;

    ServiceResponseDetailsRequest():
      type("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      uint32_t length_type = strlen( (const char*) this->type);
      memcpy(outbuffer + offset, &length_type, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->type, length_type);
      offset += length_type;
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint32_t length_type;
      memcpy(&length_type, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_type; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_type-1]=0;
      this->type = (char *)(inbuffer + offset-1);
      offset += length_type;
#endif
     return offset;
    }

    const char * getType(){ return SERVICERESPONSEDETAILS; };
    const char * getMD5(){ return "dc67331de85cf97091b7d45e5c64ab75"; };

  };

  class ServiceResponseDetailsResponse : public ros::Msg
  {
    public:
      uint8_t typedefs_length;
      rosapi::TypeDef st_typedefs;
      rosapi::TypeDef * typedefs;

    ServiceResponseDetailsResponse():
      typedefs_length(0), typedefs(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = typedefs_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < typedefs_length; i++){
      offset += this->typedefs[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t typedefs_lengthT = *(inbuffer + offset++);
      if(typedefs_lengthT < typedefs_length)
        typedefs_length = typedefs_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < typedefs_lengthT; i++){
      offset += this->st_typedefs.deserialize(inbuffer + offset);
      if (i < typedefs_length) {
          memcpy( &(this->typedefs[i]), &(this->st_typedefs), sizeof(rosapi::TypeDef));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return SERVICERESPONSEDETAILS; };
    const char * getMD5(){ return "d088db0da260a2cde072246a5f577519"; };

  };

  class ServiceResponseDetails {
    public:
    typedef ServiceResponseDetailsRequest Request;
    typedef ServiceResponseDetailsResponse Response;
  };

}
#endif
