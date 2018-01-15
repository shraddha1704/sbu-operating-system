#include<custom.h>

void clearscreen() {
	 __asm__("int $0x80"
         :: "a" (14));
//	puts("Atleast came here");
	//return pid;
}
