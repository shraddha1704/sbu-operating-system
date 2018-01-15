#include "sys/defs.h"
#include "sys/outbandinb.h"
#include "sys/irq.h"
#include "sys/kprintf.h"
#include "sys/keyboard.h"
#include "sys/syscalls.h"

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

}

void irq_key_handler(struct our_registers r) {
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

/*void my_syscalls_function(struct our_registers r)
{
	kprintf("I AM HERE OMG OMG OMG");
	if(r.rcx==7)
		{
			kprintf("Okay. This is too good to be true");
		}
}
*/
void my_pagefault_function(struct our_registers r)
{
	kprintf("OOOH.. PAAAGE FAAAAULT");

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
