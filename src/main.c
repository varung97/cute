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
#include "acc_helper.h"
#include "light_helper.h"
#include "temp_helper.h"
#include "oled.h"
#include "rgb_helper.h"
#include "led7seg_helper.h"
#include "speaker_helper.h"

uint8_t led7seg_display_val = 0;
uint8_t is_speaker_buzzing = 0;

void turn_off_blinking_rgb() {
	rgb_also_clear_red();
}

void do_every_second() {
	led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
	led7seg_set_number(led7seg_display_val);

	rgb_also_set_red();
	timer_interrupt_enable(TIMER0);
}

void toggle_speaker_buzzing() {
	if (is_speaker_buzzing) {
		timer_interrupt_disable(TIMER2);
		speaker_off();
	} else {
		timer_interrupt_enable(TIMER2);
	}

	is_speaker_buzzing = !is_speaker_buzzing;
}

int main(void) {
    i2c_init();
    ssp_init();
    gpio_init();

    timer_interrupt_setup(TIMER0, 1);
    timer_interrupt_setup(TIMER1, 1);
    timer_interrupt_setup(TIMER2, 1);

    timer_attach_interrupt(TIMER0, turn_off_blinking_rgb, 100, 1);
    timer_attach_interrupt(TIMER1, do_every_second, 1000, 0);
    timer_attach_interrupt(TIMER2, speaker_toggle, 1, 0);
    eint_attach_interrupt(EINT0, toggle_speaker_buzzing);

    rgb_init();
    leds_init();
    acc_init();
    light_init();
    speaker_init();
    oled_init();
    led7seg_init();
    temp_init(&get_ms_ticks);

    timer_interrupt_enable(TIMER1);
    eint_interrupt_enable(EINT0);
    eint_interrupt_handler_enable(EINT0);
    systick_interrupt_setup();

    light_enable();
    led7seg_set_number(0);
    oled_clearScreen(OLED_COLOR_BLACK);

    int8_t x, y, z;
    uint32_t light_val, temp_val;
    int var;
    char str_val[5];

    while(1) {
//    	acc_setMode(ACC_MODE_MEASURE);
//    	acc_setRange(ACC_RANGE_4G);
//    	acc_read(&x, &y, &z);
//
//    	light_val = light_read();

//    	temp_val = temp_read();
//    	sprintf(str_val, "%d", temp_val);
//		oled_putString(0, 0, str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
//
//		for (var = 0; var < 100000; ++var);
    }

    return 0;
}

void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

