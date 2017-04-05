/*
 * message.c
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#include "message.h"

void display_keyboard();
void write_loop();

char incoming_message[80];
volatile int uart_new_data_available = 0;
message_mode_type message_mode = WRITE;
uint8_t state;
volatile int led1 = 0;
volatile int should_read_joystick = 0;
int curr_dash_start_x = 0, curr_dash_end_x = 5, curr_dash_y = 48;
int prev_dash_start_x = 0, prev_dash_end_x = 5, prev_dash_y = 48;

void uart_rxav_isr() {
	uart_new_data_available = uart_receive_notblocking(incoming_message);
}

void read_joystick_isr() {
	should_read_joystick = 1;
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

void enable_write_mode() {
	display_keyboard();
}

void enable_message_mode() {
	reset_board();

	message_mode = VIEW;
	enable_view_mode();

	timer_interrupt_enable(TIMER3);

	uart_enable();
	uart_specific_interrupt_cmd(RXAV, ENABLE);
}

void view_loop() {
	if (uart_new_data_available) {
		uart_new_data_available = 0;
		display_incoming_message();
	}


	// switch to write mode
	if (state & JOYSTICK_CENTER) {
		message_mode = WRITE;
		enable_write_mode();
	}
}

void display_keyboard() {
	oled_clearScreen(OLED_COLOR_BLACK);

	int i;
	for (i = 0; i < 15; i++) {
		oled_putChar(i * 6, 40, 'a' + i, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	}
	for (i = 0; i < 11; i++) {
		oled_putChar(i * 6, 50, 'p' + i, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	}
	oled_putChar(68, 50, '.', OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_putChar(72, 50, 0x80, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_putChar(80, 50, 'X', OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_putChar(86, 50, 0x81, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);
}

void write_loop() {
	if (state & JOYSTICK_CENTER) {
		if(curr_dash_start_x == 78 && curr_dash_y == 58) {
			message_mode = VIEW;
			enable_view_mode();
		}
	}

	if (state & JOYSTICK_LEFT) {
		if(curr_dash_start_x > 0) {
			oled_line(prev_dash_start_x, prev_dash_y, prev_dash_end_x, prev_dash_y, OLED_COLOR_BLACK);

			curr_dash_end_x = prev_dash_start_x - 1;
			curr_dash_start_x = curr_dash_end_x - 5;
			curr_dash_y = prev_dash_y;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			prev_dash_start_x = curr_dash_start_x;
			prev_dash_end_x = curr_dash_end_x;
			prev_dash_y = curr_dash_y;
		}
	}

	if (state & JOYSTICK_RIGHT) {
		if(curr_dash_start_x < 84) {
			oled_line(prev_dash_start_x, prev_dash_y, prev_dash_end_x, prev_dash_y, OLED_COLOR_BLACK);

			curr_dash_start_x = prev_dash_end_x + 1;
			curr_dash_end_x = curr_dash_start_x + 5;
			curr_dash_y = prev_dash_y;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			prev_dash_start_x = curr_dash_start_x;
			prev_dash_end_x = curr_dash_end_x;
			prev_dash_y = curr_dash_y;
		}
	}

	if (state & JOYSTICK_DOWN) {
		if(curr_dash_y == 48) {
			oled_line(prev_dash_start_x, prev_dash_y, prev_dash_end_x, prev_dash_y, OLED_COLOR_BLACK);

			curr_dash_start_x = prev_dash_start_x;
			curr_dash_end_x = prev_dash_end_x;
			curr_dash_y = prev_dash_y + 10;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			prev_dash_start_x = curr_dash_start_x;
			prev_dash_end_x = curr_dash_end_x;
			prev_dash_y = curr_dash_y;
		}
	}

	if (state & JOYSTICK_UP) {
		if(curr_dash_y == 58) {
			oled_line(prev_dash_start_x, prev_dash_y, prev_dash_end_x, prev_dash_y, OLED_COLOR_BLACK);

			curr_dash_start_x = prev_dash_start_x;
			curr_dash_end_x = prev_dash_end_x;
			curr_dash_y = prev_dash_y - 10;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			prev_dash_start_x = curr_dash_start_x;
			prev_dash_end_x = curr_dash_end_x;
			prev_dash_y = curr_dash_y;
		}
	}
}

void message_loop() {
	if (!should_read_joystick) {
		return;
	}

	should_read_joystick = 0;
	state = joystick_read();

	switch (message_mode) {
	case VIEW:
		view_loop();
		break;
	case WRITE:
		write_loop();
	default:
		break;
	}
}
