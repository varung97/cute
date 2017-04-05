/*
 * message.h
 *
 *  Created on: Apr 4, 2017
 *      Author: Varun
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "stdio.h"
#include "stdint.h"
#include "string.h"
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

typedef enum {VIEW, WRITE} message_mode_type;

void enable_message_mode();
void uart_rxav_isr();
void read_joystick_isr();
void message_loop();

#endif /* MESSAGE_H_ */
