#include <Arduino.h>
#include "he_sensors.h"
using namespace he_sensors;

namespace he_sensors {
	int16_t adc_buffer0[BUFFER_SIZE] [[gnu::aligned(4)]]; // double buffer allow simultaneous read/write
	int16_t adc_buffer1[BUFFER_SIZE] [[gnu::aligned(4)]];
	volatile bool buffer_ready = false; // flag to collect DMA data
	int16_t* volatile completed_buffer = nullptr; // ptr to the currently unused buffer
}

void InitHESensors() {
	completed_buffer = adc_buffer1; // force buffer0 read on startup (ISR)
	
	__enable_irq(); // interrupt en
	void InitSaadcDmaPpi(void); // local declaration to prevent external visibility
	InitSaadcDmaPpi(); // SAADC setup config (sensor reading is offloaded preventing analogRead stalls)
}

HEData ReadHESensors() {
    HEData data;
    
    // SAADC read logic (SAADC reads and prints once every 10 ms)
	if (buffer_ready) {
		buffer_ready = false; // flag clear
		
		// clean local read (pause interrupts temporarily)
		__disable_irq();
		int16_t* local_buffer;
		local_buffer = completed_buffer;
		__enable_irq();
		
		int32_t ch0_sum = 0, ch1_sum = 0, ch2_sum = 0;
		int samples_per_channel = BUFFER_SIZE / CHANNEL_NUM; // 8 each (buffer size)
		
		for (int i = 0; i < BUFFER_SIZE; i += CHANNEL_NUM)  {
			ch0_sum += local_buffer[i]; // ch0 samples at mod3=0
			ch1_sum += local_buffer[i + 1]; // ch1 samples at mod3=1
			ch2_sum += local_buffer[i + 2]; // ch2 samples at mod3=2
		}
		
		data.thumb = ch0_sum / samples_per_channel; //ch0 avg
		data.index = ch1_sum / samples_per_channel; //ch1 avg
		data.middle = ch2_sum / samples_per_channel; //ch2 avg
	}
    
    return data;
}
