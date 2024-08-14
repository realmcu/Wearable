#include "stdio.h"
#include "tlsf.h"
#include "jpeg_heap.h"
#include "trace.h"



#define TLSF_BUFF_NUM_MAX  (16)

static uint8_t __attribute__((aligned(8)))  mem_array[TLSF_MEM_SIZE];

static tlsf_t jpg_tlsf = NULL;


static J_HEAP_BUFF_t jheap_buffer[TLSF_BUFF_NUM_MAX];

uint8_t *jpeg_heap_get_mem()
{
    return mem_array;
}

void jpg_tlsf_init(void)
{
    DBG_DIRECT("jpg_tlsf_init mem_array 0x%x\n", mem_array);
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

void *jpg_malloc_align(size_t size, uint8_t align)
{
    void *ptr = NULL;
    uint8_t i = 0;

    ptr = jpg_malloc((size + align - 1));
    for (i = 0; i < TLSF_BUFF_NUM_MAX; i++)
    {
        if (!jheap_buffer[i].ptr)
        {
            jheap_buffer[i].ptr = ptr;
            jheap_buffer[i].sz = size + align - 1;
            jheap_buffer[i].align = align;
            break;
        }
    }
    if ((uint32_t)ptr % align)
    {
        ptr = (void *)((((uint32_t)ptr + align - 1) / align) * align);
        DBG_DIRECT("---->jpg malloc align i:%d 0x%x %d", i, ptr, size);
    }

    return ptr;
}


void jpg_free(void *ptr)
{
    uint8_t i = 0;
    DBG_DIRECT("--->jpg_free 0x%x", ptr);

    for (i = 0; i < TLSF_BUFF_NUM_MAX; i++)
    {
        if ((ptr > jheap_buffer[i].ptr) && (ptr < jheap_buffer[i].ptr + jheap_buffer[i].align) &&
            (ptr < jheap_buffer[i].ptr + jheap_buffer[i].sz))
        {
            DBG_DIRECT("---->jpg free align i:%d 0x%x %d sz: %d", i, jheap_buffer[i].ptr, jheap_buffer[i].align,
                       jheap_buffer[i].sz);
            ptr = jheap_buffer[i].ptr;
            jheap_buffer[i].ptr = 0;
            jheap_buffer[i].sz = 0;
            jheap_buffer[i].align = 0;
            break;
        }
    }

    tlsf_free(jpg_tlsf, ptr);
}

