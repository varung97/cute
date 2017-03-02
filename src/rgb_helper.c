/*
 * rgb_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "stdint.h"
#include "rgb.h"

void init_rgb() {
	rgb_init();
}

void set_rgb(uint8_t ledMask) {
	rgb_setLeds(ledMask);
}

void only_set_blue_rgb() {
	rgb_setLeds(RGB_BLUE);
}

void only_set_red_rgb() {
	rgb_setLeds(RGB_RED);
}

void only_set_green_rgb() {
	rgb_setLeds(RGB_GREEN);
}
