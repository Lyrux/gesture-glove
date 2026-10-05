#include <Arduino.h>
#include "he_sensors.h"
using namespace he_sensors;

/**
 * @brief Interrupt Service Routine driving SAADC double buffer scheme.
 * 
 * Handles pointer swapping between the two buffer addresses. 
 * Completed buffer pointer is changed to the buffer that just 
 * finished being written to. NFR_SAADC->RESULT.PTR queues the 
 * buffer that just finished to after the currently running 
 * one. Raises flag to signal to main loop that data is available.
 * 
 */
extern "C" void SAADC_IRQHandler(void) { // ISR CASE SENSITIVE DO NOT PASCAL CASE
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
