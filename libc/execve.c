#include<custom.h>

int ret;
//char file_g[1000];
//char argv_g[20][1000];
//char env_g[1][1000];

int execve(char *filename, char *argv[], char *envp[]){
	/*
	int i = 0;
	
	//char str[20];
	i = 0;
	while(*filename != '\0')
	{
		file_g[i++] = *filename++;
	}
	file_g[i] = '\0';

	i=0;
	while(*(argv+i))
	{
	 strcpy(argv_g[i],*(argv+i));
	 i++;
	}

	i=0;
	while(*(env_g+i))
        {
         strcpy(env_g[i],*(env+i));
	 i++;

        }

	//strcpy(*env_g, NULL);
	/env_g[0] = '\0';
	*/
	/*__asm__ volatile("syscall"
        : "=a" (ret)
        : "a" (59), "D" (filename), "S" (argv), "b" (envp)
        : "memory","cc");*/

        __asm__ volatile("int $0x80"
        : "=a" (ret)
        : "a" (59), "b" (filename), "c" (argv), "d" (envp)
        : "memory","cc");	

	return ret;
}
