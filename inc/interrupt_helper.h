#ifndef INTERRUPT_HELPER_H
#define INTERRUPT_HELPER_H

#include "interface_helper.h"
#include "timer_helper.h"

#define EINT_MAX 4
#define EINT0 0
#define EINT1 1
#define EINT2 2
#define EINT3 3

typedef void (*eint_func_ptr)(void);

typedef struct {
    eint_func_ptr eint_func;
    IRQn_Type IrqNumber;
    uint8_t pin_num;
} eint_config_t;

void gpio_interrupt_enable(uint8_t port_num, uint8_t pin_num);
void gpio_interrupt_clear(uint8_t port_num, uint8_t pin_num);
int did_gpio_interrupt_occur(uint8_t port_num, uint8_t pin_num);
void eint_attach_interrupt(uint8_t int_number, eint_func_ptr func_ptr);
void eint_interrupt_enable(uint8_t int_number);
void eint_interrupt_clear(uint8_t int_number);
void eint_interrupt_handler_enable(uint8_t int_number);

#endif
