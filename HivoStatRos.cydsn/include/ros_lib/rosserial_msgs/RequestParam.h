#ifndef _ROS_SERVICE_RequestParam_h
#define _ROS_SERVICE_RequestParam_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rosserial_msgs
{

static const char REQUESTPARAM[] = "rosserial_msgs/RequestParam";

  class RequestParamRequest : public ros::Msg
  {
    public:
      const char* name;

    RequestParamRequest():
      name("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      uint32_t length_name = strlen( (const char*) this->name);
      memcpy(outbuffer + offset, &length_name, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->name, length_name);
      offset += length_name;
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint32_t length_name;
      memcpy(&length_name, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_name; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_name-1]=0;
      this->name = (char *)(inbuffer + offset-1);
      offset += length_name;
#endif
     return offset;
    }

    const char * getType(){ return REQUESTPARAM; };
    const char * getMD5(){ return "c1f3d28f1b044c871e6eff2e9fc3c667"; };

  };

  class RequestParamResponse : public ros::Msg
  {
    public:
      uint8_t ints_length;
      int32_t st_ints;
      int32_t * ints;
      uint8_t floats_length;
      float st_floats;
      float * floats;
      uint8_t strings_length;
      char* st_strings;
      char* * strings;

    RequestParamResponse():
      ints_length(0), ints(NULL),
      floats_length(0), floats(NULL),
      strings_length(0), strings(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = ints_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < ints_length; i++){
      union {
        int32_t real;
        uint32_t base;
      } u_intsi;
      u_intsi.real = this->ints[i];
      *(outbuffer + offset + 0) = (u_intsi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_intsi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_intsi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_intsi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->ints[i]);
      }
      *(outbuffer + offset++) = floats_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < floats_length; i++){
      union {
        float real;
        uint32_t base;
      } u_floatsi;
      u_floatsi.real = this->floats[i];
      *(outbuffer + offset + 0) = (u_floatsi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_floatsi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_floatsi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_floatsi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->floats[i]);
      }
      *(outbuffer + offset++) = strings_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < strings_length; i++){
      uint32_t length_stringsi = strlen( (const char*) this->strings[i]);
      memcpy(outbuffer + offset, &length_stringsi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->strings[i], length_stringsi);
      offset += length_stringsi;
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t ints_lengthT = *(inbuffer + offset++);
      if(ints_lengthT < ints_length)
        ints_length = ints_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < ints_lengthT; i++){
      union {
        int32_t real;
        uint32_t base;
      } u_st_ints;
      u_st_ints.base = 0;
      u_st_ints.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_ints.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_ints.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_ints.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_ints = u_st_ints.real;
      offset += sizeof(this->st_ints);
      if (i < ints_length) {
          memcpy( &(this->ints[i]), &(this->st_ints), sizeof(int32_t));
      }
      }
      uint8_t floats_lengthT = *(inbuffer + offset++);
      if(floats_lengthT < floats_length)
        floats_length = floats_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < floats_lengthT; i++){
      union {
        float real;
        uint32_t base;
      } u_st_floats;
      u_st_floats.base = 0;
      u_st_floats.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_st_floats.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_st_floats.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_st_floats.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->st_floats = u_st_floats.real;
      offset += sizeof(this->st_floats);
      if (i < floats_length) {
          memcpy( &(this->floats[i]), &(this->st_floats), sizeof(float));
      }
      }
      uint8_t strings_lengthT = *(inbuffer + offset++);
      if(strings_lengthT < strings_length)
        strings_length = strings_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < strings_lengthT; i++){
      uint32_t length_st_strings;
      memcpy(&length_st_strings, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_strings; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_strings-1]=0;
      this->st_strings = (char *)(inbuffer + offset-1);
      offset += length_st_strings;
      if (i < strings_length) {
          memcpy( &(this->strings[i]), &(this->st_strings), sizeof(char*));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return REQUESTPARAM; };
    const char * getMD5(){ return "9f0e98bda65981986ddf53afa7a40e49"; };

  };

  class RequestParam {
    public:
    typedef RequestParamRequest Request;
    typedef RequestParamResponse Response;
  };

}
#endif
