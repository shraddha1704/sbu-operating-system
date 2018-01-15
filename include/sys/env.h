#ifndef _ENV_H
#define _ENV_H

#include <sys/defs.h>

//environment variables struct
typedef struct {
	char name[20];
	char value[50];
} envStruct;

extern envStruct envArray[30];

#endif
