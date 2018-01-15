#include<custom.h>
#include<sys/defs.h>

void _start(void) {
  //call main() and exit() here
	
        uint64_t *rsp;
        uint64_t  *argc;
        char **argv;
        char **envp;
	/*__asm__ volatile (
	"mov $0x0400, sp\n"
	"mov rsp,rbp"
	"mov 0(rbp),rdi"
	"mov 8(rbp),rsi"
	);*/
        __asm__ volatile (
        "mov %%rsp, %0\n\t"
        : "=a"(rsp)
	:
        : "memory" );

        
	argc = (uint64_t *)(rsp+1);
        argv = (char **)(rsp+2);
        if (*argc > 1) 
          envp = argv+ *argc+1;
	else
	  envp = argv + 2;
	//int argc = 0;
	//char *agrv[] = {"first", "second"};
	//char *envp[] = {"PATH", "PS1"};
	int i = main(*argc, argv,envp);
	while(1);	
	exit(i);
}
