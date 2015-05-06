/* 
 * HiveBio Ministat turbidostat system
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
extern "C" {
#include <project.h>
#include "ADC_proc.h"
}
#include "SysTimer.h"
#include "isnprintf.h"
#include "std_msgs/Int32.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/Float32.h"


#include <ros.h>
#include "rosserial/rosserial_psoc4/src/ros_lib/examples/HelloWorld/HelloWorld.h"

extern void init(void);

ros::NodeHandle  nh;


static enum { LED_on, collect_on, LED_off, collect_off } detector_phase = LED_on;


std_msgs::Int32 int32_msg;
ros::Publisher photoval_pub("photoval", &int32_msg);
ros::Publisher darkval_pub("darkval", &int32_msg);
ros::Publisher lightval_pub("lightval", &int32_msg);
ros::Publisher photoval_raw_pub("photoval_raw", &int32_msg);
ros::Publisher ledvoltval_pub("ledvoltval", &int32_msg);
ros::Publisher thermistor_pub("thermistor", &int32_msg);

void stir_speed_cb( const std_msgs::UInt16& cmd_msg){
    // Set stir motor pwm level
    STIR_PWM_WriteCompare(cmd_msg.data & 0x3FF);
}

ros::Subscriber<std_msgs::UInt16> stir_speed_sub("stir_speed", stir_speed_cb);

// pump motor control
uint16 pump_run_pwm = 0;
uint16 pump_steps_target = 0;

void pump_speed_cb( const std_msgs::UInt16& cmd_msg){
    // Set pump motor pwm level
    pump_run_pwm = cmd_msg.data & 0x3FF;
}

ros::Subscriber<std_msgs::UInt16> pump_speed_sub("pump_speed", pump_speed_cb);

void pump_steps_cb( const std_msgs::UInt16& cmd_msg){
    // run pump for n steps (6 steps per rev)
    pump_steps_target = adc_pump_state.count + cmd_msg.data;
    PUMP_PWM_WriteCompare(pump_run_pwm);
}

ros::Subscriber<std_msgs::UInt16> pump_steps_sub("pump_steps", pump_steps_cb);


// heater control
uint16 heater_pwm = 0;

void heater_pct_cb( const std_msgs::UInt16& cmd_msg){
    // Set heater pwm level by percent
    if ((heater_pwm = cmd_msg.data*10)>1000) {
        heater_pwm = 1000;
    }
    HEATER_PWM_WriteCompare(heater_pwm);
}

ros::Subscriber<std_msgs::UInt16> heater_pct_sub("heater_pct", heater_pct_cb);

// LED thermal drift correction
#define NOM_LEDVOLT 2970000L
#define OFFSET_LEDVOLT (1./3. * 32768 * 100) // LED cathode is at Vref
#define LED_CORR_FACTOR (6.2)  // % change in light per 1% change in LED voltage
int32 nom_ledvolt = NOM_LEDVOLT;
float led_corr_factor = LED_CORR_FACTOR;

void nom_ledvolt_cb( const std_msgs::Int32& msg){
    // Set nominal led voltage for thermal drift correction
    nom_ledvolt = msg.data;
}

ros::Subscriber<std_msgs::Int32> nom_ledvolt_sub("nom_ledvolt", nom_ledvolt_cb);

void led_corr_factor_cb( const std_msgs::Float32& msg){
    // Set nominal correction coeff for thermal drift correction
    led_corr_factor = msg.data;
}

ros::Subscriber<std_msgs::Float32> led_corr_factor_sub("led_corr_factor", led_corr_factor_cb);



int main()
{
    systimer_setup(); // sets up millisec() hardware timer
    Host_UART_Start(); // serial port
    Transimpedance_Start(); // photodiode op amp
    LED_driver_Start(); // LED driver op amp
    adc_setup(); // A to D converter
    STIR_PWM_Start();
    PUMP_PWM_Start();
    HEATER_PWM_Start();
    CyGlobalIntEnable; // Enable interrupts
    
    uint32 report_interval_ms = 100;
    uint32 next_report_time = millis() + report_interval_ms;
    uint32 toggle_interval_ms = 20;
    uint32 next_toggle_time = millis() + toggle_interval_ms;
    uint32 reboot_timer = millis();
    uint8 reboot_count =0;
    uint32 photo_bg = 0, photo_fg = 0;
    uint32 led_volt_bg = 0, led_volt_fg = 0;

    nh.initNode();
    HelloWorld::setup();
    nh.advertise(photoval_pub);
    nh.advertise(darkval_pub);
    nh.advertise(lightval_pub);
    nh.advertise(photoval_raw_pub);
    nh.advertise(ledvoltval_pub);
    nh.advertise(thermistor_pub);
    nh.subscribe(stir_speed_sub);
    nh.subscribe(pump_speed_sub);
    nh.subscribe(pump_steps_sub);
    nh.subscribe(heater_pct_sub);
    nh.subscribe(nom_ledvolt_sub);
    nh.subscribe(led_corr_factor_sub);


    for(;;)
    {
        HelloWorld::loop();
 
        // alternate foreground (LED on) and backgroung (LED off) measurement
        if ((int32)(millis()-next_toggle_time) >= 0) {
            next_toggle_time += toggle_interval_ms;
            if (detector_phase == LED_on) {
                detector_phase = collect_on;
            } else if (detector_phase == collect_on) {
                photo_fg = adc_result[adc_chan_photosensor]; // photodiode signal
                led_volt_fg = adc_result[adc_chan_led_voltage]; // LED anode voltage (ref to gnd)
                LED_driver_Stop();
                detector_phase = LED_off;
            } else if (detector_phase == LED_off) {
                detector_phase = collect_off;
            } else {
                photo_bg = adc_result[adc_chan_photosensor];
                led_volt_bg = adc_result[adc_chan_led_voltage];
                LED_driver_Enable();
                detector_phase = LED_on;
            }
        }
        
        // periodically report results 
        if ((int32)(millis()-next_report_time) >= 0) {
            next_report_time += report_interval_ms;
            int32_msg.data = photo_bg;
            darkval_pub.publish(int32_msg);
            int32_msg.data = photo_fg;
            lightval_pub.publish(int32_msg);
            // take (foreground - background)
            uint32 photoval = photo_fg - photo_bg;
            int32_msg.data = photoval;
            photoval_raw_pub.publish(int32_msg);
            uint32 ledvoltval = led_volt_fg - led_volt_bg;
            int32_msg.data = ledvoltval;
            ledvoltval_pub.publish(int32_msg);
            // apply LED thermal drift correction
            float delta_ledvolt = (int32)(ledvoltval - nom_ledvolt)/(nom_ledvolt - OFFSET_LEDVOLT);
            uint32 adjusted_photoval = photoval / (1.0 + delta_ledvolt*led_corr_factor);
            int32_msg.data = adjusted_photoval;
            photoval_pub.publish(int32_msg);
            int32_msg.data = adc_result[adc_chan_thermistor];
            thermistor_pub.publish(int32_msg);

        }
        
        if((int16)(adc_pump_state.count-pump_steps_target)>=0) {
            PUMP_PWM_WriteCompare(0);
        }

        // watch for pushbutton
        //   Held down 1-2 sec = reboot request
        if ((int32)(millis()-reboot_timer) >= 0) {
            if(Button_Read()) {
                reboot_count = 0;
            } else {
                if(++reboot_count > 1) {
                    Bootloadable_Load();
                }
            }
            reboot_timer += 1000;
        }

        nh.spinOnce();
    }
}

/* [] END OF FILE */
