import os, sys
lib_path = os.path.abspath(os.path.join('..', 'Utils'))
sys.path.append(lib_path)
lib_path = os.path.abspath(os.path.join('..', 'Pump'))
sys.path.append(lib_path)

#import RPIO
#import RPi.GPIO as GPIO
#import picamera as picam

from ftplib import FTP
from credentials import Credentials

#from pump import Pump

## use BCM mode to play well with RPIO
#GPIO.setmode(GPIO.BCM)

## start dispatch loop in background
#RPIO.wait_for_interrupts(threaded=True)

## 23 pump drive, 24 pump sensor
#p0 = Pump(23, 24)

creds = Credentials('ptschnack-credentials.txt')

image = 'image.jpg'

with FTP('ftp.ptse.org', creds.user, creds.pwd) as ftp:
    ftp.cwd('/public_ftp')
    ftp.storbinary("STOR " + image, open(image, "rb"), 1024)

