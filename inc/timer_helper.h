extern volatile uint32_t msTicks;
extern volatile uint32_t secTicks;

extern void SysTick_Handler(void);
extern void TIMER0_IRQHandler(void);
void setup_systick_interrupt(void);
void setup_timer0_interrupt(void);
