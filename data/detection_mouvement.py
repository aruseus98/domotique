# Detecter des mouvements avec un capteur infrarouge
#
#
# Importation des bibliothèques python
import RPi.GPIO as GPIO
import time
import urllib
import os
 
# Utiliser les BCM GPIO et pas les numeros des pins sur P1
GPIO.setmode(GPIO.BCM)
 
# Pin GPIO utilisee
GPIO_PIR = 7
 
print("Test du capteur de presence IR (CTRL-C pour quitter)")
 
# Configurer la pin 7 en entree
GPIO.setup(GPIO_PIR,GPIO.IN)
 
# Initialisation des variables
Current_State  = 0
Previous_State = 0
 
try:
	print("Attente detection...")

	# Boucle jusqu a ce que la sortie du capteur passe a 0
	while GPIO.input(GPIO_PIR)==1:
		Current_State = 0
		print(" Pret")
	 
	# Tourne en boucle jusqu'a ce que l utilisateur quitte (par CTRL+C)
	while True :
		# Lire l'etat du capteur
		Current_State = GPIO.input(GPIO_PIR)
		 
		if Current_State==1 and Previous_State==0:
			# Le detecteur a envoyé un signal
			print(" Mouvement detecte !")
			os.system("sudo ./photos.sh");
			# On attend 5 secondes
			time.sleep(5)
			# On enregistre l'ancien etat
			Previous_State=1
		elif Current_State==0 and Previous_State==1:
			# Le capteur est a nouveau pret
			print(" Pret ")
			Previous_State=0
		 
		# On attend 1 seconde
		time.sleep(1)
 
except KeyboardInterrupt:
	print(" Quit")

# Reinitialisation des parametres GPIOs
GPIO.cleanup()

