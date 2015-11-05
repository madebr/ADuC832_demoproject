#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>
#include "assert.h"

void clock_init(void);

#define CLOCK_CYCLI(USEC)  (((((USEC)*16777216ULL) / 24)/1000000))

//Fixed cost of jump to function and setup + tear down is ignored
//These the calculated cycli are approximations (upper bounded)
#define CLOCK_BUSYWAIT_US(US) do {ct_assert(CLOCK_CYCLI(US) < 0x100); ct_assert(CLOCK_CYCLI(US) >= 0);clock_busywait(CLOCK_CYCLI(US));} while(0)
#define CLOCK_BUSYWAIT_BIG_US(US) do {ct_assert(CLOCK_CYCLI(US) < 0x10000); ct_assert(CLOCK_CYCLI(US) >= 0);clock_busywait_big(CLOCK_CYCLI(US));} while(0)

void clock_busywait_big(uint16_t cycli);
void clock_busywait(uint8_t cycli);

#endif

