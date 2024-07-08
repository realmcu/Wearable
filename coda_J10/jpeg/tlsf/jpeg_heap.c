#include "stdio.h"
#include "tlsf.h"
#include "jpeg_heap.h"
#include "trace.h"

static uint8_t __attribute__((aligned(4)))  mem_array[TLSF_MEM_SIZE];
static tlsf_t jpg_tlsf = NULL;

uint8_t *jpeg_heap_get_mem()
{
    return mem_array;
}

void jpg_tlsf_init(void)
{
    jpg_tlsf = tlsf_create_with_pool(mem_array, TLSF_MEM_SIZE);
}

void *jpg_malloc(size_t size)
{
    void *ptr = NULL;

    ptr = tlsf_malloc(jpg_tlsf, size);
    DBG_DIRECT("--->jpg malloc 0x%x %d", ptr, size);
    if (!ptr)
    {
        DBG_DIRECT("jpg_malloc assert!");
        while (1);
    }
    return ptr;
}

void jpg_free(void *ptr)
{
    DBG_DIRECT("--->jpg_free 0x%x", ptr);
    tlsf_free(jpg_tlsf, ptr);
}

