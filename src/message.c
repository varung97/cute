/*
 * message.c
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#include "message.h"

char uart_recv[120];
volatile int uart_new_data_available = 0;

void enable_message_mode() {
	reset_board();

	uart_enable();
	uart_specific_interrupt_cmd(RXAV, ENABLE);

	oled_putString(0, 0, (uint8_t *) "MESSAGE", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	uart_send_notblocking("Message Mode\r\n");
}

void uart_rxav_isr() {
	uart_new_data_available = uart_receive_notblocking(uart_recv);
}

void message_loop() {
	if (uart_new_data_available) {
		uart_new_data_available = 0;
		oled_putString(0, 0, (uint8_t *) uart_recv, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
	}
}
