/*
 * led7seg_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "led7seg_helper.h"

static uint8_t inverted_chars[] = {
		/* digits 0 - 9 */
		0x24, 0x7D, 0xE0, 0x70, 0x39, 0x32, 0x22, 0x7C, 0x20, 0x38,
		/* A to F */
		0x28, 0x23, 0xA6, 0x61, 0xA2, 0xAA,
};

void led7seg_set_char(uint8_t ch) {
	led7seg_setChar(ch, 0);
}

void led7seg_set_raw(uint8_t raw_val) {
	led7seg_setChar(raw_val, 1);
}

void led7seg_set_number(uint8_t num) {
	led7seg_set_raw(inverted_chars[num]);
}
