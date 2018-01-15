#include<custom.h>
#include<sys/printf.h>
#include <unistd.h>

unsigned int sleep(unsigned int seconds) {
	int returnvalue;
	 
	__asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (162), "b" (seconds): "memory");
	
	returnvalue = returnvalue != -1 ? 0 : -1;
	return returnvalue;
}
