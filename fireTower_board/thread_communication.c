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
#include <termios.h>
#include <memory.h>

#include "comm_with_arduino.h"
#include "comm_with_server.h"
#include "mled.h"
#include "led.h"
#include "dipsw.h"

#define BAUDRATE          B115200
#define MLED_DRIVER_NAME	"/dev/cnmled"
#define MODEMDEVICE 			"/dev/ttyACM0"

extern int                ledFD, mledFD, acmFD, dipswFD;
extern unsigned char      sendToArd;
extern pthread_mutex_t    sendToArdMut;

extern int 			        	situation;
extern pthread_mutex_t 	  situationMut;

extern struct             termios newtio;

struct fireData{
  int fire;
  int CO;
  int water;
  char mode;
}data;

int mledFire = 0, mledCO = 0;

/* For matrix LED threading */
void* thread_mled(void* args)
{
  while(1)
    mled(mledFD, mledFire, mledCO);
}

/* Communicate with arduino and server
   If fire value is bigger than 200 or CO value is bigger than 300, then this thread make situation.
   Receive data also has remaining water value. This thread use this value to manipulate LEDs.
   This thread also sends data to arduino.
   Data which is sended to arduino has tower control value.
   Data comes from touch thread.
   This thread can prevent to control the tower.
   If first switch is up, then prevent to control tower's head that moves to left or right.
   If second switch is up, then prevent to control tower's head that moves to up or down.
*/
void* thread_communication(void* args)
{
  char dipVal = 0;
  int i, flag = 0;
  pthread_t thread;

  memset(&data, 0, sizeof(data));
  pthread_create(&thread, NULL, thread_mled, NULL);                   // create Metrix LED thread.

 while(acmFD < 0)                                                     // Until connect to arduino
  {
    acmFD = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    tcflush(acmFD, TCIFLUSH);
    tcsetattr(acmFD, TCSANOW, &newtio);
  }
  while(1)
  {
    recieveFromArduino(acmFD, &data);

//    printf("fire : %d\nCO : %d\nwater : %d\n",data.fire, data.CO, data.water);    // for debugging

    if(data.fire > 200 || data.CO > 300)                                            // If fire occurs
    {
      pthread_mutex_lock(&situationMut);
      situation = 1;
      pthread_mutex_unlock(&situationMut);
    }
    pthread_mutex_lock(&situationMut);
    if(flag != situation)                                                           // Send remaining water data to server when fire occurs and fire is over
    {
      flag = situation;
      sendToServer(data.water);
    }
    pthread_mutex_unlock(&situationMut);

    /* Divide the values in eight or nine levels for display in the matrix LED or LED */

    mledFire = (data.fire <= 0) ? 0 : (data.fire >= 950) ? 7 : data.fire / 119 + 1;
    mledCO = (data.CO <= 0) ? 0 : (data.CO >= 750) ? 7 : data.CO / 80 + 1;
    data.water = (data.water <= 0) ? 0 : (data.water >= 600) ? 8 : data.water / 70 + 1;

    if(data.water == 0)
    {
      led(ledFD, 0, 0);
    }
    else if(data.water == 8)
    {
      led(ledFD, 0, 1);
    }
    else
    {
      led(ledFD, 0, 0);
      for(i = 1; i <= data.water; i++)
      {
        led(ledFD, i, 1);
      }
    }

    /*
       If first switch is up, case 1
          second switch is up, case 2
          Both switch is up, case 3
          Both switch is down, case 0
    */

    pthread_mutex_lock(&sendToArdMut);
    dipVal = dipsw(dipswFD);
    switch(dipVal)
    {
      case 0 :
      sendToArd &= 0x9F;
      break;
      case 1 :
      sendToArd |= 0x40;
      break;
      case 2 :
      sendToArd |= 0x20;
      break;
      case 3 :
      sendToArd |= 0x60;
      break;
    }

    sendToArduino(acmFD, &sendToArd);
    sendToArd &= 0x80;                      // Initailize to passive mode value.

    pthread_mutex_unlock(&sendToArdMut);
  }
}
