#ifndef _ROS_nav_msgs_GetMapResult_h
#define _ROS_nav_msgs_GetMapResult_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "nav_msgs/OccupancyGrid.h"

namespace nav_msgs
{

  class GetMapResult : public ros::Msg
  {
    public:
      nav_msgs::OccupancyGrid map;

    GetMapResult():
      map()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->map.serialize(outbuffer + offset);
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->map.deserialize(inbuffer + offset);
#endif
     return offset;
    }

    const char * getType(){ return "nav_msgs/GetMapResult"; };
    const char * getMD5(){ return "6cdd0a18e0aff5b0a3ca2326a89b54ff"; };

  };

}
#endif