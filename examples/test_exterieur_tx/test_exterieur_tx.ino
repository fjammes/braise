#include <RTCZero.h>
#include <Wire.h>
//#include <arduinoUtils.h>
//#include "arduinoLoRa.h"
#include <SX1272.h>
#include <SPI.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include <ArduinoJson.h>
#include "SparkFunBME280.h"

static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;

int e;
int anneeG, moisG, jourG, heureG, mntG, secondeG;
float tempC, tempF, pression, altM, altF, hum;
float longitude, latitude;

RTCZero rtc;

//heure initiale
const byte seconds_RTC = 0;
const byte minutes_RTC = 00;
const byte hours_RTC = 17;

const byte day_RTC = 27;
const byte month_RTC = 04;
const byte year_RTC = 16;

BME280 mySensor;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
    Serial.begin(9600);
    
    /*--------------------------------------------------------------------------------------------------------------
* Initialisation RTC
*---------------------------------------------------------------------------------------------------------------*/
  rtc.begin();
  rtc.setTime(hours_RTC, minutes_RTC, seconds_RTC);
  rtc.setDate(day_RTC, month_RTC, year_RTC);    
  
  rtc.setAlarmTime(17, 00, 05);
  rtc.enableAlarm(rtc.MATCH_SS);
  
  rtc.attachInterrupt(alarmMatch);  
/*--------------------------------------------------------------------------------------------------------------
* Initialisation LoRa
*---------------------------------------------------------------------------------------------------------------*/  


  // Print a start message
  Serial.println(F("SX1272 module and Arduino: send packets without ACK"));
  
  // Power ON the module
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e |= sx1272.setMode(4);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  
  // Set header
  e |= sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e |= sx1272.setNodeAddress(3);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));



/*--------------------------------------------------------------------------------------------------------------
* Initialisation BME280
*---------------------------------------------------------------------------------------------------------------*/  
  
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 1;
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;
  delay(10);
  
  Serial.println(mySensor.begin(), HEX);
  
/*---------------------------------------------------------------------------------------
* Initialisation GPS
*-------------------------------------------------------------------------------------------*/
 
  Serial.begin(9600);
  ss.begin(GPSBaud);  

     // rtc.standbyMode();  
}



void loop()
{
  delay(100);
/*--------------------------------------------------------------------------------------------------------------
* Mesure BME280
*---------------------------------------------------------------------------------------------------------------*/      
 uint8_t valeur_reg_ctrl_meas;

  
        valeur_reg_ctrl_meas = mySensor.readRegister(0xF4);                  //sauvegarde de la valeur du registre ctrl_meas
        delay(5);
        valeur_reg_ctrl_meas = valeur_reg_ctrl_meas | 0x01;                  //masque pour passer de sleep mode à forced mode
        delay(5);
        mySensor.writeRegister(BME280_CTRL_MEAS_REG, valeur_reg_ctrl_meas);  //ecriture de la nouvelle valeur dans le registre ctrl_meas
        delay(5);

     tempC=mySensor.readTempC();
     delay(100);
   //  tempF=mySensor.readTempF();
     //delay(10);
     pression=mySensor.readFloatPressure();
//     altM=mySensor.readFloatAltitudeMeters();
  //   altF=mySensor.readFloatAltitudeFeet();
     hum=mySensor.readFloatHumidity();
     Serial.println("g");
     delay(50);

/*--------------------------------------------------------------------------------------------------------------
* Récupération des données GPS annee, mois, jour, heure, mnt, seconde;
*---------------------------------------------------------------------------------------------------------------*/     
 if (gps.location.isValid())
  {
    longitude = gps.location.lng();
    latitude  = gps.location.lat();

  }
  else
  {
    longitude = 0;
    latitude  = 0;
  }
  
  if (gps.time.isValid())
  {
    heureG   = gps.time.hour();
    mntG     = gps.time.minute();
    secondeG = gps.time.second();
  }
  else
  {
    heureG   = 0;
    mntG     = 0;
    secondeG = 0;
  }
  
  if (gps.date.isValid())
  {
    moisG  = gps.date.month();
    jourG  = gps.date.day();
    anneeG = gps.date.year();
  }
  else
  {
    moisG  = 0;
    jourG  = 0;
    anneeG = 0;
  }
 
Serial.println(tempC);
Serial.println(pression);
Serial.println(hum);
Serial.println(longitude);
Serial.println(latitude);
Serial.println(anneeG);
Serial.println(moisG);
Serial.println(jourG);
Serial.println(heureG);
Serial.println(mntG);
Serial.println(secondeG);
/*------------------------------------------------------------------------------------------
* Encodage Json
*------------------------------------------------------------------------------------------*/     
//reservation memoire
StaticJsonBuffer<500> jsonBuffer;
//constructon des objets
JsonObject& data = jsonBuffer.createObject();

//data["capteur"]   = "BME280";
data["tempC"]     = tempC;
//data["tempF"]     = tempF;
data["pression"]  = pression;
//data["altM"]      = altM;
//data["altF"]      = altF;
data["hum"]       = hum;
//data["GPS"]       = "GPS";
data["longitude"] = longitude;
data["latitude"]  = latitude;

JsonArray& dateheure = data.createNestedArray("dateheure");
dateheure.add(anneeG);
dateheure.add(moisG); 
dateheure.add(jourG);
dateheure.add(heureG);
dateheure.add(mntG);
dateheure.add(secondeG);

//generation de la chaine json

char data_json[500];
data.printTo(data_json, sizeof(data_json));

/*--------------------------------------------------------------------------------------------------------------
* transmission LoRa
*---------------------------------------------------------------------------------------------------------------*/  
  e = sx1272.sendPacketTimeout(8, data_json);
  Serial.print(F("Packet sent, state "));
  Serial.println(e, DEC);
       // rtc.standbyMode();
}  

void alarmMatch() 
{
  


}
