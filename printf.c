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
#include "printf.h"

#include <stdarg.h>

static inline uint8_t howmuch(uint16_t den, uint16_t div, uint16_t *rem)
{
  uint8_t cnt = 0;
  uint16_t den_b = den - div;
  while (den_b <= den)
  {
    ++cnt;
    den = den_b;
    den_b = den - div;
  }
  *rem = den;
  return cnt;
}

void printf_fn(putc_t fn, const char *fmt, ...) __reentrant
{
  va_list va;
  uint8_t c;
  va_start(va, fmt);
  while (c = *fmt)
  {
    int16_t u16;
    uint16_t utmp = 0;
    uint16_t utmp2 = 0;
    ++fmt;
    switch(c)
    {
    case '%':
      c = *fmt;
      ++fmt;
      switch(c)
      {
      case 0:
        break;
      case '%':
        fn('%');
        break;
      case 'c':
        utmp = va_arg(va, uint16_t);
        fn(utmp);
        break;
      case 'd':
      case 'i':
        u16 = va_arg(va, uint16_t);
        utmp = 1;
        if (u16 & 0x8000)
        {
          fn('-');
          u16 = -u16;
        }
      case 'u':
        if (!utmp)
          u16 = va_arg(va, uint16_t);
        utmp = howmuch(u16, 10000, &u16);
        if (utmp)
        {
          fn('0' + utmp);
          utmp2 = 1;
        }
        utmp = howmuch(u16, 1000, &u16);
        if (utmp2 || utmp)
        {
          fn('0' + utmp);
          utmp2 = 1;
        }
        utmp = howmuch(u16, 100, &u16);
        if (utmp2 || utmp)
        {
          fn('0' + utmp);
          utmp2 = 1;
        }
        utmp = howmuch(u16, 10, &u16);
        if (utmp2 || utmp)
        {
          fn('0' + utmp);
        }
        fn('0' + u16);
        break;
      case 'x':
      {
        uint16_t a = va_arg(va, uint16_t);
        utmp = a >> 12;
        if (utmp < 0xa)
          fn('0' + utmp);
        else
          fn('A' + utmp - 10);
        utmp = (a >> 8) & 0xf;
        if (utmp < 0xa)
          fn('0' + utmp);
        else
          fn('A' + utmp - 10);
        utmp = (a >> 4) & 0xf;
        if (utmp < 0xa)
          fn('0' + utmp);
        else
          fn('A' + utmp - 10);
        utmp = a & 0xf;
        if (utmp < 0xa)
          fn('0' + utmp);
        else
          fn('A' + utmp - 10);
        break;
      }
      default:
        break;
      }
      break;
    default:
      fn(c);
      break;
    }
  }
  va_end(va);
}
