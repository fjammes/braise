/*--------------------------------------------------------------------------------------------
* Programme permettant d'acquérir les données gps
* Desactive le gps si aucun satellite n'est trouvé au bout d'un certain temps (Tmax)
* Fonctionne
*----------------------------------------------------------------------------------------------*/


#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <stdint.h>

static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;
const int enable_gps = 9;
int Tmax;
int T;

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
  //digitalWrite(enable_gps, HIGH);
  
}

void loop() 
{

//Serial.println(T);
  while (ss.available() > 0 && T<=Tmax)
   //while (ss.available() > 0)
   // while ( T<=Tmax)
 // Serial.println("test");
  {
    if (gps.encode(ss.read()))
    {
      displayInfo();
    } 
   T++; 
   //Serial.println(T);
  }
  
  if(T>=Tmax)
   {Serial.println("temps max depasse");
   Serial.println("GPS OFF");
   digitalWrite(enable_gps, LOW);
   T=0;  //pour relancer la recherche
   }
  //T=0;  //pour relancer la recherche

}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
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
