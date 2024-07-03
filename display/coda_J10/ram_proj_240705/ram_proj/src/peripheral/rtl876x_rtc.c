/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_rtc.c
* @brief    This file provides all the RTC firmware functions.
* @details
* @author   colin
* @date     2024-06-18
* @version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x_rtc.h"

/*============================================================================*
 *                           Private Functions
 *============================================================================*/
#if (RTC_SUPPORT_INDIRECT_ACCESS == 1)
#include "indirect_access.h"
#define REG_OFFSET(type,v)                      (&(((type *)0)->v))
#define RTC_REG_OFFSET(MEMBER)                  REG_OFFSET(RTC_TypeDef, MEMBER)
#define RTC_WRITE(reg, value)                   aon_indirect_write_reg_safe((uint32_t)RTC_REG_OFFSET(reg), value);
#else
#define RTC_WRITE(reg, value)   do {\
        RTC->reg = value;\
    } while (0)
#endif

/*============================================================================*
 *                           Public Functions
 *============================================================================*/
/**
  * @brief  Reset RTC.
  * @param  None.
  * @retval None
  */
void RTC_DeInit(void)
{
    /* Stop RTC counter */
    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};
    rtc_0x00.b.rtc_cnt_start = 0;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);

    /* Disable wakeup signal */
    rtc_0x00.b.rtc_nv_ie = 0;
    rtc_0x00.b.rtc_wk_ie = 0;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);

    /* Clear all RTC interrupt & wakeup */
    RTC_WRITE(RTC_INT_CLEAR, RTC_ALL_INT_CLR | RTC_ALL_WAKEUP_CLR);

    /* Clear prescale register */
    RTC_WRITE(RTC_PRESCALER0, 0);
    /* Clear all comparator register */
    RTC_WRITE(RTC_COMP_0, 0);
    RTC_WRITE(RTC_COMP_1, 0);
    RTC_WRITE(RTC_COMP_2, 0);
    RTC_WRITE(RTC_COMP_3, 0);
#if (RTC_SUPPORT_COMP_GT == 1)
    RTC_WRITE(RTC_COMP0_GT, 0);
    RTC_WRITE(RTC_COMP1_GT, 0);
    RTC_WRITE(RTC_COMP2_GT, 0);
    RTC_WRITE(RTC_COMP3_GT, 0);
#endif
    RTC_WRITE(RTC_PRESCALE_CMP0, 0);

    /* Reset prescale counter and counter */
    rtc_0x00.d32 = 0;
    rtc_0x00.b.rtc_cnt_rst = 1;
    rtc_0x00.b.rtc_pre_cnt_rst = 1;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
    __NOP();
    __NOP();
    RTC_WRITE(RTC_CR0, 0);
}



/**
  * @brief  Set RTC prescaler value.
  * @param  PrescaleValue: the prescaler value to be set.should be no more than 12 bits!
  * @retval None
  */
void RTC_SetPrescaler(uint16_t value)
{
    RTC_WRITE(RTC_PRESCALER0, value & 0xFFF);
}

/**
  * \brief  Start or stop RTC peripheral.
  * \param  NewState: new state of RTC peripheral.
  *         This parameter can be the following values:
  *         \arg ENABLE: start RTC.
  *         \arg DISABLE: stop RTC.
  * \return None
  */
void RTC_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Start or stop RTC */
    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};
    rtc_0x00.b.rtc_cnt_start = NewState;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
  * \brief  Enable or disable the specified RTC interrupts.
  * \param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_INT_TICK: tick interrupt
  *         \arg RTC_INT_OVF: counter overflow interrupt
  *         \arg RTC_INT_PRE_CMP: prescale compare interrupt
  *         \arg RTC_INT_PRE_CMP3: prescale & compare 3 interrupt
  *         \arg RTC_INT_CMP0: compare 0 interrupt
  *         \arg RTC_INT_CMP1: compare 1 interrupt
  *         \arg RTC_INT_CMP2: compare 2 interrupt
  *         \arg RTC_INT_CMP3: compare 3 interrupt
  * \param  NewState: new state of the specified RTC interrupt.
  *         This parameter can be: ENABLE or DISABLE.
  * \return None.
  */
void RTC_INTConfig(uint32_t RTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT(RTC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};

    if (NewState == ENABLE)
    {
        /* Enable the selected RTC comparator interrupt */
        rtc_0x00.d32 |= RTC_INT;
    }
    else
    {
        /* Disable the selected RTC comparator interrupt */
        rtc_0x00.d32 &= (~RTC_INT);
    }
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
  * \brief  Enable or disable the specified RTC wakeup function.
  * \param  RTC_WK: specifies the RTC wakeup function to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_WK_TICK: tick wakeup function
  *         \arg RTC_WK_OVF: tick wakeup function
  *         \arg RTC_WK_PRE_CMP: prescale compare wakeup function
  *         \arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
  *         \arg RTC_WK_CMP0: compare 0 wakeup function
  *         \arg RTC_WK_CMP1: compare 1 wakeup function
  *         \arg RTC_WK_CMP2: compare 2 wakeup function
  *         \arg RTC_WK_CMP3: compare 3 wakeup function
  * \param  NewState: new state of the specified RTC wakeup function.
  *         This parameter can be: ENABLE or DISABLE.
  * \return None.
  */
void RTC_WKConfig(uint32_t RTC_WK, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_WK(RTC_WK));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};

    if (NewState == ENABLE)
    {
        /* Enable the selected RTC comparator wake up function */
        rtc_0x00.d32 |= RTC_WK;
    }
    else
    {
        /* Disable the selected RTC comparator wake up function */
        rtc_0x00.d32 &= (~RTC_WK);
    }
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
  * \brief  Enable interrupt signal to CPU NVIC.
  * \param  This parameter can be: ENABLE or DISABLE.
  * \return None.
  */
void RTC_NvCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};
    rtc_0x00.b.rtc_nv_ie = NewState;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
 * \brief  Enable or disable system wake up of RTC.
 * \param  NewState: new state of the wake up function.
 *         This parameter can be: ENABLE or DISABLE.
 * \return None
 */
void RTC_SystemWakeupConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Enable or disable system wake up */
    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};
    rtc_0x00.b.rtc_wk_ie = NewState;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
  * \brief  Reset counter value of RTC.
  * \param  None
  * \return None
  */
void RTC_ResetCounter(void)
{
    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};
    rtc_0x00.b.rtc_cnt_rst = 1;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
    __NOP();
    __NOP();
    rtc_0x00.b.rtc_cnt_rst = 0;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
  * \brief  Reset prescaler counter value of RTC.
  * \param  None
  * \return None
  */
void RTC_ResetPrescalerCounter(void)
{
    RTC_CR0_TypeDef rtc_0x00 = {.d32 = RTC->RTC_CR0};
    rtc_0x00.b.rtc_pre_cnt_rst = 1;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
    __NOP();
    __NOP();
    rtc_0x00.b.rtc_pre_cnt_rst = 0;
    RTC_WRITE(RTC_CR0, rtc_0x00.d32);
}

/**
  * \brief  Checks whether the specified RTC interrupt is set or not.
  * \param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_INT_TICK: RTC tick interrupt source
  *         \arg RTC_INT_PRE_COMP: prescale compare interrupt source
  *         \arg RTC_INT_PRE_COMP3: prescale & compare 3 interrupt source
  *         \arg RTC_INT_COMP0: compare 0 interrupt source
  *         \arg RTC_INT_COMP1: compare 1 interrupt source
  *         \arg RTC_INT_COMP2: compare 2 interrupt source
  *         \arg RTC_INT_COMP3: compare 3 interrupt source
  * \return The new state of RTC_INT (SET or RESET).
  */
ITStatus RTC_GetINTStatus(uint32_t RTC_INT)
{
    /* Check the parameters */
    assert_param(IS_RTC_CONFIG_INT(RTC_INT));

    ITStatus int_status = RESET;

    if ((RTC->RTC_INT_SR & (RTC_INT >> 8)) != (uint32_t)RESET)
    {
        int_status = SET;
    }

    /* Return the RTC_INT status */
    return  int_status;
}

/**
  * \brief  Clear the interrupt pending bits of RTC.
  * \param  RTC_INT: specifies the RTC interrupt flag to clear.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_INT_TICK: RTC tick interrupt source
  *         \arg RTC_INT_OVF: RTC counter overflow interrupt source
  *         \arg RTC_INT_PRE_COMP: prescale compare interrupt source
  *         \arg RTC_INT_PRE_COMP3: prescale & compare 3 interrupt source
  *         \arg RTC_INT_COMP0: compare 0 interrupt source
  *         \arg RTC_INT_COMP1: compare 1 interrupt source
  *         \arg RTC_INT_COMP2: compare 2 interrupt source
  *         \arg RTC_INT_COMP3: compare 3 interrupt source
  * \return None
  */
void RTC_ClearINTPendingBit(uint32_t RTC_INT)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT(RTC_INT));

    RTC_INT_CLEAR_TypeDef rtc_0x04 = {.d32 = RTC->RTC_INT_CLEAR};

    rtc_0x04.d32 |= RTC_INT >> 8;
    RTC_WRITE(RTC_INT_CLEAR, rtc_0x04.d32);
    __NOP();
    __NOP();
    RTC_WRITE(RTC_INT_CLEAR, 0);
}

/**
  * \brief  Checks whether the specified RTC wakeup state is set or not.
  * \param  RTC_WK: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_WK_TICK: tick wakeup function
  *         \arg RTC_WK_OVF: tick wakeup function
  *         \arg RTC_WK_PRE_COMP: prescale compare wakeup function
  *         \arg RTC_WK_PRE_COMP3: prescale & compare 3 wakeup function
  *         \arg RTC_WK_COMP0: compare 0 wakeup function
  *         \arg RTC_WK_COMP1: compare 1 wakeup function
  *         \arg RTC_WK_COMP2: compare 2 wakeup function
  *         \arg RTC_WK_COMP3: compare 3 wakeup function
  * \return The new state of RTC_INT (SET or RESET).
  */
ITStatus RTC_GetWakeupStatus(uint32_t RTC_WK)
{
    /* Check the parameters */
    assert_param(IS_RTC_CONFIG_INT(RTC_WK));

    ITStatus wakeup_status = RESET;

    if ((RTC->RTC_INT_SR & (RTC_WK >> 8)) != (uint32_t)RESET)
    {
        wakeup_status = SET;
    }

    /* Return the RTC_INT status */
    return  wakeup_status;
}

/**
  * \brief  Clear the wakeup status bits of RTC.
  * \param  RTC_WK: specifies the RTC wakeup flag to clear.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_WK_TICK: tick wakeup function
  *         \arg RTC_WK_OVF: tick wakeup function
  *         \arg RTC_WK_PRE_COMP: prescale compare wakeup function
  *         \arg RTC_WK_PRE_COMP3: prescale & compare 3 wakeup function
  *         \arg RTC_WK_COMP0: compare 0 wakeup function
  *         \arg RTC_WK_COMP1: compare 1 wakeup function
  *         \arg RTC_WK_COMP2: compare 2 wakeup function
  *         \arg RTC_WK_COMP3: compare 3 wakeup function
  * \return None
  */
void RTC_ClearWakeupStatusBit(uint32_t RTC_WK)
{
    /* Check the parameters */
    assert_param(IS_RTC_WK(RTC_WK));

    RTC_INT_CLEAR_TypeDef rtc_0x04 = {.d32 = RTC->RTC_INT_CLEAR};

    rtc_0x04.d32 |= RTC_WK >> 8;
    RTC_WRITE(RTC_INT_CLEAR, rtc_0x04.d32);
    __NOP();
    __NOP();
    RTC_WRITE(RTC_INT_CLEAR, 0);
}

/**
  * \brief  Clear the interrupt pending bit of the select comparator of RTC.
  * \param  index: the comparator number 0~3.
  * \return None
  */
void RTC_ClearCompINT(RTCComIndex_TypeDef index)
{
    RTC_INT_CLEAR_TypeDef rtc_0x04 = {.d32 = RTC->RTC_INT_CLEAR};

    rtc_0x04.d32 |= RTC_COMP0_CLR << (uint32_t)index;
    RTC_WRITE(RTC_INT_CLEAR, rtc_0x04.d32);
    __NOP();
    __NOP();
    RTC_WRITE(RTC_INT_CLEAR, 0);
}

/**
  * \brief  Clear the overflow interrupt pending bit of RTC.
  * \param  None
  * \return None
  */
void RTC_ClearOverFlowINT(void)
{
    RTC_INT_CLEAR_TypeDef rtc_0x04 = {.d32 = RTC->RTC_INT_CLEAR};

    rtc_0x04.b.rtc_cnt_ov_clr = 0x1;
    RTC_WRITE(RTC_INT_CLEAR, rtc_0x04.d32);
    __NOP();
    __NOP();
    RTC_WRITE(RTC_INT_CLEAR, 0);
}

/**
  * \brief  Clear the tick interrupt pending bit of RTC.
  * \param  None
  * \return None
  */
void RTC_ClearTickINT(void)
{
    RTC_INT_CLEAR_TypeDef rtc_0x04 = {.d32 = RTC->RTC_INT_CLEAR};

    rtc_0x04.b.rtc_tick_clr = 0x1;
    RTC_WRITE(RTC_INT_CLEAR, rtc_0x04.d32);
    __NOP();
    __NOP();
    RTC_WRITE(RTC_INT_CLEAR, 0);
}

/**
  * \brief  Set RTC comparator value.
  * \param  index: The comparator number,can be 0 ~ 3.
  * \param  value: The comparator value to be set.Should be no more than 24 bits!
  * \return None
  */
void RTC_SetCompValue(RTCComIndex_TypeDef index, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_RTC_COMP(index));

    switch (index)
    {
    case RTC_COMP0:
        RTC_WRITE(RTC_COMP_0, value & 0xFFFFFF);
        break;
    case RTC_COMP1:
        RTC_WRITE(RTC_COMP_1, value & 0xFFFFFF);
        break;
    case RTC_COMP2:
        RTC_WRITE(RTC_COMP_2, value & 0xFFFFFF);
        break;
    case RTC_COMP3:
        RTC_WRITE(RTC_COMP_3, value & 0xFFFFFF);
        break;
    default:
        break;
    }
}

#if (RTC_SUPPORT_COMP_GT == 1)
/**
  * \brief  Set RTC comparator GT value.
  * \param  index: The comparator gt number, can be 0 ~ 3.
  * \param  value: The comparator value to be set.
  * \return None
  */
void RTC_SetCompGTValue(RTCCmopGTIndex_TypeDef index, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_RTC_COMPGT(index));

    switch (index)
    {
    case RTC_COMP0GT:
        RTC_WRITE(RTC_COMP0_GT, value);
        break;
    case RTC_COMP1GT:
        RTC_WRITE(RTC_COMP1_GT, value);
        break;
    case RTC_COMP2GT:
        RTC_WRITE(RTC_COMP2_GT, value);
        break;
    case RTC_COMP3GT:
        RTC_WRITE(RTC_COMP3_GT, value);
        break;
    default:
        break;
    }
}
#endif

/**
  * \brief  Set RTC prescaler comparator value.
  * \param  value: The comparator value to be set.Should be no more than 12 bits!
  * \return None
  */
void RTC_SetPreCompValue(uint32_t value)
{
    RTC_WRITE(RTC_PRESCALE_CMP0, value & 0xFFF);
}

/**
  * \brief  Get counter value of RTC.
  * \param  None.
  * \return The counter value.
  */
uint32_t RTC_GetCounter(void)
{
    return (RTC->RTC_CNT0 & 0xFFFFFF);
}

/**
  * \brief  Get prescaler counter value of RTC.
  * \param  None.
  * \return The prescaler counter value.
  */
uint32_t RTC_GetPreCounter(void)
{
    return (RTC->RTC_PRESCALE_CNT0 & 0xFFF);
}

/**
  * \brief  Get RTC comparator value.
  * \param  index: The comparator number.
  * \return The comparator value.
  */
uint32_t RTC_GetCompValue(RTCComIndex_TypeDef index)
{
    return ((*(&(RTC->RTC_COMP_0) + index)) & 0xFFFFFF);
}

#if (RTC_SUPPORT_COMP_GT ==1 )
/**
  * \brief  Get RTC comparator gt value.
  * \param  index: The comparator number 0~3.
  * \return The comparator value.
  */
uint32_t RTC_GetCompGTValue(RTCCmopGTIndex_TypeDef index)
{
    return *(&(RTC->RTC_COMP0_GT) + index);
}
#endif

/**
  * \brief  Get RTC prescaler comparator value.
  * \param  None.
  * \return The prescaler comparator value.
  */
uint32_t RTC_GetPreCompValue(void)
{
    return (RTC->RTC_PRESCALE_CMP0 & 0xFFF);
}

/**
  * \brief  Write backup register for store time information.
  * \param  value: valuer=write to back up reister
  * \return None.
  */
void RTC_WriteBackupReg(uint32_t value)
{
    RTC_WRITE(RTC_BACKUP_REG, value);
}

/**
  * \brief  Read backup register.
  * \param  None.
  * \return Register value.
  */
uint32_t RTC_ReadBackupReg(void)
{
    return (RTC->RTC_BACKUP_REG);
}

/**
  * \brief  Select source clock to gpio input of RTC.
  * \param  gpio: the selected gpio.
  * \return None
  */
#if (RTC_SUPPORT_CLK_INPUT_FROM_PAD_SEL == 1)
void RTC_SelectSrcToGpioInput(RTCInSel_TypeDef rtc_in)
{
    /* Set en_gpio_32k if any muxes need USE_32K_GPIO_IN */
    AON_REG_UPDATE(AON_REG176X_Page0, EN_32K_GPIO_IN_MSK, true << __builtin_ctz(EN_32K_GPIO_IN_MSK));
    AON_REG_UPDATE(AON_NS_PAD_AON_PINMUX_CFG0, RTC_IN_SEL_MSK, rtc_in << __builtin_ctz(RTC_IN_SEL_MSK));
    AON_REG_UPDATE(AON_REG00_SEL_32K, 0x1F000000, 1 << 25);
}
#endif

/**
  * @brief  mask bit remove from the bbpro2 ,for compatible with bbpro and bblite overflow keep this alive.
  * @param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  * @param  NewState: new state of the specified RTC interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_MaskINTConfig(uint32_t RTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != ENABLE)
    {
        RTC_CompINTConfig(RTC_INT, ENABLE);
    }
    else
    {
        RTC_CompINTConfig(RTC_INT, DISABLE);
    }
}
/**
  * @brief  Enable or disable the specified RTC interrupts for comb.
  * @param  RTC_INT: specifies the RTC interrupt source
  *         RTC_INT arge can reference to RTC_INT_t .
  *     @arg RTC_INT_COMPX : compare  x interrupt x =0~3
  *     @arg RTC_WK_COMPX : RTC_CMPx wakeup  Enable x =0~3
  * @param  NewState: new state of the specified RTC interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_CompINTConfig(uint32_t RTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_COMP_INT(RTC_INT_IE));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    RTC_INTConfig(RTC_INT, NewState);

    if (NewState == ENABLE)
    {
        RTC_NvCmd(ENABLE);
    }
}

/**
  * @brief  Enable or disable RTC tick interrupts.
  * @param  NewState: new state of RTC tick interrupt.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void RTC_TickINTConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    RTC_INTConfig(RTC_INT_TICK, NewState);

    if (NewState == ENABLE)
    {
        RTC_NvCmd(ENABLE);
    }
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

