/*-----------------------------------------------------------------------------------------------------
* Test de la transmission LoRa sans multiprotocol shield
* ----------------------------------------------------------------------------------------------------*/


#include <Wire.h>
#include <SPI.h>
#include <SX1272.h>




int e;


char my_packet[100];




char message1 [] = "Message";


void setup()
{
 Serial.begin(9600);
 
 
 
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
    
    
}

void loop()
{

  delay(100);
    Serial.println("Boucle principale");    //pour verifier la sortie du mode sleep
  
  e = sx1272.sendPacketTimeout(8, message1);
  Serial.print(F("Packet sent, state "));
  Serial.println(e, DEC);
  
 
}


