//------------------------------------------------------------------------------
// File: jdi.c
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
//#include "../jdi.h"
//#include "../../include/jpulog.h"
#include "jdi.h"
#include "trace.h"
#include "jpeg_heap.h"

#define JPU_BIT_REG_SIZE        0x300
// #    define JPU_BIT_REG_BASE     (0x10000000 + 0x3000)
#define JPU_BIT_REG_BASE        (0x0)

#define JDI_DRAM_PHYSICAL_BASE  0x00
// #define JDI_DRAM_PHYSICAL_SIZE   (128*1024*1024)
#define JDI_DRAM_PHYSICAL_SIZE  (240*1024) // 240 k
#define JDI_SYSTEM_ENDIAN   JDI_LITTLE_ENDIAN

typedef struct jpu_buffer_t jpudrv_buffer_t;

typedef struct jpu_buffer_pool_t
{
    jpudrv_buffer_t jdb;
    int inuse;
} jpu_buffer_pool_t;

static int s_jpu_fd;
static jpu_instance_pool_t *s_pjip;
static jpu_instance_pool_t s_jip;
static int s_task_num;
static int s_clock_state;
static jpudrv_buffer_t s_jdb_video_memory;
static jpudrv_buffer_t s_jdb_register;
static jpu_buffer_pool_t s_jpu_buffer_pool[MAX_JPU_BUFFER_POOL];
static int s_jpu_buffer_pool_count;


static int jpu_swap_endian(unsigned char *data, int len, int endian);

int jdi_probe()
{
    int ret;

    ret = jdi_init();
    jdi_release();

    return ret;
}

// uint8_t video_mm[JDI_DRAM_PHYSICAL_SIZE];

int jdi_init()
{
    int ret;

    if (s_jpu_fd != -1 && s_jpu_fd != 0x00)
    {
        s_task_num++;
        return 0;
    }

    s_jpu_fd = 1;

    memset((void *)&s_jpu_buffer_pool, 0x00, sizeof(jpu_buffer_pool_t)*MAX_JPU_BUFFER_POOL);
    s_jpu_buffer_pool_count = 0;

    // DBG_DIRECT("s_jpu_fd %d\n", s_jpu_fd);
    if (!(s_pjip = jdi_get_instance_pool()))
    {
        DBG_DIRECT("[jdi] fail to create instance pool for saving context \n");
        goto ERR_JDI_INIT;
    }
    memset(s_pjip, 0x00, 16);
    // ?
    // s_jdb_video_memory.phys_addr = JDI_DRAM_PHYSICAL_BASE;
    // s_jdb_video_memory.size = JDI_DRAM_PHYSICAL_SIZE;

    // if((uint32_t)video_mm % 8)
    // {
    //  uint8_t offset = (uint32_t)video_mm % 8;
    //  s_jdb_video_memory.phys_addr = (unsigned long)video_mm + offset;
    //  s_jdb_video_memory.size = JDI_DRAM_PHYSICAL_SIZE - offset;
    // }
    // else
    // {
    //  s_jdb_video_memory.phys_addr = (unsigned long)video_mm;
    //  s_jdb_video_memory.size = JDI_DRAM_PHYSICAL_SIZE;
    // }

    DBG_DIRECT("s_jpu_fd %d\n", s_jpu_fd);
    uint8_t *pmem = jpeg_heap_get_mem();
    DBG_DIRECT("s_jpu_fd %d\n", s_jpu_fd);
    s_jdb_video_memory.phys_addr = (unsigned long)pmem;
    s_jdb_video_memory.size = TLSF_MEM_SIZE;
    DBG_DIRECT("pmem 0x%x, 0x%x \n", pmem, s_jdb_video_memory.phys_addr);


// DBG_DIRECT("%s %d: phys_addr 0x%x, %d \n", __FUNCTION__, __LINE__, s_jdb_video_memory.phys_addr, s_jdb_video_memory.size);
//  DBG_DIRECT("phys_addr 0x%x, %d \n", s_jdb_video_memory.phys_addr, s_jdb_video_memory.size);
    DBG_DIRECT("s_jpu_fd %d\n", s_jpu_fd);

    s_jdb_register.phys_addr = JPU_BIT_REG_BASE;
    s_jdb_register.virt_addr = JPU_BIT_REG_BASE;
    s_jdb_register.size = JPU_BIT_REG_SIZE;

    jdi_set_clock_gate(1);

    s_task_num++;
    DBG_DIRECT("s_jpu_fd %d\n", s_jpu_fd);
    return s_jpu_fd;

ERR_JDI_INIT:

    jdi_release();
    return -1;
}

int jdi_release()
{

    if (s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return 0;
    }

    if (jdi_lock() < 0)
    {
        DBG_DIRECT("[jdi] fail to handle lock function\n");
        return -1;
    }

    if (s_task_num > 1) // means that the opened instance remains
    {
        s_task_num--;
        jdi_unlock();
        return 0;
    }

    s_task_num--;

    memset(&s_jdb_register, 0x00, sizeof(jpudrv_buffer_t));

    if (s_jpu_fd != -1 && s_jpu_fd != 0x00)
    {
        s_jpu_fd = -1;
    }

    s_pjip = NULL;

    jdi_unlock();


    return 0;
}


jpu_instance_pool_t *jdi_get_instance_pool()
{
    if (!s_pjip)
    {
        s_pjip = &s_jip;

        memset(s_pjip, 0x00, sizeof(jpu_instance_pool_t));
    }

    return (jpu_instance_pool_t *)s_pjip;
}


int jdi_open_instance(unsigned long instIdx)
{
    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return -1;
    }

    s_pjip->jpu_instance_num++;

    return 0;
}

int jdi_close_instance(unsigned long instIdx)
{
    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return -1;
    }

    s_pjip->jpu_instance_num--;

    return 0;
}


int jdi_get_instance_num()
{
    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return -1;
    }

    return s_pjip->jpu_instance_num;
}

int jdi_hw_reset()
{
    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return -1;
    }

    // to do any action for hw reset

    return 0;
}



int jdi_lock()
{

    return 0;
}
void jdi_unlock()
{

}

void jdi_write_register(unsigned int addr, unsigned int data)
{
    unsigned long *reg_addr;

    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        DBG_DIRECT("[E] REG NOT WR !!");
        return ;
    }

    // reg_addr = (unsigned long *)(addr + (unsigned long)s_jdb_register.virt_addr);
    reg_addr = (unsigned long *)(addr);
    *(volatile unsigned long *)reg_addr = data;

}

unsigned int jdi_read_register(unsigned int addr)
{
    unsigned long *reg_addr;

    // reg_addr = (unsigned long *)(addr + (unsigned long)s_jdb_register.virt_addr);
    reg_addr = (unsigned long *)(addr);
    return *(volatile unsigned long *)reg_addr;
}

int jdi_write_memory(unsigned int addr, unsigned char *data, int len, int endian)
{
    jpudrv_buffer_t jdb = {0, };
    unsigned long offset;
    int i;


    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return -1;
    }

    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        DBG_DIRECT("addr 0x%x %d-- jdi_write_memory i %d 0x%x inuse %d address 0x%x %d\n", addr, len, i,
                   &(s_jpu_buffer_pool[i].jdb), s_jpu_buffer_pool[i].inuse, \
                   s_jpu_buffer_pool[i].jdb.phys_addr, s_jpu_buffer_pool[i].jdb.size);
        if (s_jpu_buffer_pool[i].inuse == 1)
        {
            jdb = s_jpu_buffer_pool[i].jdb;
            if (addr >= jdb.phys_addr && addr < (jdb.phys_addr + jdb.size))
            {
                break;
            }
        }
    }

    if (!jdb.size)
    {
        DBG_DIRECT("address 0x%08x is not mapped address!!!\n", addr);
        return -1;
    }

    // offset = addr - (unsigned long)jdb.phys_addr;


    jpu_swap_endian(data, len, endian);

    // memcpy((void *)((unsigned long)jdb.virt_addr + offset), data, len);
    DBG_DIRECT("WR cp 0x%x -> 0x%x 0x%x(%d)", data, (void *)((unsigned long)jdb.phys_addr), addr, len);
    // memcpy((void *)((unsigned long)jdb.phys_addr), data, len);
    memcpy((void *)addr, data, len);

    return len;
}

int jdi_read_memory(unsigned int addr, unsigned char *data, int len, int endian)
{
    jpudrv_buffer_t jdb = {0};
    unsigned long offset;
    int i;

    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return -1;
    }

    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        if (s_jpu_buffer_pool[i].inuse == 1)
        {
            jdb = s_jpu_buffer_pool[i].jdb;
            if (addr >= jdb.phys_addr && addr < (jdb.phys_addr + jdb.size))
            {
                break;
            }
        }
    }

    if (!jdb.size)
    {
        DBG_DIRECT("address 0x%08x is not mapped address!!!\n", addr);
        return -1;
    }


    // offset = addr - (unsigned long)jdb.phys_addr;


    DBG_DIRECT("RD: cp 0x%x 0x%x -> 0x%x (%d)", (const void *)((unsigned long)jdb.virt_addr), addr,
               data, len);
    // memcpy(data, (const void *)((unsigned long)jdb.virt_addr + offset), len);
    memcpy(data, (const void *)((unsigned long)jdb.virt_addr), len);

    jpu_swap_endian(data, len,  endian);


    return len;
}

// static uint8_t encoder_jpg[40 * 1024] __attribute__((section(".ARM.__at_0x20200000")));
// static uint8_t __attribute__((aligned(8)))   encoder_jpg[40 * 1024];
int jdi_register_dma_memory(jpu_buffer_t *vb, void *addr)
{
    int i;
    int ret = 0;
    unsigned long offset;
    jpudrv_buffer_t jdb = {0, };
    static uint8_t cnt = 0;

    cnt ++;
    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        DBG_DIRECT("jdi_allocate_dma_memory FAILD %d!!\n", cnt);
        return -1;
    }

    jdb.size = vb->size;
    // jdb.phys_addr = (unsigned long)jmem_alloc(&s_pjip->vmem, jdb.size, 0);

    // jdb.phys_addr = (unsigned long)jpg_malloc_align(jdb.size, 8);
//    memset(encoder_jpg, 0, sizeof(encoder_jpg));
//    jdb.phys_addr = (unsigned long)encoder_jpg;
    jdb.phys_addr = (unsigned long)addr;
    DBG_DIRECT("jdi_allocate_dma_memory get 0x%x,0x%x %d\n", jdb.phys_addr, jdb.phys_addr + jdb.size,
               jdb.size + 7);

    if (jdb.phys_addr == (unsigned long) - 1)
    {
        return -1;    // not enough memory
    }

    offset = (unsigned long)(jdb.phys_addr - s_jdb_video_memory.phys_addr);
    jdb.base = (unsigned long)s_jdb_video_memory.base + offset;
    jdb.virt_addr = jdb.phys_addr;

    vb->phys_addr = (unsigned long)jdb.phys_addr;
    vb->base = (unsigned long)jdb.base;
    vb->virt_addr = (unsigned long)jdb.phys_addr;


    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        if (s_jpu_buffer_pool[i].inuse == 0)
        {
            s_jpu_buffer_pool[i].jdb = jdb;
            s_jpu_buffer_pool_count++;
            s_jpu_buffer_pool[i].inuse = 1;
            DBG_DIRECT("jdi_allocate_dma_memory i %d 0x%x 0x%x sz %d\n", i, &(s_jpu_buffer_pool[i].jdb),
                       s_jpu_buffer_pool[i].jdb.phys_addr, s_jpu_buffer_pool[i].jdb.size);
            break;
        }
    }

    return 0;
}

int jdi_allocate_dma_memory(jpu_buffer_t *vb)
{
    int i;
    int ret = 0;
    unsigned long offset;
    jpudrv_buffer_t jdb = {0, };
    static uint8_t cnt = 0;

    cnt ++;
    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        DBG_DIRECT("jdi_allocate_dma_memory FAILD %d!!\n", cnt);
        return -1;
    }

    jdb.size = vb->size;
    // jdb.phys_addr = (unsigned long)jmem_alloc(&s_pjip->vmem, jdb.size, 0);

    jdb.phys_addr = (unsigned long)jpg_malloc_align(jdb.size, 8);
    DBG_DIRECT("jdi_allocate_dma_memory get 0x%x,0x%x %d\n", jdb.phys_addr, jdb.phys_addr + jdb.size,
               jdb.size + 7);

    if (jdb.phys_addr == (unsigned long) - 1)
    {
        return -1;    // not enough memory
    }

    offset = (unsigned long)(jdb.phys_addr - s_jdb_video_memory.phys_addr);
    jdb.base = (unsigned long)s_jdb_video_memory.base + offset;
    jdb.virt_addr = jdb.phys_addr;

    vb->phys_addr = (unsigned long)jdb.phys_addr;
    vb->base = (unsigned long)jdb.base;
    vb->virt_addr = (unsigned long)jdb.phys_addr;


    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        if (s_jpu_buffer_pool[i].inuse == 0)
        {
            s_jpu_buffer_pool[i].jdb = jdb;
            s_jpu_buffer_pool_count++;
            s_jpu_buffer_pool[i].inuse = 1;
            DBG_DIRECT("jdi_allocate_dma_memory i %d 0x%x 0x%x sz %d\n", i, &(s_jpu_buffer_pool[i].jdb),
                       s_jpu_buffer_pool[i].jdb.phys_addr, s_jpu_buffer_pool[i].jdb.size);
            break;
        }
    }

    return 0;
}


int jdi_register_extern_memory(jpu_buffer_t *vb, void *addr, size_t sz)
{
    int i;
    int ret = 0;
    unsigned long offset;
    jpudrv_buffer_t jdb = {0, };

    jdb.size = sz;
    // jdb.phys_addr = (unsigned long)jmem_alloc(&s_pjip->vmem, jdb.size, 0);

    jdb.phys_addr = (unsigned long)addr;
    DBG_DIRECT("jdi_register_extern_memory get 0x%x,0x%x %d\n", jdb.phys_addr, jdb.phys_addr + jdb.size,
               jdb.size);

    offset = (unsigned long)(jdb.phys_addr - s_jdb_video_memory.phys_addr);
    jdb.base = (unsigned long)s_jdb_video_memory.base + offset;
    jdb.virt_addr = jdb.phys_addr;

    vb->phys_addr = (unsigned long)jdb.phys_addr;
    vb->base = (unsigned long)jdb.base;
    vb->virt_addr = (unsigned long)jdb.phys_addr;


    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        if (s_jpu_buffer_pool[i].inuse == 0)
        {
            s_jpu_buffer_pool[i].jdb = jdb;
            s_jpu_buffer_pool_count++;
            s_jpu_buffer_pool[i].inuse = 1;
            DBG_DIRECT("jdi_register_extern_memory i %d 0x%x 0x%x sz %d\n", i, &(s_jpu_buffer_pool[i].jdb),
                       s_jpu_buffer_pool[i].jdb.phys_addr, s_jpu_buffer_pool[i].jdb.size);
            break;
        }
    }

    return 0;
}

void jdi_free_dma_memory(jpu_buffer_t *vb)
{
    int i;
    int ret = 0;
    jpudrv_buffer_t jdb = {0, };


    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return ;
    }

    if (vb->size == 0)
    {
        return ;
    }



    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        if (s_jpu_buffer_pool[i].jdb.phys_addr == vb->phys_addr)
        {
            s_jpu_buffer_pool[i].inuse = 0;
            s_jpu_buffer_pool_count--;
            jdb = s_jpu_buffer_pool[i].jdb;
            break;
        }
    }

    if (!jdb.size)
    {
        DBG_DIRECT("[JDI] invalid buffer to free address = 0x%x\n", (int)jdb.virt_addr);
        return ;
    }

    // jmem_free(&s_pjip->vmem, (unsigned long)jdb.phys_addr, 0);
    jpg_free((void *)jdb.phys_addr);
    memset(vb, 0, sizeof(jpu_buffer_t));
}

void jdi_clean_dma_memory(jpu_buffer_t *vb)
{
    int i;
    int ret = 0;
    jpudrv_buffer_t jdb = {0, };


    if (!s_pjip || s_jpu_fd == -1 || s_jpu_fd == 0x00)
    {
        return ;
    }

    if (vb->size == 0)
    {
        return ;
    }



    for (i = 0; i < MAX_JPU_BUFFER_POOL; i++)
    {
        if (s_jpu_buffer_pool[i].jdb.phys_addr == vb->phys_addr)
        {
            s_jpu_buffer_pool[i].inuse = 0;
            s_jpu_buffer_pool_count--;
            jdb = s_jpu_buffer_pool[i].jdb;
            break;
        }
    }

    if (!jdb.size)
    {
        DBG_DIRECT("[JDI] invalid buffer to free address = 0x%x\n", (int)jdb.virt_addr);
        return ;
    }

    // jmem_free(&s_pjip->vmem, (unsigned long)jdb.phys_addr, 0);
    // jpg_free((void *)jdb.phys_addr);
    memset(vb, 0, sizeof(jpu_buffer_t));
}

int jdi_set_clock_gate(int enable)
{
    s_clock_state = enable;
    return 0;
}


int jdi_get_clock_gate()
{

    return s_clock_state;
}


int jdi_wait_interrupt(int timeout)
{
    //int count = 0;

    while (1)
    {
        if (jdi_read_register(MJPEG_PIC_STATUS_REG))
        {
            break;
        }

        //Sleep(1); // 1ms sec
        //if (count++ > timeout)
        //  return -1;
    }

    return 0;
}


int jpu_swap_endian(unsigned char *data, int len, int endian)
{
    unsigned long *p;
    unsigned long v1, v2, v3;
    int i;
    int swap = 0;
    p = (unsigned long *)data;

    if (endian == JDI_SYSTEM_ENDIAN)
    {
        swap = 0;
    }
    else
    {
        swap = 1;
    }

    if (swap)
    {
        if (endian == JDI_LITTLE_ENDIAN ||
            endian == JDI_BIG_ENDIAN)
        {
            for (i = 0; i < len / 4; i += 2)
            {
                v1 = p[i];
                v2  = (v1 >> 24) & 0xFF;
                v2 |= ((v1 >> 16) & 0xFF) <<  8;
                v2 |= ((v1 >>  8) & 0xFF) << 16;
                v2 |= ((v1 >>  0) & 0xFF) << 24;
                v3 =  v2;
                v1  = p[i + 1];
                v2  = (v1 >> 24) & 0xFF;
                v2 |= ((v1 >> 16) & 0xFF) <<  8;
                v2 |= ((v1 >>  8) & 0xFF) << 16;
                v2 |= ((v1 >>  0) & 0xFF) << 24;
                p[i]   =  v2;
                p[i + 1] = v3;
            }
        }
        else
        {
            int sys_endian = JDI_SYSTEM_ENDIAN;
            int swap4byte = 0;
            swap = 0;
            if (endian == JDI_32BIT_LITTLE_ENDIAN)
            {
                if (sys_endian == JDI_BIG_ENDIAN)
                {
                    swap = 1;
                }
            }
            else
            {
                if (sys_endian == JDI_BIG_ENDIAN)
                {
                    swap4byte = 1;
                }
                else if (sys_endian == JDI_LITTLE_ENDIAN)
                {
                    swap4byte = 1;
                    swap = 1;
                }
                else
                {
                    swap = 1;
                }
            }
            if (swap)
            {
                for (i = 0; i < len / 4; i++)
                {
                    v1 = p[i];
                    v2  = (v1 >> 24) & 0xFF;
                    v2 |= ((v1 >> 16) & 0xFF) <<  8;
                    v2 |= ((v1 >>  8) & 0xFF) << 16;
                    v2 |= ((v1 >>  0) & 0xFF) << 24;
                    p[i] = v2;
                }
            }
            if (swap4byte)
            {
                for (i = 0; i < len / 4; i += 2)
                {
                    v1 = p[i];
                    v2 = p[i + 1];
                    p[i]   = v2;
                    p[i + 1] = v1;
                }
            }
        }
    }
    return swap;
}
