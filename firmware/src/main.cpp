#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Include the Adafruit TinyUSB library for serial functionality
#include "he_sensors.h"

// put function declarations here:


// pinouts
const int THUMB_PIN = A0;
const int INDEX_PIN = A1;
const int MIDDLE_PIN = A2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // default baud rate is 115200 bps

  InitHESensors();

  Serial.println("time_ms,thumb,index,middle");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long time = millis();

  HEData data = ReadHESensors();

  Serial.print(time);
  Serial.print(",");
  Serial.print(data.thumb);
  Serial.print(",");
  Serial.print(data.index);
  Serial.print(",");
  Serial.println(data.middle);

  delay(100); // 100 ms delay between readings
}

// put function definitions here:
