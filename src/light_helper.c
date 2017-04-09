/*
 * light_helper.c
 *
 *  Created on: Mar 6, 2017
 *      Author: Varun
 */

#include "light_helper.h"

void lightEnable(uint32_t light_low_threshold) {
	light_enable();
	light_setRange(LIGHT_RANGE_4000);
	light_setLoThreshold(light_low_threshold);
	light_clearIrqStatus();
	light_setIrqInCycles(LIGHT_CYCLE_1);
}
