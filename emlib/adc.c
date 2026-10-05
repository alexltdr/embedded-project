#include "adc.h"
void adc_init(uint8_t pin)
{
	// sample time register (max = 111 = 7)
    ADC1->SMPR2 |= (7 << (pin * 3));
	//adc on 
    ADC1->CR2 |= ADC_CR2_ADON;
}

uint16_t adc_read(void) {
    ADC1->SR &= ~ADC_SR_EOC;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR;
}
