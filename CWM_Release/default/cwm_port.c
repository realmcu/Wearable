#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "os_sched.h"


void CWM_OS_uSleep(uint32_t time)
{
    os_delay(time / 1000);
}

void *CWM_OS_malloc(int size)
{
    void *p = malloc(size);

    return p;
}

void CWM_OS_free(void *ptr)
{
    free(ptr);
}




