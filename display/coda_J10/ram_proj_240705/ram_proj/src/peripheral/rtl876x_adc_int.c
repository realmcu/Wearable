/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_adc_int.c
* @brief    This file provides all the ADC firmware internal functions.
* @details
* @author
* @date
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_adc.h"
#include "indirect_access.h"
#include "rtl876x_aon_reg.h"

void ADC_SISet(void)
{
    AON_FAST_REG0X_AUXADC_TYPE adc_si = {.d16 = btaon_fast_read_safe(AON_FAST_REG0X_AUXADC)};
    adc_si.AUXADC_SEL_VREF = 2;
    adc_si.AUXADC_VCM_SEL = 1;
    adc_si.AUXADC_EN_LN = 1;
    adc_si.AUXADC_EN_LNA = 1;
    btaon_fast_write_safe(AON_FAST_REG0X_AUXADC, adc_si.d16);
}

void ADC_DelayStructConfig(ADC_InitTypeDef *ADC_InitStruct)
{
    ADC_InitStruct->ADC_PowAD1Dly       = ADC_POW_AD1_DELAY_40_US;
    ADC_InitStruct->ADC_PowAD2Dly       = ADC_POW_AD2_DELAY_20_US;
    ADC_InitStruct->ADC_PowRefDly       = ADC_POW_REF_DELAY_30_US;
}

void ADC_DelayConfig(ADC_TypeDef *ADCx, ADC_InitTypeDef *ADC_InitStruct)
{
    ADC_POW_DATA_DLY_CTRL_TypeDef adc_0x50 = {.d32 = ADCx->ADC_POW_DATA_DLY_CTRL};

    adc_0x50.b.adc_rg2x_auxadc_0_delay_sel = ADC_InitStruct->ADC_PowAD1Dly;
    adc_0x50.b.adc_rg0x_auxadc_1_delay_sel = ADC_InitStruct->ADC_PowAD2Dly;
    adc_0x50.b.adc_rg0x_auxadc_0_delay_sel = ADC_InitStruct->ADC_PowRefDly;

    ADCx->ADC_POW_DATA_DLY_CTRL = adc_0x50.d32;
}

void ADC_BypassRegConfig(uint8_t ChannelNum, FunctionalState NewState)
{
    uint16_t tmpVal;

    if (NewState != DISABLE)
    {
        tmpVal = btaon_fast_read_safe(AON_FAST_REG1X_AUXADC);
        tmpVal |= BIT(ChannelNum);
        btaon_fast_write_safe(AON_FAST_REG1X_AUXADC, tmpVal);
        tmpVal = btaon_fast_read_safe(AON_FAST_REG1X_AUXADC);
    }
    else
    {
        tmpVal = btaon_fast_read_safe(AON_FAST_REG1X_AUXADC);
        tmpVal &= ~BIT(ChannelNum);
        btaon_fast_write_safe(AON_FAST_REG1X_AUXADC, tmpVal);
    }
}

#if ADC_SUPPORT_MANUAL_MODE
void ADC_delayUS(uint32_t t)
{
    platform_delay_us(t);
}

void ADC_ManualModeConfig(void)
{
    ADC->ADC_POW_DATA_DLY_CTRL  |= (BIT10 | BIT12);
    ADC_delayUS(120);
    ADC->ADC_POW_DATA_DLY_CTRL  |= (BIT11);
    ADC_delayUS(30);
    ADC->ADC_POW_DATA_DLY_CTRL  |= (BIT13);
    ADC_delayUS(30);
    ADC->ADC_POW_DATA_DLY_CTRL  |= (BIT14);
}

void ADC_PowerAlwaysOnCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADC_PERIPH(ADCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    ADC_POW_DATA_DLY_CTRL_TypeDef adc_0x50 = {.d32 = ADCx->ADC_POW_DATA_DLY_CTRL};

    if (NewState == ENABLE)
    {
        adc_0x50.b.adc_manual_poweron = 0x1;
    }
    else
    {
        adc_0x50.b.adc_manual_poweron = 0x0;
    }

    ADCx->ADC_POW_DATA_DLY_CTRL = adc_0x50.d32;
}
#endif

#if ADC_SUPPORT_GET_RESULT
/**
  * @brief  Get ADC Result
  * @param  RawData: ADC raw data.
  * @param  adcMode:
  *      this parameter can be one of the following values:
  *     @arg EXT_SINGLE_ENDED(index)
  *     @arg EXT_DIFFERENTIAL(index)
  *     @arg INTERNAL_VBAT_MODE
  *     @arg INTERNAL_VADPIN_MODE
  * @retval ADC result
  */
int32_t ADC_GetRes(uint16_t RawData, uint8_t adcMode)
{
    /*Check internal channel or External Channel*/
    if ((adcMode & 0x18) == 0x18)
    {
        switch (adcMode)
        {
        case INTERNAL_VBAT_MODE:
            /* INTERNAL_VBAT_MODE */
            return (int32_t)(1000 * ((double)RawData + 47) / 900);
        case INTERNAL_VADPIN_MODE:
            /* INTERNAL_VADPIN_MODE */
            return (int32_t)(1000 * ((double)RawData + 53) / 647);
        case INTERNAL_VDDCORE_MODE:
        case INTERNAL_VCORE_MODE:
            return (int32_t)(1000 * ((double)RawData) / 4096 * 5 / 4);
        case INTERNAL_ICHARGE_MODE:
            /* INTERNAL_ICHARGE_MODE */
            return (int32_t)(1000 * ((double)RawData - 0.65) / 16787.5);
        case INTERNAL_IDISCHG_MODE:
            /* INTERNAL_IDISCHG_MODE */
            return (int32_t)(1000 * ((double)RawData + 100) / 6000);
        case INTERNAL_ICHGM1_MODE:
            /* INTERNAL_ICHGM1_MODE */
            return (int32_t)(1000 * ((double)RawData) / 4096 * 5 / 4);
        case INTERNAL_VDDIO_MODE:
            /* INTERNAL_VDDIO_MODE */
            return (int32_t)(1000 * ((double)RawData) / 4096 * 18 / 5);
        default:
            break;
        }
    }
    else
    {
        /* External channel */
        return (int32_t)(((83124 * RawData) / 100000) - ((933 * RawData * RawData) / 100000000) + 36);
    }
    return 0;
}

/**
  * @brief  Get ADC Result in high bypass resistor mode.
  * @param  RawData: ADC raw data.
  * @param  adcMode:
  *      this parameter can be one of the following values:
  *     @arg EXT_SINGLE_ENDED(index)
  *     @arg EXT_DIFFERENTIAL(index)
  *     @arg INTERNAL_VBAT_MODE
  *     @arg INTERNAL_VADPIN_MODE
  * @retval ADC result
  */
int32_t ADC_GetHighBypassRes(uint16_t RawData, uint8_t adcMode)
{
    return (int32_t)(((22651 * RawData) / 100000) - ((169 * RawData * RawData) / 100000000) + 4);
}
#endif

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

