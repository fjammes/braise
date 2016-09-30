void setup() 
{
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
  while (!SerialUSB);
  //{
    // Serial.println("Test native USB");

  //}
}

void loop() {
  // put your main code here, to run repeatedly:
  //  SerialUSB.write("Test native USB");
  SerialUSB.println("Test native USB");

}
