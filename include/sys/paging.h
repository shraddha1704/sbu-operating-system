#ifndef PAGING_H
#define PAGING_H
#include <sys/defs.h>
#define PAGESIZE 4096
#define VIRTUAL 0xffffffff80000000
#define PHYSICAL 0x000000000fffffful
#define MASKPERMISSION 0xFFFFFFFFFFFFF000
#define PML4OFFSET 39
#define PDPOFFSET 30
#define PDOFFSET 21
#define PTOFFSET 12

#define PT_P 0x001
#define PT_R 0x002
#define PT_U 0x004

void setUpPageTables(uint64_t, uint64_t physfree,uint64_t);
void map_vir_phy_userspace(uint64_t vaddr, uint64_t paddr, uint64_t* pml4_user);
void set_cr3();
void* get_cr3();
void* kmalloc();
#endif
