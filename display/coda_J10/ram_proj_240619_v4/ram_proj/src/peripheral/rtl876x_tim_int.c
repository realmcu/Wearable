/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     rtl876x_tim_int.c
* \brief    This file provides all the TIMER firmware internal functions.
* \details
* \author
* \date
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x_tim.h"
#include "pcc_reg.h"
#include "boot_rom_func_ptr.h"

#define TIM0_CH0_ID    0
#define TIM0_CH1_ID    1
#define TIM1_CH0_ID    0
#define TIM1_CH1_ID    1
#define TIM1_CH2_ID    2
#define TIM1_CH3_ID    3
#define TIM1_CH4_ID    4
#define TIM1_CH5_ID    5
#define TIM1_CH6_ID    6
#define TIM1_CH7_ID    7
#define TIM1_CH8_ID    8
#define TIM1_CH9_ID    9

uint32_t TIM_GetTimerID(TIM_TypeDef *TIMx)
{
    uint32_t timerid = 0;
    uint32_t tempreg = (uint32_t)TIMx;

    if (patch_TIM_GetTimerID)
    {
        uint32_t ret;
        if (patch_TIM_GetTimerID(TIMx, &ret))
        {
            return ret;
        }
    }

    if ((TIMx == TIM0_CH0) || (TIMx == TIM0_CH1))
    {
        timerid = (tempreg - TIMER0_BASE) / 0x50;
    }
    else
    {
        timerid = (tempreg - TIMER1_BASE) / 0x50;
    }
    return timerid;
}

TIMCASE_TypeDef TIM_GetCase(TIM_TypeDef *TIMx)
{
    if (patch_TIM_GetCase)
    {
        TIMCASE_TypeDef ret;
        if (patch_TIM_GetCase(TIMx, &ret))
        {
            return ret;
        }
    }

    if ((TIMx == TIM0_CH0) || (TIMx == TIM0_CH1) || (TIMx == TIM1_CH0) || (TIMx == TIM1_CH1) ||
        (TIMx == TIM1_CH2) || (TIMx == TIM1_CH3))
    {
        return TIM_CASE1;
    }
    else if (TIMx == TIM1_CH9)
    {
        return TIM_CASE5;
    }
    else
    {
        return TIM_CASE7;
    }
}

TIM_ShareTypeDef *TIM_GetTimerShareBase(TIM_TypeDef *TIMx)
{
    if (patch_TIM_GetTimerShareBase)
    {
        TIM_ShareTypeDef *ret;
        if (patch_TIM_GetTimerShareBase(TIMx, &ret))
        {
            return ret;
        }
    }

    if ((TIMx == TIM0_CH0) || (TIMx == TIM0_CH1))
    {
        return TIMER0_SHARE;
    }
    else
    {
        return TIMER1_SHARE;
    }
}

void TIM_ClkConfig(TIM_TypeDef *TIMx, TIMClockSrc_TypeDef ClockSrc,
                   TIMClockDiv_TypeDef ClockDiv)
{
    uint32_t timerid = TIM_GetTimerID(TIMx);

    if (patch_TIM_ClkConfig)
    {
        if (patch_TIM_ClkConfig(TIMx, ClockSrc, ClockDiv))
        {
            return;
        }
    }

    if ((TIMx == TIM0_CH0) || (TIMx == TIM0_CH1))
    {
        switch (timerid)
        {
        case TIM0_CH0_ID:
            SYSBLKCTRL->u_36C.BITS_36C.ETIMER0_0_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_36C.BITS_36C.ETIMER0_0_CLK_DIV_EN = 1;
            break;
        case TIM0_CH1_ID:
            SYSBLKCTRL->u_36C.BITS_36C.ETIMER0_1_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_36C.BITS_36C.ETIMER0_1_CLK_DIV_EN = 1;
            break;
        }
    }
    else
    {
        switch (timerid)
        {
        case TIM1_CH0_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_0_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_0_CLK_DIV_EN = 1;
            break;
        case TIM1_CH1_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_1_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_1_CLK_DIV_EN = 1;
            break;
        case TIM1_CH2_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_2_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_2_CLK_DIV_EN = 1;
            break;
        case TIM1_CH3_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_3_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_3_CLK_DIV_EN = 1;
            break;
        case TIM1_CH4_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_4_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_4_CLK_DIV_EN = 1;
            break;
        case TIM1_CH5_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_5_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_5_CLK_DIV_EN = 1;
            break;
        case TIM1_CH6_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_6_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_6_CLK_DIV_EN = 1;
            break;
        case TIM1_CH7_ID:
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_7_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_370.BITS_370.ETIMER1_7_CLK_DIV_EN = 1;
            break;
        case TIM1_CH8_ID:
            if ((ClockSrc == CK_PLL2_TIMER) || (ClockSrc == CK_PLL3_TIMER))
            {
                SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_PLL_CLK_SEL = ClockSrc;
                SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_SEL0 = 1;
                SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_SEL1 = 1;
            }
            else if (ClockSrc == CK_PLL1_TIMER)
            {
                SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_SEL0 = 0;
                SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_SEL1 = 1;
            }
            else
            {
                SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_SEL1 = 0;
            }
            SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_374.BITS_374.ETIMER1_8_CLK_DIV_EN = 1;
            break;
        case TIM1_CH9_ID:
            SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_CLK_DIV_SEL = ClockDiv;
            SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_CLK_DIV_EN = 1;
            break;
        }
    }
}

void TIM_PWMDZClockConfig(PWMDZClockSrc_TypeDef ClockSrc, TIMClockDiv_TypeDef ClockDiv,
                          FunctionalState NewState)
{
    if (patch_TIM_PWMDZClockConfig)
    {
        if (patch_TIM_PWMDZClockConfig(ClockSrc, ClockDiv, NewState))
        {
            return;
        }
    }

    if (NewState != ENABLE)
    {
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_PWM_CLK_DIV_EN = 0;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_PWM_CLK_EN = 0;
    }
    else
    {
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_PWM_CLK_SEL = ClockSrc;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_PWM_CLK_EN = 1;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_PWM_CLK_DIV_SEL = ClockDiv;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_PWM_CLK_DIV_EN = 1;
    }
}

void TIM_LatchTrigDebClkConfig(TIMClockDiv_TypeDef ClockDiv, FunctionalState NewState)
{
    if (patch_TIM_LatchTrigDebClkConfig)
    {
        if (patch_TIM_LatchTrigDebClkConfig(ClockDiv, NewState))
        {
            return;
        }
    }

    if (NewState != ENABLE)
    {
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_DEB_CLK_DIV_EN = 0;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_DEB_CLK_CLK_EN = 0;
    }
    else
    {
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_DEB_CLK_CLK_EN = 1;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_DEB_CLK_DIV_SEL = ClockDiv;
        SYSBLKCTRL->u_374.BITS_374.ETIMER1_9_DEB_CLK_DIV_EN = 1;
    }
}
/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/
