#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "stm32f4xx.h"

void adc_init(uint8_t pin);
uint16_t adc_read(void);

#endif 
