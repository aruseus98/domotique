#include <SPI.h>
#include <MFRC522.h>
#include <Ethernet.h>
//On défini les pin
#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 6
unsigned long previousMillis=0;
unsigned long interval = 10000L;
unsigned long interval2 = 25000L;
int incomingByte;
byte SerialOK=0;


// Definition des pin du RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); 

char st[20];

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  //initialisation serie(pour le relevé d'information via le moniteur série)
  //et initialisation des deux bibliotheque SPI et MFRC522
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init(); 
  // Message initial (le message afiché au démarrage 
  //dans le moniteur serie de notre programme arduino)
  Serial.println("------ Mobihouse++ ------");
  Serial.println("Veuillez passer votre badge");
}

void loop() {
  // put your main code here, to run repeatedly:
if(millis() - previousMillis <= interval)
 { 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    digitalWrite(LED_PIN, HIGH);
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    digitalWrite(LED_PIN, HIGH);
    return;
  }
  Serial.print("UID de tag :");
  String tag= "";
  byte caractere;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     tag.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     tag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  tag.toUpperCase();

 // ici on va vérifier l'autorisation

 if (tag.substring(1) == "30 4B BA 79")
 {
    Serial.println("Vérification de votre badge...");
    delay(2000);
    Serial.println("Acces autorisé ! Vous pouvez entrer");
    digitalWrite(LED_PIN, LOW);
    Serial.println();
    delay(3000);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Sécurité OFF");
    Serial.println("Désactivé");
    }
 
  // ici on va vérifier un second badge 
  //(il est possible d'ajouter plusieurs "else if" il peut être interessant 
  
   else if (tag.substring(1) == "ED F1 49 15")
    //il suffit d'insérer ci-dessus le tag que l'on souhaite authoriser
    //ici on dit que si le tag = ED F1 49 15 est lu 
    //alors on affiche dans le moniteur serie
    
  // Tag verifie - Acces Autorisé et nous eteignons notre led pendant 3sec
  {
    Serial.println("Vérification de votre badge...");
    delay(2000);
    Serial.println("Acces autorisé ! Vous pouvez entrer");
    digitalWrite(LED_PIN, LOW);
    Serial.println();
    delay(3000);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Sécurité OFF");
    Serial.println("Désactivé");
  }
  
  else
  //sinon si le Tag n'est pas valide //on affiche Acces refuse !!!
  { 
    Serial.println("Vérification de votre badge...");
    Serial.flush();
    delay(2000);
    Serial.println("Badge non reconnu ! Accès refusé !!!");
    Serial.flush();
    Serial.println();
    
    // On repete 5 fois
    for (int i= 1; i<5 ; i++)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(300);
      digitalWrite(LED_PIN, LOW);
      delay(300);
    }

    delay(4000);
    if (tag.substring(1) != "30 4B BA 79" && tag.substring(1) != "ED F1 49 15")
    {
      Serial.println("------ Activation de Mobihouse++'s Security ------");
      Serial.flush();
      Serial.println("Prêt");
    }
    
  }
}

else if(millis() - previousMillis >= interval && millis() - previousMillis <= interval2)
   {
     Serial.println("Aucun badge détecté !");
     Serial.println("Code");
     while(SerialOK==0)
      {
        if(Serial.available())
        {
          incomingByte = Serial.parseInt();//Lecture d'un entier sur le tampon série
          if(incomingByte=="1")
          {
            Serial.println("Code Bon");
            Serial.println("Sécurité OFF");
            Serial.println("Désactivé");
            SerialOK==1;
          }
      
          else
          {
            Serial.println("Code erroné");
            Serial.println("------ Activation de Mobihouse++'s Security ------");
            Serial.flush();
            Serial.println("Prêt");
            delay(1000);
          }
     /*if(millis() - previousMillis >= interval2)
     {
       Serial.println("------ Activation de Mobihouse++'s Security ------");
       Serial.flush();
       Serial.println("Prêt");
       delay(1000);
       exit(0);
     }*/
   }
  
   else
   {
     if(millis() - previousMillis >= interval2)
     {
       Serial.println("------ Activation de Mobihouse++'s Security ------");
       Serial.flush();
       Serial.println("Prêt");
       delay(1000);
      /*while(SerialOK==0)
      {
        if(Serial.available() > 0)
        {
          incomingByte = Serial.read();
          if(incomingByte=='0000')
          {
            Serial.println("Code Bon");
            Serial.println("Sécurité OFF");
            Serial.println("Désactivé");
            SerialOK==1;
          }
      
          else
          {
            Serial.println("Code erroné");
            Serial.println("------ Activation de Mobihouse++'s Security ------");
            Serial.flush();
            Serial.println("Prêt");
            delay(1000);
            exit(0);
          }
        }
      }*/
      } 
    }
   }
  }
}
