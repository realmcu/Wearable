/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    mem_config.h
  * @brief   Memory Configuration
  * @date    2017.6.6
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __MEM_CONFIG__
#define __MEM_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

#include "address_map.h"

/** @defgroup MEM_CONFIG Memory Configure
  * @brief Memory configuration for user application
  * @{
  */
#define CM33_RAM_ADDR    CM33_RAM_BASE
#define CM33_RAM_SIZE    (128*1024)
#define DATA_SRAM0_ADDR  DATA_SRAM0_BASE
#define DATA_SRAM0_SIZE  (128*1024)
#define DATA_RAM_SIZE    DATA_SRAM0_SIZE
#define DSP_SHM_ADDR     DATA_SRAM1_BASE
#define DSP_SHM_SIZE     (448 * 1024)

#define CODE_RAM_SIZE    CM33_RAM_SIZE

/*============================================================================*
 *                        Constants
 *============================================================================*/
/** @defgroup MEM_CONFIG_Exported_Constant Memory Configure Exported Constants
  * @{
  */
/* --------------------CONFIG BY USER------------------------- */

/** @brief  following is for application define
    @note  SUM OF APP_GLOBAL_SIZE PLUS HEAP_DATA_OFF_SIZE SHOULD BE SMALLER THAN DTCM1_SIZE.
    APP_RAM_TEXT_SIZE SHOULD BE SMALLER THAN (ITCM1_END_ADDR - APP_RAM_TEXT_ADDR).
    */
#define APP_RAM_TEXT_SIZE           (8*1024)
#define APP_GLOBAL_SIZE             (10*1024)
#define HEAP_DATA_OFF_SIZE          (110 * 1024)

/* --------------------CONFIG BY USER -------------------------*/

/* --------------------The following macros should not be modified!------------------------- */

#define DATA_SIZE_FOR_RANDOM_SEED   16

#define APP_TRACE_ADR               0x08800000
#define APP_TRACE_SIZE              (4*1024*1024)

/** @brief  data ram layout */
#define HEAP_DATA_ON_SIZE           (DATA_RAM_SIZE - APP_GLOBAL_SIZE - HEAP_DATA_OFF_SIZE)


#define APP_GLOBAL_ADDR             DATA_SRAM0_ADDR
#define HEAP_DATA_ON_ADDR           (APP_GLOBAL_ADDR + APP_GLOBAL_SIZE)
#define HEAP_DATA_OFF_ADDR          (HEAP_DATA_ON_ADDR + HEAP_DATA_ON_SIZE)

/* Instruction RAM(CM33_RAM):    Patch used only
example:
    1) platform ext           320 bytes
    2) syspatch               (14K-320) bytes
    3) lowerstack ext         256 bytes
    4) stackpatch             (20K-256) bytes
    5) app                    8K bytes
    6) HEAP_CM33_RAM          86k bytes
*/

#define MPLOADER_RAM_PATCH_START_ADDR   CM33_RAM_ADDR
#define PLATFORM_EXT_RAM_START_ADDR     CM33_RAM_ADDR
#define PLATFORM_EXT_IMAGE_RAM_SIZE     (320)  /*aligned by 16 bytes*/
#define TOTAL_SYS_PATCH_RAM_SIZE        (14*1024)
#define SYS_PATCH_RAM_START_ADDR        (PLATFORM_EXT_RAM_START_ADDR + PLATFORM_EXT_IMAGE_RAM_SIZE)
#define SYS_PATCH_RAM_SIZE              (TOTAL_SYS_PATCH_RAM_SIZE - PLATFORM_EXT_IMAGE_RAM_SIZE)
#define STACK_EXT_RAM_START_ADDR        (SYS_PATCH_RAM_START_ADDR + SYS_PATCH_RAM_SIZE)
#define STACK_EXT_IMAGE_RAM_SIZE        (256)  /*aligned by 16 bytes*/
#define TOTAL_STACK_PATCH_RAM_SIZE      (20*1024)
#define STACK_PATCH_RAM_START_ADDR      (STACK_EXT_RAM_START_ADDR + STACK_EXT_IMAGE_RAM_SIZE)
#define STACK_PATCH_RAM_SIZE            (TOTAL_STACK_PATCH_RAM_SIZE - STACK_EXT_IMAGE_RAM_SIZE)
#define APP_RAM_TEXT_ADDR               (STACK_PATCH_RAM_START_ADDR + STACK_PATCH_RAM_SIZE)
#define HEAP_CM33_RAM_ADDR              (APP_RAM_TEXT_ADDR + APP_RAM_TEXT_SIZE)

/** @brief  boundary check */

#if(( APP_GLOBAL_SIZE + HEAP_DATA_OFF_SIZE) > DATA_RAM_SIZE)
#error " SUM OF APP_GLOBAL_SIZE PLUSHEAP_DATA_OFF_SIZE SHOULD BE SMALLER THAN DTCM1_SIZE"
#endif

#if(( APP_RAM_TEXT_SIZE) > (PLATFORM_EXT_RAM_START_ADDR + CODE_RAM_SIZE - APP_RAM_TEXT_ADDR))
#error "APP_RAM_TEXT_SIZE SHOULD BE SMALLER THAN (ITCM1_END_ADDR - APP_RAM_TEXT_ADDR)"
#endif
/** @} */ /* End of group MEM_CONFIG_Exported_Constents */



#ifdef __cplusplus
}
#endif


/** @} */ /* End of group MEM_CONFIG */

#endif

