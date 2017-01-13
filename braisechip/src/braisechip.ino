/*
 Blink
 Turns on an LED on for one second, then off for one second, repeatedly.

 Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
 it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
 the correct LED pin independent of which board is used.
 If you want to know what pin the on-board LED is connected to on your Arduino model, check
 the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products

 This example code is in the public domain.

 modified 8 May 2014
 by Scott Fitzgerald

 modified 2 Sep 2016
 by Arturo Guadalupi

 modified 8 Sep 2016
 by Colby Newman
 */

// Standards libraries
#include <stdint.h>

// Arduino SDK libraries
// #include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>

// Third-party libraries
#include <Adafruit_MCP23017.h>
#include <ArduinoJson.h>
#include <Log.h>
#include "SparkFunBME280.h"
#include "SX1272.h"
#include <TinyGPS++.h>

static const int RXPin = 13, TXPin = 12;

char data_json[256];

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

BME280 sensor_BME280;

// the setup function runs once when you press reset or power the board
void setup() {
	const uint32_t GPS_BAUDRATE = 9600;

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	int baud_rate = 9600;
	LOG_INIT(Log::TRACE, baud_rate);

	LOG_TRACE("Start setup()");
	LOG_INFO("** SX1272 module and Arduino: send packets without ACK  **");

	_setupExpander();

	Serial1.begin(GPS_BAUDRATE);

	//serialGPS.begin(GPSBaud);
	//setupSX1272();
	if (Serial1.available()) {

	}

	// Configure BME280
	sensor_BME280.settings.commInterface = I2C_MODE;
	sensor_BME280.settings.I2CAddress = 0x76;
	sensor_BME280.settings.runMode = 1;
	sensor_BME280.settings.tStandby = 0;
	sensor_BME280.settings.filter = 0;
	sensor_BME280.settings.tempOverSample = 1;
	sensor_BME280.settings.pressOverSample = 1;
	sensor_BME280.settings.humidOverSample = 1;

	LOG_TRACE("Set up BME280");
	//Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
	uint8_t hex_result = sensor_BME280.begin();
	LOG_TRACE("BME.begin() returned: %x", hex_result);
	LOG_TRACE("Finish setup()");
}

// the loop function runs over and over again forever
void loop() {
	digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
	delay(500);
	digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
	LOG_TRACE("Coucou Log");
	SerialUSB.println("Coucou");
	SerialUSB.println("Coucou 3");
	readBME280();
	delay(100);
}

void _setupExpander() {
	// Enable GPS en BME280 using expander, switches, en reset
	Adafruit_MCP23017 mcp;
	LOG_TRACE("Set up Expander MCP23017");
	mcp.begin(1);

	const int GPS_PIN_ID = 9;
	LOG_TRACE("Switch on GPS (GPB1)");
	mcp.pinMode(GPS_PIN_ID, OUTPUT);
	mcp.digitalWrite(GPS_PIN_ID, HIGH);

	const int GPS_NRESET_PIN_ID = 15;
	LOG_TRACE("Prevent GPS reset (GPB7)");
	mcp.pinMode(GPS_NRESET_PIN_ID, OUTPUT);
	mcp.digitalWrite(GPS_NRESET_PIN_ID, HIGH);

	const int BME280_PIN_ID = 10;
	LOG_TRACE("Switch on BME280 (GPB2)");
	mcp.pinMode(BME280_PIN_ID, OUTPUT);
	mcp.digitalWrite(BME280_PIN_ID, HIGH);

	//LOG_TRACE("Use writeGPIOAB()");
	//mcp.writeGPIOAB(0xFFFF);

	const int POWER_VOLTAGE_STABILITY_TIME = 10;
	delay(POWER_VOLTAGE_STABILITY_TIME);

}

void readBME280() {
	uint8_t valeur_reg_ctrl_meas;

	// record register ctrl_meas
	valeur_reg_ctrl_meas = sensor_BME280.readRegister(BME280_CTRL_MEAS_REG);
	delay(5);

	// apply mask to swith from sleep-mode to forced_mode
	valeur_reg_ctrl_meas = valeur_reg_ctrl_meas | 0x01;
	delay(5);

	// write new value to register ctrl_meas
	sensor_BME280.writeRegister(BME280_CTRL_MEAS_REG, valeur_reg_ctrl_meas);
	delay(5);

	temp_bme = sensor_BME280.readTempC();
	delay(10);
	press_bme = sensor_BME280.readFloatPressure();
	delay(10);
	hum_bme = sensor_BME280.readFloatHumidity();
	LOG_INFO("temperature: %f °C", temp_bme);
	LOG_INFO("pressure: %f Pa", press_bme);
	LOG_INFO("humidity: %f %", hum_bme);
}
