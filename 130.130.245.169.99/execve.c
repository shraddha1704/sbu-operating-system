#include<custom.h>

int ret;
char file_g[1000];
char argv_g[20][1000];
char env_g[1][1000];

int execve(char *filename, char *argv[], char *env[]){
	
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

/*
	while(*env)
        {
         strcpy(*env_g,*env);
	 i++;

        }
*/
	strcpy(*env_g, NULL);
	*env_g[0] = '\0';
	
	__asm__ ("int $0x80"
        : "=a" (ret)
        : "a" (11), "b" (file_g), "c" (argv_g), "d" (env_g)
        : "memory");	

	return ret;
}
