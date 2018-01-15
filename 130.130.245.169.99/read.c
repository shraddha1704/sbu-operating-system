#include<custom.h>

int ret;
char bufferarray[1000];

int read(int fd, char *buffer, int size) {
	
	int i=0;

	__asm__ ("int $0x80"
	: "=a" (ret)
	: "a" (3), "b" (fd), "c" (bufferarray), "d" (size)
	: "memory");
	

	for(i=0;bufferarray[i]!='\0'&& bufferarray[i]!=EOF;i++)
	{
		buffer[i]=bufferarray[i];
	}

	//To copy the terminating character ('\0' or EOF)
	buffer[i] = bufferarray[i];
	return ret;

}
