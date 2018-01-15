#include <sys/process.h>
#include <sys/paging.h>
#include <sys/kprintf.h>
#include <sys/idt.h>
#include <sys/irq.h>
#include <sys/gdt.h>
#include <sys/physmemory.h>
#include <sys/elf64.h>
#include <sys/string.h>

int processArray[2048];
taskStruct *thread1;
taskStruct *thread2;
taskStruct *currentTask;
taskStruct *nextTask;

void setfdArray(taskStruct * thread) {
	for (int i=0; i< 20; i++){
		thread->fd[i].fd = i;
		thread->fd[i].node = NULL;
		thread->fd[i].permission = 0;
	}
}

//int getfd(taskStruct * thread){
int getfd() {
	taskStruct *thread = currentTask;
	for(int i=3; i<20; i++){
		if(thread->fd[i].node == NULL ) {
			return i;
		}
	}
	return -1;
}

void copyfdArray(taskStruct *thread1,taskStruct *thread2) {
	for (int i=0; i< 20; i++){
                thread1->fd[i].fd = thread2->fd[i].fd;
                thread1->fd[i].node = thread2->fd[i].node;
                thread1->fd[i].permission = thread2->fd[i].permission;
        }
}

int fetchpid() {
	taskStruct *thread = currentTask;
	return thread->pid;
	
}

int fetchppid() {
        taskStruct *thread = currentTask;
        return thread->ppid;

}

void setProcessArray(void) {
        for(int i=0; i< 2048; i++) {
                processArray[i] = 0;
        }
}

uint64_t getPid() {
        for(int i = 0; i<2048; i++) {
                if(processArray[i]==0) {
                        processArray[i] = 1;
                        return i;
                }
        }
        return -1;
}

void ps()
{
	char state[20];	
	
	if(currentTask->taskState==2)
		strcpy(state,"SLEEPING");

	else if(currentTask->taskState==1)
		strcpy(state,"RUNNING");

	kprintf("PID: %d STATE: %s \n",currentTask->pid,state);

	if(nextTask->taskState==2)
		strcpy(state,"SLEEPING");

	else if(nextTask->taskState==1)
		strcpy(state,"RUNNING");

	kprintf("PID: %d STATE: %s \n",nextTask->pid,state);

}
/*
void createProcess(void){
        process = (taskStruct *) kmalloc();
        process->pid = getPid();
        process->ppid = 0;
        process->taskState = THREAD_RUNNING;
        process->cr3 = (uint64_t)get_cr3();
        process->kstack = (char*)(void *) kmalloc() + 0x1000;
        process->rsp = (uint64_t) process->kstack;

	createQueueElement(process);	
	
	if(process->pid==0)
	{
        	process->rip = (uint64_t)&scheduler;
		currentTask = process;
	}

	else 
		process->rip = (uint64_t)&initSchedule;

}


*/

void contextSwitch(taskStruct *current, taskStruct *next ) {
		set_tss_rsp((void*)next->krsp);
		uint64_t * start_address = (uint64_t *)((uint64_t) next->cr3 & PHYSICAL);
	__asm volatile("cli");
	__asm volatile("movq %%rsp, %0"::"r"(&(current->krsp)));
	__asm volatile("movq %0, %%rsp"::"r"(next->krsp));
	__asm volatile("movq %0, %%cr3"::"r"(start_address));
	__asm volatile("movq %0, %%rax"::"r"(next->rip));
	__asm volatile("pushq %%rax"::);
	__asm volatile("ret");
}

int waitpid(int pid, int*status) {
        taskStruct * current = currentTask;
        taskStruct * next = nextTask;
        currentTask->nextTask = nextTask->nextTask;
        nextTask->nextTask = currentTask;
        currentTask = next;
        nextTask = current;
        contextSwitch(current, next);
	return 1;
}


void switch_to_ring3()
{
	set_tss_rsp((void*)currentTask->krsp);
	uint64_t * start_address = (uint64_t *)((uint64_t) currentTask->cr3 & PHYSICAL);
	__asm volatile("cli;"\
	"movq %0, %%cr3;"\
	"mov $0x23, %%ax;"\
	"mov %%ax, %%ds;"\
	"mov %%ax, %%es;"\
	"mov %%ax, %%fs;"\
	"mov %%ax, %%gs;"\
	"movq %1, %%rax;"\
	"pushq $0x23;"\
	"pushq %%rax;"\
	"pushfq;"\
	"popq %%rax;"\
	"orq $0x200, %%rax;"\
	"pushq %%rax;"\
	"pushq $0x2B;"\
	"pushq %2;"\
	"movq $0x0, %%rdi;"\
	"movq $0x0, %%rsi;"
	::"r"(start_address),"r"(currentTask->rsp),"r"(currentTask->rip));
	__asm volatile("iretq;");
}

void createInitThread(void) {

	uint64_t temp;

        thread1 = (taskStruct *) kmalloc();
        thread1->pid = getPid();
        thread1->ppid = 0;
        thread1->kstack = (char*)kmalloc()+0x1000 ;
        thread1->rsp = (uint64_t)thread1->kstack-32;
        thread1->cr3 = (uint64_t *)((uint64_t)get_cr3()|VIRTUAL);
	*(uint64_t *)(thread1->rsp) = (uint64_t)(&switch_to_ring3);
        thread1->taskState = THREAD_SLEEPING;
	setfdArray(thread1);

	//THREAD FOR USER PROCESSES
        thread2 = (taskStruct *) kmalloc();        
        thread2->pid = getPid();
        thread2->ppid = thread2->pid;
	uint64_t* pml4_user = (uint64_t *) kmalloc();
        thread2->cr3 = pml4_user;
	uint64_t *vcr3 = (uint64_t *)thread1->cr3;
	thread2->cr3[511] = vcr3[511];
        thread2->taskState = THREAD_RUNNING;
	setfdArray(thread2);
	strcpy(thread2->cwd,"rootfs/");	
	
	thread2->mm_struct = (mm*)kmalloc();
	thread2->kstack =(char*) kmalloc()+0x1000;
	thread2->krsp = (uint64_t)(thread2->kstack) - 32;
	
	temp=((uint64_t)thread2->cr3&PHYSICAL);
	__asm volatile("movq %0, %%cr3":: "b"(temp));
	
	//AFTER ELF IS DONE, LINK ANY BINARY TO THREAD2	
	int err = load_binaries("bin/sbush",thread2);

	if(err == -1) {
		kprintf("\nBinary could not be loaded \n");
		//exit call to be done
	}

	temp = ((uint64_t) thread1->cr3 &PHYSICAL);
	__asm volatile("movq %0, %%cr3":: "b"(temp));

	nextTask = thread1;
	currentTask = thread2;
	thread2->nextTask = thread1;
	thread1->nextTask = thread2;
        switch_to_ring3();

}

void copy_parents_pagetables(taskStruct *parent, taskStruct *child){

	uint64_t *child_pml4,*parent_pml4;

        child_pml4 = child->cr3;
	parent_pml4 = currentTask->cr3;

	int i,j,k,l;

	for(i=0;i<511;i++)
	{
		if(parent_pml4[i]&PT_P)
		{
			child_pml4 = (uint64_t*)((uint64_t)child->cr3|VIRTUAL);
			uint64_t parent_pdpt = parent_pml4[i]&MASKPERMISSION;
			parent_pdpt = parent_pdpt | VIRTUAL;
			uint64_t * parent_pdpt_entry = (uint64_t *) parent_pdpt;

			uint64_t * child_pdpt;
			child_pdpt = (uint64_t*)allocatePage();
			child_pml4[i] = (uint64_t)child_pdpt|0x7;

			child_pdpt = (uint64_t*)((uint64_t)child_pdpt|VIRTUAL);

			for(j=0;j<512;j++)
			{
				if(parent_pdpt_entry[j]&PT_P)
				{	
					uint64_t parent_pdt = parent_pdpt_entry[j]&MASKPERMISSION;
					parent_pdt = parent_pdt | VIRTUAL;
					uint64_t * parent_pdt_entry = (uint64_t*) parent_pdt;

					uint64_t * child_pdt;
					child_pdt = (uint64_t*) allocatePage();
					child_pdpt[j] = (uint64_t) child_pdt | 0x7;

					child_pdt = (uint64_t*)((uint64_t)child_pdt|VIRTUAL);

					for(k=0;k<512;k++)
					{
						if(parent_pdt_entry[k]&PT_P)
						{
							uint64_t parent_pt = parent_pdt_entry[k]&MASKPERMISSION;
							parent_pt = parent_pt|VIRTUAL;
							uint64_t * parent_pt_entry = (uint64_t*)parent_pt;
							
							uint64_t *child_pt;
							child_pt = (uint64_t*)allocatePage();
							child_pdt[k] = (uint64_t) child_pt | 0x7;

							child_pt = (uint64_t*)((uint64_t)child_pt|VIRTUAL);

							for(l=0;l<512;l++)
							{
								if(parent_pt_entry[l]&PT_P)
								{
									parent_pt_entry[l] = parent_pt_entry[l] & MASKPERMISSION;
									parent_pt_entry[l] = parent_pt_entry[l] | 0x7;
								}

								child_pt[l] = parent_pt_entry[l];

							}	
						

						}

					}

				}
			}
			

		}

	}




}

void copy_parents_pagetables2(taskStruct *parent, taskStruct *child) {

        uint64_t *child_pml4, *parent_pml4;
	child_pml4 = child->cr3;
        parent_pml4=(uint64_t*)((uint64_t)currentTask->cr3|VIRTUAL);

        for(volatile int i=0;i<511;i++)
        {
                //IF THE PARENT'S PML4[i] ENTRY HAS 
                if(parent_pml4[i]&PT_P)
                {
			
        		child_pml4=(uint64_t*)((uint64_t)child->cr3|VIRTUAL);
			uint64_t *child_pdpt = (uint64_t*)allocatePage();
			uint64_t child_pdpt_entry = (uint64_t)(child_pdpt)|PT_P|PT_U|PT_R;
			child_pml4[i] = (uint64_t)child_pdpt_entry;

                        uint64_t *parent_pdpt_entry = (uint64_t*)((uint64_t)(parent_pml4[i]&MASKPERMISSION)|VIRTUAL);

                        for(volatile int j=0;j<512;j++)
                        {

                                if(parent_pdpt_entry[j]&PT_P)
				{
					
        				child_pdpt=(uint64_t*)((uint64_t)child_pdpt|VIRTUAL);
					uint64_t *child_pdt = (uint64_t*)allocatePage();
					uint64_t child_pdt_entry = (uint64_t)(child_pdt)|0x7;
					child_pdpt[j] = (uint64_t)child_pdt_entry;

                        		uint64_t *parent_pdt_entry = (uint64_t*)(uint64_t)((parent_pdpt_entry[i]&MASKPERMISSION)|VIRTUAL);


                                        for(volatile int k=0;k<512;k++)
                                        {
                                                if(parent_pdt_entry[k]&PT_P)
                                                {
							
        						child_pdt=(uint64_t*)((uint64_t)child_pdt|VIRTUAL);
							uint64_t *child_pt = (uint64_t*)allocatePage();
							uint64_t child_pt_entry = (uint64_t)child_pdt|0x7;
							child_pdt[k] = (uint64_t)child_pt_entry;
							uint64_t *parent_pt_entry = (uint64_t*)((uint64_t)(parent_pdt_entry[k] & MASKPERMISSION)|VIRTUAL);

                                                        for(volatile int l=0;l<512;l++)
                                                        {
								
								
								uint64_t pt_entry = (parent_pt_entry[l] | VIRTUAL);
								
								if(parent_pt_entry[l] & 0x001) {
									//uint64_t x = (uint64_t) (pt_entry&MASKPERMISSION);
									//pt_entry = x|0x100|0x7;
									child_pt=(uint64_t*)((uint64_t)child_pt|VIRTUAL);
									child_pt[l] = pt_entry;
									//parent_pt_entry[l] = pt_entry;
								}

                                                        }


                                                }

                                        }

                                }

                        }



                }


        }

        child_pml4[511] = parent_pml4[511];


}
int fork() {
	taskStruct * parent = currentTask;
	taskStruct * child = (taskStruct *)kmalloc();

	
	child->pid = getPid();
	if(child->pid == -1) {
		kprintf("Reached Max limit of processes. Can't fork anymore");
		return -1;
	}
	child->ppid = parent->pid;
	strcpy(child->cwd, parent->cwd);

	child->cr3 = allocatePage();	
	copy_parents_pagetables(parent,child);

	kprintf("value of cr3 before set cr3 asm %x",child->cr3);

	uint64_t temp2 = (uint64_t)child->cr3;

	__asm volatile("movq %0, %%cr3;"::"b"(temp2));

	//kprintf("value of cr3 after set cr3 asm %x",child->cr3);

	child->cr3 =(uint64_t*)((uint64_t)child->cr3|VIRTUAL);

	kprintf("value of cr3 after orring with virtual %x",child->cr3);
	setfdArray(child);
	copyfdArray(child, parent);
	//mm * temp =  (mm*)kmalloc();
	child->mm_struct = (mm*)kmalloc();
	memcpy(child->mm_struct,parent->mm_struct,sizeof(mm));
	

	vma *parent_vma = parent->mm_struct->mmap_base;
	//vma *child_vma = NULL;
	vma * var = child->mm_struct->mmap_base = NULL;
        //vma * var = child->mm_struct->mmap_base;
	

	if(parent_vma!=NULL) {
		var = (vma*) kmalloc();
        	memcpy(var,parent_vma,sizeof(vma));
		var->mm_parent = child->mm_struct;
        	child->mm_struct->mmap_base = var;

        	//if(var->vm_next) {
        	//	var = var->vm_next;
        	//}
		var->vm_next = NULL;

        	parent_vma = parent_vma->vm_next;
	}
	
	while(parent_vma!=NULL)
	{
		vma * var2 = (vma*) kmalloc();
		memcpy(var2,parent_vma,sizeof(vma));
		var2->mm_parent = child->mm_struct;
		//if(var->vm_next) {
		//	var = var->vm_next;
		//}
		var->vm_next = var2;
		var = var2;
		var->vm_next = NULL;
		parent_vma = parent_vma->vm_next;

	}
	if(!var){
		child->mm_struct->mmap_base = NULL;
		return -1;

	}
	child->rsp = (uint64_t)((uint64_t)STACK_START - 32);
	parent->nextTask = child;
        child->nextTask = nextTask;
        currentTask = parent;
        nextTask = child;

	//var->vm_next = NULL;
	child->taskState = THREAD_SLEEPING;	
	child->kstack = (char *)kmalloc() + 0x1000;
	kprintf("%x",child->kstack);
	child->krsp = (uint64_t)(child->kstack) - 32;
	memcpy((child->kstack-0x1000),(parent->kstack-0x1000),4096);	

/*	child->nextTask = parent;
	parent->nextTask = nextTask;
	currentTask = child;
	nextTask = parent;
*/	
	uint64_t offset = 0;

	uint64_t* temp =(uint64_t *)((uint64_t) parent->cr3 & PHYSICAL);

	__asm volatile("movq %0,%%cr3"::"b"(temp));

	__asm volatile("movq $2f,%0;":"=r"(child->rip));
	__asm volatile("movq %%rsp,%0;":"=r"(offset));
	__asm volatile("2:\t;");	

	if (currentTask == parent) {
		child->krsp = (uint64_t)(child->krsp-parent->krsp+offset);
		return child->pid;
	}
	else if (currentTask == child) {
		kprintf("reached back from context switch");
		return 0;
	}
	
	return -1;
}

int execve(char *fileName, char **argv, char **envp) {

	taskStruct *thread = currentTask;
	thread->mm_struct = (mm*)kmalloc();
	thread->mm_struct->mmap_base = NULL;
	uint64_t start_address = (uint64_t)get_cr3();
	uint64_t temp=((uint64_t)thread->cr3&PHYSICAL);
        __asm volatile("movq %0, %%cr3":: "b"(temp));

        //AFTER ELF IS DONE, LINK ANY BINARY TO THREAD2
        int err = load_binaries(fileName,thread);

	__asm volatile("movq %0, %%cr3":: "b"(start_address));
	
        if(err == -1) {
                kprintf("\nBinary could not be loaded \n");
		return -1;
                //exit call to be done
        }

       // temp = ((uint64_t) thread1->cr3 &PHYSICAL);
       // __asm volatile("movq %0, %%cr3":: "b"(temp));
	/*thread = (taskStruct *) kmalloc();
        thread->pid = currentTask->pid;
        thread->ppid = currentTask->ppid;
        uint64_t* pml4_user = (uint64_t *) kmalloc();
        thread->cr3 = pml4_user;
        uint64_t curr_cr3 = (uint64_t)get_cr3();
        uint64_t *vcr3 = (uint64_t *)(curr_cr3 |VIRTUAL);
        thread->cr3[511] = vcr3[511];
        thread->taskState = THREAD_RUNNING;
        setfdArray(thread);
	copyfdArray(thread, currentTask);
        strcpy(thread->cwd,currentTask->cwd);

        mm *mm_temp = (mm*) kmalloc();
        //vma1->vm_start = (uint64_t)thread2->ustack;
        //vma1->vm_end = thread2->rsp;
        mm_temp->mmap_base = NULL;
        thread->mm_struct = mm_temp;

        thread->kstack = (char*) kmalloc() + 0x1000;
        thread->krsp =  (uint64_t)(thread->kstack) - 32;
        uint64_t start_address = ((uint64_t) thread->cr3 &PHYSICAL);
        __asm volatile("movq %0, %%cr3":: "b"(start_address));

        //AFTER ELF IS DONE, LINK ANY BINARY TO THREAD2

        int err = load_binaries(fileName,thread);
        if(err == -1) {
                kprintf("\nBinary could not be loaded \n");
                //exit call to be done
		return -1;
        }
	
	start_address = ((uint64_t) curr_cr3 & PHYSICAL);
        __asm volatile("movq %0, %%cr3":: "b"(start_address));
	
	thread->nextTask = nextTask;
	//taskStruct *temp = currentTask;
	currentTask = thread;
	//exit(temp);
*/
        switch_to_ring3();
	return 0;		
}
