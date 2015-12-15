from picamera import PiCamera
import subprocess as sp

class PumpCamera(PiCamera):
    __image = None
    __path = None

    def __init__(self, path):
        self.__path = path
        PiCamera.__init__(self)

    def capture(self, image):
        self.__image = image
        PiCamera.capture(self, image)

    def send(self):
        try:
            sp.Popen(['scp', self.__image, self.__path + '/' + self.__image]).wait()
        except Exception, ex:
            print str(ex)
