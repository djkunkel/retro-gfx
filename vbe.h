#ifndef VBE__H
#define VBE__H

#include "types.h"

extern uint8 *screen;

enum
{
  // Only 8bpp modes are supported.
  //  VBE_MODE = 0x100, VBE_WIDTH = 640, VBE_HEIGHT = 400,
  VBE_MODE = 0x101,
  VBE_WIDTH = 640,
  VBE_HEIGHT = 480,
  //  VBE_MODE = 0x103, VBE_WIDTH = 800, VBE_HEIGHT = 600,
  //  VBE_MODE = 0x105, VBE_WIDTH = 1024, VBE_HEIGHT = 768,
  //  VBE_MODE = 0x107, VBE_WIDTH = 1280, VBE_HEIGHT = 1024, // > 1MB in size, will need A20 in unreal mode(l)
  VBE_AREA = VBE_WIDTH * VBE_HEIGHT
};

int vbe_Init();
void vbe_End();
void vbe_Swap();

int vbe_GetInfo(tVbeInfo *p);
int vbe_GetModeInfo(tVbeModeInfo *p, uint16 mode);
int vbe_SetMode(uint16 mode);
void pixel(int x, int y,
           uint8 color);
void line(int x1, int y1,
          int x2, int y2,
          uint8 color);

#endif