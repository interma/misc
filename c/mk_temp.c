/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 04/06/17 13:42:07 CST
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() 
{
	char template[] = "/tmp/my_folder/fileXXXXXX"; //my_folder need created manually
	char content[] = "xxx\nyyy";
	int fd = mkstemp(template);
	write(fd, content, strlen(content));
	close(fd);
}

