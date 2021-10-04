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

//game state
int x_pos = 0;
int y_pos = 20;
uint8 bg_color = 0;

//update game state for this frame
void update()
{
    x_pos = (x_pos + 1) % VBE_WIDTH;
    y_pos = (int)(sin(x_pos/40.0)*100+120);
}

//draw this frame
void draw()
{
    line(x_pos, y_pos, x_pos, 480-y_pos, 10);
}

//run a loop of drawing, swapping, syncing (todo)
int main(void)
{
    
    //init vbe with the hardcoded size
    if (!vbe_Init())
    {
        return EXIT_FAILURE;
    }
        
    //get the address of the buffer so we can tell the user where it was 
    uint32 buffAddress = &buffer;

    //do our loop 3 times until we can read the keyboard or something better
    for (int i = 0; i < VBE_WIDTH * 3; i++)
    {
        update();
        draw();
        vbe_Swap(bg_color);
    }

    vbe_End();

    printf("The back buffer was at 0x%08x\n",buffAddress);

    printf("This has been a production of the Retro Renaissance...\n");

    return 0;
}
