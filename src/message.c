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
volatile int uart_new_data_available = 0, should_read_joystick = 0, should_play_note = 0;
message_mode_type message_mode = WRITE;
uint8_t state = 0;
int curr_dash_start_x = 0, curr_dash_end_x = 5, curr_dash_y = 48;
int curr_char = 0;
uint32_t notes[] = {170, 114, 85};
int num_notes = 3, curr_note = 0;


/**********************************************************
 * Required function headers
 ***********************************************************/

void enable_write_mode();
void play_note();


/**********************************************************
 * ISRs
 ***********************************************************/

void uart_rxav_isr() {
	uart_new_data_available = uart_receive_notblocking(incoming_message);
}

void read_joystick_isr() {
	should_read_joystick = 1;
}

void play_note_isr() {
	should_play_note = 1;
}


/**********************************************************
 * Common helper functions
 ***********************************************************/

void play_note() {
	timer_interrupt_disable(TIMER2);
	timer_interrupt_disable(TIMER1);

	if (curr_note == num_notes) {
		curr_note = 0;
		speaker_off();
	} else {
		timer_interrupt_setup(TIMER2, 10);
		timer_attach_interrupt(TIMER2, speaker_toggle, notes[curr_note], 0);
		curr_note++;
		timer_interrupt_enable(TIMER2);
		timer_interrupt_enable(TIMER1);
	}
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
		if (curr_note == 0) play_note();
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

	if (curr_note == 0) play_note();
}

void reset_write_mode() {
	outgoing_message[0] = '\0';
	curr_char = 0;
	curr_dash_start_x = 0;
	curr_dash_end_x = 5;
	curr_dash_y = 48;
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

	speaker_init();

	timer_interrupt_setup(TIMER2, 10);
	timer_interrupt_setup(TIMER1, 1000);
	timer_attach_interrupt(TIMER1, play_note_isr, 150, 1);
	timer_interrupt_enable(TIMER3);

	uart_enable();
	uart_specific_interrupt_cmd(RXAV, ENABLE);
	uart_interrupt_enable();

	curr_note = should_read_joystick = 0;

	uart_send_notblocking("Entering Message Mode\r\n");
}

void message_loop() {
	if (should_play_note) {
		should_play_note = 0;

		play_note();
	}

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
