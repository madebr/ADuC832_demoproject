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

