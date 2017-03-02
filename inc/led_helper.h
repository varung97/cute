#include "stdint.h"

void init_leds(void);
void set_leds(uint16_t ledOnMask, uint16_t ledOffMask);
void only_turn_on_leds(uint16_t ledOnMask);
void only_turn_off_leds(uint16_t ledOffMask);
void also_turn_on_leds(uint16_t ledOnMask);
void also_turn_off_leds(uint16_t ledOffMask);
