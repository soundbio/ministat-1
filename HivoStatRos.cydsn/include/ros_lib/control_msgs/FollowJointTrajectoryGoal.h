#ifndef _ROS_control_msgs_FollowJointTrajectoryGoal_h
#define _ROS_control_msgs_FollowJointTrajectoryGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "trajectory_msgs/JointTrajectory.h"
#include "control_msgs/JointTolerance.h"
#include "ros/duration.h"

namespace control_msgs
{

  class FollowJointTrajectoryGoal : public ros::Msg
  {
    public:
      trajectory_msgs::JointTrajectory trajectory;
      uint8_t path_tolerance_length;
      control_msgs::JointTolerance st_path_tolerance;
      control_msgs::JointTolerance * path_tolerance;
      uint8_t goal_tolerance_length;
      control_msgs::JointTolerance st_goal_tolerance;
      control_msgs::JointTolerance * goal_tolerance;
      ros::Duration goal_time_tolerance;

    FollowJointTrajectoryGoal():
      trajectory(),
      path_tolerance_length(0), path_tolerance(NULL),
      goal_tolerance_length(0), goal_tolerance(NULL),
      goal_time_tolerance()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->trajectory.serialize(outbuffer + offset);
      *(outbuffer + offset++) = path_tolerance_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < path_tolerance_length; i++){
      offset += this->path_tolerance[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset++) = goal_tolerance_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < goal_tolerance_length; i++){
      offset += this->goal_tolerance[i].serialize(outbuffer + offset);
      }
      *(outbuffer + offset + 0) = (this->goal_time_tolerance.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->goal_time_tolerance.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->goal_time_tolerance.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->goal_time_tolerance.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->goal_time_tolerance.sec);
      *(outbuffer + offset + 0) = (this->goal_time_tolerance.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->goal_time_tolerance.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->goal_time_tolerance.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->goal_time_tolerance.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->goal_time_tolerance.nsec);
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->trajectory.deserialize(inbuffer + offset);
      uint8_t path_tolerance_lengthT = *(inbuffer + offset++);
      if(path_tolerance_lengthT < path_tolerance_length)
        path_tolerance_length = path_tolerance_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < path_tolerance_lengthT; i++){
      offset += this->st_path_tolerance.deserialize(inbuffer + offset);
      if (i < path_tolerance_length) {
          memcpy( &(this->path_tolerance[i]), &(this->st_path_tolerance), sizeof(control_msgs::JointTolerance));
      }
      }
      uint8_t goal_tolerance_lengthT = *(inbuffer + offset++);
      if(goal_tolerance_lengthT < goal_tolerance_length)
        goal_tolerance_length = goal_tolerance_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < goal_tolerance_lengthT; i++){
      offset += this->st_goal_tolerance.deserialize(inbuffer + offset);
      if (i < goal_tolerance_length) {
          memcpy( &(this->goal_tolerance[i]), &(this->st_goal_tolerance), sizeof(control_msgs::JointTolerance));
      }
      }
      this->goal_time_tolerance.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->goal_time_tolerance.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->goal_time_tolerance.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->goal_time_tolerance.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->goal_time_tolerance.sec);
      this->goal_time_tolerance.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->goal_time_tolerance.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->goal_time_tolerance.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->goal_time_tolerance.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->goal_time_tolerance.nsec);
#endif
     return offset;
    }

    const char * getType(){ return "control_msgs/FollowJointTrajectoryGoal"; };
    const char * getMD5(){ return "69636787b6ecbde4d61d711979bc7ecb"; };

  };

}
#endif