/*
 desc:
 author: interma (interma@outlook.com)
 created: 10/21/2020 15:12:48 CST
*/

/* sscanf example */
#include <stdio.h>

int main ()
{
	char host[1024];
	int port;
	char sentence []="hongxu-partition-test-jp:18983_solr";
	int ret	= sscanf (sentence,"%s:%d_solr",host,&port);
	if (ret > 0) {
		printf ("%s\n",host);
		printf ("%d\n",port);
	}

	return 0;
}

