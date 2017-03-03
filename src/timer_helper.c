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

volatile uint32_t ms_ticks; // counter for 1ms SysTicks
volatile uint32_t sec_ticks; // counter for 1s SysTicks

//  SysTick_Handler - just increment SysTick counter
extern void SysTick_Handler(void) {
	ms_ticks++;
	if (ms_ticks % 1000 == 0) {
		sec_ticks++;
	}
}

extern void TIMER0_IRQHandler(void) {
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

    clear_all_rgb();
}


// Setup SysTick Timer to interrupt at 1 msec intervals
void setup_systick_interrupt() {
	if (SysTick_Config(SystemCoreClock / 1000)) {
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

void setup_timer_interrupt(LPC_TIM_TypeDef* timer, uint32_t ms, uint32_t num_cycles_to_interrupt, uint8_t should_reset) {
	TIM_TIMERCFG_Type timer_config;
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timer_config);
	timer_config.PrescaleValue = ms * 1000;
	TIM_Init(timer, TIM_TIMER_MODE, &timer_config);

	TIM_MATCHCFG_Type matchConfig;
	matchConfig.MatchChannel = 0;
	matchConfig.IntOnMatch = ENABLE;
	matchConfig.StopOnMatch = should_reset ? ENABLE : DISABLE;
	matchConfig.ResetOnMatch = should_reset ? ENABLE : DISABLE;
	matchConfig.ExtMatchOutputType = 0;
	matchConfig.MatchValue = num_cycles_to_interrupt;

	TIM_ConfigMatch(timer, &matchConfig);

	NVIC_EnableIRQ(get_interrupt_handler(timer));
}

void enable_timer_interrupt(LPC_TIM_TypeDef* timer) {
	TIM_Cmd(timer, ENABLE);
}
