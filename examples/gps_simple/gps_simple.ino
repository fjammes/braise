#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "Wire.h"
#include "SPI.h"
#include <Wire.h>
#include <ArduinoJson.h>
#include <arduinoUtils.h>
#include "arduinoLoRa.h"
#include <SPI.h>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

char data_json[256];

float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;
int e;
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  
   
  Serial.println("Demarrage..."); 
 
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

  Serial.println();
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
     {
      displayInfo();
      getInfo();
      encodage_json();
      transmission_json();
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

void getInfo()
{
     latitude_gps = gps.location.lat();
     longitude_gps = gps.location.lng();
     mois_gps = gps.date.month();
     jour_gps = gps.date.day();
     annee_gps = gps.date.year();
     heure_gps = gps.time.hour();
     minute_gps = gps.time.minute();
     seconde_gps = gps.time.second();  
/*Serial.println(latitude_gps);
Serial.println(longitude_gps);
Serial.println(mois_gps);
Serial.println(jour_gps);
Serial.println(annee_gps);
Serial.println(heure_gps);
Serial.println(minute_gps);
Serial.println(seconde_gps);*/
}  

void encodage_json()
{
StaticJsonBuffer<256> jsonBuffer;

//constructon des objets
JsonObject& data = jsonBuffer.createObject();

JsonArray& latlon = data.createNestedArray("position");
latlon.add(gps.location.lat()/*latitude_gps*/, 6);  // 6 is the number of decimals to print
latlon.add(gps.location.lng()/*longitude_gps*/, 6);

data["annee"] = gps.date.year();//annee_gps;
data["mois"] = gps.date.month();//mois_gps;
data["jour"] = gps.date.day();//jour_gps;
data["heure"] = gps.time.hour();//heure_gps;
data["minute"] = gps.time.minute();//minute_gps;
data["seconde"] = gps.time.second();//seconde_gps;


//char data_json[256];
data.printTo(data_json, sizeof(data_json));  
//Serial.println(data_json);
}

void transmission_json()
{
       e = sx1272.sendPacketTimeout(0, data_json);
       Serial.println("transmission data_json");
       Serial.print(F("Packet sent, state "));
       Serial.println(e, DEC);
  
}
