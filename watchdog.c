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
#include "watchdog.h"

#define WDCON_PRE_TIMEOUT_0015_6_MS_MASKVALUE 0x00
#define WDCON_PRE_TIMEOUT_0031_2_MS_MASKVALUE 0x10
#define WDCON_PRE_TIMEOUT_0062_5_MS_MASKVALUE 0x20
#define WDCON_PRE_TIMEOUT_0125_0_MS_MASKVALUE 0x30
#define WDCON_PRE_TIMEOUT_0250_0_MS_MASKVALUE 0x40
#define WDCON_PRE_TIMEOUT_0500_0_MS_MASKVALUE 0x50
#define WDCON_PRE_TIMEOUT_1000_0_MS_MASKVALUE 0x60
#define WDCON_PRE_TIMEOUT_2000_0_MS_MASKVALUE 0x70
#define WDCON_PRE_TIMEOUT_0000_0_MS_MASKVALUE 0x80

void watchdog_interrupt_handler(void) __interrupt INTERRUPT_WDS __using 1
{
  P2_6 ^= 0x1;
}

void watchdog_init(void)
{
  //timeout period = (2 ** PRE x 2 ** 9 / f_xtal) (f_xtal=32.768kHz)
  // (0 <= PRE <= 7)
  WDWR = 1;

  //Start the watchdog.
  //The watchdog will reset after 500ms of no reset.
  WDCON = WDCON_PRE_TIMEOUT_0500_0_MS_MASKVALUE;

  //The watchdog will interrupt after 500ms of no reset.
  //WDCON = WDCON_PRE_TIMEOUT_0500_0_MS_MASKVALUE | WDCON_WDIR_mask | WDCON_WDE_mask;
}

