#include "sys/defs.h"
#include "sys/outbandinb.h"
#include "sys/irq.h"
#include "sys/kprintf.h"
#include "sys/keyboard.h"
#include "sys/syscalls.h"
#include "sys/process.h"
#include "sys/physmemory.h"

#define STACK_START 0xFFFF0000
#define STACK_END 0xFFFF0000-(4096*9)
const uint64_t constVirAddr1 = 0xffffffff80000000ul;
const uint64_t constPhyAddr1 = 0x000000000fffffful;
extern taskStruct* currentTask;

int timer_value = 0;
functionpointer interrupt_handlers[256];
int count = 0;
void register_handler(uint8_t index, functionpointer ourfunc)
{
	interrupt_handlers[index] = ourfunc;

}
uint64_t tCount = 0, tCountSec = 0, tCountSecond = 0, tCountMin = 0, tCountHr = 0;

void my_timer_function(struct our_registers r)
{

	/*if(timer_value%38==0)
	{
		count++;
		printTime(count);
	}

	timer_value++;*/
	tCount++;
        /*if(tCount%38==0) {
                tCountSec++;
                kprintfWithPos(70,24,"%d", tCountSec);
        }*/
        if(tCount%3800==0) {
                tCountSec++;
                tCountHr = tCountSec/3600;
                tCountMin = (tCountSec % 3600)/60;;
                tCountSecond = (tCountSec % 3600)%60;
                //kprintf("%d",tCountSecond);
//kprintfWithPos(70,24,"%t:%t:%t",tCountHr,tCountMin,tCountSecond);
//		printTim/e(tCountSecond);
        }


}

void irq_handler(struct our_registers r)
{
	outb(0x20,0x20);
	functionpointer handler = interrupt_handlers[32];
	handler(r);
}

void syscalls_handler(struct our_registers r)
{
	outb(0x20,0x20);
	functionpointer handler = interrupt_handlers[128];
	handler(r);
}

void pagefault_handler(struct our_registers r)
{
	outb(0x20,0x20);
	functionpointer handler = interrupt_handlers[14];
	handler(r);
//	outb(0x20,0x20);
}

void irq_key_handler(struct our_registers r) {
//	kprintf("In key handler");
	outb(0x20,0x20);
	functionpointer handler = interrupt_handlers[33];
	handler(r);
}

void timer()
{
	
	int irq0 = 32;
//	int irq1 = 33;
	int frequency=1000;	
	register_handler(irq0, my_timer_function);
	//register_handler(irq1, my_timer_function);

	//PIT happens now
	frequency = (uint16_t)1193180*frequency;
	outb(0x43,0x36);
	
	uint8_t low = (uint8_t) frequency & 0xff;
	uint8_t high = (uint8_t) frequency >> 8;
	
	outb(0x40,low);
	outb(0x40,high);

}

void grow_heap() {
//      uint64_t count = pages;
        uint64_t brk = currentTask->mm_struct->brk;
        uint64_t * temp = (uint64_t *)((uint64_t)currentTask->cr3 | constVirAddr1);
//      if((brk + pages*4096) >HEAP_END){
//              return -1;
//      }
//      while(count!=0) {
        if (brk+4096 < HEAP_END) {
                uint64_t * paddr = allocatePage();
                map_vir_phy_userspace(brk,(uint64_t)paddr,temp);
                brk = brk +4096;
                currentTask->mm_struct->brk = brk;
        }
//      return pages;
}

void my_pagefault_function(struct our_registers r) {
	//kprintf("OOOH.. PAAAGE FAAAAULT");
	uint64_t address=0;
	__asm__ volatile("movq %%cr2, %0":"=b"(address):);
	//kprintf("PF on Addr: %x \n",address);

	/*vma *temp = currentTask->mm_struct->mmap_base;
	int flag=0;
	while(temp!=NULL)
	{
		if(address>=temp->vm_start && address<=temp->vm_end)
		{
			kprintf("Gracefully handling pagefault at %x \n",address);
			address = address & 0xFFFFF000;
			uint64_t *paddr = allocatePage();
			uint64_t *temp = (uint64_t*)((uint64_t)currentTask->cr3|VIRTUAL);
			map_vir_phy_userspace(address,(uint64_t)paddr,temp);
			flag=1;
			break;	
		}

		temp = temp->vm_next;


	}	
	*/
	vma *temp = currentTask->mm_struct->mmap_base;
	while(temp!=NULL) {
		if(temp->type == 0) {
			if((address>=temp->vm_start) && (address<=temp->vm_end)) {
				uint64_t * vir_addr = allocatePage();
				address = address & 0xFFFFFF000;
				uint64_t * temp_cr3 = (uint64_t *)((uint64_t)currentTask->cr3 | constVirAddr1);
				map_vir_phy_userspace(address,(uint64_t)vir_addr, temp_cr3);
				return;
			}
		}
		temp = temp->vm_next;
	}

	if((address >= STACK_END) && (address < STACK_START )) {
               // kprintf("Inside N PFH \n");         
                address = address & 0xFFFFFF000;
                uint64_t * paddr = allocatePage();
		uint64_t * temp = (uint64_t *)((uint64_t)currentTask->cr3 | constVirAddr1);
                map_vir_phy_userspace(address,(uint64_t)paddr,temp);
       	}
	
	else if((address>=HEAP_START)&&(address<=HEAP_END)) {
                grow_heap();
        }

        else {
                //kprintf("HLT \n");
                 __asm volatile("hlt");
        }

/*
	if(flag==0) {
	        kprintf("HLT \n");
        	 __asm volatile("hlt");
        }
*/
}

void syscalls_init()
{
	
	register_handler(128,my_syscalls_function);

}

void pagefault_init()
{
	register_handler(14,my_pagefault_function);

}

void irq_main()
{
	__asm__ volatile("sti");
	register_handler(33, keyboard_handler);
	timer();
	syscalls_init();
	pagefault_init();
}
