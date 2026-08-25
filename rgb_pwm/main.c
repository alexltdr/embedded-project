#include "stm32f4xx.h"
#include "../emlib/emlib.h"
#include "stm32f4xx.h"


#define RED 2
#define BLUE 0
#define GREEN 1

int main(void)
{
	rcc_gpioa_enable();
	rcc_tim2_enable();

	gpioa_alternate_function(BLUE);
	gpioa_alternate_function(RED);
	
	gpioa_alternate_function(GREEN);
	
	gpioa_tim2_set_low(BLUE);
	gpioa_tim2_set_low(GREEN);
	gpioa_tim2_set_low(RED);

		// the original value for tim2 is 16Mhz or 16 millions ticks per seconde. 
		// with 16000 as a prescaler, it become 1000 ticks per seconde. 
	set_prescaler_tim2(16000);
		// this is the limit of the counter. so if 1000 is the limit and the ticks per seconde
		// is 1000 it means it would do a full cycle in 1 seconde. 
	set_autoreload_tim2(1000);
	tim2_pwm_channel_setup();
	tim2_pwm_outputs_enable();
	set_duty_cycle();
	tim2_generate_update_event();
	start_timer();
}
