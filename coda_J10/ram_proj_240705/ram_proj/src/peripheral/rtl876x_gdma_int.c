/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_gdma_int.c
* @brief    This file provides all the GDMA firmware internal functions.
* @details
* @author   colin
* @date     2024-06-14
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_gdma.h"

GDMA_TypeDef *GDMA_GetGDMAx(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    GDMA_TypeDef *GDMAx = GDMA_BASE;

    return GDMAx;
}

GDMA_TypeDef *GDMA_GetGDMAxByCh(uint8_t GDMA_ChannelNum)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_ChannelNum));

    GDMA_TypeDef *GDMAx = GDMA_BASE;

    return GDMAx;
}

uint8_t GDMA_GetGDMAChannelNum(uint8_t GDMA_ChannelNum)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_ChannelNum));

    return GDMA_ChannelNum;
}

uint8_t GDMA_GetHandshakeNum(GDMA_ChannelTypeDef *GDMA_Channelx, uint8_t handshake)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ALL_PERIPH(GDMA_Channelx));

    return handshake;
}

GDMA_ChannelTypeDef *GDMA_GetGDMAChannelx(uint8_t GDMA_ChannelNum)
{
    /* Check the parameters */
    assert_param(IS_GDMA_ChannelNum(GDMA_ChannelNum));

    GDMA_ChannelTypeDef *GDMA_Channelx = (GDMA_ChannelTypeDef *)(GDMA_Channel0_BASE +
                                                                 GDMA_ChannelNum * 0x0058);;
    if (GDMA_ChannelNum < 8)
    {
        GDMA_Channelx = (GDMA_ChannelTypeDef *)(GDMA_Channel0_BASE + GDMA_ChannelNum * 0x0058);
    }
    else
    {
        GDMA_Channelx = (GDMA_ChannelTypeDef *)(GDMA_Channel0_BASE + 0x0400 +
                                                (GDMA_ChannelNum - 8) * 0x0058);
    }

    return GDMA_Channelx;
}

bool GDMA_IsValidHandshake(uint8_t handshake)
{
    return true;
}

bool GDMA_IsGatherScatterChannel(GDMA_ChannelTypeDef *GDMA_Channelx)
{
    if ((GDMA_Channelx == GDMA_Channel2) || \
        (GDMA_Channelx == GDMA_Channel3) || \
        (GDMA_Channelx == GDMA_Channel4) || \
        (GDMA_Channelx == GDMA_Channel5))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

