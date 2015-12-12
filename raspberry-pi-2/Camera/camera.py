from ftplib import FTP
import picamera as picam

image = "image.jpg"

camera = picam.PiCamera()
camera.capture(image)

ftp = FTP('ftp.ptse.org','ptschnack','Copperdoor54!')
ftp.cwd('/public_ftp')
ftp.storbinary("STOR " + image, open(image, "rb"), 1024)
ftp.close()

