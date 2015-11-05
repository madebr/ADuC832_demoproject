#ifndef _PRINTF_H_
#define _PRINTF_H_

#include <stdint.h>

typedef void (*putc_t)(uint8_t);

void printf_fn(putc_t fn, const char *fmt, ...) __reentrant;

#endif //_PRINTF_H_
