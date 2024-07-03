/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_icfg.h
* @brief    This file provides operation interface for SD library.
* @details
* @author   elliot_chen
* @date     2021-02-02
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_ICFG_H
#define __SD_ICFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd.h"

/* Defines ------------------------------------------------------------------*/
/* Configure MCU system clock which used to calculate software timeout, unit: MHz */
#define SD_MCU_CLOCK            ((uint32_t)100U)
/* Configure SD bus cmd line timeout value, unit: ms test max last time is 1290us */
#define SD_CMD_SW_TIMEOUT       ((uint32_t)2000U)
/* Configure SD bus cmd and data line timeout value, unit: ms test max last is 55400us */
#define SD_CMD_DATA_SW_TIMEOUT  ((uint32_t)2000U)

/* Configure SD host bus voltage */
#define SDH_VOLTAGE             SDIO_VOLT_3_3V

/* Enable or disable SD block mode */
#define ASYNC_MODE_ENABLE

#ifdef ASYNC_MODE_ENABLE
/* Enable or disable SD task for sd async mode test */
#define SD_TASK_TEST
#endif
/* Enable or disable SD library log print function */
#define SD_PRINT_LOG
#ifdef SD_PRINT_LOG
#include "trace.h"
#define SD_DBG_BUFFER(MODULE, LEVEL, pFormat, para_num,...)   \
    DBG_BUFFER_##LEVEL(TYPE_BBPRO2, SUBTYPE_FORMAT, MODULE, pFormat, para_num, ##__VA_ARGS__)
#define SD_DBG_DIRECT(...)                do {\
        log_direct(COMBINE_TRACE_INFO(TYPE_BBPRO2, SUBTYPE_DIRECT, 0, 0), __VA_ARGS__);\
    } while (0)
#else
#define SD_DBG_BUFFER(MODULE, LEVEL, pFormat, para_num,...) ((void)0)
#define SD_DBG_DIRECT(...)                ((void)0)
#endif

/* Exported functions -------------------------------------------------------*/
extern T_SD_CONFIG      sd_cfg;

#ifdef __cplusplus
}
#endif

#endif /*__SD_IF_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

