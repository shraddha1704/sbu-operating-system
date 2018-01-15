#include<custom.h>
#include<sys/printf.h>
#include <unistd.h>
#include <sys/defs.h>

pid_t wait(int *status) {
	pid_t returnvalue;
	 
	__asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (114), "b" (status): "memory");
	
	return returnvalue;
}
