#include "stm32f4xx.h"
#include "../emlib/emlib.h"
#include <stdint.h>

int main(void) {
	uint8_t pin = 0;
    uart_init();
  	rcc_enable_ahb1(MY_GPIOA);
    rcc_adc1_enable();
    gpio_init(MY_GPIOA, pin, Analog);
	adc_init(pin); // PA0
    setvbuf(stdout, NULL, _IONBF, 0);

    while (1) {
        uint16_t value = adc_read();
        printf("ADC: %u\r\n", value);
        for (volatile int i = 0; i < 1000000; i++);
    }
}
