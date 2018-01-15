#ifndef SYSCALLS_H
#define SYSCALLS_H
#include <sys/irq.h>

void my_syscalls_function(struct our_registers r);
int read(int fd, void * buf, uint64_t length);

#endif
