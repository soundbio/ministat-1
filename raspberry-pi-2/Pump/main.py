from __future__ import print_function

import os, sys
lib_path = os.path.abspath(os.path.join('..', 'utils'))
sys.path.append(lib_path)
lib_path = os.path.abspath(os.path.join('..', 'Camera'))
sys.path.append(lib_path)

import datetime
import time
import subprocess as sp
import RPi.GPIO as GPIO
from pump import Pump
from camera import PumpCamera 

# done() is callback for pump end-of-steps 
def done(timestamp):
    global pc
    now = datetime.datetime.now().strftime("%Y%m%d-%H%M%S.%f")
    now = now + ".jpg"
    pc.capture(now)
    pc.send()
    os.remove(now)

#---------------------------------
# Here's the start of the program
#---------------------------------

pc = PumpCamera("ptschnack@ptse.org:~/public_ftp")

try:
    # main loop

    # use BCM mode to play well with RPIO
    GPIO.setmode(GPIO.BCM)
    p0 = Pump(23, 13, done)

    if True:
        # ri = raw_input()
        # if ri != None:
        p0.run_steps(6)
        time.sleep(30)

except KeyboardInterrupt:
    print("Got ^C. Quitting.",end="\n")

except Exception, ex:
    print(str(ex), end="\n")

finally:
    GPIO.cleanup()

