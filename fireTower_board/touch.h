#ifndef __TOUCH_H__
#define __TOUCH_H__

#define	 FBDEV_FILE "/dev/fb0"
#define  INPUT_DEVICE_LIST	"/proc/bus/input/devices"
#define  EVENT_STR	"/dev/input/event2"

void readFirstCoordinate(int fd, int* cx , int* cy, int screen_width, int screen_height);
void initScreen(unsigned char *fb_mem, int screen_width, int screen_height);

#endif
