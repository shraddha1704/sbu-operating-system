#include<sys/defs.h>
#include<sys/physmemory.h>
#include<sys/paging.h>

uint64_t *freeList;
uint64_t maxBase=0, maxEnd=0;
int flag=0;

void setPageFree(uint64_t bit)
{
	freeList[bit/64] &= ~(1ull << (bit%64)); 
}

void setPageBusy(uint64_t bit)
{
	freeList[bit/64] |= (1ull<< (bit%64));
}


void makeFreeList(uint64_t physbase, uint64_t physfree, uint64_t baseAddress, uint64_t endAddress)
{
	freeList = (uint64_t *) physfree;
	uint64_t i;

	if(baseAddress>maxBase)
		maxBase = baseAddress;

	if(endAddress>maxEnd)
		maxEnd = endAddress;
	
	
	baseAddress = baseAddress/PAGESIZE;
	endAddress = endAddress/PAGESIZE;
	
	for(i=baseAddress;i<endAddress;i++)
	{
		setPageFree(i);	
	}

	//We have to not use lower 1MB and things below physfree. So lets set those bits to 1.

	int pageCount = 0;

	for(i=0;i<=physfree;i+=PAGESIZE)
	{
		setPageBusy(pageCount);
		pageCount++;
	}
}

int findFreePage()
{
	
	for(int i=0;i<256;i++)
	{
		for(int j=0;j<64;j++)
		{
			uint64_t available = (1ull<<j);
	
			if(!(freeList[i]&available))
				return i*64+j;
		}

	}
	return -1;
}

void* allocatePage()
{

	uint64_t page;
	int result = findFreePage();

	if(result==-1)
		return 0;

	setPageBusy(result);
	page=result*PAGESIZE;
	return (void*)page;
}
