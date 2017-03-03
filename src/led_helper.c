/*
 * led_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "led_helper.h"

void init_leds() {
	pca9532_init();
}

void set_leds(uint16_t ledOnMask, uint16_t ledOffMask) {
	pca9532_setLeds(ledOnMask, ledOffMask);
}

void only_turn_on_leds(uint16_t ledOnMask) {
	set_leds(ledOnMask, 0xffff);
}

void only_turn_off_leds(uint16_t ledOffMask) {
	set_leds(~ledOffMask, 0xffff);
}

void also_turn_on_leds(uint16_t ledOnMask) {
	set_leds(ledOnMask, 0x0000);
}

void also_turn_off_leds(uint16_t ledOffMask) {
	set_leds(0x0000, ledOffMask);
}
