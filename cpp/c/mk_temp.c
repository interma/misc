/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 04/06/17 13:42:07 CST
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void foo1()
{
	char template[] = "/tmp/my_folder/fileXXXXXX"; //my_folder need created manually
	char content[] = "xxx\nyyy";
	int fd = mkstemp(template);
	write(fd, content, strlen(content));
	close(fd);
}


void foo2()
{
	char template[] = "/tmp/my_folder/fileXXXXXX"; //my_folder need created manually
	char content[] = "xxx\nyyy";
	const char *file_name = mktemp(template);
	int fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY);
	write(fd, content, strlen(content));
	close(fd);
	
}

int main() 
{
	foo2();
}

