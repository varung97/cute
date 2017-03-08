#ifndef TIMER_HELPER_H
#define TIMER_HELPER_H

#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "led7seg_helper.h"
#include "speaker_helper.h"
#include "rgb_helper.h"

#define TIMER_MAX 4
#define TIMER0 0
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3

extern volatile uint32_t ms_ticks;

typedef void (*timer_func_ptr)(void);

typedef struct {
    timer_func_ptr timer_func;
    LPC_TIM_TypeDef *TIMx;
    IRQn_Type IrqNumber;
} timer_config_t;

void systick_interrupt_setup(void);
void timer_interrupt_setup(uint8_t timer_num, uint32_t ms);
void timer_attach_interrupt(uint8_t timer_num, timer_func_ptr timer_func, uint32_t num_cycles_to_interrupt, uint8_t should_stop);
void timer_interrupt_enable(uint8_t timer_num);
void timer_interrupt_disable(uint8_t timer);
void call_timer_func(uint8_t timer_num);

#endif
