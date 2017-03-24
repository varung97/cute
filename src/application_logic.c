/*
 * application_logic.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "application_logic.h"

#define LIGHT_RANGE 973

uint8_t led7seg_display_val;

mode_type current_mode = PASSIVE;
volatile int led = 0;
int8_t x, y, z;
int32_t temp_val;
uint32_t light_val;
char str_val[12];

volatile int is_new_second = 0,
		     should_toggle_mode = 0,
		     is_rgb_leds_on = 0,
		     is_blue_rgb_blinking = 0,
		     is_red_rgb_blinking = 0,
		     pwm_count = 0,
		     pwm_val = 10;

uint32_t prev_ms_temp = 0;
volatile int current_temp_edges = 0;

void turn_off_blue_and_red_rgbs() {
	rgb_also_clear_blue();
	rgb_also_clear_red();
}

void conditionally_turn_on_blue_and_red_rgbs() {
	is_blue_rgb_blinking ? rgb_also_set_blue() : rgb_also_clear_blue();
	is_red_rgb_blinking  ? rgb_also_set_red()  : rgb_also_clear_red();
}

void enable_monitor_mode() {
	current_mode = MONITOR;

	led7seg_display_val = is_blue_rgb_blinking = is_red_rgb_blinking = 0;
	pwm_val = 10;

	led7seg_set_number(led7seg_display_val);
	acc_setMode(ACC_MODE_MEASURE);
	light_enable();
	oled_putString(0, 0, (uint8_t *) "MONITOR", OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	timer_interrupt_enable(TIMER0);
	timer_interrupt_enable(TIMER1);

	light_clearIrqStatus();
	gpio_interrupt_clear(2, 5);
	gpio_interrupt_clear(0, 2);

	eint_interrupt_handler_enable(EINT3);
	current_temp_edges = 0;
	prev_ms_temp = get_ms_ticks();
}

void enable_passive_mode() {
	current_mode = PASSIVE;

	led7seg_set_raw(0xFF);
	acc_setMode(ACC_MODE_STANDBY);
	light_shutdown();
	oled_clearScreen(OLED_COLOR_BLACK);

	turn_off_blue_and_red_rgbs();

	timer_interrupt_disable(TIMER0);
	timer_interrupt_disable(TIMER1);
	timer_interrupt_disable(TIMER2);

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

void toggle_leds() {
	is_rgb_leds_on = !is_rgb_leds_on;

	if (is_rgb_leds_on) {
		pwm_count = 19;
		timer_interrupt_enable(TIMER2);
	} else {
		timer_interrupt_disable(TIMER2);
		turn_off_blue_and_red_rgbs();
	}
}

void display_values() {
	sprintf(str_val, "Temp: %.1f   ", temp_val/10.0);
	oled_putString(0, 10, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "Light: %d   ", (int) light_val);
	oled_putString(0, 20, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AX: %d   ", (int) x);
	oled_putString(0, 30, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AY: %d   ", (int) y);
	oled_putString(0, 40, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AZ: %d   ", (int) z);
	oled_putString(0, 50, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

int should_read_vals() {
	return led7seg_display_val == 5 || led7seg_display_val == 10 || led7seg_display_val == 15;
}

void pwm() {
	pwm_count++;
	if (pwm_count == 20) {
		pwm_count = 0;
	}

	if (pwm_count == 0 && pwm_val != 0) {
		conditionally_turn_on_blue_and_red_rgbs();
	}

	if (pwm_count == pwm_val) {
		turn_off_blue_and_red_rgbs();
	}
}

void toggle_isr() {
	should_toggle_mode = 1;
}

void do_every_second() {
	is_new_second = 1;
}

void eint3_isr(void) {
	if (did_gpio_interrupt_occur(2, 5)) {
		// light interrupt
		gpio_interrupt_clear(2, 5);
		light_clearIrqStatus();
		is_blue_rgb_blinking = 1;
	}
	if (did_gpio_interrupt_occur(0, 2)) {
		gpio_interrupt_clear(0, 2);
		current_temp_edges++;
	}
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

		if (should_read_vals()) {
			acc_read(&x, &y, &z);
			light_val = light_read();
			pwm_val = (light_val * 20) / LIGHT_RANGE + 1;

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
		temp_val = (get_ms_ticks() - prev_ms_temp) * 3 - 2731;
		if (temp_val > TEMP_HIGH_WARNING) {
			is_red_rgb_blinking = 1;
		}
		current_temp_edges = 0;
		prev_ms_temp = get_ms_ticks();
	}
}
