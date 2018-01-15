#include <custom.h>
#include <sys/defs.h>
#include <sys/printf.h>
#define HEAP_END  0x40000000
#define HEAP_START  0x08000000
//static int heap_flag = 1;
//uint64_t brk = 0x08001000;
//uint64_t start_heap =0x08000000;
//uint64_t  heap_ptr =(uint64_t) HEAP_START;
//uint64_t heap_ptr =134217728;
// 0x08000000;
//#define HEAP_END  0x40000000
void *malloc(int size) {
static uint64_t  heap_ptr = (uint64_t) HEAP_START;
/*if(heap_flag == 1) {
heap_flag = 0;
heap_ptr =(uint64_t) HEAP_START;
}
//int a=0;
if(size > (brk - heap_ptr)) {
*/	//int pages = (size/8)*8;
/*	int a=0;
	uint64_t ptr;
        if(size%8 !=0) {
                a = 8;
        }
        a += (size/8)*8;
	__asm__ ("int $0x80"
         : "=a" (ptr)
         : "a" (120), "b" (a)
         : "memory");

	return (void *)ptr;
	
	if(a<0) {
		printf("Couldn't allocate request: Ran out of Heap memory");
		return NULL;
	}
	else {
		brk = brk + 0x1000*pages;
	}

}
*/
	if(size<0) {
                printf("Couldn't allocate request: Ran out of Heap memory");
                return NULL;
        }
	int a;
	if(size%8 !=0) {
		a = 8;
	}
	a = a+ (size/8)*8;
	//printf("heap_pts initially is %d",(uint64_t)heap_ptr);
	// temp = heap_ptr;
	uint64_t temp = heap_ptr;
	heap_ptr = heap_ptr + a;
	if (heap_ptr >(uint64_t) HEAP_END) {
		puts("Couldn't allocate request: Ran out of Heap memory");
                heap_ptr = heap_ptr - a;
		return NULL;
	}
	//heap_ptr = heap_ptr + a;
	return (void *)temp;

}
