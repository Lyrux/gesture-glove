#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Include the Adafruit TinyUSB library for serial functionality
#include "he_sensors.h"

// put macros here:
// analog i/o pin count
#define CHANNEL_NUM 3
// 64 samples/read
#define BUFFER_SIZE 64*CHANNEL_NUM


// put function declarations here:
void init_saadc_dma_ppi(void); // SAADC setup config (sensor reading is offloaded preventing analogRead stalls)
extern "C" void SAADC_IRQHandler(void); // SAADC interrupt


// global vars
alignas(4) int16_t adc_buffer0[BUFFER_SIZE]; // double buffer allow simultaneous read/write
alignas(4) int16_t adc_buffer1[BUFFER_SIZE];
volatile bool buffer_ready = false; // flag to collect DMA data
int16_t* volatile completed_buffer = nullptr; // ptr to the currently unused buffer


void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	
	__enable_irq(); // interrupt en
	init_saadc_dma_ppi();
	
	Serial.println("time_ms,thumb,index,middle");
}

void loop() {
	// put your main code here, to run repeatedly:
	
	unsigned long time = millis();
	
	
	
	// SAADC read logic (SAADC reads and prints once every ~6.4 ms)
	if (buffer_ready) {
		buffer_ready = false; // flag clear
		
		// clean local read (pause interrupts temporarily)
		__disable_irq();
		int16_t* local_buffer;
		local_buffer = completed_buffer;
		__enable_irq();
		
		int32_t ch0_sum = 0, ch1_sum = 0, ch2_sum = 0;
		int samples_per_channel = BUFFER_SIZE / CHANNEL_NUM; // 64 each (buffer size)
		
		for (int i = 0; i < BUFFER_SIZE; i += CHANNEL_NUM)  {
			ch0_sum += local_buffer[i]; // ch0 samples at mod3=0
			ch1_sum += local_buffer[i + 1]; // ch1 samples at mod3=1
			ch2_sum += local_buffer[i + 2]; // ch2 samples at mod3=2
		}
		
		int16_t thumb = ch0_sum / samples_per_channel; //ch0 avg
		int16_t index = ch1_sum / samples_per_channel; //ch1 avg
		int16_t middle = ch2_sum / samples_per_channel; //ch2 avg
		
		Serial.printf("%lu, %d, %d, %d\n", time, thumb, index, middle);
	}
	
}



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

void init_saadc_dma_ppi() {
	NRF_SAADC->ENABLE = SAADC_ENABLE_ENABLE_Enabled; // saadc en
	
	// interrupt config
	NRF_SAADC->INTENSET = (SAADC_INTENSET_END_Msk);
	NVIC_SetPriority(SAADC_IRQn, 3);
	NVIC_EnableIRQ(SAADC_IRQn);
	
	// channel config (expand to channel size)
	NRF_SAADC->CH[0].PSELP = SAADC_CH_PSELP_PSELP_AnalogInput1; // pos ref A0
	NRF_SAADC->CH[0].PSELN = SAADC_CH_PSELN_PSELN_NC; // neg ref ground
	NRF_SAADC->CH[0].CONFIG = (SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos)   |
	                          (SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos)   |
	                          (SAADC_CH_CONFIG_GAIN_Gain1_6    << SAADC_CH_CONFIG_GAIN_Pos)   | 
	                          (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) | 
	                          (SAADC_CH_CONFIG_TACQ_10us       << SAADC_CH_CONFIG_TACQ_Pos)   | 
	                          (SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos);
	
	NRF_SAADC->CH[1].PSELP = SAADC_CH_PSELP_PSELP_AnalogInput2;
	NRF_SAADC->CH[1].PSELN = SAADC_CH_PSELN_PSELN_NC; 
	NRF_SAADC->CH[1].CONFIG = (SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos)   |
	                          (SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos)   |
	                          (SAADC_CH_CONFIG_GAIN_Gain1_6    << SAADC_CH_CONFIG_GAIN_Pos)   | 
	                          (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) | 
	                          (SAADC_CH_CONFIG_TACQ_10us       << SAADC_CH_CONFIG_TACQ_Pos)   | 
	                          (SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos);
	
	NRF_SAADC->CH[2].PSELP = SAADC_CH_PSELP_PSELP_AnalogInput3;
	NRF_SAADC->CH[2].PSELN = SAADC_CH_PSELN_PSELN_NC; 
	NRF_SAADC->CH[2].CONFIG = (SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos)   |
	                          (SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos)   |
 	                          (SAADC_CH_CONFIG_GAIN_Gain1_6    << SAADC_CH_CONFIG_GAIN_Pos)   | 
	                          (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) | 
	                          (SAADC_CH_CONFIG_TACQ_10us       << SAADC_CH_CONFIG_TACQ_Pos)   | 
	                          (SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos);
	
	for (int i = 3; i < 8; i++) {
		NRF_SAADC->CH[i].PSELP = SAADC_CH_PSELP_PSELP_NC; // no connect unused saadc analog pins
	}
	
	// 12-bit resolution
	NRF_SAADC->RESOLUTION = (SAADC_RESOLUTION_VAL_12bit << SAADC_RESOLUTION_VAL_Pos);
	
	// load current and next buffer reads
	NRF_SAADC->RESULT.PTR = (uint32_t)adc_buffer0;
	NRF_SAADC->RESULT.MAXCNT = BUFFER_SIZE;
	NRF_SAADC->TASKS_START = 1; // latch active
	NRF_SAADC->RESULT.PTR = (uint32_t)adc_buffer1; // load next
	
	completed_buffer = adc_buffer1; // force buffer0 read on startup (ISR)
	
	// calibrate
	NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
	while (NRF_SAADC->EVENTS_CALIBRATEDONE == 0);
	NRF_SAADC->EVENTS_CALIBRATEDONE = 0;
	
	//timer1 config
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
	NRF_TIMER1->PRESCALER = 4; // 16 MHz / 2^4 = 1 MHz (1 us/tick)
	NRF_TIMER1->CC[0] = 100; // 100 ticks = 100 us (10 kHz)
	NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk; //ccp clear timer
	
	// samples at ccp event detection
	NRF_PPI->CH[0].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[0];
	NRF_PPI->CH[0].TEP = (uint32_t)&NRF_SAADC->TASKS_SAMPLE;
	NRF_PPI->CHENSET = PPI_CHENSET_CH0_Msk; // short ch0 enable
	
	// timer start
	NRF_TIMER1->TASKS_START = 1;
}
