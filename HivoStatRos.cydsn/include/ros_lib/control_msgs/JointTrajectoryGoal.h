#ifndef _ROS_control_msgs_JointTrajectoryGoal_h
#define _ROS_control_msgs_JointTrajectoryGoal_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "trajectory_msgs/JointTrajectory.h"

namespace control_msgs
{

  class JointTrajectoryGoal : public ros::Msg
  {
    public:
      trajectory_msgs::JointTrajectory trajectory;

    JointTrajectoryGoal():
      trajectory()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->trajectory.serialize(outbuffer + offset);
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->trajectory.deserialize(inbuffer + offset);
#endif
     return offset;
    }

    const char * getType(){ return "control_msgs/JointTrajectoryGoal"; };
    const char * getMD5(){ return "2a0eff76c870e8595636c2a562ca298e"; };

  };

}
#endif