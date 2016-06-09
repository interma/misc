/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 06/09/16 21:16:16 HKT
*/

#include <stdio.h>
#include <atomic>

std::atomic<int> id;

int main(int argc, char **argv)
{
	id++;
	//printf("i am ok[%d]\n", id);
	printf("i am ok\n");
	return 0;
}

