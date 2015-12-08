#!/usr/bin/env python

import RPi.GPIO as GPIO

pump_drive_pin = 23
pump_sense_pin = 22
pump_counts = 0
target_counts = 10

def sensor_callback(channel):
	global sensor_phase	
	global pump_counts
	sensor_high = GPIO.input(pump_sense_pin)
	# use pullup/pulldown control to add hysteresis
	if sensor_high:
		GPIO.setup(pump_sense_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)
	else:
		GPIO.setup(pump_sense_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
                pump_counts = pump_counts + 1
                if pump_counts >= target_counts:
                        GPIO.output(pump_drive_pin, False)

def pc():
    print pump_counts

def set_target(val):
    global target_counts
    target_counts = val
    
def step(steps=1):
	print "%d ..."%pump_counts
	set_target(pump_counts+steps)
	GPIO.output(pump_drive_pin, True)

GPIO.setmode(GPIO.BCM)
GPIO.setup(pump_drive_pin, GPIO.OUT)
GPIO.setup(pump_sense_pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)


GPIO.remove_event_detect(pump_sense_pin)
GPIO.add_event_detect(pump_sense_pin, GPIO.BOTH, callback=sensor_callback, bouncetime=2)



