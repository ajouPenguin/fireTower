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
#include <pthread.h>		// for threading

#include "dipsw.h"
#include "led.h"
#include "buzzer.h"
#include "mled.h"
#include "cled.h"
#include "keypad.h"
#include "oled.h"
#include "tlcd.h"
#include "fnd.h"
#include "display.h"
#include "comm_with_arduino.h"
#include "login.h"
#include "comm_with_server.h"
#include "three-thread.h"

#define BAUDRATE 					B115200		// baudrate to communicate with arduino

/* path of device driver */

#define MODEMDEVICE 			"/dev/ttyACM0"
#define DIPSW_DRIVER_NAME	"/dev/cndipsw"
#define LED_DRIVER_NAME		"/dev/cnled"
#define BUZZ_DRIVER_NAME	"/dev/cnbuzzer"
#define MLED_DRIVER_NAME	"/dev/cnmled"
#define CLED_DRIVER_NAME	"/dev/cncled"
#define TLCD_DRIVER_NAME	"/dev/cntlcd"
#define OLED_DRIVER_NAME	"/dev/cnoled"
#define KEY_DRVIER_NAME		"/dev/cnkey"
#define FND_DRIVER_NAME		"/dev/cnfnd"
#define FBDEV_FILE 				"/dev/fb0"


int 				dipswFD, ledFD, buzzFD, mledFD, oledFD, tlcdFD, keyFD, cledFD, fndFD, acmFD;			// device file pointers
int 				screen_width, screen_height, bits_per_pixel, line_length;													// informations of touch screen
int 				fb_fd;																																						// touch screen device file pointer

unsigned char 		sendToArd;																																	// information which is sended to arduino
pthread_mutex_t		sendToArdMut = PTHREAD_MUTEX_INITIALIZER;																		// mutex structure for sendToArd variable

int 							situation;																																	// when sensors detect fire situation = 1, else 0
pthread_mutex_t 	situationMut = PTHREAD_MUTEX_INITIALIZER;																		// mutext structure for situation variable

struct termios 	newtio;																																				// new communication setting for arduino

int main(int argc, char *argv[])
{
	/* thread[0] is touch screen thread, thread[1] is communication thread, thread[2] is peripheral thread */
	pthread_t		thread[3];
	struct  		fb_var_screeninfo fbvar;	// ?
	struct  		fb_fix_screeninfo fbfix;	// ?

	/*
		Open each components' drivers and error handling code.
		When any driver causes open error, prints error description and exits program.
	*/
	acmFD = open(MODEMDEVICE, O_RDWR | O_NOCTTY);									// open file for arduino control
	if (acmFD < 0) {perror(MODEMDEVICE);};												// open error
	bzero(&newtio, sizeof(newtio));																// initialize the newtio
	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	tcflush(acmFD, TCIFLUSH);																			// device driver file flushing
	tcsetattr(acmFD, TCSANOW, &newtio);

	dipswFD = open(DIPSW_DRIVER_NAME,O_RDWR);					// Open file for dip switch control
	if ( dipswFD < 0 )																// open error
	{
		perror("dip switch driver open error.\n");
	}

	ledFD = open(LED_DRIVER_NAME, O_RDWR);						// Open file for led control
	if( ledFD < 0 )																		// open error
	{
		perror("LED driver open error.\n");
	}

	buzzFD = open(BUZZ_DRIVER_NAME, O_RDWR);					// Open file for buzzer control
	if( buzzFD < 0 )																	// open error
	{
		printf("flag\n");
		perror("Buzzer driver open error.\n");
	}

	mledFD = open(MLED_DRIVER_NAME,O_RDWR);						// Open file for matrix led control
	if ( mledFD < 0 )																	// open error
	{
		perror("Matrix led driver open error.\n");
	}

	cledFD = open(CLED_DRIVER_NAME, O_RDWR);					// Open file for full color led control
	if(cledFD < 0)																		// open error
	{
		perror("Full color led driver open error.\n");
	}

	oledFD = open(OLED_DRIVER_NAME,O_RDWR);						// Open file for matrix oled control.
	if ( oledFD < 0 )																	// open error
	{
		perror("oled driver open error.\n");
	}

	keyFD = open(KEY_DRVIER_NAME,O_RDWR);							// Open file for key button control.
	if ( keyFD < 0 )																	// open error
	{
		perror("key pad driver open error.\n");
	}

	tlcdFD = open(TLCD_DRIVER_NAME,O_RDWR);						// Open file for text lcd control.
	if ( tlcdFD < 0 )																	// open error
	{
		perror("TLCD driver open error.\n");
	}

	fndFD = open(FND_DRIVER_NAME,O_RDWR);							// Open file for 7-segment lcd
	if ( fndFD < 0 )																	// open error
	{
		perror("7-segment lcd driver open error.\n");
	}

	if( access(FBDEV_FILE, F_OK) )										// Checks FBDEV_FILE is exists and can be accessed.
	{
		printf("%s: access error\n", FBDEV_FILE);
	}

	if( (fb_fd = open(FBDEV_FILE, O_RDWR)) < 0)				// Open FBDEV_FILE(display device)
	{
		printf("%s: open error\n", FBDEV_FILE);
	}


	if( ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvar) )		// fbvar에 FBIOGET_VSCREENINFO 명령 수행 값을 저장
	{
		printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
		exit(1);
	}

	if( ioctl(fb_fd, FBIOGET_FSCREENINFO, &fbfix) )		// fbfix에 FBIOGET_FSSCREENINFO 명령 수행 값을 저장
	{
		printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
		exit(1);
	}

	screen_width    =   fbvar.xres;
	screen_height   =   fbvar.yres;
	bits_per_pixel  =   fbvar.bits_per_pixel;
	line_length     =   fbfix.line_length;

	oled(oledFD, "mountain.img");											// situation is not occured; oled, tlcd initialize
	tlcd(tlcdFD, "Guarding!");

	pthread_create(&thread[0], NULL, thread_touchLCD, NULL);				// create the touch lcd thread
	pthread_create(&thread[1], NULL, thread_communication, NULL);		// create the communication thread
	pthread_create(&thread[2], NULL, thread_etcPeri, NULL);					// create the peripheral thread

	/* Joins threads before program exiting */

	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	pthread_join(thread[2], NULL);

	/* close device driver files before program exiting */

		close(buzzFD);
		close(dipswFD);
		close(oledFD);
		close(tlcdFD);
		close(keyFD);
		close(cledFD);
		close(mledFD);
		close(ledFD);


		return 0;

}
