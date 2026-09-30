#ifndef SAADC_DMA_H
#define SAADC_DMA_H

// put macros here:
// analog i/o pin count
#define CHANNEL_NUM 3
// 64 samples/read
#define BUFFER_SIZE 64*CHANNEL_NUM


// put function declarations here:
void init_saadc_dma_ppi(void); // SAADC setup config (sensor reading is offloaded preventing analogRead stalls)

// global vars
extern alignas(4) int16_t adc_buffer0[BUFFER_SIZE]; // double buffer allow simultaneous read/write
extern alignas(4) int16_t adc_buffer1[BUFFER_SIZE];
extern volatile bool buffer_ready = false; // flag to collect DMA data
extern int16_t* volatile completed_buffer = nullptr; // ptr to the currently unused buffer

#endif
