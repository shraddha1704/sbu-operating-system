#include<custom.h>

int close(int fd) {
	int ret = 0;
	
	//__asm volatile("sti");
	__asm__ volatile ("int $0x80"
	:"=a" (fd)
	: "a" (6), "b" (0), "c" (fd)
        : "memory");
	
	return ret;
}
