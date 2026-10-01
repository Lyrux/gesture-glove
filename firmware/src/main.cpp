#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Include the Adafruit TinyUSB library for serial functionality
#include "he_sensors.h"

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	
	initHESensors();
	
	Serial.println("time_ms,thumb,index,middle");
}

void loop() {
	// put your main code here, to run repeatedly:
	__WFE();

	unsigned long time = millis();
	
	HEData data = readHESensors();
	
	Serial.printf("%lu, %d, %d, %d\n", time, data.thumb, data.index, data.middle);
}
