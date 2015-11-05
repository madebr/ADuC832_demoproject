#include <stdint.h>

#include "adc.h"
#include "dac.h"
#include "clock.h"
#include "interrupts.h"
#include "lcd.h"
#include "led.h"
#include "pwm.h"
#include "serial.h"
#include "watchdog.h"

#include "ADUC832.h"

// __code put variable in text section (variable will be const)
// __data/__near: directly addressable portion of internal RAM (will decrease maximum stack) (small memory model)
// __idata: indirectly addressable portion of internal ram
// __pdata: eXternal Data
// __xdata/__far puts variable in eXternal RAM (large memory model)

#include <string.h>

int main()
{
  uint8_t led_raw = 0xff;
  clock_init();
  adc_init();
  dac_init();
  serial_init();
  lcd_init();
  led_init();
  pwm_init();
  watchdog_init();
  interrupts_enable();
  while (1)
  {
    //char __idata str[16];
    char __pdata str1[] = "Hallo world\n";
    char __pdata str2[] = "FUCK YOU\n";
    uint8_t n;
    //strncpy(str, str1, sizeof(str1));
    n = serial_puts(str1);
    //strncpy(str, str2, 9);
    n = serial_puts(str2);
    CLOCK_BUSYWAIT_BIG_US(700000);
    led_write_raw(led_raw--);
    watchdog_tick();
  }
}
