// -*- mode: c++ -*-

#include <Arduino.h>
#include <inttypes.h>
#include <ctype.h>

// required libraries
#include <SPI.h>
#include <EPD_FLASH.h>
#include <EPD_V110_G1.h>
#define SCREEN_SIZE 270
#include <EPD_PANELS.h>
#include <S5813A.h>
#include <EPD_PINOUT.h>

//available: scout cleric dwarf skull yodtail pnj1 pnj2 pnj3 pnj4
// From now on value is always "pnj"
#define IMAGE_1_FILE "/home/yann/Documents/DND5/ArdNanoeInk/pnj.xbm"
#define IMAGE_1_BITS pnj_bits

PROGMEM const
#define unsigned
#define char uint8_t
#include IMAGE_1_FILE
#undef char
#undef unsigned

// LED anode through resistor to I/O pin
// LED cathode to Ground
#define LED_ON  HIGH
#define LED_OFF LOW


// define the E-Ink display
EPD_Class EPD(EPD_SIZE,
	      Pin_PANEL_ON,
	      Pin_BORDER,
	      Pin_DISCHARGE,
#if EPD_PWM_REQUIRED
	      Pin_PWM,
#endif
	      Pin_RESET,
	      Pin_BUSY,
	      Pin_EPD_CS);


// I/O setup
void setup() {
	pinMode(Pin_RED_LED, OUTPUT);
	pinMode(Pin_SW2, INPUT);
	pinMode(Pin_TEMPERATURE, INPUT);
#if EPD_PWM_REQUIRED
	pinMode(Pin_PWM, OUTPUT);
#endif
	pinMode(Pin_BUSY, INPUT);
	pinMode(Pin_RESET, OUTPUT);
	pinMode(Pin_PANEL_ON, OUTPUT);
	pinMode(Pin_DISCHARGE, OUTPUT);
	pinMode(Pin_BORDER, OUTPUT);
	pinMode(Pin_EPD_CS, OUTPUT);
	pinMode(Pin_EPD_FLASH_CS, OUTPUT);

	digitalWrite(Pin_RED_LED, LOW);
#if EPD_PWM_REQUIRED
	digitalWrite(Pin_PWM, LOW);
#endif
	digitalWrite(Pin_RESET, LOW);
	digitalWrite(Pin_PANEL_ON, LOW);
	digitalWrite(Pin_DISCHARGE, LOW);
	digitalWrite(Pin_BORDER, LOW);
	digitalWrite(Pin_EPD_CS, LOW);
	digitalWrite(Pin_EPD_FLASH_CS, HIGH);

	Serial.begin(9600);
	Serial.println("Image 1: " IMAGE_1_FILE);
	Serial.println();

	EPD_FLASH.begin(Pin_EPD_FLASH_CS);
	if (EPD_FLASH.available()) {
		Serial.println("EPD FLASH chip detected OK");
	} else {
		uint8_t maufacturer;
		uint16_t device;
		EPD_FLASH.info(&maufacturer, &device);
		Serial.print("unsupported EPD FLASH chip: MFG: 0x");
		Serial.print(maufacturer, HEX);
		Serial.print("  device: 0x");
		Serial.print(device, HEX);
		Serial.println();
	}

	// configure temperature sensor
	S5813A.begin(Pin_TEMPERATURE);
}

// main loop
void loop() {
	int temperature = S5813A.read();
	Serial.print("Temperature = ");
	Serial.print(temperature, DEC);
	Serial.println(" Celsius");

	EPD.begin(); // power up the EPD panel
	switch (EPD.error()) {
	case EPD_OK:
		Serial.println("EPD: ok");
		break;
	case EPD_UNSUPPORTED_COG:
		Serial.println("EPD: unsuppported COG");
		break;
	case EPD_PANEL_BROKEN:
		Serial.println("EPD: panel broken");
		break;
	case EPD_DC_FAILED:
		Serial.println("EPD: DC failed");
		break;
	}

	EPD.setFactor(temperature); // adjust for current temperature
	EPD.clear();
	EPD.image_0(IMAGE_1_BITS);
	EPD.end();   // power down the EPD panel

	// flash LED for 5 seconds
	for (int x = 0; x < 50; ++x) {
		digitalWrite(Pin_RED_LED, LED_ON);
		delay(5000);
		digitalWrite(Pin_RED_LED, LED_OFF);
		delay(5000);
	}
}
