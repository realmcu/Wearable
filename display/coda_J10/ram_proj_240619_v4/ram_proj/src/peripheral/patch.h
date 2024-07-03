/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _PATCH_H_
#define _PATCH_H_

#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define PATCH_POINTER_SECTION

typedef void (*VOID_PATCH_FUNC)();
typedef void *(*VOIDPTR_PATCH_FUNC)();
typedef bool (*BOOL_PATCH_FUNC)();
typedef uint8_t (*UINT8_PATCH_FUNC)();
typedef uint16_t (*UINT16_PATCH_FUNC)();
typedef uint32_t (*UINT32_PATCH_FUNC)();


#ifdef __cplusplus
}
#endif

#endif /* _PATCH_H_ */
