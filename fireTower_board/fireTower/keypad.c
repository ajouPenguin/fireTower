

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

/* This function returns number of pushed key */

int Keypad(int fd)
{
	int rdata ;

	read(fd,&rdata,4);
	return rdata;
}
