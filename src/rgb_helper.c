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

void rgb_clear_all() {
	rgb_set(0);
}
