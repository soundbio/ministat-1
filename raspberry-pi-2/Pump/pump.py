import RPIO.GPIO as GPIO
import threading
import sys
import time

class Pump(object):
    """ Pump class operates a peristaltic pump connected to gpio
        It assumes gpio is compatible with Rpi mode setting.
    """
    __pumpio = None
    __senseio = None
    __run_timer = None
    __steps = 0

    def __init__(self, pumpio, senseio = None):
        self.__pumpio = pumpio
        self.__senseio = senseio
        GPIO.setup(self.__pumpio, GPIO.OUT)

    def start(self):
        GPIO.output(self.__pumpio, GPIO.HIGH)

    def stop(self):
        GPIO.output(self.__pumpio, GPIO.LOW)

    def run_time(self, seconds):
        self.start()
        self.__run_timer = threading.Timer(seconds, self.stop)

    def cancel(self):
        if self.__run_timer != None:
            self.__run_timer.cancel()

    def run_steps(self, steps):
        if sensio == None:
            return # throw?
        
        RPIO.setup(self.__pumpsense, RPIO.IN, pull_up_down=RPIO.PUD_UP)
        if RPIO.input(self.__sensio) == 1:
            self.__steps = steps + 1
        else:
            self.__steps = steps
                      
        RPIO.add_interrupt_callback(self.__pumpsense, self.step_count, edge='rising', threaded_callback=True, debounce_timeout_ms=500)
        
            
