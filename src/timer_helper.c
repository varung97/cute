/*
 * timer_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "timer_helper.h"

volatile uint32_t ms_ticks;
char str_bleh[18];

timer_config_t timer_config_table[TIMER_MAX] = {
   { NULL, LPC_TIM0, TIMER0_IRQn, },
   { NULL, LPC_TIM1, TIMER1_IRQn, },
   { NULL, LPC_TIM2, TIMER2_IRQn, },
   { NULL, LPC_TIM3, TIMER3_IRQn, },
};

// Called every milliseconds
void SysTick_Handler(void) {
	ms_ticks++;
}

uint32_t get_ms_ticks() {
	return ms_ticks;
}

// Setup SysTick Timer to interrupt at 1 msec intervals
void systick_interrupt_init() {
	ms_ticks = 0;

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
}

void timer_interrupt_setup(uint8_t timer_num, uint32_t us) {
	if (timer_num >= TIMER_MAX) return;

	LPC_TIM_TypeDef* timer = timer_config_table[timer_num].TIMx;
	TIM_TIMERCFG_Type timer_config;
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timer_config);
	timer_config.PrescaleValue = us;
	TIM_Init(timer, TIM_TIMER_MODE, &timer_config);
}

void timer_attach_interrupt(uint8_t timer_num, timer_func_ptr timer_func, uint32_t num_cycles_to_interrupt, uint8_t should_stop) {
	if (timer_num >= TIMER_MAX) return;

	LPC_TIM_TypeDef* timer = timer_config_table[timer_num].TIMx;

	TIM_MATCHCFG_Type matchConfig;
	matchConfig.MatchChannel = 0;
	matchConfig.IntOnMatch = ENABLE;
	matchConfig.StopOnMatch = should_stop ? ENABLE : DISABLE;
	matchConfig.ResetOnMatch = ENABLE;
	matchConfig.ExtMatchOutputType = 0;
	matchConfig.MatchValue = num_cycles_to_interrupt;

	TIM_ConfigMatch(timer, &matchConfig);

	timer_config_table[timer_num].timer_func = timer_func;
}

void timer_interrupt_enable(uint8_t timer_num) {
	if (timer_num >= TIMER_MAX) return;

	NVIC_ClearPendingIRQ(timer_config_table[timer_num].IrqNumber);
	NVIC_EnableIRQ(timer_config_table[timer_num].IrqNumber);
	TIM_Cmd(timer_config_table[timer_num].TIMx, ENABLE);
}

void timer_interrupt_disable(uint8_t timer_num) {
	if (timer_num >= TIMER_MAX) return;

	NVIC_DisableIRQ(timer_config_table[timer_num].IrqNumber);
	TIM_Cmd(timer_config_table[timer_num].TIMx, DISABLE);
}

void call_timer_func(uint8_t timer_num) {
	if (timer_num >= TIMER_MAX) return;

	if (timer_config_table[timer_num].timer_func != NULL) {
		timer_config_table[timer_num].timer_func();
	}
}

void TIMER0_IRQHandler(void) {
    TIM_ClearIntPending(timer_config_table[TIMER0].TIMx, TIM_MR0_INT);

    call_timer_func(TIMER0);
}

void TIMER1_IRQHandler(void) {
    TIM_ClearIntPending(timer_config_table[TIMER1].TIMx, TIM_MR0_INT);

    call_timer_func(TIMER1);
}

void TIMER2_IRQHandler(void) {
	TIM_ClearIntPending(timer_config_table[TIMER2].TIMx, TIM_MR0_INT);

	call_timer_func(TIMER2);
}

void TIMER3_IRQHandler(void) {
	TIM_ClearIntPending(timer_config_table[TIMER3].TIMx, TIM_MR0_INT);

	call_timer_func(TIMER3);
}
