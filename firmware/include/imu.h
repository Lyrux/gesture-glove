#ifndef IMU_H
#define IMU_H

#ifndef HIDE_MACROS
/**
 * @def BNO08X_RESET_PIN
 * @brief Reset pin assigned to the BNO085.
 *
 * A value of -1 indicates that no dedicated hardware reset pin is used.
 */
#define BNO08X_RESET_PIN -1

/**
 * @def I2C_ADDRESS
 * @brief I2C address of the BNO085.
 *
 * The default BNO085 I2C address is 0x4A. The alternate address is 0x4B.
 */
#define I2C_ADDRESS 0x4A

/**
 * @def IMU_UPDATE_INTERVAL
 * @brief BNO085 sensor report interval in microseconds.
 *
 * 100,000 microseconds corresponds to a 100 ms interval, or a 10 Hz
 * update rate.
 */
#define IMU_UPDATE_INTERVAL 100000
#endif

/**
 * @brief Stores the current orientation measured by the BNO085.
 */
struct IMUData {
    float pitch; // Rotation about the lateral axis, in degrees.
    float roll;  // Rotation about the longitudinal axis, in degrees.
};

/**
 * @brief Initializes the BNO085 over the I2C interface.
 *
 * Configures the XIAO's default I2C bus, initializes the BNO085,
 * and enables the rotation-vector report.
 *
 * @return true if initialization and report configuration succeed,
 *         false otherwise.
 */
bool InitIMU();

/**
 * @brief Reads and processes the newest BNO085 sensor event.
 *
 * Updates the internal pitch and roll values when a new rotation-vector
 * event is available.
 *
 * @return true if a new rotation-vector event was processed;
 *         false otherwise.
 */
bool UpdateIMU();

/**
 * @brief Returns the most recently processed IMU orientation.
 *
 * @return The latest pitch and roll values in degrees.
 */
IMUData ReadIMU();

#endif // IMU_H