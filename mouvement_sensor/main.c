#include "stm32f4xx.h"
#include "../emlib/emlib.h"

volatile uint8_t mouvement_flag = 0;

void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & (1 << 9)) {
        mouvement_flag = 1;
        EXTI->PR = (1 << 9);
    }
}

int main(void) {
    rcc_gpioa_enable();
    rcc_tim2_enable();
    rcc_syscfg_enable();
    gpioa_input(9);
    uart_init();

    exti_select_port(GPIOA, 9);
    exti_rising_edge(9);
    exti_falling_edge(9);
    exti_enable_line(9);
    nvic_enable_irq(EXTI9_5_IRQn);

    while (1) {
        if (mouvement_flag) {
            mouvement_flag = 0;
            uint32_t etat = (GPIOA->IDR >> 9) & 1;
            if (etat) printf("mouvement détecté\r\n");
            else      printf("fin de mouvement\r\n");
        }
    }
}
