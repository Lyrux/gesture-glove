#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Include the Adafruit TinyUSB library for serial functionality
#include "he_sensors.h"

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	
	__enable_irq(); // interrupt en
	init_saadc_dma_ppi();
	
	Serial.println("time_ms,thumb,index,middle");
}

void loop() {
	// put your main code here, to run repeatedly:
	
	unsigned long time = millis();
	
	HEData data = readHESensors();
	
	Serial.printf("%lu, %d, %d, %d\n", time, data.thumb, data.index, data.middle);
}
