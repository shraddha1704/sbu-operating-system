/* 
#include<custom.h>

int ret;
int pid_g;
int *status_g;
int options_g;

int wait4(int pid, int *status, int options){
        
        pid_g = pid;
        status_g = status;
        options_g = options;    

        __asm__ ("int 0x80"
        : "=a" (ret)
        : "a" (114), "b" (pid_g), "c" (status_g), "d" (options)
        : "memory");    
        return ret;
} */
