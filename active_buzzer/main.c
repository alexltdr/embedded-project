#include "stm32f4xx.h"
#include "../emlib/emlib.h"

int main(void) {
	rcc_gpioa_enable();
	gpioa_output(3);
	led_on(3);

	while(1) {}
}
