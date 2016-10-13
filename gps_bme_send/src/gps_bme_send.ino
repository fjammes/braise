/* ----------------------------------------------------------------------------------------
* Recupère les données gps et capteur et les transmet à la carte maitre
* Fonctionne aléatoirement pour l'acquisition de la date, le reste est OK
* -------------------------------------------------------------------------------------------*/

// Standards libraries
#include <stdint.h>

// Third-party libraries
#include <ArduinoJson.h>
#include <Log.h>
#include <SoftwareSerial.h>
#include "SparkFunBME280.h"
#include <SPI.h>
#include "SX1272.h"
#include <TinyGPS++.h>
#include <Wire.h>

static const int RXPin = 13, TXPin = 12;
static const uint32_t GPSBaud = 9600;

char data_json[256];

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;
int e;

BME280 mySensor;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
    int baud_rate = 9600;
    LOG_INIT(Log::TRACE, baud_rate);


    ss.begin(GPSBaud);

    LOG_TRACE("Start setup()");
    LOG_INFO("-- SX1272 module and Arduino: send packets without ACK  --");

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

    if (e == 0)
    {
        LOG_INFO("SX1272 successfully configured");
    }
    else
    {
        LOG_ERROR("SX1272 initialization failed");
    }

    mySensor.settings.commInterface = I2C_MODE;
    mySensor.settings.I2CAddress = 0x76;
    mySensor.settings.runMode = 1;
    mySensor.settings.tStandby = 0;
    mySensor.settings.filter = 0;
    mySensor.settings.tempOverSample = 1;
    mySensor.settings.pressOverSample = 1;
    mySensor.settings.humidOverSample = 1;


    LOG_TRACE("Set up BME");
    delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
    Serial.println(mySensor.begin(), HEX);
    LOG_TRACE("Finish setup()");

}

void loop()
{

    while (ss.available() > 0)
    {
        if (gps.encode(ss.read()))
        {
            mesureBME280();
            displayInfo();
            getInfogps();
            encodage_json();
            transmission_json();
            delay(3000);
        }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        LOG_FATAL("FATAL: No GPS detected: check wiring.");
        while(true);
    }


}

void mesureBME280()
{
    uint8_t valeur_reg_ctrl_meas;


    valeur_reg_ctrl_meas = mySensor.readRegister(0xF4);                  //sauvegarde de la valeur du registre ctrl_meas
    delay(5);
    valeur_reg_ctrl_meas = valeur_reg_ctrl_meas | 0x01;                  //masque pour passer de sleep mode à forced mode
    delay(5);
    mySensor.writeRegister(BME280_CTRL_MEAS_REG, valeur_reg_ctrl_meas);  //ecriture de la nouvelle valeur dans le registre ctrl_meas
    delay(5);

    temp_bme=mySensor.readTempC();
    delay(10);
    press_bme=mySensor.readFloatPressure();
    delay(10);
    hum_bme=mySensor.readFloatHumidity();
    LOG_INFO("temperature: %f °C", temp_bme);
    LOG_INFO("pressure: %f Pa", press_bme);
    LOG_INFO("humidity: %f %", hum_bme);
}

void displayInfo()
{
    if (gps.location.isValid())
    {
        LOG_INFO("Location %f,%f", gps.location.lat(), gps.location.lng());
    }
    else
    {
        LOG_WARN("Location INVALID");
    }

    if (gps.date.isValid())
    {
        LOG_INFO("Date (MM/DD/YYYY): %d/%d/%d", gps.date.month(), gps.date.day(), gps.date.year());
    }
    else LOG_WARN("Date from GPS INVALID");

    if (gps.time.isValid())
    {
        char time[8];
        sprintf(time, "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());
        LOG_INFO("Time: %s", time);
        //Serial.print(F("."));
        //if (gps.time.centisecond() < 10) Serial.print(F("0"));
        //Serial.print(gps.time.centisecond());
    }
    else LOG_WARN("Time INVALID");
}

void getInfogps()
{
    latitude_gps = gps.location.lat();
    longitude_gps = gps.location.lng();
    mois_gps = gps.date.month();
    jour_gps = gps.date.day();
    annee_gps = gps.date.year();
    heure_gps = gps.time.hour();
    minute_gps = gps.time.minute();
    seconde_gps = gps.time.second();

}

void encodage_json()
{
    LOG_DEBUG("Start encodage_json()");
    StaticJsonBuffer<256> jsonBuffer;

//constructon des objets
    JsonObject& data = jsonBuffer.createObject();

    JsonArray& bmedata = data.createNestedArray("bme");
    bmedata.add(temp_bme, 2);  // 6 is the number of decimals to print
    bmedata.add(press_bme);
    bmedata.add(hum_bme);

    JsonArray& latlon = data.createNestedArray("position");
    latlon.add(gps.location.lat()/*latitude_gps*/, 6);  // 6 is the number of decimals to print
    latlon.add(gps.location.lng()/*longitude_gps*/, 6);

    data["annee"] = gps.date.year();//annee_gps;
    data["mois"] = gps.date.month();//mois_gps;
    data["jour"] = gps.date.day();//jour_gps;
    data["heure"] = gps.time.hour();//heure_gps;
    data["minute"] = gps.time.minute();//minute_gps;
    data["seconde"] = gps.time.second();//seconde_gps;

//char data_json[256];
    data.printTo(data_json, sizeof(data_json));
//Serial.println(data_json);
}

void transmission_json()
{
    LOG_DEBUG("Start transmission_json()");
    e = sx1272.sendPacketTimeout(0, data_json);

    LOG_INFO("Packet sent, state: %d", e);
}
