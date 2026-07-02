#include "stm32f4xx.h"
#define LED_PINOUT 6
#define LED_PINOUT1 (1U << 6)
#define BUTTON_PINOUT 9
#include <stdio.h>
#include "uart.h"
void setup_led()
{
	RCC->AHB1ENR |= (1U << 0);
	GPIOA->MODER &= ~(3U << (LED_PINOUT * 2));
	GPIOA->MODER |= (1U << (LED_PINOUT * 2));
}

void setup_button(){
	GPIOA->MODER &= ~(3U << (BUTTON_PINOUT * 2));
	GPIOA->MODER |= (0U << (BUTTON_PINOUT * 2));
	GPIOA->PUPDR &= ~(3U << (18));
	GPIOA->PUPDR |= (1U << 18);
	
}
int main(void) {
	
	setup_led();
	setup_button();
	uart_init();
	while(1)
	{
		if (!(GPIOA->IDR & (1U << 9)))
			GPIOA->BSRR = (1U << 22);
		else
			GPIOA->BSRR |= LED_PINOUT1;
		printf("Hello from STM32...\r\n");
	}

}
