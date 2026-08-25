#include "exti.h"


// use Interrupt mask register (IMR) to allow EXTI generate interruption 
// when the line detech an event.

void exti_enable_line(uint8_t line)
{
	EXTI->IMR |= (1 << line);	
}

void exti_disable_line(uint8_t line)
{
	EXTI->IMR &= ~(1 << line);	
}


// tell exti which type of transition to trigger. 0 to 1 or 1 to 0. 
void exti_rising_edge(uint8_t line)
{
	EXTI->RTSR |= (1 << line);
}


void exti_falling_edge(uint8_t line)
{
	EXTI->FTSR |= (1 << line);
}

// allow nvic to forward  this interrup to the cpu 
void nvic_enable_irq(IRQn_Type irq)
{
	NVIC_EnableIRQ(irq);
}

void exti_clear_pending(uint8_t  line)
{
	EXTI->PR = (1 << line);
}


void exti_select_port(GPIO_TypeDef *port, uint8_t pin) {
	uint32_t port_idx = ((uint32_t)port - AHB1PERIPH_BASE) / 0x400U;
	uint32_t reg_idx  = pin / 4;
	uint32_t bit_pos  = (pin % 4) * 4;
	SYSCFG->EXTICR[reg_idx] &= ~(0xFU << bit_pos);
	SYSCFG->EXTICR[reg_idx] |=  (port_idx << bit_pos);
}

