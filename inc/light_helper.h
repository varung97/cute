#include "stdint.h"
#include "light.h"

void lightInit(uint32_t light_low_threshold);
void light_enable(void);
uint32_t light_read(void);
void light_setMode(light_mode_t mode);
void light_setWidth(light_width_t width);
void light_setRange(light_range_t newRange);
void light_setHiThreshold(uint32_t luxTh);
void light_setLoThreshold(uint32_t luxTh);
void light_setIrqInCycles(light_cycle_t cycles);
uint8_t light_getIrqStatus(void);
void light_clearIrqStatus(void);
void light_shutdown(void);
