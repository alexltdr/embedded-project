#include "emlib.h"

#define AF1_TIM2   0x1U
#define AF_MASK 0xFU

void rcc_gpioa_enable(void) {
    RCC->AHB1ENR |= (1 << 0);
}

void gpio_enable_clock(GPIO_TypeDef *port) {
	uint32_t idx = ((uint32_t)port - AHB1PERIPH_BASE) / 0x400U;
	RCC->AHB1ENR |= (1U << idx);
}
void gpio_set_input(GPIO_TypeDef *port, uint8_t pin) {
	port->MODER &= ~(3 << (pin * 2));
}


void rcc_syscfg_enable(void)
{
	RCC->APB2ENR |= (1 << 14);
}
void rcc_tim2_enable(void)
{
	RCC->APB1ENR |= (1 << 0);
}

void gpioa_output(uint8_t pin) {
    GPIOA->MODER &= ~(3 << (pin * 2));
    GPIOA->MODER |=  (1 << (pin * 2));
}

void gpioa_input(uint8_t pin) {
    GPIOA->MODER &= ~(3 << (pin * 2));
}

void gpioa_pull_up(uint8_t pin) {
    GPIOA->PUPDR &= ~(3 << (pin * 2));
    GPIOA->PUPDR |=  (1 << (pin * 2));
}

void led_on(uint8_t pin) {
    GPIOA->ODR |= (1 << pin);
}

void led_off(uint8_t pin) {
    GPIOA->ODR &= ~(1 << pin);
}

// broches 0 à 7
void gpioa_tim2_set_low(uint8_t pin) {
    GPIOA->AFR[0] &= ~(AF_MASK  << (4 * pin));
    GPIOA->AFR[0] |=  (AF1_TIM2 << (4 * pin));
}

// broches 8 à 15
void gpioa_tim2_set_high(uint8_t pin) {
    GPIOA->AFR[1] &= ~(AF_MASK  << (4 * (pin - 8)));
    GPIOA->AFR[1] |=  (AF1_TIM2 << (4 * (pin - 8)));
}

void gpioa_alternate_function(uint8_t pin)
{

    GPIOA->MODER &= ~(3 << (pin * 2));
	GPIOA->MODER |= (2 << (pin * 2));
}

void set_autoreload_tim2(uint16_t value) {
	TIM2->ARR = value - 1;
}
void set_prescaler_tim2(uint16_t value)
{
	TIM2->PSC = value - 1;
}

/*
 * Configure les canaux 1, 2 et 3 de TIM2 en mode PWM.
 *
 * Pour chaque canal, deux réglages sont appliqués :
 *   - Mode PWM 1 (OCxM = 110) : la sortie est HIGH tant que le compteur
 *     est en dessous de la valeur CCRx, puis LOW jusqu'au reset du compteur.
 *     Plus CCRx est grand, plus le duty cycle est élevé, plus la LED brille.
 *   - Preload activé (OCxPE = 1) : les changements de CCRx écrits en cours
 *     de cycle sont mis en attente et appliqués proprement au début du
 *     cycle suivant. Évite les glitches visibles pendant les animations.
 *
 * Canaux 1 et 2 sont configurés dans CCMR1 (8 bits chacun).
 * Canal 3 est configuré dans CCMR2.
 *
 * Ne configure ni la broche de sortie (voir CCER), ni la valeur initiale
 * du duty cycle (voir CCRx), ni le démarrage du timer (voir CR1).
 */

void tim2_pwm_channel_setup(void)
{
    /* --- Canaux 1 et 2 : registre CCMR1 --- */

    /* Canal 1 : PWM mode 1 (110) sur OC1M (bits 4-6) */
    TIM2->CCMR1 |= (6 << 4);
    /* Canal 1 : preload activé sur OC1PE (bit 3) */
    TIM2->CCMR1 |= (1 << 3);

    /* Canal 2 : PWM mode 1 (110) sur OC2M (bits 12-14) */
    TIM2->CCMR1 |= (6 << 12);
    /* Canal 2 : preload activé sur OC2PE (bit 11) */
    TIM2->CCMR1 |= (1 << 11);

    /* --- Canal 3 : registre CCMR2 --- */

    /* Canal 3 : PWM mode 1 (110) sur OC3M (bits 4-6) */
    TIM2->CCMR2 |= (6 << 4);
    /* Canal 3 : preload activé sur OC3PE (bit 3) */
    TIM2->CCMR2 |= (1 << 3);
}


/*
 * Active la sortie physique des canaux 1, 2 et 3 de TIM2.
 *
 * Chaque canal a un bit CCxE dans le registre CCER qui fait office
 * d'interrupteur entre le signal PWM généré en interne et la broche
 * physique associée. Tant que ce bit est à 0, le canal tourne dans
 * le vide et la broche reste muette.
 *
 *   - CC1E (bit 0)  : active la sortie du canal 1
 *   - CC2E (bit 4)  : active la sortie du canal 2
 *   - CC3E (bit 8)  : active la sortie du canal 3
 *
 * La polarité (CCxP) est laissée à 0 (normale), ce qui va de pair
 * avec le mode PWM 1 configuré à l'étape précédente.
 */

void tim2_generate_update_event()
{
	TIM2->EGR |= (1 << 0);
}
void set_duty_cycle() {
	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 1000;
}
void tim2_pwm_outputs_enable() {
	TIM2->CCER |= (1 << 0);
	TIM2->CCER |= (1 << 4);
	TIM2->CCER |= (1 << 8);
}

void start_timer() {
	TIM2->CR1 |= (1 << 0);
}
