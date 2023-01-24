/************** uart.c file *******************
Flag register (offset 0x18): status of UART port

   7    6    5    4    3   2   1   0
| TXFE RXFF TXFF RXFE BUSY -   -   - |
  
TXFE=Tx buffer empty
RXFF=Rx buffer full
TXFF=Tx buffer full
RXFE=Rx buffer empty
BUSY = device busy
**********************************************/
  
#define DR 0x00
#define FR 0x18

typedef struct uart{
  char *base;           // base address
  int  n;               // UART number 
}UART;

UART *up;

// versatile_epb : UART0 at 0x101F1000

void uart_init()
{
  up->base = (UART *)(0x101F1000);
  up->n = 0;
}

void uputc(char c)
{
  while ( *(up->base + FR) & 0x20 ); // while FR.bit5 != 0 ;  (TX FULL)
  *(up->base + DR) = (int)c;         // write c to DR
}

int ugetc()
{
  while ( *(up->base + FR) & 0x10 ); // while FR.bit4 != 0 ;  (RX EMPTY)
  return *(up->base + DR);           // get data in DR
}

int uprints(char *s)
{
  while(*s)
    uputc(*s++);
}
