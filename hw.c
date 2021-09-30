#include <stdio.h>
#include "types.h"
#include "hw.h"

int hw_IsInEvenNumberedMegabyte(uint32 addr, uint32 size)
{
  uint32 end = addr + size - 1;
  // Zero size makes no sense.
  //
  // size larger than 1MB means the buffer lies in two or more different
  // megabytes and one of them is odd-numbered.
  //
  // size of 1MB or smaller means the buffer lies in at most two different
  // megabytes, check where the first and last bytes are.
  return (size > 0) && (size <= ONE_MB) && !((addr | end) & ONE_MB);
}

int hw_IsA20Enabled(void)
{
  volatile unsigned char *p1 = (volatile unsigned char *)1;
  volatile unsigned char *p1M1 = (volatile unsigned char *)(ONE_MB + 1);
  unsigned char x1, x1M1;
  int enabled = 0;

  asm("cli");

  x1 = *p1;
  x1M1 = *p1M1;
  *p1M1 = ~x1;

  if (*p1 == x1)
  {
    *p1M1 = x1M1;
    enabled = 1;
  }
  else
  {
    *p1 = x1;
  }

  asm("sti");

  return enabled;
}

static void TryEnableA20_Bios(void)
{
  asm("push ebp\n"
      "push dword 0\n"
      "mov  ax, 0x2401\n"
      "int  0x15\n"
      "pop  es\n"
      "pop  ds\n"
      "pop  ebp");
}

unsigned char hw_InPortByte(unsigned short port)
{
  asm("mov   dx, [bp+8]\n"
      "in    al, dx\n"
      "movzx eax, al");
}
void hw_OutPortByte(unsigned short port, unsigned char value)
{
  asm("mov dx, [bp+8]\n"
      "mov al, [bp+12]\n"
      "out dx, al");
}

static void KbdWaitDone2(void)
{
  while ((hw_InPortByte(PORT_KBD_STATUS) & 2) == 2)
    ;
}
static void KbdWaitDone1(void)
{
  while (!(hw_InPortByte(PORT_KBD_STATUS) & 1))
    ;
}
static void KbdSendCmd(unsigned char cmd)
{
  hw_OutPortByte(PORT_KBD_STATUS, cmd);
}

static void TryEnableA20_AtKbd(void)
{
  unsigned char x;
  asm("cli");

  KbdWaitDone2();
  KbdSendCmd(0xAD);
  KbdWaitDone2();

  KbdSendCmd(0xD0);
  KbdWaitDone1();
  x = hw_InPortByte(PORT_KBD_A);
  KbdWaitDone2();

  KbdSendCmd(0xD1);
  KbdWaitDone2();

  hw_OutPortByte(PORT_KBD_A, x | 2 | 1); // set bit 0 to prevent reset
  KbdWaitDone2();

  KbdSendCmd(0xAE);
  KbdWaitDone2();

  asm("sti");
}

static void TryEnableA20_Ps2Fast(void)
{
  unsigned char x;
  asm("cli");
  x = hw_InPortByte(0x92) & 0xFE; // reset bit 0 to prevent reset
  if ((x & 2) == 0)
    hw_OutPortByte(0x92, x | 2);
  asm("sti");
}

// This A20 code is likely imperfect (no delays, no retries),
// but it appears to be working in DOSBox and VirtualBox.
// See:
// http://wiki.osdev.org/A20
// http://www.win.tue.nl/~aeb/linux/kbd/A20.html
// http://lxr.free-electrons.com/source/arch/x86/boot/a20.c
int hw_EnableA20(void)
{
  if (hw_IsA20Enabled())
  {
    puts("A20 already enabled.");
    return 1;
  }
  // Try himem.sys first? OTOH, if it's loaded, it's probably enabled A20 already.
  puts("Trying to enable A20 via BIOS...");
  TryEnableA20_Bios();
  if (hw_IsA20Enabled())
  {
    puts("A20 enabled via BIOS.");
    return 1;
  }
  puts("Trying to enable A20 via keyboard controller...");
  TryEnableA20_AtKbd();
  if (hw_IsA20Enabled())
  {
    puts("A20 enabled via keyboard controller.");
    return 1;
  }
  puts("Trying to enable A20 via fast method...");
  TryEnableA20_Ps2Fast();
  if (hw_IsA20Enabled())
  {
    puts("A20 enabled via fast method.");
    return 1;
  }
  puts("Failed to enable A20!");
  return 0;
}

void hw_waitForRetrace(void)
{
  /*https://github.com/JesseMaurais/VGAlib/blob/master/VGAio.h*/
  //not sure how to do this yet or even if its needed

}