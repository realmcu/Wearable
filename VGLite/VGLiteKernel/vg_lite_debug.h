/****************************************************************************
*
*    Copyright 2012 - 2023 Vivante Corporation, Santa Clara, California.
*    All Rights Reserved.
*
*    Permission is hereby granted, free of charge, to any person obtaining
*    a copy of this software and associated documentation files (the
*    'Software'), to deal in the Software without restriction, including
*    without limitation the rights to use, copy, modify, merge, publish,
*    distribute, sub license, and/or sell copies of the Software, and to
*    permit persons to whom the Software is furnished to do so, subject
*    to the following conditions:
*
*    The above copyright notice and this permission notice (including the
*    next paragraph) shall be included in all copies or substantial
*    portions of the Software.
*
*    THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
*    IN NO EVENT SHALL VIVANTE AND/OR ITS SUPPLIERS BE LIABLE FOR ANY
*    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

#ifndef _vg_lite_debug_h_
#define _vg_lite_debug_h_

#include "vg_lite_kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VGL_DEBUG
#define VGL_TRACE

#ifdef VGL_DEBUG
# define vg_lite_kernel_print(fmt, arg...) printk("[VGL DEBUG] "fmt,##arg)
#else
# define vg_lite_kernel_print(fmt, arg...) do{}while(VG_FALSE)
#endif

#ifdef VGL_TRACE
# define vg_lite_kernel_trace() printk("[VGL TRACE] [%s, %d, %s]", __FUNCTION__, __LINE__, __FILE__)
#else
# define vg_lite_kernel_trace() do{}while(VG_FALSE)
#endif

#define vg_lite_kernel_error(fmt, arg...) printk("[VGL ERROR] "fmt,##arg)
#define vg_lite_kernel_hintmsg(fmt, arg...) printk("[VGL HINTMSG] "fmt,##arg)

#define VG_IS_SUCCESS(error) (error == VG_LITE_SUCCESS)
#define VG_IS_ERROR(error)   (error != VG_LITE_SUCCESS)

#define ONERROR(func) \
    do \
    { \
        error = func; \
        if (VG_IS_ERROR(error)) \
        { \
            vg_lite_kernel_error("%d %s(%d)\n", error, __FUNCTION__, __LINE__); \
            goto on_error; \
        } \
    } \
    while (VG_FALSE)

#define ASSERT(arg) \
    do \
    { \
        if (!(arg)) \
        { \
            error = VG_LITE_INVALID_ARGUMENT;\
            goto on_error; \
        } \
    } \
    while (VG_FALSE)

#ifdef  __cplusplus
}
#endif

#endif
