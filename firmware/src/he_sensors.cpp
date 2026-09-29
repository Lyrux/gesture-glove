#include <Arduino.h>
#include "he_sensors.h"

// Pinouts
const int THUMB_PIN = A0;
const int INDEX_PIN = A1;
const int MIDDLE_PIN = A2;

void initHESensors()
{
    // 12-bit ADC resolution (0-4095)
    analogReadResolution(12);
}

HEData readHESensors()
{
    HEData data;

    data.thumb = analogRead(THUMB_PIN);
    data.index = analogRead(INDEX_PIN);
    data.middle = analogRead(MIDDLE_PIN);

    return data;
}