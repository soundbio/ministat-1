#ifndef _ROS_trajectory_msgs_MultiDOFJointTrajectoryPoint_h
#define _ROS_trajectory_msgs_MultiDOFJointTrajectoryPoint_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Transform.h"
#include "geometry_msgs/Twist.h"
#include "ros/duration.h"

namespace trajectory_msgs
{

  class MultiDOFJointTrajectoryPoint : public ros::Msg
  {
    public:
      uint8_t transforms_length;
      geometry_msgs::Transform st_transforms;
      geometry_msgs::Transform * transforms;
      uint8_t velocities_length;
      geometry_msgs::Twist st_velocities;
      geometry_msgs::Twist * velocities;
      uint8_t accelerations_length;
      geometry_msgs::Twist st_accelerations;
      geometry_msgs::Twist * accelerations;
      ros::Duration time_from_start;

    MultiDOFJointTrajectoryPoint():
      transforms_length(0), transforms(NULL),
      velocities_length(0), velocities(NULL),
      accelerations_length(0), accelerations(NULL),
      time_from_start()
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
      *(outbuffer + offset++) = velocities_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < velocities_length; i++){
      offset += this->velocities[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset++) = accelerations_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < accelerations_length; i++){
      offset += this->accelerations[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->time_from_start.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_from_start.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_from_start.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_from_start.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_from_start.sec);
      *(outbuffer + offset + 0) = (this->time_from_start.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->time_from_start.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->time_from_start.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->time_from_start.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->time_from_start.nsec);
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
          memcpy( &(this->transforms[i]), &(this->st_transforms), sizeof(geometry_msgs::Transform));
      }
      }
      uint8_t velocities_lengthT = *(inbuffer + offset++);
      if(velocities_lengthT < velocities_length)
        velocities_length = velocities_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < velocities_lengthT; i++){
      offset += this->st_velocities.deserialize(inbuffer + offset);
      if (i < velocities_length) {
          memcpy( &(this->velocities[i]), &(this->st_velocities), sizeof(geometry_msgs::Twist));
      }
      }
      uint8_t accelerations_lengthT = *(inbuffer + offset++);
      if(accelerations_lengthT < accelerations_length)
        accelerations_length = accelerations_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < accelerations_lengthT; i++){
      offset += this->st_accelerations.deserialize(inbuffer + offset);
      if (i < accelerations_length) {
          memcpy( &(this->accelerations[i]), &(this->st_accelerations), sizeof(geometry_msgs::Twist));
      }
      }
      this->time_from_start.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->time_from_start.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_from_start.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_from_start.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_from_start.sec);
      this->time_from_start.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->time_from_start.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->time_from_start.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->time_from_start.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->time_from_start.nsec);
#endif
     return offset;
    }

    const char * getType(){ return "trajectory_msgs/MultiDOFJointTrajectoryPoint"; };
    const char * getMD5(){ return "3ebe08d1abd5b65862d50e09430db776"; };

  };

}
#endif