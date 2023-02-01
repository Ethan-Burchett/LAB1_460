/*********** uart.c file **********/
#define DR 0x00
#define FR 0x18

typedef struct uart{
  char *base;           // base address
  int   unit;           // UART ID number
}UART;

UART *up;

// versatile_epb : UART0 at 0x101F1000

void uart_init()
{
  up->base = (UART *)(0x101F1000);
  up->unit = 0;
}

//loop while data buffer is full -> or wait until data buffer is empty 
//then write char to data buffer at up->base + DR (ofset to write to data buffer location in UART memory)
void uputc(char c)
{
  while ( *(up->base + FR) & 0x20 );  // while FR.bit5 != 0 ;
  *(up->base + DR) = (int)c;          // write c to DR  
}

//loop while the data buffer is empty -> or wait until data buffer is full 
// return value from data buffer
int ugetc()
{
  while ( *(up->base + FR) & 0x10 ); // while FR.bit4 != 0 ;
  return *(up->base + DR);           // DR has data
}

void uprints(char *s)
{
  while(*s)
    uputc(*s++);
}

// loop as long as character returned is not '\r' 
// put *s back into UART 
// increment *s pointer to next character
// note: *s get current value at data buffer location on each loop. 
int ugets(char *s)
{
  uprints("in ugets\n");
  while ((*s = ugetc(up)) != '\r')
  {
    uputc(*s);
    s++;
  }
}

// write your own ugets(char s[ ]) to input a string (end by \r)
// example: enter 12345\r ==> s[ ] = "12345";

// Write your own uprintf(char *fmt, ...) for formatter printing as in CS360
