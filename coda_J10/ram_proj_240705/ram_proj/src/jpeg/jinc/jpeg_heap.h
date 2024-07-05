
#ifndef ___JPEG_HEAP__H
#define ___JPEG_HEAP__H
#include "stdio.h"
#include "tlsf.h"

#define TLSF_MEM_SIZE (70 * 1024)

typedef unsigned char  uint8_t;

uint8_t *jpeg_heap_get_mem();
void jpg_tlsf_init(void);
void *jpg_malloc(size_t size);

void jpg_free(void *ptr);

#endif