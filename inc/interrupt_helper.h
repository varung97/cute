#include "interface_helper.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "stdint.h"

void enable_gpio_interrupt(uint8_t port_num, uint8_t pin_num);
void clear_gpio_interrupt(uint8_t port_num, uint8_t pin_num);
int did_interrupt_occur(uint8_t port_num, uint8_t pin_num);
void enable_eint_interrupt(int int_number);
void enable_eint_interrupt_handler(int int_number);
extern void EINT3_IRQHandler(void);
