#ifndef HE_SENSORS_H
#define HE_SENSORS_H

/**
 * @def CHANNEL_NUM
 * @brief Number of Hall-effect sensor channels used.
 */
#define CHANNEL_NUM 3

/**
 * @def BUFFER_SIZE
 * @brief Total number of ADC samples stored in each DMA buffer.
 *
 * With three channels and eight samples per channel, the buffer
 * contains 24 interleaved ADC samples.
 */
#define BUFFER_SIZE (8 * CHANNEL_NUM)

/**
 * @def SAMPLE_FREQ
 * @brief Frequency of each Hall-effect sensor acquisition burst in hertz.
 *
 * RTC2 starts an acquisition burst every 10 ms. The SAADC/PPI hardware
 * performs the sampling; the interrupt runs when the DMA buffer is full.
 */
#define SAMPLE_FREQ 100

// structs
struct HEData {
    int16_t thumb;
    int16_t index;
    int16_t middle;
};

/**
 * @brief Configures registers and enables, and sets initial value(s).
 *
 * Enables interrupts, and gives an initial value to the
 * completed buffer pointer. Runs DMA config function
 * InitSaadcDmaPPi(), a helper function describing specific
 * register values for PPI. This helper does the following:
 *
 * Enables and configures used SAADC channels,
 * calibration and resolution. Sets up interrupt sequence
 * for when SAADC saturates its pointer address. Describes
 * a double buffer sequence to allow simultaneous
 * read/write of data, and queues both buffers. Enables
 * Timer1 and RTC2 to drive PPI and create an on/off duty
 * cycle through CCP shorts..
 *
 */
void InitHESensors(); // HE initilization

/**
 * @brief Handles read logic in main loop of DMA to RAM.
 *
 * When the interrupt raises a flag signalling the end of
 * data collection, starts the sampling process. Creates
 * a local copy of the buffer to reference and then average
 * the most recent readings. Note that all ADC readings are
 * in reference to the MCU internal 0.6V * 6 = 3.6V, despite
 * 3.3V Vdd power supply.
 *
 * @return HEData struct holds ADC finger values
 */
HEData ReadHESensors(); // in loop DMA RAM write

#endif