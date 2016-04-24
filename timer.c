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
#include "timer.h"

#include "lcd.h"

//Gate for Timer 0: P3.2=~INT0
//Gate for Timer 1: P3.3=~INT1
//

//Important: The timers increment!
//The ISR are triggered when the value is 0,
//so to trigger every X machine cycle,
//load the timer with 0-X.

void timer0_interrupt_handler(void) __interrupt  INTERRUPT_TF0 __using 1
{
  P2_1 ^= 0x1;
}

void timer1_interrupt_handler(void) __interrupt  INTERRUPT_TF1 __using 1
{
  P2_2 ^= 0x1;
}

void timer2_interrupt_handler(void) __interrupt  INTERRUPT_TF2_EXF2 __using 1
{
  P2_3 ^= 0x1;
}

#define TMOD_M_T0_13BIT_MODE_mask       0x00  //mode 0
#define TMOD_M_T0_16BIT_MODE_mask       0x01  //mode 1
#define TMOD_M_T0_8BIT_MODE_RELOAD_mask 0x02  //mode 2
#define TMOD_M_T0_2_8BIT_MODE_mask      0x03  //mode 3
//When timer 0 is in mode 3, timer 0 will generate interrupts for
//both timer 0 and 1. Timer 1 can still be used as a seperate timer
//but will be unable to generate interrupts.

#define TMOD_M_T1_13BIT_MODE_mask       0x00  //mode 0
#define TMOD_M_T1_16BIT_MODE_mask       0x10  //mode 1
#define TMOD_M_T1_8BIT_MODE_RELOAD_mask 0x20  //mode 2
#define TMOD_M_T1_DISABLE_mask          0x30  //mode 3
//Timer 1 in mode 3 is equivalent to TR1 = 0 (disable it).

void timer_init(void)
{
// Timers configuration:
// - Timer 0:
//   * Gate the clock with P3.2 (~INT0)
//   * Use low-to-high edges of P3.4 (T0)
//   * Call the ISR every 5 low-to-high cycles
  TH0 = TL0 = 0-5;

  TH1 = 0xff;
  TL1 = 0-10;
  TMOD = TMOD_CT_T0_mask | TMOD_M_T0_8BIT_MODE_RELOAD_mask | TMOD_GATE_T0_mask |
      TMOD_M_T1_16BIT_MODE_mask | TMOD_GATE_T1_mask;
  // Timer 0 is edge sensitive
  //IT0 = 0
  IT1 = 1;
  //Enable the timer interrupts
  ET0 = 1;
  ET1 = 1;
  //Enable the timers
  TR0 = 1;
  TR1 = 1;
}
