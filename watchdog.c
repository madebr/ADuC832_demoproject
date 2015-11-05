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
#include "watchdog.h"

void watchdog_interrupt_handler(void) __interrupt INTERRUPT_WDS __using 1
{
}

void watchdog_init(void)
{
  //timeout period = (2 ** PRE x 2 ** 9 / f_xtal) (f_xtal=32.768kHz)
  // (0 <= PRE <= 7)
  WDWR = 1;

  //Start the watchdog.
  //The watchdog will reset after 500ms of no reset.
  WDCON = (0x5 << WDCON_PRE_shift) | WDCON_WDE_mask;

  //The watchdog will interrupt after 500ms of no reset.
  //WDCON = (0x0 << WDCON_PRE_shift) | WDCON_WDIR_mask | WDCON_WDE_mask;
}

