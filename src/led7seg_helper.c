/*
 * led7seg_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "led7seg.h"
#include "stdint.h"

void init_led7seg(void) {
	led7seg_init();
}

void set_char_led7seg(uint8_t ch) {
	led7seg_setChar(ch, 0);
}

void set_raw_led7seg(uint8_t raw_val) {
	led7seg_setChar(raw_val, 1);
}

void set_number_led7seg(uint8_t num) {
	if (num < 10) {
		set_char_led7seg('0' + num);
	} else {
		set_char_led7seg('A' + num - 10);
	}
}
