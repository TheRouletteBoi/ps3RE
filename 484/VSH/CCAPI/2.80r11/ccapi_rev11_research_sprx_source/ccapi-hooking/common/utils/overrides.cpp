
#include <sys/sys_types.h>

extern "C" void* _sys_malloc(size_t);
extern "C" void _sys_free(void* ptr);

void* operator new(size_t size)
{
    return _sys_malloc(size);
}

void* operator new(size_t size, size_t)
{
    return _sys_malloc(size);
}

void* operator new[](size_t size)
{
    return _sys_malloc(size);
}

void operator delete(void* ptr)
{
    _sys_free(ptr);
}

void operator delete[](void* ptr)
{
    _sys_free(ptr);
}