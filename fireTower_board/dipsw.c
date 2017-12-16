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

/* This function return the number of dip switch which state is up. */

int dipsw(int fd)
{
	int retvalue;

	read(fd,&retvalue,4);
	retvalue &= 0x3;				// We use 3 leftmost switch.

	return retvalue;
}
