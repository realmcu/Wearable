/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      hal_dma.h
* @brief
* @details
* @author
* @date
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _HAL_DMA_
#define _HAL_DMA_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "hal_def.h"

/** @addtogroup HAL_DMA HAL DMA
  * @brief HAL DMA driver module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/


/** @defgroup HAL_DMA_Exported_Types HAL DMA Exported Types
  * @{
  */
typedef enum
{
    HAL_DMA_CH0,
    HAL_DMA_CH1,
    HAL_DMA_CH2,
    HAL_DMA_CH3,
    HAL_DMA_CH4,
    HAL_DMA_CH5,
    HAL_DMA_CH6,
    HAL_DMA_CH7,
#if (CHIP_DMA_CHANNEL_NUM > 8)
    HAL_DMA_CH8,
#endif
#if (CHIP_DMA_CHANNEL_NUM > 15)
    HAL_DMA_CH9,
    HAL_DMA_CH10,
    HAL_DMA_CH11,
    HAL_DMA_CH12,
    HAL_DMA_CH13,
    HAL_DMA_CH14,
    HAL_DMA_CH15,
#endif
    HAL_DMA_CH_MAX
} T_DMA_CH_NUM;

typedef union
{
    struct
    {
        uint32_t DMA_ISR_EVENT_TRANSFOR_DONE   : 1;
        uint32_t DMA_ISR_EVENT_BLOCK           : 1;
        uint32_t DMA_ISR_EVENT_ERROR           : 1;
        uint32_t DMA_ISR_EVENT_RESERVED        : 29;
    };
    uint32_t event;
} T_HAL_DMA_EVENT;

typedef void (*P_HAL_DMA_CALLBACK)(T_HAL_DMA_EVENT event, uint32_t context);

/**
  * @brief  DMA handle definition
  */

/**
  * @brief  DMA callback definition
  */

/** End of group hal_dma_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/



/** @defgroup JAL_DMA_Exported_Functions HAL DMA Exported Functions
  * @{
  */

void hal_dma_update_isr_cb(T_DMA_CH_NUM ch_num, P_HAL_DMA_CALLBACK cb, uint32_t context);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_DMA_ */

/** @} */ /* End of group hal_dma_Exported_Functions */
/** @} */ /* End of group hal_dma */

/******************* (C) COPYRIGHT 2023 Realtek Semiconductor Corporation *****END OF FILE****/

