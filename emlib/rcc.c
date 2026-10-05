#include "rcc.h"

#define RCC_AHB1ENR  (*(volatile uint32_t*)0x40023830)
#define RCC_AHB2ENR  (*(volatile uint32_t*)0x40023834)
#define RCC_APB1ENR  (*(volatile uint32_t*)0x40023840)
#define RCC_APB2ENR  (*(volatile uint32_t*)0x40023844)

#define AHB1_BASE    0x40020000UL
#define AHB2_BASE    0x50000000UL


void rcc_enable_ahb1(volatile uint32_t* port) {
    uint32_t idx = ((uint32_t)port - AHB1_BASE) / 0x400;
    RCC_AHB1ENR |= (1U << idx);
}

void rcc_enable_ahb2(volatile uint32_t* port) {
    uint32_t idx = ((uint32_t)port - AHB2_BASE) / 0x400;
    RCC_AHB2ENR |= (1U << idx);
}

void rcc_gpioa_enable(void) {
    RCC_AHB1ENR |= (1 << 0);
}

void rcc_syscfg_enable(void) {
    RCC_APB2ENR |= (1 << 14);
}

void rcc_tim2_enable(void) {
    RCC_APB1ENR |= (1 << 0);
}

void rcc_spi1_enable(void) {
    RCC_APB2ENR |= (1 << 12);
}

//active adc
void rcc_adc1_enable(void) {
    RCC_APB2ENR |= (1 << 8);
}
