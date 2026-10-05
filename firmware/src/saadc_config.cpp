#include <Arduino.h>
#include "he_sensors.h"
using namespace he_sensors;

void InitSaadcDmaPpi() {
	NRF_SAADC->ENABLE = SAADC_ENABLE_ENABLE_Enabled; // saadc en
	
	// interrupt config
	NRF_SAADC->INTENSET = (SAADC_INTENSET_END_Msk);
	NVIC_SetPriority(SAADC_IRQn, 3);
	NVIC_EnableIRQ(SAADC_IRQn);
	
	// channel config (expand to channel size)
	for (int i = 0; i < HES_CHANNEL_NUM; i++) {
	NRF_SAADC->CH[i].PSELP = SAADC_CH_PSELP_PSELP_AnalogInput1 + i; // sets ADC channels to pins sequentially
	
	NRF_SAADC->CH[i].PSELN = SAADC_CH_PSELN_PSELN_NC; 
	NRF_SAADC->CH[i].CONFIG = (SAADC_CH_CONFIG_RESP_Bypass     << SAADC_CH_CONFIG_RESP_Pos)   | 
	                          (SAADC_CH_CONFIG_RESN_Bypass     << SAADC_CH_CONFIG_RESN_Pos)   | 
 	                          (SAADC_CH_CONFIG_GAIN_Gain1_6    << SAADC_CH_CONFIG_GAIN_Pos)   | 
	                          (SAADC_CH_CONFIG_REFSEL_Internal << SAADC_CH_CONFIG_REFSEL_Pos) | 
	                          (SAADC_CH_CONFIG_TACQ_10us       << SAADC_CH_CONFIG_TACQ_Pos)   | 
	                          (SAADC_CH_CONFIG_MODE_SE         << SAADC_CH_CONFIG_MODE_Pos);
	}

	for (int i = HES_CHANNEL_NUM; i < 8; i++) {
		NRF_SAADC->CH[i].PSELP = SAADC_CH_PSELP_PSELP_NC; // no connect unused saadc analog pins
	}
	
	// 12-bit resolution
	NRF_SAADC->RESOLUTION = (SAADC_RESOLUTION_VAL_12bit << SAADC_RESOLUTION_VAL_Pos);
	
	// load current and next buffer reads
	NRF_SAADC->RESULT.PTR = (uint32_t)adc_buffer0;
	NRF_SAADC->RESULT.MAXCNT = HES_BUFFER_SIZE;
	NRF_SAADC->TASKS_START = 1; // latch active
	NRF_SAADC->RESULT.PTR = (uint32_t)adc_buffer1; // load next
	
	// calibrate
	NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
	while (NRF_SAADC->EVENTS_CALIBRATEDONE == 0);
	NRF_SAADC->EVENTS_CALIBRATEDONE = 0;
	
	// timer1 config
	NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;
	NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
	NRF_TIMER1->PRESCALER = 4; // 16 MHz / 2^4 = 1 MHz (1 us/tick)
	NRF_TIMER1->CC[0] = 100; // 100 ticks = 100 us (10 kHz)
	NRF_TIMER1->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk; //ccp clear timer
	
	// rtc2 config (low power)
	NRF_RTC2->PRESCALER = 0; 
	NRF_RTC2->CC[0] = 32768/HES_SAMPLE_FREQ; // ~10 ms interval
	
	// starts timer 1 at rtc2 ccp event detection
	NRF_PPI->CH[0].EEP = (uint32_t)&NRF_RTC2->EVENTS_COMPARE[0];
	NRF_PPI->CH[0].TEP = (uint32_t)&NRF_RTC2->TASKS_CLEAR;
	NRF_PPI->FORK[0].TEP = (uint32_t)&NRF_TIMER1->TASKS_START;
	
	// samples at timer1 ccp event detection
	NRF_PPI->CH[1].EEP = (uint32_t)&NRF_TIMER1->EVENTS_COMPARE[0];
	NRF_PPI->CH[1].TEP = (uint32_t)&NRF_SAADC->TASKS_SAMPLE;

	// powers down at sample end to conserve power
	NRF_PPI->CH[2].EEP = (uint32_t)&NRF_SAADC->EVENTS_END;
	NRF_PPI->CH[2].TEP = (uint32_t)&NRF_SAADC->TASKS_STOP;
	NRF_PPI->FORK[2].TEP = (uint32_t)&NRF_TIMER1->TASKS_STOP;

	NRF_PPI->CHENSET = PPI_CHENSET_CH0_Msk | PPI_CHENSET_CH1_Msk | PPI_CHENSET_CH2_Msk; // short ch[0:2] enable
	
	// timer start
	NRF_RTC2->TASKS_START = 1;
}
