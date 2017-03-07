#include "stdint.h"
#include "temp.h"
#include "timer_helper.h"

void temp_init (uint32_t (*get_ms_tick)(void));
int32_t temp_read(void);
uint32_t (*get_temp_function())();
