/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_lppwm.c
* @brief    This file provides all the LPPWM firmware functions.
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/
#include "rtl876x_lppwm.h"
#include "rtl876x_aon_reg.h"
#include "indirect_access.h"

void LPPWM_Reset(void)
{
    AON_FAST_REG_LPPWM_CH0_CTL_TYPE aon_fast_reg_lppwm_ch0_ctl = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_CTL)};
    aon_fast_reg_lppwm_ch0_ctl.LPPWM_CH0_REG_RST_N = 0;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_CTL, aon_fast_reg_lppwm_ch0_ctl.d16);
    aon_fast_reg_lppwm_ch0_ctl.LPPWM_CH0_REG_RST_N = 1;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_CTL, aon_fast_reg_lppwm_ch0_ctl.d16);
}

void LPPWM_Init(LPPWM_InitTypeDef *LPPWM_InitStruct)
{
    AON_FAST_REG_LPPWM_CH0_CTL_TYPE aon_fast_reg_lppwm_ch0_ctl = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_CTL)};
    aon_fast_reg_lppwm_ch0_ctl.LPPWM_CH0_POLARITY = LPPWM_InitStruct->polarity;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_CTL, aon_fast_reg_lppwm_ch0_ctl.d16);

    AON_FAST_REG_LPPWM_CH0_P0_H_TYPE aon_fast_reg_lppwm_ch0_p0_h = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_P0_H)};
    aon_fast_reg_lppwm_ch0_p0_h.LPPWM_CH0_P0_H = LPPWM_InitStruct->period_high;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_P0_H, aon_fast_reg_lppwm_ch0_p0_h.d16);

    AON_FAST_REG_LPPWM_CH0_P0_L_TYPE aon_fast_reg_lppwm_ch0_p0_l = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_P0_L)};
    aon_fast_reg_lppwm_ch0_p0_l.LPPWM_CH0_P0_L = LPPWM_InitStruct->period_low;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_P0_L, aon_fast_reg_lppwm_ch0_p0_l.d16);
}

void LPPWM_StructInit(LPPWM_InitTypeDef *LPPWM_InitStruct)
{
    LPPWM_InitStruct->polarity    = LPPWM_OUTPUT_NORMAL;
    LPPWM_InitStruct->period_high = 0;
    LPPWM_InitStruct->period_low  = 0;
}

void LPPWM_Cmd(FunctionalState NewState)
{
    AON_FAST_REG_LPPWM_CH0_CTL_TYPE aon_fast_reg_lppwm_ch0_ctl = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_CTL)};
    aon_fast_reg_lppwm_ch0_ctl.LPPWM_CH0_EN = NewState;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_CTL, aon_fast_reg_lppwm_ch0_ctl.d16);
}

void LPPWM_ChangeFreqAndDuty(uint16_t period_high, uint16_t period_low)
{
    AON_FAST_REG_LPPWM_CH0_P0_H_TYPE aon_fast_reg_lppwm_ch0_p0_h = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_P0_H)};
    aon_fast_reg_lppwm_ch0_p0_h.LPPWM_CH0_P0_H = period_high;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_P0_H, aon_fast_reg_lppwm_ch0_p0_h.d16);

    AON_FAST_REG_LPPWM_CH0_P0_L_TYPE aon_fast_reg_lppwm_ch0_p0_l = {.d16 = btaon_fast_read_safe(AON_FAST_REG_LPPWM_CH0_P0_L)};
    aon_fast_reg_lppwm_ch0_p0_l.LPPWM_CH0_P0_L = period_low;
    btaon_fast_write_safe(AON_FAST_REG_LPPWM_CH0_P0_L, aon_fast_reg_lppwm_ch0_p0_l.d16);
}

