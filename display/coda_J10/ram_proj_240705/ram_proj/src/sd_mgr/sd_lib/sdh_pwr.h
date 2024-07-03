/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdh_pwr.h
* @brief    This file provides power management for sd library.
* @details  Adjust firmware functions according to the specified sd host driver.
* @author   elliot_chen
* @date     2021-02-02
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SD_PWR_H
#define __SD_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_def.h"
#include "rtl876x.h"
/* Defines ------------------------------------------------------------------*/

/* Exported functions -------------------------------------------------------*/
T_SD_STATUS SDH_PowerOnForIdentifyCard(void);
void SDH_InitForDataTransfer(void);
T_SD_STATUS SDH_PowerOnForDataTransfer(void);
void SDH_ClockCmd(FunctionalState NewState);
void SDH_PinGroupConfig(uint32_t group_index, uint32_t data_width);

#ifdef __cplusplus
}
#endif

#endif /*__SD_APP_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

