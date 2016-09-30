/*------------------------------------------------------------------------------------------------------------
* Test encodage et decodage JSON
*------------------------------------------------------------------------------------------------------------*/

#include <ArduinoJson.h>

void setup() 
{
  Serial.begin(9600);

/*----------------------------------------------------------------------------------------
* Encodage json
*-----------------------------------------------------------------------------------------*/


//
// reservation espace memoire
//
StaticJsonBuffer<200> jsonBuffer;

//
// création de l'objet JSON
//
JsonObject& root = jsonBuffer.createObject();
root["sensor"] = "gps";
root["time"] = 1351824120;

JsonArray& data = root.createNestedArray("data");
data.add(48.756080, 6);  // 6 is the number of decimals to print
data.add(2.302038, 6);   // if not specified, 2 digits are printed

//
// génération de la trame JSON
//
char data_json[256];
root.printTo(data_json, sizeof(data_json));
Serial.println(data_json);
root.printTo(Serial);
Serial.print("\n");

root.prettyPrintTo(data_json, sizeof(data_json));
Serial.println(data_json);
root.prettyPrintTo(Serial);
Serial.print("\n");
 
 
 
/*-------------------------------------------------------------------------------------------
* Decodage json
*--------------------------------------------------------------------------------------------*/


//
// Reservation espace memoire
//
StaticJsonBuffer<200> jsonBuffer_decod;

//
// Deserialisation de la trame
//
JsonObject& root_decod = jsonBuffer_decod.parseObject(data_json);

if (!root_decod.success())
{
  Serial.println("parseObject() failed");
  return;
}

//
// Récuperation des valeurs et affichage
//
const char* sensor    = root_decod["sensor"];
long        time      = root_decod["time"];
double      latitude  = root_decod["data"][0];
double      longitude = root_decod["data"][1];

Serial.println(sensor);
Serial.println(time);
Serial.println(latitude,6);
Serial.println(longitude,6);

}

void loop()
{


}
