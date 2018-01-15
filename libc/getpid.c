#include<custom.h>
#include<sys/printf.h>
#include <unistd.h>
#include <sys/defs.h>

pid_t getpid(void) {
	pid_t returnvalue;
	 
	__asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (20): "memory");
	
	return returnvalue;
}
