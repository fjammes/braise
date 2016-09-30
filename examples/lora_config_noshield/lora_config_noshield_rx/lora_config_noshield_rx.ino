#include "SX1272.h"
#include <SPI.h>

int e;

char message1 [] = "a";
char message2 [] = "B";
char my_packet[256];

int longueur_paquet = 0;

const int inter_tx = 7;
const int led = 13;

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
  
}

void loop() 
{
  /* // Send message1 and print the result
  etat = sx1272.sendPacketTimeout(8, message1);
  Serial.print(F("Packet sent, state "));
  Serial.println(etat, DEC);

  delay(4000);  */

  /*  // Send message2 broadcast and print the result
  etat = sx1272.sendPacketTimeout(0, message2);
  Serial.print(F("Packet sent, state "));
  Serial.println(etat, DEC);

  delay(4000);  
*/

 e = sx1272.receivePacketTimeout(10000);
  if ( e == 0 )
  {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      //longueur_paquet = sx1272.packet_received.length;
      //Serial.println(longueur_paquet);
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    Serial.print(F("Message: "));
    Serial.println(my_packet);
  }
  else {
    Serial.print(F("Receive packet, state "));
    Serial.println(e, DEC);
  }
}

