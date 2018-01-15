#include <sys/defs.h>
#include <sys/tarfs.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/dir.h>

int getPar(char* dir, int flagType);

tarfsStruct tarfs_fs[100];

void tarfs_init() {
       	struct posix_header_ustar *tarfs_var = (struct posix_header_ustar *)&_binary_tarfs_start;
	int c = 0;
	int i = 0, temp = 0;
	uint64_t size;
       	tarfsStruct tarfs_e;

	tarfsStruct rootNode;
	strcpy(&rootNode.name[0], "rootfs/");
	rootNode.size = 0;
	rootNode.headerAddress = 0;
	rootNode.par = 100;
	rootNode.flagType = 5;
	//memset(&rootNode.name[0], '\0', strlen(&rootNode.name[0]));
	tarfs_fs[0] = rootNode;
	i++;

	while(1) {
		tarfs_var = (struct posix_header_ustar *)(&_binary_tarfs_start + temp);
		memset(&tarfs_e.name[0], '\0', strlen(&tarfs_e.name[0]));
           	if(strlen(tarfs_var->name) == 0) {
                	break;
		}
          
		uint64_t octSize = charToInt(tarfs_var->size);
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
		//strcpy(&tarfs_e.name[0], tarfs_var->name);
		strcpy(&tarfs_e.name[0], "rootfs/");
		strcat(&tarfs_e.name[0],tarfs_var->name);	
		tarfs_e.size = size;
           	tarfs_e.headerAddress = (uint64_t)&_binary_tarfs_start + temp;
           	tarfs_e.flagType = charToInt(tarfs_var->typeflag);
           	
		if(tarfs_e.flagType == 0) {	
                	tarfs_e.par = getPar(&(tarfs_e.name[0]), 0);
			kprintf("\n In tarfs flagType is file %s par %d", &(tarfs_e.name[0]), tarfs_e.par);
		} else if(tarfs_e.flagType == 5) {
                	tarfs_e.par = getPar(&(tarfs_e.name[0]), 5);
			kprintf("\n In tarfs flagType is dir %s par %d", &(tarfs_e.name[0]), tarfs_e.par);
           	}
            
           	tarfs_fs[i] = tarfs_e;
           	i++;
	
		if(size == 0) {
			temp = temp + 512;
		} else {
			temp +=  (size%512==0) ? size + 512 : size + (512 - size%512) + 512;
		}
		c++;
	}
	kprintf("\n escaped while loop");
}

int getPar(char* dir, int flagType) {
    	int length = strlen(dir);
    	char name[100];

	strcpy(&name[0], dir);
	//kprintf("\n IN tarfs getPar name %s length %d", dir, length);	
	if(flagType == 0) {
		length = length - 1;
	} else {
	    	length = length - 2;
	}

	for(; name[length] != '/'; length--) {
        	if(length == 0) {
            		return 999;
		}
    	}
    	name[++length] = '\0';

	int par = 0;
	for(par = 0; strcmp(&name[0], &(tarfs_fs[par].name[0])) !=  0 && par<100 ; par++) {
	}
    	return par;
}


