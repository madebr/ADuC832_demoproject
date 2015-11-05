#ifndef _PWM_H_
#define _PWM_H_

#include <stdint.h>

#include "ADUC832.h"

#define pwm_init pwm_disable

void pwm_enable(void);

static inline void pwm_disable(void)
{
    PWMCON = 0x0;
}

#endif

