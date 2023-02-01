#include <stdio.h>
/*********** uart.c file **********/
#define DR 0x00
#define FR 0x18

typedef struct uart{
  char *base;           // base address
  int   unit;           // UART ID number
}UART;

UART *up;

// versatile_epb : UART0 at 0x101F1000

char *ctable = "0123456789ABCDEF";
// int BASE = 10; // hard code 10 to make this work

void uart_init()
{
  up->base = (UART *)(0x101F1000);
  up->unit = 0;
}

void uputc(char c)
{
  while ( *(up->base + FR) & 0x20 );  // while FR.bit5 != 0 ;
  *(up->base + DR) = (int)c;          // write c to DR  
}

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

int ugets(char *s)
{
  uprints("ugets(): under construction\n\r");
}

// write your own ugets(char s[ ]) to input a string (end by \r)
// example: enter 12345\r ==> s[ ] = "12345";


int rpu(u32 x) // recursive put char
{
    char c;
    if (x)
    {
        c = ctable[x % 10];
        rpu(x / 10);
        uputc(c);
    }
}

int rpx(u32 x) // recursive put int in hex
{
    int hexBASE = 16;
    char c;
    if (x)
    {
        c = ctable[x % 16];
        rpu(x / 16);
        uputc(c);
    }
}

int rps(char *x) // recursive put char
{
    //this marches through the string printing one char each recursive step
    if (*x != '\0')
    {
        uputc(*x);
        rps(x + 1);
    }
}

int printu(u32 x) // unsigned integer
{
    (x == 0) ? uputc('0') : rpu(x);
}

int prints(char *s)
{
    (s == 0) ? uputc('0') : rps(s);
}

int printx(u32 x)
{
    prints("0x");
    (x == 0) ? uputc('0') : rpx(x);
}

int printd(int x)
{
    if (x < 0)
    {
        prints("-");
        printu(-x);
    }
    else
      printu(x);
}

int uprintf(char *fmt, ...)
{
    printf(" in uprintf \n");

    char *cp = fmt;
    int *ip = (int *)&fmt + 1;
    // Use cp to scan the format string for %TYPE symbols;
    // Use ip to access and print each item by TYPE;

    while (*cp != '\0')
    {
        if (*cp == '%')
        {
            cp++;
            char type = *cp;
            //printf("[t: %c] ",type);
            if (type == 'd')
            {
                printd(*ip);
            }
            if (type == 's')
            {
                prints((char *)*ip);
            }
            if (type == 'u')
            {
                printu(*ip);
            }
            if (type == 'x')
            {
                printx(*ip);
            }
            if (type == 'c')
            {
                uputc(*ip);
            }
            ip++;
            cp++;
        }
        if (*cp == 92)
        {
            cp++;
            if (*cp == 'n')
            {
                cp++;
                uputc(13); // '\r'
            }
        }
        if (*cp != '%')
        {
            uputc(*cp);
        }
        cp++;
    }
}


// int uprintf(char *fmt, ...)
// {
//   char *cp = fmt;

//   int *ip = (int *) &fmt +1;

//   while (*cp != '\0')
//   {
//     if (*cp != '%')
//         {
//             putchar(*cp);
//         }
//         cp++;
//   }
// }

// Write your own uprintf(char *fmt, ...) for formatter printing as in CS360
// uprintf("s=%s u=%u i=%d x=%x\n", "string", 128, -100, 256);

// typedef unsigned int u32;

// char *ctable = "0123456789ABCDEF";
// int  BASE = 10; 

// int rpu(u32 x)t
// {  
//     char c;
//     if (x){
//        c = ctable[x % BASE];   
//        rpu(x / BASE);
//        putchar(c);           
//     }
// }

// int printu(u32 x)
// {
//    (x==0)? putchar('0') : rpu(x);
//    putchar(' ');
// }