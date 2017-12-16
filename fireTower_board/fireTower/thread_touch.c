#include "touch.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <string.h>
#include <linux/input.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

extern int 				screen_width, screen_height, pthread_kill;
extern pthread_mutex_t	pthread_kill_mutex;
extern unsigned char		sendToArd;
extern pthread_mutex_t	sendToArdMut;
extern int 				situation;
extern pthread_mutex_t	situationMut;

void* thread_touch(void* args)
{
	int 	*touchFP;
	int 	touchX, touchY;

	touchFP = open(EVENT_STR, O_RDONLY);					// Open file for touch LCD control
	if (-1 == touchFP)											// If open error occurs then prints error message.
	{
		printf("%s open fail\n",EVENT_STR);
		pthread_exit(NULL);
	}
	while(1)
	{
		readFirstCoordinate(touchFP, &touchX , &touchY, screen_width, screen_height);
		printf("%d, %d\n", touchX, touchY);

		if(touchY >= 0 && touchY < 100 && touchX >= 0 && touchX < 100)
		{
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 130;											// left
			pthread_mutex_unlock(&sendToArdMut);
		}
		if(touchY >= 100 && touchY < 200 && touchX >= 0 && touchX < 100)
		{
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 136;											// down
			pthread_mutex_unlock(&sendToArdMut);
		}
		if(touchY >= 200 && touchY < 300 && touchX >= 0 && touchX < 100)
		{
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 132;											// right
			pthread_mutex_unlock(&sendToArdMut);
		}
		if(touchY >= 100 && touchY < 200 && touchX >= 100 && touchX < 200)
		{
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 144;											// up
			pthread_mutex_unlock(&sendToArdMut);
		}
		if(touchY >= 600 && touchY < 800 && touchX >= 0 && touchX < 100)
		{
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 129;
			pthread_mutex_unlock(&sendToArdMut);
		}
		if(touchY >= 600 && touchY < 800 && touchX >= 1180 && touchX < 1280)
		{
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 0;
			pthread_mutex_unlock(&sendToArdMut);
			pthread_mutex_lock(&situationMut);
			situation = 0;
			pthread_mutex_unlock(&situationMut);
			usleep(100000);
		}

		if(touchY >= 0 && touchY <= 200 && touchX > 1180 && touchX < 1280)
		{
			pthread_mutex_lock(&pthread_kill_mutex);
			pthread_kill = 0;
			pthread_mutex_unlock(&pthread_kill_mutex);
			pthread_mutex_lock(&sendToArdMut);
			sendToArd = 0;
			pthread_mutex_unlock(&sendToArdMut);
			pthread_mutex_lock(&situationMut);
			situation = 0;
			pthread_mutex_unlock(&situationMut);
			usleep(100000);
			close(touchFP);
			return;
		}
	}
}
