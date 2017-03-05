#include "stdint.h"
#include "led7seg.h"

void led7seg_init(void);
void led7seg_set_char(uint8_t ch);
void led7seg_set_raw(uint8_t raw_val);
void led7seg_set_number(uint8_t num);
