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

/* if this function is called then the buzzer will make siren 5 times */

int buzzer(int fd)
{
	int buzzerNumber = 20;
	int i;

	for(i = 0; i < 10; i++)
	{
		write(fd,&buzzerNumber,4);
		usleep(500000);																	// The buzzer sounds for 0.5 seconds.
		buzzerNumber = (buzzerNumber==15) ?20 :15;			// buzzerNumber points the 'si' then change the 're', else 're' to 'si'
	}
	buzzerNumber = 0;
	write(fd,&buzzerNumber,4);												// Turn off the siren.

	return 0;
}
