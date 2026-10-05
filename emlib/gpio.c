#include "gpio.h"

void gpio_init(volatile uint32_t* gpio_port, uint8_t pin, Gpiomode mode) {
    volatile uint32_t *moder = gpio_port + MODER_IDX;  // MODER est à l'offset 0
    
    *moder &= ~(3 << (pin * 2));
    *moder |= (mode << (pin * 2));
}

void gpio_write(volatile uint32_t* gpio_port, uint8_t pin, uint8_t value)
{
	volatile uint32_t *odr = gpio_port + ODR_IDX;
	
	*odr &= ~(1 << pin);
	*odr |= (value << pin);
}

uint8_t gpio_read(volatile uint32_t* gpio_port, uint8_t pin)
{
	volatile uint32_t *idr = gpio_port + IDR_IDX;
	
	if (*idr & (1 << pin))
		return 1;
	return 0;
}


void gpio_toggle(volatile uint32_t* gpio_port, uint8_t pin) {
	return; 
}

void gpio_set_af(volatile uint32_t* gpio_port, uint8_t pin, uint8_t af) {
    uint8_t reg_idx = (pin < 8) ? AFRL_IDX : AFRH_IDX;
    uint8_t shift = (pin % 8) * 4;
    
    gpio_port[reg_idx] &= ~(0xFU << shift);         // clear les 4 bits
    gpio_port[reg_idx] |=  ((af & 0xFU) << shift);  // set l'AF
}
