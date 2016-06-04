#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/tcp.h> /* TCP_NODELAY */
#include <netinet/in.h>  /* inet_ntoa */
#include <arpa/inet.h>   /* inet_ntoa */

#include <ev.h>

int main(int argc, char **argv)
{
	printf("server exit\n");
	return 0;
}
