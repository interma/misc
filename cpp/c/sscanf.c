/*
 desc:
 author: interma (interma@outlook.com)
 created: 10/21/2020 15:12:48 CST
*/

/* sscanf example */
#include <stdio.h>

int main ()
{
	char sentence []="shard123";
	char sentence1 []="shardxxx";
	int i;

	int ret;
	ret	= sscanf (sentence,"shard%d",&i);
	if (ret > 0)
		printf ("%d\n",i);
	ret	= sscanf (sentence1,"shard%d",&i);
	if (ret > 0)
		printf ("%d\n",i);

	return 0;
}

