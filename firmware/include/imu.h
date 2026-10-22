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
 * The default BNO085 I2C address is 0x4A.
 * The alternate address is 0x4B (by pulling the DI pin high to VCC).
 */
#define I2C_ADDRESS 0x4A

/**
 * @def IMU_UPDATE_INTERVAL
 * @brief BNO085 sensor report interval in microseconds.
 *
 * 100,000 microseconds corresponds to a 100 ms interval, or a requested update rate of 10 Hz.
 */
#define IMU_UPDATE_INTERVAL 100000
#endif

/**
 * @brief Stores the most recently processed BNO085 orientation.
 *
 * Pitch and roll are expressed in degrees.
 */
struct IMUData {
    float pitch; // Rotation about the lateral axis, in degrees.
    float roll;  // Rotation about the longitudinal axis, in degrees.
};

/**
 * @brief Initializes the BNO085 over I2C.
 *
 * Initializes the default I2C bus, establishes communication
 * with the BNO085, and enables the game rotation-vector report.
 *
 * @return true if initialization and report configuration succeed,
 *         false otherwise.
 */
bool InitIMU();

/**
 * @brief Processes an available BNO085 sensor event.
 *
 * Updates the stored pitch and roll values when a game rotation-vector report is received.
 *
 * @return true if a rotation-vector report was processed,
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