#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define PERIPH_BASE 0x40000000UL
#define AHB1_BASE   (PERIPH_BASE + 0x00020000UL)

#define MY_GPIOA ((volatile uint32_t*)(AHB1_BASE + 0x0000UL))
#define MY_GPIOB ((volatile uint32_t*)(AHB1_BASE + 0x0400UL))
#define MY_GPIOC ((volatile uint32_t*)(AHB1_BASE + 0x0800UL))
#define MY_GPIOD ((volatile uint32_t*)(AHB1_BASE + 0x0C00UL))
#define MY_GPIOE ((volatile uint32_t*)(AHB1_BASE + 0x1000UL))
#define MY_GPIOH ((volatile uint32_t*)(AHB1_BASE + 0x1C00UL))

typedef enum {
    Input,
    Output,
    Alternate,
    Analog
} Gpiomode;

#define MODER_IDX    0
#define OTYPER_IDX   1
#define OSPEEDR_IDX  2
#define PUPDR_IDX    3
#define IDR_IDX      4
#define ODR_IDX      5
#define BSRR_IDX     6
#define LCKR_IDX     7
#define AFRL_IDX     8
#define AFRH_IDX     9

// configure gpio mod via moder
void gpio_init(volatile uint32_t* gpio_port, uint8_t pin, Gpiomode mode);

// put a output pin at 1 or 0 
void gpio_write(volatile uint32_t* gpio_port, uint8_t pin, uint8_t value);

// read a state from a gpio
uint8_t gpio_read(volatile uint32_t* gpio_port, uint8_t pin);

void gpio_set_af(volatile uint32_t* gpio_port, uint8_t pin, uint8_t af);

#endif
