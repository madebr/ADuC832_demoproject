#include "clock.h"
#include "ADUC832.h"

void clock_init(void)
{
  //Run at 16.78MHz
  PLLCON = 0;
  CLOCK_BUSYWAIT_US(50);
}

void clock_busywait_big(uint8_t cycli_msb, uint8_t cycli_lsb)
{
  cycli_msb; cycli_lsb;
  __asm;
    mov r7,dpl
    inc r7 ;	clock.c:13: cycli_msb++;
  start_msb_loop:
    djnz r7, end_msb_loop ;while (--cycli_msb)
    mov r6,#0x00; v = 0
  inner_msb_loop:
    djnz r6, inner_msb_loop ;while (--v);
    sjmp start_msb_loop
  end_msb_loop:
    inc _clock_busywait_big_PARM_2 ;cycli_lsb++
    mov	r7,_clock_busywait_big_PARM_2
  lsb_loop:
    djnz r7, lsb_loop ;while (--cycli_lsb--);
__endasm;
/*
  cycli_msb++;
  while (--cycli_msb)
  {
    uint8_t v = 0;
    while (--v);
  }
  cycli_lsb++;
  while (--cycli_lsb);
*/
}

void clock_busywait(uint8_t cycli)
{
  //Jump to this function: 24 cycles
  cycli;
  __asm;
    ;us++;
    ;while (--us);
    mov r7,dpl      ;24 cycles
    inc r7          ;12 cycles
  loop:
    djnz r7,loop   ;24 cycles * us
  __endasm;
}

