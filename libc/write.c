#include <custom.h>
#include <sys/defs.h>
#include <unistd.h>
int ret;

//int write(int fd, const void *s, int count) {
ssize_t write(int fd, const void *s, size_t count) {
	puts(" ");
	__asm__ ("int $0x80"
         : "=a" (ret)
         : "a" (4), "b" (fd), "c" (s), "d" (count)
         : "memory"
        );

//	printsomething(s);
//	return 1;

 /*for( ; *s; ++s) if (putchar(*s) != *s) return EOF;
 return (putchar('\n') == '\n') ? 0 : EOF;*/
	return ret;
}
