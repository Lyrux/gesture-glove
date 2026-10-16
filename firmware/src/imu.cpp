#include "imu.h"
#include <Arduino.h>
#include <Adafruit_BNO08x.h>

void initIMU() {
    Adafruit_BNO08x imu = Adafruit_BNO08x();
    if (!imu.begin_I2C()) {
        Serial.println("Failed to initialize BNO08x IMU!");
        while (1);
    }
    Serial.println("BNO08x IMU initialized successfully.");
}
