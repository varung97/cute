/*****************************************************************************
 *   CUTE - Assignment 2, EE2024
 *
 *   Varun Gupta      A0147924X
 *   Shailesh Nahata  A0147619W
 *
 ******************************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "LPC17xx.h"
#include "interface_helper.h"
#include "timer_helper.h"
#include "interrupt_helper.h"

#include "led_helper.h"
#include "joystick.h"
#include "acc_helper.h"
#include "light_helper.h"
#include "temp_helper.h"
#include "oled.h"
#include "rgb_helper.h"
#include "led7seg_helper.h"
#include "speaker_helper.h"


int main(void) {
    i2c_init();
    ssp_init();
    gpio_init();

    timer_interrupt_setup(LPC_TIM0, 1,   100, 1);
    timer_interrupt_setup(LPC_TIM1, 1,  1000, 0);
    timer_interrupt_setup(LPC_TIM2, 1,     1, 0);

    rgb_init();
    leds_init();
    acc_init();
    light_init();
//    speaker_init();
    oled_init();
    led7seg_init();
    temp_init(get_temp_function());

    timer_interrupt_enable(LPC_TIM1);
    eint_interrupt_enable(0);
    eint_interrupt_handler_enable(0);
    systick_interrupt_setup();

    light_enable();
    led7seg_set_number(0);
    oled_clearScreen(OLED_COLOR_BLACK);

    int8_t x, y, z;
    uint32_t light_val, temp_val;
    int var;
    char str_val[5];

    while(1) {
//    	acc_setMode(ACC_MODE_MEASURE);
//    	acc_setRange(ACC_RANGE_4G);
//    	acc_read(&x, &y, &z);
//
//    	light_val = light_read();

//    	temp_val = temp_read();
//    	sprintf(str_val, "%d", temp_val);
//		oled_putString(0, 0, str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
//
//		for (var = 0; var < 100000; ++var);
    }

    return 0;
}

void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

