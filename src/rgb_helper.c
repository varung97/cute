/*
 * rgb_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "rgb_helper.h"

void rgb_set(uint8_t ledMask) {
	rgb_setLeds(ledMask);
}

void rgb_only_set_blue() {
	rgb_setLeds(RGB_BLUE);
}

void rgb_only_set_red() {
	rgb_setLeds(RGB_RED);
}

void rgb_only_set_green() {
	rgb_setLeds(RGB_GREEN);
}

void rgb_also_set(uint8_t turn_on) {
	uint8_t current = rgb_read();
	rgb_setLeds(current | turn_on);
}

void rgb_also_set_blue() {
	rgb_also_set(RGB_BLUE);
}

void rgb_also_set_red() {
	rgb_also_set(RGB_RED);
}

void rgb_also_set_green() {
	rgb_also_set(RGB_GREEN);
}

void rgb_also_clear(uint8_t turn_on) {
	uint8_t current = rgb_read();
	rgb_setLeds(current & ~turn_on);
}

void rgb_also_clear_blue() {
	rgb_also_clear(RGB_BLUE);
}

void rgb_also_clear_red() {
	rgb_also_clear(RGB_RED);
}

void rgb_also_clear_green() {
	rgb_also_clear(RGB_GREEN);
}

void rgb_clear_all() {
	rgb_set(0);
}

uint8_t rgb_read() {
	return  pin_read_val(RGB_BLUE_PORT, RGB_BLUE_PIN) * RGB_BLUE |
			pin_read_val(RGB_RED_PORT, RGB_RED_PIN) * RGB_RED |
			pin_read_val(RGB_GREEN_PORT, RGB_GREEN_PIN) * RGB_GREEN;
}
