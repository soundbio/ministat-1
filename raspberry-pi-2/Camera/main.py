from camera import PumpCamera

pc = PumpCamera("ptschnack@ptse.org:~/public_ftp")

pc.capture("img.jpg")
pc.send()

