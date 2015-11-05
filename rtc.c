#include "rtc.h"

#include "clock.h"

void rtc_interrupt_handler(void) __interrupt INTERRUPT_TII __using 1
{
  P2_0 ^= 1;
  TIMECON &= ~TIMECON_TII_mask;
}

void rtc_init(void)
{
  INTVAL = 2; //Interrupt every XX seconds
  HOUR = 0;
  MIN = 0;
  SEC = 0;
  HTHSEC = 0;
  CLOCK_BUSYWAIT_US(50);
  TIMECON = (TIMECON_TIEN_mask | TIMECON_ITS_SECONDS_mask | TIMECON_TCEN_mask);
  IEIP2 |= IEIP2_ETI_mask;
}
