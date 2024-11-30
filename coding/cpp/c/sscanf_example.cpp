/*
 desc: 	
 author: interma (interma@outlook.com)
 created: 07/06/2017 09:53:46 CST
*/

#include <stdio.h>
#include <string.h>
int main() {

	char query[] = "select * from orders where ts = 123";
	char query2[] = "select sum(price) from orders where ts > 123";
	
	char sfield[16];
	char wfield[16];
	char op[2];
	int value;

	int ret = sscanf(query, "select %s from orders where %s %s %d", 
		sfield, wfield, op, &value);
	if (ret > 0)
		printf("result1:[%s][%s][%s][%d]\n", sfield, wfield, op, value);

	ret = sscanf(query2, "select %s from orders where %s %s %d", 
		sfield, wfield, op, &value);
	if (ret > 0)
		printf("result2:[%s][%s][%s][%d]\n", sfield, wfield, op, value);

	return 0;
}
