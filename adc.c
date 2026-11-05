//adc.c : Reads voltage and current using ADC driver

#include "eps.h"

void ADC_Initialize(void) {
	
	//Select AVCC as reference voltage
	ADMUX = (1 << REFS0); //REFS1 = 0, REFS0 = 1 -> AVCC
	
	//Set ADC prescaler to 128 (16 MHz / 128 = 125 kHz) and enable ADC
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
}

unsigned long ADC_ReadChannel(uint8_t channel_id) {
	
	ADMUX &= 0xF0; //Clear previous channel
	
	ADMUX |= (channel_id & 0x0F); //Select ADC channel between 0 and 15
	
	//Start ADC conversion
	ADCSRA |= (1 << ADSC);
	
	//Wait until conversion complete (ADSC becomes 0)
	while(ADCSRA & (1 << ADSC))
	{
		//wait
	}
	
	//10-bit result
	return ADC;
	
}

//read voltage
float read_voltage() {
	uint16_t raw = ADC_ReadChannel(ADC_CH0);
	float adc_volt = (raw * ADC_REF_VOLT) / ADC_MAX; //voltage formula
	return adc_volt;
}

//read current
float read_current() {
	uint16_t raw = ADC_ReadChannel(ADC_CH1);
	float adc_volt = (raw * ADC_REF_VOLT) / ADC_MAX; //current formula
	float adc_curr = (adc_volt/ SHUNT_RES); //current formula
	return adc_curr;
}





