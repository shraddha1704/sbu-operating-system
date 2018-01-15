#include "custom.h"
#include "unistd.h"
#include "sys/printf.h"
#include "stdlib.h"
#define INPUT_SIZE 1000

/*
int strcmp(char *str1, char *str2)
{
        while(*str1==*str2)
        {
                if(*str1=='\0'||*str2=='\0')
                        break;

                str1++;
                str2++;

        }

        if(*str1=='\0' && *str2=='\0')
                return 0;

        else
                return -1;

        return -2;

}

void tokenize_input(char* input, char **tokens,char separator){
        int token_count = 0;
        while(*input!='\0'){

                //Token saves the address of the first letter of each word.
                *tokens++ = input;
                token_count++;
                while(*input!=separator&& *input!='\0')
                {

                        input++;
                }

                //Once End of String is reached

                if (*input=='\0')
                        continue;

                *input = '\0';
                input++;
        }

        *tokens = '\0';
        //return token_count;
}

*/
int change_directory(char *input){
	int i = 0;
	puts(input);
        char dir[INPUT_SIZE] = {'\0'};

        if ((i = chdir(input)) == 0) {
                if (getcwd(dir,sizeof(dir)) == NULL) {
                        puts("getcwd error");
			return -1;
                 }else {
                        puts("Current directory: ");
                        puts(dir);
                        return 0;
                }

        } else {
                puts("sbush: No such file or directory");
                return -1;
        }

}

int main(int argc, char *argv[], char*envp[])
{/*
	char input[100];
	gets(input);
	char inputcopy[100];	
        char *tokens[INPUT_SIZE];

        if(gets(input)!=NULL && input[0] != '\0'){
                strcpy(inputcopy,input);
                tokenize_input(input,tokens,' ');

                if(!strcmp(tokens[0],"cd"))
                	change_directory(tokens[1]);

	}
	puts(input);
*/

//	puts("HELLO!!! BEFORE FORK");
	//int ret = fork();
	//printf("%d",ret);
//	puts("AFTER FORK -> HELLO");

	//int status;
	int pid;
	int status;
	int *a = malloc(sizeof(int));
	*a = 4;
	printf("%d",*a);	
	pid = fork();
	puts("reached n number of times");
	if(pid==0) {
	puts("reached child and going in while loop");
	while(1);
	}
	puts("called parent now");
	waitpid(pid,&status);
	puts("after while");	

//	int k = 0;
/*        char argv1[10] = "bin/ls";
	char *tokens[10] = {'\0'};
	char *envp1[10] = {'\0'};
//	execvpe(argv1,tokens, envp1);

        if ((pid = fork()) < 0 )
        {
                puts("error forking child");
                //exit(1);

         } else if (pid == 0) {

		puts("fork reached child");
		k = k+1;
		execvpe(argv1,tokens,envp1);
		puts("after k modified");
//		printf("value of k at child is %d/n", k);		
//          //      strcat1(argv, tokens[0]);
               // if (execvpe("bin/ls",tokens, envp1) < 0) {
                         // exit(1);
               // }
        } else {
                puts("fork reached parent");
		k = k+10;
		execvpe(argv1,tokens,envp1);
                //printf("value of k at parent is %d/n", k);
		puts("after k modified in parent");
		//puts("done with execve");
		//waitpid(-1, &status);
        }
*/
	//puts("in child;");
	//printf("Value returned from fork: %d",a);
	//if(ret==2)
//	open("bin/ls",0);

	//else
//	open("bin/cat",0);

	while(1);


	//open("bin/ls",0);
	//char input[100];
	//gets(input);
	//puts("Hello. Welcome to Sbush");
//	open("bin/test", 0);
//	open("bin/ls", 0);
	//char fileData[1024];
	//read(4,fileData,1000);
	//change_directory("test/");
	//while(1);
}
