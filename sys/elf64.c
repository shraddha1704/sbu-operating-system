#include <sys/process.h>
#include <sys/elf64.h>
#include <sys/string.h>
#include <sys/paging.h>
#include <sys/physmemory.h>
#include <sys/tarfs.h>
#include <sys/dir.h>
#include <sys/kprintf.h>
#include <sys/idt.h>
#include <sys/dir.h>
#include <sys/irq.h>
#include <sys/gdt.h>

extern tarfsStruct VFS[100];
const int LOADABLE = 1;
const int VM_EXEC = 0x1;
const int VM_READ = 0x4 ;
const int VM_WRITE = 0x2;
const int PAGE_SIZE = 4096;
const int HEAP = 1;
const int STACK = 2;

vma * createvma(taskStruct *thread) {
	vma * var = thread->mm_struct->mmap_base;
	if (var == NULL) {
		var = (vma *)kmalloc();
		var->vm_next = NULL;
		thread->mm_struct->mmap_base = var;
		var->mm_parent = thread->mm_struct;
		return var;
	}
	else {
		while(var->vm_next != NULL) {
			var = var->vm_next;
		}
		var->vm_next = (vma*)kmalloc();
		var->vm_next->vm_next = NULL;
		var->vm_next->mm_parent = thread->mm_struct;
		return var->vm_next;
	}	

}

int load_binaries(char *path, taskStruct *thread) {

	uint64_t offset = getFileOffset(path);
	
	if(offset >=0) {
		
		Elf64_Ehdr *elf_ehdr = (Elf64_Ehdr *)(&_binary_tarfs_start + offset);
		Elf64_Phdr *elf_phdr = (Elf64_Phdr *)((uint64_t)elf_ehdr + elf_ehdr->e_phoff);

		int count = elf_ehdr->e_phnum;
	
		for (int i=0;i<count;i++) {
	
			if(elf_phdr->p_type== LOADABLE) {

				vma *vm = createvma(thread);
				vm->vm_start =elf_phdr->p_vaddr;
				vm->vm_end = vm->vm_start + elf_phdr->p_memsz;
				vm->flags = elf_phdr->p_flags;
				vm->type = 0;		
				uint64_t lower_addr, higher_addr;				
				if ((vm->vm_start % PAGE_SIZE) != 0){				
					lower_addr = (vm->vm_start/4096)*4096;
				}
				else {
					lower_addr = vm->vm_start;
				}
				

				if ((vm->vm_end % PAGE_SIZE) != 0){
					higher_addr = (vm->vm_end/4096)*4096 + 4096;
                                }
                                else {
                                       higher_addr = vm->vm_end;
                                }

				for (uint64_t i=lower_addr; i<higher_addr;  i= i+PAGE_SIZE) {
					uint64_t * vir_addr = allocatePage();
					map_vir_phy_userspace( i,(uint64_t)vir_addr,thread->cr3);
					//i = i+PAGE_SIZE;
				}

					
				if ((vm->flags == (VM_EXEC|VM_READ)) || (vm->flags== (VM_READ|VM_WRITE))) {
					memcpy((char *)vm->vm_start,(char *)(elf_ehdr + elf_phdr->p_offset), elf_phdr->p_filesz);
				}
				
			}

			elf_phdr++;
		}
	
		vma *heap = createvma(thread);
		heap->vm_start = thread->mm_struct->start_brk= thread->mm_struct->brk = HEAP_START;
		heap->vm_end = HEAP_START + 4096;
		heap->flags = (VM_READ | VM_WRITE);
		heap->type = 1;
		uint64_t * addr = allocatePage();
                map_vir_phy_userspace(HEAP_START, (uint64_t)addr,thread->cr3);
		
		vma *stack = createvma(thread);
		thread->mm_struct->start_stack = STACK_START;
		uint64_t * addr2 = allocatePage();
                map_vir_phy_userspace((STACK_START-0x1000),(uint64_t)addr2,thread->cr3);
		stack->type = 2;
		stack->flags = (VM_READ | VM_WRITE);
		stack->vm_start = (uint64_t)STACK_START;
                stack->vm_end = (uint64_t)STACK_START-STACK_MAX_SIZE;
		thread->rsp = (uint64_t)(stack->vm_start - 32);
		thread->rip=elf_ehdr->e_entry;
		return 0;
	} else {
		return -1;
	}

}
