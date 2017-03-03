#include "stdint.h"

extern volatile uint32_t ms_ticks;
extern volatile uint32_t sec_ticks;

extern void SysTick_Handler(void);
extern void TIMER0_IRQHandler(void);
void setup_systick_interrupt(void);
void setup_timer_interrupt(LPC_TIM_TypeDef* timer, uint32_t prescale_val, uint32_t num_cycles_to_interrupt, uint8_t should_reset);
void enable_timer0_interrupt(LPC_TIM_TypeDef* timer);
