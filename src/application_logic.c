/*
 * application_logic.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "application_logic.h"

#define LIGHT_LOW_WARNING 40
#define LIGHT_HIGH_THRESHOLD 200
#define TEMP_HIGH_WARNING 450

uint8_t led7seg_display_val;

mode_type current_mode = PASSIVE;
int led = 0;
int8_t x, y, z;
int32_t temp_val;
uint32_t light_val;
char str_val[12];

volatile int is_new_second = 0, should_toggle_mode = 0;

void enable_monitor_mode() {
	current_mode = MONITOR;

	led7seg_display_val = 0;
	led7seg_set_number(led7seg_display_val);
	timer_interrupt_enable(TIMER1);
	acc_setMode(ACC_MODE_MEASURE);
	light_enable();
	temp_init(&get_ms_ticks);
	oled_putString(0, 0, (uint8_t *) "MONITOR", OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void enable_passive_mode() {
	current_mode = PASSIVE;

	led7seg_set_raw(0xFF);
	timer_interrupt_disable(TIMER1);
	acc_setMode(ACC_MODE_STANDBY);
	light_shutdown();
	oled_clearScreen(OLED_COLOR_BLACK);
}

void toggle_mode() {
	switch (current_mode) {
	case PASSIVE:
		enable_monitor_mode();
		break;
	case MONITOR:
		enable_passive_mode();
		break;
	default:
		break;
	}
}

void toggle_isr() {
	should_toggle_mode = 1;
}

void turn_off_blinking_rgb() {
	rgb_also_clear_red();
}

void display_values() {
	sprintf(str_val, "Temp: %.1f", temp_val/10.0);
	oled_putString(0, 10, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "Light: %d", (int) light_val);
	oled_putString(0, 20, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AX: %d", (int) x);
	oled_putString(0, 30, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AY: %d", (int) y);
	oled_putString(0, 40, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AZ: %d", (int) z);
	oled_putString(0, 50, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void do_every_second() {
	is_new_second = 1;
}

void loop() {
	if (should_toggle_mode) {
		should_toggle_mode = 0;

		toggle_mode();
	}

	if (is_new_second) {
		is_new_second = 0;

		led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
		led7seg_set_number(led7seg_display_val);

		if (led7seg_display_val == 5 ||
			led7seg_display_val == 10 ||
			led7seg_display_val == 15) {

			acc_read(&x, &y, &z);
			temp_val = temp_read();
			light_val = light_read();

			display_values();
		}

		if (led7seg_display_val == 15) {
			// Send values through UART
		}
	}
}

void eint3_isr(void) {
	if(did_gpio_interrupt_occur(2, 5)) {
		gpio_interrupt_clear(2, 5);
		light_clearIrqStatus();
		led = !led;
		if(led)
			leds_also_turn_on(0x1);
		else
			leds_also_turn_off(0x1);
	}
//	leds_also_turn_off(0x1);
}

void read_light_sensor() {
	pin_config(0, 0, 2, 2, 5);
	pin_set_dir(2, 5, 0);
	light_setMode(LIGHT_MODE_D1);
	light_setRange(LIGHT_RANGE_1000);
	light_setWidth(LIGHT_WIDTH_16BITS);
	light_setLoThreshold(LIGHT_LOW_WARNING);
	light_setHiThreshold(LIGHT_HIGH_THRESHOLD);
	light_setIrqInCycles(LIGHT_CYCLE_1);
	light_clearIrqStatus();
	gpio_interrupt_enable(2, 5);
	eint_attach_interrupt(3, eint3_isr);
	NVIC_ClearPendingIRQ(EINT3_IRQn);
}
