#include <sys/defs.h>
#include <sys/irq.h>
#include <sys/keyboard.h>
#include <sys/kprintf.h>
#include <sys/dir.h>
#include <sys/process.h>
#include <sys/string.h> 


int linearsearch(const char *str, char c)
{
        int flag=-1;

        while(*str)
        {
                if(*str==c)
                {
                        flag = 1;
                        break;
                }

                str++;
        }

        return flag;
}
int sys_read(int fd, void * buf, uint64_t length) {
        if (fd==0) {
		//kprintf("In syscalls, sys_read fd=0 %s \n",(char *)buf);
                int x = readFromStdin((void *) buf);
		//kprintf("In syscalls, sys_read int value %d -- %s \n",x, buf );
		return x;
        }
        else {
		int x = read_file(fd, (uint64_t)buf, length);
		//kprintf("In syscalls, sys_read %s \n",(char *)buf);
		return x;
		
	}
}



int sys_write(int fd,const void * buf, uint64_t length) {
	//kprintf("In sys_write %s -----", buf);
        if (fd==1) {

		if(strcmp("sbush:~$",buf)==0)
		kprintf("%s",buf);

		else if(linearsearch(buf,'@')!=-1)
		kprintf("%s",buf);
                
		else kprintf("%s \n",buf);
		return 0;
        }
        return 0;
}

/*int sys_cat(char *path) {
        kprintf("in cat");
        return 0;
}
*/

int sys_malloc(int size) {
	return 0;
	//return malloc(size);
}

int sys_chdir(char *path) {
	//kprintf("IN sys_chdir %s ", path);
	return chdir(path);
}
int sys_readDir(char *path) {
        return read_dir(path);
}

int sys_clearscreen() {
	clearScreen();
	return 0;
}

int sys_fork() {
	return fork();
}

int sys_open(char *path, int flags){
	//kprintf("In sys_open path passed from asm %s \n", path);
	int ret = open(path, flags);
	//kprintf("In syscalls sys_open open return %d \n", ret);
	return ret;
}

int sys_close(int fd) {
	//kprintf("In syscalls sys_close", fd);
	return close(fd);
}


int sys_getcwd(char *buf, int size) {
	getcwd(buf, size);
	//kprintf("\nIn syscalls getcwd returned buffer %s \n", buf);
	return 1;
}

int sys_execvpe(const char *file, char * argv[], char * envp[]) {
	//kprintf("In syscalls inside sys_execvpe %s\n", file);
	char fileName[50];
	strcpy(fileName, file);
	int x = execve(fileName, argv,envp);
	//kprintf("In syscalls sys_execvpe %d\n", x);
	return x;
}

int sys_ps()
{
	ps();
	return 0;

}

void sys_exit() {
	//exit();
}
/*
int sys_wait(int *status){
	return wait(int *status);
}
*/
int sys_waitpid(int pid, int *status) {
	//context switch
	return waitpid(pid,status);
	//return 0;
}

int sys_sleep(unsigned int seconds) {
	//return sleep(unsigned int seconds);
	return 0;
}


int sys_getpid(void) {
	return fetchpid();
}
int sys_getppid(void) {
	return fetchppid();
}


void my_syscalls_function(struct our_registers r) {
        //kprintf("I am in my_syscalls_function");
	uint64_t sysCallNumber = r.rax;
	uint64_t arg1 = r.rbx;
	uint64_t arg2 = r.rcx;
	uint64_t arg3 = r.rdx;

	//kprintf("\nIn my_syscalls_function %d %d %d %d\n", r.rax, arg1, arg2, arg3);

	switch(sysCallNumber) {
		case 1:
			sys_exit();
			break;
		case 3:
			sys_read((int)arg1, (void*)arg2,(uint64_t)arg3);
			break;
		case 4:
                        sys_write((int)arg1, (void*)arg2,(uint64_t)arg3);
			break;
		case 5:
                        sys_open((char *)arg1, (int)arg2);
                        break;
		case 6:
                        sys_close((int)arg1);
                        break;
		case 12:
                        sys_chdir((char*)arg1);
                        break;
		case 20:
                        sys_getpid();
                        break;
		case 64:
                        sys_getppid();
                        break;
		case 59:
                        sys_execvpe((char*)arg1, (char**)arg2, (char **)arg3);
                        break;
		case 7:
                        sys_waitpid((int)arg1, (int*)arg2);
                        break;
		case 2:
                        sys_fork();
                        break;
		case 162:
                        sys_sleep((int)arg1);
                        break;
		case 114:
                        //sys_wait((int*)arg1);
                        break;
		case 17:
                        //sys_catfiles
                        break;
		case 18:
                        //sys_echofiles
                        break;
		case 19:
                        //sys_yield
                        break;
		case 120:
			sys_malloc((int)arg1);
			break;
		case 183:
                        sys_getcwd((char*)arg1,(int)arg2);
                        break;
		case 200:
			sys_readDir((char *)arg1);
			break;

		case 270:
			sys_ps();
			break;
		case 14: 
			sys_clearscreen();
			break;
	}
}


