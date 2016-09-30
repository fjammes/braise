/*--------------------------------------------------------------------------------------------------------------
* Ce programme récupère les données provenant du capteur BME280 ainsi que la date et l'heure, les encode au format Json
* et les transmet via LoRa.
*---------------------------------------------------------------------------------------------------------------*/


#include <stdint.h>
#include "SparkFunBME280.h"
#include <Wire.h>
#include <SPI.h>


int e;

float tempC, tempF, pression, altM, altF, hum;

BME280 mySensor;




void setup() 
{

  Serial.begin(9600);


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
  
      
        tempC=mySensor.readTempC();
        delay(10);
        tempF=mySensor.readTempF();
        delay(10);
        pression=mySensor.readFloatPressure();
        altM=mySensor.readFloatAltitudeMeters();
        altF=mySensor.readFloatAltitudeFeet();
        hum=mySensor.readFloatHumidity();
  Serial.print("Temperature: ");    
  Serial.println(tempC);
  Serial.print("Pression: ");
  Serial.println(pression);
  Serial.print("Humidite: ");
  Serial.println(hum);
    Serial.println("");
  delay(5000);




}




  
