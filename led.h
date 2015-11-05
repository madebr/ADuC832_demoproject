#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>

#include "ADUC832.h"

static void inline led_init(void)
{
  P2 = 0xff;
}

static void inline led_write(uint8_t data)
{
  P2 = ~data;
}

static void inline led_write_raw(uint8_t data)
{
  P2 = data;
}

#endif

