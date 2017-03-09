/*
 * application_logic.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#ifndef APPLICATION_LOGIC_H_
#define APPLICATION_LOGIC_H_

#include "interface_helper.h"
#include "timer_helper.h"
#include "interrupt_helper.h"

#include "led_helper.h"
#include "joystick.h"
#include "acc_helper.h"
#include "light_helper.h"
#include "temp_helper.h"
#include "oled.h"
#include "rgb_helper.h"
#include "led7seg_helper.h"
#include "speaker_helper.h"

void turn_off_blinking_rgb();
void do_every_second();
void toggle_speaker_buzzing();


#endif /* APPLICATION_LOGIC_H_ */
