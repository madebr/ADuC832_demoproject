#include "lcd.h"

#include "clock.h"

#include "ADUC832.h"

#define CMD0_DISPLAY_CLEAR                   0x01
#define CMD0_RETURN_HOME                     0x02
#define CMD0_ENTRY_MODE_SET                  0x04
#define ENTRY_MODE_SET_DIRECTION_INCR   0x02
#define ENTRY_MODE_SET_SHIFT            0x01
#define CMD0_DISPLAY_ONOFF                   0x08
#define DISPLAY_ONOFF_DISPLAY_ON        0x04
#define DISPLAY_ONOFF_CURSOR_ON         0x02
#define DISPLAY_ONOFF_BLINKING_ON       0x01
#define CMD0_CURSOR_DISPLAY_SHIFT            0x10
#define CURSOR_DISPLAY_SHIFT_SHIFT      0x08
#define CURSOR_DISPLAY_SHIFT_DIRECTION  0x04
#define CMD0_FUNCTION_SET                    0x20
#define FUNCTION_SET_8BIT               0x10
#define FUNCTION_SET_2LINES             0x08
#define FUNCTION_SET_FONT_TYPE_5x11     0x04
#define CMD0_SET_CGRAM_ADDRESS               0x40
#define SET_CGRAM_ADDRESS_MASK          0x3f
#define CMD0_SET_DDRAM_ADDRESS               0x80
#define SET_DDRAM_ADDRESS_MASK          0x7f

#define LCD_RS    0x8
#define LCD_E     0x4
#define LCD_KEEP  0x3
#define LCD_PORT (P0)

void lcd_command(uint8_t cmd)
{
  const uint8_t keep = LCD_PORT & LCD_KEEP;
  const uint8_t ms_nibble = cmd & 0xf0;
  const uint8_t ls_nibble = cmd << 4;

  const uint8_t cmd1 = keep | ms_nibble;
  const uint8_t cmd2 = keep | ls_nibble;

  LCD_PORT = cmd1 | LCD_E;
  CLOCK_BUSYWAIT_US(10);
  LCD_PORT = cmd1;
  CLOCK_BUSYWAIT_US(10);

  LCD_PORT = cmd2 | LCD_E;
  CLOCK_BUSYWAIT_US(10);
  LCD_PORT = cmd2;
  CLOCK_BUSYWAIT_US(10);
}

void lcd_init(void)
{
  CLOCK_BUSYWAIT_BIG_US(30000); //Wait 30ms after Vdd rises
  lcd_command(CMD0_FUNCTION_SET | FUNCTION_SET_2LINES | FUNCTION_SET_FONT_TYPE_5x11);
  CLOCK_BUSYWAIT_US(39);
  lcd_command(CMD0_DISPLAY_ONOFF | DISPLAY_ONOFF_DISPLAY_ON
    | DISPLAY_ONOFF_CURSOR_ON | DISPLAY_ONOFF_BLINKING_ON);
  CLOCK_BUSYWAIT_US(39);
  lcd_command(CMD0_DISPLAY_CLEAR);
  CLOCK_BUSYWAIT_BIG_US(1530); //Wait 1.53ms to clear display
  lcd_command(CMD0_ENTRY_MODE_SET | ENTRY_MODE_SET_DIRECTION_INCR);
}

