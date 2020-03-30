/*
 desc:
 author: interma (interma@outlook.com)
 created: 03/30/2020 16:15:31 CST
*/

#include <stdio.h>
#include <sys/time.h>

#define difftime(start,stop)   ((stop.tv_sec - start.tv_sec) * 1000 + (stop.tv_usec - start.tv_usec)/1000)

int main()
{
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	system("sleep 1");
	gettimeofday(&stop, NULL);
	printf("took %lu us\n", difftime(start,stop));
	return 0;
}

