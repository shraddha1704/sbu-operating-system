#include <custom.h>
#include<sys/string.h>
#include <sys/printf.h>
#include <unistd.h>
#define INPUT_SIZE 100
/*int str_len(char *str)
{
        int len=0;

        while(*str++!='\0')
                len++;

        return len;

}
*/

struct env_vars{
	char path[20];
	char PS1[30];
};

int strcmp1(char *str1, char *str2)
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

char* strcat1(char *str1, char *str2)
{

        while(*str1)
                str1++;

        while(*str2)
        {
                *str1 = *str2;
                str1++;
                str2++;

        }

        return str1;


}


char* strcpy1(char *str1, char *str2)
{

        while(*str2)
        {
                *str1++ = *str2++;

        }
	*str1 = '\0';
        return str1;

}

int linearsearch(char *str, char c)
{
        int flag=-1;

        while(*str)
        {
                if(*str==c)
                {
                        flag = 1;
                        break;
                }

                str++;
        }

        return flag;
}


struct env_vars envvars;


int tokenize_input(char* input, char **tokens,char separator){
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
        return token_count;
}


void print_tokens(char **tokens){
	tokens++;
        while(*tokens!=NULL)
        {
                puts(*tokens);
                tokens++;
        }
}


int change_directory(char *input){
	int i = 0;
        char dir[INPUT_SIZE] = {'\0'};

        if ((i = chdir(input)) == 0) {
                if (getcwd(dir,sizeof(dir)) == NULL) {
                        puts("getcwd error");
			return -1;
                 }else {
                        //puts("Current directory: ");
                        puts(dir);
                        return 0;
                }

        } else {
                puts("sbush: No such file or directory");
                return -1;
        }

}

char * getenv(char *envname)
{

	//char pathsearch[1000];
	//strcpy(envname,envp[8]);
	if((strcmp1(envname,"$path")==0)|| strcmp1(envname,"$PATH")==0)
	return (char*)envvars.path;


	else if((strcmp1(envname,"$ps1")==0)|| strcmp1(envname,"$PS1")==0)
	return (char*)envvars.PS1;

	else puts("Invalid Environment Variable");
	return '\0';
}

void setenv(char *envname, char *env){

	if((strcmp1(envname,"$path")==0)|| strcmp1(envname,"$PATH")==0){
	strcat(envvars.path,";");
	strcat(envvars.path,env);
	}	

	else if((strcmp1(envname,"$ps1")==0)|| strcmp1(envname,"$PS1")==0)
	strcpy1(envvars.PS1,env);

	else puts("Invalid Environment Variable. Usage: export $PATH <path> or $PS1 <ps1>");

}

void parse_path(char *input, char **value, int *append){

        *value++ = input;
        while(*input != '='){
                input++;
        }
        *input = '\0';
        if (*++input == '$') {
                *append = 1;
                while(*input != ':')
                        input++;
        }
        *value++ = input;
        *value = '\0';
}

int cat(char *filename)
{
        int fd;
        char buffer[1000] = {'\0'};
	fd = open(filename, 0);
	read(fd, buffer, sizeof(buffer));
	puts(buffer);
        return 0;
}


int ls(char *tokens1)
{
        int returnvalue=0;
         __asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (200), "b" (tokens1): "memory");
        return returnvalue;	
}

void mysleep(int time)
{

	unsigned long timer=0;
	
	int frequency = (1193182*120*time);
	while(timer!=frequency)	
	{
		timer++;
	}

}
void execute_binaries(char **tokens,char **envp, int token_count){
	//execvpe(tokens[0],tokens,envp);

	if(strcmp1(tokens[0],"ls")==0)
		{
			if(token_count==1)
				ls("");

			else
				ls(tokens[1]);

		}

	else if(strcmp1(tokens[0],"cat")==0)
        {
	 	if(token_count!=2) {
                	puts("Invalid format. Correct format: cat filename");
        	}

		else{
			cat(tokens[1]);
		}

	}

	
	else if(strcmp1(tokens[0],"sleep")==0)
        {

		if(token_count==1)
			puts("Invalid. Usage sleep <forhowmuchtimeinseconds>");
	 	
		else{
			int time = charToInt(tokens[1]);
			mysleep(time);
		}

	}

	else if((strcmp1(tokens[0],"export")==0)|(strcmp1(tokens[0],"EXPORT")==0))
		setenv(tokens[1],tokens[2]);

	else if(strcmp1(tokens[0],"echo")==0)
        {

		if(tokens[1][0]=='$')
			{
				char * res = getenv(tokens[1]);
				puts(res);

			}

		else
			print_tokens(tokens);
	 	

	}

	else if(strcmp1(tokens[0],"ps")==0)
	{
         	__asm__( "int $0x80"
         	: : "a" (270) : );
	} else if(strcmp1(tokens[0],"clear")==0) {
		clearscreen();
	}
        /*int pid;
	int status;
	char argv[100] = "/bin/";
        if ((pid = fork()) < 0 )
	{
                puts("error forking child");
                exit(1);

	 } else if (pid == 0) {
		strcat1(argv, tokens[0]);
                if (execve(argv,tokens, envp) < 0) {
			  exit(1);
		}
        } else {
                waitpid(-1, &status, 2);
	}*/
}

void execute_scripts(char **tokens_lines,char **envp)
{
        char *tokens_words[100];
	int token_count=0;

        while(strcmp1("#!rootfs/bin/sbush",*tokens_lines)!=0)
        {
                tokens_lines++;
        }

        if(!strcmp1("#!rootfs/bin/sbush",*tokens_lines))
        {
                ++(tokens_lines);

                while(*tokens_lines!=NULL)
                {
                        token_count = tokenize_input(*tokens_lines,tokens_words,' ');
                        tokens_lines++;

                        if(*tokens_words!=NULL && !strcmp1(*tokens_words,"cd")){
                                change_directory(*(tokens_words+1));
                        }
                        else
                                execute_binaries(tokens_words,envp,token_count);
                }

        }
        else
        {
                puts("Not a sbush script. Cannot execute");
        }
}


void parse_scripts(char *filename,char **envp){

        
	filename=filename+2;
        int fd;
        char buffer[1000] = {'\0'};
	fd = open(filename, 0);
	read(fd, buffer, sizeof(buffer));
	puts(buffer);
        
        char *tokens_lines[100];

        tokenize_input(buffer,tokens_lines,'\n');
        execute_scripts(tokens_lines,envp);

}

void parse_pipe(char *input, char **token, char **token2) {
	int i = 0;
	while(*input != '|'){
                while (*input == ' '){
                        *input = '\0';
                        input++;
              }
                token[i++] = input;
                while(*input != ' ' && *input != '|') {
                        input++;
                }
                if (*input == ' '){
                        *input = '\0';
                        input++;
                }
                if(*input == '|') {
                        *input = '\0';
                        input++;
                        break;
                }

        }
	i = 0;
        while (*input != '\0'){
                while (*input == ' '){
                        *input = '\0';
                        input++;
                }
                token2[i++] = input;
                while (*input != ' ' && *input != '\0') {
                        input++;
                }
       }
	
}

void background_run(char **in, int tokencount) {

        int pid, w;
	int status;
	char argc[100] = "/bin/";
	char *env[] = {NULL};
	//struct rusage usage;
        in[tokencount-1] = '\0';

        pid = fork();
        if (pid == 0) {
		strcat1(argc, in[0]);
                if (execvpe(argc, in, env) < 0)
                        exit(1);
                exit(0);
        } else {
	
		//w = wait4(pid, &status, WNOHANG, &usage);
                w = waitpid(pid, &status);
                if (w == -1)
                        puts("waitpid error");
                        if (status == 0)
                                puts("Done");
                        else if (status == 1)
          
                     puts("Error");
        }
}



int main(int argc, char *argv[], char *envp[]) {
        char input[INPUT_SIZE]={'\0'};
	//char inputcopy[INPUT_SIZE]={'\0'};
        char *tokens[INPUT_SIZE];
        char *pipe1[INPUT_SIZE];
        char *pipe2[INPUT_SIZE];
        int fd[2];
        int pid;
	int i=0,len=0;
	//char *env[] = {'\0'};
	char *env[10];
	int status;

	char *ps1lol;
	ps1lol = getenv("$PS1");
	char path[20] = "rootfs";
	char PS1[30] = "osunderferdman@sbush:~$";

	strcpy1(envvars.path,path);
	strcpy1(envvars.PS1,PS1);
        //FILE *filein;
        //char buffer[INPUT_SIZE] = {'\0'};
        //set_environment_variable("PS1", "amanmalvika", env);
        int flag = 1, token_count = 0;
        //char *ps1;
        //envp = "PATH=/home/mamodi/os/workdir";
        //int i=0;

        while(flag)
        {
		ps1lol = getenv("$PS1");

                if (ps1lol!= NULL) {
			puts(ps1lol);
                } 
               	else puts("sbush:~$");
			//printf("\n");

                //print_tokens(argv);

                /*if(argc>1)
                {
                        parse_scripts(argv,envp);
                        flag=0;
                }*/
		 char *ic = '\0';
		 //memset1(tokens,0,100);
		 ic = gets(input);
                 if(ic!=NULL && ic[0] != '\0'){
			len = strlen(ic);
                        strcpy(ic,input);
                        token_count = tokenize_input(input,tokens,' ');
                        //print_tokens(tokens);
	
                        if(!strcmp1(tokens[0],"cd")) {

				if(token_count==1)
				{
					tokens[1]="./";

				}
                                change_directory(tokens[1]);
			} 
			else if(tokens[0][0]=='.' && tokens[0][1]=='/')
			{
				parse_scripts(tokens[0],envp);
			}

			else if (!strcmp1(tokens[0], "exit")) {
				puts("Exiting Shell");
                                flag=0;
                        } else if (linearsearch(ic, '|') != -1){
                                parse_pipe(ic, pipe1, pipe2);
                                if (pipe(fd) < 0) {
                                        puts("error in pipe");
					exit(-1);
				}
                                pid = fork();
                                if (pid == 0) {
                                        if (dup2(fd[1], 1) < 0) {
						puts("error in dup2");
						exit(-1);
					}
                                        execvpe(pipe1[0], pipe1, env);
                                        exit(0);                              
                                } else {
					waitpid(-1, &status);
                                        pid = fork();
                                        if (pid == 0) {
                                                if (dup2(fd[0], 0) < 0) {
							puts("error in dup2 child 2");
							exit(-1);
						}
                                                execvpe(pipe2[0], pipe2, env);
                                                exit(0);
                                        } else {
                                                waitpid(-1, &status);
                                        }
                                }
                        } else if (linearsearch(ic, '&') != -1) {
                                background_run(tokens, token_count);
			} 
			 else if(strcmp1(tokens[0], "pwd") == 0) {
        			char dir[INPUT_SIZE] = {'\0'};
				//getcwd(dir,sizeof(dir));

				if (getcwd(dir,sizeof(dir)) == NULL) {
                 		        puts("getcwd error");
                        		//return -1;
                 		}else {
                        		//printf("Current directory: %s\n", dir);
                        		puts(dir);
					//printf("\n");
                        		//return 0;
                		}
			} else {
                        //Else it is a Binary
                                execute_binaries(tokens,envp,token_count);
			}

			for(i=0;i<len;i++) {
				input[i]='\0';
			}
                }
        }


        return 0;
}




