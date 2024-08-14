
#ifndef ___JPEG_HEAP__H
#define ___JPEG_HEAP__H
#include "stdio.h"
#include "tlsf.h"
#include "jputypes.h"

#define TLSF_MEM_SIZE (89 * 1024)

// typedef unsigned char  uint8_t;

typedef struct
{
    void *ptr;
    uint32_t sz;
    uint8_t align;
} J_HEAP_BUFF_t;




uint8_t *jpeg_heap_get_mem();
void jpg_tlsf_init(void);
void *jpg_malloc(size_t size);
void *jpg_malloc_align(size_t size, uint8_t align);
void jpg_free(void *ptr);

#endif