import RPi.GPIO as GPIO
import threading
import sys
import time

class Pump(object):
    """ Pump class operates a peristaltic pump connected to gpio
        It assumes gpio is compatible with Rpi mode setting.
    """
    __firstcall = 0 
    __lastcall = 0
    __pumpio = None
    __senseio = None
    __run_timer = None
    __steps = 0
    __debounce = 20 # ms

    def __init__(self, pumpio, senseio = None, callback = None):
        self.__firstcall = 0
        self.__lastcall = 0
        self.__pumpio = pumpio
        self.__senseio = senseio
        self.__callback = callback
        GPIO.setup(self.__pumpio, GPIO.OUT, pull_up_down=GPIO.PUD_OFF)
        if (not senseio == None) and (not callback == None):
            GPIO.setup(self.__senseio, GPIO.IN, pull_up_down=GPIO.PUD_OFF)
            GPIO.remove_event_detect(self.__senseio)
            GPIO.add_event_detect(self.__senseio, GPIO.RISING, self.__callback)

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

        self.stop() #just in case we were running ...

    def run_steps(self, steps):
        if (self.__senseio == None) and (steps <= 0):
            return # throw?
        
        GPIO.setup(self.__senseio, GPIO.IN, pull_up_down=GPIO.PUD_OFF)
        GPIO.remove_event_detect(self.__senseio)
        GPIO.add_event_detect(self.__senseio, GPIO.RISING, callback=self._listener)
        
        # set up for steps
        self.__steps = steps
        self.__lastcall = int(round(time.time() * 1000))
        self.__firstcall = self.__lastcall

        self.start()

    def _listener(self, channel):
        # process steps and stop if done
        self.__steps = self.__steps - 1
        if self.__steps == 0:
            self.stop()

        if not self.__callback is None:
            self.__callback(self.__lastcall)


