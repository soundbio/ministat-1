from __future__ import print_function

import os, sys
lib_path = os.path.abspath(os.path.join('..', 'utils'))
sys.path.append(lib_path)
lib_path = os.path.abspath(os.path.join('..', 'Camera'))
sys.path.append(lib_path)

import datetime
import subprocess as sp
import RPi.GPIO as GPIO
from pump import Pump

# done() is callback for pump end-of-steps 
def done(timestamp):
    print("done steps " + timestamp, end="\n", flush=True)

try:
    # main loop

    # use BCM mode to play well with RPIO
    GPIO.setmode(GPIO.BCM)
    p0 = Pump(23, 13, done)
    on = False	

    while True:
        ri = raw_input()
        if ri != None:
            if not on:
                p0.start()
            else:
                p0.stop()
            on = not on

except KeyboardInterrupt:
    print("Got ^C. Quitting.",end="\n")

except Exception, ex:
    print(str(ex), end="\n")

finally:
    GPIO.cleanup()

# start dispatch loop in background
# RPIO.wait_for_interrupts(threaded=True)



