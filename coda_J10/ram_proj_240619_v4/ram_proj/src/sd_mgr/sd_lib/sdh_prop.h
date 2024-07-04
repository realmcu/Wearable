/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdh_prop.h
* @brief    This file provides sd host property firmware functions for sd library.
* @details
* @author   elliot_chen
* @date     2021-01-29
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SDH_PROP_H
#define __SDH_PROP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_def.h"

/* Defines ------------------------------------------------------------------*/
typedef struct
{
    uint32_t cmd;
    uint32_t argument;
    bool with_data;
    bool cmd_idx_check;
    bool cmd_crc_check;
    uint32_t resp_len;
} T_SDH_CMD_CFG;

typedef struct
{
    bool is_read_dir;
    uint32_t buf;
    uint16_t blk_size;
    uint16_t blk_count;
} T_SDH_DATA_CFG;

typedef struct
{
    T_SD_STATUS(*SDH_WaitCmdLineIdle)(void);
    T_SD_STATUS(*SDH_WaitCmdDatComplete)(void);
    T_SD_STATUS(*SDH_CheckCmd)(uint32_t cmd);
    uint32_t (*SDH_GetResponse)(uint32_t SDIO_RSP);
    T_SD_STATUS(*SDH_SetBusWide)(uint32_t bus_wide);
    void (*SDH_SendCommand)(T_SDH_CMD_CFG *p_sdh_cmd);
    T_SD_STATUS(*SDH_DataConfig)(T_SDH_DATA_CFG *p_sdh_data);
} T_SD_HOST_PROP;

/* Exported variable --------------------------------------------------------*/
extern const T_SD_HOST_PROP *const p_sd_host_prop;

#ifdef __cplusplus
}
#endif

#endif /*__SDH_PROP_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

