#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define INDEX_LED		0
#define INDEX_RED_LED		1
#define INDEX_GREEN_LED		2
#define INDEX_BLUE_LED		3
#define INDEX_MAX			4

/* If this function called then full color LED emits blue and red lights sequentially. */

int cled(int fd)
{
	unsigned short colorArray[INDEX_MAX];

	int i;

	/* Initialize the full color LED to blue light. */

	colorArray[INDEX_LED] = 0;
	colorArray[INDEX_GREEN_LED] = 0;
	colorArray[INDEX_RED_LED] = 0;
	colorArray[INDEX_BLUE_LED] = 255;

	for(i = 0; i < 10; i++)
	{
		colorArray[INDEX_RED_LED] = (colorArray[INDEX_RED_LED] == 255) ? 0 : 255;				// If previous color is red then number is 0, else number is 255
		colorArray[INDEX_BLUE_LED] = (colorArray[INDEX_BLUE_LED] == 255) ? 0 : 255;			// If previous color is blue then number is 0, else number is 255

		write(fd,&colorArray,6);
		usleep(500000);
	}

	return 0;
}
