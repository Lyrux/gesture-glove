#include "imu.h"

#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <Wire.h>
#include <math.h>

namespace
{

    /**
     * @brief BNO085 driver instance.
     *
     * A reset pin value of -1 indicates that the reset pin is not being used.
     */
    Adafruit_BNO08x BNO08X(-1);

    /**
     * @brief Stores the latest event received from the BNO085.
     */
    sh2_SensorValue_t SENSOR_VALUE;

    /**
     * @brief Stores the most recently calculated orientation.
     */
    IMUData IMU_DATA = {
        .pitch = 0.0f,
        .roll = 0.0f};

    /**
     * @brief Enables the BNO085 rotation-vector report.
     *
     * The report interval is 50 ms, corresponding to a 20 Hz update rate.
     */
    void EnableReports() {
        if (!BNO08X.enableReport(SH2_ROTATION_VECTOR, 50000)) {
            Serial.println("Failed to enable rotation-vector report.");
        }
    }

} // namespace

void InitIMU() {
    Wire.begin();

    if (!BNO08X.begin_I2C(0x4A, &Wire)) {
        Serial.println("Failed to initialize BNO085 IMU.");

        while (true) {
            delay(100);
        }
    }

    EnableReports();

    Serial.println("BNO085 IMU initialized successfully.");
}

bool UpdateIMU() {
    if (BNO08X.wasReset()) {
        EnableReports();
    }

    if (!BNO08X.getSensorEvent(&SENSOR_VALUE)) {
        return false;
    }

    if (SENSOR_VALUE.sensorId != SH2_ROTATION_VECTOR) {
        return false;
    }

    float i = SENSOR_VALUE.un.rotationVector.i;
    float j = SENSOR_VALUE.un.rotationVector.j;
    float k = SENSOR_VALUE.un.rotationVector.k;
    float real = SENSOR_VALUE.un.rotationVector.real;

    IMU_DATA.roll = atan2(
                        2.0f * (real * i + j * k),
                        1.0f - 2.0f * (i * i + j * j)) * RAD_TO_DEG;

    IMU_DATA.pitch = asin(
                        constrain(2.0f * (real * j - k * i), -1.0f, 1.0f)) * RAD_TO_DEG;

    return true;
}

IMUData ReadIMU() {
    return IMU_DATA;
}