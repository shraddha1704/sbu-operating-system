#include "custom.h"
#include "unistd.h"
#define INPUT_SIZE 100
#include "sys/printf.h"
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

char in[100];
int returnvalue;

int read_dir(char *path) {
        int i = 0;
        while(*path) {
                in[i++] = *path++;
        }
        in[i]='\0';
         __asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (200), "b" (in): "memory");
        return returnvalue;
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

	//TESTING STDIO FUNCTIONS:
	//char input[100];
	//gets(input);
	//puts("Hello. Welcome to Sbush");
	//printf("\n H \n",123,'x');
	//putchar('x');	

	/*open("bin/test", 0);
	open("bin/ls", 0);
	char fileData[100]={0};
	read(4,fileData,100);
	close(4);
	char dir[100]={0};
	getcwd(dir, 100);
	chdir("bin");*/
	read_dir("");
	//chdir("bin");
	//chdir("../");
	//chdir("./bin");
	//char *a = (char *)malloc(5000);
	//*(a+4500) = 'a';
	//putchar(*(a+4500));
	//char *a[] = {NULL};
	//char fileName[50] = "bin/ls";
	//execve(fileName,a,a);
	
	while(1);
}
