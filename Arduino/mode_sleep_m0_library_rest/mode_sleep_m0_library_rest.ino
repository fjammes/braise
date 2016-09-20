/*********************************************************************************************************************************************************************************
* Mode sleep et reveil par interruption sur broche 8, Utilisation de la librairie rest.h
* Bouton poussoir sur broche 8 (attention aux rebonds)
* Fonctionne
*********************************************************************************************************************************************************************************/  



#include <rest.h>

rest mcu_std;

unsigned int i=0;

void setup() 
{
  //Serial.begin(9600);
  mcu_std.begin(WAKE_EXT_INTERRUPT,8,reveil);  //standby setup for external interrupts
  

}

void loop() 
{

 // Serial.println("Sleeping in 3");
  toggleAndDelay();
  //Serial.println("Sleeping in 2");
  toggleAndDelay();
  //Serial.println("Sleeping in 1");
  toggleAndDelay();
  

  
  mcu_std.standby();  //now mcu go to standby

}


void reveil(void)  
{
  
} 

void toggleAndDelay()
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
