#ifndef _PWM_H_
#define _PWM_H_

#include <stdint.h>

#define pwm_init pwm_disable

void pwm_enable(void);
void pwm_disable(void);

#endif

