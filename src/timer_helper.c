/*
 * timer_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "stdint.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "rgb_helper.h"

volatile uint32_t msTicks; // counter for 1ms SysTicks
volatile uint32_t secTicks; // counter for 1s SysTicks

//  SysTick_Handler - just increment SysTick counter
extern void SysTick_Handler(void) {
	msTicks++;
	if (msTicks % 1000 == 0) {
		secTicks++;
	}
}

extern void TIMER0_IRQHandler(void) {
    unsigned int isrMask;

    isrMask = LPC_TIM0->IR;
    LPC_TIM0->IR = isrMask;         /* Clear the Interrupt Bit */

    set_rgb(0);
}


// Setup SysTick Timer to interrupt at 1 msec intervals
void setup_systick_interrupt() {
	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
}

void setup_timer0_interrupt() {
	TIM_TIMERCFG_Type timer0;
	TIM_ConfigStructInit(TIM_TIMER_MODE, &timer0);
	timer0.PrescaleValue = 100000;
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer0);

	TIM_MATCHCFG_Type matchConfig;
	matchConfig.MatchChannel = 0;
	matchConfig.IntOnMatch = SET;
	matchConfig.StopOnMatch = SET;
	matchConfig.ResetOnMatch = SET;
	matchConfig.ExtMatchOutputType = 0;
	matchConfig.MatchValue = 1;

	TIM_ConfigMatch(LPC_TIM0, &matchConfig);

	NVIC_EnableIRQ(TIMER0_IRQn);
}

void enable_timer0_interrupt() {
	TIM_Cmd(LPC_TIM0, ENABLE);
}
