#include <stdint.h>

#include "adc.h"
#include "clock.h"
#include "interrupts.h"
#include "lcd.h"
#include "pwm.h"
#include "rtc.h"
#include "serial.h"
#include "watchdog.h"
#include "time.h"

#include "ADUC832.h"

// __code put variable in text section (variable will be const)
// __data/__near: directly addressable portion of internal RAM (will decrease maximum stack) (small memory model)
// __idata: indirectly addressable portion of internal ram
// __pdata: eXternal Data
// __xdata/__far puts variable in eXternal RAM (large memory model)

int main()
{
  const char __code str1[] = "Input some data:";
  const char __code str2[] = " GOT SOMETHING\r\n";
  clock_init();
  adc_init();
  lcd_init();
  pwm_init();
  rtc_init();
  serial_init();
  watchdog_init();
  P2 = 0xff;
  interrupts_enable();
  adc_calibrate();
  serial_puts_blocking(str1);
  while (1)
  {
    if (serial_getc_available())
    {
      uint8_t d = serial_getc_nb();
      uint16_t sample;
      serial_putc_blocking(d);
      s_printf_b("(0x%x)", d);
      serial_puts_blocking(str2);
      lcd_clear();
      l_printf_b("%u:%u:%u:%u\r\n", HOUR, MIN, SEC, HTHSEC);
      serial_flush();
      //sample  = adc_sample_channel(ADC_CHANNEL_TEMPERATURE);
      sample  = adc_sample_channel(ADC_CHANNEL_7);
      l_printf_b("Sample=%u\r\n", sample);
      serial_puts_blocking(str1);
      //lcd_puts_b(d);
    }
    watchdog_tick();
  }
}
