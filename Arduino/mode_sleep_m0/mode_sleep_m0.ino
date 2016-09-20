/*--------------------------------------------------------------------------------- 
*              Test de mode sleep et reveil par front sur bus SPI (MOSI)
* Simulation du front par bouton poussoir 
*----------------------------------------------------------------------------------*/ 


void setup()
{
  Serial.begin(9600);
  pinMode(10, OUTPUT);  // Led pin 13
  
  attachInterrupt(21u, reveil, RISING); //activation interruption lors d'un front montant sur MOSI (pin 18u, voir assignation des pins dans le fichier variant.h)
  
  SCB->SCR |= 1<<2; // validation du mode STANDBY, bit DEEPSLEEP du registre SCR (System Control Register à 1), voir doc atmel samd21/power management
  

  EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN12; //interruption sur MOSI gérée par EXTINT[12] du EIC (External Interrupt Controleur, voir tableau p.34 doc samd21)
}

void loop()
{
  Serial.println("Sleeping in 3");
  toggleAndDelay();
  Serial.println("Sleeping in 2");
  toggleAndDelay();
  Serial.println("Sleeping in 1");
  toggleAndDelay();
  __WFI(); // instruction ARM, Wait For Interrupt permet d'entrer en mode sleep et d'y rester jusqu'a ce qu'une interruption soit declenchée
}


void reveil()
{

}


void toggleAndDelay()
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}
