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
#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "ADUC832.h"

void watchdog_init(void);

void watchdog_interrupt_handler(void) __interrupt INTERRUPT_WDS __using 1;

static void inline watchdog_enable(void)
{
  WDWR = 1;
  WDE = 1;
}

static void inline watchdog_disable(void)
{
  WDWR = 1;
  WDE = 0;
}

#define watchdog_tick watchdog_enable

#endif

