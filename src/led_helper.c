/*
 * led_helper.c
 *
 *  Created on: Mar 2, 2017
 *      Author: Varun
 */

#include "led_helper.h"

void leds_init() {
	pca9532_init();
}

void leds_set(uint16_t ledOnMask, uint16_t ledOffMask) {
	pca9532_setLeds(ledOnMask, ledOffMask);
}

void leds_only_turn_on(uint16_t ledOnMask) {
	leds_set(ledOnMask, 0xffff);
}

void leds_only_turn_off(uint16_t ledOffMask) {
	leds_set(~ledOffMask, 0xffff);
}

void leds_also_turn_on(uint16_t ledOnMask) {
	leds_set(ledOnMask, 0x0000);
}

void leds_also_turn_off(uint16_t ledOffMask) {
	leds_set(0x0000, ledOffMask);
}
