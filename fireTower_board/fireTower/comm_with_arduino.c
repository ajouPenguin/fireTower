#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>

/* Structure for data which comes from arduino */

struct fireData{
  int fire;
  int CO;
  int water;
  char mode;
};

/* sendToArduino function sends 1 byte data to arduino */

void sendToArduino(int fd, char *data)
{
//	printf("sended data : %x\n", *data);                                // print for debugging
	write(fd, data, 1);
}

/* recieveFromArduino get data from arduino */

void recieveFromArduino(int fd, struct fireData *data)
{
	unsigned char buf[30];
	int i;

	memset(buf, 0, 30);
	usleep(500000);                                                       // sleep for synchronization. (why synchronize again?)
	read(fd, buf, 30);                                                    // read from driver file
//	printf("buf : %s\n", buf);                                          // print for debugging too.
	sscanf(buf, "%d%d%d", &(data->fire), &(data->water), &(data->CO));    // buf -> data; parsing
}
