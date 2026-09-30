#ifndef HE_SENSORS_H
#define HE_SENSORS_H

// put macros here:
// analog i/o pin count
#define CHANNEL_NUM 3
// 64 samples/read
#define BUFFER_SIZE 64*CHANNEL_NUM

// global vars
extern alignas(4) int16_t adc_buffer0[BUFFER_SIZE]; // double buffer allow simultaneous read/write
extern alignas(4) int16_t adc_buffer1[BUFFER_SIZE];
extern volatile bool buffer_ready = false; // flag to collect DMA data
extern int16_t* volatile completed_buffer = nullptr; // ptr to the currently unused buffer

// structs
struct HEData {
    int16_t thumb;
    int16_t index;
    int16_t middle;
};

// put function declarations here:
HEData readHESensors(); // in loop DMA RAM write
void init_saadc_dma_ppi(void); // SAADC setup config (sensor reading is offloaded preventing analogRead stalls)


#endif
