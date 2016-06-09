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

#include "rlsvr.h"
#include "rlcon.h"

int main(int argc, char **argv)
{
	RLServer server;
	server.start();
	printf("server exit\n");
	return 0;
}

