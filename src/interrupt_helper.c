/*
 * interrupt_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "interrupt_helper.h"

eint_config_t eint_config_table[EINT_MAX] = {
   { NULL, EINT0_IRQn, 10 },
   { NULL, EINT1_IRQn, 11 },
   { NULL, EINT2_IRQn, 12 },
   { NULL, EINT3_IRQn, 13 },
};

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
		LPC_GPIOINT->IO2IntClr = 1 << pin_num;
	}
}

int did_gpio_interrupt_occur(uint8_t port_num, uint8_t pin_num) {
	if (port_num == 0) {
		return (LPC_GPIOINT->IO0IntStatF >> pin_num) & 0x1;
	} else {
		return (LPC_GPIOINT->IO2IntStatF >> pin_num) & 0x1;
	}
}

void eint_attach_interrupt(uint8_t int_number, eint_func_ptr func_ptr) {
	if (int_number >= EINT_MAX) return;

	eint_config_table[int_number].eint_func = func_ptr;
}

void eint_interrupt_enable(uint8_t int_number, uint8_t extmode, uint8_t extpolar) {
	if (int_number >= EINT_MAX) return;

	LPC_SC->EXTMODE = (LPC_SC->EXTMODE & ~(1 << int_number)) | (extmode << int_number);
	LPC_SC->EXTPOLAR = (LPC_SC->EXTPOLAR & ~(1 << int_number)) | (extpolar << int_number);
	NVIC_ClearPendingIRQ(eint_config_table[int_number].IrqNumber);

	if(int_number != 3) {
		pin_config(1, 0, 0, 2, eint_config_table[int_number].pin_num);
	}
}

void eint_interrupt_clear(uint8_t int_number) {
	if (int_number >= EINT_MAX) return;

	LPC_SC->EXTINT |= 1 << int_number;
}

void eint_interrupt_handler_enable(uint8_t int_number) {
	if (int_number >= EINT_MAX) return;

	NVIC_EnableIRQ(eint_config_table[int_number].IrqNumber);
}

void eint_interrupt_handler_disable(uint8_t int_number) {
	if (int_number >= EINT_MAX) return;

	NVIC_DisableIRQ(eint_config_table[int_number].IrqNumber);
}

void call_eint_func(uint8_t int_number) {
	if (int_number >= EINT_MAX) return;

	if (eint_config_table[int_number].eint_func != NULL) {
		eint_config_table[int_number].eint_func();
	}
}

void EINT0_IRQHandler(void) {
	eint_interrupt_clear(EINT0);

	call_eint_func(EINT0);
}

void EINT1_IRQHandler(void) {
	eint_interrupt_clear(EINT1);

	call_eint_func(EINT1);
}

void EINT2_IRQHandler(void) {
	eint_interrupt_clear(EINT2);

	call_eint_func(EINT2);
}

void EINT3_IRQHandler(void) {
//	eint_interrupt_clear(EINT3);

	call_eint_func(EINT3);
}
