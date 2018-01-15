#include <sys/paging.h>
#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physmemory.h>
#include <sys/string.h>

extern char kernmem,physbase;
extern uint64_t * freeList;
uint64_t *pml4, *pdpt, *pdt, *pt;
uint64_t cr3;

void* get_cr3()
{
	__asm volatile("movq %%cr3, %0":"=r"(cr3));
	return(void*)cr3;
}

int get_PML4_offset(uint64_t address)
{
	return(address >> PML4OFFSET)&0x1FFull;
}


int get_PDPT_offset(uint64_t address)
{
	return(address >> PDPOFFSET)&0x1FFull;

}


int get_PDT_offset(uint64_t address)
{
	return(address >> PDOFFSET)&0x1FFull;

}


int get_PT_offset(uint64_t address)
{
	return(address >> PTOFFSET)&0x1FFull;

}

//Virtual Addresses in all upper page tables. Onlt PT will have physical memory.
/*
void* allocatePDPT(uint64_t *pml4, int offset,uint64_t perm_bits)
{
	uint64_t *pdpt = (uint64_t*)allocatePage();
	pml4[offset] = (uint64_t)pdpt|perm_bits;
	return (void*)pdpt;
}


void* allocatePDT(uint64_t *pdpt, int offset, uint64_t perm_bits)
{
	uint64_t *pdt = (uint64_t*)allocatePage();
	pdpt[offset] = (uint64_t)pdt|perm_bits;
	return (void*)pdt;
}


void* allocatePT(uint64_t *pdt, int offset, uint64_t perm_bits)
{
	uint64_t *pt = (uint64_t*)allocatePage();
	pdt[offset] = (uint64_t)pt|perm_bits;
	return (void*)pt;
}
*/

void map_vir_phy(uint64_t vadd, uint64_t padd)
{
	int pml4_offset = get_PML4_offset((uint64_t)vadd);
	int pdpt_offset = get_PDPT_offset((uint64_t)vadd);
	int pdt_offset = get_PDT_offset((uint64_t)vadd);
	int pt_offset = get_PT_offset((uint64_t)vadd);

	uint64_t *pdptc, *pdtc, *ptc;
	
	if(pml4[pml4_offset]&PT_P)
		pdptc = (uint64_t*)(pml4[pml4_offset]&MASKPERMISSION);
	

	else
		{
			pdptc = (uint64_t*)allocatePage();
			memset((void*)(VIRTUAL|(uint64_t)pdptc),0,4096);
			pml4[pml4_offset]=(uint64_t)pdptc|PT_P|PT_R;

		}

	pdptc = (uint64_t*)((uint64_t)pdptc|VIRTUAL);

	
	if(pdptc[pdpt_offset]&PT_P)
		pdtc = (uint64_t*)(pdptc[pdpt_offset]&MASKPERMISSION);
	

	else
		{
			pdtc = (uint64_t*)allocatePage();
			memset((void*)(VIRTUAL|(uint64_t)pdtc),0,4096);
			pdptc[pdpt_offset]=(uint64_t)pdtc|PT_P|PT_R;

		}

	pdtc = (uint64_t*)((uint64_t)pdtc|VIRTUAL);

	
	if(pdtc[pdt_offset]&PT_P)
		ptc = (uint64_t*)(pdtc[pdt_offset]&MASKPERMISSION);
	

	else
		{
			ptc = (uint64_t*)allocatePage();
			memset((void*)(VIRTUAL|(uint64_t)ptc),0,4096);
			pdtc[pdt_offset]=(uint64_t)ptc|PT_P|PT_R;

		}

	ptc = (uint64_t*)((uint64_t)ptc|VIRTUAL);

	ptc[pt_offset] = padd|PT_P|PT_R;	
}

void map_vir_phy_userspace(uint64_t vadd, uint64_t padd, uint64_t* pml4_user)
{
	int pml4_offset = get_PML4_offset((uint64_t)vadd);
	int pdpt_offset = get_PDPT_offset((uint64_t)vadd);
	int pdt_offset = get_PDT_offset((uint64_t)vadd);
	int pt_offset = get_PT_offset((uint64_t)vadd);

	uint64_t *pdptc, *pdtc, *ptc;
	
	if(pml4_user[pml4_offset]&PT_P)
		pdptc = (uint64_t*)(pml4_user[pml4_offset]&MASKPERMISSION);
	
	else
		{
			pdptc = (uint64_t*)allocatePage();
			memset((void*)(VIRTUAL|(uint64_t)pdptc),0,4096);
			pml4_user[pml4_offset]=(uint64_t)pdptc|PT_P|PT_R|PT_U;

		}

	pdptc = (uint64_t*)((uint64_t)pdptc|VIRTUAL);
	
	if(pdptc[pdpt_offset]&PT_P)
		pdtc = (uint64_t*)(pdptc[pdpt_offset]&MASKPERMISSION);
	
	else
		{
			pdtc = (uint64_t*)allocatePage();
			memset((void*)(VIRTUAL|(uint64_t)pdtc),0,4096);
			pdptc[pdpt_offset]=(uint64_t)pdtc|PT_P|PT_R|PT_U;

		}

	pdtc = (uint64_t*)((uint64_t)pdtc|VIRTUAL);

	
	if(pdtc[pdt_offset]&PT_P)
		ptc = (uint64_t*)(pdtc[pdt_offset]&MASKPERMISSION);
	
	else
		{
			ptc = (uint64_t*)allocatePage();
			memset((void*)(VIRTUAL|(uint64_t)ptc),0,4096);
			pdtc[pdt_offset]=(uint64_t)ptc|PT_P|PT_R|PT_U;

		}

	ptc = (uint64_t*)((uint64_t)ptc|VIRTUAL);

	ptc[pt_offset] = padd|PT_P|PT_R|PT_U;

}

void map_kernel_space(uint64_t physbase,uint64_t physfree)
{
	
	int pml4_offset = get_PML4_offset((uint64_t)&kernmem);
	int pdpt_offset = get_PDPT_offset((uint64_t)&kernmem);
	int pdt_offset = get_PDT_offset((uint64_t)&kernmem);
	int pt_offset = get_PT_offset((uint64_t)&kernmem);
	
	kprintf("PML4 Offset: %d PDPT Offset: %d PDT Offset: %d \n",pml4_offset,pdpt_offset,pdt_offset);

	pml4 = (uint64_t *) allocatePage();
	memset((void *)pml4, 0x0, 512);

	pdpt = (uint64_t *) allocatePage();
	memset((void *)pdpt, 0x0, 512);

	pdt = (uint64_t *) allocatePage();
	memset((void *)pdt, 0x0, 512);

	pt = (uint64_t *) allocatePage();
	memset((void *)pt, 0x0, 512);


	cr3 = (uint64_t)pml4;
	pml4[pml4_offset] = (uint64_t) pdpt|PT_P|PT_R;
	pdpt[pdpt_offset] = (uint64_t) pdt|PT_P|PT_R;
	pdt[pdt_offset] = (uint64_t) pt|PT_P|PT_R;

	kprintf("PML4: %p  PDPT: %p PDT: %p \n",
	pml4[pml4_offset],pdpt[pdpt_offset],pdt[pdt_offset],pt[pt_offset]);

	//MAPPING KERNEL MEMORY
	kprintf("physbase physfree %p %p \n",physbase,physfree);
	while(physbase<=physfree)
	{
		pt[pt_offset++] = (uint64_t)physbase|PT_P|PT_R;
		physbase = physbase + 0x1000;
	}
	
	kprintf("PT %p \n",pt[pt_offset-1]);
}


void identity_mapping(uint64_t max)
{
	uint64_t pstart = 0x0UL;
	kprintf("MAX FREE IS %p",max);
	
	while(pstart<=max)
	{
		map_vir_phy((VIRTUAL|pstart),pstart);
		pstart+=PAGESIZE;
	}

}

void map_freelist()
{
	kprintf("freelist phy %p",freeList);
	uint64_t freeList_virtual = ((uint64_t)freeList|VIRTUAL);
	map_vir_phy(freeList_virtual,(uint64_t)freeList);
	freeList = (uint64_t*) freeList_virtual;
	kprintf("\nfreelist %p \n",freeList);
}


void map_video_memory()
{
	uint64_t video_virtual = (uint64_t)0xffffffff800b8000UL;
	int pdt_offset = get_PDT_offset((uint64_t) video_virtual);
	int pt_offset = get_PT_offset((uint64_t) video_virtual);
	uint64_t *pt = (uint64_t*)allocatePage();
	pdt[pdt_offset] = (uint64_t)pt|PT_P|PT_R;
	uint64_t video_physical = (uint64_t)0xb8000UL;
	pt[pt_offset]=video_physical|PT_P|PT_R;

	kprintf("VIDEO PDT %p PT %p \n",pdt[pdt_offset],pt[pt_offset]);
}

void map_tables()
{
	uint64_t pml4v = (uint64_t) pml4|VIRTUAL;
	map_vir_phy(pml4v,(uint64_t)pml4);
	pml4 = (uint64_t*)pml4v;

	uint64_t pdptv = (uint64_t) pdpt|VIRTUAL;
	map_vir_phy(pdptv,(uint64_t)pdpt);
	pdpt = (uint64_t*)pdptv;

	uint64_t pdtv = (uint64_t) pdt|VIRTUAL;
	map_vir_phy(pdtv,(uint64_t)pdt);
	pdt = (uint64_t*)pdtv;	

	uint64_t ptv = (uint64_t) pt|VIRTUAL;
	map_vir_phy(ptv,(uint64_t)pt);
	pt = (uint64_t*)ptv;	

	kprintf("Virtual PML$ %p PDPT %p PDT %p PT %p",pml4,pdpt,pdt,pt);

}

void setUpPageTables(uint64_t physbase, uint64_t physfree,uint64_t max)
{
	map_kernel_space(physbase,physfree);
	map_freelist();
	map_video_memory();
	map_tables();
	identity_mapping(max);
}

void* kmalloc(){

	uint64_t address = (uint64_t)allocatePage();
	address=address|VIRTUAL;
	memset((void *)address,0,4096);
	return (void *)(address);	
}

