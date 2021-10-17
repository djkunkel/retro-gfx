#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw.h"
#include "types.h"
#include "mem.h"
#include "vbe.h"



//the back buffer that the user builds frames to
uint8 *buffer;
uint8 *screen;
uint16 scanline=0;


int vbe_Init()
{
    tVbeInfo vbeInfo;
    tVbeModeInfo modeInfo;
    int err;

    // Get and check VBE info

    if ((err = vbe_GetInfo(&vbeInfo)) != 0x4F)
    {
        printf("GetVbeInfo() failed (0x%08X)!\n", err);
        return 0;
    }

    printf("%.4s %u.%u \"%s\"\n",
           vbeInfo.VbeSignature,
           vbeInfo.VbeVersion >> 8, vbeInfo.VbeVersion & 0xFF,
           ((vbeInfo.OemStringPtr >> 16) << 4) + (vbeInfo.OemStringPtr & 0xFFFF));

    if (vbeInfo.VbeVersion < 0x0200)
    {
        printf("VBE 2.0+ required!\n");
        return 0;
    }

    // Get and check VBE mode info

    if ((err = vbe_GetModeInfo(&modeInfo, VBE_MODE)) != 0x4F)
    {
        printf("GetVbeModeInfo() failed (0x%08X)!\n", err);
        return 0;
    }

   

    if ((modeInfo.ModeAttributes & 0x80) == 0 || modeInfo.PhysBasePtr == 0)
    {
        printf("Linear Frame Buffer not available!\n", err);
        return 0;
    }

    // Map the linear frame buffer into the virtual address space
    if (!hw_IsInEvenNumberedMegabyte(modeInfo.PhysBasePtr, VBE_AREA) && !hw_EnableA20())
    {
        printf("Linear Frame Buffer at 0x%08X (physical) needs A20 enabled!\n", modeInfo.PhysBasePtr);
        return 0;
    }
    
    //set up draw pointers
    screen = modeInfo.PhysBasePtr;
    buffer = screen+VBE_AREA;

    printf("Bytes per scanline: %d\n", modeInfo.BytesPerScanLine);
    printf("x res: %d\n", modeInfo.XResolution);
    printf("y res: %d\n", modeInfo.YResolution);
    printf("Image pages: %d\n", modeInfo.NumberOfImagePages);
    printf("bits per pixel: %d\n", modeInfo.BitsPerPixel);

    printf("Frame buffer start: 0x%08X\n", modeInfo.PhysBasePtr);
    printf("Back buffer start: 0x%08X\n",buffer);
    printf("Offscreen offset: 0x%08X\n", modeInfo.OffScreenMemoryOffset);
    printf("Offscreen memory size: 0x%08X\n", modeInfo.OffScreenMemSize);
    
    
    printf("Press enter to continue...\n");
    getchar();

    // Set mode VBE_MODE (VBE_WIDTH x VBE_HEIGHT x 8bpp) with LFB
    if ((err = vbe_SetMode(VBE_MODE | (1 << 14) /*use LFB*/)) != 0 && err != 0x4F)
    {
        printf("SetVbeMode() failed (0x%08X)!\n", err);
        return 0;
    }

    

    return 1;
}

int vbe_SetDisplayStart(const uint16 scanline){
    asm(
        "mov   ax, 0x4f07\n"
        "mov   bh, 0x00 \n"
        "mov   bl, 0x80 \n" //0x80 says do the swap at vertical refresh
        "mov   cx, 0x00 \n"
        "mov   dx, [bp+8]\n"
        "int   0x10\n"
        "movzx eax,ax\n"
    );
}


void vbe_Swap(uint8 color)
{
    
    //swap buffers
    uint8* temp = screen;
    screen=buffer;
    buffer=temp;

    //set scanline for the display start
    if(scanline==VBE_HEIGHT){
        scanline=0;
    }else{
        scanline=VBE_HEIGHT;
    }

    //tell the video card the new scanline to start from
    vbe_SetDisplayStart(scanline);
    
    //wait for sync, shouldn't be needed with 0x80 option in set display start
    //s3 card on Packard Bell seemed to need Univbe to work correctly

    // while (hw_InPortByte(0x3DA) & (1<<3));
    // while (!(hw_InPortByte(0x3DA) & (1<<3)));
  
    //clear the back buffer
    memset_32(buffer,color,VBE_AREA);

}

void vbe_End()
{
    //free back buffer
    //free(buffer);
    //switch back to text mode
    vbe_SetMode(0x3);
}

int vbe_GetInfo(tVbeInfo *p)
{
    asm(
        "mov   ax, 0x4f00\n"
        "mov   edi, [bp+8]\n"
        "ror   edi, 4\n"
        "mov   es, di\n"
        "shr   edi, 28\n"
        "int   0x10\n"
        "movzx eax, ax\n"
        "push  word 0\n"
        "pop   es");
}

int vbe_GetModeInfo(tVbeModeInfo *p, uint16 mode)
{
    asm(
        "mov   ax, 0x4f01\n"
        "mov   cx, [bp+12]\n" //mode number
        "mov   edi, [bp+8]\n" //mode info struct
        "ror   edi, 4\n"
        "mov   es, di\n"
        "shr   edi, 28\n"
        "int   0x10\n"
        "movzx eax, ax\n"
        "push  word 0\n"
        "pop   es");
}

int vbe_SetMode(uint16 mode)
{
    asm(
        "mov   ax, 0x4f02\n"
        "mov   bx, [bp+8]\n"
        "int   0x10\n"
        "movzx eax, ax");
}

void pixel(int x, int y,
           uint8 color)
{
    buffer[y * VBE_WIDTH + x] = color;
}

void line(int x1, int y1,
          int x2, int y2,
          uint8 color)
{
    int sx, sy, dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;

    sx = x2 - x1;
    sy = y2 - y1;

    if (sy < 0 || sy == 0 && sx < 0)
    {
        k = x1;
        x1 = x2;
        x2 = k;
        k = y1;
        y1 = y2;
        y2 = k;
        sx = -sx;
        sy = -sy;
    }

    if (sx > 0)
        dx1 = 1;
    else if (sx < 0)
        dx1 = -1;
    else
        dy1 = 0;

    if (sy > 0)
        dy1 = 1;
    else if (sy < 0)
        dy1 = -1;
    else
        dy1 = 0;

    m = (sx >= 0) ? sx : -sx;
    n = (sy >= 0) ? sy : -sy;
    dx2 = dx1;
    dy2 = 0;

    if (m < n)
    {
        m = (sy >= 0) ? sy : -sy;
        n = (sx >= 0) ? sx : -sx;
        dx2 = 0;
        dy2 = dy1;
    }

    x = x1;
    y = y1;
    cnt = m + 1;
    k = n / 2;

    while (cnt--)
    {
        if (x >= 0 && x < VBE_WIDTH && y >= 0 && y < VBE_HEIGHT)
            buffer[y * VBE_WIDTH + x] = color;

        k += n;
        if (k < m)
        {
            x += dx2;
            y += dy2;
        }
        else
        {
            k -= m;
            x += dx1;
            y += dy1;
        }
    }
}

