#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#define MSG_SIZE 1000																		// maximum length of message

struct sockaddr_in server_addr;

int sendToServer(int water)
{
	int cli_socket = socket(PF_INET, SOCK_STREAM, 0);			// socket setting
	char msg[MSG_SIZE];																		// message for server
	time_t timer;																					// timer structure to time function
	struct tm *start_time;																// time structure to know the current time
	int waterLeng = 0, tmp = water;												// waterLeng is length of parameter data

	if(cli_socket == -1)																	// if socket setting fails then prints error message and return -1.
	{
		printf("creates fail the server socket.\n");
		return -1;
	}

	while(tmp != 0)																				// find length of water
	{
		waterLeng++;
		tmp /= 10;
	}

	timer = time(NULL);
	start_time = localtime(&timer);												// current time

/* message setting for sending.
 	 POST /POST HTTP/1.0
	 Content-Type:application/x-www-form-urlencoded
	 Content-Length:(the number of waterLeng)+31

	 date=(year)-(month)-(date) (hour):(minute):(second)&water=(water)0
	 last number(zero) is ignored.
	 */

	sprintf(msg, "%s%d%s%4d-%02d-%02d %02d:%02d:%02d&%s%d0", "POST /POST HTTP/1.0\r\nContent-Type:application/x-www-form-urlencoded\r\nContent-Length:",waterLeng + 31,"\r\n\r\ndate=",
		start_time->tm_year + 1900, start_time->tm_mon + 1, start_time->tm_mday,
		start_time->tm_hour, start_time->tm_min, start_time->tm_sec,
		"water=", water);

//	printf("%s\n", msg);																		// print for debugging

	memset(&server_addr, 0, sizeof(server_addr));							// initialize the memory of server_addr variable.
	server_addr.sin_family = AF_INET;													// IPv4 internet protocol
	server_addr.sin_port = htons(9000);												// port setting to 9000
	server_addr.sin_addr.s_addr = inet_addr("52.78.53.34");		// server address setting to 52.78.53.34

	if(connect(cli_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)	// connect to server, if fail to connect, then print error messages and return -1
	{
		printf("fail to connect!\n");
		return -1;
	}

	write(cli_socket, msg, strlen(msg + 1));																						// send msg including NULL

	close(cli_socket);																																	// close connecting.

	return 0;
}
