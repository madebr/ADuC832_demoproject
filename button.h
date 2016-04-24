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
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>

//The buttons are active low
//(voltage drops when button is pressed)
#define NOT_BUTTON1 P3_7
#define NOT_BUTTON2 P3_6
#define NOT_BUTTON3 P3_5
#define NOT_BUTTON4 P3_4

#define _BUTTON_WAIT(BTN) \
  do { \
    while(BTN); \
    while(!(BTN)); \
  } while (0)

#define button_wait1() _BUTTON_WAIT(NOT_BUTTON1)
#define button_wait2() _BUTTON_WAIT(NOT_BUTTON2)
#define button_wait3() _BUTTON_WAIT(NOT_BUTTON3)
#define button_wait4() _BUTTON_WAIT(NOT_BUTTON4)

void button_init(void);
uint8_t button_readdip(void);

#endif //_BUTTON_H_
