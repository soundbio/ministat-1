#ifndef _ROS_smach_msgs_SmachContainerStructure_h
#define _ROS_smach_msgs_SmachContainerStructure_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace smach_msgs
{

  class SmachContainerStructure : public ros::Msg
  {
    public:
      std_msgs::Header header;
      const char* path;
      uint8_t children_length;
      char* st_children;
      char* * children;
      uint8_t internal_outcomes_length;
      char* st_internal_outcomes;
      char* * internal_outcomes;
      uint8_t outcomes_from_length;
      char* st_outcomes_from;
      char* * outcomes_from;
      uint8_t outcomes_to_length;
      char* st_outcomes_to;
      char* * outcomes_to;
      uint8_t container_outcomes_length;
      char* st_container_outcomes;
      char* * container_outcomes;

    SmachContainerStructure():
      header(),
      path(""),
      children_length(0), children(NULL),
      internal_outcomes_length(0), internal_outcomes(NULL),
      outcomes_from_length(0), outcomes_from(NULL),
      outcomes_to_length(0), outcomes_to(NULL),
      container_outcomes_length(0), container_outcomes(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_SERIALIZE
      offset += this->header.serialize(outbuffer + offset);
      uint32_t length_path = strlen( (const char*) this->path);
      memcpy(outbuffer + offset, &length_path, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->path, length_path);
      offset += length_path;
      *(outbuffer + offset++) = children_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < children_length; i++){
      uint32_t length_childreni = strlen( (const char*) this->children[i]);
      memcpy(outbuffer + offset, &length_childreni, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->children[i], length_childreni);
      offset += length_childreni;
      }
      *(outbuffer + offset++) = internal_outcomes_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < internal_outcomes_length; i++){
      uint32_t length_internal_outcomesi = strlen( (const char*) this->internal_outcomes[i]);
      memcpy(outbuffer + offset, &length_internal_outcomesi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->internal_outcomes[i], length_internal_outcomesi);
      offset += length_internal_outcomesi;
      }
      *(outbuffer + offset++) = outcomes_from_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < outcomes_from_length; i++){
      uint32_t length_outcomes_fromi = strlen( (const char*) this->outcomes_from[i]);
      memcpy(outbuffer + offset, &length_outcomes_fromi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->outcomes_from[i], length_outcomes_fromi);
      offset += length_outcomes_fromi;
      }
      *(outbuffer + offset++) = outcomes_to_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < outcomes_to_length; i++){
      uint32_t length_outcomes_toi = strlen( (const char*) this->outcomes_to[i]);
      memcpy(outbuffer + offset, &length_outcomes_toi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->outcomes_to[i], length_outcomes_toi);
      offset += length_outcomes_toi;
      }
      *(outbuffer + offset++) = container_outcomes_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < container_outcomes_length; i++){
      uint32_t length_container_outcomesi = strlen( (const char*) this->container_outcomes[i]);
      memcpy(outbuffer + offset, &length_container_outcomesi, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->container_outcomes[i], length_container_outcomesi);
      offset += length_container_outcomesi;
      }
#endif
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
#ifndef ROS_MSG_DONT_DESERIALIZE
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t length_path;
      memcpy(&length_path, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_path; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_path-1]=0;
      this->path = (char *)(inbuffer + offset-1);
      offset += length_path;
      uint8_t children_lengthT = *(inbuffer + offset++);
      if(children_lengthT < children_length)
        children_length = children_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < children_lengthT; i++){
      uint32_t length_st_children;
      memcpy(&length_st_children, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_children; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_children-1]=0;
      this->st_children = (char *)(inbuffer + offset-1);
      offset += length_st_children;
      if (i < children_length) {
          memcpy( &(this->children[i]), &(this->st_children), sizeof(char*));
      }
      }
      uint8_t internal_outcomes_lengthT = *(inbuffer + offset++);
      if(internal_outcomes_lengthT < internal_outcomes_length)
        internal_outcomes_length = internal_outcomes_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < internal_outcomes_lengthT; i++){
      uint32_t length_st_internal_outcomes;
      memcpy(&length_st_internal_outcomes, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_internal_outcomes; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_internal_outcomes-1]=0;
      this->st_internal_outcomes = (char *)(inbuffer + offset-1);
      offset += length_st_internal_outcomes;
      if (i < internal_outcomes_length) {
          memcpy( &(this->internal_outcomes[i]), &(this->st_internal_outcomes), sizeof(char*));
      }
      }
      uint8_t outcomes_from_lengthT = *(inbuffer + offset++);
      if(outcomes_from_lengthT < outcomes_from_length)
        outcomes_from_length = outcomes_from_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < outcomes_from_lengthT; i++){
      uint32_t length_st_outcomes_from;
      memcpy(&length_st_outcomes_from, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_outcomes_from; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_outcomes_from-1]=0;
      this->st_outcomes_from = (char *)(inbuffer + offset-1);
      offset += length_st_outcomes_from;
      if (i < outcomes_from_length) {
          memcpy( &(this->outcomes_from[i]), &(this->st_outcomes_from), sizeof(char*));
      }
      }
      uint8_t outcomes_to_lengthT = *(inbuffer + offset++);
      if(outcomes_to_lengthT < outcomes_to_length)
        outcomes_to_length = outcomes_to_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < outcomes_to_lengthT; i++){
      uint32_t length_st_outcomes_to;
      memcpy(&length_st_outcomes_to, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_outcomes_to; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_outcomes_to-1]=0;
      this->st_outcomes_to = (char *)(inbuffer + offset-1);
      offset += length_st_outcomes_to;
      if (i < outcomes_to_length) {
          memcpy( &(this->outcomes_to[i]), &(this->st_outcomes_to), sizeof(char*));
      }
      }
      uint8_t container_outcomes_lengthT = *(inbuffer + offset++);
      if(container_outcomes_lengthT < container_outcomes_length)
        container_outcomes_length = container_outcomes_lengthT;
      offset += 3;
      for( uint8_t i = 0; i < container_outcomes_lengthT; i++){
      uint32_t length_st_container_outcomes;
      memcpy(&length_st_container_outcomes, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_st_container_outcomes; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_st_container_outcomes-1]=0;
      this->st_container_outcomes = (char *)(inbuffer + offset-1);
      offset += length_st_container_outcomes;
      if (i < container_outcomes_length) {
          memcpy( &(this->container_outcomes[i]), &(this->st_container_outcomes), sizeof(char*));
      }
      }
#endif
     return offset;
    }

    const char * getType(){ return "smach_msgs/SmachContainerStructure"; };
    const char * getMD5(){ return "3d3d1e0d0f99779ee9e58101a5dcf7ea"; };

  };

}
#endif