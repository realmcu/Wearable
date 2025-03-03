/**
**********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_nvic.c
* @brief    This file provides all the NVIC firmware functions.
* @details
* @author   elliot chen
* @date     2015-05-19
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_nvic.h"

/**
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the NVIC_InitStruct.
  * @param  NVIC_InitStruct: pointer to a NVIC_InitTypeDef structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @retval None
  */
void NVIC_Init_rom(NVIC_InitTypeDef *NVIC_InitStruct)
{
    // @Renee, please check it
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));

    if (GET_FIRST_IRQn(NVIC_InitStruct->NVIC_IRQChannel))
    {
        NVIC_InitStruct->NVIC_IRQChannel = GET_FIRST_IRQn(NVIC_InitStruct->NVIC_IRQChannel);
    }

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
    {
        NVIC_ClearPendingIRQ(NVIC_InitStruct->NVIC_IRQChannel);
        NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel,
                         NVIC_InitStruct->NVIC_IRQChannelPriority);
        NVIC_EnableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
    else
    {
        /* Disable the Selected IRQ Channels */
        NVIC_DisableIRQ(NVIC_InitStruct->NVIC_IRQChannel);
    }
}


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/

