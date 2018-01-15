#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/defs.h>
#include <sys/paging.h>
#include <sys/tarfs.h>

#define THREAD_RUNNING 1
#define THREAD_SLEEPING 2
#define THREAD_ZOMBIE 3

#define STACK_START 0xFFFF0000
#define STACK_MAX_SIZE 40960

#define HEAP_START 0x08000000
#define HEAP_END 0x40000000

typedef struct{
	uint64_t fd;
	tarfsStruct *node;
	uint64_t permission;
}__attribute__((packed)) fdStruct;

struct task{
	uint64_t pid;
	uint64_t ppid;
	uint64_t krsp;
	char* kstack;
	uint64_t rsp;
	struct task *nextTask;
	uint64_t taskState;
	fdStruct fd[20];
	char cwd[100];
	struct mm *mm_struct;
	uint64_t *cr3;
	uint64_t rip;
	uint64_t rax;
        uint64_t rbx;
        uint64_t rsi;
        uint64_t rdi;
        uint64_t rbp;
}__attribute__((packed));

typedef struct task taskStruct;

void createInitThread();
void createProcess(void);
extern taskStruct * currentTask;
extern taskStruct * nextTask;

struct vma {
	struct mm * mm_parent;
	uint64_t vm_start;
	uint64_t vm_end;
	struct vma *vm_next;
	uint64_t flags;
	tarfsStruct *fp;
	int type;
}__attribute__((packed));

typedef struct vma vma;

struct mm {
	uint64_t start_code;
	uint64_t end_code;
	uint64_t start_data;
	uint64_t end_data;
	uint64_t start_brk;
	uint64_t brk;
	uint64_t start_stack;
	vma *mmap_base;
}__attribute__((packed));
typedef struct mm mm;

int fork();
int getfd();
void setfdArray(taskStruct *thread);
int fetchpid();
int fetchppid();
int execve(char *fileName, char **argv, char **envp);
int waitpid(int pid, int *status);
void ps();
#endif
