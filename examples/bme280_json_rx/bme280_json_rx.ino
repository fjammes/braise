/*-------------------------------------------------------------------------------------------------------------------------------------------------------
* Reception des données du capteur de température par loRa, en format JSON
* Décodage JSON et affichage
*---------------------------------------------------------------------------------------------------------------------------------------------------------*/ 

#include <RTCZero.h>
#include <stdint.h>
#include "Wire.h"
//#include "SPI.h"
//#include <Wire.h>
#include <ArduinoJson.h>
//#include <arduinoUtils.h>
//#include "arduinoLoRa.h"
#include <SPI.h>
#include "SX1272.h"

RTCZero rtc;

unsigned int i=0;
int e;
/*int annee, mois, jour, heure, minute, seconde;
float tempC, tempF, pression, altM, altF, hum;*/
char donnees_recues[256];

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 00;
const byte hours = 17;

/* Change these values to set the current initial date */
const byte day = 27;
const byte month = 04;
const byte year = 16;

void setup()
{

  Serial.begin(9600);
  
  /*--------------------------------------------------------------------------------------------------------------
* Initialisation RTC
*---------------------------------------------------------------------------------------------------------------*/
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(17, 00, 10);
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(alarmMatch);  
  
  
/*--------------------------------------------------------------------------------------------------------------
* Initialisation LoRa
*---------------------------------------------------------------------------------------------------------------*/  

  Serial.println(F("SX1272 module and Arduino: receive packets without ACK"));

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
  e |= sx1272.setNodeAddress(8);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);

  // Print a success message
  if (e == 0){
    Serial.println(F("SX1272 successfully configured"));
    Serial.println("\n");}
  else{
    Serial.println(F("SX1272 initialization failed"));
    Serial.println("");}

  rtc.standbyMode();


}

void loop()
{
/*------------------------------------------------------------------------------------------
* Reception des données
*------------------------------------------------------------------------------------------*/
    delay(100);
  e = sx1272.receivePacketTimeout(10000);
  
    if ( e == 0 )
  {
    Serial.print(F("Reception..., state "));
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
const char* capteur    = donnees_decod["capteur"];
float       tempC      = donnees_decod["tempC"]; 
float       tempF      = donnees_decod["tempF"]; 
float       pression   = donnees_decod["pression"]; 
float       altM       = donnees_decod["altM"]; 
float       altF       = donnees_decod["altF"]; 
float       hum        = donnees_decod["hum"];
int         annee      = donnees_decod["dateheure"][0];
int         mois       = donnees_decod["dateheure"][1];
int         jour       = donnees_decod["dateheure"][2];
int         heure      = donnees_decod["dateheure"][3];
int         minute     = donnees_decod["dateheure"][4];
int         seconde    = donnees_decod["dateheure"][5];


/*------------------------------------------------------------------------------------------
* Affichage
*------------------------------------------------------------------------------------------*/
Serial.print("-------");
Serial.print(jour);
Serial.print("/");
Serial.print(mois);
Serial.print("/");
Serial.print(annee);
Serial.print("/");
Serial.print(heure);
Serial.print("/");
Serial.print(minute);
Serial.print("/");
Serial.print(seconde);
Serial.println("-------");

Serial.print("capteur: ");
Serial.println(capteur);
Serial.print("temperature (C): ");
Serial.println(tempC);
Serial.print("temperature (F): ");
Serial.println(tempF);
Serial.print("pression (Pa): ");
Serial.println(pression);
Serial.print("altitude (m): ");
Serial.println(altM);
Serial.print("altitude (ft): ");
Serial.println(altF);
Serial.print("humidite (%): ");
Serial.println(hum);
Serial.println("");
    rtc.standbyMode();
}

void alarmMatch() 
{
  


}
