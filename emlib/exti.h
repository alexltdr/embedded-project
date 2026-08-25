#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

#include "stm32f4xx.h"
extern volatile uint8_t mouvement_flag;

void exti_enable_line(uint8_t line);

void exti_disable_line(uint8_t line);

void exti_rising_edge(uint8_t line);


void exti_falling_edge(uint8_t line);


void exti_select_port(GPIO_TypeDef *port, uint8_t pin);

// allow nvic to forward  this interrup to the cpu 
void nvic_enable_irq(IRQn_Type irq);

void exti_clear_pending(uint8_t  line);


#endif 
