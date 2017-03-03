#include "stdint.h"
#include "stdbool.h"
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "rgb_helper.h"
#include "led7seg_helper.h"

extern void SysTick_Handler(void);
extern void TIMER0_IRQHandler(void);
extern void TIMER1_IRQHandler(void);
void setup_systick_interrupt(void);
void setup_timer_interrupt(LPC_TIM_TypeDef* timer, uint32_t prescale_val, uint32_t num_cycles_to_interrupt, uint8_t should_stop);
void enable_timer_interrupt(LPC_TIM_TypeDef* timer);
