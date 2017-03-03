/*
 * timer_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "stdint.h"
#include "stdbool.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "rgb_helper.h"


uint8_t led7seg_display_val = 0;

// Called every second
extern void SysTick_Handler(void) {
	led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
	set_number_led7seg(led7seg_display_val);

	set_rgb(RGB_BLUE);
	enable_timer_interrupt(LPC_TIM0);
}

extern void TIMER0_IRQHandler(void) {
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

    clear_all_rgb();
}


// Setup SysTick Timer to interrupt at 1 msec intervals
void setup_systick_interrupt() {
	if (SysTick_Config(SystemCoreClock)) {
		while (1);  // Capture error
	}
}

uint8_t get_interrupt_handler(LPC_TIM_TypeDef* timer) {
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

void setup_timer_interrupt(LPC_TIM_TypeDef* timer, uint32_t ms, uint32_t num_cycles_to_interrupt, uint8_t should_stop) {
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

void enable_timer_interrupt(LPC_TIM_TypeDef* timer) {
	TIM_Cmd(timer, ENABLE);
}
