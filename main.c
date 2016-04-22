/*
=============================================================================
Copyright (C) 2015, Anonymous Maarten

This file is part of ADuC832 demoproject.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
=============================================================================
*/
#include <stdint.h>

#include "adc.h"
#include "button.h"
#include "clock.h"
#include "interrupts.h"
#include "lcd.h"
#include "pwm.h"
#include "rtc.h"
#include "serial.h"
#include "watchdog.h"
#include "time.h"
#include "timer.h"
#include "util.h"

#include "console.h"

#include "ADUC832.h"

// __code put variable in text section (variable will be const)
// __data/__near: directly addressable portion of internal RAM (will decrease maximum stack) (small memory model)
// __idata: indirectly addressable portion of internal ram
// __pdata: eXternal Data
// __xdata/__far puts variable in eXternal RAM (large memory model)

int main()
{
  clock_init();
  adc_init();
  lcd_init();
  pwm_init();
  rtc_init();
  serial_init();
  timer_init();
  watchdog_init();
  P2 = 0xff;
  interrupts_enable();
  adc_calibrate();
  console_init();
  while (1)
  {
    __idata char *str;
    console_tick();
    str = console_valid();
    if (str)
    {
      s_printf_b("Received \"%s\"\r\n", str);
      s_printf_b("0x%x\r\n", button_readdip());
      s_printf_b("T%d: 0x%x\r\n", 0, convert8_16(TL0, TH0));
      s_printf_b("T%d: 0x%x\r\n", 1, convert8_16(TL1, TH1));
    }
    clock_idle();
    P2_7 ^= 0x1;
  }
}
