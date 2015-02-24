/* 
 * HiveBio Ministat turbidostat system
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
extern "C" {
#include <project.h>
}
#include "SysTimer.h"
#include "isnprintf.h"


#include <ros.h>
#include "rosserial/rosserial_psoc4/src/ros_lib/examples/HelloWorld/HelloWorld.h"

extern void init(void);

ros::NodeHandle  nh;

int main()
{
    systimer_setup(); // sets up millisec() hardware timer
    Host_UART_Start(); // serial port
    Transimpedance_Start(); // photodiode op amp
    LED_driver_Start(); // LED driver op amp
    //adc_setup(); // A to D converter
    STIR_PWM_Start();
    PUMP_PWM_Start();
    CyGlobalIntEnable; // Enable interrupts
 
    nh.initNode();
    HelloWorld::setup();

    for(;;)
    {
        HelloWorld::loop();
        
        nh.spinOnce();
    }
}

/* [] END OF FILE */
