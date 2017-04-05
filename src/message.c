/*
 * message.c
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#include "message.h"

void display_keyboard();
void write_loop();

char incoming_message[INCOMING_MAX_LEN];
char outgoing_message[OUTGOING_MAX_LEN + 3]; // For terminating character and CRLF
volatile int uart_new_data_available = 0;
message_mode_type message_mode = WRITE;
uint8_t state;
volatile int should_read_joystick = 0, is_speaker_on = 0;
int curr_dash_start_x = 0, curr_dash_end_x = 5, curr_dash_y = 48;
int curr_char = 0;


/**********************************************************
 * Required function headers
 ***********************************************************/

void enable_write_mode();


/**********************************************************
 * ISRs
 ***********************************************************/

void uart_rxav_isr() {
	uart_new_data_available = uart_receive_notblocking(incoming_message);
	if (uart_new_data_available) {
		timer_interrupt_enable(TIMER1);
		timer_interrupt_enable(TIMER2);
	}
}

void read_joystick_isr() {
	should_read_joystick = 1;
}

void turn_speaker_off() {
	timer_interrupt_disable(TIMER2);
	speaker_off();
}


/**********************************************************
 * Common helper functions
 ***********************************************************/

void reset_write_mode() {
	outgoing_message[0] = '\0';
	curr_char = 0;
	curr_dash_start_x = 0;
	curr_dash_end_x = 5;
	curr_dash_y = 48;
}


/**********************************************************
 * View mode functions
 ***********************************************************/

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
	message_mode = VIEW;
	display_incoming_message();
}

void view_loop() {
	if (uart_new_data_available) {
		uart_new_data_available = 0;
		display_incoming_message();
	}


	// switch to write mode
	if (state & JOYSTICK_CENTER) {
		enable_write_mode();
	}
}


/**********************************************************
 * Write mode functions
 ***********************************************************/

void display_keyboard() {
	oled_clearScreen(OLED_COLOR_BLACK);

	int i;
	for (i = 0; i < 15; i++) {
		oled_putChar(i * CHARACTER_WIDTH, FIRST_KEYBOARD_COLUMN, 'a' + i, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	}
	for (i = 0; i < 11; i++) {
		oled_putChar(i * CHARACTER_WIDTH, SECOND_KEYBOARD_COLUMN, 'p' + i, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	}
	oled_putChar(66, SECOND_KEYBOARD_COLUMN, 0x82, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_putChar(72, SECOND_KEYBOARD_COLUMN, 0x80, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_putChar(79, SECOND_KEYBOARD_COLUMN, 'X', OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_putChar(86, SECOND_KEYBOARD_COLUMN, 0x81, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);
}

void outgoing_add_char(char chr) {
	int len = strlen(outgoing_message);
	if (len == OUTGOING_MAX_LEN) return;

	outgoing_message[len] = chr;
	outgoing_message[len + 1] = '\0';

	int col = len % 15;
	int row = len / 15;
	oled_putChar(col * CHARACTER_WIDTH, row * ROW_HEIGHT, chr, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void outgoing_del_char() {
	int len = strlen(outgoing_message);
	if (len == 0) return;

	outgoing_message[len - 1] = '\0';

	int col = (len - 1) % 15;
	int row = (len - 1) / 15;
	oled_putChar(col * CHARACTER_WIDTH, row * ROW_HEIGHT, ' ', OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void outgoing_message_send() {
	int len = strlen(outgoing_message);
	if (len == 0) return;

	outgoing_message[len] = '\r';
	outgoing_message[len + 1] = '\n';
	outgoing_message[len + 2] = '\0';
	uart_send_notblocking(outgoing_message);

	timer_interrupt_enable(TIMER1);
	timer_interrupt_enable(TIMER2);
}

void enable_write_mode() {
	message_mode = WRITE;
	reset_write_mode();
	display_keyboard();
}

void write_loop() {

	if (state & JOYSTICK_CENTER) {
		if (curr_char == 29) {
			// Send
			outgoing_message_send();
			enable_view_mode();
		} else if (curr_char == 28) {
			// Cancel
			enable_view_mode();
		} else if (curr_char == 27) {
			// Backspace
			outgoing_del_char();
		} else if (curr_char == 26) {
			// Space
			outgoing_add_char(' ');
		} else {
			outgoing_add_char('a' + curr_char);
		}
	}

	if (state & JOYSTICK_LEFT) {
		if(curr_dash_start_x > OLED_LEFT_BOUND) {
			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_BLACK);

			curr_dash_end_x = curr_dash_start_x - 1;
			curr_dash_start_x = curr_dash_end_x - DASH_WIDTH;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			curr_char--;
		}
	}

	if (state & JOYSTICK_RIGHT) {
		if(curr_dash_start_x < OLED_RIGHT_BOUND) {
			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_BLACK);

			curr_dash_start_x = curr_dash_end_x + 1;
			curr_dash_end_x = curr_dash_start_x + DASH_WIDTH;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			curr_char++;
		}
	}

	if (state & JOYSTICK_DOWN) {
		if(curr_dash_y == FIRSTLINE_DASH_Y) {
			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_BLACK);

			curr_dash_y += ROW_HEIGHT;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			curr_char += 15;
		}
	}

	if (state & JOYSTICK_UP) {
		if(curr_dash_y == SECONDLINE_DASH_Y) {
			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_BLACK);

			curr_dash_y -= ROW_HEIGHT;

			oled_line(curr_dash_start_x, curr_dash_y, curr_dash_end_x, curr_dash_y, OLED_COLOR_WHITE);

			curr_char -= 15;
		}
	}
}


/**********************************************************
 * Message mode functions
 ***********************************************************/

void enable_message_mode() {
	reset_board();

	enable_view_mode();

	timer_attach_interrupt(TIMER1, turn_speaker_off, 200, 1);
	timer_attach_interrupt(TIMER2, speaker_toggle, 1, 0);
	timer_interrupt_enable(TIMER3);

	uart_enable();
	uart_specific_interrupt_cmd(RXAV, ENABLE);
	uart_interrupt_enable();
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
