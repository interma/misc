/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 05/11/2021 17:17:33 CST
*/

/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 05/11/2021 17:17:33 CST
*/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
 #include <string.h>
 #include <errno.h>
#include<sys/types.h>
#include<sys/stat.h>
int main()
{
	int f;
	f = open("test.txt",O_RDWR);
	char c[128];
	scanf("%s", c);
	int ret = lseek(f,5,SEEK_SET);//skips 5 characters from the current position
	printf("result: %d %s", ret, strerror(errno));
	return 0;
}
