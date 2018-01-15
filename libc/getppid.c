#include<custom.h>
#include<sys/printf.h>
#include <unistd.h>
#include <sys/defs.h>

pid_t getppid(void) {
	pid_t returnvalue;
	 
	__asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (64): "memory");
	
	return returnvalue;
}
