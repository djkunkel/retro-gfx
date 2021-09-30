#include "types.h"
#include "mem.h"

void* memcpy_32(void* dst, void* src, unsigned n)
{

  char* p1 = dst;
  char* p2 = src;

  uint32* pl1 = dst;
  uint32* pl2 = src;

  while(n-4>0)
  {
      n-=4;
      p1+=4;
      p2+=4;

      *pl1++ = *pl2++;
  }

  while (n--)
    *p1++ = *p2++;

  return dst;
}

void* memset_32(void* s, uint8 c, unsigned n)
{
  char* p = s;
  uint32* pl = s;

  uint32 val=(uint32)(c<<24|c<<16|c<<8|c);

  while(n-4>0)
  {
      n-=4;
      p+=4;
      *pl++ = val;
  }

  while (n--)
    *p++ = c;

  return s;
}