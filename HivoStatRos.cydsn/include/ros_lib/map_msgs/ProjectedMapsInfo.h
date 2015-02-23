#ifndef _ROS_SERVICE_ProjectedMapsInfo_h
#define _ROS_SERVICE_ProjectedMapsInfo_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "map_msgs/ProjectedMapInfo.h"

namespace map_msgs
{

static const char PROJECTEDMAPSINFO[] = "map_msgs/ProjectedMapsInfo";

  class ProjectedMapsInfoRequest : public ros::Msg
  {
    public:
      uint8_t projected_maps_info_length;
      map_msgs::ProjectedMapInfo st_projected_maps_info;
      map_msgs::ProjectedMapInfo * projected_maps_info;

    ProjectedMapsInfoRequest():
      projected_maps_info_length(0), projected_maps_info(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = projected_maps_info_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < projected_maps_info_length; i++){
      offset += this->projected_maps_info[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t projected_maps_info_lengthT = *(inbuffer + offset++);
      if(projected_maps_info_lengthT < projected_maps_info_length)
        projected_maps_info_length = projected_maps_info_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < projected_maps_info_lengthT; i++){
      offset += this->st_projected_maps_info.deserialize(inbuffer + offset);
      if (i < projected_maps_info_length) {
          memcpy( &(this->projected_maps_info[i]), &(this->st_projected_maps_info), sizeof(map_msgs::ProjectedMapInfo));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return PROJECTEDMAPSINFO; };
    const char * getMD5(){ return "d7980a33202421c8cd74565e57a4d229"; };

  };

  class ProjectedMapsInfoResponse : public ros::Msg
  {
    public:

    ProjectedMapsInfoResponse()
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

    const char * getType(){ return PROJECTEDMAPSINFO; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class ProjectedMapsInfo {
    public:
    typedef ProjectedMapsInfoRequest Request;
    typedef ProjectedMapsInfoResponse Response;
  };

}
#endif
