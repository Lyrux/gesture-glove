#ifndef IMU_H
#define IMU_H

/**
 * @brief Stores the current orientation measured by the BNO085.
 */
struct IMUData
{
    float pitch; /**< Rotation about the lateral axis, in degrees. */
    float roll;  /**< Rotation about the longitudinal axis, in degrees. */
};

/**
 * @brief Initializes the BNO085 over the I2C interface.
 *
 * Configures the XIAO's default I2C bus and enables the BNO085
 * rotation-vector report.
 */
void InitIMU();

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