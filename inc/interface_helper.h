/* @file	: interface_helper.h
 *
 */

#include "stdint.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"

void pin_config(uint8_t func_num, uint8_t open_drain, uint8_t pin_mode, uint8_t port_num, uint8_t pin_num);
void pin_set_dir(uint8_t port_num, uint8_t pin_num, int dir);
void pin_set_val(uint8_t port_num, uint8_t pin_num);
void pin_clear_val(uint8_t port_num, uint8_t pin_num);
int pin_read_val(uint8_t port_num, uint8_t pin_num);
void i2c_init();
void ssp_init();
void gpio_init();
