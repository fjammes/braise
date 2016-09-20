/*----------------------------------------------------------------------------------------------------------------------------------------------------
* Test mode sleep et reveil par RTC
* Reception LoRa lorsque la carte est reveillée
*Fonctionne à moitié, le premier message reçu est OK mais les suivants sont incomplets
* Fonctionne si l'emmision ne se fait pas en continu (un envoi chaque minute par exemple)
*-----------------------------------------------------------------------------------------------------------------------------------------------------*/

#include <rest.h>
#include <RTCInt.h>
#include <Wire.h>
#include <arduinoUtils.h>
#include "arduinoLoRa.h"
#include <SPI.h>

rest mcu_std;
RTCInt rtc;

unsigned int i=0;
int e;
int flag =0;
unsigned int h,m,s;
char my_packet[100];

void setup() 
{
  

  Serial.begin(9600); 
  
  /*-------------------------------------------------------------------------------------------------------------
  * Initialisation RTC et mode sleep
  .
  *-------------------------------------------------------------------------------------------------------------*/
  
  rtc.begin(TIME_H24);      //format de l'heure, TIME_H12 pour format 12h
  rtc.setTime(12,0,0,0);      //heure initiale (HH,meridien, mm, ss)      
  rtc.setDate(22,10,15);      //date initiale (JJ,MM,AA)      
  rtc.enableAlarm(SEC,ALARM_INTERRUPT,rest_alarm_int);   // (mode, type d'alarme, programme d'interruption) 
  
                                                         /* mode    OFF / RTC_MODE2_MASK_SEL_OFF alarme désactivée
                                                                    SEC / RTC_MODE2_MASK_SEL_SS déclenchée sur secondes
                                                                    MMSS / RTC_MODE2_MASK_SEL_MMSS déclenchée sur secondes et minutes
                                                                    HHMMSS / RTC_MODE2_MASK_SEL_HHMMSS déclenchée sur secondes, minutes et heures
                                                                    DDHHMMSS / RTC_MODE2_MASK_SEL_DDHHMMSS déclenchée sur secondes, minutes, heures et jours
                                                                    MMDDHHMMSS / RTC_MODE2_MASK_SEL_MMDDHHMMSS déclenchée sur secondes, minutes, heures, jours et mois
                                                                    YYMMDDHHMMSS / RTC_MODE2_MASK_SEL_YYMMDDHHMMSS déclenchée sur secondes, minutes, heures, jours, mois et année
										 	
				                            type d'alarme    ALARM_POLLED     alarme géré par le code
									     ALARM_INTERRUPT  l'alarme génére une interruption lorsqu'elle se déclenche
									
				   
				                            programme d'interruption    NULL si mode ALARM_POLLED)*/		
/*--------------------------------------------------------------------------------------------------------------
* réglage alarme
---------------------------------------------------------------------------------------------------------------*/
  rtc.time.hour=12;      
  rtc.time.minute=0;
  rtc.time.second=20;
  
  
  mcu_std.begin(WAKE_RTC_ALARM);      //validation du mode sleep par reveil RTC
  rtc.setAlarm();      //activation de l'alarme
 
 /*--------------------------------------------------------------------------------------------------------------
* initialisation LoRa
---------------------------------------------------------------------------------------------------------------*/
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
}

void loop() 
{
  
  
  mcu_std.standby();      //mise en veille
  
  delay(500);    //du temps pour se reveiller
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
     
mcu_std.standby();       


}

void rest_alarm_int(void)
{

} 
