/*****************************************************************************
 *   CUTE - Assignment 2, EE2024
 *
 *   Varun Gupta      A0147924X
 *   Shailesh Nahata  A0147619W
 *
 ******************************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "LPC17xx.h"
#include "setup.h"

int main(void) {
	setup();

    while(1) {
    	loop();
    }
}

void check_failed(uint8_t *file, uint32_t line) {
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}

