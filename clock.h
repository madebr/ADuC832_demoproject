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
#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stdint.h>
#include "assert.h"

void clock_init(void);

#define F_CORE (512 * 32768ULL)

#define CLOCK_CYCLI(USEC)  (((((USEC)*F_CORE) / 24)/1000000))

//Fixed cost of jump to function and setup + tear down is ignored
//These the calculated cycli are approximations (upper bounded)
#define CLOCK_BUSYWAIT_US(US) do {ct_assert(CLOCK_CYCLI(US) < 0x100); ct_assert(CLOCK_CYCLI(US) >= 0);clock_busywait(CLOCK_CYCLI(US));} while(0)

#define CLOCK_BUSYWAIT_BIG_US(US) do {ct_assert(CLOCK_CYCLI(US) < 0x10000); ct_assert(CLOCK_CYCLI(US) >= 0);clock_busywait_big(CLOCK_CYCLI(US));} while(0)

void clock_busywait_big(uint16_t cycli);
void clock_busywait(uint8_t cycli);

#endif

