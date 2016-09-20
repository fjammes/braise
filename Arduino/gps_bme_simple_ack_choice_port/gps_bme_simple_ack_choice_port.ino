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

/*  Pour programmer à partir du "native USB"
Choisir :
      1- programmer "AVR ISP", 
      2- Board sur "Arduino M0 pro (native USB port)
      3- Choisir le port adequate
Programme utilisant la librairie streaming.h permettant d'utiliser une seule et unique commande pour l'affichage sur la console.
    Rappel:
      En mode "programming usb port" la syntaxe est Serial.begin(9600) et l'affichage Serial.println("texte")
      En mode "native USB port" la syntaxe est SerialUSB.begin(9600) et l'affichage SerialUSB.println("texte")
Ici à travers la librairie streaming.h nous avons la m^me syntaxe quelque soit le port choisis.

De plus il n'y a qu'une modification mineure à apporter au programme pour basculer d'un port à un autre
*/

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <stdint.h>
#include "SparkFunBME280.h"
#include "SX1272.h"
#include <Streaming.h>


static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;

char data_json[256];

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;
int e;


#define __nativeUSB__ // A commenter si jamais on utilises le port Programming !!!

#if defined(__nativeUSB__)
#define PORT SerialUSB
#else
#define PORT Serial
#endif



BME280 mySensor;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup() 
{
  PORT.begin(115200); 
  while(!PORT); // for the native USB port to print from start or reset
  delay(2000); // for the SAM D21 Programming (USB) Port to print from reset or start, do open the serial monitor within two seconds 
  
  ss.begin(GPSBaud);
  PORT.println("Demarrage..."); 
 
  PORT.println(F("SX1272 module and Arduino: send packets with ACK"));
  delay(2000);
  PORT.println(F("test"));
  e = sx1272.ON();
  PORT.print(F("Setting power ON: state "));
  PORT.println(e, DEC);
  
  e |= sx1272.setMode(4);
  PORT.print(F("Setting Mode: state "));
  PORT.println(e, DEC);
  
  e |= sx1272.setHeaderON();
  PORT.print(F("Setting Header ON: state "));
  PORT.println(e, DEC);
  
  e |= sx1272.setChannel(CH_10_868);
  PORT.print(F("Setting Channel: state "));
  PORT.println(e, DEC);
  
  e |= sx1272.setCRC_ON();
  PORT.print(F("Setting CRC ON: state "));
  PORT.println(e, DEC);
  
  e |= sx1272.setPower('H');
  PORT.print(F("Setting Power: state "));
  PORT.println(e, DEC);
  
  e |= sx1272.setNodeAddress(3);
  PORT.print(F("Setting node address: state "));
  PORT.println(e, DEC);
  
  
  if (e == 0)
    PORT.println(F("SX1272 successfully configured"));
  else
    PORT.println(F("SX1272 initialization failed"));

  PORT.println();
  
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 1;
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;

	delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
	PORT.println(mySensor.begin(), HEX);


}

void loop() 
{
int i =0;
int j =0;
  
  
  while (ss.available() > 0 && i < 4 /*&& j < ...*/)    
  
  {
    
    if (gps.encode(ss.read()))
     {     
      ////Compteur J : Peut être nécessaire si jamais on souhaite limiter le temps de recherche des satellites !!///////////////////
      
      PORT.println("Recherche de satellites...");
      j++;  
      
      ////////////////Vérification de la trame complète venant du GPS ///////////////////////////////////
      if(verification_gps())
       {
      PORT.println("Premieres donnees acquises...");
      getInfogps();
      i++;
     // PORT.println(i);      
      displayInfo(); 
      mesureBME280(); 
      encodage_json();
      transmission_json();
       PORT.println("");
       }
      
     } 
  }
  
     
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
         PORT.print("temp: ");
         PORT.println(temp_bme,6);
         PORT.print("pression: ");
         PORT.println(press_bme,6);
         PORT.print("humidite: ");
         PORT.println(hum_bme,6);
} 

void displayInfo()
{
  PORT.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    PORT.print(gps.location.lat(), 6);
    PORT.print(F(","));
    PORT.print(gps.location.lng(), 6);
  }
  else
  {
    PORT.print(F("INVALID"));
  }

  PORT.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    PORT.print(gps.date.month());
    PORT.print(F("/"));
    PORT.print(gps.date.day());
    PORT.print(F("/"));
    PORT.print(gps.date.year());
  }
  else
  {
    PORT.print(F("INVALID"));
  }

  PORT.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) PORT.print(F("0"));
    PORT.print(gps.time.hour());
    PORT.print(F(":"));
    if (gps.time.minute() < 10) PORT.print(F("0"));
    PORT.print(gps.time.minute());
    PORT.print(F(":"));
    if (gps.time.second() < 10) PORT.print(F("0"));
    PORT.print(gps.time.second());
    PORT.print(F("."));
    if (gps.time.centisecond() < 10) PORT.print(F("0"));
    PORT.print(gps.time.centisecond());
  }
  else
  {
    PORT.print(F("INVALID"));
  }

  PORT.println();
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
PORT.println(data_json);
}

void transmission_json()
{
  e = sx1272.sendPacketTimeoutACK(0, data_json);
  PORT.print(F("Packet sent, state "));
  PORT.println(e, DEC);
  switch(e)
  {
   case 0: PORT.println("Packet envoye, ACK recu");
           break; 
   case 1: PORT.println("Erreur");
           break; 
   case 2: PORT.println("Commande non executee");
           break;
   case 3: PORT.println("Packet envoye, ACK non recu");
           break;   
  }
  
}
