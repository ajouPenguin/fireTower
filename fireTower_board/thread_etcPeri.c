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
#include <memory.h>
#include <pthread.h>

#include "oled.h"
#include "tlcd.h"
#include "buzzer.h"
#include "fnd.h"
#include "cled.h"

extern int 		oledFD, tlcdFD, buzzFD, fndFD, cledFD;
extern int 				situation;
extern pthread_mutex_t 	situationMut;

void* thread_buzzer(void *args)
{
	buzzer(buzzFD);
	pthread_detach(pthread_self());
}

void* thread_cled(void *args)
{
	cled(cledFD);
	pthread_detach(pthread_self());
}

void* thread_fnd(void *args)
{
	int i = 0;
	while(1)
	{
		fnd(fndFD, i++);
		pthread_mutex_lock(&situationMut);
		if(!situation)
		{
			pthread_mutex_unlock(&situationMut);
			pthread_detach(pthread_self());
			return;
		}
		pthread_mutex_unlock(&situationMut);
	}
}

void* thread_etcPeri(void *args)
{
	int flag = 0;
	pthread_t thread[3];

	while(1)
	{
		usleep(50000);

		pthread_mutex_lock(&situationMut);
		if(!flag && situation)
		{
			flag = 1;
			pthread_create(&thread[0], NULL, thread_buzzer, NULL);
			pthread_create(&thread[1], NULL, thread_cled, NULL);
			pthread_create(&thread[2], NULL, thread_fnd, NULL);
			oled(oledFD, "fire.img");
			tlcd(tlcdFD, "Fire!");
		}

		else if(flag && !situation)
		{
			flag = 0;
			oled(oledFD, "mountain.img");
			tlcd(tlcdFD, "Guarding!");
		}
		pthread_mutex_unlock(&situationMut);
	}
}
