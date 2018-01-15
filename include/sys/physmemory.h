#ifndef __PHYSMEMORY_H
#define __PHYSMEMORy_H

void makeFreeList(uint64_t, uint64_t, uint64_t, uint64_t);
void* allocatePage();

#endif
