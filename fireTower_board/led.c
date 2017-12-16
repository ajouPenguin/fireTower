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



#define LED_ON		1
#define LED_OFF		0
#define MAX_LED_NO	8

/*
	ledNo points ith led.
	ledCtr controls the led's states. 1 = turn on, 0 = turn off.
	If ledNo equals 0 and ledCtr equals 1 then all leds are turned on.
	And ledNo equals 0 and ledCtr equals 0 then all leds are turned off.
*/

int led(int fd, int ledNo, int ledCtr)
{
	int wdata, rdata, temp;

	if ( ledNo == 0 )
	{
		if ( ledCtr ==  1 ) wdata = 0xff;
		else wdata = 0;
	}
	else
	{
		read(fd,&rdata,4);
		temp = 1;

		if ( ledCtr == 1 )			// Turn on the 'ledNo'th LED
		{
			temp <<=(ledNo - 1);
			wdata = rdata | temp;
		}
		else										// Turn off the 'ledNo'th LED
		{
			temp =  ~(temp<<(ledNo-1));
			wdata = rdata & temp;
		}
	}

	write(fd,&wdata,4);

	return 0;
}
