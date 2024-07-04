/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_lib.h
* @brief    This file provides common header file for SD library.
* @details
* @author   elliot_chen
* @date     2021-01-29
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_LIB_H
#define __SD_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd.h"
#include "sd_def.h"
#include "sd_if.h"
#include "sdh_pwr.h"
#include "sdh_int.h"
#ifdef ASYNC_MODE_ENABLE
#include "sd_os.h"
#endif
/* Defines ------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*__SD_LIB_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

