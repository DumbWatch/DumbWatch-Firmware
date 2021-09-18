/**
 * 
 */
#include <cstdlib>

void* operator new(size_t alloc_size)
{
    return malloc(alloc_size);
}

void* operator new[](size_t alloc_size)
{
    return malloc(alloc_size);
}

void operator delete(void* ptr)
{ 
    free(ptr);
}

void operator delete(void* ptr, size_t)
{ 
    free(ptr);
}

void operator delete[](void* ptr, size_t)
{
    free(ptr);
}

void operator delete[](void* ptr)
{
    free(ptr);
}