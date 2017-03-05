#include "stdint.h"
#include "pca9532.h"

void leds_init(void);
void leds_set(uint16_t ledOnMask, uint16_t ledOffMask);
void leds_only_turn_on(uint16_t ledOnMask);
void leds_only_turn_off(uint16_t ledOffMask);
void leds_also_turn_on(uint16_t ledOnMask);
void leds_also_turn_off(uint16_t ledOffMask);
