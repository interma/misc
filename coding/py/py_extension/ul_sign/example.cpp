#include <stdio.h>
#include <string.h>
#include "ul_sign.h"

int main(int argc, char *argv[]) {
	if (argc > 1) {
		unsigned int sign1 = 0;
		unsigned int sign2 = 0;

		creat_sign_fs64(argv[1], (int)strlen(argv[1]), &sign1 , &sign2);

		printf("%s:[%u][%u]\n",argv[1],sign1,sign2);
	}
	return 0;
}
