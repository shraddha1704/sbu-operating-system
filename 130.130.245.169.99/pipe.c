#include<custom.h>

int ret;
int fdg[2];

int pipe(int *fd){
	
	fdg[0] = fd[0];
	fdg[1] = fd[1];

	__asm__("int $0x80"
        :"=a" (ret) : "a" (42), "b" (fdg) : "memory");
	return ret;

}
