#ifndef EMLIB_H
#define EMLIB_H

#include "adc.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "exti.h"
#include "gpio.h"
#include "rcc.h"

/* --- Défines --- */
#define AF1_TIM2   0x1U
#define AF_MASK    0xFU


/* --- GPIOA : contrôle des LEDs --- */
void led_on(uint8_t pin);
void led_off(uint8_t pin);

/* --- GPIOA : sélection de l'alternate function TIM2 --- */
void gpioa_tim2_set_low(uint8_t pin);   /* broches 0 à 7  */
void gpioa_tim2_set_high(uint8_t pin);  /* broches 8 à 15 */

/* --- TIM2 : configuration de base --- */
void set_prescaler_tim2(uint16_t value);
void set_autoreload_tim2(uint16_t value);

/* --- TIM2 : configuration PWM --- */
void tim2_pwm_channel_setup(void);
void tim2_pwm_outputs_enable(void);
void set_duty_cycle(void);

/* --- TIM2 : mise à jour et démarrage --- */
void tim2_generate_update_event(void);
void start_timer(void);

/* ---- SYSCFG ---- */

void rcc_syscfg_enable(void);
#endif 
