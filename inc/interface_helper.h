/* @file	: interface_helper.h
 *
 */

#ifndef INTERFACE_HELPER_H_
#define INTERFACE_HELPER_H_

#include "string.h"
#include "stdint.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_uart.h"

typedef enum {THRE, RXAV} uart_int_type;
typedef void (*uart_int_func_ptr)(void);

void pin_config(uint8_t func_num, uint8_t open_drain, uint8_t pin_mode, uint8_t port_num, uint8_t pin_num);
void pin_set_dir(uint8_t port_num, uint8_t pin_num, int dir);
void pin_set_val(uint8_t port_num, uint8_t pin_num);
void pin_clear_val(uint8_t port_num, uint8_t pin_num);
int pin_read_val(uint8_t port_num, uint8_t pin_num);
void i2c_init();
void ssp_init();
void gpio_init();
void uart_init();
void uart_enable();
void uart_disable();
void uart_interrupt_enable();
void uart_interrupt_disable();
void uart_specific_interrupt_cmd(uart_int_type int_type, FunctionalState command);
void uart_send(char str[]);
void uart_send_notblocking(char str[]);
int uart_receive_notblocking(char* bufin);
void uart_queue_into_fifo();
void uart_attach_interrupt(uart_int_type int_type, uart_int_func_ptr func_ptr);

#endif /* INTERFACE_HELPER_H_ */
