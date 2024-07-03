/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_if.h
* @brief    This file provides operation interface for SD library.
* @details
* @author   elliot_chen
* @date     2021-02-02
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_IF_H
#define __SD_IF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_def.h"

/* Defines ------------------------------------------------------------------*/

/* Exported functions -------------------------------------------------------*/
T_SD_STATUS sd_if_init(T_SD_IF_TYPE sd_if_type);
void sd_if_deinit(void);
T_SD_STATUS sd_if_read(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num);
T_SD_STATUS sd_if_write(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num);
T_SD_STATUS sd_if_erase(uint32_t start_addr, uint32_t end_addr);
T_SD_IF_TYPE sd_if_get_dev_type(void);
uint64_t sd_if_get_dev_capacity(void);
uint32_t sd_if_get_dev_block_num(void);
uint32_t sd_if_get_dev_block_size(void);
T_SD_STATUS sd_if_set_block_len(uint32_t block_len);
T_SD_STATUS sd_if_check_program_status(void);
void sd_if_pin_group_cfg(uint32_t group_index);
T_SD_STATUS sd_if_sdh_pow_on(void);
void sd_if_sdh_clk_cmd(bool NewState);

#ifdef ASYNC_MODE_ENABLE
T_SD_STATUS sd_if_async_read(uint64_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num);
T_SD_STATUS sd_if_async_write(uint64_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num);
#endif
#ifdef __cplusplus
}
#endif

#endif /*__SD_IF_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

