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
#include "stdlib.h"
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

typedef enum {PASSIVE, MONITOR} mode_type;

#define LIGHT_RANGE 973
#define LIGHT_LOW_WARNING 50
#define TEMP_HIGH_WARNING 240
#define ACC_STABLE 50
#define ACC_THRESHOLD 7

void toggle_leds();
void do_every_second();
void toggle_isr();
void loop();
void eint3_isr();
void pwm();
void uart_thre_isr();


#endif /* APPLICATION_LOGIC_H_ */
