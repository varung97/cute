/* @file	: interface_helper.h
 *
 */

#include "stdint.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"

void configPin(uint8_t func_num, uint8_t open_drain, uint8_t pin_mode, uint8_t port_num, uint8_t pin_num);
void init_i2c();
void init_ssp();
void init_GPIO();
