#include "stdio.h"
#include "tlsf.h"
#include "jpeg_heap.h"
#include "trace.h"
#include "string.h"


#define TLSF_BUFF_NUM_MAX  (16)

static uint8_t  mem_array[TLSF_MEM_SIZE]  __attribute__((section(".ARM.__at_0x20200000")));
//static uint8_t __attribute__((aligned(8)))  mem_array[TLSF_MEM_SIZE] __attribute__((section(".ARM.__at_0x100000")));
// static uint8_t __attribute__((aligned(8)))  mem_array[TLSF_MEM_SIZE];
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
    memset(ptr, 0, size);
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


#include "trace.h"
static uint8_t myisprint(uint8_t ch)
{
    if ((ch >= 0x20) && (ch <= 0x7e)) { return 1; }
    else { return 0; }
}

void log_hex(uint8_t *addr, uint32_t n)
{
    uint32_t i = 0, j = 1;

    if (!addr)
    {
        DBG_DIRECT("LOG NULL!");
    }
    else
    {
        DBG_DIRECT("--------------------- LOG start: 0x%x, len: %d -----------------------------------------",
                   addr, n);
    }
    DBG_DIRECT("\t \t 1 \t 2 \t 3 \t 4 \t 5 \t 6 \t 7 \t 8 \t 9 \t 10 \t|");
    while (i < n)
    {
        DBG_DIRECT("0x%x: %d> \t %02x \t %02x \t %02x \t %02x \t %02x \t %02x \t %02x \t %02x \t %02x \t %02x  \t|%c%c%c%c%c%c%c%c%c%c|",
                   (uint8_t *)addr + (j - 1) * 10, j\
                   , *((uint8_t *)addr + i), *((uint8_t *)addr + i + 1), *((uint8_t *)addr + i + 2),
                   *((uint8_t *)addr + i + 3), *((uint8_t *)addr + i + 4)\
                   , *((uint8_t *)addr + i + 5), *((uint8_t *)addr + i + 6), *((uint8_t *)addr + i + 7),
                   *((uint8_t *)addr + i + 8), *((uint8_t *)addr + i + 9)\
                   , myisprint(*(addr + i)) ? * (addr + i) : '.', myisprint(*(addr + i + 1)) ? * (addr + i + 1) : '.'\
                   , myisprint(*(addr + i + 2)) ? * (addr + i + 2) : '.',
                   myisprint(*(addr + i + 3)) ? * (addr + i + 3) : '.'\
                   , myisprint(*(addr + i + 4)) ? * (addr + i + 4) : '.',
                   myisprint(*(addr + i + 5)) ? * (addr + i + 5) : '.'\
                   , myisprint(*(addr + i + 6)) ? * (addr + i + 6) : '.',
                   myisprint(*(addr + i + 7)) ? * (addr + i + 7) : '.'\
                   , myisprint(*(addr + i + 8)) ? * (addr + i + 8) : '.',
                   myisprint(*(addr + i + 9)) ? * (addr + i + 9) : '.');

        i += 10;
        if (j % 5 == 0) { DBG_DIRECT("\n"); }
        j++;
    }
}