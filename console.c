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
#include "console.h"

#include "serial.h"

#define con_getc_av serial_getc_available
#define con_getc_nb serial_getc_nb
#define con_putc_b  serial_putc_blocking
#define con_puts_b  serial_puts_blocking

#define CON_PREFIX "> "

#define CONSTATBUFBITS 5
#define CONSTATBUFLEN (1 << 5)

enum console_control_t
{
  DATA = 0,
  CONTROL1 = 1,
  CONTROL2 = 2,
  POSTCONTROL = 3,
  DATAVALID = 4
};

struct s_console_status
{
  char buf[CONSTATBUFLEN + 1]; //1 extra for the terminating null character
  uint8_t len;
  uint8_t pos;
  enum console_control_t control : 8;
};

__idata struct s_console_status constat;

void console_init(void)
{
  constat.len = 0;
  constat.pos = 0;
  constat.control = DATAVALID;
}

#define ASCII_NUL   0x00
#define ASCII_BS    0x08 // '\b' character (move cursor to the left)
#define ASCII_LF    0x0a // '\n' character
#define ASCII_CR    0x0d // '\r' character
#define ASCII_ESC   0x1b
#define ASCII_SPACE 0x20 // ' ' character
#define ASCII_A     0x41 // 'A' character
#define ASCII_B     0x42 // 'B' character
#define ASCII_C     0x43 // 'C' character
#define ASCII_D     0x44 // 'D' character
#define ASCII_LB    0x5b // '[' character
#define ASCII_TIL   0x72 // '~' character
#define ASCII_DEL   0x7f // Backspace button on keyboard

uint8_t console_tick(void)
{
  if (con_getc_av())
  {
    uint8_t c = con_getc_nb();

    if (constat.control == DATAVALID)
    {
      con_puts_b(CON_PREFIX);
      constat.control = DATA;
    }

    // Check for control characters
    switch (c)
    {
    case ASCII_ESC:
      constat.control = CONTROL1;
      break;
    case ASCII_LB:
      if (constat.control == CONTROL1)
      {
        constat.control = CONTROL2;
      }
      break;
    case ASCII_DEL:
      if (constat.pos)
      {
        uint8_t it, itnext;
        constat.pos -= 1;
        it = constat.pos;
        itnext = constat.pos + 1;
        while (itnext < constat.len)
        {
          constat.buf[it] = constat.buf[itnext];
          it = itnext;
          ++itnext;
        }
        constat.len -= 1;
        con_putc_b(ASCII_BS);
        if (constat.pos == constat.len)
        {
          con_putc_b(ASCII_SPACE);
          con_putc_b(ASCII_BS);
        } else
        {
          it = constat.pos;
          while (it < constat.len)
          {
            con_putc_b(constat.buf[it]);
            ++it;
          }
          con_putc_b(ASCII_SPACE);
          con_putc_b(ASCII_BS);
          it = constat.len;
          while (it > constat.pos)
          {
            con_putc_b(ASCII_BS);
            --it;
          }
        }
      }
      constat.control = POSTCONTROL;
      break;
    case ASCII_A: //Button UP
    case ASCII_B: //Button DOWN
      if (constat.control == CONTROL2)
      {
        constat.control = POSTCONTROL;
      }
      //History not implemented
      break;
    case ASCII_C: //Button RIGHT
      if (constat.control == CONTROL2)
      {
        constat.control = POSTCONTROL;
        if (constat.pos != constat.len)
        {
          con_putc_b(constat.buf[constat.pos]);
          constat.pos += 1;
        }
      }
      break;
    case ASCII_D: //Button LEFT
      if (constat.control == CONTROL2)
      {
        constat.control = POSTCONTROL;
        if (constat.pos != 0)
        {
          con_putc_b(ASCII_BS);
          constat.pos -= 1;
        }
      }
      break;
    case ASCII_TIL:
      if (constat.control != DATA)
      {
        constat.control = POSTCONTROL;
      }
      break;
    case ASCII_LF:
    case ASCII_CR:
      constat.buf[constat.len] = ASCII_NUL;
      constat.pos = 0;
      constat.len = 0;
      con_putc_b('\r');
      con_putc_b('\n');
      constat.control = DATAVALID;
      break;
    default:
      break;
    }

    //Only put data in buffer if no control byte has been seen (control == DATA)
    if (constat.control == DATA)
    {
      if (constat.len != CONSTATBUFLEN)
      {
        uint8_t it = constat.len;
        uint8_t itnext = it - 1;
        while (it != constat.pos)
        {
          constat.buf[it] = constat.buf[itnext];
          it = itnext;
          --itnext;
        }
        // it == constat.pos
        constat.buf[it] = c;
        constat.len += 1;
        constat.pos += 1;
        while (it != constat.len)
        {
          con_putc_b(constat.buf[it]);
          ++it;
        }
        // it == constat.len
        while (it != constat.pos)
        {
          con_putc_b(ASCII_BS);
          --it;
        }
        //Add data
      }
    }

    //Put mode back to data if a end-of-sequence byte has been detected.
    if (constat.control == POSTCONTROL)
    {
      constat.control = DATA;
    }
    return 1;
  }
  else if (constat.control == DATAVALID)
  {
    con_puts_b(CON_PREFIX);
    constat.control = DATA;
  }
  return 0;
}

__idata char *console_valid(void)
{
  if (constat.control == DATAVALID)
  {
    return constat.buf;
  }
  else
    return 0;
}
