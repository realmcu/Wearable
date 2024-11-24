
#ifndef ___JPEG_HEAP__H
#define ___JPEG_HEAP__H
#include "stdio.h"
#include "tlsf.h"
#include "jputypes.h"

#define TLSF_MEM_SIZE (110 * 1024)

// typedef unsigned char  uint8_t;

typedef struct
{
    void *ptr;
    uint32_t sz;
    uint8_t align;
} J_HEAP_BUFF_t;


#define JDATA_RAM_ADDR      ((void *)0x20200000)
#define JPSRAM_ADDR         ((void *)0x22000000)


uint8_t *jpeg_heap_get_mem();
void jpg_tlsf_init(void);
void jpg_tlsf_init_psrm(void);
void *jpg_malloc(size_t size);
void *jpg_malloc_align(size_t size, uint8_t align);
void jpg_free(void *ptr);
void log_hex(uint8_t *addr, uint32_t n);


#endif