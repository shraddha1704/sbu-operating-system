#ifndef _STRING_H
#define _STRING_H

#include <sys/defs.h>
int strtok(char* input, char **tokens,char separator);
const char* strcpy(char *dst, const char *src);
int strlen(const char *s);
int strcmp(const char *p, const char *q);
int strncmp(const char *p, const char *q, size_t n);
uint64_t charToInt(char *s);
void strcat(char *newString, const char *orig);
void memcpy(void *destination, void *source, uint64_t length);
void* memset(void* target, int val, int length);
void memset1(void* target, int val, int length);
#endif
