/*
 * application_logic.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "application_logic.h"

mode_type current_mode = PASSIVE;

volatile int should_toggle_mode_single = 0,
		     should_toggle_mode_double = 0,
		     uart_should_queue = 0;

/**********************************************************
 * Helper functions
 ***********************************************************/

void reset_board() {
	timer_interrupt_disable(TIMER0);
	timer_interrupt_disable(TIMER1);
	timer_interrupt_disable(TIMER2);

	eint_interrupt_handler_disable(EINT3);

	uart_specific_interrupt_cmd(THRE, DISABLE);
	uart_specific_interrupt_cmd(RXAV, DISABLE);

	led7seg_set_raw(0xFF);
	acc_setMode(ACC_MODE_STANDBY);
	light_shutdown();
	uart_disable();
	oled_clearScreen(OLED_COLOR_BLACK);
	uart_should_queue = 0;

	rgb_also_clear_red();
	rgb_also_clear_blue();
}

void toggle_mode_single() {
	switch (current_mode) {
		case PASSIVE:
		case MESSAGE:
			current_mode = MONITOR;
			enable_monitor_mode();
			break;
		case MONITOR:
			current_mode = PASSIVE;
			enable_passive_mode();
			break;
		default:
			break;
	}
}

void toggle_mode_double() {
	switch (current_mode) {
		case PASSIVE:
		case MONITOR:
			current_mode = MESSAGE;
			enable_message_mode();
			break;
		case MESSAGE:
			current_mode = PASSIVE;
			enable_passive_mode();
			break;
		default:
			break;
	}
}


/**********************************************************
 * ISRs
 **********************************************************/

void toggle_isr() {
	if (pin_read_val(1, 31)) {
		// SW4 pulls low when button is pressed
		should_toggle_mode_single = 1;
	} else {
		should_toggle_mode_double = 1;
	}
}

void uart_thre_isr() {
	uart_should_queue = 1;
}

/**********************************************************
 * Loop
 **********************************************************/

void loop() {
	if (should_toggle_mode_single) {
		should_toggle_mode_single = 0;

		toggle_mode_single();
	}

	if (should_toggle_mode_double) {
		should_toggle_mode_double = 0;

		toggle_mode_double();
	}

	if (uart_should_queue) {
		uart_queue_into_fifo();
		uart_should_queue = 0;
	}

	switch (current_mode) {
		case MONITOR:
			monitor_loop();
			break;
		case MESSAGE:
			message_loop();
		default:
			break;
	}
}
