/*
 * application_logic.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "application_logic.h"

mode_type current_mode = PASSIVE;

int8_t x, y, z;
uint8_t led7seg_display_val;
int num_transmissions = 0;
int did_motion_occur = 0;
volatile int led = 0;

int32_t temp_val;
uint32_t light_val;
char str_val[12];
char uart_str[120];

volatile int is_new_second = 0,
		should_toggle_mode = 0,
		is_rgb_leds_on = 0,
		is_blue_rgb_blinking = 0,
		is_red_rgb_blinking = 0,
		pwm_count = 0,
		pwm_val = 10,
		uart_should_queue = 0;

volatile uint32_t temp_curr_time = 0;
volatile int should_update_temp = 0, start = 0, end = 0;
volatile uint32_t temp_times[334] = {0};

void temp_reset_times() {
	start = end = 333;
	should_update_temp = temp_curr_time = 0;
}

void temp_update_times() {
	end = end == 333 ? 0 : end + 1;
	temp_times[end] = temp_curr_time;
	if (end == start) {
		start = start == 333 ? 0 : start + 1;
		temp_val = (temp_times[end] - temp_times[start]) * 3 - 2731;
	}
}


/**********************************************************
 * Helper functions
 ***********************************************************/

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

	led7seg_display_val = is_blue_rgb_blinking = is_red_rgb_blinking = uart_should_queue = num_transmissions = 0;
	pwm_val = 10;

	led7seg_set_number(led7seg_display_val);
	acc_setMode(ACC_MODE_MEASURE);
	light_enable();
	uart_enable();
	oled_putString(0, 0, (uint8_t *) "MONITOR", OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	timer_interrupt_enable(TIMER0);
	timer_interrupt_enable(TIMER1);

	light_clearIrqStatus();
	gpio_interrupt_clear(2, 5);
	gpio_interrupt_clear(0, 2);

	eint_interrupt_handler_enable(EINT3);
	temp_reset_times();
}

void enable_passive_mode() {
	current_mode = PASSIVE;

	timer_interrupt_disable(TIMER0);
	timer_interrupt_disable(TIMER1);
	timer_interrupt_disable(TIMER2);

	eint_interrupt_handler_disable(EINT3);

	led7seg_set_raw(0xFF);
	acc_setMode(ACC_MODE_STANDBY);
	light_shutdown();
	uart_disable();
	oled_clearScreen(OLED_COLOR_BLACK);

	turn_off_blue_and_red_rgbs();
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

void display_values() {
	int8_t x1 = x;
	int8_t y1 = y;
	int8_t z1 = z;

	sprintf(str_val, "Temp: %.1f   ", temp_val/10.0);
	oled_putString(0, 10, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "Light: %d   ", (int) light_val);
	oled_putString(0, 20, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AX: %d   ", (int) x1);
	oled_putString(0, 30, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AY: %d   ", (int) y1);
	oled_putString(0, 40, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);

	sprintf(str_val, "AZ: %d   ", (int) z1);
	oled_putString(0, 50, (uint8_t *) str_val, OLED_COLOR_WHITE, OLED_COLOR_BLACK);
}

void uart_transmit_vals() {
	num_transmissions++;

	sprintf(uart_str,
			"%s%s%03d_-_T%.1f_L%04d_AX%03d_AY%03d_AZ%03d\r\n",
			is_red_rgb_blinking  ? "Fire was Detected.\r\n" : "",
					is_blue_rgb_blinking ? "Movement in darkness was Detected.\r\n" : "",
							num_transmissions,
							temp_val / 10.0,
							(int) light_val,
							(int) x,
							(int) y,
							(int) z
	);

	uart_send_notblocking(uart_str);
}

int should_read_vals() {
	return led7seg_display_val == 5 || led7seg_display_val == 10 || led7seg_display_val == 15;
}


/**********************************************************
 * ISRs
 **********************************************************/

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
		if(did_motion_occur) {
			is_blue_rgb_blinking = 1;
		}
	}
	if (did_gpio_interrupt_occur(0, 2)) {
		gpio_interrupt_clear(0, 2);
		temp_curr_time = get_ms_ticks();
		should_update_temp = 1;
	}
}

void uart_thre_isr() {
	uart_should_queue = 1;
}

/**********************************************************
 * Loop
 **********************************************************/

void loop() {
	if (should_toggle_mode) {
		should_toggle_mode = 0;

		toggle_mode();
	}

	if (is_new_second) {
		is_new_second = 0;

		led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
		led7seg_set_number(led7seg_display_val);

		acc_read(&x, &y, &z);

		did_motion_occur = (abs(x) + abs(y) + abs(z) > ACC_STABLE + ACC_THRESHOLD ||
		   abs(x) + abs(y) + abs(z) < ACC_STABLE - ACC_THRESHOLD);

		if (should_read_vals()) {

			light_val = light_read();
			pwm_val = (light_val * 20) / LIGHT_RANGE + 1;

			eint_interrupt_handler_disable(EINT3);

			if (led7seg_display_val == 15) {
				uart_transmit_vals();
			}

			display_values();

			eint_interrupt_handler_enable(EINT3);
			temp_reset_times();
		}


	}

	if (should_update_temp) {
		temp_update_times();
		if (temp_val > TEMP_HIGH_WARNING) {
			is_red_rgb_blinking = 1;
		}
		should_update_temp = 0;
	}

	if (uart_should_queue) {
		uart_queue_into_fifo();
		uart_should_queue = 0;
	}
}
