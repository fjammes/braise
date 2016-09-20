 #include <Wire.h>
#include <arduinoUtils.h>
#include "arduinoLoRa.h"
#include <SPI.h>
#include <ArduinoJson.h>

int e;
char donnees_recues[256];
char message1 [] = "go";
char ordre_stop [] = "stop";

float       tempC_recue;
float       pression_recue;   
float       hum_recue;        


void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  // Print a start message
  Serial.println(F("SX1272 module and Arduino: send packets without ACK"));
  
  // Power ON the module
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e |= sx1272.setMode(4);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  
  // Set header
  e |= sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e |= sx1272.setNodeAddress(3);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));
}

void loop(void)
{
  e = sx1272.sendPacketTimeout(0, message1);
  Serial.print(F("Packet sent, state "));
  Serial.println(e, DEC);
  Serial.println("Ordre GO envoye, en attente de reception...");
  
    e = sx1272.receivePacketTimeout(100000);
  if ( e == 0 )
  {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);
    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      donnees_recues[i] = (char)sx1272.packet_received.data[i];
    }    
  }

/*------------------------------------------------------------------------------------------
* Decodage Json
*------------------------------------------------------------------------------------------*/
//reservation memoire
StaticJsonBuffer<256> donnees_recues_decodage;

//deserialisation

JsonObject& donnees_decod = donnees_recues_decodage.parseObject(donnees_recues);

if (!donnees_decod.success())
{
  Serial.println("parseObject() failed");
  return;
}    

char data_json[256];
donnees_decod.printTo(data_json, sizeof(data_json));
Serial.println(data_json);


//récuperation des données
       tempC_recue      = donnees_decod["tempC"]; 
       pression_recue   = donnees_decod["pression"]; 
       hum_recue        = donnees_decod["hum"];


affichage();


  // Send message2 broadcast and print the result
  e = sx1272.sendPacketTimeout(0, ordre_stop);
  Serial.println("ordre STOP envoye");
  Serial.print(F("Packet sent, state "));
  Serial.println(e, DEC);
  Serial.println("");

  delay(10000);  
}





void affichage(void) 
{
/* Serial.print("-------");
Serial.print(jour_recue);
Serial.print("/");
Serial.print(mois_recue);
Serial.print("/");
Serial.print(annee_recue);
Serial.print("/");
Serial.print(heure_recue);
Serial.print("/");
Serial.print(minute_recue);
Serial.print("/");
Serial.print(seconde_recue);
Serial.println("-------");*/

Serial.print("temperature (C): ");
Serial.println(tempC_recue);
Serial.print("pression (Pa): ");
Serial.println(pression_recue);
Serial.print("humidite (%): ");
Serial.println(hum_recue);

}

