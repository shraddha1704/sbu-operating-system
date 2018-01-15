#include<custom.h>
#include<sys/printf.h>
#include <sys/string.h>
long ret;
char buffer[1024];
int fd;


int  main(int argc, char *argv[], char *engv[]) {
	
	//struct linux_dirent *d;
	//int pos;
	//puts("NPOOOOOHEU$GF");	

	//if(argc<=1)
	//	fd= open(".", 0);

	//else

	/*fd=open("/home/amaagarwal/cse506/git/",0);
	for(;;) {
	 __asm__ volatile ("int $0x80"
                : "=a" (ret)
                : "a" (220), "b" (fd), "c" (buffer), "d" (sizeof(buffer))
                : "memory"); 
	
	if (ret == 0)
         	exit(-1);
		for (pos = 0; pos < ret;) 
		{
            		d = (struct linux_dirent *) (buffer + pos);
           		puts(d->d_name); 
            		pos += d->d_reclen;
        	}
	}*/
/*	char *path={'\0'};
	if(argc < 1) {
		path = "";
	} else {
		strcpy(path,argv[1]);
	}
*/
	char in[50] = {'\0'};
	//int i = 0;
	int returnvalue=0;
        /*while(*path) {
                in[i++] = *path++;
        }*/
//        in[i]='\0';
         __asm__( "int $0x80"
         :"=a" (returnvalue) : "a" (200), "b" (in): "memory");
        //puts("successfully done ls in ls.c");
	return returnvalue;
}
