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
#include "serial.h"

#define DEBUFSIZE 32
#define DEBUFSIZEMASK (DEBUFSIZE-1)

#if DEBUFSIZE & (DEBUFSIZE-1)
#  error DEBUFSIZE must be an exponential of 2
#endif

//Circular RX/TX buffers. Always keep one slot open.

//debug variable: check whether the interrupt handler is actually called

//TX buffer:
//tx.put = index where to put next symbol to transmit
//tx.get = index of the current symbol in transmit
//if (tx.put == current.get)
//  tx.buffer == empty
//if (tx.put != current.get)
//  tx.get == current symbol in transmit
//if (tx.put+1 == tx.get)
//  tx.buffer == full

#define BUFSIZE(BUF) (((BUF.put - BUF.get) >= 0) ? (BUF.put - BUF.get) : (BUF.put + DEBUFSIZE - BUF.get))

struct s_outdebuffer {
  uint8_t put;
  volatile uint8_t get;
  uint8_t buffer[DEBUFSIZE];
};

//For RX: only the put pointer is modified by the interrupt service.
struct s_indebuffer {
  volatile uint8_t put;
  uint8_t get;
  uint8_t buffer[DEBUFSIZE];
};

__idata struct s_outdebuffer outbuffer;
__idata struct s_indebuffer inbuffer;

void serial_init(void)
{
  T3FD = 0x6d;
  T3CON = (T3CON_T3BAUDEN_mask) | (0x5 << T3CON_DIV_shift);

  outbuffer.get = 0;
  outbuffer.put = 0;

  inbuffer.get = 0;
  inbuffer.put = 0;

  SCON = (SCON_SM1_mask) | (SCON_REN_mask); //Enable Reception
  ES = 1; //Enable Serial Interrupt (IE.ES)
}

void serial_interrupt_handler(void) __interrupt INTERRUPT_RI_TI __using 1
{
  if (TI) //SCON.TI
  {
    uint8_t getpos = outbuffer.get;
    uint8_t nextgetpos = (getpos + 1) & DEBUFSIZEMASK;
    if (outbuffer.put != nextgetpos)
    {
      SBUF = outbuffer.buffer[nextgetpos];
      outbuffer.get = nextgetpos;
    }
    else
    {
      outbuffer.get = outbuffer.put;
      //There is no more data in the buffer
    }
    TI = 0; //SCON.TI
  }
  if (RI) //SCON.RI
  {
    uint8_t putpos = inbuffer.put;
    uint8_t nextputpos = (putpos + 1) & DEBUFSIZEMASK;
    if (nextputpos != inbuffer.get)
    {
      inbuffer.buffer[putpos] = SBUF;
      inbuffer.put = nextputpos;
    }
    else
    {
        //Dropped a character
    }
    RI = 0;
  }
}

void serial_putc_blocking(uint8_t c)
{
  uint8_t nextput = (outbuffer.put + 1) & DEBUFSIZEMASK;
  if (outbuffer.put == outbuffer.get)
  {
    outbuffer.buffer[outbuffer.put] = c;
    SBUF = c;
    outbuffer.put = nextput;
    return;
  }
  while (nextput == outbuffer.get);
  outbuffer.buffer[outbuffer.put] = c;
  outbuffer.put = nextput;
}

void serial_flush(void)
{
  while (outbuffer.put != outbuffer.get);
}

void serial_puts_blocking(const char *s)
{
  while (*s)
  {
    serial_putc_blocking(*s);
    ++s;
  }
}

uint8_t serial_getc_nb(void)
{
  if (inbuffer.put == inbuffer.get)
    return 0;
  else
  {
    uint8_t c = inbuffer.buffer[inbuffer.get];
    inbuffer.get = (inbuffer.get + 1) & DEBUFSIZEMASK;
    return c;
  }
}

uint8_t serial_getc_available(void)
{
  return (inbuffer.put != inbuffer.get);
}
