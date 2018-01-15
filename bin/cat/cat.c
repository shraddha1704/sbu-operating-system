#include<custom.h>
#include <unistd.h>
int main(int argc, char *argv[], char *env[]) {
	argc = 2;
	int fd;
	char buffer[100] = {'\0'};
	if (argc > 1) {
		fd = open("qwerty.txt", 0);
		read(fd, buffer, sizeof(buffer));
		puts(buffer); 
	} else { 
		puts("Invalid format. Correct format: cat filename");
		exit(-1);
	}
	return 0;

}
