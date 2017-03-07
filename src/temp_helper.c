/*
 * temp_helper.c
 *
 *  Created on: Mar 6, 2017
 *      Author: Varun
 */

#include "temp_helper.h"

uint32_t get_ms_ticks() {
	return (uint32_t) ms_ticks;
}

uint32_t (*get_temp_function())() {
	return &get_ms_ticks;
}

