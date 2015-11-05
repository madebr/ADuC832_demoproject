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
#include "lcd.h"

#include "clock.h"

#include "ADUC832.h"

#define LCD_PORT (P0)

//P0: open drain port ==> writing 1 ==> high impedance input
//P0_0 and P0_1: inputs ==> always write 1

#define LCD_DB4 P0_4
#define LCD_DB5 P0_5
#define LCD_DB6 P0_6
#define LCD_DB7 P0_7
#define LCD_RS  P0_3 //0: Instruction; 1: Data
#define LCD_E   P0_2 //Enable
//RW -> GND ==> only writing is possible

#define LCD_DISPLAY_CLEAR_CMD                   0x01

#define LCD_RETURN_HOME_CMD                     0x02

#define LCD_ENTRY_MODE_SET_CMD                  0x04
#define LCD_ENTRY_MODE_SET_DIRECTION_INCR   0x02
#define LCD_ENTRY_MODE_SET_SHIFT            0x01

#define LCD_DISPLAY_ONOFF_CMD                   0x08
#define LCD_DISPLAY_ONOFF_DISPLAY_ON        0x04
#define LCD_DISPLAY_ONOFF_CURSOR_ON         0x02
#define LCD_DISPLAY_ONOFF_BLINKING_ON       0x01

#define LCD_CURSOR_DISPLAY_SHIFT_CMD            0x10
#define LCD_CURSOR_DISPLAY_SHIFT_SHIFT      0x08
#define LCD_CURSOR_DISPLAY_SHIFT_DIRECTION  0x04

#define LCD_FUNCTION_SET_CMD                    0x20
#define LCD_FUNCTION_SET_8BIT               0x10
#define LCD_FUNCTION_SET_2LINES             0x08
#define LCD_FUNCTION_SET_FONT_TYPE_5x11     0x04

#define LCD_SET_CGRAM_ADDRESS_CMD               0x40

//1-line display modus: 0x00-0x4f
//2-line display modus: 0x00-0x27; 0x40-0x67
#define LCD_SET_DDRAM_ADDRESS_CMD               0x80

#define LCD_RS_mask      0x8
#define LCD_E_mask       0x4
#define LCD_ALWAYS_mask  0x3 //Keep lower bits in open drain mode

//LCD: 4-bit, 5x8, 2lines modues
#define LCD_FUNCTION_SET (LCD_FUNCTION_SET_CMD | LCD_FUNCTION_SET_2LINES)

//LCD: display on, cursor on and blinking
#define LCD_DISPLAY_ALL (LCD_DISPLAY_ONOFF_CMD | LCD_DISPLAY_ONOFF_DISPLAY_ON | \
  LCD_DISPLAY_ONOFF_CURSOR_ON | LCD_DISPLAY_ONOFF_BLINKING_ON)

//LCD: clear command
#define LCD_CLEAR (LCD_DISPLAY_CLEAR_CMD)

//LCD: set pos at home
#define LCD_RETURNHOME (LCD_RETURN_HOME_CMD)

//LCD: increment mode
#define LCD_ENTRY_MODE (LCD_ENTRY_MODE_SET_CMD | LCD_ENTRY_MODE_SET_DIRECTION_INCR)

//LCD: change cursorpos
#define LCD_CHANGE_CURSOR(ROW, COL) (LCD_SET_DDRAM_ADDRESS_CMD | (((ROW)?0x40:0x00) | (COL)))

//#define CREATE_RAWDATA_FROM_INSTRUCTION_NIBBLE(NIBBLE) (LCD_ALWAYS_mask | ((NIBBLE) << 4))
//#define CREATE_RAWDATA_FROM_DATA_NIBBLE(NIBBLE) (LCD_ALWAYS_mask | ((NIBBLE) << 4) | LCD_RS_mask)

static void lcd_send_nibble(uint8_t rawdata)
{
  LCD_PORT = rawdata;
  CLOCK_BUSYWAIT_US(4); //RS,RW Set-up time (0.1us)
  LCD_E = 1;
  CLOCK_BUSYWAIT_US(4); // Enable pulse width (0.3us)
  LCD_E = 0;
  //CLOCK_BUSYWAIT_US(0.06); // RS,RW,data hold time (0.01us)

  CLOCK_BUSYWAIT_US(4); // Enable cycle time (min E cycle=0.5us)
}

#define LCD_COMMAND(COMMAND) do { \
    lcd_send_nibble(LCD_ALWAYS_mask | ((COMMAND) & 0xf0)); \
    lcd_send_nibble(LCD_ALWAYS_mask | ((COMMAND) << 4)); \
  } while (0)

#define LCD_DATA(DATA) do { \
    lcd_send_nibble(LCD_ALWAYS_mask | LCD_RS_mask | ((DATA) & 0xf0)); \
    lcd_send_nibble(LCD_ALWAYS_mask | LCD_RS_mask | ((DATA) << 4)); \
  } while (0)

struct s_lcdinfo
{
  uint8_t pos;
};

__idata struct s_lcdinfo lcdinfo;

void lcd_init(void)
{

  CLOCK_BUSYWAIT_BIG_US(30000 + 10); //Wait 30ms after Vdd rises
  lcd_send_nibble(LCD_ALWAYS_mask | (LCD_FUNCTION_SET & 0xf0));
  LCD_COMMAND(LCD_FUNCTION_SET);
  CLOCK_BUSYWAIT_US(39+10);
  LCD_COMMAND(LCD_DISPLAY_ALL);
  CLOCK_BUSYWAIT_US(39+10);
  lcd_clear();
  LCD_COMMAND(LCD_ENTRY_MODE);
  CLOCK_BUSYWAIT_US(39+10);
  lcdinfo.pos = 0;
}

#include "serial.h"

void lcd_puts_b(uint8_t c)
{
  if (c == '\r')
    return;
  if (c == '\n')
    lcdinfo.pos = (lcdinfo.pos + 0x10) & 0xf0;
  else
  {
    LCD_DATA(c);
    CLOCK_BUSYWAIT_US(43);
    lcdinfo.pos++;
  }
  if (lcdinfo.pos == 0x10)
  {
    LCD_COMMAND(LCD_CHANGE_CURSOR(1, 0));
  }
  else if (lcdinfo.pos == 0x20)
  {
    LCD_COMMAND(LCD_CHANGE_CURSOR(0, 0));
    lcdinfo.pos = 0x0;
  }

}

void lcd_clear(void)
{
  LCD_COMMAND(LCD_CLEAR);
  CLOCK_BUSYWAIT_BIG_US(1530+10); //Wait 1.53ms to clear display
}
