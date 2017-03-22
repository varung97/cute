/*
 * application_logic.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "application_logic.h"

uint8_t led7seg_display_val;

mode_type current_mode = PASSIVE;
int led = 0;
int8_t x, y, z;
int32_t temp_val;
uint32_t light_val;
char str_val[12];
char empty_string[6] = {32, 32, 32, 32, 32, 32};

volatile int is_new_second = 0, should_toggle_mode = 0, is_rgb_leds_on = 0, is_blue_rgb_blinking = 0, is_red_rgb_blinking = 0;

uint32_t prev_ms_temp = 0;
volatile int current_temp_edges = 0;

void enable_monitor_mode() {
	current_mode = MONITOR;

	led7seg_display_val = is_blue_rgb_blinking = is_red_rgb_blinking = 0;
	led7seg_set_number(led7seg_display_val);
	timer_interrupt_enable(TIMER1);
	acc_setMode(ACC_MODE_MEASURE);
	light_enable();
	temp_init(&get_ms_ticks);
	oled_putString(0, 0, (uint8_t *) "MONITOR", OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	timer_interrupt_enable(TIMER0);

	eint_interrupt_handler_enable(EINT3);
	current_temp_edges = 0;
	prev_ms_temp = get_ms_ticks();
}

void enable_passive_mode() {
	current_mode = PASSIVE;

	is_blue_rgb_blinking = is_red_rgb_blinking = 0;
	rgb_also_clear_blue();
	rgb_also_clear_red();

	timer_interrupt_disable(TIMER0);
	timer_interrupt_disable(TIMER1);
	led7seg_set_raw(0xFF);
	acc_setMode(ACC_MODE_STANDBY);
	light_shutdown();
	oled_clearScreen(OLED_COLOR_BLACK);

	eint_interrupt_handler_disable(EINT3);
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

void toggle_leds() {
	is_rgb_leds_on = !is_rgb_leds_on;

	if (is_blue_rgb_blinking) {
		is_rgb_leds_on ? rgb_also_set_blue() : rgb_also_clear_blue();
	}

	if (is_red_rgb_blinking) {
		is_rgb_leds_on ? rgb_also_set_red() : rgb_also_clear_red();
	}
}

void write_empty_string(uint8_t col, uint8_t row) {
	oled_putString(col, row, (uint8_t *) empty_string, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void display_values() {
	write_empty_string(47, 10);
	sprintf(str_val, "Temp: %.1f", temp_val/10.0);
	oled_putString(0, 10, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	write_empty_string(55, 20);
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
			light_val = light_read();

			eint_interrupt_handler_disable(EINT3);
			display_values();
			eint_interrupt_handler_enable(EINT3);
			current_temp_edges = 0;
			prev_ms_temp = get_ms_ticks();
		}

		if (led7seg_display_val == 15) {
			// Send values through UART
		}
	}

	if (current_temp_edges == 333) {
		current_temp_edges = 0;
		temp_val = (get_ms_ticks() - prev_ms_temp) * 3 - 2731;
		prev_ms_temp = get_ms_ticks();
		if (temp_val > TEMP_HIGH_WARNING) {
			is_red_rgb_blinking = 1;
		}
	}
}

void eint3_isr(void) {
	if (did_gpio_interrupt_occur(2, 5)) {
		// light interrupt
		gpio_interrupt_clear(2, 5);
		light_clearIrqStatus();
		if (!is_blue_rgb_blinking) {
			is_blue_rgb_blinking = 1;
		}
	}
	if (did_gpio_interrupt_occur(0, 2)) {
		gpio_interrupt_clear(0, 2);
		current_temp_edges++;
	}
}
