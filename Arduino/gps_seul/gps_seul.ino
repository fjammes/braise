/*--------------------------------------------------------------------------------------------
* Programme permettant d'acquérir les données gps lorsqu'un ordre est reçu et transmet les infos au maitre
* Desactive le gps si aucun satellite n'est trouvé au bout d'un certain temps (Tmax) et envoie une donnée echec
* 
*----------------------------------------------------------------------------------------------*/


#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//#include "Wire.h"
//#include "SPI.h"
#include <Wire.h>
#include <SPI.h>
#include <stdint.h>

static const int RXPin = 13, TXPin = 12;
//static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;
const int enable_gps = 9;
//int Tmax;
//int T;
int e;

//char data_json[256];
//char ordre[10];
//char echec [] = "echec";

float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);
 
void setup()
{
  pinMode(enable_gps, OUTPUT);
  Serial.begin(9600);
  ss.begin(GPSBaud);  
  Serial.println("Demarrage...");

    
}

void loop() 
{
   
    
    while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
     {         
      getInfogps();
      displayInfo();
      
      delay(5000);
   
     }     
     
  } 
  
 
  
   if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
 
   
}



void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    latitude_gps = gps.location.lat();
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    longitude_gps = gps.location.lng();
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    mois_gps = gps.date.month();
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    jour_gps = gps.date.day();
    Serial.print(F("/"));
    Serial.print(gps.date.year());
    annee_gps = gps.date.year();
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) {Serial.print(F("0")); heure_gps = gps.time.hour();}
    Serial.print(gps.time.hour());
    //heure_gps = gps.time.hour();
    Serial.print(F(":"));
    if (gps.time.minute() < 10) {Serial.print(F("0"));minute_gps = gps.time.minute();}
    Serial.print(gps.time.minute());
    //minute_gps = gps.time.minute();
    Serial.print(F(":"));
    if (gps.time.second() < 10) {Serial.print(F("0"));seconde_gps = gps.time.second();}
    Serial.print(gps.time.second());
    //seconde_gps = gps.time.second();
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}

void getInfogps()
{
     latitude_gps = gps.location.lat();
     longitude_gps = gps.location.lng();
     mois_gps = gps.date.month();
     jour_gps = gps.date.day();
     annee_gps = gps.date.year();
     heure_gps = gps.time.hour();
     minute_gps = gps.time.minute();
     seconde_gps = gps.time.second();  

} 

