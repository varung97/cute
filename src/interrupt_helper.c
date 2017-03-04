/*
 * interrupt_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "interrupt_helper.h"

extern void EINT3_IRQHandler(void) {
	if (did_interrupt_occur(2, 10)) {
        clear_gpio_interrupt(2, 10);
	}
}

void enable_gpio_interrupt(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		LPC_GPIOINT->IO0IntEnF |= 1 << pin_num;
	} else {
		LPC_GPIOINT->IO2IntEnF |= 1 << pin_num;
	}
}

void clear_gpio_interrupt(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		LPC_GPIOINT->IO0IntClr = 1 << pin_num;
	} else {
		LPC_GPIOINT->IO2IntEnF |= 1 << pin_num;
	}
}

int did_interrupt_occur(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		return (LPC_GPIOINT->IO0IntStatF >> pin_num) & 0x1;
	} else {
		return (LPC_GPIOINT->IO2IntStatF >> pin_num) & 0x1;
	}
}

void enable_eint_interrupt(int int_number) {
	LPC_SC->EXTMODE |= 1 << int_number;
	LPC_SC->EXTMODE &= ~(1 << int_number);

	switch (int_number) {
		case 0:
			config_pin(1, 0, 0, 2, 10);
			break;
		case 1:
			config_pin(1, 0, 0, 2, 11);
			break;
		case 2:
			config_pin(1, 0, 0, 2, 12);
			break;
		case 3:
			config_pin(1, 0, 0, 2, 13);
			break;
		default:
			break;
	}
}

void enable_eint_interrupt_handler(int int_number) {
	switch (int_number) {
		case 0:
			NVIC_EnableIRQ(EINT0_IRQn);
			break;
		case 1:
			NVIC_EnableIRQ(EINT1_IRQn);
			break;
		case 2:
			NVIC_EnableIRQ(EINT2_IRQn);
			break;
		case 3:
			NVIC_EnableIRQ(EINT3_IRQn);
			break;
		default:
			break;
	}
}
