/* ----------------------------------------------------------------------------------------
* Recupère les données gps et capteur et les transmet à la carte maitre
* Fonctionne aléatoirement pour l'acquisition de la date, le reste est OK
* -------------------------------------------------------------------------------------------*/

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

BME280 mySensor;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println("Start setup()");

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

   mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 1;
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;


        Serial.println(F("Set up BME"));
	delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
	Serial.println(mySensor.begin(), HEX);
        Serial.println(F("Finish setup()"));

}

void loop()
{

    while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
     {
      mesureBME280();
      displayInfo();
      getInfogps();
      encodage_json();
      transmission_json();
      delay(3000);
     }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("FATAL: No GPS detected: check wiring."));
    while(true);
  }


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
         Serial.println(temp_bme,2);
         Serial.print("pression: ");
         Serial.println(press_bme,2);
         Serial.print("humidite: ");
         Serial.println(hum_bme,2);
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
    Serial.print(F("Location INVALID"));
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
    Serial.print(F("Date INVALID"));
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
    //Serial.print(F("."));
    //if (gps.time.centisecond() < 10) Serial.print(F("0"));
    //Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("Time INVALID"));
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
  Serial.println(F("Start encodage_json()"));
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
//Serial.println(data_json);
}

void transmission_json()
{
  Serial.println("Start transmission data_json()");
       e = sx1272.sendPacketTimeout(0, data_json);

       Serial.print(F("Packet sent, state "));
       Serial.println(e, DEC);

}
