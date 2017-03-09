/*
 * application_logic.c
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#include "application_logic.h"

uint8_t led7seg_display_val = 0;
uint8_t is_speaker_buzzing = 0;

void turn_off_blinking_rgb() {
	rgb_also_clear_red();
}

void do_every_second() {
	led7seg_display_val = led7seg_display_val == 15 ? 0 : led7seg_display_val + 1;
	led7seg_set_number(led7seg_display_val);

	rgb_also_set_red();
	timer_interrupt_enable(TIMER0);
}

void toggle_speaker_buzzing() {
	if (is_speaker_buzzing) {
		timer_interrupt_disable(TIMER2);
		speaker_off();
	} else {
		timer_interrupt_enable(TIMER2);
	}

	is_speaker_buzzing = !is_speaker_buzzing;
}


