#ifndef _ROS_trajectory_msgs_JointTrajectoryPoint_h
#define _ROS_trajectory_msgs_JointTrajectoryPoint_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/duration.h"

namespace trajectory_msgs
{

  class JointTrajectoryPoint : public ros::Msg
  {
    public:
      uint8_t positions_length;
      float st_positions;
      float * positions;
      uint8_t velocities_length;
      float st_velocities;
      float * velocities;
      uint8_t accelerations_length;
      float st_accelerations;
      float * accelerations;
      uint8_t effort_length;
      float st_effort;
      float * effort;
      ros::Duration time_from_start;

    JointTrajectoryPoint():
      positions_length(0), positions(NULL),
      velocities_length(0), velocities(NULL),
      accelerations_length(0), accelerations(NULL),
      effort_length(0), effort(NULL),
      time_from_start()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      *(outbuffer + offset++) = positions_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < positions_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->positions[i]);
      }
      *(outbuffer + offset++) = velocities_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < velocities_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->velocities[i]);
      }
      *(outbuffer + offset++) = accelerations_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < accelerations_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->accelerations[i]);
      }
      *(outbuffer + offset++) = effort_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < effort_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->effort[i]);
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
      uint8_t positions_lengthT = *(inbuffer + offset++);
      if(positions_lengthT < positions_length)
        positions_length = positions_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < positions_lengthT; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_positions));
      if (i < positions_length) {
          memcpy( &(this->positions[i]), &(this->st_positions), sizeof(float));
      }
      }
      uint8_t velocities_lengthT = *(inbuffer + offset++);
      if(velocities_lengthT < velocities_length)
        velocities_length = velocities_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < velocities_lengthT; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_velocities));
      if (i < velocities_length) {
          memcpy( &(this->velocities[i]), &(this->st_velocities), sizeof(float));
      }
      }
      uint8_t accelerations_lengthT = *(inbuffer + offset++);
      if(accelerations_lengthT < accelerations_length)
        accelerations_length = accelerations_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < accelerations_lengthT; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_accelerations));
      if (i < accelerations_length) {
          memcpy( &(this->accelerations[i]), &(this->st_accelerations), sizeof(float));
      }
      }
      uint8_t effort_lengthT = *(inbuffer + offset++);
      if(effort_lengthT < effort_length)
        effort_length = effort_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < effort_lengthT; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_effort));
      if (i < effort_length) {
          memcpy( &(this->effort[i]), &(this->st_effort), sizeof(float));
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

    const char * getType(){ return "trajectory_msgs/JointTrajectoryPoint"; };
    const char * getMD5(){ return "f3cd1e1c4d320c79d6985c904ae5dcd3"; };

  };

}
#endif