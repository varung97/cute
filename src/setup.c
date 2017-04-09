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
	uart_init();
}

void init_external_peripherals() {
	acc_init();
	light_init();
	temp_init(&get_ms_ticks);
	rgb_init();
	oled_init();
	led7seg_init();
	joystick_init();

	acc_setMode(ACC_MODE_STANDBY);
	acc_setRange(ACC_RANGE_4G);
	led7seg_set_raw(0xFF);
	oled_clearScreen(OLED_COLOR_BLACK);
}

void init_interrupts() {
	timer_interrupt_setup(TIMER0, 1000);
	timer_interrupt_setup(TIMER1, 1000);
	timer_interrupt_setup(TIMER2, 500);
	timer_interrupt_setup(TIMER3, 1000);
	systick_interrupt_init();
}

void attach_interrupts() {
	timer_attach_interrupt(TIMER0, toggle_leds, 333, 0);
	timer_attach_interrupt(TIMER3, read_joystick_isr, 150, 0);
	eint_attach_interrupt(EINT0, toggle_isr);
	eint_attach_interrupt(EINT3, eint3_isr);
	uart_attach_interrupt(THRE, uart_thre_isr);
	uart_attach_interrupt(RXAV, uart_rxav_isr);
}

void set_interrupt_priorities() {
	NVIC_SetPriorityGrouping(4);
	NVIC_SetPriority(TIMER0_IRQn, 8);
	NVIC_SetPriority(TIMER1_IRQn, 5);
	NVIC_SetPriority(TIMER2_IRQn, 1);
	NVIC_SetPriority(TIMER3_IRQn, 2);
	NVIC_SetPriority(EINT0_IRQn, 12);
	NVIC_SetPriority(EINT3_IRQn, 4);
	NVIC_SetPriority(SysTick_IRQn, 0);
	NVIC_SetPriority(UART3_IRQn, 9);
}

void enable_interrupts() {
	set_interrupt_priorities();

	eint_interrupt_enable(EINT0, EDGE_TRIGGERED, ACTIVELOW_OR_FALLINGEDGE);
	eint_interrupt_handler_enable(EINT0);
	eint_interrupt_enable(EINT3, LEVEL_TRIGGERED, ACTIVELOW_OR_FALLINGEDGE);

	gpio_interrupt_enable(2, 5);
	gpio_interrupt_enable(0, 2);
}

void setup() {
	init_interfaces();
	init_external_peripherals();
	init_interrupts();

	attach_interrupts();
	enable_interrupts();
}
