#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Detecter des mouvements avec un capteur infrarouge
#
#
# Importation des bibliothèques python
import RPi.GPIO as GPIO
import time
import urllib
import os
import sys
import mysql.connector
 
# Utiliser les BCM GPIO et pas les numeros des pins sur P1
GPIO.setmode(GPIO.BCM)
 
# Pin GPIO utilisee
GPIO_PIR = 7


conn = mysql.connector.connect(host="192.168.0.250", user="Alex", password="Admin327", database="Projet")
cursor = conn.cursor()
cursor.execute("""SELECT activer_capteur FROM Securite WHERE id = 1""")
rows = cursor.fetchone()

if "On" in rows :

	print("Capteur de mouvement enclenché")
 
	# Configurer la pin 7 en entree
	GPIO.setup(GPIO_PIR,GPIO.IN)
	# Initialisation des variables
	Current_State  = 0
	Previous_State = 0
	try:
		if "On" in rows :
			print("Attente detection...")
		
		# Boucle jusqu a ce que la sortie du capteur passe a 0
			while GPIO.input(GPIO_PIR)==0:
				Current_State = 0
				print("Pret")
	 
				# Tourne en boucle jusqu'a ce que l utilisateur quitte (par CTRL+C)
				while True :
					# Lire l'etat du capteur
					Current_State = GPIO.input(GPIO_PIR)
			
					if Current_State==1 and Previous_State==0:
						# Le detecteur a envoyé un signal
						print("Mouvement détecté !")
						print(" ")
						# On enregistre l'ancien etat
						Previous_State=1
						os.system("python2 securite.py");
						time.sleep(20)
					elif "Off" in rows :
						print("Capteur désactivé")
						sys.exit(0)
					elif Current_State==0 and Previous_State==1:
						# Le capteur est a nouveau pret
						print("Attente detection ")
						Previous_State=0
		 
					# On attend 1 seconde
					time.sleep(1)
	

	except KeyboardInterrupt:
		print(" Quit")

	# Reinitialisation des parametres GPIOs
		GPIO.cleanup()

else :
	print("Capteur OFF")
	os.system("python2 securite.py");
