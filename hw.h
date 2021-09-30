#ifndef HW_H
#define HW_H

#include "types.h"

enum
{
  ONE_MB = 0x100000,
  PORT_KBD_A = 0x60,
  PORT_KBD_STATUS = 0x64
};

int hw_IsInEvenNumberedMegabyte(uint32 addr, uint32 size);
int hw_IsA20Enabled(void);
unsigned char hw_InPortByte(unsigned short port);
void hw_OutPortByte(unsigned short port, unsigned char value);
int hw_EnableA20(void);

void hw_waitForRetrace(void);

#endif