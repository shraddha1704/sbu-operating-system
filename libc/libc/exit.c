#include<custom.h>

void exit(int status) {

	__asm__ ("int $0x80"
         :: "a" (1), "b" (status)
         : "memory");
}
