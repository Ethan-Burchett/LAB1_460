/*********** uart.c file **********/
#define DR 0x00
#define FR 0x18

typedef struct uart
{
  char *base; // base address
  int unit;   // UART ID number
} UART;

UART *up;

#include "stdio.h"
// versatile_epb : UART0 at 0x101F1000

void uart_init()
{
  up->base = (UART *)(0x101F1000);
  up->unit = 0;
}

// loop while data buffer is full -> or wait until data buffer is empty
// then write char to data buffer at up->base + DR (ofset to write to data buffer location in UART memory)
void uputc(char c) // write c to data buffer
{
  while (*(up->base + FR) & 0x20)
    ;                        // while FR.bit5 != 0 ;
  *(up->base + DR) = (int)c; // write c to DR
}

// loop while the data buffer is empty -> or wait until data buffer is full
//  return value from data buffer
int ugetc()
{
  while (*(up->base + FR) & 0x10)
    ;                      // while FR.bit4 != 0 ;
  return *(up->base + DR); // DR has data
}

void uprints(char *s)
{
  while (*s)
    uputc(*s++);
}

// loop as long as character returned is not '\r'
// put *s back into UART
// increment *s pointer to next character
// note: *s get current value at data buffer location on each loop.
int ugets(char *s)
{
  // uprints("in ugets\n");
  while ((*s = ugetc(up)) != '\r')
  {
    uputc(*s);
    s++;
  }
}

// prints, *printu*, printc(putchar) - ignore hex for now - implement later if we need it

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";

int rpu(u32 x) // recursive put char
{
  char c;
  if (x)
  {
    c = ctable[x % 10];
    rpu(x / 10);
  }
  uputc(c);
}

int printu(u32 x)
{
  (x == 0) ? uputc('0') : rpu(x);
}

int prints(char *s)
{
  while (*s)
  {
    uputc(*s);
    s++;
  }
}

int rpx(int x)
{
  char c;
  if (x)
  {
    c = ctable[x % 16];
    rpx(x / 16);
  }
  uputc(c);
}

int printx(int x)
{
  // uprints("in printx\n");
  uputc('0');
  uputc('x');
  if (x == 0)
  {
    uputc('0');
  }
  else
  {
    rpx(x);
  }
  uputc(' ');
}

int printi(int x)
{
  if (x < 0)
  {
    uputc('-');
    x = -x;
  }
  printu(x);
}

int uprintf(char *fmt, ...)
{
  int *ip;
  char *cp;
  cp = fmt;
  ip = (int *)&fmt + 1;

  while (*cp)
  {
    if (*cp != '%')
    {
      uputc(*cp);
      if (*cp == '\n')
      {
        uputc('\r');
      }
      cp++;
      continue;
    }
    cp++;
    // uprints("-cp:");
    // uputc(*cp);
    // uprints("-\n");
    switch (*cp)
    {
    case 'c':
      uputc((char)*ip);
      break;
    case 's':
      prints((char*)*ip);
      break;
    case 'd':
      printi(*ip);
      break;
    case 'u':
      printu(*ip);
      break;
    case 'x':
      printx(*ip);
      break;
    }
    cp++;
    ip++;
  }
}