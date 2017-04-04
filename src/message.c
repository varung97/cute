/*
 * message.c
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#include "message.h"

char uart_recv[120];
volatile int uart_new_data_available = 0;
message_mode_type message_mode = VIEW;
uint8_t state;

void enable_message_mode() {
	reset_board();

	message_mode = VIEW;

	uart_enable();
	uart_specific_interrupt_cmd(RXAV, ENABLE);

	oled_putString(0, 0, (uint8_t *) "MESSAGE", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void uart_rxav_isr() {
	uart_new_data_available = uart_receive_notblocking(uart_recv);
}

void message_loop() {
	if (uart_new_data_available) {
		uart_new_data_available = 0;
		oled_putString(0, 0, (uint8_t *) uart_recv, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	}

	// Only do this every 200ms or so??
	state = joystick_read();
	if (state & JOYSTICK_CENTER) {
		if (message_mode == VIEW) {
			// switch to write mode
		} else {
			// select something on the write screen
		}
	}
	if (state & JOYSTICK_DOWN) {
		if (message_mode == WRITE) {
			// move down, only if possible
		}
	}
}
