import os, sys
lib_path = os.path.abspath(os.path.join('..', 'utils'))
sys.path.append(lib_path)

import subprocess as sp
import RPi.GPIO as GPIO
from pump import Pump
from camera import i

def done(timestamp):
    
    print "done at " + str(timestamp) + "\n"

# use BCM mode to play well with RPIO
GPIO.setmode(GPIO.BCM)
p0 = Pump(23, 13, done)

while True:
    if raw_input() != None:
        print "\n"
        p0.run_steps(1)
    else:
        print "."


# start dispatch loop in background
# RPIO.wait_for_interrupts(threaded=True)



