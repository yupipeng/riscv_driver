#ifndef __WRAP_MALLOC__
#define __WRAP_MALLOC__

#ifdef __cplusplus
extern "C" {
#endif

#include "sys_config.h"

#include <stdint.h>
#include <stddef.h>

void* __wrap_malloc(size_t size);
void* __real_malloc(size_t size);
void __wrap_free(void* ptr);
void __real_free(void* ptr);
void __wrap_realloc(void* ptr, size_t size);
void __real_realloc(void* ptr, size_t size);


// this function should be called before any memory allocation
void mem_init(void);

#ifdef __cplusplus
}
#endif

#endif