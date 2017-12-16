

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define TRUE			1
#define FALSE		0

#define SUCCESS		0
#define FAIL			1

/******************************************************************************
*
*      TEXT LCD FUNCTION
*
******************************************************************************/
#define CLEAR_DISPLAY		0x0001
#define CURSOR_AT_HOME		0x0002

// Entry Mode set 
#define MODE_SET_DEF			0x0004
#define MODE_SET_DIR_RIGHT	0x0002
#define MODE_SET_SHIFT		0x0001

// Display on off
#define DIS_DEF				0x0008
#define DIS_LCD				0x0004
#define DIS_CURSOR			0x0002
#define DIS_CUR_BLINK		0x0001

// shift
#define CUR_DIS_DEF			0x0010
#define CUR_DIS_SHIFT		0x0008
#define CUR_DIS_DIR			0x0004

// set DDRAM  address 
#define SET_DDRAM_ADD_DEF	0x0080

// read bit
#define BUSY_BIT				0x0080
#define DDRAM_ADD_MASK		0x007F


#define DDRAM_ADDR_LINE_1	0x0000
#define DDRAM_ADDR_LINE_2	0x0040


#define SIG_BIT_E			0x0400
#define SIG_BIT_RW			0x0200
#define SIG_BIT_RS			0x0100

/***************************************************
read /write  sequence
write cycle 
RS,(R/W) => E (rise) => Data => E (fall) 

***************************************************/
int IsBusy(int fd)
{
	unsigned short wdata, rdata;

	wdata = SIG_BIT_RW;
	write(fd ,&wdata,2);

	wdata = SIG_BIT_RW | SIG_BIT_E;
	write(fd ,&wdata,2);

	read(fd,&rdata ,2);

	wdata = SIG_BIT_RW;
	write(fd,&wdata,2);

	if (rdata &  BUSY_BIT)
		return TRUE;

	return FALSE;
}
int writeCmd(unsigned short cmd, int fd)
{
	unsigned short wdata ;

	if ( IsBusy(fd))
		return FALSE;

	wdata = cmd;
	write(fd ,&wdata,2);

	wdata = cmd | SIG_BIT_E;
	write(fd ,&wdata,2);

	wdata = cmd ;
	write(fd ,&wdata,2);

	return TRUE;
}

int setDDRAMAddr(int x , int y, int fd)
{
	unsigned short cmd = 0;
	printf("x :%d , y:%d \n",x,y);
	if(IsBusy(fd))
	{
		perror("setDDRAMAddr busy error.\n");
		return FALSE;

	}

	if ( y == 1 )
	{
		cmd = DDRAM_ADDR_LINE_1 +x;
	}
	else if(y == 2 )
	{
		cmd = DDRAM_ADDR_LINE_2 +x;
	}
	else
		return FALSE;

	if ( cmd >= 0x80)
		return FALSE;

	
//	printf("setDDRAMAddr w1 :0x%X\n",cmd);

	if (!writeCmd(cmd | SET_DDRAM_ADD_DEF, fd))
	{
		perror("setDDRAMAddr error\n");
		return FALSE;
	}
//	printf("setDDRAMAddr w :0x%X\n",cmd|SET_DDRAM_ADD_DEF);
	usleep(1000);
	return TRUE;
}


int writeCh(unsigned short ch, int fd)
{
	unsigned short wdata =0;

	if ( IsBusy(fd))
		return FALSE;

	wdata = SIG_BIT_RS | ch;
	write(fd ,&wdata,2);

	wdata = SIG_BIT_RS | ch | SIG_BIT_E;
	write(fd ,&wdata,2);

	wdata = SIG_BIT_RS | ch;
	write(fd ,&wdata,2);
	usleep(1000);
	return TRUE;

}

int functionSet(int fd)
{
	unsigned short cmd = 0x0038; // 5*8 dot charater , 8bit interface , 2 line

	if (!writeCmd(cmd, fd))
		return FALSE;
	return TRUE;
}

int writeStr(char* str, int fd)
{
	unsigned char wdata;
	int i;
	for(i =0; i < strlen(str) ;i++ )
	{
		if (str[i] == '_')
			wdata = (unsigned char)' ';
		else
			wdata = str[i];
		writeCh(wdata, fd);
	}
	return TRUE;

}

#define LINE_NUM			2
#define COLUMN_NUM			16			
int clearScreen(int nline, int fd)
{
	int i;
	if (nline == 0)
	{
		if(IsBusy(fd))
		{	
			perror("clearScreen error\n");
			return FALSE;
		}
		if (!writeCmd(CLEAR_DISPLAY, fd))
			return FALSE;
		return TRUE;
	}
	else if (nline == 1)
	{	
		setDDRAMAddr(0,1, fd);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			writeCh((unsigned char)' ', fd);
		}	
		setDDRAMAddr(0,1, fd);

	}
	else if (nline == 2)
	{	
		setDDRAMAddr(0,2, fd);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			writeCh((unsigned char)' ', fd);
		}	
		setDDRAMAddr(0,2, fd);
	}
	return TRUE;
}

int TdisplayMode(int bCursor, int bCursorblink, int blcd, int fd)
{
	unsigned short cmd  = 0;

	if ( bCursor)
	{
		cmd = DIS_CURSOR;
	}

	if (bCursorblink )
	{
		cmd |= DIS_CUR_BLINK;
	}

	if ( blcd )
	{
		cmd |= DIS_LCD;
	}

	if (!writeCmd(cmd | DIS_DEF, fd))
		return FALSE;

	return TRUE;
}

#define CMD_TXT_WRITE		0
#define CMD_CURSOR_POS		1
#define CMD_CEAR_SCREEN		2



int tlcd(int fd, char *string)
{

	int nCmdMode;
	int bCursorOn = 1, bBlink = 1, nline = 2, nColumn = 12;
	unsigned short cmd = 1;
	
	functionSet(fd);

	clearScreen(1, fd);
	TdisplayMode(bCursorOn, bBlink, TRUE, fd);
	setDDRAMAddr(nline-1, nColumn, fd);
	TdisplayMode(bCursorOn, bBlink, TRUE, fd);
	setDDRAMAddr(nline-1, nColumn, fd);
	usleep(2000);
	writeStr(string, fd);

	return 0;
}
