/**
**********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_lpc.c
* @brief    This file provides all the lpcomp firmware functions.
* @details
* @author
* @date
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_lpc.h"
#include "indirect_access.h"
#include "rtl876x_aon_reg.h"

/* Internal defines ------------------------------------------------------------*/
#define REG_OFFSET(type,v)                      (&(((type *)0x78)->v))
#define LPC_REG_OFFSET(MEMBER)                  REG_OFFSET(LPC_TypeDef, MEMBER)
#define LPC_WRITE(reg, value)                   aon_indirect_write_reg_safe((uint32_t)LPC_REG_OFFSET(reg), value);
#define REG2X_AUXADC_DEFAULT_VALUE              (0x81C4UL)
#define REG5X_MBIAS_DEFAULT_VALUE               (0xA090UL)

/**
  * @brief  Deinitializes the LPC peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void LPC_DeInit(void)
{
    btaon_fast_write_safe(AON_FAST_REG2X_AUXADC, REG2X_AUXADC_DEFAULT_VALUE);
    btaon_fast_write_safe(AON_FAST_REG5X_MBIAS, REG5X_MBIAS_DEFAULT_VALUE);

    LPC_CR0_TypeDef lpc_0x80 = {.d32 = LPC->LPC_CR0};
    LPC_CMP_TypeDef lpc_0x88 = {.d32 = LPC->LPC_CMP};

    lpc_0x80.d32 = 0;
    lpc_0x88.b.lpcomp_comp = 0;
    LPC_WRITE(LPC_CR0, lpc_0x80.d32);
    LPC_WRITE(LPC_CMP, lpc_0x88.d32);
}

/**
  * @brief  Initializes LPC peripheral according to
  *    the specified parameters in the LPC_InitStruct.
  * @param  LPC_InitStruct: pointer to a LPC_InitTypeDef
  *    structure that contains the configuration information for the
  *    specified LPC peripheral.
  * @retval None
  */
void LPC_Init(LPC_InitTypeDef *LPC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_LPC_CHANNEL(LPC_InitStruct->LPC_Channel));
    assert_param(IS_LPC_EDGE(LPC_InitStruct->LPC_Edge));
    assert_param(IS_LPC_THRESHOLD(LPC_InitStruct->LPC_Threshold));

    /* Configure parameters */
    AON_FAST_REG2X_AUXADC_TYPE aon1514;
    aon1514.d16 = btaon_fast_read_safe(AON_FAST_REG2X_AUXADC);
    aon1514.AUXADC_SEL_SD_CH = LPC_InitStruct->LPC_Channel;
    aon1514.AUXADC_EN_SD_POSEDGE = LPC_InitStruct->LPC_Edge;
    btaon_fast_write_safe(AON_FAST_REG2X_AUXADC, aon1514.d16);

    AON_FAST_REG5X_MBIAS_TYPE aon_coa;
    aon_coa.d16 = btaon_fast_read_safe(AON_FAST_REG5X_MBIAS);
    aon_coa.MBIAS_SEL_VR_AUXADC = LPC_InitStruct->LPC_Threshold;
    btaon_fast_write_safe(AON_FAST_REG5X_MBIAS, aon_coa.d16);

}

/**
  * @brief  Fills each LPC_InitStruct member with its default value.
  * @param  LPC_InitStruct : pointer to a LPC_InitTypeDef structure which will be initialized.
  * @retval None
  */
void LPC_StructInit(LPC_InitTypeDef *LPC_InitStruct)
{
    LPC_InitStruct->LPC_Channel   = ADC_0;
    LPC_InitStruct->LPC_Edge      = LPC_Vin_Below_Vth;
    LPC_InitStruct->LPC_Threshold = LPC_200_mV;
}

/**
  * @brief  Enables or disables LPC peripheral.
  * @param  NewState: new state of LPC peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LPC_Cmd(FunctionalState NewState)
{
    uint32_t reg_value = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    AON_FAST_REG2X_AUXADC_TYPE aon1514;
    LPC_CR0_TypeDef lpc_0x80 = {.d32 = LPC->LPC_CR0};

    aon1514.d16 = btaon_fast_read_safe(AON_FAST_REG2X_AUXADC);
    lpc_0x80.b.lpc_power = NewState;
    lpc_0x80.b.lpcomp_flag_clr = 0;
    aon1514.AUXADC_POW_SD = NewState;

    LPC_WRITE(LPC_CR0, lpc_0x80.d32);
    btaon_fast_write_safe(AON_FAST_REG2X_AUXADC, aon1514.d16);
}

/**
  * @brief Start or stop the LPC counter.
  * @param  NewState: new state of the LPC counter.
  *   This parameter can be one of the following values:
  *     @arg ENABLE: Start LPCOMP counter.
  *     @arg DISABLE: Stop LPCOMP counter.
  * @retval None
  */
void LPC_CounterCmd(FunctionalState NewState)
{
    uint32_t reg_value = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    LPC_CR0_TypeDef lpc_0x80 = {.d32 = LPC->LPC_CR0};

    lpc_0x80.b.lpcomp_counter_start = NewState;
    lpc_0x80.b.lpcomp_flag_clr = 0;
    LPC_WRITE(LPC_CR0, lpc_0x80.d32);
}

/**
  * @brief Reset the LPC counter.
  * @retval None
  */
void LPC_CounterReset(void)
{
    /* Reset the LPCOMP counter */
    LPC_CR0_TypeDef lpc_0x80 = {.d32 = LPC->LPC_CR0};

    lpc_0x80.b.lpcomp_counter_rst = 1;
    lpc_0x80.b.lpcomp_flag_clr = 0;
    LPC_WRITE(LPC_CR0, lpc_0x80.d32);

    lpc_0x80.b.lpcomp_counter_rst = 0;
    LPC_WRITE(LPC_CR0, lpc_0x80.d32);

}

/**
  * @brief  Configure LPCOMP counter's comparator value.
  * @param  data: LPCOMP counter's comparator value which can be 0 to 0xfff.
  * @retval None.
  */
void LPC_WriteComparator(uint32_t data)
{
    LPC_CMP_TypeDef lpc_0x88 = {.d32 = LPC->LPC_CMP};
    if (data > 0xfff)
    {
        data = 0xfff;
    }
    lpc_0x88.b.lpcomp_comp = data;
    LPC_WRITE(LPC_CMP, lpc_0x88.d32);
}

/**
  * @brief  read LPCOMP comparator value.
  * @param none.
  * @retval LPCOMP comparator value.
  */
uint16_t LPC_ReadComparator(void)
{
    LPC_CMP_TypeDef lpc_0x88 = {.d32 = LPC->LPC_CMP};

    return (lpc_0x88.b.lpcomp_comp);
}

/**
  * @brief  Read LPC counter value.
  * @retval LPCOMP counter value.
  */
uint16_t LPC_ReadCounter(void)
{
    LPC_CNT_TypeDef lpc_0x8c = {.d32 = LPC->LPC_CNT};

    return (lpc_0x8c.b.lpcomp_counter);
}

/**
  * @brief  Enables or disables the specified LPC interrupts.
  * @param  LPC_INT: specifies the LPC interrupt source to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg LPC_INT_VOLTAGE_COMP: voltage detection interrupt.If Vin<Vth, cause this interrupt.
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @param  NewState: new state of the specified LPC interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void LPC_INTConfig(uint32_t LPC_INT, FunctionalState NewState)
{
    uint32_t reg_value = 0;

    /* Check the parameters */
    assert_param(IS_LPC_CONFIG_INT(LPC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    LPC_CR0_TypeDef lpc_0x80 = {.d32 = LPC->LPC_CR0};

    if (NewState != DISABLE)
    {
        /* Enable the selected LPC interrupt */
        lpc_0x80.d32 |= LPC_INT;
    }
    else
    {
        /* Disable the selected LPC interrupt */
        lpc_0x80.d32 &= (~LPC_INT);
    }
    lpc_0x80.b.lpcomp_flag_clr = 0;
    LPC_WRITE(LPC_CR0, lpc_0x80.d32);
}

/**
  * @brief  Clear the specified LPC interrupt.
  * @param  LPC_INT: specifies the LPC interrupt to clear.
  *   This parameter can be one of the following values:
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @retval None.
  */
void LPC_ClearINTPendingBit(uint32_t LPC_INT)
{
    /* Check the parameters */
    assert_param(IS_LPC_CLEAR_INT(LPC_INT));

    /* Clear counter comparator interrupt */
    if (LPC_INT == LPC_INT_COUNT_COMP)
    {
        LPC_CR0_TypeDef lpc_0x80 = {.d32 = LPC->LPC_CR0};
        lpc_0x80.b.lpcomp_flag_clr = 1;
        LPC_WRITE(LPC_CR0, lpc_0x80.d32);

        lpc_0x80.b.lpcomp_flag_clr = 0;
        LPC_WRITE(LPC_CR0, lpc_0x80.d32);
    }
}

/**
  * @brief  Checks whether the specified LPC interrupt is set or not.
  * @param  LPC_INT: specifies the LPC interrupt to check.
  *   This parameter can be one of the following values:
  *     @arg LPC_INT_COUNT_COMP: couter comparator interrupt.
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus LPC_GetINTStatus(uint32_t LPC_INT)
{
    /* Check the parameters */
    assert_param(IS_LPC_STATUS_INT(LPC_INT));

    ITStatus bit_status = RESET;
    LPC_SR_TypeDef lpc_0x84 = {.d32 = LPC->LPC_SR};

    if (LPC_INT == LPC_INT_COUNT_COMP)
    {
        bit_status = lpc_0x84.b.lpcomp_flag;
    }
    if (LPC_INT == LPC_INT_LPCOMP_AON)
    {
        bit_status = lpc_0x84.b.lpcomp_out_aon;
    }

    /* Return the LPC_INT status */
    return  bit_status;
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

