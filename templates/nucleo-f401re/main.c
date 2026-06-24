#include "stm32f4xx.h"

static void delay(volatile uint32_t n) {
    while (n--) __asm__("nop");
}

int main(void) {
    // 1. Activer l'horloge du port GPIOA (bus AHB1)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Configurer PA5 en mode "general purpose output"
    GPIOA->MODER &= ~(0x3 << (5 * 2));
    GPIOA->MODER |=  (0x1 << (5 * 2));

    while (1) {
        GPIOA->ODR ^= (1 << 5);
        delay(400000);
    }
}
