/*
 * message.c
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#include "message.h"

char incoming_message[80];
volatile int uart_new_data_available = 0;
message_mode_type message_mode = VIEW;
uint8_t state;
volatile int led1 = 0;

void uart_rxav_isr() {
	leds_only_turn_on(led1 = !led1);
	uart_new_data_available = uart_receive_notblocking(incoming_message);
}

void display_incoming_message() {
	oled_clearScreen(OLED_COLOR_BLACK);
	oled_putString(0, 0, (uint8_t *) "    MESSAGE", OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	int i = 0, row = 10;

	while (i < strlen(incoming_message) && row <= 60) {
		oled_putString(0, row, (uint8_t *) (incoming_message + i), OLED_COLOR_WHITE, OLED_COLOR_BLACK);
		i += 15;
		row += 10;
	}
}

void enable_view_mode() {
	display_incoming_message();
}

void enable_message_mode() {
	reset_board();

	message_mode = VIEW;
	enable_view_mode();

	uart_enable();
	uart_specific_interrupt_cmd(RXAV, ENABLE);
}

void message_loop() {
	if (uart_new_data_available) {
		uart_new_data_available = 0;
		display_incoming_message();
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
