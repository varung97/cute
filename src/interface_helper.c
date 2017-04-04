/*
 * interface_helper.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Varun
 */

#include "interface_helper.h"

PINSEL_CFG_Type PinCfg;

void pin_config(uint8_t func_num, uint8_t open_drain, uint8_t pin_mode, uint8_t port_num, uint8_t pin_num) {
	PinCfg.Funcnum = func_num;
	PinCfg.OpenDrain = open_drain;
	PinCfg.Pinmode = pin_mode;
	PinCfg.Portnum = port_num;
	PinCfg.Pinnum = pin_num;
	PINSEL_ConfigPin(&PinCfg);
}

void pin_set_dir(uint8_t port_num, uint8_t pin_num, int dir) {
	GPIO_SetDir(port_num, 1 << pin_num, dir);
}

void pin_set_val(uint8_t port_num, uint8_t pin_num) {
	GPIO_SetValue(port_num, 1 << pin_num);
}

void pin_clear_val(uint8_t port_num, uint8_t pin_num) {
	GPIO_ClearValue(port_num, 1<<pin_num);
}

int pin_read_val(uint8_t port_num, uint8_t pin_num) {
	return (GPIO_ReadValue(port_num) >> pin_num) & 0x1;
}

void ssp_init(void) {
	SSP_CFG_Type SSP_ConfigStruct;

	/*
	 * Initialize SPI pin connect
	 * P0.7 - SCK;
	 * P0.8 - MISO
	 * P0.9 - MOSI
	 * P2.2 - SSEL - used as GPIO
	 */
	pin_config(2, 0, 0, 0, 7);
	pin_config(2, 0, 0, 0, 8);
	pin_config(2, 0, 0, 0, 9);
	pin_config(0, 0, 0, 2, 2);

	SSP_ConfigStructInit(&SSP_ConfigStruct);

	// Initialize SSP peripheral with parameter given in structure above
	SSP_Init(LPC_SSP1, &SSP_ConfigStruct);

	// Enable SSP peripheral
	SSP_Cmd(LPC_SSP1, ENABLE);

}

void i2c_init(void) {
	/* Initialize I2C2 pin connect */
	pin_config(2, 0, 0, 0, 10);
	pin_config(2, 0, 0, 0, 11);

	// Initialize I2C peripheral
	I2C_Init(LPC_I2C2, 100000);

	/* Enable I2C1 operation */
	I2C_Cmd(LPC_I2C2, ENABLE);
}

void gpio_init(void) {
	// Light interrupt
	pin_config(0, 0, 2, 2, 5);
	pin_set_dir(2, 5, 0);

	// SW3
	pin_config(0, 0, 0, 2, 10);
	pin_set_dir(2, 10, 0);

	// SW4
	pin_config(0, 0, 0, 1, 31);
	pin_set_dir(1, 31, 0);
}

void uart_init(void) {
	pin_config(2, 0, 0, 0, 0);
	pin_config(2, 0, 0, 0, 1);

	UART_CFG_Type uartCfg;
	uartCfg.Baud_rate = 115200;
	uartCfg.Databits = UART_DATABIT_8;
	uartCfg.Parity = UART_PARITY_NONE;
	uartCfg.Stopbits = UART_STOPBIT_1;

	UART_Init(LPC_UART3, &uartCfg);
}

void uart_enable(void) {
	UART_SendData(LPC_UART3, 0);
	UART_SendData(LPC_UART3, 0);

	UART_FIFO_CFG_Type fifoCfg;
	UART_FIFOConfigStructInit(&fifoCfg);
	fifoCfg.FIFO_Level = UART_FIFO_TRGLEV3;
	UART_FIFOConfig(LPC_UART3, &fifoCfg);

	UART_TxCmd(LPC_UART3, ENABLE);
}

void uart_disable(void) {
	UART_TxCmd(LPC_UART3, DISABLE);
}

void uart_interrupt_enable() {
	NVIC_EnableIRQ(UART3_IRQn);
}

void uart_interrupt_disable() {
	NVIC_DisableIRQ(UART3_IRQn);
}

void uart_specific_interrupt_cmd(uart_int_type int_type, FunctionalState command) {
	switch (int_type) {
		case THRE:
			UART_IntConfig(LPC_UART3, UART_INTCFG_THRE, command);
			break;
		case RXAV:
			UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, command);
			break;
		default:
			break;
	}
}

void uart_send(char str[]) {
	UART_SendString(LPC_UART3, (uint8_t *) str);
}

char* to_send;
char* buf;
int amt_left, amt_recv, uart_receiving_in_progress = 0;

/**
 * Should have max length of 100 chars
 */
void uart_send_notblocking(char str[]) {
	to_send = str;
	amt_left = strlen(str);

	uart_specific_interrupt_cmd(THRE, ENABLE);
}

void uart_queue_into_fifo() {
	UART_Send(LPC_UART3, (uint8_t *) to_send, amt_left <= UART_TX_FIFO_SIZE ? amt_left : UART_TX_FIFO_SIZE, NONE_BLOCKING);

	amt_left -= UART_TX_FIFO_SIZE;
	if (amt_left <= 0) {
		uart_specific_interrupt_cmd(THRE, DISABLE);
	} else {
		to_send += UART_TX_FIFO_SIZE;
	}
}

// Returns whether terminating character was received
int uart_get_from_rbr() {
	uint32_t amt_just_recv = UART_Receive(LPC_UART3, (uint8_t *) buf, 16, NONE_BLOCKING);
	amt_recv += amt_just_recv;
	buf += amt_just_recv;
	return (buf - 1)[0] == '\n';
}

// Returns whether terminating character was received
int uart_receive_notblocking(char* bufin) {
	if (!uart_receiving_in_progress) {
		uart_receiving_in_progress = 1;
		buf = bufin;
		amt_recv = 0;
	}

	if (uart_get_from_rbr()) {
		// Receiving has ended
		uart_receiving_in_progress = 0;

		// Terminate string instead of \r char
		(buf - 2)[0] = '\0';
		return 1;
	} else {
		return 0;
	}
}

void uart_attach_interrupt(uart_int_type int_type, uart_int_func_ptr func_ptr) {
	switch (int_type) {
		case THRE:
			UART_SetupCbs(LPC_UART3, 1, func_ptr);
			break;
		case RXAV:
			UART_SetupCbs(LPC_UART3, 0, func_ptr);
		default:
			break;
	}
}

void UART3_IRQHandler() {
	UART3_StdIntHandler();
}
