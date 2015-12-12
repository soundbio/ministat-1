import RPi.GPIO as GPIO
import RPIO
from pump import Pump

GPIO.setmode(GPIO.BOARD)

p0 = Pump(23)
po.start()

