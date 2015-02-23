#ifndef _ROS_dynamic_reconfigure_ConfigDescription_h
#define _ROS_dynamic_reconfigure_ConfigDescription_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "dynamic_reconfigure/Group.h"
#include "dynamic_reconfigure/Config.h"

namespace dynamic_reconfigure
{

  class ConfigDescription : public ros::Msg
  {
    public:
      uint8_t groups_length;
      dynamic_reconfigure::Group st_groups;
      dynamic_reconfigure::Group * groups;
      dynamic_reconfigure::Config max;
      dynamic_reconfigure::Config min;
      dynamic_reconfigure::Config dflt;

    ConfigDescription():
      groups_length(0), groups(NULL),
      max(),
      min(),
      dflt()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = groups_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < groups_length; i++){
      offset += this->groups[i].serialize(outbuffer + offset);
      }
      offset += this->max.serialize(outbuffer + offset);
      offset += this->min.serialize(outbuffer + offset);
      offset += this->dflt.serialize(outbuffer + offset);
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t groups_lengthT = *(inbuffer + offset++);
      if(groups_lengthT < groups_length)
        groups_length = groups_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < groups_lengthT; i++){
      offset += this->st_groups.deserialize(inbuffer + offset);
      if (i < groups_length) {
          memcpy( &(this->groups[i]), &(this->st_groups), sizeof(dynamic_reconfigure::Group));
      }
      }
      offset += this->max.deserialize(inbuffer + offset);
      offset += this->min.deserialize(inbuffer + offset);
      offset += this->dflt.deserialize(inbuffer + offset);
#endif
     return offset;
    }

    const char * getType(){ return "dynamic_reconfigure/ConfigDescription"; };
    const char * getMD5(){ return "757ce9d44ba8ddd801bb30bc456f946f"; };

  };

}
#endif