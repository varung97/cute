/*
 * interface_helper.c
 *
 *  Created on: Mar 1, 2017
 *      Author: Varun
 */

#include "stdint.h"
#include "lpc17xx_pinsel.h"

PINSEL_CFG_Type PinCfg;

void configPin(uint8_t func_num, uint8_t open_drain, uint8_t pin_mode, uint8_t port_num, uint8_t pin_num) {
	PinCfg.Funcnum = func_num;
	PinCfg.OpenDrain = open_drain;
	PinCfg.Pinmode = pin_mode;
	PinCfg.Portnum = port_num;
	PinCfg.Pinnum = pin_num;
	PINSEL_ConfigPin(&PinCfg);
}


