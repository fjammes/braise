// Standards libraries

// Third-party libraries
#include <ArduinoJson.h>
#include <Log.h>
#include <SPI.h>
#include "SX1272.h"
#include <Wire.h>

char ordre_acqui [] = "ACQUI";

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

void setup()
{
    int baud_rate = 9600;
    LOG_INIT(Log::TRACE, baud_rate);
    Serial.begin(9600);

    LOG_TRACE("Start setup()");
    LOG_INFO("** SX1272 module and Arduino: receive packets without ACK  **");

    setupSX1272();

}

/**
 * Setup Lora hardware
 */
void setupSX1272() {
    int e;
    e = sx1272.ON();
    LOG_TRACE("Set power ON: state %d", e);

    e |= sx1272.setMode(4);
    LOG_TRACE("Set Mode: state %d", e);

    e |= sx1272.setHeaderON();
    LOG_TRACE("Set Header ON: state %d", e);

    e |= sx1272.setChannel(CH_10_868);
    LOG_TRACE("Set Channel: state %d", e);

    e |= sx1272.setCRC_ON();
    LOG_TRACE("Set CRC ON: state %d", e);

    e |= sx1272.setPower('H');
    LOG_TRACE("Setting Power: state %d", e);

    e |= sx1272.setNodeAddress(3);
    LOG_TRACE("Set node address: state %d", e);

    if (e == 0) {
        LOG_INFO("SX1272 successfully configured");
    } else {
        LOG_ERROR("SX1272 initialization failed");
    }
}

void loop()
{
	int e;
	char donnees_recues[256];

    e = sx1272.receivePacketTimeout(10000);
    if (e==0)
    {
    	LOG_DEBUG("Receive packet, state %d", e);
        for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
        {
            donnees_recues[i] = (char)sx1272.packet_received.data[i];
        }
    }

    StaticJsonBuffer<256> donnees_recues_decodage;

//deserialisation

    JsonObject& donnees_decod = donnees_recues_decodage.parseObject(donnees_recues);

    if (!donnees_decod.success())
    {
    	LOG_ERROR("parseObject() failed");
        return;
    }

    char data_json[256];
    donnees_decod.printTo(data_json, sizeof(data_json));
    LOG_DEBUG("json: %s", data_json);


//récuperation des données
    annee_gps         = donnees_decod["annee"];
    mois_gps          = donnees_decod["mois"];
    jour_gps          = donnees_decod["jour"];
    heure_gps         = donnees_decod["heure"];
    minute_gps        = donnees_decod["minute"];
    seconde_gps       = donnees_decod["seconde"];
    latitude_gps      = donnees_decod["position"][0];
    longitude_gps     = donnees_decod["position"][1];
    temp_bme          = donnees_decod["bme"][0];
    press_bme         = donnees_decod["bme"][1];
    hum_bme           = donnees_decod["bme"][2];

    affichage();

//delay(10000);
}

void affichage(void)
{
    LOG_INFO("------- GPS -------");
    LOG_INFO("Date (MM/DD/YYYY): %d/%d/%d", mois_gps, jour_gps, annee_gps);
    char time[8];
    sprintf(time, "%02d:%02d:%02d", heure_gps, minute_gps, seconde_gps);
    LOG_INFO("Time: %s", time);
    LOG_INFO("Location %f,%f", latitude_gps, longitude_gps);


    LOG_INFO("------- Capteur T/P/H -------");
    LOG_INFO("temperature: %f °C", temp_bme);
    LOG_INFO("pressure: %f Pa", press_bme);
    LOG_INFO("humidity: %f %", hum_bme);
}

