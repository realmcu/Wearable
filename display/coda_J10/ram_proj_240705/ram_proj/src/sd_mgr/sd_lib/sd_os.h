/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_os.h
* @brief    This file provides sd task for sd library.
* @details
* @author   elliot_chen
* @date     2021-05-11
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_OS_H
#define __SD_OS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include "sd.h"
#ifdef ASYNC_MODE_ENABLE
#include "app_msg.h"
#include "trace.h"

/* Defines ------------------------------------------------------------------*/
typedef enum
{
    IO_MSG_TYPE_SD,
} T_SD_MSG_TYPE;

/* Exported functions -------------------------------------------------------*/
uint8_t sd_create_queue(void);
uint8_t send_sdh_msg_to_task(T_IO_MSG *p_msg);
uint8_t recv_sdh_msg(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /*__SD_OS_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

