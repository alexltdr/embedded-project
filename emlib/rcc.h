#ifndef RCC_H
#define RCC_H

#include "gpio.h"
#include <stdint.h>


void rcc_enable_ahb1(volatile uint32_t* port);
void rcc_enable_ahb2(volatile uint32_t* port);


void rcc_gpioa_enable(void);
void rcc_syscfg_enable(void);
void rcc_tim2_enable(void);

void rcc_spi1_enable(void);

void rcc_adc1_enable(void);
#endif 
