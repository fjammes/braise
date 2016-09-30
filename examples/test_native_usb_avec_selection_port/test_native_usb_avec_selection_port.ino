/*  
Choisir comme :
      1- programmer "AVR ISP", 
      2- Board sur "Arduino M0 pro (native USB port)
      3- Choisir le port adequate
Programme utilisant la librairie streaming.h permettant d'utiliser une seule et unique commande pour l'affichage sur la console.
    Rappel:
      En mode "programming usb port" la syntaxe est Serial.begin(9600) et l'affichage Serial.println("texte")
      En mode "native USB port" la syntaxe est SerialUSB.begin(9600) et l'affichage SerialUSB.println("texte")
Ici à travers la librairie streaming.h nous avons la m^me syntaxe quelque soit le port choisis.

De plus il n'y a qu'une modification mineure à apporter au programme pour basculer d'un port à un autre
*/



#include <Arduino.h>
#include <Streaming.h>

#define __nativeUSB__ // A commenter si jamais on utilises le port Programming !!!

#if defined(__nativeUSB__)
#define PORT SerialUSB
#else
#define PORT Serial
#endif


int variable = 12;
  
void setup() 
{ PORT.begin(9600); 
  while(!PORT); // for the native USB port to print from start or reset
  delay(2000); // for the SAM D21 Programming (USB) Port to print from reset or start, do open the serial monitor within two seconds 
  
}

void loop()
{ PORT.println("Test programming port USB");
//PORT << "Test" << endl;
//PORT << variable << endl;
PORT << "Le test me donne une valeur de :" << variable << "."<< endl;
delay(1000);
}
