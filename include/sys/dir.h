#ifndef _DIR_H
#define _DIR_H

#include <sys/defs.h>

uint64_t open_dir(char * dir);
uint64_t read_dir(char* dir);
uint64_t open(char * file, int flags);
int read_file(int fd,  uint64_t buf, int size);
int close(int fd);
void closedir(uint64_t dir_addr);
uint64_t chdir(char * dirName);
uint64_t getFileOffset(char* filename);
void getcwd(char *buf, int size);
#endif
