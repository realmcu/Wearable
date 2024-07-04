/**
**********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     rtl876x_tim.c
* \brief    This file provides all the Timer firmware functions.
* \details
* \author
* \date
* \version  v1.0
**********************************************************************************************************
*/

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x_tim.h"
#include "rtl876x_rcc.h"
#include "boot_rom_func_ptr.h"

/*============================================================================*
 *                        Private Functions
 *============================================================================*/
extern uint32_t TIM_GetTimerID(TIM_TypeDef *TIMx);
extern TIMCASE_TypeDef TIM_GetCase(TIM_TypeDef *TIMx);
extern TIM_ShareTypeDef *TIM_GetTimerShareBase(TIM_TypeDef *TIMx);
/*============================================================================*
 *                        Public Functions
 *============================================================================*/

/**
 * \brief  PWM complementary output emergency stop.
 *         PWM_P emergency stop level state is configured by PWM_Stop_State_P,
 *         PWM_N emergency stop level state is configured by PWM_Stop_State_N.
 * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
 * \param  NewState: New state of complementary output.
 *         \arg DISABLE: Resume PWM complementary output.
 *         \arg ENABLE: PWM complementary output emergency stop.
 * \return None.
 */
void TIM_PWMComplOutputEMCmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMER_PWM_CFG_TypeDef timer_0x14 = {.d32 = TIMx->TIMER_PWM_CFG};
    timer_0x14.b.reg_timer_pwm_dz_emg_stop = NewState;
    TIMx->TIMER_PWM_CFG = timer_0x14.d32;
}

#if TIM_SUPPORT_PWM_SRC_SELECT
/**
 * \brief  TIM PWMP/N Source Select.
 * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
 * \param  PWMSrcSel: State of the TIMx PWMP/N.
 * \return None.
 */
void TIM_PWMDZRefSel(TIM_TypeDef *TIMx, TIMPWMDZRef_TypeDef PWMSrcSel)
{
    /* Check the parameters. */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMER_PWM_CFG_TypeDef timer_0x14 = {.d32 = TIMx->TIMER_PWM_CFG};
    timer_0x14.b.reg_timer_pwm_dz_ref_sel = PWMSrcSel;
    TIMx->TIMER_PWM_CFG = timer_0x14.d32;
}
#endif

/**
  * @brief  Enable or Disable TIM peripheral clock.
  * @param  is_enable:
  *  @args  true:  enable hw timer hw clock
  *  @args  false: disable hw timer hw clock
  * @retval None
  */
void TIM_HwClock(bool is_enable)
{
    RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, (FunctionalState)is_enable);
}

/**
  * \brief  Initializes the TIMx Time Base Unit peripheral according to
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * \param  TIMx: Select the TIM peripheral. \ref TIMER_Declaration
  * \param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for the
  *         specified TIM peripheral.
  * \return None
  */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_TIM_MODE(TIM_TimeBaseInitStruct->TIM_Mode));
    assert_param(IS_TIM_PWM_DeadZone_En(TIM_TimeBaseInitStruct->PWMDeadZone_En));

    TIMCASE_TypeDef timercase = TIM_GetCase(TIMx);

    if (patch_TIM_TimeBaseInit)
    {
        if (patch_TIM_TimeBaseInit(TIMx, TIM_TimeBaseInitStruct))
        {
            return;
        }
    }

    /* Clock source config */
    TIM_ClkConfig(TIMx, TIM_TimeBaseInitStruct->TIM_ClockSrc, TIM_TimeBaseInitStruct->TIM_SOURCE_DIV);

    /* set timer mode and mask interrupt */
    TIMER_MODE_CFG_TypeDef timer_0x04 = {.d32 = TIMx->TIMER_MODE_CFG};
    timer_0x04.b.reg_timer_mode = TIM_TimeBaseInitStruct->TIM_Mode;
    timer_0x04.b.reg_timer_oneshot_mode_en = TIM_TimeBaseInitStruct->TIM_OneShotEn;
    TIMx->TIMER_MODE_CFG = timer_0x04.d32;

    if (TIM_TimeBaseInitStruct->TIM_Mode == TIM_Mode_FreeRun)
    {
        if (timercase == TIM_CASE5)
        {
#if TIM_SUPPORT_LATCH_CNT_0
            /* Config latch0 function. */
            TIMER_LAT_CNT_0_CFG_TypeDef timer_0x1C = {.d32 = TIMx->TIMER_LAT_CNT_0_CFG};
            timer_0x1C.b.reg_timer_latch_cnt_0_en = TIM_TimeBaseInitStruct->TIM_LatchCountEn[0];
            timer_0x1C.b.reg_timer_latch_cnt_0_trig_mode = TIM_TimeBaseInitStruct->TIM_LatchCountTrigger[0];
            timer_0x1C.b.reg_timer_latch_cnt_0_fifo_th = TIM_TimeBaseInitStruct->TIM_LatchCountThd;
            TIMx->TIMER_LAT_CNT_0_CFG = timer_0x1C.d32;

            /* Config latch0 trigger. */
            if (TIM_TimeBaseInitStruct->TIM_LatchCountEn[0] == ENABLE)
            {
                TIM_LatchTrigDebClkConfig(TIM_TimeBaseInitStruct->TIM_LatchTrigDebClkDiv,
                                          TIM_TimeBaseInitStruct->TIM_LatchTriggerDebEn);
                SoC_VENDOR->u_310.BITS_310.timer1_9_gpio_trig_deb_en =
                    TIM_TimeBaseInitStruct->TIM_LatchTriggerDebEn;
                SoC_VENDOR->u_310.BITS_310.timer1_9_gpio_trig_deb_cnt_limit =
                    TIM_TimeBaseInitStruct->TIM_LatchTriggerDebSize;
                SoC_VENDOR->u_310.BITS_310.timer1_9_gpio_trig_sel = TIM_TimeBaseInitStruct->TIM_LatchTriggerPad;
            }
#endif
#if TIM_SUPPORT_LATCH_CNT_1
            /* Config latch1 function. */
            TIMER_LAT_CNT_1_CFG_TypeDef timer_0x28 = {.d32 = TIMx->TIMER_LAT_CNT_1_CFG};
            timer_0x28.b.reg_timer_latch_cnt_1_en = TIM_TimeBaseInitStruct->TIM_LatchCountEn[1];
            timer_0x28.b.reg_timer_latch_cnt_1_trig_mode = TIM_TimeBaseInitStruct->TIM_LatchCountTrigger[1];
            TIMx->TIMER_LAT_CNT_1_CFG = timer_0x28.d32;
#endif
#if TIM_SUPPORT_LATCH_CNT_2
            /* Config latch2 function. */
            TIMER_LAT_CNT_2_CFG_TypeDef timer_0x34 = {.d32 = TIMx->TIMER_LAT_CNT_2_CFG};
            timer_0x34.b.reg_timer_latch_cnt_2_en = TIM_TimeBaseInitStruct->TIM_LatchCountEn[2];
            timer_0x34.b.reg_timer_latch_cnt_2_trig_mode = TIM_TimeBaseInitStruct->TIM_LatchCountTrigger[2];
            TIMx->TIMER_LAT_CNT_2_CFG = timer_0x34.d32;
#endif
        }
    }
#if TIM_SUPPORT_PWM_FUNCTION
    else if ((TIM_TimeBaseInitStruct->TIM_Mode == TIM_Mode_UserDefine_Auto) ||
             (TIM_TimeBaseInitStruct->TIM_Mode == TIM_Mode_UserDefine_Manual))
    {
        /* set timer period */
        TIMx->TIMER_MAX_CNT = TIM_TimeBaseInitStruct->TIM_Period;

        /* set pwm */
        TIMER_PWM_CFG_TypeDef timer_0x14 = {.d32 = TIMx->TIMER_PWM_CFG};
        if (TIM_TimeBaseInitStruct->TIM_PWM_En == DISABLE)
        {
            /* set timer period */
            timer_0x14.b.reg_timer_pwm_en = 0x0;
        }
        else
        {
            timer_0x14.b.reg_timer_pwm_en = 0x1;
            timer_0x14.b.reg_timer_pwm_polarity = TIM_TimeBaseInitStruct->TIM_PWMStartPolarity;
            timer_0x14.b.reg_timer_pwm_open_drain_en = TIM_TimeBaseInitStruct->PWM_OpenDrain;

            if (TIM_TimeBaseInitStruct->TIM_Mode == TIM_Mode_UserDefine_Manual)
            {
                if (TIM_TimeBaseInitStruct->TIM_PWMStartPolarity == TIM_PWM_START_WITH_HIGH)
                {
                    if (TIM_TimeBaseInitStruct->TIM_PWM_High_Count > TIM_TimeBaseInitStruct->TIM_Period)
                    {
                        TIM_TimeBaseInitStruct->TIM_PWM_High_Count = TIM_TimeBaseInitStruct->TIM_Period;
                    }
                    TIMx->TIMER_CCR = ((TIM_TimeBaseInitStruct->TIM_Period) - \
                                       (TIM_TimeBaseInitStruct->TIM_PWM_High_Count));
                }
                else
                {
                    TIMx->TIMER_CCR = TIM_TimeBaseInitStruct->TIM_PWM_High_Count;
                }
            }
        }
        TIMx->TIMER_PWM_CFG = timer_0x14.d32;
    }
#endif

#if TIM_SUPPORT_PWM_DEADZONE
    /* Config PWM Deadzone */
    if (TIM_TimeBaseInitStruct->PWMDeadZone_En == ENABLE)
    {
        /* Set deadzone clock source and div */
        TIM_PWMDZClockConfig(TIM_TimeBaseInitStruct->PWM_Deazone_ClockSrc,
                             TIM_TimeBaseInitStruct->PWM_Deadzone_DIV, ENABLE);

        /* Enable pwm deadzone mode. */
        TIMER_PWM_CFG_TypeDef timer_0x14 = {.d32 = TIMx->TIMER_PWM_CFG};
        timer_0x14.b.reg_timer_pwm_dz_en = 1;
        timer_0x14.b.reg_timer_pwm_dz_stop_state_p = TIM_TimeBaseInitStruct->PWM_Stop_State_P;
        timer_0x14.b.reg_timer_pwm_dz_stop_state_n = TIM_TimeBaseInitStruct->PWM_Stop_State_N;
        timer_0x14.b.reg_timer_pwm_dz_ref_p_inv_en = TIM_TimeBaseInitStruct->PWM_DZ_INV_P;
        timer_0x14.b.reg_timer_pwm_dz_ref_n_inv_en = TIM_TimeBaseInitStruct->PWM_DZ_INV_N;
        timer_0x14.b.reg_timer_pwm_dz_size = TIM_TimeBaseInitStruct->PWM_Deazone_Size;
        TIMx->TIMER_PWM_CFG = timer_0x14.d32;
    }
    else
    {
        /* Disable deadzone clock source and div */
        TIM_PWMDZClockConfig(TIM_TimeBaseInitStruct->PWM_Deazone_ClockSrc,
                             TIM_TimeBaseInitStruct->PWM_Deadzone_DIV, DISABLE);

        /* Disable pwm deadzone mode. */
        TIMER_PWM_CFG_TypeDef timer_0x14 = {.d32 = TIMx->TIMER_PWM_CFG};
        timer_0x14.b.reg_timer_pwm_dz_en = 0;
        TIMx->TIMER_PWM_CFG = timer_0x14.d32;
    }
#endif
#if TIM_SUPPORT_DMA
    if (timercase == TIM_CASE5)
    {
        /* DMA function. */
        if (TIM_TimeBaseInitStruct->TIM_DmaEn != DISABLE)
        {
            TIMER_DMA_CFG_TypeDef timer_0x40 = {.d32 = TIMx->TIMER_DMA_CFG};
            timer_0x40.b.reg_timer_dma_en = 1;
            timer_0x40.b.reg_timer_dma_mode = 0;
            timer_0x40.b.reg_timer_dma_target = TIM_TimeBaseInitStruct->TIM_DmaTragget;
            TIMx->TIMER_DMA_CFG = timer_0x40.d32;
        }
    }
#endif
    /* Clear the IT */
    TIM_ClearInterrupt(TIMx, TIM_INT_STS | TIM_INT_LATCH_CNT_FIFO_FULL | TIM_INT_LATCH_CNT_FIFO_THD |
                       TIM_INT_PAUSE);
}

/**
  * \brief  Fills each TIM_InitStruct member with its default value.
  * \param  TIM_InitStruct : pointer to a TIM_TimeBaseInitTypeDef structure which will be initialized.
  * \return None
  */
void TIM_StructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct)
{
    if (patch_TIM_StructInit)
    {
        if (patch_TIM_StructInit(TIM_TimeBaseInitStruct))
        {
            return;
        }
    }

    TIM_TimeBaseInitStruct->TIM_ClockSrc = CK_40M_TIMER;
    TIM_TimeBaseInitStruct->TIM_SOURCE_DIV = TIM_CLOCK_DIVIDER_1;
    TIM_TimeBaseInitStruct->TIM_Mode = TIM_Mode_UserDefine_Manual;
    TIM_TimeBaseInitStruct->TIM_OneShotEn = DISABLE;
    TIM_TimeBaseInitStruct->TIM_Period = 0xfffffff;
#if TIM_SUPPORT_PWM_FUNCTION
    TIM_TimeBaseInitStruct->TIM_PWM_En = DISABLE;
    TIM_TimeBaseInitStruct->TIM_PWMStartPolarity = TIM_PWM_START_WITH_HIGH;
    TIM_TimeBaseInitStruct->PWM_OpenDrain = PWM_PUSH_PULL;
#endif
#if TIM_SUPPORT_PWM_DEADZONE
    TIM_TimeBaseInitStruct->PWM_Deazone_ClockSrc = PWM_CK_32K_TIMER;
    TIM_TimeBaseInitStruct->PWM_Deadzone_DIV = TIM_CLOCK_DIVIDER_1;
    TIM_TimeBaseInitStruct->PWM_Stop_State_P = PWM_STOP_AT_LOW;
    TIM_TimeBaseInitStruct->PWM_Stop_State_N = PWM_STOP_AT_HIGH;
    TIM_TimeBaseInitStruct->PWM_DZ_INV_P = DISABLE;
    TIM_TimeBaseInitStruct->PWM_DZ_INV_N = DISABLE;
    TIM_TimeBaseInitStruct->PWMDeadZone_En = DISABLE;
    TIM_TimeBaseInitStruct->PWM_Deazone_Size = 10;
#endif
#if TIM_SUPPORT_LATCH_CNT_0
    TIM_TimeBaseInitStruct->TIM_LatchCountEn[0]       = DISABLE;
    TIM_TimeBaseInitStruct->TIM_LatchCountTrigger[0]  = TIM_LATCH_TRIGGER_RISING_EDGE;
    TIM_TimeBaseInitStruct->TIM_LatchTrigDebClkDiv    = TIM_CLOCK_DIVIDER_1;
    TIM_TimeBaseInitStruct->TIM_LatchTriggerDebEn     = DISABLE;
    TIM_TimeBaseInitStruct->TIM_LatchTriggerDebSize   = 1;
    TIM_TimeBaseInitStruct->TIM_LatchTriggerPad       = 0;
#endif
#if TIM_SUPPORT_LATCH_CNT_1
    TIM_TimeBaseInitStruct->TIM_LatchCountEn[1]       = DISABLE;
    TIM_TimeBaseInitStruct->TIM_LatchCountTrigger[1]  = TIM_LATCH_TRIGGER_RISING_EDGE;
#endif
#if TIM_SUPPORT_LATCH_CNT_2
    TIM_TimeBaseInitStruct->TIM_LatchCountEn[2]       = DISABLE;
    TIM_TimeBaseInitStruct->TIM_LatchCountTrigger[2]  = TIM_LATCH_TRIGGER_RISING_EDGE;
#endif
#if TIM_SUPPORT_DMA
    TIM_TimeBaseInitStruct->TIM_DmaEn = DISABLE;
    TIM_TimeBaseInitStruct->TIM_DmaTragget = TIM_DMA_CCR_FIFO;
#endif
}

/**
  * \brief  Enables or disables the specified TIM peripheral.
  * \param  TIMx: Select the TIM peripheral. \ref TIMER_Declaration
  * \param  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * \return None
  */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    if (patch_TIM_Cmd)
    {
        if (patch_TIM_Cmd(TIMx, NewState))
        {
            return;
        }
    }

    if (NewState != DISABLE)
    {
        timer_share_base->TIMER_EN_CTRL |= BIT(timerid);
    }
    else
    {
        timer_share_base->TIMER_EN_CTRL &= ~(BIT(timerid));
    }
}

/**
  * \brief  change TIM period value.
  * \param  TIMx: Select the TIM peripheral. \ref TIMER_Declaration
  * \return The new state of success or not  (SET or RESET).
  */
void TIM_ChangePeriod(TIM_TypeDef *TIMx, uint32_t period)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (patch_TIM_ChangePeriod)
    {
        if (patch_TIM_ChangePeriod(TIMx, period))
        {
            return;
        }
    }

    TIMx->TIMER_MAX_CNT = period;

    return;
}

/**
  * \brief  Enables or disables the specified TIMx interrupt.
  * \param  TIMx: Select the TIM peripheral. \ref TIMER_Declaration
  * \param  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * \return None
  */
void TIM_InterruptConfig(TIM_TypeDef *TIMx, uint8_t TIM_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    /* Enable the Interrupt sources */
    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIMCASE_TypeDef timercase = TIM_GetCase(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    if (patch_TIM_InterruptConfig)
    {
        if (patch_TIM_InterruptConfig(TIMx, TIM_INT, NewState))
        {
            return;
        }
    }

    if (NewState != DISABLE)
    {
        if (TIM_INT & TIM_INT_STS)
        {
            timer_share_base->TIMER_INTR_EN_CTRL |= BIT(timerid);
        }
        if ((TIM_INT & TIM_INT_LATCH_CNT_FIFO_FULL) && \
            (timercase == TIM_CASE4 || timercase == TIM_CASE5))
        {
            timer_share_base->TIMER_LAT_CNT_0_FIFO_FULL_INTR_EN_CTRL |= BIT(timerid);
        }
        if ((TIM_INT & TIM_INT_LATCH_CNT_FIFO_THD) && \
            (timercase == TIM_CASE4 || timercase == TIM_CASE5))
        {
            timer_share_base->TIMER_LAT_CNT_0_FIFO_MET_THRESHOLD_INTR_EN_CTRL |= BIT(timerid);
        }
        if (TIM_INT & TIM_INT_PAUSE)
        {
            timer_share_base->TIMER_PAUSE_INTR_EN_CTRL |= BIT(timerid);
        }
    }
    else
    {

        if (TIM_INT & TIM_INT_STS)
        {
            timer_share_base->TIMER_INTR_EN_CTRL &= ~(BIT(timerid));
        }
        if ((TIM_INT & TIM_INT_LATCH_CNT_FIFO_FULL) && \
            (timercase == TIM_CASE4 || timercase == TIM_CASE5))
        {
            timer_share_base->TIMER_LAT_CNT_0_FIFO_FULL_INTR_EN_CTRL &= ~(BIT(timerid));
        }
        if ((TIM_INT & TIM_INT_LATCH_CNT_FIFO_THD) && \
            (timercase == TIM_CASE4 || timercase == TIM_CASE5))
        {
            timer_share_base->TIMER_LAT_CNT_0_FIFO_MET_THRESHOLD_INTR_EN_CTRL &= ~(BIT(timerid));
        }
        if (TIM_INT & TIM_INT_PAUSE)
        {
            timer_share_base->TIMER_PAUSE_INTR_EN_CTRL &= ~(BIT(timerid));
        }
    }
}

/**
  * \brief  Change PWM freq and duty
  * \param  TIMx: Select the TIM peripheral. \ref TIMER_Declaration
  * \param  period: Specifies the period value.
  * \param  compare: Specifies the capture/compare value.
  * \return None
  */
void TIM_PWMChangeFreqAndDuty(TIM_TypeDef *TIMx, uint32_t period, uint32_t high_count)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMER_MODE_CFG_TypeDef timer_0x04 = {.d32 = TIMx->TIMER_MODE_CFG};
    TIMER_PWM_CFG_TypeDef timer_0x14 = {.d32 = TIMx->TIMER_PWM_CFG};
    uint16_t tim_mode = timer_0x04.b.reg_timer_mode;
    uint16_t tim_polarity = timer_0x14.b.reg_timer_pwm_polarity;

    TIMx->TIMER_MAX_CNT = period;

    if (tim_polarity == TIM_PWM_START_WITH_HIGH)
    {
        if (high_count > period)
        {
            high_count = period;
        }
        if (tim_mode == TIM_Mode_UserDefine_Manual)
        {
            TIMx->TIMER_CCR = (period - high_count);
        }
        else if (tim_mode == TIM_Mode_UserDefine_Auto)
        {
            TIMx->TIMER_CCR_FIFO = (period - high_count);
        }
    }
    else
    {
        if (tim_mode == TIM_Mode_UserDefine_Manual)
        {
            TIMx->TIMER_CCR = high_count;
        }
        else if (tim_mode == TIM_Mode_UserDefine_Auto)
        {
            TIMx->TIMER_CCR_FIFO = high_count;
        }
    }
}

/**
 * \brief   Get TIMx current value when timer is running.
 * \param   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \return  The counter value.
 */
uint32_t TIM_GetCurrentValue(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if (patch_TIM_GetCurrentValue)
    {
        uint32_t ret;
        if (patch_TIM_GetCurrentValue(TIMx, &ret))
        {
            return ret;
        }
    }

    return TIMx->TIMER_CUR_CNT;
}

/**
 * \brief   Check whether the TIM interrupt has occurred or not.
 * \param   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \return  The new state of the TIM_INTR(SET or RESET).
 */
ITStatus TIM_GetInterruptStatus(TIM_TypeDef *TIMx, uint8_t TIM_INT)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIMCASE_TypeDef timercase = TIM_GetCase(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);
    ITStatus itstatus = RESET;

    if (patch_TIM_GetInterruptStatus)
    {
        ITStatus ret;
        if (patch_TIM_GetInterruptStatus(TIMx, TIM_INT, &ret))
        {
            return ret;
        }
    }

    if (TIM_INT == TIM_INT_STS)
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_INTR) >> (timerid));
    }
    else if ((TIM_INT == TIM_INT_LATCH_CNT_FIFO_FULL) && \
             (timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_LAT_CNT_0_FIFO_FULL_INTR) >> (timerid));
    }
    else if ((TIM_INT == TIM_INT_LATCH_CNT_FIFO_THD) && \
             (timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_LAT_CNT_0_FIFO_MET_THRESHOLD_INTR) >> (timerid));
    }
    else if (TIM_INT == TIM_INT_PAUSE)
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_PAUSE_INTR) >> (timerid));
    }
    else
    {
        itstatus = RESET;
    }

    return itstatus;
}

/**
 * \brief   Check whether the TIM is in operation or not.
 * \param   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \return  The new state of the timer operation status (SET or RESET).
 */
ITStatus TIM_GetOperationStatus(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    ITStatus itstatus = (ITStatus)((timer_share_base->TIMER_EN_CTRL) >> (timerid));

    return itstatus;
}

/**
 * \brief  Get TIMx elapsed value when timer is running.
 * \param  TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \return The elapsed counter value.
 */
uint32_t TIM_GetElapsedValue(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    return (TIMx->TIMER_MAX_CNT - TIMx->TIMER_CUR_CNT);
}

/**
 * \brief   Clear TIM interrupt.
 * \param   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \return  None.
 */
void TIM_ClearInterrupt(TIM_TypeDef *TIMx, uint8_t TIM_INT)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIMCASE_TypeDef timercase = TIM_GetCase(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    if (patch_TIM_ClearInterrupt)
    {
        if (patch_TIM_ClearInterrupt(TIMx, TIM_INT))
        {
            return;
        }
    }

    /* Clear the IT */
    if (TIM_INT & TIM_INT_STS)
    {
        timer_share_base->TIMER_INTR_STS |= BIT(timerid);
    }
    if ((TIM_INT & TIM_INT_LATCH_CNT_FIFO_FULL) && \
        (timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        timer_share_base->TIMER_LAT_CNT_0_FIFO_FULL_INTR |= BIT(timerid);
    }
    if ((TIM_INT & TIM_INT_LATCH_CNT_FIFO_THD) && \
        (timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        timer_share_base->TIMER_LAT_CNT_0_FIFO_MET_THRESHOLD_INTR |= BIT(timerid);
    }
    if (TIM_INT & TIM_INT_PAUSE)
    {
        timer_share_base->TIMER_PAUSE_INTR_STS |= BIT(timerid);
    }
}

/**
  * \brief  Activates one-shot mode of the specified TIM peripheral.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \return None.
  */
#if TIM_SUPPORT_ONESHOT_CMD
void TIM_OneShotActive(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    timer_share_base->TIMER_ONESHOT_GO_CTRL |= BIT(timerid);
}

ITStatus TIM_GetOneshotStatus(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    ITStatus itstatus = (ITStatus)((timer_share_base->TIMER_ONESHOT_GO_CTRL) >> (timerid));

    return itstatus;
}
#endif

/**
  * \brief  Read TIMx latch counter0 fifo data.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  length: Latch count0 fifo length, max 8.
  * \pBuf   pBuf: FIFO data out buffer.
  * \return None.
  */
void TIM_ReadLatchCountFIFO(TIM_TypeDef *TIMx, uint32_t *pBuf, uint8_t length)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    if ((pBuf == NULL) || (length > 8))
    {
        return;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        pBuf[i] = TIMx->TIMER_LAT_CNT_0;
    }
}

/**
  * \brief  Change TIM pwm_phase_shift count
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  PhaseShiftCnt:This parameter can be 0 to ccr;
  * \return None
  */
#if TIM_SUPPORT_PWM_PHASE_SHIFT
void TIM_PhaseShiftCnt(TIM_TypeDef *TIMx, uint32_t PhaseShiftCnt)
{
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMx->TIMER_PWM_SHIFT_CNT = PhaseShiftCnt;
}
#endif

/**
  * \brief  Set Max Count value.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  count: Max counter value for user-define PWM mode (leagel value: 1 ~ 2^32-2).
  * \return None
  */
void TIM_SetMaxCount(TIM_TypeDef *TIMx, uint32_t count)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMx->TIMER_MAX_CNT = count & 0xFFFFFFFE;
}

/**
  * \brief  Set TIMx capture/compare value for user-define PWM manual mode.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  value: User-defined TIM capture/compare value for PWM manual mode,
  *         ranging from 0 to 2^31.
  * \return None
  */
void TIM_SetCCValue(TIM_TypeDef *TIMx, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMx->TIMER_CCR = value;
}

/**
  * \brief  Set TIMx capture/compare value for user-define PWM auto mode.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  value:  User-defined TIM capture/compare value for PWM auto mode,
  *         ranging from 0 to 2^31.
  * \return None
  */
void TIM_WriteCCFIFO(TIM_TypeDef *TIMx, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    TIMx->TIMER_CCR_FIFO = value;
}

/**
  * \brief  Get TIM latch counter0 fifo length.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \return FIFO data length.
  */
uint8_t TIM_GetLatchCountFIFOLength(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    return (uint8_t)(TIMx->TIMER_LAT_CNT_0_FIFO_DEPTH) & 0xFF;
}

/**
  * \brief  Get TIM toggle state
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \return The new state of toggle state(true or false).
  */
bool TIM_GetToggleState(TIM_TypeDef *TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIMCASE_TypeDef timercase = TIM_GetCase(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);

    if (((timer_share_base->TIMER_PWM_STATE & BIT(timerid)) != 0) && \
        (timercase != TIM_CASE1))
    {
        return true;
    }

    return false;
}

/**
  * \brief  Check whether the specified TIM flag is set.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  TIM_FLAG: Specifies the flag to check.
  * \return The new state of TIM_FLAG (SET or RESET).
  */
FlagStatus TIM_GetFIFOFlagStatus(TIM_TypeDef *TIMx, uint32_t TIM_FLAG)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);
    TIMCASE_TypeDef timercase = TIM_GetCase(TIMx);
    FlagStatus itstatus = RESET;

    if ((TIM_FLAG == TIM_FLAG_CCR_FIFO_FULL) && \
        (timercase == TIM_CASE2 || timercase == TIM_CASE3 || \
         timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_CCR_FIFO_FULL_STS) >> (timerid));
    }
    else if ((TIM_FLAG == TIM_FLAG_CCR_FIFO_EMPTY) && \
             (timercase == TIM_CASE2 || timercase == TIM_CASE3 || \
              timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_CCR_FIFO_EMPTY_STS) >> (timerid));
    }
    else if ((TIM_FLAG == TIM_FLAG_LATCH_CNT_FIFO_EMPTY) && \
             (timercase == TIM_CASE4 || timercase == TIM_CASE5))
    {
        itstatus = (ITStatus)((timer_share_base->TIMER_LAT_CNT_0_FIFO_EMPTY_STS) >> (timerid));
    }
    else
    {
        itstatus = RESET;
    }

    return itstatus;
}

/**
  * \brief  Clear capture/compare or latch count0 fifo.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  FIFO_CLR: Specifies the FIFO type which to be clear.
  * \return FIFO data length.
  */
void TIM_ClearFIFO(TIM_TypeDef *TIMx, uint8_t FIFO_CLR)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));

    uint32_t timerid = TIM_GetTimerID(TIMx);

    if (FIFO_CLR == TIM_FIFO_CLR_CCR)
    {
        TIMER_MODE_CFG_TypeDef timer_0x04 = {.d32 = TIMx->TIMER_MODE_CFG};
        timer_0x04.b.reg_timer_ccr_fifo_clr = 1;
        TIMx->TIMER_MODE_CFG = timer_0x04.d32;
    }
    else if (FIFO_CLR == TIM_FIFO_CLR_CNT)
    {
        TIMER_LAT_CNT_0_CFG_TypeDef timer_0x1c = {.d32 = TIMx->TIMER_LAT_CNT_0_CFG};
        timer_0x1c.b.reg_timer_latch_cnt_0_fifo_clr = 1;
        TIMx->TIMER_LAT_CNT_0_CFG = timer_0x1c.d32;
    }
}

void TIM_PauseCmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    uint32_t timerid = TIM_GetTimerID(TIMx);
    TIM_ShareTypeDef *timer_share_base = TIM_GetTimerShareBase(TIMx);
    if (NewState != DISABLE)
    {
        timer_share_base->TIMER_PAUSE_CTRL |= BIT(timerid);
    }
    else
    {
        timer_share_base->TIMER_PAUSE_CTRL &= ~(BIT(timerid));
    }
}
/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/
