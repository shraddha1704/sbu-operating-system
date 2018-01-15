#include<custom.h>

char in[1000];
int returnvalue;

int chdir(char *path) {
	//long add = (long) path;
	int i = 0;
	//while(*path != '\0') {
	//	in[i++] = *path++;
	//}

	while(*path)
	{
		in[i++] = *path++;
	}
	in[i]='\0';
	//puts(in);
	 __asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (12), "b" (in): "memory");
	return returnvalue;
}
