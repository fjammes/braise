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
// required before wiring_private.h
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <wiring_private.h>

// Third-party libraries
#include <Adafruit_MCP23017.h>
#include <ArduinoJson.h>
#include <Log.h>
#include "SparkFunBME280.h"
#include "SX1272.h"
#include <TinyGPS++.h>

// Serial for GPS
const unsigned long PIN_GPS_RX = 9;
const unsigned long PIN_GPS_TX = 8;
const SercomRXPad PAD_GPS_RX = SERCOM_RX_PAD_3;
const SercomUartTXPad PAD_GPS_TX = UART_TX_PAD_2;

// GPS data
TinyGPSPlus gps;
float longitude_gps, latitude_gps;
int annee_gps, mois_gps, jour_gps, heure_gps, minute_gps, seconde_gps;

// See https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial
// Comment Serial1, because of conflict on SERCOM0_Handler() callback:
// see line 213 in /home/lora/.platformio/packages/framework-arduinosam/variants/arduino_zero/variant.cpp
Uart SerialGps(&sercom0, PIN_GPS_RX, PIN_GPS_TX, PAD_GPS_RX, PAD_GPS_TX);

void SERCOM0_Handler()
{
  SerialGps.IrqHandler();
}

// BME data
BME280 sensor_BME280;
float temp_bme, press_bme, hum_bme;

char data_json[256];

void _switchLed(const int delayHigh = 1000, const int delayLow = 500);

// the setup function runs once when you press reset or power the board
void setup() {

	const int SERIAL_BAUDRATE = 9600;

	// initialize digital pin LED_BUILTIN as an output.
	pinMode(LED_BUILTIN, OUTPUT);
	LOG_INIT(Log::TRACE, SERIAL_BAUDRATE);

	LOG_TRACE("Start setup()");
	LOG_INFO("** SX1272 module and Arduino: send packets without ACK **");

	_setupExpander();
	//_setupBME280();

	_setupGPS();

	_setupSX1272();

	LOG_TRACE("Finish setup()");

	jsonEncode();
	loraSend();
}

// the loop function runs over and over again forever
void loop() {

	_switchLed();

	//readBME280();

	//SerialUSB.println("Sent ATI to GPS");
	//SerialGps.print("AT\r\n");
	//int read = SerialGps.read();
	//LOG_TRACE("GPS read: %i", read);
	while (SerialGps.available() > 0) {
		char read = SerialGps.read();
		// TODO check log level
		SerialUSB.write(read);
		gps.encode(read);
	}
	// TODO check log level
	SerialUSB.println();
	logGpsData();
	if (millis() > 5000 && gps.charsProcessed() < 10) {
		LOG_FATAL(" BAZOOKA: No GPS detected: check wiring.");
		_switchLedError();
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
	const int BLINK_DELAY = 100;
	for (int i=0; i<4; i++) {
		_switchLed(BLINK_DELAY, BLINK_DELAY);
	}
}

void _setupBME280() {

	LOG_INFO("Set up Expander BME280");

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

void _setupGPS() {

	LOG_INFO("Set up GPS");

	const int GPS_BAUDRATE = 9600;
	SerialGps.begin(GPS_BAUDRATE);

    pinPeripheral(PIN_GPS_RX, PIO_SERCOM_ALT);
    pinPeripheral(PIN_GPS_TX, PIO_SERCOM_ALT);

}

void _setupSX1272() {
    int e;
    LOG_INFO("Set up SX1272");

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

void _setupExpander() {

	// Expander pin number
	enum {
		MCP23017_GPA0,
		MCP23017_GPA1,
		MCP23017_GPA2,
		MCP23017_GPA3,
		MCP23017_GPA4,
		MCP23017_GPA5,
		MCP23017_GPA6,
		MCP23017_GPA7,
		MCP23017_GPB0,
		MCP23017_GPB1,
		MCP23017_GPB2,
		MCP23017_GPB3,
		MCP23017_GPB4,
		MCP23017_GPB5,
		MCP23017_GPB6,
		MCP23017_GPB7,
	};

	const int STABILITY_TIME = 10;

	// Switch on GPS and BME280 using expander, switches
	// and reset
	Adafruit_MCP23017 mcp23017;
	LOG_INFO("Set up Expander MCP23017");
	mcp23017.begin(1);

	LOG_TRACE("Switch on GPS (GPB1)");
	mcp23017.pinMode(MCP23017_GPB1, OUTPUT);
	mcp23017.digitalWrite(MCP23017_GPB1, HIGH);

	LOG_TRACE("Reset GPS (GPB7)");
	mcp23017.pinMode(MCP23017_GPB7, OUTPUT);
	mcp23017.digitalWrite(MCP23017_GPB7, LOW);
	mcp23017.digitalWrite(MCP23017_GPB7, HIGH);

	LOG_TRACE("Switch on BME280 (GPB2)");
	mcp23017.pinMode(MCP23017_GPB2, OUTPUT);
	mcp23017.digitalWrite(MCP23017_GPB2, HIGH);

	//LOG_TRACE("Use writeGPIOAB()");
	// mcp.writeGPIOAB(0xFFFF);

	delay(STABILITY_TIME);
}

void readBME280() {
	// TODO: check stability time and register management

	const int STABILITY_TIME = 10;
	uint8_t valeur_reg_ctrl_meas;

	// record register ctrl_meas
	valeur_reg_ctrl_meas = sensor_BME280.readRegister(BME280_CTRL_MEAS_REG);
	delay(STABILITY_TIME);

	// apply mask to swith from sleep-mode to forced_mode
	valeur_reg_ctrl_meas = valeur_reg_ctrl_meas | 0x01;
	delay(STABILITY_TIME);

	// write new value to register ctrl_meas
	sensor_BME280.writeRegister(BME280_CTRL_MEAS_REG, valeur_reg_ctrl_meas);
	delay(STABILITY_TIME);

	temp_bme = sensor_BME280.readTempC();
	delay(STABILITY_TIME);
	press_bme = sensor_BME280.readFloatPressure();
	delay(STABILITY_TIME);
	hum_bme = sensor_BME280.readFloatHumidity();
	LOG_INFO("temperature: %f °C", temp_bme);
	LOG_INFO("pressure: %f Pa", press_bme);
	LOG_INFO("humidity: %f %", hum_bme);
}

void logGpsData()
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

void jsonEncode()
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
    latlon.add(gps.location.lat(), 6);  // 6 is the number of decimals to print
    latlon.add(gps.location.lng(), 6);

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

void loraSend()
{
    LOG_INFO("Start loraSend()");
    int e = sx1272.sendPacketTimeout(0, data_json);

    LOG_INFO("Packet sent, state: %d", e);
}
