#include<custom.h>
#include <sys/printf.h>
//int ret;
//char patharray[1000];

int open(char *path, int flag) {
	/*for(int i = 0 ; i<1000; i++) {
		patharray[i] = '\0';
	}*/
	char patharray[100] = {0};
	int ret;
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
	//printf("\nIn libc/open return value %d \n", ret);
	return ret;

}
