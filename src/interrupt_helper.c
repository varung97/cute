/*
 * interrupt_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "interrupt_helper.h"

uint8_t is_speaker_buzzing = 0;

extern void EINT0_IRQHandler(void) {
	eint_interrupt_clear(0);

	if (is_speaker_buzzing) {
		timer_interrupt_enable(2);
	} else {
		timer_interrupt_disable(2);
	}

	is_speaker_buzzing = !is_speaker_buzzing;
}

void gpio_interrupt_enable(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		LPC_GPIOINT->IO0IntEnF |= 1 << pin_num;
	} else {
		LPC_GPIOINT->IO2IntEnF |= 1 << pin_num;
	}
}

void gpio_interrupt_clear(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		LPC_GPIOINT->IO0IntClr = 1 << pin_num;
	} else {
		LPC_GPIOINT->IO2IntEnF |= 1 << pin_num;
	}
}

int did_gpio_interrupt_occur(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		return (LPC_GPIOINT->IO0IntStatF >> pin_num) & 0x1;
	} else {
		return (LPC_GPIOINT->IO2IntStatF >> pin_num) & 0x1;
	}
}

void eint_interrupt_enable(int int_number) {
	LPC_SC->EXTMODE |= 1 << int_number;
	LPC_SC->EXTPOLAR &= ~(1 << int_number);

	switch (int_number) {
		case 0:
			pin_config(1, 0, 0, 2, 10);
			break;
		case 1:
			pin_config(1, 0, 0, 2, 11);
			break;
		case 2:
			pin_config(1, 0, 0, 2, 12);
			break;
		case 3:
			pin_config(1, 0, 0, 2, 13);
			break;
		default:
			break;
	}
}

void eint_interrupt_clear(int int_number) {
	LPC_SC->EXTINT |= 1 << int_number;
}

void eint_interrupt_handler_enable(int int_number) {
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
