#include<custom.h>
#include<sys/printf.h>

int chdir(char *path) {
	int i = 0;
	char in[100] = {'\0'};
	int returnvalue;
	while(*path) {
		in[i++] = *path++;
	}
	in[i]='\0';
	//puts(in);
	 __asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (12), "b" (in): "memory");
	//printf("In chdir return value !!!!! %d\n", returnvalue);
	returnvalue = returnvalue != -1 ? 0 : -1;
	return returnvalue;
}
