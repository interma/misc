#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <ev.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	printf("server exit\n");
	return 0;
}
