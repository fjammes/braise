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
#include <ArduinoJson.h>
//#include <arduinoUtils.h>
////#include "arduinoLoRa.h"
#include <SPI.h>
#include "SX1272.h"

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
const int enable_gps = 8;
int Tmax;
int T;
int e;

char data_json[256];
char ordre[10];
char echec [] = "echec";

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
  T=0;
  Tmax=15000; //~2min30
  digitalWrite(enable_gps, HIGH);
  
  Serial.println(F("SX1272 module and Arduino: send packets without ACK"));
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  e |= sx1272.setMode(4);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  e |= sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  e |= sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  e |= sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  e |= sx1272.setPower('H');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  e |= sx1272.setNodeAddress(3);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));
    
}

void loop() 
{

  e = sx1272.receivePacketTimeout(20000);
  if ( e == 0 )
  {
    Serial.print(F("Paquet recu, state "));
    Serial.println(e, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      ordre[i] = (char)sx1272.packet_received.data[i];
    }
      if(strcmp(ordre,"ACQUI"))
       {Serial.println("Mauvais ordre recu");
       }
       else
       {
       Serial.println("Ordre 'ACQUI' recu, aqcuisition...");
         while (ss.available() > 0 && T<=Tmax)
          {
            if (gps.encode(ss.read()))
              {
                displayInfo();
                transmission_json();
              } 
             T++; 
          }
  
        if(T>=Tmax)
         {Serial.println("temps max depasse");
          Serial.println("GPS OFF");
          digitalWrite(enable_gps, LOW);
          e = sx1272.sendPacketTimeout(0, echec);
          Serial.print(F("Packet sent, state "));
          Serial.println(e, DEC);
          Serial.println("Notification echec envoyee");
         }
 
 
 
 transmission_json();
 
       }
       

  //T=0;  //pour relancer la recherche

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
    heure_gps = gps.time.hour();
    Serial.print(F(":"));
    if (gps.time.minute() < 10) {Serial.print(F("0"));minute_gps = gps.time.minute();}
    Serial.print(gps.time.minute());
    minute_gps = gps.time.minute();
    Serial.print(F(":"));
    if (gps.time.second() < 10) {Serial.print(F("0"));seconde_gps = gps.time.second();}
    Serial.print(gps.time.second());
    seconde_gps = gps.time.second();
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

void encodage_json()
{
  
 StaticJsonBuffer<200> jsonBuffer;
//constructon des objets
JsonObject& data = jsonBuffer.createObject();

data["latitude"] = latitude_gps;
data["longitude"] = longitude_gps;
data["annee"] = annee_gps;
data["mois"] = mois_gps;
data["jour"] = jour_gps;
data["heure"] = heure_gps;
data["minute"] = minute_gps;
data["seconde"] = seconde_gps;

//char data_json[256];
data.printTo(data_json, sizeof(data_json));  

}  

void transmission_json()
{
       e = sx1272.sendPacketTimeout(0, data_json);
       Serial.println("transmission data_json");
       Serial.print(F("Packet sent, state "));
       Serial.println(e, DEC);
  
}  
