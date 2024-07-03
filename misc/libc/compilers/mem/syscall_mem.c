#include <stdio.h>
#include <string.h>
#include "mem_types.h"
#include <os_mem.h>
#include <os_sched.h>

#define FreeRTOS_portBYTE_ALIGNMENT 8
typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK *pxNextFreeBlock;   /*<< The next free block in the list. */
    size_t xBlockSize : 28;                 /*<< The size of the free block. */
    size_t xRamType : 3;
    size_t xAllocateBit : 1;
} BlockLink_t;

/*retarget to weak function to ensure that the application has freedom of redefinition*/
static volatile bool hang_malloc = true;
void *malloc(size_t size)
{
    return os_mem_alloc(RAM_TYPE_DATA_ON, size);
}

void *calloc(size_t n, size_t size)
{
    return os_mem_zalloc(RAM_TYPE_DATA_ON, n * size);
}


#if 0
void *realloc(void *ptr, size_t n)
{
    void *new_ptr;
    BlockLink_t *pxLink;
    size_t old_size, new_size = 0;

    if (ptr == NULL)
    {
        return os_mem_alloc(RAM_TYPE_DATA_ON, n);
    }

    if (n == 0)
    {
        os_mem_free(ptr);
        ptr = NULL;
        return NULL;
    }

    pxLink = (BlockLink_t *)((uint8_t *)ptr - sizeof(BlockLink_t));
    old_size = pxLink->xBlockSize;

    if ((old_size - sizeof(BlockLink_t)) ==
        (n + (FreeRTOS_portBYTE_ALIGNMENT - (n & (FreeRTOS_portBYTE_ALIGNMENT - 1)))))
    {
        return ptr;
    }

    os_sched_suspend();
    os_mem_free(ptr);
    new_ptr = os_mem_alloc(RAM_TYPE_DATA_ON, n);
    if (ptr != new_ptr)
    {
        pxLink = (BlockLink_t *)((uint8_t *)new_ptr - sizeof(BlockLink_t));
        new_size = pxLink->xBlockSize;
        memcpy(new_ptr, ptr, (old_size < new_size) ? (old_size - sizeof(BlockLink_t)) : (new_size - sizeof(
                    BlockLink_t)));
    }
    os_sched_resume();

    return new_ptr;
}

#else

void *realloc(void *ptr, size_t n)
{
    if (ptr == NULL)
    {
        return os_mem_alloc(RAM_TYPE_DATA_ON, n);
    }
    BlockLink_t *pxLink;
    pxLink = (BlockLink_t *)((uint8_t *)ptr - sizeof(BlockLink_t));
    size_t old_size = pxLink->xBlockSize - sizeof(BlockLink_t);
    if (old_size >= n)
    {
        return ptr;
    }

    os_sched_suspend();
    void *p = os_mem_alloc(RAM_TYPE_DATA_ON, n);
    if (ptr)
    {
        memcpy(p, ptr, old_size);
        os_mem_free(ptr);
    }
    os_sched_resume();
    return p;
}
#endif

void free(void *ptr)
{
    os_mem_free(ptr);
}
