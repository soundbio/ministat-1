/* 
 * rosserial Encoder example for PSoC4 "RosOnAStick"
 * 
 * reads a quadrature optical encoder using PSoC hardware counter
 *
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Int16.h>
#include "Encoder.h"

extern ros::NodeHandle nh;

namespace Encoder {

std_msgs::Int32 encoder_msg;
ros::Publisher p("encoder", &encoder_msg);
std_msgs::Int16 counter_msg;
ros::Publisher pc("counter", &counter_msg);

uint32_t next_report_time;
const uint32_t kReportIntervalMs = 200;
uint32_t next_read_time;
const uint32_t kReadIntervalMs = 10; // fast enough to avoid 0x4000 counts in one interval
int16_t prev_counter_val;

void setup()
{ 
  nh.advertise(p);
  nh.advertise(pc);
  next_report_time = SysTimer::millis();
  next_read_time = SysTimer::millis();
  QuadDec_1_Start();
  Quad_Clock_Start();
  QuadDec_1_TriggerCommand(QuadDec_1_MASK, QuadDec_1_CMD_RELOAD);
  prev_counter_val = 0; // reload sets value to 0x8000
  encoder_msg.data = 0;
}

void read_counter() {
    // PSoC4 16-bit hardware counter rolls over in a special way; ignore that and use lower 15 bits
    int16_t counter_val = 0x7FFF & QuadDec_1_ReadCounter();
    counter_msg.data = counter_val;
    int32_t delta = counter_val - prev_counter_val;
    if (delta > 0x4000) { delta -= 0x8000; }
    else if (delta < -0x4000) { delta += 0x8000; }
    encoder_msg.data += delta;
    prev_counter_val = counter_val;
}
void loop()
{

  if ((int32_t)(SysTimer::millis()-next_read_time) > 0) {
    next_read_time += kReadIntervalMs;
    read_counter();
    pc.publish(&counter_msg);
  }
  if ((int32_t)(SysTimer::millis()-next_report_time) > 0) {
    next_report_time += kReportIntervalMs;
    read_counter();
    p.publish(&encoder_msg);
  }
}

} // namespace Encoder
