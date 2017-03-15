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
	acc_init();
	light_init();
	temp_init(&get_ms_ticks);
	rgb_init();
	oled_init();
	led7seg_init();

	acc_setMode(ACC_MODE_STANDBY);
	acc_setRange(ACC_RANGE_4G);
	led7seg_set_raw(0xFF);
	oled_clearScreen(OLED_COLOR_BLACK);
}

void init_interrupts() {
	timer_interrupt_setup(TIMER0, 1);
	timer_interrupt_setup(TIMER1, 1);
}

void attach_interrupts() {
	timer_attach_interrupt(TIMER0, turn_off_blinking_rgb, 100, 1);
	timer_attach_interrupt(TIMER1, do_every_second, 1000, 0);
	eint_attach_interrupt(EINT0, toggle_mode);

	NVIC_SetPriority(TIMER0_IRQn, 31);
	NVIC_SetPriority(TIMER1_IRQn, 31);
	NVIC_SetPriority(SysTick_IRQn, 31);
	NVIC_SetPriority(EINT0_IRQn, 31);
}

void enable_interrupts() {
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


