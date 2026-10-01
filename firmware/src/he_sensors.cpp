#include <Arduino.h>
#include "he_sensors.h"

// Pinouts
const int THUMB_PIN = A0;
const int INDEX_PIN = A1;
const int MIDDLE_PIN = A2;

void InitHESensors() {
    // 12-bit ADC resolution (0-4095)
    analogReadResolution(12);
}

HEData ReadHESensors() {
    HEData data;

    data.thumb = analogRead(THUMB_PIN);
    data.index = analogRead(INDEX_PIN);
    data.middle = analogRead(MIDDLE_PIN);

    return data;
}