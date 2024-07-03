/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    System_rtl876x.h
  * @brief   CMSIS Device System Header file
  * @details CMSIS Device System Header File for RTL876x Device Series
  * @author
  * @date    3. March 2016
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
   * ************************************************************************************
   */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef SYSTEM_RTL876X_H
#define SYSTEM_RTL876X_H


/*============================================================================*
 *                      Headers
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>

/** @defgroup SYSTEM_RTL876X_API  System Rtl876x
  * @brief CMSIS API sets for RTL876x Device Series
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                      Types
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_types SYSTEM RTL876X Exported types
  * @{
  */
#define CLOCK_40MHZ 40000000
#define CLOCK_53MHZ 53333333
#define CLOCK_80MHZ 80000000
#define CLOCK_90MHZ 90000000

#define FLASH_START_ADDR 0x2000000
#define RSA_SIG_SIZE 256
#define CMAC_SIZE 16
#define AUTH_HEADER_SIZE (16 * 2 + 256 * 2)
#define IMG_BOOTPATCH_MAX_SECURE_VERSION 16
#define SEC_DBG_LINE(x) (secure_boot_failed_line = (x))

typedef enum _SOCV_KEY_TYPE
{
    SOCV_IMG_AUTH_KEY,
    SOCV_IMG_ENC_KEY,
    OEM_IMG_ENC_KEY,
    FLASH_ENC_KEY,
} SOCV_KEY_TYPE;

typedef enum
{
    BOOT_BYPASS_EFUSE_VALID = 0,
    BOOT_BYPASS_BOOT_ERROR_EFUSE_INVALID,
    BOOT_NORMAL_EFUSE_VALID,
    BOOT_NORMAL_BOOT_ERROR_EFUSE_INVALID,
} BOOT_Type;

typedef enum
{
    LOAD_NONE                   = 0,
    LOAD_LOGICAL_PARA           = 1,
    LOAD_FT_PARA                = 2,
    LOAD_OEM_PARA               = 4,
    LOAD_ALL                    = 7,
} EFUSE_LOAD_Type;

typedef enum
{
    CHECK_NONE                  = 0,
    CHECK_EUID                  = 1,
    CHECK_SECURE_BOOT_CTRL      = 2,
    CHECK_SYSTEM_FUNC_CTRL      = 4,
    CHECK_GENERAL_FUNC_CTRL     = 8,
    CHECK_PRIVATE_FUNC_CTRL     = 16,
    CHECK_KEY_AND_PASSWORD      = 32,
    CHECK_ALL                   = 63,
} EFUSE_CHECK_Type;

typedef enum
{
    SWD_CLOSE = 0,
    SWD_NOT_DEFINE,
    SWD_PROTECT,
    SWD_OPEN,
} SWD_CONTROL_Type;

#define IS_WHOLE_CHIP_RESET                 (!(btaon_fast_read_8b(BTAON_FAST_BOOT) & BIT0))
#define MARK_NOT_FIRST_BOOT()               (btaon_fast_update_8b(BTAON_FAST_BOOT, BIT0, BIT0))
#define INVALID_CNT_THRESHOLD               15
#define SET_IMG_VALID_BITMAP(img_id)        (image_check_bitmap |= BIT(img_id - IMG_OCCD))
#define GET_IMG_VALID_BITMAP(img_id)        ((image_check_bitmap & BIT(img_id - IMG_OCCD)) > 0)
/** @} */ /* End of group SYSTEM_RTL876X_Exported_types */

/*============================================================================*
 *                      Variables
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Variables SYSTEM RTL876X Exported Variables
  * @{
  */
extern unsigned int Image$$ROM_RO$$RO$$Base;
extern uint32_t secure_boot_failed_line;
extern uint32_t secure_boot_image_id;
extern uint32_t ota_bank_num;
extern uint32_t image_check_bitmap;
extern uint8_t SystemInitFailInfo[3];
extern uint32_t ota_header_addr_rom;
extern uint32_t header_size_rom;

static __inline uint32_t get_active_ota_bank_addr(void)
{
    return ota_header_addr_rom;
}

static __inline void copy_data_and_clear_bss(void)
{
    extern unsigned int Image$$RW_SECTION$$RW$$Base;
    extern unsigned int Load$$RW_SECTION$$RW$$Base;
    extern unsigned int Image$$RW_SECTION$$RW$$Length;

    unsigned int *data_image_base = (unsigned int *)& Image$$RW_SECTION$$RW$$Base;
    unsigned int *data_load_base = (unsigned int *)& Load$$RW_SECTION$$RW$$Base;
    unsigned int data_image_length = (unsigned int)& Image$$RW_SECTION$$RW$$Length;

    for (unsigned int i = 0; i < data_image_length / sizeof(unsigned int); i ++)
    {
        data_image_base[i] = data_load_base[i];
    }

    extern unsigned int Image$$RW_SECTION$$ZI$$Base;
    extern unsigned int Image$$RW_SECTION$$ZI$$Length;

    unsigned int *bss_image_base = (unsigned int *)& Image$$RW_SECTION$$ZI$$Base;
    unsigned int bss_image_length = (unsigned int)& Image$$RW_SECTION$$ZI$$Length;

    for (unsigned int i = 0; i < bss_image_length / sizeof(unsigned int); i ++)
    {
        bss_image_base[i] = 0;
    }
}


/** @} */ /* End of group SYSTEM_RTL876X_Exported_Variables */

void Reset_Handler(void);
void HardFault_Handler(void);


#ifdef __cplusplus
}
#endif


/** @} */ /* End of group SYSTEM_RTL876X_API */



#endif /* SYSTEM_RTL876X_H */

