/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd.h
* @brief    This file provides application functions for sd card or emmc card library..
* @details
* @author   elliot_chen
* @date     2021-02-01
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_APP_H
#define __SD_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/** @addtogroup SD SD
  * @brief SD driver module
  * @{
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/

/** @defgroup SD_Exported_constants SD Exported Constants
  * @{
  */

typedef enum
{
    GROUP_0,
    GROUP_1,
    GROUP_MAX,
} T_SDIO_PIN_GROUP;

typedef enum
{
    SD_IF_NONE    = 0x00,
    SD_IF_SD_CARD = 0x01,
    SD_IF_MMC     = 0x02,
} T_SD_IF_TYPE;

typedef enum
{
    SD_BUS_WIDTH_1B,
    SD_BUS_WIDTH_4B = 2,
    SD_BUS_WIDTH_MAX
} T_SD_BUS_WIDTH;

typedef enum
{
    /*80M 40M only 8773do support*/
    SD_BUS_CLK_80M = 0x100,
    SD_BUS_CLK_40M = 0x101,

    SD_BUS_CLK_20M = 0x00,
    SD_BUS_CLK_10M = 0x01,
    SD_BUS_CLK_5M = 0x02,
    SD_BUS_CLK_2M5 = 0x04,
    SD_BUS_CLK_1M25 = 0x08,
    SD_BUS_CLK_625K = 0x10,
    SD_BUS_CLK_312K5 = 0x20,
    SD_BUS_CLK_156K25 = 0x40,
    SD_BUS_CLK_78K125 = 0x80,

} T_SD_BUS_CLK_FREQ;

typedef enum
{
    SDH_CLK,
    SDH_CMD,
    SDH_D0,
    SDH_D1,
    SDH_D2,
    SDH_D3,
    SDH_WT_PROT,
    SDH_CD,
} T_SD_PIN_NUM;

/** End of group SD_Exported_constants
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/


/** @defgroup SD_Exported_Types SD Exported Types
  * @{
  */

typedef void(*RW_DONE_CBACK)(void *);

typedef struct
{
    uint8_t     sd_if_type: 2;
    uint8_t     rsv0: 2;
    uint8_t     sd_power_en: 1;
    uint8_t     sd_power_high_active: 1;
    uint8_t     sdh_bus_width: 2;

    uint8_t     sdh_group: 2;
    uint8_t     sd_debug_en: 1;
    uint8_t     rsv1: 5;
    uint16_t    sd_bus_clk_sel;
    uint8_t     sd_power_pin;
    RW_DONE_CBACK rw_cback;
    void       *rw_user_data;
} T_SD_CONFIG;

/** End of group SD_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/


/** @defgroup SD_Exported_Functions SD Exported Functions
  * @{
  */

void sd_board_init(void);
void sd_config_init(T_SD_CONFIG *app_sd_cfg);
uint32_t sd_card_init(void);
void sd_enter_dlps(bool disk_powerdown);
void sd_exit_dlps(bool disk_powerdown);
void sd_print_binary_data(uint8_t *p, uint32_t len);
uint32_t sd_erase(uint32_t start_addr, uint32_t end_addr);
uint32_t sd_read(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num);
uint32_t sd_write(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num);
uint32_t sd_set_block_len(uint32_t block_len);

#ifdef __cplusplus
}
#endif

#endif /*__SD_APP_H*/

/** @} */ /* End of group SD_Exported_Functions */
/** @} */ /* End of group SD */

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

