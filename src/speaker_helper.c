/*
 * speaker_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "speaker_helper.h"

volatile uint8_t is_speaker_on;

void speaker_init() {
	pin_set_dir(0, 27, 1);
	pin_set_dir(0, 28, 1);
	pin_set_dir(2, 13, 1);

	// Main tone signal : P0.26
	pin_set_dir(0, 26, 1);

	pin_clear_val(0, 27); //LM4811-clk
	pin_clear_val(0, 28); //LM4811-up/dn
	pin_clear_val(2, 13); //LM4811-shutdn

	is_speaker_on = 0;
}

void speaker_on() {
	pin_set_val(0, 26);
}

void speaker_off() {
	pin_clear_val(0, 26);
}
