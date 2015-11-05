#ifndef _RTC_H_
#define _RTC_H_

#include "ADUC832.h"

void rtc_init(void);

void rtc_interrupt_handler(void) __interrupt INTERRUPT_TII __using 1;

#endif //_RTC_H_
