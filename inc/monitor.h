/*
 * monitor.h
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "application_logic.h"
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

#define LIGHT_RANGE 3892
#define LIGHT_LOW_WARNING 50
#define TEMP_HIGH_WARNING 45
#define ACC_THRESHOLD 5

void enable_monitor_mode();
void toggle_leds();
void pwm();
void do_every_second();
void eint3_isr();
void monitor_loop();

#endif /* MONITOR_H_ */
