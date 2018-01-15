#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/idt.h>
#include <sys/irq.h>
#include <sys/paging.h>
#include <sys/process.h>
#include <sys/physmemory.h>
#include <sys/tarfs.h>
#include <sys/dir.h>

#define INITIAL_STACK_SIZE 4096

uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;
uint64_t maxBase,maxEnd;
extern uint64_t cr3;
void start(uint32_t *modulep, void *physbase, void *physfree)
{
//	int i = 0x4F, j = 79;
  struct smap_t {
    uint64_t base, length;
    uint32_t type;
  }__attribute__((packed)) *smap;
  while(modulep[0] != 0x9001) modulep += modulep[1]+2;
  for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
    if (smap->type == 1 /* memory */ && smap->length != 0) {
		makeFreeList((uint64_t)physbase, (uint64_t)physfree, smap->base, smap->base+smap->length);

	if((smap->length)> maxEnd)
	{
		maxEnd=(uint64_t)smap->length;
		maxBase=(uint64_t)smap->base;
	}

    }
  }
  //kprintf("Testing Char %x %d\r",i, j);
  //kprintf("Int testing: %d\n", 1);
  //kprintf("physfree %p\n", (uint64_t)physfree);
  //kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

  uint64_t vphysfree = (uint64_t)physfree + 0x1000;
  setUpPageTables((uint64_t)physfree, vphysfree,maxBase+maxEnd);
  //set_cr3();
  __asm volatile("movq %0, %%cr3"::"b"(cr3));
  tarfs_init();
  initialise_idt();
  irq_main();

  clearScreen();
  
  createInitThread();
  
  //uint64_t addr = open_dir("test/test1/");
  //kprintf("\n In main test function %p", addr);
  //read_dir("bin/");
  //uint64_t addr = open("bin/test", 0);
  //kprintf("In main Address of the file \n%d", addr);

  //addr = open("bin/ls", 0);
  //kprintf("In main Address of the file \n%d", addr);
  /*char buf[64];
  int size = read_file(open("bin/hello"), 32, (uint64_t) buf);
  kprintf("Size of hello.c %d", size);*/


  //createQueue();
  //checkProcesses();
  while(1);
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp1, *temp2;

  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
  init_gdt();
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
  for(
    temp1 = "!!!!start() returned!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;
  while(1) __asm__ volatile ("hlt");
}
