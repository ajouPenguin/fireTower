#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include "touch.h"

#define  MAX_BUFF	200
#define MAX_TOUCH_X	0x740
#define MAX_TOUCH_Y	0x540

void readFirstCoordinate(int fd, int* cx , int* cy, int screen_width, int screen_height)
{
	struct input_event event;
	int readSize;
	while(1)
	{
		readSize = read(fd, &event, sizeof(event));

		if ( readSize == sizeof(event) )
		{
			if( event.type == EV_ABS )
			{
				if (event.code == ABS_MT_POSITION_X )
				{
					*cx = event.value*screen_width/MAX_TOUCH_X; 
					printf("%d\n", *cx);
				}
				else if ( event.code == ABS_MT_POSITION_Y )
				{
					*cy = event.value*screen_height/MAX_TOUCH_Y;
					printf("%d\n", *cy);
				}
			}
			else if ((event.type == EV_SYN) && (event.code == SYN_REPORT ))
			{
				break;
			}

		}
	}
}

void initScreen(unsigned char *fb_mem, int screen_width, int screen_height)
{
    int		coor_y;
    int		coor_x;
    unsigned long *ptr;

    for(coor_y = 0; coor_y < screen_height; coor_y++)
    {
        ptr =   (unsigned long *)fb_mem + screen_width * coor_y ;
        for(coor_x = 0; coor_x < screen_width; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
}
