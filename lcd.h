#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>

#include "printf.h"

void lcd_init(void);
void lcd_clear(void);

void lcd_puts_b(uint8_t c);

#define l_printf_b(...) printf_fn(lcd_puts_b, __VA_ARGS__)
#endif

