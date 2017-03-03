/*****************************************************************************
 *   A demo example using several of the peripherals on the base board
 *
 *   Copyright(C) 2011, EE2024
 *   All rights reserved.
 *
 ******************************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "LPC17xx.h"
#include "interface_helper.h"
#include "timer_helper.h"

#include "led_helper.h"
#include "joystick.h"
#include "acc.h"
#include "oled.h"
#include "rgb_helper.h"
#include "led7seg_helper.h"


int main(void) {
    init_i2c();
    init_ssp();
    init_GPIO();
    setup_systick_interrupt();
    setup_timer_interrupt(LPC_TIM0, 100, 2, 1);

    init_rgb();
    init_leds();

    uint32_t prev_sec = 0;
    uint8_t led7seg_display_val = 0;

    while(1) {
    	if (sec_ticks != prev_sec) {
    		// 1 second has passed
    		led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
    		set_number_led7seg(led7seg_display_val);

    		if (sec_ticks - prev_sec == 2) {
    			// 2 seconds have passed
    			prev_sec = sec_ticks;
				enable_timer_interrupt(LPC_TIM0);
				set_rgb(RGB_BLUE);
    		}
    	}
    }
}

void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

