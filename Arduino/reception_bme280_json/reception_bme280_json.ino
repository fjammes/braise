#include <RTCZero.h>
#include <stdint.h>
//#include "SparkFunBME280.h"
#include "Wire.h"
#include "SPI.h"
#include <Wire.h>
#include <ArduinoJson.h>
#include <arduinoUtils.h>
#include "arduinoLoRa.h"
#include <SPI.h>

RTCZero rtc;

unsigned int i=0;
int e;
//int e;
int annee, mois, jour, heure, minute, seconde;
float tempC, tempF, pression, altM, altF, hum;
RTCZero rtc;
char donnees_recues[256];

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 00;
const byte hours = 17;

/* Change these values to set the current initial date */
const byte day = 25;
const byte month = 04;
const byte year = 16;

void setup()
{

  Serial.begin(9600);
  
  /*--------------------------------------------------------------------------------------------------------------
* Initialisation RTC
*---------------------------------------------------------------------------------------------------------------*/
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(17, 00, 10);
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(alarmMatch);  
  
  
/*--------------------------------------------------------------------------------------------------------------
* Initialisation LoRa
*---------------------------------------------------------------------------------------------------------------*/  

  Serial.println(F("SX1272 module and Arduino: receive packets without ACK"));

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
  e |= sx1272.setNodeAddress(8);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);

  // Print a success message
  if (e == 0){
    Serial.println(F("SX1272 successfully configured"));
    Serial.println("\n");}
  else{
    Serial.println(F("SX1272 initialization failed"));
    Serial.println("\n");}

  rtc.standbyMode();


}

void loop()
{
    delay(100);
  e = sx1272.receivePacketTimeout(10000);
  
    if ( e == 0 )
  {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      donnees_recues[i] = (char)sx1272.packet_received.data[i];
    }
/*------------------------------------------------------------------------------------------
* Decodage Json
*------------------------------------------------------------------------------------------*/

    
}
