/*****************************************************************************
 *   CUTE - Assignment 2, EE2024
 *
 *   Varun Gupta      A0147924X
 *   Shailesh Nahata  A0147619W
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
    setup_timer_interrupt(LPC_TIM0, 100,  2, 1);

    init_rgb();
    init_leds();

    return 0;
}

void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

