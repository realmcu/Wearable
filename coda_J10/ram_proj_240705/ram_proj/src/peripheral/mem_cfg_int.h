/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      mem_cfg_int.h
   * @brief     memory config header file for internal project
   * @details
   * @author
   * @date
   * @version
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
  *                   Define to prevent recursive inclusion
  *============================================================================*/
#ifndef _MEM_CFG_INT_H
#define _MEM_CFG_INT_H

#include "address_map.h"

/*============================================================================*
  *                               Header Files
  *============================================================================*/

/*============================================================================*
  *                                   Macros
  *============================================================================*/
#define CM33_MEM_ADDR     CM33_MEM_BASE
#define CM33_MEM_SIZE     (64*1024)
#define EXT0_ADDR         BUFFER_SRAM_BASE
#define EXT0_SIZE         (64*1024)
#define SPIC0_ADDR        SPIC0_MEM_BASE
#define SPIC0_SIZE        (32*1024*1024)
#define SPIC1_ADDR        SPIC1_MEM_BASE
#define SPIC1_SIZE        (32*1024*1024)
#define SPIC2_ADDR        SPIC2_MEM_BASE
#define SPIC2_SIZE        (256*1024*1024)

/* --------------------The following macros should not be modified!------------------------- */
/* Data RAM(CM33_MEM):       64K
example:
   1) rom global:            3K bytes
   2) platform global:       6K bytes
   3) lowerstack global:     22K
   4) upperstack global:     5K
   5) platform ext global    1K bytes
   5) syspatch global        2K bytes
   6) lowerstack ext global  1K bytes
   7) stackpatch global      2K bytes
   6) main stack:            2K
   7) HEAP_DTCM0_heap        25K
*/
/** @brief  data ram start address */
/** @brief  Global size of rom variable in data ram */
#define DATA_RAM_START_ADDR               CM33_MEM_ADDR
#define DATA_RAM_ROM_GLOBAL_SIZE          (3*1024)
#define DATA_RAM_PLATFORM_START_ADDR      (DATA_RAM_START_ADDR + DATA_RAM_ROM_GLOBAL_SIZE)
#define DATA_RAM_PLATFORM_SIZE            (6*1024)
#define DATA_RAM_LOWERSTACK_START_ADDR    (DATA_RAM_PLATFORM_START_ADDR + DATA_RAM_PLATFORM_SIZE)
#define DATA_RAM_LOWERSTACK_SIZE          (22*1024)
#define DATA_RAM_UPPERSTACK_START_ADDR    (DATA_RAM_LOWERSTACK_START_ADDR + DATA_RAM_LOWERSTACK_SIZE)
#define DATA_RAM_UPPERSTACK_SIZE          (5*1024)
#define DATA_RAM_PLATFORM_EXT_START_ADDR  (DATA_RAM_UPPERSTACK_START_ADDR + DATA_RAM_UPPERSTACK_SIZE)
#define DATA_RAM_PLATFORM_EXT_SIZE        (1*1024)
#define DATA_RAM_SYSPATCH_START_ADDR      (DATA_RAM_PLATFORM_EXT_START_ADDR + DATA_RAM_PLATFORM_EXT_SIZE)
#define DATA_RAM_SYSPATCH_SIZE            (2*1024)
#define DATA_RAM_STACK_EXT_START_ADDR     (DATA_RAM_SYSPATCH_START_ADDR + DATA_RAM_SYSPATCH_SIZE)
#define DATA_RAM_STACK_EXT_SIZE           (1*1024)
#define DATA_RAM_STACKPATCH_START_ADDR    (DATA_RAM_STACK_EXT_START_ADDR + DATA_RAM_STACK_EXT_SIZE)
#define DATA_RAM_STACKPATCH_SIZE          (2*1024)
#define MAIN_STACK_START_ADDR             (DATA_RAM_STACKPATCH_START_ADDR + DATA_RAM_STACKPATCH_SIZE)
#define MAIN_STACK_SIZE                   (2*1024)
#define HEAP_CM33_MEM_ADDR                (MAIN_STACK_START_ADDR + MAIN_STACK_SIZE)
#define HEAP_CM33_MEM_SIZE                (CM33_MEM_ADDR + CM33_MEM_SIZE - HEAP_CM33_MEM_ADDR)

#define BOOT_PATCH0_RAM_START_ADDR        (CM33_RAM_BASE)
#define BOOT_PATCH_RAM_SIZE               (BOOTPATCH_NAND_DEFAULT_SIZE)
#define BOOT_PATCH1_RAM_START_ADDR        (BOOT_PATCH0_RAM_START_ADDR + BOOT_PATCH_RAM_SIZE)

/* Buffer RAM(External0):      64K
example:
   1) lowerstack rom global:   2K
   2) HEAP ON:                 46K
   4) Heap OFF:                16K
*/
/** @brief  buffer ram start address */
#define BUFFER_RAM_START_ADDR             BUFFER_SRAM_BASE
#define BUFFER_RAM_ROM_GLOBAL_SIZE        (2*1024)
#define HEAP_BUFFER_ON_ADDR               (BUFFER_RAM_START_ADDR + BUFFER_RAM_ROM_GLOBAL_SIZE)
#define HEAP_BUFFER_ON_SIZE               (46*1024)
#define HEAP_BUFFER_OFF_ADDR              (HEAP_BUFFER_ON_ADDR + HEAP_BUFFER_ON_SIZE)
#define HEAP_BUFFER_OFF_SIZE              (16*1024)
#define LOW_STACK_BUFFER_RAM_START_ADDR   BUFFER_RAM_START_ADDR

#endif

