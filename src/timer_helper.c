/*
 * timer_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "timer_helper.h"

uint8_t led7seg_display_val = 0;
volatile uint32_t ms_ticks;

// Called every second
extern void SysTick_Handler(void) {
	ms_ticks++;
}

extern void TIMER0_IRQHandler(void) {
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

    rgb_also_clear_blue();
}

extern void TIMER1_IRQHandler(void) {
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

	led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
	led7seg_set_number(led7seg_display_val);

	rgb_also_set_blue();
	timer_interrupt_enable(LPC_TIM0);
}

extern void TIMER2_IRQHandler(void) {
	TIM_ClearIntPending(LPC_TIM2, TIM_MR0_INT);

	if (is_speaker_on) {
		speaker_off();
	} else {
		speaker_on();
	}
	is_speaker_on = !is_speaker_on;
}


// Setup SysTick Timer to interrupt at 1 msec intervals
void systick_interrupt_setup() {
	ms_ticks = 0;

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
}

IRQn_Type get_interrupt_handler(LPC_TIM_TypeDef* timer) {
	if (timer == LPC_TIM0) {
		return TIMER0_IRQn;
	}
	else if (timer == LPC_TIM1) {
		return TIMER1_IRQn;
	}
	else if (timer == LPC_TIM2) {
		return TIMER2_IRQn;
	}
	else {
		return TIMER3_IRQn;
	}
}

void timer_interrupt_setup(LPC_TIM_TypeDef* timer, uint32_t ms, uint32_t num_cycles_to_interrupt, uint8_t should_stop) {
	TIM_TIMERCFG_Type timer_config;
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timer_config);
	timer_config.PrescaleValue = ms * 1000;
	TIM_Init(timer, TIM_TIMER_MODE, &timer_config);

	TIM_MATCHCFG_Type matchConfig;
	matchConfig.MatchChannel = 0;
	matchConfig.IntOnMatch = ENABLE;
	matchConfig.StopOnMatch = should_stop ? ENABLE : DISABLE;
	matchConfig.ResetOnMatch = ENABLE;
	matchConfig.ExtMatchOutputType = 0;
	matchConfig.MatchValue = num_cycles_to_interrupt;

	TIM_ConfigMatch(timer, &matchConfig);

	NVIC_EnableIRQ(get_interrupt_handler(timer));
}

void timer_interrupt_enable(LPC_TIM_TypeDef* timer) {
	TIM_Cmd(timer, ENABLE);
}

void timer_interrupt_disable(LPC_TIM_TypeDef* timer) {
	TIM_Cmd(timer, DISABLE);
}
