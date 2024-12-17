
#include "mem_alloc.h"

#if defined(_USE_HEAP4_MEM_MANAGE_)



#include <stdint.h>
#include "tlsf.h"

uint8_t heap[TLSF_POOL_SIZE];
tlsf_t tlsf;

void mem_init(void)
{
    tlsf = tlsf_create_with_pool(heap, sizeof(heap));
}

void* __wrap_malloc(size_t size)
{
    // printf("call __wrap_malloc , size %ld \n", size);
    //! uart cannot be used here, use fifo instead
    // in c++ some thing will call malloc very first
    return tlsf_malloc(tlsf, size);
}

void __wrap_free(void* ptr)
{
    // printf("call free \n");
    tlsf_free(tlsf, ptr);
}

void __wrap_realloc(void* ptr, size_t size)
{
    tlsf_realloc(tlsf, ptr, size);
}





#endif