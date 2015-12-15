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
from camera import PumpCamera 

# done() is callback for pump end-of-steps 
def done(timestamp):
    print("done steps " + timestamp, end="\n", flush=True)

try:
    # main loop

    # use BCM mode to play well with RPIO
    GPIO.setmode(GPIO.BCM)
    p0 = Pump(23, 13, done)
    pc = PumpCamera("ptschnack@ptse.org:~/public_ftp")

    while True:
        ri = raw_input()
        if ri == None: print("ri None",end="\n")
        if ri != None:
            print("raw in=" + ri, end="\n")
            p0.run_steps(1)
            now = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
            now = now + ".jpg"
            pc.capture(now)
            pc.send()
            print(now, end="\n")
        else:
            print(".", end="")

except KeyboardInterrupt:
    print("Got ^C. Quitting.",end="\n")

except Exception, ex:
    print(str(ex), end="\n")

finally:
    GPIO.cleanup()

# start dispatch loop in background
# RPIO.wait_for_interrupts(threaded=True)



