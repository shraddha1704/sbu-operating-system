#ifndef _CUSTOM_H
#define _CUSTOM_H

#define NULL ((void*)0)


static const int EOF = -1;

static const int WNOHANG = 1;
static const int O_RDONLY = 0;

int main(int argc, char *argv[], char *env[]);
void exit(int status);

//char *getcwd(char *buf, int size);
char *gets(char *input);
int putchar(int c);
//int fork();
//int chdir(char *path);
int puts(char *c);
int pipe(int *fd);
int dup2(int oldfd, int newfd);
//int open(char *path, int flag);
//int read(int fd, char *buffer, int size);
//int close(int fd);
//int execve(char *filename, char *argv[], char *env[]);
//long waitpid(long pid, int *status, int options);
//char* strcpy(char *str1, char *str2);
int strlen(const char *s);
void clearscreen();
//void *malloc(int size);
//void free(void *ptr);
#endif
