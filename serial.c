#include "serial.h"

#include <stdint.h>

#define DEBUFSIZE 32
#define DEBUFSIZEMASK (DEBUFSIZE-1)
#if DEBUFSIZE & (DEBUFSIZE-1)
#error DEBUFSIZE must be an exponential of 2
#endif

//Circular RX/TX buffers. Always keep one slot open.

//debug variable: check whether the interrupt handler is actually called

//For TX: only the get pointer is modified by the interrupt service.
struct outdebuffer {
  uint8_t put;
  volatile uint8_t get;
  uint8_t buffer[DEBUFSIZE];
};

//For RX: only the put pointer is modified by the interrupt service.
struct indebuffer {
  volatile uint8_t put;
  uint8_t get;
  uint8_t buffer[DEBUFSIZE];
};

__xdata struct outdebuffer outbuffer;
__xdata struct indebuffer inbuffer;

void serial_init(void)
{
  outbuffer.put = 0;
  outbuffer.get = 0;
  inbuffer.put = 0;
  outbuffer.get = 0;

  //SCON = 0x10; //Enable Reception
  ES = 1; //Enable Serial Interrupt (IE.ES)
}

void serial_interrupt_handler(void) __interrupt INTERRUPT_RI_TI __using 1
{
  if (TI) //SCON.TI
  {
    uint8_t getpos;
    getpos = outbuffer.get;
    if (outbuffer.put != getpos)
    {
      SBUF = outbuffer.buffer[getpos];
      outbuffer.get = (getpos + 1) & DEBUFSIZEMASK;
    }
    else
    {
      TI = 0;//SCON.TI
    }
  }
  if (RI)//SCON.RI
  {
    uint8_t putpos, nextput;
    RI = 0;
    putpos = inbuffer.put;
    nextput = (putpos + 1) & DEBUFSIZEMASK;
    if (nextput != inbuffer.get)
    {
      inbuffer.buffer[putpos] = SBUF;
      inbuffer.put = nextput; 
    }
  }
}

uint8_t serial_get(void)
{
  if (inbuffer.put == inbuffer.get)
    return 0;
  else
  {
    uint8_t c = inbuffer.buffer[inbuffer.get];
    inbuffer.get = (inbuffer.get + 1) & DEBUFSIZEMASK;
    return c;
  }
}

void serial_putc(uint8_t c)
{
  uint8_t nextput = (outbuffer.put + 1) & DEBUFSIZEMASK;
  while (nextput == outbuffer.get);
  //ES = 0; //No need to disable interrupts since only modifying non-conflicting variables
  outbuffer.buffer[outbuffer.put] = c;
  outbuffer.put = nextput;
  TI = 1;
  //ES = 1;
}

uint8_t serial_puts(char *s)
{
  uint8_t i = 0;
  uint8_t put = outbuffer.put;
  while (*s)
  {
    uint8_t nextput = (put + 1) & DEBUFSIZEMASK;
    if (nextput == outbuffer.get)
    {
      // No place available
      break;
    }
    else
    {
      outbuffer.buffer[put] = *s;
      ++i;
      ++s;
      put = nextput;
      continue;
    }
  }
  outbuffer.put = put;
  TI = 1;
  return i;
}
