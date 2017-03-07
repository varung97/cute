#include "acc.h"
#include "stdint.h"

void acc_init(void);
void acc_read(int8_t *x, int8_t *y, int8_t *z);
void acc_setRange(acc_range_t range);
void acc_setMode(acc_mode_t mode);
