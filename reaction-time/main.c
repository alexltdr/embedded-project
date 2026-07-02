#include "stm32f4xx.h"
#define LED_PIN 6
#define LED_PINOUT1 (1U << 6)
#define BUTTON_PIN 9
#include <stdio.h>
#include "uart.h"

volatile uint32_t millis = 0; // variable pour le temps
void setup_led()
{
	RCC->AHB1ENR |= (1U << 0);
	GPIOA->MODER &= ~(3U << (LED_PIN * 2));
	GPIOA->MODER |= (1U << (LED_PIN * 2));
}

void setup_button(){
	GPIOA->MODER &= ~(3U << (BUTTON_PIN * 2));
	GPIOA->MODER |= (0U << (BUTTON_PIN * 2));
	GPIOA->PUPDR &= ~(3U << (18));
	GPIOA->PUPDR |= (1U << 18);
	
}

void tim2_init(void) {
    RCC->APB1ENR |= (1U << 0);
    TIM2->PSC = 16 - 1;
    TIM2->ARR = 0xFFFFFFFF;
    TIM2->EGR |= 1;   // force update event pour charger PSC dans le shadow register
    TIM2->CNT = 0;
}

void tim3_init(void) {
    RCC->APB1ENR |= (1U << 1);
    TIM3->PSC = 16 - 1;
    TIM3->ARR = 1000 - 1;
    TIM3->DIER |= (1U << 0);
    NVIC_EnableIRQ(TIM3_IRQn);
    TIM3->CR1 |= (1U << 0);
}

void TIM3_IRQHandler(void) {
    if (TIM3->SR & (1U << 0)) {
        TIM3->SR &= ~(1U << 0);
        millis++;
    }
}
static inline int button_pressed(void) {
    return !(GPIOA->IDR & (1U << BUTTON_PIN));
}

void delay_ms(uint32_t ms) {
    uint32_t start = millis;
    while ((millis - start) < ms) {}
}
void wait_for_two_presses(void) {
    for (int i = 0; i < 2; i++) {
        while (!button_pressed()) {}   // 1. attendre l'appui
        delay_ms(20);                  // 2. antirebond
        while (button_pressed()) {}    // 3. attendre le relâchement
        delay_ms(20);                  // 4. antirebond
    }
}

uint32_t random_delay_ms(void) {
    return 3000 + (millis % 2001);   // entre 3000 et 5000 ms
}
	int main(void) {
    setup_led();
    setup_button();
    uart_init();
    tim2_init();
    tim3_init();

    printf("\r\n=== Reaction Time Game ===\r\n");

    while (1) {
        printf("\r\nAppuyez 2 fois sur le bouton pour commencer...\r\n");
        wait_for_two_presses();

        printf("Attention...\r\n");

        uint32_t wait = random_delay_ms();
        uint32_t start = millis;
        while ((millis - start) < wait) {
            GPIOA->BSRR = (1U << LED_PIN);           // LED ON
            delay_ms(200);
            GPIOA->BSRR = (1U << (LED_PIN + 16));    // LED OFF
            delay_ms(200);
        }

        GPIOA->BSRR = (1U << LED_PIN);   // LED ON
        TIM2->CNT = 0;                    // reset compteur
        TIM2->CR1 |= (1U << 0);           // START TIM2

        // Attendre l'appui du joueur
        while (!button_pressed()) {}
        TIM2->CR1 &= ~(1U << 0);          // STOP TIM2
        uint32_t reaction_us = TIM2->CNT;

        GPIOA->BSRR = (1U << (LED_PIN + 16));   // LED OFF

        printf("Temps de reaction : %lu ms %lu us\r\n",
               reaction_us / 1000, reaction_us % 1000);

        while (button_pressed()) {}
        delay_ms(200);
    }
}
