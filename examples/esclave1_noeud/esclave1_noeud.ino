/*--------------------------------------------------------------------------
* reveil de la carte par SX1272
* reception des données
*--------------------------------------------------------------------------*/

#include "SX1272.h"
#include <SPI.h>
#include <rest.h>

const int rxdone = 8;   //interruption de fin de réception sur pin 7
int e;                  //état des fonctions sx1272
const int led = 13;

char my_packet[256];

rest mcu_std;

void setup() 
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
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
  e |= sx1272.setNodeAddress(2);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  if (e == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));   
    

  

  mcu_std.begin(WAKE_EXT_INTERRUPT,rxdone,reveil);
  
  e = sx1272.receive(); 
  Serial.print("state ");
  Serial.println(e);

}

void loop() 
{
  mcu_std.standby();
  delay(500);
     
      e = sx1272.getPacket();
      Serial.println("state ");
      Serial.println(e);
      
      for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    Serial.print(F("Message: "));
    Serial.println(my_packet);    
    

  e = sx1272.receive(); 
  Serial.print("state ");
  Serial.println(e);
  digitalWrite(13, LOW);
  
}

void reveil(void)  
{
  
  digitalWrite(13, HIGH);
  
} 
