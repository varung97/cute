/*
 * light_helper.c
 *
 *  Created on: Mar 6, 2017
 *      Author: Varun
 */

#include "light_helper.h"

void lightInit(uint32_t light_low_threshold) {
	light_init();
	//light_setMode(LIGHT_MODE_D1);
	light_setRange(LIGHT_RANGE_1000);
	//light_setWidth(LIGHT_WIDTH_16BITS);
//	light_setLoThreshold(light_low_threshold);
//	light_clearIrqStatus();
//	light_setIrqInCycles(LIGHT_CYCLE_1);
}
