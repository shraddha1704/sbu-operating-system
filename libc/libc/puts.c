#include <custom.h>
int ret;
void printsomething();

int puts(char *s)
{
	__asm__ ("int $0x80"
         : "=a" (ret)
         : "a" (4), "b" (1), "c" (s), "d" (sizeof(s)-1)
         : "memory"
        );

//	printsomething(s);
//	return 1;

 /*for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
 return (putchar('\n') == '\n') ? 0 : EOF;*/
	return ret;
}
