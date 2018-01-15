#include <sys/defs.h>
#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/dir.h>
#include <sys/process.h>

char cwd[1024] = "";

void getcwd(char *buf, int size){
	strcpy(buf,currentTask->cwd);
}

void getFileParent(char * basepath) {
	for (int i =0; i<500; i++){
		//modify cwd to currentTask->cwd after creating processes
		if(strcmp(tarfs_fs[i].name,cwd)==0){
			strcpy(basepath,tarfs_fs[tarfs_fs[i].par].name);
			//kprintf("\nIn dir getFileParent %s\n", basepath);
			return;
		}
	}
}

void getAbsolutePath(char *dirName, char *basepath, int flagType) {
	getcwd(cwd, 1024);
        char *paths[500];
        char concat[2] = "/";
        int token_count = strtok(dirName,paths,'/');

        int i = 0;

        if((strcmp(paths[0],"..")==0)) {
                getFileParent(basepath);
                i++;
        } else {
		if((strcmp(paths[0],".")==0)) {
			i++;
		}
                strcpy(basepath, cwd);
                //i++;
        }

        for(; i<token_count;i++){
                strcat(basepath,paths[i]);
		if(i != token_count - 1) {
                	strcat(basepath,concat);
		}
        }
        if(basepath[0]!='\0') {
                int j = 1;
                while(basepath[j]!='\0') {
                        j++;
                }
		//kprintf("In sys/dir before if condition %s --- %c --- %c --- %d\n", basepath, basepath[j], basepath[j-1], j);
                if(basepath[j-1]!='/' && flagType == 5) {
                        strcat(basepath,concat);
			j++;
                        basepath[j++]='\0';
			//kprintf("\n=======%s=====%c===", basepath, basepath[j--]);
                }
		//kprintf("\n-----basepath----%s\n", basepath);
        }
}

uint64_t chdir(char * dirName) {
	char basepath[500]="";
	getAbsolutePath(dirName, basepath, 5);

	//kprintf("\n Final path is %s\n", basepath);
	int dirNum = open_dir(basepath);
	return dirNum;
}


uint64_t open_dir(char * dirName) {
	int i = 0;
	while(1) {
		if(strlen(tarfs_fs[i].name) == 0)
			break;
		int compareNames = strcmp(&(tarfs_fs[i].name[0]), dirName);
		if((compareNames == 0) && (tarfs_fs[i].flagType == 5)) {
			//kprintf("\n In open_dir %x  ---- %d\n ", tarfs_fs[i].headerAddress, i);
			strcpy(currentTask->cwd, tarfs_fs[i].name);
			return tarfs_fs[i].headerAddress;
		}
		i++;
	}
	kprintf("No such directory \n ");
	kprintf("%s \n", dirName);
	return 0;
}

uint64_t read_dir(char* dirName) {
	int x = strcmp(dirName,"");
	if(x == 0) {
		strcpy(dirName,"./");
	}
	char basepath[500]="";
        getAbsolutePath(dirName, basepath, 5);
	strcpy(dirName, basepath);
	int i = 0, nParents = -1, dirLength = strlen(dirName);
	//memset(dirName, '\0', dirLength);
	//strcpy(dirName, basepath);
	//kprintf("In dir.c read_dir %s\n", dirName);
	while(1) {
		if(strlen(tarfs_fs[i].name) == 0) {
            		break;
		}        
		
        	if( strcmp(&(tarfs_fs[i].name[0]), dirName) == 0) {  
			//kprintf("\nIn dir read_dir tarfs_fs %s -- %s\n", &(tarfs_fs[i].name[0]), dirName);
            		nParents = i;
			i++; 
			continue; 
         	}

		int compareNames = strncmp(&(tarfs_fs[i].name[0]), dirName, dirLength);
         	if((compareNames == 0 ) &&( tarfs_fs[i].par == nParents)) {
            		kprintf("%s \n", tarfs_fs[i].name + dirLength);
         	}
        	i++;
    	}
	memset(dirName,'\0',dirLength);
    	return 0;
}

uint64_t open(char * fileName, int flags) {
	int x = strcmp(fileName,"");
        if(x == 0) {
                strcpy(fileName,"./");
        }
        char basepath[500]="";
        getAbsolutePath(fileName, basepath, 0);
        strcpy(fileName, basepath);
	int i = 0;
	while(1) {
		i++;
		if(strlen(tarfs_fs[i].name) == 0) {
			break;
		}
		
		int compareNames = strcmp(&(tarfs_fs[i].name[0]), fileName);
		if(compareNames == 0 && tarfs_fs[i].flagType == 0) {
			//kprintf("\n In dir open %s ", tarfs_fs[i].headerAddress);

			//enter in openFilesTable
			int fd = getfd();
			currentTask->fd[fd].node = &tarfs_fs[i];
			return fd;
		}
	}
	kprintf("No such file %s \n", fileName);
	return 0;
}

int read_file(int fd,  uint64_t buf, int size) {
	
	tarfsStruct * node = currentTask->fd[fd].node;
	uint64_t file_addr = node->headerAddress;    	

	if(file_addr != 0) {
		struct posix_header_ustar* file_hdr = (struct posix_header_ustar *) file_addr; 
		int file_size = 0;
		uint64_t octSize = charToInt(file_hdr->size);
        	uint64_t octalPower = 1, decimalSize = 0;

        	for(int x=0 ; octSize!=0; x++) {
        		int remainder = (int) octSize % 10;
                	octSize /= 10;
                	if( x > 0 ) {
                		octalPower *= 8;
                	}
                	decimalSize += remainder * octalPower;
                	x++;
        	}
        	file_size = decimalSize;

    		if(file_size < size) {
        		size = file_size;
		}

    		char* tmp =(char *)buf;
    		char* file_start_addr = (char *)(file_addr + 512);
    	
		int i = 0, j = size;
    		while(j-- > 0)
    		{
        		tmp[i++] = *file_start_addr++;
    		}    
    		tmp[i]='\0';
    		return size;
	} else {
		return -1;
	}
}

/*void close(uint64_t fileAddress) {
	unsigned long* file = (unsigned long* )fileAddress;
	*file = 0;
	kprintf("\nFile at %p has been closed",fileAddress);
}*/
int close(int fd) {
        currentTask->fd[fd].node = 0x0;
	return 0;
}

void closedir(uint64_t dirAddress) {
        unsigned long* dir = (unsigned long* )dirAddress;
        *dir = 0;
        kprintf("Directory at %p has been closed \n",dirAddress);
}

uint64_t getFileOffset(char* filename)
{
        struct posix_header_ustar* tarfsStart = (struct posix_header_ustar *)&_binary_tarfs_start;
        int i = 1, temp = 512;
        uint64_t size;

        while(strlen(tarfsStart->name) != 0)
        {
                tarfsStart = (struct posix_header_ustar *)(&_binary_tarfs_start + temp);

                uint64_t octSize = charToInt(tarfsStart->size);
                uint64_t octalPower = 1, decimalSize = 0;

                for(int x=0 ; octSize!=0; x++) {
                        int remainder = (int) octSize % 10;
                        octSize /= 10;
                        if( x > 0 ) {
                                octalPower *= 8;
                        }
                        decimalSize += remainder * octalPower;
                        x++;
                }
                size = decimalSize;

                if(strlen(tarfsStart->name) == 0) {
			return 999;
		}
                if(strcmp(tarfsStart->name, filename) >= 0) {
			return temp + 512;
		}
                if(size == 0) {
			temp = temp + 512;
		} else {
			temp +=  (size%512==0) ? size + 512 : size + (512 - size%512) + 512;
		}
                i += 1;
        }
        return -1;
}
