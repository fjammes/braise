#include "SX1272.h"
#include <SPI.h>

int etat;

char message1 [] = "ABC123";
char message2 [] = "B";
char my_packet[100];

int longueur_paquet = 0;

const int inter_tx = 7;

void setup() 
{
  pinMode(inter_tx, INPUT);

  SerialUSB.begin(9600);
  SerialUSB.println("etat");
  etat = sx1272.ON();
  Serial.println(etat);
  
  etat |= sx1272.setMode(4);
  Serial.println(etat);
  
  etat |= sx1272.setHeaderON();
  Serial.println(etat);
  
  etat |= sx1272.setChannel(CH_10_868);
  Serial.println(etat);
  
  etat |= sx1272.setCRC_ON();
  Serial.println(etat);
  
  etat |= sx1272.setPower('H');
  Serial.println(etat);
  
  etat |= sx1272.setNodeAddress(3);
  Serial.println(etat);
  
    if (etat == 0)
    Serial.println(F("SX1272 successfully configured"));
  else
    Serial.println(F("SX1272 initialization failed"));
  
}

void loop() 
{
   // Send message1 and print the result
  //longueur_paquet = readRegister(REG_PAYLOAD_LENGTH_LORA);
 // Serial.println(longueur_paquet);
  etat = sx1272.sendPacketTimeout(3, message1);
  Serial.print(F("Packet sent, state "));
  Serial.println(etat, DEC);

  delay(4000);  

  /*  // Send message2 broadcast and print the result
  etat = sx1272.sendPacketTimeout(0, message2);
  Serial.print(F("Packet sent, state "));
  Serial.println(etat, DEC);

  delay(4000);  
*/
 /*etat = sx1272.receivePacketTimeout(10000);
  if ( etat == 0 )
  { 
    Serial.print(F("Receive packet, state "));
    Serial.println(etat, DEC);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    Serial.print(F("Message: "));
    Serial.println(my_packet);
  }
  else {
    Serial.print(F("Receive packet, state "));
    Serial.println(etat, DEC);
  } */
}
