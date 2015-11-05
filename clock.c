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
#include "clock.h"
#include "ADUC832.h"

//Chip intended for use with 32.768kHz watch chrystal
//PLL locks on multiple (512) = 16.78MHz

void clock_init(void)
{
  //Run at 16.78MHz
  PLLCON = 0;
  CLOCK_BUSYWAIT_US(50);
}

#define ASM_BUSYWAIT

void clock_busywait_big(uint16_t cycli)
{
#ifdef ASM_BUSYWAIT
  //First argument goes into dpl, dph, ...
  //next arguments go into _FUNCNAME_PARM_X
  cycli;
  __asm;
    mov r7, dph                         ;24 cycles
    mov r6, dpl                         ;24 cycles
    inc r7                              ;12 cycles
    inc r6                              ;12 cycles
  inner_msb_loop_big:
    djnz r6, inner_msb_loop_big         ;24 cycles
    djnz r7, inner_msb_loop_big         ;24 cycles
  __endasm;
#else
  while (cycli--);
#endif
}

void clock_busywait(uint8_t cycli)
{
#ifdef ASM_BUSYWAIT
  //Jump to this function: 24 cycles
  cycli;
  __asm;
    mov r7, dpl     ;24 cycles
    inc r7          ;12 cycles
  loop:
    djnz r7, loop   ;24 cycles
  __endasm;
#else
  while (cycli--);
#endif
}

