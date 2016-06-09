/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/07/16 14:35:23 HKT
*/

//single thread, echo server
//use telnet as client
//use context mode

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h> /* TCP_NODELAY */
#include <netinet/in.h>  /* inet_ntoa */
#include <arpa/inet.h>   /* inet_ntoa */

#include "rlsvr.h"
#include "rlcon.h"

void set_nonblock (int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	assert(0 <= r && "Setting socket non-block failed!");
}



int main(int argc, char **argv)
{
	RLServer server;
	server.start();
	printf("server exit\n");
	return 0;
}

