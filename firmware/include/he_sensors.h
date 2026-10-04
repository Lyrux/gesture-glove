#ifndef HE_SENSORS_H
#define HE_SENSORS_H

// put macros here:
/**
 * @brief macro constant definitions
 * 
 * CHANNEL_NUM - 
 * Describes amount of Hall-Effect Sensor analog pins in use.
 * 
 * BUFFER_SIZE - 
 * Describes how many samples to average per sample cycle.
 * 
 * SAMPLE_FREQ - 
 * Describes how often the software interrupts queue a DMA 
 * write to RAM. (NOTE - the current design allows a maximum 
 * period of BUFFER_SIZE*100us. This may be reduced later 
 * dependent on specific hardware implementation.
 * 
 */
#define CHANNEL_NUM 3
#define BUFFER_SIZE (8 * CHANNEL_NUM)
#define SAMPLE_FREQ 100

// global vars
extern int16_t adc_buffer0[BUFFER_SIZE] [[gnu::aligned(4)]]; // double buffer allow simultaneous read/write
extern int16_t adc_buffer1[BUFFER_SIZE] [[gnu::aligned(4)]];
extern volatile bool buffer_ready; // flag to collect DMA data
extern int16_t* volatile completed_buffer; // ptr to the currently unused buffer

// structs
struct HEData {
    int16_t thumb;
    int16_t index;
    int16_t middle;
};

// put function declarations here:
/**
 * @brief Handles read logic in main loop of DMA to RAM.
 * 
 * When the interrupt raises a flag signalling the end of 
 * data collection, starts the sampling process. Creates 
 * a local copy of the buffer to reference and then averages 
 * the most recent readings.
 * 
 * @return HEData struct holds ADC finger values
 */
HEData ReadHESensors(); // in loop DMA RAM write

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
void InitHESensors(); //HE initilization

#endif
