/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl_can_int.c
* @brief    This file provides all the CAN firmware internal functions.
* @details
* @author
* @date     2024-06-28
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl_can.h"
#include "pcc_reg.h"
#include "platform_utils.h"

void CAN_ClkDivConfig(CAN_TypeDef *CANx, CANClkDIV_TypeDef div)
{
    assert_param(IS_CAN_CLK_DIV(div));

    if (CANx == CAN0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_CAN0 = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_36C.BITS_36C.CAN0_SCAN_DIV_SEL = div;

        platform_delay_us(1);
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_CAN0 = 1;
    }
    else if (CANx == CAN1)
    {
        /* disable clock first */
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_CAN1_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_36C.BITS_36C.CAN1_SCAN_DIV_SEL = div;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_CAN1_EN = 1;
    }
}

#if (CAN_SUPPORT_SLEEP_MODE == 1)
void CAN_LowPowerClkDIV(CAN_TypeDef *CANx, CANLowPowerClkDIV_TypeDef div)
{
    if (CANx == CAN0)
    {
        SYSBLKCTRL->u_36C.BITS_36C.CAN0_LP_SCAN_DIV_SEL = div;
    }
    else if (CANx == CAN1)
    {
        SYSBLKCTRL->u_36C.BITS_36C.CAN1_LP_SCAN_DIV_SEL = div;
    }
}
#endif
/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

