/*
    RetroRen GFX library example
    September 29, 2021 - DJ Kunkel
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "vbe.h"

int pos=0;

void update()
{
    pos = (pos+1)%VBE_WIDTH;
}

void draw()
{
    line(pos,10,pos,470,10);
}

int main(void)
{
    if (!vbe_Init())
    {
        return EXIT_FAILURE;
    }

    for(int i=0; i < VBE_WIDTH*4; i++)
    {
        update();
        draw();
        vbe_Swap();
    }

    vbe_End();

    printf("This has been a production of the Retro Renaissance...\n");

    return 0;
}

