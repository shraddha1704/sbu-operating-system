#include "sys/defs.h"
#include "sys/outbandinb.h"
#include "sys/irq.h"

struct idt_info
{
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type;
	uint16_t offset_middle;
	uint32_t offset_high;
	uint32_t ist;

}__attribute((packed));

struct idt_info idt[256];

struct idt_pointer
{
	uint16_t limit;
	uint64_t base;

}__attribute((packed));

struct idt_pointer idtp;

extern void loading(uint64_t);
void reconfigure_pic()
{
	outb(0x20,0x20);
	outb(0x20,0x11);
	outb(0xA0,0x11);	
	outb(0x21,0x20);
	outb(0xA1,0x28);
	outb(0x21,0x04);	
	outb(0xA1,0x02);
	outb(0x21,0x01);
	outb(0xA1,0x01);
}


void create_idt_entry(int index, uint64_t address, uint16_t selector, int type)
{
	idt[index].offset_low = address & 0x0000ffff;
	idt[index].offset_middle = (address >>16 ) & 0x0000ffff;
	idt[index].offset_high = (address >>32) & 0xffffffff;
	idt[index].selector = selector;
	idt[index].ist = 0x0;
	idt[index].type = type;
	idt[index].zero = 0x0;
}


void initialise_idt()
{

	//Okay. We need to call Load Interrupt Descriptor table with base value and limit value
	//of the table
	
	idtp.base = (uint64_t)&idt;
	idtp.limit = (uint16_t)(sizeof(struct idt_info)*256-1);
	__asm__ volatile("lidt (%0)": : "rdi"(&idtp));
	//Next, we need to reconfigure our PIC. Restart, 32: PIC1, 40:PIC2, cascade, etc
	
	reconfigure_pic();
	//Make an entry in the IDT table. Map IRQ0 to index 32, IRQ1 to index 33. 
	//IDT Table is where we will store the address of the subroutine we write in the .s file
	create_idt_entry(32,(uint64_t)&irq0,0x08,0x8e);
	create_idt_entry(33,(uint64_t)&irqkey,0x08,0x8e);
	create_idt_entry(128,(uint64_t)&syscalls,0x08,0xee);
	create_idt_entry(14,(uint64_t)&pagefault,0x08,0x8e);

	//create_idt_entry(33,(uint64_t)irq1);


		
}
