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
	// Initialize button
	pin_config(0, 0, 0, 1, 31);
	GPIO_SetDir(1, 1<<31, 0);
}
