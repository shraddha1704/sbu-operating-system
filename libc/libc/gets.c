#include<custom.h>

long a;
char in[1000];

char *gets(char *input){
	int i = 0;
	int len = 0;
	
	//__asm volatile("sti");
	__asm__ volatile ("int $0x80"
	:"=a" (a)
	: "a" (3), "b" (0), "c" (in), "d" (sizeof(in))
        : "memory");
	
	i = 0;
	while(in[i] != '\0') {
		len++;
                input[i] = in[i];
		i++;
        }
	
	in[i] = '\0';
	
	for(i=0;i<len;i++)
	{
		in[i] = '\0';	

	}

        input[i] = '\0';
	//__asm volatile("cli");

	return input;
}
