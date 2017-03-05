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
#include "interrupt_helper.h"

#include "led_helper.h"
#include "joystick.h"
#include "acc.h"
#include "oled.h"
#include "rgb_helper.h"
#include "led7seg_helper.h"
#include "speaker_helper.h"


int main(void) {
    i2c_init();
    ssp_init();
    gpio_init();

    timer_interrupt_setup(LPC_TIM0, 1,   100, 1);
    timer_interrupt_setup(LPC_TIM1, 1,  1000, 0);
    timer_interrupt_setup(LPC_TIM2, 1,     1, 0);

    rgb_init();
    leds_init();

    speaker_init();

    led7seg_set_number(0);

    timer_interrupt_enable(LPC_TIM1);
    eint_interrupt_enable(0);
    eint_interrupt_handler_enable(0);

    return 0;
}

void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

