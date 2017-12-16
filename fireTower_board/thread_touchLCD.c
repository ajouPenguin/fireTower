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
#include <termios.h>
#include <pthread.h>

#include "keypad.h"
#include "display.h"
#include "login.h"
#include "touch.h"
#include "three-thread.h"


extern int keyFD, fb_fd;
extern int screen_width, screen_height, bits_per_pixel, line_length;

int pthread_kill;
pthread_mutex_t pthread_kill_mutex = PTHREAD_MUTEX_INITIALIZER;

unsigned char   	*fb_mapped;

void* thread_touchLCD(void *args)
{
	unsigned char 	*back_buffer;
	int 				mem_size = screen_height * screen_width * 4;
	BITMAPDATA 		change, left, right, up, down, shot, finish;
	pthread_t		thread;

	fb_mapped   =   (unsigned char *)mmap(0, mem_size,
		             PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
	back_buffer	=	(unsigned char *)malloc(mem_size);
	if (fb_mapped < 0)
	{
		printf("mmap error!\n");
		exit(1);
	}

	printf("mapping success!\n");
	printf("screen width : %d\nscreen height : %d\nbits per pixel : %d\nline_length : %d\n", screen_width, screen_height, bits_per_pixel, line_length);
	printf("%d\n",mem_size);

	initScreen(fb_mapped, screen_width, screen_height);
	while(1)
	{
		login(fb_mapped, back_buffer, screen_width, screen_height, keyFD);
		change = open_bitmap("change.bmp");
		left = open_bitmap("left.bmp");
		right = open_bitmap("right.bmp");
		up = open_bitmap("up.bmp");
		down = open_bitmap("down.bmp");
		shot = open_bitmap("shot.bmp");
		finish = open_bitmap("finish.bmp");

		CreateCamera(0);
		startPreview();

		pthread_mutex_lock(&pthread_kill_mutex);
		pthread_kill = 1;
		pthread_mutex_unlock(&pthread_kill_mutex);
		pthread_create(&thread, NULL, thread_touch, NULL);
		
		while(1)
		{
			pthread_mutex_lock(&pthread_kill_mutex);
			if(!pthread_kill)
			{
				pthread_mutex_unlock(&pthread_kill_mutex);
				break;
			}
			pthread_mutex_unlock(&pthread_kill_mutex);
			draw_camera(back_buffer, screen_width, screen_height);
			draw_bitmap(back_buffer, screen_width, screen_height, 0, 200, right);
			draw_bitmap(back_buffer, screen_width, screen_height, 0, 100, down);
			draw_bitmap(back_buffer, screen_width, screen_height, 0, 0, left);
			draw_bitmap(back_buffer, screen_width, screen_height, 100, 100, up);
			draw_bitmap(back_buffer, screen_width, screen_height, 0, 600, shot);
			draw_bitmap(back_buffer, screen_width, screen_height, 1180, 0, change);
			draw_bitmap(back_buffer, screen_width, screen_height, 1180, 600, finish);
			memcpy(fb_mapped, back_buffer, mem_size);
		}
		pthread_join(thread, NULL);
		close_bitmap(&right);
		close_bitmap(&left);
		close_bitmap(&up);
		close_bitmap(&down);
		close_bitmap(&shot);
		close_bitmap(&change);
		stopPreview();
		DestroyCamera();
	}
}
