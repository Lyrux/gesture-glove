#include "imu.h"
#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include <Wire.h>
#include <math.h>

namespace {
    /**
     * @brief BNO085 driver instance.
     *
     * A reset pin value of -1 indicates that the reset pin is not being used.
     */
    Adafruit_BNO08x bno08x(BNO08X_RESET_PIN);

    /**
     * @brief Stores the latest event received from the BNO085.
     */
    sh2_SensorValue_t sensor_value;

    /**
     * @brief Stores the latest calculated IMU orientation.
     *
     * Pitch and roll values are expressed in degrees. Both values are initialized to zero
     * until the first valid rotation-vector event is received from the BNO085.
     */
    IMUData imu_data = {
        0.0f, // pitch
        0.0f  // roll
    };

    /**
     * @brief Enables the BNO085 rotation-vector report.
     *
     * The report interval is 100 ms, corresponding to a 10 Hz update rate.
     */
    bool EnableReports() {
        Serial.println("Setting desired reports");
        if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR, IMU_UPDATE_INTERVAL)) {
            Serial.println("Error: Failed to enable rotation-vector report.");
            return false;
        }
        return true;
    }

} // namespace

bool InitIMU() {
    Wire.begin();
    Wire.setClock(100000);

    if (!bno08x.begin_I2C(I2C_ADDRESS, &Wire)) {
        Serial.println("Failed to find BNO08x chip");
        return false;
    }
    Serial.println("BNO08x Found!");

    if (!EnableReports()) {
        return false;
    }

    Serial.println("BNO085 IMU initialized successfully.");
    // delay(100); // Only needed if there's startup timing issues
    return true;
}

bool UpdateIMU() {
    if (bno08x.wasReset()) {
        if (!EnableReports()) {
            return false;
        }
    }

    if (!bno08x.getSensorEvent(&sensor_value)) {
        return false;
    }

    switch (sensor_value.sensorId) {
        // Roll & Pitch calculations
        case SH2_GAME_ROTATION_VECTOR: {
            float w = sensor_value.un.gameRotationVector.real;
            float x = sensor_value.un.gameRotationVector.i;
            float y = sensor_value.un.gameRotationVector.j;
            float z = sensor_value.un.gameRotationVector.k;

            float sinr = 2.0f * (w * x + y * z);
            float cosr = 1.0f - 2.0f * (x * x + y * y);

            float sinp = 2.0f * (w * y - z * x);
            sinp = constrain(sinp, -1.0f, 1.0f);

            imu_data.roll = atan2f(sinr, cosr) * 180.0f / PI;
            imu_data.pitch = asinf(sinp) * 180.0f / PI;
            break;
        }

        default:
            return false;
    }
    return true;
}

IMUData ReadIMU() {
    return imu_data;
}