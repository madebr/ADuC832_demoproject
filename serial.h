#ifndef _SERIAL_H_
#define _SERIAL_H_
#include <stdint.h>

#include "ADUC832.h"

void serial_init(void);

void serial_interrupt_handler(void) __interrupt INTERRUPT_RI_TI __using 1;

uint8_t serial_getc(void);
void serial_putc(uint8_t c);
uint8_t serial_puts(char *s);

#endif //_SERIAL_H_

