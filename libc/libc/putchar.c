#include <custom.h>

int ret;
char a[1];
int putchar(int c)
{	
	a[0] = c;
	// write character to stdout
	//write(STDOUT_FILENO, (char)c,1);
	__asm__("int $0x80": "=a" (ret)
          : "a" (4), "b" (1), "c" (a), "d" (1)
          : "memory"
          );
	if (ret == 1) {
		ret = (int) a[0];
	} else {
		ret = EOF;
	}
	return ret;
}
