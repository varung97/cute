/*
 * timer_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "stdint.h"
#include "LPC17xx.h"

volatile uint32_t msTicks; // counter for 1ms SysTicks
volatile uint32_t secTicks; // counter for 1s SysTicks

//  SysTick_Handler - just increment SysTick counter
void SysTick_Handler(void) {
	msTicks++;
	if (msTicks % 1000 == 0) {
		secTicks++;
	}
}

// Setup SysTick Timer to interrupt at 1 msec intervals
void setup_timer_interrupt() {
	msTicks = 0;
	secTicks = 0;

	if (SysTick_Config(SystemCoreClock / 1000)) {
		while (1);  // Capture error
	}
}
