#include "stdint.h"
#include "interface_helper.h"
#include "rgb.h"

#define RGB_BLUE_PORT 0
#define RGB_RED_PORT 2
#define RGB_GREEN_PORT 2
#define RGB_BLUE_PIN 26
#define RGB_RED_PIN 0
#define RGB_GREEN_PIN 1

void rgb_init();
void rgb_set(uint8_t ledMask);
void rgb_only_set_blue();
void rgb_only_set_red();
void rgb_only_set_green();
void rgb_also_set(uint8_t turn_on);
void rgb_also_set_blue();
void rgb_also_set_red();
void rgb_also_set_green();
void rgb_also_clear(uint8_t turn_on);
void rgb_also_clear_blue();
void rgb_also_clear_red();
void rgb_also_clear_green();
void rgb_clear_all();
uint8_t rgb_read();
