#ifndef MEM_H
#define MEM_H

#include "types.h"

void *memcpy_32(void *dst, void *src, unsigned n);
void *memset_32(void *s, uint8 c, unsigned n);

#endif