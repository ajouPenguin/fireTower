#include "touch.h"
#include "display.h"
#include "keypad.h"
#include <malloc.h>
#include <memory.h>

/* This function prints login screen to touch LCD.
 	 When keypad pushed, empty rectangle change to stuffed rectangle with circle. */

int login(unsigned char *fmap, unsigned char *backBuffer, int screen_width, int screen_height, int fd)
{
	BITMAPDATA loginBackground;																						// login background image
	BITMAPDATA stuff, empty;																							// stuff is stuffed rectangle; empty is empty rectangle.
	int cnt = 0;																													// inputted password counter
	char password[4];																											// save input password numbers
	int arr[17] = {0, 1, 2, 3, 0, 4, 5, 6, 0, 7, 8, 9, 0, 11, 10, 12, 0};	// The number that corresponds to the number received from the keypad
	int x = 117, y = 595, add = 150;																			// x and y are the location of the first rectangle; add is the intervals between rectagles.

	backBuffer = (char*)malloc(screen_width * screen_height * 4);					// backBuffer has screen data before it appears in the front buffer.

	loginBackground = open_bitmap("back.bmp");														// login image
	stuff = open_bitmap("stuff1.bmp");																		// stuffed rectangle image
	empty = open_bitmap("empty1.bmp");																		// empty rectangle image

	draw_bitmap(backBuffer, screen_width, screen_height, 0, 0, loginBackground);	// Draw the login image in the position (0, 0) of the backBuffer.
	draw_bitmap(backBuffer, screen_width, screen_height, y, x, empty);						// Draw the empty rectangle image in the position (x, y) of the backBuffer.
	draw_bitmap(backBuffer, screen_width, screen_height, y, x + add, empty);			// Draw the empty rectangle image in the position (x + add, y) of the backBuffer.
	draw_bitmap(backBuffer, screen_width, screen_height, y, x + add * 2, empty);	// Draw the empty rectangle image in the position (x + add * 2, y) of the backBuffer.
	draw_bitmap(backBuffer, screen_width, screen_height, y, x + add * 3, empty);	// Draw the empty rectangle image in the position (x + add * 3, y) of the backBuffer.
	memcpy(fmap, backBuffer, screen_width * screen_height * 4);										// Copy the backBuffer's data to front buffer.

	while(cnt <= 4)
	{
		int temp = arr[Keypad(fd)];
		if(temp == 0)																																// keypad doesn't be pushed
		{
			continue;
		}
		else if(temp == 11)																													// Cancel button pushed
		{
			if(cnt > 0)
			{
				cnt--;
			}
			draw_bitmap(backBuffer, screen_width, screen_height, y, x + add * cnt, empty);	// Replace the rightmost stuffed rectangle with an empty rectangle.
			memcpy(fmap, backBuffer, screen_width * screen_height * 4);
			temp = 0;
		}
		else if((temp == 12) && (cnt == 4) && (password[0] == 1) && (password[1] == 2) && (password[2] == 3) && (password[3] == 4))	// When all entered number is correct and enter key is pushed then return 1.
		{
			close_bitmap(&loginBackground);
			close_bitmap(&stuff);
			close_bitmap(&empty);
			return 1;
		}
		else if(temp == 12)					// When enter key is input then clear all inputted number.
		{
			cnt = 0;
			draw_bitmap(backBuffer, screen_width, screen_height, y, x, empty);
			draw_bitmap(backBuffer, screen_width, screen_height, y, x + add, empty);
			draw_bitmap(backBuffer, screen_width, screen_height, y, x + add * 2, empty);
			draw_bitmap(backBuffer, screen_width, screen_height, y, x + add * 3, empty);
			memcpy(fmap, backBuffer, screen_width * screen_height * 4);
			temp = 0;
		}
		else if(cnt < 4)						// When entered key is number and cnt less than 4
		{
			password[cnt] = temp;
			draw_bitmap(backBuffer, screen_width, screen_height, y, x + add * cnt++, stuff);
			memcpy(fmap, backBuffer, screen_width * screen_height * 4);
		}
		usleep(500000);
	}
}
