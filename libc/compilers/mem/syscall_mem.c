#include <stdio.h>
#include "mem_types.h"
#include <os_mem.h>

/*retarget to weak function to ensure that the application has freedom of redefinition*/
void *malloc(size_t size)
{
    void *ptr = os_mem_alloc(RAM_TYPE_DATA_ON, size);
    if (ptr == NULL)
    {
        //DBG_DIRECT("[malloc]!!!!!!!!!!MALLOC FAIL!!!!!!!!!");
    }

    return ptr;
}

void *calloc(size_t n, size_t size)
{
    return os_mem_zalloc(RAM_TYPE_DATA_ON, n * size);
}



void *realloc(void *ptr, size_t n)
{
    void *p = os_mem_alloc(RAM_TYPE_DATA_ON, n);
    if (ptr)
    {
        memcpy(p, ptr, n);
        os_mem_free(ptr);
    }
    return p;
}

void free(void *ptr)
{
    os_mem_free(ptr);
}
