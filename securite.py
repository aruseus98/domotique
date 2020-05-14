#!/usr/bin/env python
# -*- coding: utf8 -*-

import serial
import subprocess
import sys
ser = serial.Serial('/dev/ttyACM0', 9600)
#subprocess.call("python3 capteur.py", shell=True)
while 1 :
  	x=ser.readline()
	print x
	if "Prêt" in x :
		subprocess.call("./connect.sh")
		sys.exit(0)
	elif "Désactivé" in x :
		sys.exit(0)
	elif "Code" in x :
		message=input("Saisir code :")
		ser.write(message.encode('utf-8'))
