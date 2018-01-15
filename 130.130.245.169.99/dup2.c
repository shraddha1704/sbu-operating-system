#include<custom.h>

int ret;
int ofd;
int nfd;

int dup2(int oldfd, int newfd) {
	ofd = oldfd;
	nfd = newfd;	
	__asm__ ("int $0x80"
         : "=a" (ret)
         :"a" (63), "b" (ofd), "c" (nfd)
         : "memory");
	
	return ret;
}
