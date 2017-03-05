/*
 * led7seg_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "led7seg_helper.h"

void led7seg_set_char(uint8_t ch) {
	led7seg_setChar(ch, 0);
}

void led7seg_set_raw(uint8_t raw_val) {
	led7seg_setChar(raw_val, 1);
}

void led7seg_set_number(uint8_t num) {
	if (num < 10) {
		led7seg_set_char('0' + num);
	} else {
		led7seg_set_char('A' + num - 10);
	}
}
