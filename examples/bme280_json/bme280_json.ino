/*--------------------------------------------------------------------------------------------------------------
* Ce programme récupère les données provenant du capteur BME280 ainsi que la date et l'heure et les encode au format Json
*---------------------------------------------------------------------------------------------------------------*/

#include <RTCZero.h>
#include <stdint.h>
#include "SparkFunBME280.h"
//#include "Wire.h"
//#include "SPI.h"
#include <Wire.h>
#include <ArduinoJson.h>
//#include <arduinoUtils.h>
//#include "arduinoLoRa.h"
#include <SPI.h>
#include <SX1272.h>

int e;
int Annee, Mois, Jour, Heure, Minutes, Seconde;
float tempC, tempF, pression, altM, altF, hum;
RTCZero rtc;
BME280 mySensor;

//heure initiale
const byte seconds = 0;
const byte minutes = 30;
const byte hours = 15;

const byte day = 06;
const byte month = 9;
const byte year = 16;

int flag_alarme = 0;


void setup() 
{
    // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // Print a start message
  Serial.println(F("SX1272 module configuration in Arduino"));

  // Power ON the module
      // Only allowed in SOCKET 1 (3,3V SPI interface)
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

  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);    
  
  rtc.setAlarmTime(16, 30, 10);
  rtc.enableAlarm(rtc.MATCH_SS);
  
  rtc.attachInterrupt(alarmMatch);  
}

void loop()
{

while(flag_alarme == 1)
{
  
 uint8_t valeur_reg_ctrl_meas;

        valeur_reg_ctrl_meas = mySensor.readRegister(0xF4);                  //sauvegarde de la valeur du registre ctrl_meas
        valeur_reg_ctrl_meas = valeur_reg_ctrl_meas | 0x01;                  //masque pour passer de sleep mode à forced mode
        mySensor.writeRegister(BME280_CTRL_MEAS_REG, valeur_reg_ctrl_meas);  //ecriture de la nouvelle valeur dans le registre ctrl_meas


      Annee=rtc.getYear();
      Serial.print("******** ");
      Serial.print(Annee);
      Serial.print("/");
      Mois=rtc.getMonth();
      Serial.print(rtc.getMonth());
      Serial.print("/");
      Jour=rtc.getYear();
      Serial.print(Jour);
      Serial.print("\t");
  
      Heure=rtc.getHours();  
      Serial.print(Heure);
      Serial.print(":");
      Minutes=rtc.getMinutes();
      Serial.print(Minutes);
      Serial.print(":");
      Seconde=rtc.getSeconds();
      Serial.print(Seconde);
      Serial.print(" ********");
      
  Serial.println();

        tempC=mySensor.readTempC();
	Serial.print("Temperature: ");
	Serial.print(tempC, 2);
	Serial.println(" degrees C");

        tempF=mySensor.readTempF();
	Serial.print("Temperature: ");
	Serial.print(tempF, 2);
	Serial.println(" degrees F");

        pression=mySensor.readFloatPressure();
	Serial.print("Pression: ");
	Serial.print(pression, 2);
	Serial.println(" Pa");

        altM=mySensor.readFloatAltitudeMeters();
	Serial.print("Altitude: ");
	Serial.print(altM, 2);
	Serial.println("m");

        altF=mySensor.readFloatAltitudeFeet();
	Serial.print("Altitude: ");
	Serial.print(altF, 2);
	Serial.println("ft");	

        hum=mySensor.readFloatHumidity();
	Serial.print("%RH: ");
	Serial.print(hum, 2);
	Serial.println(" %");

	Serial.println();

/*------------------------------------------------------------------------------------------
* Encodage Json
*------------------------------------------------------------------------------------------*/
//reservation memoire
StaticJsonBuffer<256> jsonBuffer;
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
dateheure.add(Annee);
dateheure.add(Mois); 
dateheure.add(Jour);
dateheure.add(Heure);
dateheure.add(Minutes);
dateheure.add(Seconde);

//generation de la chaine json

char data_json[256];
data.printTo(data_json, sizeof(data_json));
Serial.println(data_json);
data.printTo(Serial);
Serial.print("\n");

data.prettyPrintTo(data_json, sizeof(data_json));
Serial.println(data_json);
data.prettyPrintTo(Serial);
Serial.print("\n");


	flag_alarme = 0;
	delay(2000); 

}

}

void alarmMatch()   //delay() ne fonctionne pas dans un prog d'interrupt, donc execution du prog d'interrupt dans le while activé par le flag
{
  
flag_alarme = 1;

}



  
