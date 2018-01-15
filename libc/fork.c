#include<custom.h>

int fork() {
	int pid;
	 __asm__("int $0x80"
         :"=a" (pid) : "a" (2));
//	puts("Atleast came here");
	return pid;
}
