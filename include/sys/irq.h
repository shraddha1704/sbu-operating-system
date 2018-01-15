#ifndef IRQ_H
#define IRQ_H


struct our_registers{
	uint64_t flags;
	uint64_t r15,r14,r13,r12,r11,r10,r9,r8;
	uint64_t rbp;
	uint64_t rdi,rsi,rdx,rcx,rbx,rax;
	uint64_t rip,cs,eflags,rsp,ss;
}__attribute__((packed));

typedef void (*functionpointer)(struct our_registers);

extern void irq0();
extern void irqkey();
extern void syscalls();
void irq_main();
extern void pagefault();
#endif
