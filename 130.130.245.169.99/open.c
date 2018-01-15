#include<custom.h>

int ret;
char patharray[1000];

int open(char *path, int flag) {


	int i=0;

	while(*path!='\0')
        {
                patharray[i++] = *path;
		path++;
        }

	__asm__ ("int $0x80"
	: "=a" (ret)
	: "a" (5), "b" (patharray), "c" (flag)
	: "memory");

	return ret;

}
