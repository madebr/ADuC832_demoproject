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
#ifndef _SERIAL_H_
#define _SERIAL_H_
#include <stdint.h>
#include <stdarg.h>

#include "ADUC832.h"
#include "printf.h"

void serial_init(void);

void serial_interrupt_handler(void) __interrupt INTERRUPT_RI_TI __using 1;

void serial_flush(void);
void serial_putc_blocking(uint8_t c);
void serial_puts_blocking(const char *s);

uint8_t serial_getc_available(void);
uint8_t serial_getc_nb(void);

#define s_printf_b(...) printf_fn(serial_putc_blocking, __VA_ARGS__)

#endif //_SERIAL_H_

