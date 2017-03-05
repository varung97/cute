#include "interface_helper.h"
#include "timer_helper.h"
#include "stdint.h"

void gpio_interrupt_enable(uint8_t port_num, uint8_t pin_num);
void gpio_interrupt_clear(uint8_t port_num, uint8_t pin_num);
int did_gpio_interrupt_occur(uint8_t port_num, uint8_t pin_num);
void eint_interrupt_enable(int int_number);
void eint_interrupt_clear(int int_number);
void eint_interrupt_handler_enable(int int_number);
extern void EINT0_IRQHandler(void);
