#include "encoding.h"
#include <stdint.h>

#define CYCLES_PER_SECONDS 50000000
#define UART_BASE_ADDRESS 0x100000
#define MTIMECMP 0x200000
#define MTIME 0x200008

#define TIMER_COUNT 32768

void putch(char ch)
{
  *((volatile char*)UART_BASE_ADDRESS) = ch;
}

void increase_timer_interrupt(long long counter)
{
  unsigned long long volatile * const port_mtimecmp = (unsigned long long*) MTIMECMP;
  unsigned long long volatile * const port_mtime = (unsigned long long*) MTIME;
  unsigned long long val_mtimecmp = *port_mtimecmp;
  unsigned long long val_mtime = *port_mtime;
  val_mtimecmp = val_mtime + counter;
  *port_mtimecmp = val_mtimecmp;
}

void print_mcsr(uint64_t mcsr)
{
  char str[16];
  char c;
  uint8_t mod;
  for (int i=15; i>=0; i--)
  {
    mod = mcsr % 16;
    mcsr = mcsr / 16;
    if (mod >= 10)
    {
      c = 'A' + mod - 10;
    }
    else
    {
      c = '0' + mod;
    }
    str[i] = c;
  }
  putch('0');
  putch('x');
  for (int i=0; i<16; i++)
  {
    putch(str[i]);
  }
  putch('\r');
  putch('\n');
}

void handle_timer_interrupt()
{
  increase_timer_interrupt(TIMER_COUNT);

  static unsigned int min = 0;
  static unsigned int sec = 0;
  unsigned char min1,min0;
  unsigned char sec1,sec0;

  min1 = '0' + min / 10;
  min0 = '0' + min % 10;
  sec1 = '0' + sec / 10;
  sec0 = '0' + sec % 10;
  putch(27);
  putch('[');
  putch('2');
  putch('J');
  putch(27);
  putch('[');
  putch('H');
  putch(min1);
  putch(min0);
  putch(':');
  putch(sec1);
  putch(sec0);
  putch('\r');
  putch('\n');
  sec = sec + 1;
  if ((sec % 60) == 0)
  {
    min = min + 1;
    sec = 0;
  }
  if ((min % 60) == 0)
  {
    min = 0;
  }
}

void init_timer_interrupt()
{
  increase_timer_interrupt(TIMER_COUNT);

  uintptr_t address;

  __asm__("la %0,_mtvec" : "=r"(address));

  write_csr(mtvec,address);

  unsigned int val;

  val = 0;

  val |= MSTATUS_MIE;

  write_csr(mstatus,val);

  val = 0;

  val |= MIP_MTIP;

  write_csr(mie,val);
}

int main()
{
  init_timer_interrupt();

  while(1);
}
