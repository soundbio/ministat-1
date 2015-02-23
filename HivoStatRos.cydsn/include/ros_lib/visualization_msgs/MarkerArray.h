#ifndef _ROS_visualization_msgs_MarkerArray_h
#define _ROS_visualization_msgs_MarkerArray_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "visualization_msgs/Marker.h"

namespace visualization_msgs
{

  class MarkerArray : public ros::Msg
  {
    public:
      uint8_t markers_length;
      visualization_msgs::Marker st_markers;
      visualization_msgs::Marker * markers;

    MarkerArray():
      markers_length(0), markers(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = markers_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < markers_length; i++){
      offset += this->markers[i].serialize(outbuffer + offset);
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      uint8_t markers_lengthT = *(inbuffer + offset++);
      if(markers_lengthT < markers_length)
        markers_length = markers_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < markers_lengthT; i++){
      offset += this->st_markers.deserialize(inbuffer + offset);
      if (i < markers_length) {
          memcpy( &(this->markers[i]), &(this->st_markers), sizeof(visualization_msgs::Marker));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "visualization_msgs/MarkerArray"; };
    const char * getMD5(){ return "90da67007c26525f655c1c269094e39f"; };

  };

}
#endif