#include "watchdog.h"

void watchdog_interrupt_handler(void) __interrupt INTERRUPT_WDS __using 1
{
}

void watchdog_init(void)
{
  //timeout period = (2 ** PRE x 2 ** 9 / f_xtal) (f_xtal=32.768kHz)
  // (0 <= PRE <= 7)
  WDWR = 1;

  //Start the watchdog.
  //The watchdog will reset after 500ms of no reset.
  WDCON = (0x5 << WDCON_PRE_shift) | WDCON_WDE_mask;

  //The watchdog will interrupt after 500ms of no reset.
  //WDCON = (0x0 << WDCON_PRE_shift) | WDCON_WDIR_mask | WDCON_WDE_mask;
}

