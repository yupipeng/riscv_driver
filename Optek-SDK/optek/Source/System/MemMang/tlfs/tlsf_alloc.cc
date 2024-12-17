
#include "mem_alloc.h"

#if defined(_USE_TLSF_MEM_MANAGE_) && defined(_USE_CPP_MEM_MANAGE_)

#include <cstdio>
#include <cstdint>
#include <cstddef>

#include "tlsf.h"
extern tlsf_t tlsf;
_GLIBCXX_NODISCARD void* operator new(size_t size) _GLIBCXX_THROW (std::bad_alloc)
{
    return tlsf_malloc(tlsf, size);
}

_GLIBCXX_NODISCARD void * operator new[](size_t size) _GLIBCXX_THROW (std::bad_alloc)
{
    return tlsf_malloc(tlsf, size);
}

void operator delete(void * phead) noexcept
{
    tlsf_free(tlsf, phead);
}

void operator delete[](void * phead) noexcept
{
    tlsf_free(tlsf, phead);
}

void operator delete(void * phead, size_t size) noexcept
{
    tlsf_free(tlsf, phead);
}

void operator delete[](void * phead, size_t size) noexcept
{
    tlsf_free(tlsf, phead);
}

#endif
