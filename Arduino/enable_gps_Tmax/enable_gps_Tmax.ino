#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//const int validation_gps = 7;
int location_valide;
static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;
long int Tmax = 30000;   // ~temps d'attente max 1min30
long int T_attente = 0;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup() 
{
  
/*pinMode(validation_gps, OUTPUT);  
digitalWrite(validation_gps, LOW);*/

Serial.begin(9600);
ss.begin(GPSBaud);     //début de la liaison série avec le GPS



}

void loop()
{
/*digitalWrite(validation_gps, HIGH);     //activation du gps
delay(1000);*/
Serial.println("Demarrage");
  
    while (ss.available() > 0)
    {
    if (gps.encode(ss.read()))   //encode regarde si un caractère est présent sur la liaison série et renvoie un boolean
      {
        Serial.println("debug");
       displayInfo();
      }
    }


 /* if (millis() > 10000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }*/
  
/*digitalWrite(validation_gps, LOW);
delay(10000);*/
}

void displayInfo()
{
  Serial.println("display");
  do
  {
   T_attente++;
   Serial.println(T_attente);
   location_valide = gps.location.isValid();
   Serial.println(location_valide);
  }while(T_attente < Tmax && (location_valide==0)); 
  
  delay(100);
  if(location_valide==1)
   {
    Serial.println("affichage data"); 
   } 
  /*while(!gps.location.isValid() && T_attente < Tmax)
  {
    T_attente++;
  }
  
  if (gps.location.isValid())
  
  { Serial.print(F("Location: "));
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }*/


  
  /*if (gps.date.isValid())
  {
    Serial.print(F("  Date/Time: "));
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
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
  }*/

  Serial.println();
}
