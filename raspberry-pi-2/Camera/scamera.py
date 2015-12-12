import subprocess as sp
import picamera as picam

image = "image.jpg"

camera = picam.PiCamera()
camera.capture(image)

sp.Popen(['scp',image, 'ptschnack@ptse.org:/public_ftp/'+image]).wait()

