/*----------------------------------------------------------------------------------------------------------------------------------------------------
* Ce programme reçoit un ordre de la carte master
* Cet ordre permet de declencher la mesure capteur et de transmettre les infos
* Fonctionnement recherché: REVEILLER la carte lorsque le bon ordre est recu 
* ---------------------------------------------------------------------------------------------------------------------------------------------------*/
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

//void mesureBME280(void);
//void encodage_json(void);
BME280 mySensor;
char ordre[10];
char data_json[256];
float tempC_mesure, pression_mesure, hum_mesure;
int e;

void setup() 
{
  Serial.begin(9600);

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
        Serial.println("En attente d'ordre...");
}


void loop()
{

  e = sx1272.receivePacketTimeout(20000);
  if ( e == 0 )
  {
    Serial.print(F("Paquet recu, state "));
    Serial.println(e, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      ordre[i] = (char)sx1272.packet_received.data[i];
    }
      if(strcmp(ordre,"go"))
       {Serial.println("Mauvais ordre recu A");
       }
       else
       {
       Serial.println("Ordre 'GO' recu, mesure...");
       mesureBME280();
   
       e = sx1272.sendPacketTimeout(0, data_json);
       Serial.println("transmission data_json");
       Serial.print(F("Packet sent, state "));
       Serial.println(e, DEC);
       }
         
      if(strcmp(ordre,"stop"))
      { Serial.println("Mauvais ordre recu B"); 
      }
      else
      {Serial.println("Ordre 'STOP' recu");
      }  

  }
  else {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);
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
  
        tempC_mesure=mySensor.readTempC();
        delay(10);
        pression_mesure=mySensor.readFloatPressure();
        delay(10);
        hum_mesure=mySensor.readFloatHumidity();
        
        encodage_json();
}  

void encodage_json()
{
  
StaticJsonBuffer<200> jsonBuffer;
//constructon des objets
JsonObject& data = jsonBuffer.createObject();

data["tempC"] = tempC_mesure;
data["pression"] = pression_mesure;
data["hum"] = hum_mesure;

//char data_json[256];
data.printTo(data_json, sizeof(data_json));  
  
}  
