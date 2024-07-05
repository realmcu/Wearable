/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_def.h
* @brief    This file provides definition for SD library.
* @details
* @author   elliot_chen
* @date     2021-03-30
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_DEF_H
#define __SD_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_icfg.h"
#include "sd_define.h"
#ifdef SD_MONITOR_EN
#include "sd_monitor.h"
#endif

/* Defines -------------------------------------------------------------------*/
#define SD_BUS_WIDTH_1B                 0x00
#define SD_BUS_WIDTH_4B                 0x02
#define SDH_RSP0                        ((uint32_t)0x00000000)
#define SDH_RSP2                        ((uint32_t)0x00000004)
#define SDH_RSP4                        ((uint32_t)0x00000008)
#define SDH_RSP6                        ((uint32_t)0x0000000C)

typedef struct
{
    T_SD_STATUS(*sd_if_init_dev)(void *p_dev_info);
    T_SD_STATUS(*sd_if_get_dev_info)(void *p_info);
    T_SD_STATUS(*sd_if_read_single_block)(uint32_t sector, uint32_t buf, uint16_t block_size);
    T_SD_STATUS(*sd_if_read_multi_block)(uint32_t sector, uint32_t buf, uint16_t block_size,
                                         uint16_t block_count);
    T_SD_STATUS(*sd_if_write_single_block)(uint32_t sector, uint32_t buf, uint16_t block_size);
    T_SD_STATUS(*sd_if_write_multi_block)(uint32_t sector, uint32_t buf, uint16_t block_size,
                                          uint16_t block_count);
    T_SD_STATUS(*sd_if_earse)(uint32_t start_addr, uint32_t end_addr);
    T_SD_STATUS(*sd_if_wait_card_finish_program)(void);
    T_SD_STATUS(*sd_if_async_read_single_block)(uint32_t sector, uint32_t buf, uint16_t block_size);
    T_SD_STATUS(*sd_if_async_read_multi_block)(uint32_t sector, uint32_t buf, uint16_t block_size,
                                               uint16_t block_count);
    T_SD_STATUS(*sd_if_async_write_single_block)(uint32_t sector, uint32_t buf, uint16_t block_size);
    T_SD_STATUS(*sd_if_async_write_multi_block)(uint32_t sector, uint32_t buf, uint16_t block_size,
                                                uint16_t block_count);
    T_SD_STATUS(*sd_if_set_block_len)(uint32_t block_len);
    void(*sd_if_deinit_dev)(void);
    T_SD_STATUS(*sd_if_set_bus_delay)(void *p_info);
} T_SDIF_PROT_PROP;

#ifdef __cplusplus
}
#endif

#endif /*__SD_DEF_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

