#include "stdint.h"
#include "temp.h"

void temp_init (uint32_t (*get_ms_tick)(void));
int32_t temp_read(void);
