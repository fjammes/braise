#include <Wire.h>
#include <arduinoUtils.h>
////#include "arduinoLoRa.h"
#include <SPI.h>
#include <ArduinoJson.h>
#include "SX1272.h"
int e;

char donnees_recues[256];
char ordre_acqui [] = "ACQUI";

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

void setup()
{
  Serial.begin(9600);

   Serial.println(F("SX1272 module and Arduino: receive packets without ACK"));
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

    e = sx1272.receivePacketTimeout(10000);
  if ( e == 0 )
  {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);
    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      donnees_recues[i] = (char)sx1272.packet_received.data[i];
    }

  }

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
       //latitude_gps      = donnees_decod["latitude"];
       //longitude_gps     = donnees_decod["longitude"];
       annee_gps         = donnees_decod["annee"];
       mois_gps          = donnees_decod["mois"];
       jour_gps          = donnees_decod["jour"];
       heure_gps         = donnees_decod["heure"];
       minute_gps        = donnees_decod["minute"];
       seconde_gps       = donnees_decod["seconde"];
       latitude_gps      = donnees_decod["position"][0];
       longitude_gps     = donnees_decod["position"][1];
       temp_bme          = donnees_decod["bme"][0];
       press_bme         = donnees_decod["bme"][1];
       hum_bme           = donnees_decod["bme"][2];

affichage();

//delay(10000);
}

void affichage(void)
{
Serial.print("******* ");
Serial.print(annee_gps);
Serial.print("/");
Serial.print(mois_gps);
Serial.print("/");
Serial.print(jour_gps);
Serial.print("/");
Serial.print(heure_gps);
Serial.print("/");
Serial.print(minute_gps);
Serial.print("/");
Serial.print(seconde_gps);
Serial.println(" *******");

Serial.println("------- Position -------");
Serial.print(latitude_gps,6);
Serial.print("/");
Serial.println(longitude_gps,6);


Serial.println("------- Capteur T/P/H -------");
Serial.print("Temp: ");
Serial.print(temp_bme);
Serial.println("C");
Serial.print("Pression: ");
Serial.print(press_bme);
Serial.println("Pa");
Serial.print("Humidite: ");
Serial.print(hum_bme);
Serial.println("%");
Serial.println("");
}

