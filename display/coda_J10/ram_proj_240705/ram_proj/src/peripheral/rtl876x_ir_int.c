/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     rtl876x_ir_int.c
* \brief    This file provides all the IR firmware internal functions.
* \details
* \author   qinyuan_hu
* \date     2024-06-27
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x_ir.h"
#include "rtl876x_rcc.h"

/*============================================================================*
 *                           Public Functions
 *============================================================================*/

/**
 * \brief  IR clock source config.
 * \param  ClockDiv: specifies the APB peripheral to gates its clock. \ref Clock_Divider
 * \return None
 */
void IR_ClkSrcConfig(IRSrcClock_TypeDef ClockSrc)
{
    if (ClockSrc == IR_SRC_CLOCK_40M)
    {
        SYSBLKCTRL->u_348.BITS_348.r_irrc_clk_sel = 0;
    }
    else if (ClockSrc == IR_SRC_CLOCK_PPL1)
    {
        SYSBLKCTRL->u_348.BITS_348.r_irrc_clk_sel = 1;
        SYSBLKCTRL->u_348.BITS_348.r_irrc_clk_src_pll_sel = 0;
    }
    else if (ClockSrc == IR_SRC_CLOCK_PPL2)
    {
        SYSBLKCTRL->u_348.BITS_348.r_irrc_clk_sel = 1;
        SYSBLKCTRL->u_348.BITS_348.r_irrc_clk_src_pll_sel = 1;
    }
    return;
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

