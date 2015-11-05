#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "ADUC832.h"

void watchdog_init(void);

void watchdog_interrupt_handler(void) __interrupt INTERRUPT_WDS __using 1;

static void inline watchdog_tick(void)
{
  WDWR = 1;
  WDE = 0;
}

#endif

