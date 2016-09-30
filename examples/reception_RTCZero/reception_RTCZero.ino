/*---------------------------------------------------------------------------------------------------------------------------------------------
* Test de la reception LoRa combinée au mode sleep avec reveil par rtc, library RTCZero.h
* Fonctionne avec le programme transmission_RTCZero
*----------------------------------------------------------------------------------------------------------------------------------------------*/

#include <RTCZero.h>
#include <Wire.h>
#include <arduinoUtils.h>
#include "arduinoLoRa.h"
#include <SPI.h>

RTCZero rtc;

unsigned int i=0;
int e;
int flag =0;
unsigned int h,m,s;
char my_packet[100];

// heure initiale
const byte seconds = 0;
const byte minutes = 00;
const byte hours = 17;

// date initiale
const byte day = 25;
const byte month = 04;
const byte year = 16;



void setup() 
{
  Serial.begin(9600);
  
  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(17, 00, 30);
  rtc.enableAlarm(rtc.MATCH_SS);

  rtc.attachInterrupt(alarmMatch);
  
 /*--------------------------------------------------------------------------------------------------------------
* initialisation LoRa
* ---------------------------------------------------------------------------------------------------------------*/
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
  delay(100);    //du temps pour se reveiller
  Serial.println("Boucle principale");    //pour verifier la sortie du mode sleep
  
  
  //delay(500);  
  e = sx1272.receivePacketTimeout(10000);

  if ( e == 0 )
  {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    delay(100);
    Serial.print(F("Message: "));
    delay(100);
    Serial.println(my_packet);
    Serial.println(my_packet[0]);
    Serial.println(my_packet[1]);
    Serial.println(my_packet[2]);
    Serial.println(my_packet[3]);
    Serial.println(my_packet[4]);
    Serial.println(my_packet[5]);
    Serial.println(my_packet[6]);
    Serial.println(my_packet[7]);
    Serial.println(my_packet[8]);
    Serial.println(my_packet[9]);
    //char my_packet[100] = "";
    /*s=rtc.getSecond();
    m=rtc.getMinute();
    h=rtc.getHour();
    Serial.print(h);
    Serial.print(":");
    Serial.print(m);
    Serial.print(":");
    Serial.print(s);
    Serial.println("\n");*/

  }
  else {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);  
       }
     

rtc.standbyMode();

}



void alarmMatch()
{

}
