/*
 * speaker_helper.c
 *
 *  Created on: Mar 3, 2017
 *      Author: Varun
 */

#include "speaker_helper.h"

void init_speaker() {
	set_pin_dir(2, 0, 1);
	set_pin_dir(2, 1, 1);

	set_pin_dir(0, 27, 1);
	set_pin_dir(0, 28, 1);
	set_pin_dir(2, 13, 1);

	// Main tone signal : P0.26
	set_pin_dir(0, 26, 1);

	clear_pin_val(0, 27); //LM4811-clk
	clear_pin_val(0, 28); //LM4811-up/dn
	clear_pin_val(2, 13); //LM4811-shutdn
}
