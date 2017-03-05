#include "stdint.h"
#include "rgb.h"

void rgb_init();
void rgb_set(uint8_t ledMask);
void rgb_only_set_blue();
void rgb_only_set_red();
void rgb_only_set_green();
void rgb_clear_all();
