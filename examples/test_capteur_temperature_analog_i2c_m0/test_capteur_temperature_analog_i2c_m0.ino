//pour arduino m0

#include <Wire.h>


const int capteur=2;
float tension;
float temperature;
float temperature_i2c_dec;

#define adresse_capteur 0x48

void setup() {
pinMode(capteur, INPUT);
Wire.begin();
Serial.begin(9600);
}

void loop() {
  
  
tension = (analogRead(capteur)*3.3)/4096;
temperature = tension/0.028;


Wire.beginTransmission(adresse_capteur);
Wire.write((byte)0x00);  //rajout de "byte" sinon erreur à la compilation
Wire.endTransmission();

Wire.requestFrom(adresse_capteur,1);
  byte temp_i2c = Wire.read();

  temperature_i2c_dec = (float) temp_i2c;     //conversion binaire-decimal
  if (temp_i2c & 0x80)                        //octet signé => test du 1er bit, si msb=1 alors temperature negative et complement à 2
     {temperature_i2c_dec -= 256;}            //complement à 2

Serial.print("Temperature analog: ");
Serial.print(temperature);
Serial.println("C");

Serial.print("Temperature i2c bin: ");
Serial.println(temp_i2c, BIN);

Serial.print("Temperature i2c: ");
Serial.print(temperature_i2c_dec);
Serial.println("C");
Serial.println("");
delay(2000);



}
