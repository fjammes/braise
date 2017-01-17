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

// Serial for GPS
const unsigned long PIN_SERIALGPS_RX = 9ul;
const unsigned long PIN_SERIALGPS_TX =  8ul;
const SercomRXPad PAD_SERIALGPS_RX = SERCOM_RX_PAD_3;
const SercomUartTXPad PAD_SERIALGPS_TX = UART_TX_PAD_2;

char data_json[256];

float temp_bme, press_bme, hum_bme;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

BME280 sensor_BME280;

TinyGPSPlus gps;

// See https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial
// Is conflicted with Serial1: see /home/lora/.platformio/packages/framework-arduinosam/variants/arduino_zero/variant.cpp
// l. 213
Uart SerialGps (&sercom0, PIN_SERIALGPS_RX, PIN_SERIALGPS_TX, PAD_SERIALGPS_RX, PAD_SERIALGPS_TX);

void SERCOM0_Handler()
{
  SerialGps.IrqHandler();
}

void _switchLed(const int delayHigh = 1000, const int delayLow = 500);

// the setup function runs once when you press reset or power the board
void setup() {
	const int GPS_BAUDRATE = 9600;
	const int SERIAL_BAUDRATE = 9600;

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	LOG_INIT(Log::TRACE, SERIAL_BAUDRATE);

	LOG_TRACE("Start setup()");
	LOG_INFO("** SX1272 module and Arduino: send packets without ACK **");

	_setupExpander();
	//_setupBME280();

	SerialGps.begin(GPS_BAUDRATE);

	//setupSX1272();

	LOG_TRACE("Finish setup()");
}

// the loop function runs over and over again forever
void loop() {

	_switchLed();

	//readBME280();

	//SerialUSB.println("Sent ATI to GPS");
	//SerialGps.print("AT\r\n");
	//int read = SerialGps.read();
	//LOG_TRACE("GPS read: %i", read);
	if (SerialGps.available() > 0)
	{
		LOG_TRACE("GPS available");
		if (gps.encode(SerialGps.read()))
		{
			displayInfo();
		}
	}
	if (millis() > 5000 && gps.charsProcessed() < 10)
	{
		LOG_FATAL(" BAZOOKA: No GPS detected: check wiring.");
		_switchLedError();
		//while(true);
	}

	delay(3000);
}

void _switchLed(const int delayHigh, const int delayLow) {
	digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
	delay(delayHigh);
	digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
	delay(delayLow);
}

void _switchLedError() {
	const int shortDelay = 100;
	for (int i=0; i<4; i++) {
		_switchLed(100, 100);
	}
}

void _setupBME280() {
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
}

void _setupExpander() {
	const int STABILITY_TIME = 10;

	// Enable GPS en BME280 using expander, switches, en reset
	Adafruit_MCP23017 mcp;
	LOG_TRACE("Set up Expander MCP23017");
	mcp.begin(1);

	const int GPS_PIN_ID = 9;
	LOG_TRACE("Switch on GPS (GPB1)");
	mcp.pinMode(GPS_PIN_ID, OUTPUT);
	mcp.digitalWrite(GPS_PIN_ID, HIGH);

	const int GPS_NRESET_PIN_ID = 15;
	LOG_TRACE("Reset GPS (GPB7)");
	mcp.pinMode(GPS_NRESET_PIN_ID, OUTPUT);
	mcp.digitalWrite(GPS_NRESET_PIN_ID, LOW);
	delay(STABILITY_TIME);
	mcp.digitalWrite(GPS_NRESET_PIN_ID, HIGH);

	const int BME280_PIN_ID = 10;
	LOG_TRACE("Switch on BME280 (GPB2)");
	mcp.pinMode(BME280_PIN_ID, OUTPUT);
	mcp.digitalWrite(BME280_PIN_ID, HIGH);

	//LOG_TRACE("Use writeGPIOAB()");
	mcp.writeGPIOAB(0xFFFF);

	delay(STABILITY_TIME);
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
