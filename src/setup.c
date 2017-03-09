/*
 * setup.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "setup.h"

void init_interfaces() {
	i2c_init();
	ssp_init();
	gpio_init();
}

void init_external_peripherals() {
	rgb_init();
	leds_init();
	acc_init();
	light_init();
	speaker_init();
	oled_init();
	led7seg_init();
	temp_init(&get_ms_ticks);

	light_enable();
	led7seg_set_number(0);
	oled_clearScreen(OLED_COLOR_BLACK);
}

void init_interrupts() {
	timer_interrupt_setup(TIMER0, 1);
	timer_interrupt_setup(TIMER1, 1);
	timer_interrupt_setup(TIMER2, 1);
	systick_interrupt_setup();
}

void attach_interrupts() {
	timer_attach_interrupt(TIMER0, turn_off_blinking_rgb, 100, 1);
	timer_attach_interrupt(TIMER1, do_every_second, 1000, 0);
	timer_attach_interrupt(TIMER2, speaker_toggle, 1, 0);
	eint_attach_interrupt(EINT0, toggle_speaker_buzzing);
}

void enable_interrupts() {
	timer_interrupt_enable(TIMER1);
	eint_interrupt_enable(EINT0);
	eint_interrupt_handler_enable(EINT0);
}

void setup() {
	init_interfaces();
	init_external_peripherals();
	init_interrupts();

	attach_interrupts();
	enable_interrupts();
}


