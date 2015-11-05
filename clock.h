#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>

void clock_init(void);

#define CLOCK_CYCLI(US) ((US)*16.78 / 24)

//Fixed cost of jump to function and setup + tear down is ignored
//These the calculated cycli are approximations (upper bounded)
#define CLOCK_BUSYWAIT_US(US) clock_busywait(CLOCK_CYCLI(US))
#define CLOCK_BUSYWAIT_BIG_US(US) clock_busywait_big(((int)CLOCK_CYCLI(US)) >> 8, ((int)CLOCK_CYCLI(US)) & 0xff)

void clock_busywait_big(uint8_t cycli_msb, uint8_t cycli_lsb);
void clock_busywait(uint8_t cycli);

#endif

