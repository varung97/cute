extern volatile uint32_t ms_ticks;
extern volatile uint32_t sec_ticks;

extern void SysTick_Handler(void);
extern void TIMER0_IRQHandler(void);
void setup_systick_interrupt(void);
void setup_timer0_interrupt(void);
