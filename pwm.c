#include "pwm.h"
#include "ADUC832.h"

void pwm_enable(void)
{
}

void pwm_disable(void)
{
  PWMCON = 0x0;
}
