#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FND_NUM		6

#define  DOT_OR_DATA	0x80

/* This function activates 7 segments and represent 'num'. */

const unsigned short segNum[10] =
{
	0x3F, // The value of to represent 0
	0x06,	// The value of to represent 1
	0x5B,	// The value of to represent 2
	0x4F,	// The value of to represent 3
	0x66,	// The value of to represent 4
	0x6D,	// The value of to represent 5
	0x7D,	// The value of to represent 6
	0x27,	// The value of to represent 7
	0x7F,	// The value of to represent 8
	0x6F  // The value of to represent 9
};

const unsigned short segSelMask[MAX_FND_NUM] =
{
	0xFE00,
	0xFD00,
	0xFB00,
	0xF700,
	0xEF00,
	0xDF00
};

static struct termios oldt, newt;

void fnd_changemode(int dir)
{
	if( dir == 1)
	{
		tcgetattr(STDIN_FILENO , &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO );
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	}
	else
	{
		tcsetattr(STDIN_FILENO , TCSANOW, &oldt);

	}
}

#define ONE_SEG_DISPLAY_TIME_USEC	1000

int fndDisp(int driverfile, int num , int dotflag,int durationSec)
{
	int cSelCounter,loopCounter;
	int temp , totalCount, i ;
	unsigned short wdata;
	int dotEnable[MAX_FND_NUM];
	int fndChar[MAX_FND_NUM];

	for (i = 0; i < MAX_FND_NUM ; i++ )
	{
		dotEnable[i] = dotflag & (0x1 << i);
	}
	// if 6 fnd
	temp = num % 1000000;
	fndChar[0]= temp /100000;

	temp = num % 100000;
	fndChar[1]= temp /10000;

	temp = num % 10000;
	fndChar[2] = temp /1000;

	temp = num %1000;
	fndChar[3] = temp /100;

	temp = num %100;
	fndChar[4] = temp /10;

	fndChar[5] = num % 10;

	totalCount = durationSec*(1000000 / ONE_SEG_DISPLAY_TIME_USEC);
	cSelCounter = 0;
	loopCounter = 0;
	while(1)
	{
		wdata = segNum[fndChar[cSelCounter]]  | segSelMask[cSelCounter] ;
		if (dotEnable[cSelCounter])
			wdata |= DOT_OR_DATA;

		write(driverfile,&wdata,2);

		cSelCounter++;
		if ( cSelCounter >= MAX_FND_NUM )
			cSelCounter = 0;

		usleep(ONE_SEG_DISPLAY_TIME_USEC);

		loopCounter++;
		if ( loopCounter > totalCount )
			break;
	}

	wdata= 0;
	write(driverfile,&wdata,2);

	return 1;
}

int fnd(int fd, int num)
{
	int durationtime = 1;

	fnd_changemode(1);

	int hour = num / 3600;
	int min = (num % 3600) / 60;
	int sec = num % 60;

	fndDisp(fd, hour * 10000 + min * 100 + sec, 0, durationtime);

	fnd_changemode(0);

	return 0;
}
