#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Include the Adafruit TinyUSB library for serial functionality
#include "he_sensors.h"
#include "saadc_dma.h"


// global vars
volatile bool buffer_ready = false; // flag to collect DMA data


// put function definitions here:
extern "C" void SAADC_IRQHandler(void) {
	if (NRF_SAADC->EVENTS_END) {
		NRF_SAADC->EVENTS_END = 0; // clear flag
		
		// assign completed buffer to alternate state and load just finished buffer next
		if (completed_buffer == adc_buffer0) {
			completed_buffer = adc_buffer1;
			NRF_SAADC->RESULT.PTR = (uint32_t)adc_buffer1; // queue buffer1 next
		} else {
			completed_buffer = adc_buffer0;
			NRF_SAADC->RESULT.PTR = (uint32_t)adc_buffer0; // queue buffer0 next
		}
		buffer_ready = true;
		NRF_SAADC->TASKS_START = 1;
	}
}
