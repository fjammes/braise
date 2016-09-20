  /* ----------------------------------------------------------------------------------------
* Reveil esclave 1 puis esclave 2 (esclave1_noeud.ino, esclave2_noeud.ino)
* 
* -------------------------------------------------------------------------------------------*/

#include "SPI.h"
#include <Wire.h>
#include "SX1272.h"

int e;

char ordre1 [] = "reveil_1";
char ordre2 [] = "reveil_2";

void setup()
{
  Serial.begin(9600);
 
  Serial.println("Demarrage..."); 
 
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
  e |= sx1272.setNodeAddress(1);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));


  
}

void loop()
{
delay(5000);
transmission_esclave1(); 
transmission_esclave2();


}





void transmission_esclave1()
{
  //delay(5000);     
  e = sx1272.sendPacketTimeout(8, ordre1);
       Serial.println("transmission ordre reveil esclave 1");
       Serial.print(F("Packet sent, state "));
       Serial.println(e, DEC);
  
}

void transmission_esclave2()
{
  //delay(5000);     
  e = sx1272.sendPacketTimeout(9, ordre2);
       Serial.println("transmission ordre reveil esclave 2");
       Serial.print(F("Packet sent, state "));
       Serial.println(e, DEC);
  
}  
