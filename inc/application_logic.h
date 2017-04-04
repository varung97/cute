/*
 * application_logic.h
 *
 *  Created on: Mar 9, 2017
 *      Author: Varun
 */

#ifndef APPLICATION_LOGIC_H_
#define APPLICATION_LOGIC_H_

#include "stdio.h"
#include "stdint.h"
#include "monitor.h"
#include "message.h"
#include "passive.h"
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

typedef enum {PASSIVE, MONITOR, MESSAGE} mode_type;

void reset_board();
void toggle_isr();
void uart_thre_isr();
void loop();


#endif /* APPLICATION_LOGIC_H_ */
