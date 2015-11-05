#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include "ADUC832.h"

static void inline interrupts_enable(void)
{
  EA = 1;
}

static void inline interrupts_disable(void)
{
  EA = 0;
}

#endif
