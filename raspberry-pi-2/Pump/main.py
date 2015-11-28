import os, sys
lib_path = os.path.abspath(os.path.join('..', 'utils'))
sys.path.append(lib_path)

import ftplib as FTP
import credentials as cred
import RPIO
import RPi.GPIO as GPIO
from pump import Pump

# use BCM mode to play well with RPIO
GPIO.setmode(GPIO.BCM)

# start dispatch loop in background
RPIO.wait_for_interrupts(threaded=True)

p0 = Pump(23, 24)


