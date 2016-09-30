/*--------------------------------------------------------------------------------------------------------------
* Ce programme récupère les données provenant du capteur BME280 ainsi que la date et l'heure, les encode au format Json
* et les transmet via LoRa.
*---------------------------------------------------------------------------------------------------------------*/

#include <RTCZero.h>
#include <stdint.h>
#include "SparkFunBME280.h"
//#include "Wire.h"
#include <Wire.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SX1272.h>
//#include <Streaming.h>




int e;
int annee, mois, jour, heure, minute, seconde;
float tempC, tempF, pression, altM, altF, hum;
RTCZero rtc;
BME280 mySensor;

//heure initiale
const byte seconds = 0;
const byte minutes = 00;
const byte hours = 17;

const byte day = 26;
const byte month = 04;
const byte year = 16;



void setup() 
{

  Serial.begin(9600);
while (!Serial) 
 
/*--------------------------------------------------------------------------------------------------------------
* Initialisation RTC
*---------------------------------------------------------------------------------------------------------------*/
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);    
  
  rtc.setAlarmTime(16, 30, 10);
  rtc.enableAlarm(rtc.MATCH_SS);
  
  rtc.attachInterrupt(alarmMatch);  
  
/*--------------------------------------------------------------------------------------------------------------
* Initialisation LoRa
*---------------------------------------------------------------------------------------------------------------*/  
//Serial << "Test" << endl;

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
  

	Serial.print("Program Started\n");
	Serial.print("Starting BME280... result of .begin(): 0x");

	delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
	Serial.println(mySensor.begin(), HEX);


delay(20);

   

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
        
/*--------------------------------------------------------------------------------------------------------------
* Récupération des données
*---------------------------------------------------------------------------------------------------------------*/  
      annee=rtc.getYear();
      mois=rtc.getMonth();
      jour=rtc.getDay();
      heure=rtc.getHours();  
      minute=rtc.getMinutes();
      seconde=rtc.getSeconds();
      
        tempC=mySensor.readTempC();
        delay(10);
        tempF=mySensor.readTempF();
        delay(10);
        pression=mySensor.readFloatPressure();
        altM=mySensor.readFloatAltitudeMeters();
        altF=mySensor.readFloatAltitudeFeet();
        hum=mySensor.readFloatHumidity();


/*------------------------------------------------------------------------------------------
* Encodage Json
*------------------------------------------------------------------------------------------*/
//reservation memoire
StaticJsonBuffer<200> jsonBuffer;
//constructon des objets
JsonObject& data = jsonBuffer.createObject();

data["capteur"] = "BME280";
data["tempC"] = tempC;
data["tempF"] = tempF;
data["pression"] = pression;
data["altM"] = altM;
data["altF"] = altF;
data["hum"] = hum;

JsonArray& dateheure = data.createNestedArray("dateheure");
dateheure.add(annee);
dateheure.add(mois); 
dateheure.add(jour);
dateheure.add(heure);
dateheure.add(minute);
dateheure.add(seconde);

//generation de la chaine json

char data_json[256];
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



  
