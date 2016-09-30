/*--------------------------------------------------------------------
* Recupère les données gps et capteur et les transmet à la carte maitre
* avec ACK
* -------------------------------------------------------------------*/


	//! It receives a packet before a timeout and reply with an ACK.
  	/*!
  	\param uint16_t wait : time to wait to receive something.
	\return '0' on success, '1' otherwise
	 */
	//uint8_t receivePacketTimeoutACK(uint16_t wait);

	//! It sends the packet wich payload is a parameter before a timeout, and replies with ACK.
	/*!
	\param uint8_t dest : packet destination.
	\param char *payload : packet payload.
	\return '0' on success, '1' otherwise
	*/
	//uint8_t sendPacketTimeoutACK(uint8_t dest, char *payload);

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//#include "Wire.h"
//#include "SPI.h"
#include <Wire.h>
#include <ArduinoJson.h>
//#include <arduinoUtils.h>
//#include "arduinoLoRa.h"
#include <SPI.h>
#include <stdint.h>
#include "SparkFunBME280.h"
#include "SX1272.h"

static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;

char data_json[256];

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;
int e;
int info;

BME280 mySensor;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup() 
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println("Demarrage..."); 
 
   Serial.println(F("SX1272 module and Arduino: send packets with ACK"));
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
  
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 1;
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;

	delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
	Serial.println(mySensor.begin(), HEX);


}

void loop() 
{
int i =0;
int j =0;
  

 
  while (ss.available() > 0 && i < 4 /*&& j < ...*/)    
  //while (ss.available() > 0 )
  {
    
    if (gps.encode(ss.read()))
     {     
      ////Compteur J : Peut être nécessaire si jamais on souhaite limiter le temps de recherche des satellites !!///////////////////
      
      Serial.println("Recherche de satellites...");
      //Serial.println(j);
      j++;  
      
      ////////////////Vérification de la trame complète venant du GPS ///////////////////////////////////
      if(verification_gps())
       {
      Serial.println("Premieres donnees acquises...");
      getInfogps();
      i++;
      Serial.println(i);
      //delay(1000);  
      
      displayInfo(); 
      mesureBME280(); 
      encodage_json();
      transmission_json();
      
       }
      //displayInfo();
      //delay(3000);
     } 
  }
  
      //mesureBME280(); 
      //encodage_json();
     // transmission_json();
}

boolean verification_gps()
{
boolean all_ok = false;
boolean pos_ok = gps.location.isValid();
boolean date_ok = gps.date.isValid();
boolean time_ok = gps.time.isValid();

if(pos_ok && date_ok && time_ok == true)
{
  return all_ok = true;
}  
  else{return all_ok = false;}

}

void mesureBME280()
{
 uint8_t valeur_reg_ctrl_meas;

  
        valeur_reg_ctrl_meas = mySensor.readRegister(0xF4);                  //sauvegarde de la valeur du registre ctrl_meas
        delay(5);
        valeur_reg_ctrl_meas = valeur_reg_ctrl_meas | 0x01;                  //masque pour passer de sleep mode à forced mode
        delay(5);
        mySensor.writeRegister(BME280_CTRL_MEAS_REG, valeur_reg_ctrl_meas);  //ecriture de la nouvelle valeur dans le registre ctrl_meas
        delay(5);
  
        temp_bme=mySensor.readTempC();
        delay(10);
        press_bme=mySensor.readFloatPressure();
        delay(10);
        hum_bme=mySensor.readFloatHumidity();
         Serial.print("temp: ");
         Serial.println(temp_bme,6);
         Serial.print("pression: ");
         Serial.println(press_bme,6);
         Serial.print("humidite: ");
         Serial.println(hum_bme,6);
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


void encodage_json()
{
StaticJsonBuffer<256> jsonBuffer;

//constructon des objets
JsonObject& data = jsonBuffer.createObject();

JsonArray& bmedata = data.createNestedArray("bme");
bmedata.add(temp_bme, 2);  // 6 is the number of decimals to print
bmedata.add(press_bme);
bmedata.add(hum_bme);

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
Serial.println(data_json);
}

void transmission_json()
{
  e = sx1272.sendPacketTimeoutACK(0, data_json);
  
  
    info = sx1272.getSNR();
    Serial.print(F("-> SNR: "));
    Serial.println(sx1272._SNR); 
    
    info = sx1272.getRSSI();
    Serial.print(F("-> RSSI: "));
    Serial.println(sx1272._RSSI);   
    
    info = sx1272.getRSSIpacket();
    Serial.print(F("-> Last packet RSSI value is: "));    
    Serial.println(sx1272._RSSIpacket); 
    
    
  Serial.print(F("Packet sent, state "));
  Serial.println(e, DEC);
  switch(e)
  {
   case 0: Serial.println("Packet envoye, ACK recu");
           break; 
   case 1: Serial.println("Erreur");
           break; 
   case 2: Serial.println("Commande non executee");
           break;
   case 3: Serial.println("Packet envoye, ACK non recu");
           break;   
  }
  
}
