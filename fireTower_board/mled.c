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
#include <termios.h>

/* Show level of fire sensor value and CO sensor value */

#define MAX_COLUMN_NUM	5
// 0 ~ 7 level
const unsigned short NumData[8][MAX_COLUMN_NUM]=
{
	{0xfe00,0xfd00,0xfb00,0xf700,0xef00}, // This row represent level 0
	{0xfe40,0xfd40,0xfb40,0xf740,0xef40}, // This row represent level 1
	{0xfe60,0xfd60,0xfb60,0xf760,0xef60}, // This row represent level 2
	{0xfe70,0xfd70,0xfb70,0xf770,0xef70}, // This row represent level 3
	{0xfe78,0xfd78,0xfb78,0xf778,0xef78}, // This row represent level 4
	{0xfe7c,0xfd7c,0xfb7c,0xf77c,0xef7c}, // This row represent level 5
	{0xfe7e,0xfd7e,0xfb7e,0xf77e,0xef7e}, // This row represent level 6
	{0xfe7f,0xfd7f,0xfb7f,0xf77f,0xef7f}  // This row represent level 7
};

static struct termios oldt, newt;
void changemode(int dir)
{
	if( dir == 1)													// new communication setting for matrix LED
	{
		tcgetattr(STDIN_FILENO , &oldt);
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO );
		tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	}
	else																	// reset to old setting
	{
		tcsetattr(STDIN_FILENO , TCSANOW, &oldt);

	}
}

#define ONE_LINE_TIME_U 	1000
// exit return => 0 , success return => 1
int displayDotLed(int driverfile , int fireLevel, int COLevel ,int timeS)
{
	int cSelCounter,loopCounter;
	int highChar , lowChar;
	int totalCount ;
	unsigned short wdata[2];


	highChar = fireLevel;
	lowChar = COLevel;


	totalCount = timeS*(1000000 / ONE_LINE_TIME_U);
	cSelCounter = 0;
	loopCounter = 0;
	while(1)
	{
		// high byte display
		wdata[0] = NumData[highChar][cSelCounter];

		// low byte display
		wdata[1] = NumData[lowChar][cSelCounter];

		write(driverfile,(unsigned char*)wdata,4);

		cSelCounter++;
		if ( cSelCounter >= (MAX_COLUMN_NUM))
			cSelCounter = 0;

		usleep(ONE_LINE_TIME_U);

		loopCounter++;
		if ( loopCounter > totalCount )
			break;
	}
	wdata[0]= 0;
	wdata[1]= 0;
	write(driverfile,(unsigned char*)wdata,4);

	return 1;
}

int mled(int fd, int fireLevel, int COLevel)		// range of level (0 <= Level <= 7)
{
	changemode(1);

	displayDotLed(fd , fireLevel, COLevel ,1);		// Show levels for 1 second.

	changemode(0);

	return 0;
}
