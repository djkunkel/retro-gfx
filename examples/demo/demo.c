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
int y_pos = 10;
uint8 bg_color = 0;
uint8 line_color=10;

//update game state for this frame
void update()
{
    x_pos = (x_pos + 1) % VBE_WIDTH;
    y_pos = (int)(sin(x_pos/40.0)*100+120);
    line_color = (line_color+1%256);
}

//draw this frame
void draw()
{
    line(x_pos, y_pos, x_pos, VBE_HEIGHT-y_pos, line_color);
    line(x_pos+1, y_pos, x_pos+1, VBE_HEIGHT-y_pos, line_color);
    line(x_pos+2, y_pos, x_pos+2, VBE_HEIGHT-y_pos, line_color);
    line(x_pos+3, y_pos, x_pos+3, VBE_HEIGHT-y_pos, line_color);
}

//run a loop of drawing, swapping, syncing (todo)
int main(void)
{
    
    //init vbe with the hardcoded size
    if (!vbe_Init())
    {
        return EXIT_FAILURE;
    }

    //do our loop

    for (int i = 0; i < VBE_WIDTH-1; i++)
    {
        update();
        draw();
        vbe_Swap(bg_color);
        // if(getchar()=='q')
        //     break;
    }

    //getchar();

    vbe_End();

    printf("This has been a production of the Retro Renaissance...\n");

    return 0;
}
