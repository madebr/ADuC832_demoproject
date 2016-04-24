/*
=============================================================================
Copyright (C) 2015-2016, Anonymous Maarten

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
#include "rtc.h"

#include "clock.h"

void rtc_interrupt_handler(void) __interrupt INTERRUPT_TII __using 1
{
  P2_0 ^= 1;
  TIMECON &= ~TIMECON_TII_mask;
}

void rtc_init(void)
{
  INTVAL = 2; //Interrupt every XX seconds
  HOUR = 0;
  MIN = 0;
  SEC = 0;
  HTHSEC = 0;
  CLOCK_BUSYWAIT_US(50);
  TIMECON = (TIMECON_TIEN_mask | TIMECON_ITS_SECONDS_mask | TIMECON_TCEN_mask);
  IEIP2 |= IEIP2_ETI_mask;
}
