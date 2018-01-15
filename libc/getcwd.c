#include<custom.h>
#include <sys/printf.h>
int a;
char buffer[100];
char *getcwd(char *buf, int size) {
	
	__asm__ ("int $0x80"
         : "=a" (a)
         : "a" (183), "b" (buffer), "c" (size)
         : "memory");

	int i = 0;
        while(buffer[i] != '\0') {
                *buf++ = buffer[i];
                i++;
        }
	//printf("In libc/getcwd buff@@@@@ %s\n",buf);
	return buf;

}
