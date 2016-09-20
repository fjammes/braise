/*------------------------------------------------------------------------------
* Fonctionne quand il veut
*-------------------------------------------------------------------------------*/

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

int data_ok;
int i;
long int T_attente;
long int Tmax = 10000;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

}

void loop()
{

  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))   //encode regarde si un caractère est présent sur la liaison série et renvoie un boolean
     {
      displayInfo();
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
data_ok = 0;
T_attente = 0; 
while((T_attente < Tmax) && (data_ok == 0))
{
  if (gps.location.isValid())
  {
    delay(1000);
    Serial.println(gps.location.isValid());
    
    for(i=0;i<=1000;i++)         //boucle for car les premières valeurs acquises ne sont pas correctes,
      {                        //les valeurs définitives sont prises après 20 acquisitions
    latitude_gps = gps.location.lat();
    Serial.println(latitude_gps,6);
    longitude_gps = gps.location.lng();
    Serial.println(longitude_gps, 6);
    mois_gps = gps.date.month();
    Serial.println(mois_gps);
    jour_gps = gps.date.day();
    Serial.println(jour_gps);
    annee_gps = gps.date.year();    
    Serial.println(annee_gps);
    
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
    Serial.println(gps.time.centisecond());
  }
      }
    data_ok = 1;
  }
  else
  {
    Serial.println(gps.location.isValid());
    Serial.println(F("INVALID"));
  }
delay(500);
}
  Serial.println();
}
