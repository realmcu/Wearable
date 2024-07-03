/**
**********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_enh_tim.c
* @brief    This file provides all the Timer firmware functions.
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_enh_tim.h"
#include <stdbool.h>

/**
 * \brief   Initialize the ENH_TIMx time base unit peripheral according to
 *          the specified parameters in ENH_TIM_TimeBaseInitStruct.
 * \param[in] ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \param[in] ENH_TIM_TimeBaseInitStruct: Pointer to a ENH_TIM_TimeBaseInitTypeDef
 *            structure that contains the configuration information for the selected  ENH_TIM peripheral.
 * \return  None.
 */
void ENH_TIM_TimeBaseInit(ENH_TIM_TypeDef *ENH_TIMx,
                          ENH_TIM_TimeBaseInitTypeDef *ENH_TIM_TimeBaseInitStruct)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));
    assert_param(IS_TIM_CLOCK_SOURCE(ENH_TIM_TimeBaseInitStruct->TIM_ClockSrc));
    assert_param(IS_TIM_SOURCE_DIVIDER(ENH_TIM_TimeBaseInitStruct->TIM_ClockSrcDiv));
    assert_param(IS_TIM_MODE(ENH_TIM_TimeBaseInitStruct->TIM_Mode));
    assert_param(IS_TIM_OneShotEn(ENH_TIM_TimeBaseInitStruct->TIM_OneShotEn));

    temp_reg = (uint32_t)ENH_TIMx;

    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);


    switch (timer_id)
    {
    case ENH_TIM_0_ID:

        SYSBLKCTRL->u_36C.BITS_36C.etimer_0_div_en = 1;
        SYSBLKCTRL->u_36C.BITS_36C.etimer_0_div_sel = ENH_TIM_TimeBaseInitStruct->TIM_ClockSrcDiv;

        break;
    case ENH_TIM_1_ID:
        SYSBLKCTRL->u_36C.BITS_36C.etimer_1_div_en = 1;
        SYSBLKCTRL->u_36C.BITS_36C.etimer_1_div_sel = ENH_TIM_TimeBaseInitStruct->TIM_ClockSrcDiv;
        break;
    case ENH_TIM_2_ID:
        SYSBLKCTRL->u_36C.BITS_36C.etimer_2_div_en = 1;
        SYSBLKCTRL->u_36C.BITS_36C.etimer_2_div_sel = ENH_TIM_TimeBaseInitStruct->TIM_ClockSrcDiv;
        break;
    case ENH_TIM_3_ID:
        SYSBLKCTRL->u_36C.BITS_36C.etimer_3_div_en = 1;
        SYSBLKCTRL->u_36C.BITS_36C.etimer_3_div_sel = ENH_TIM_TimeBaseInitStruct->TIM_ClockSrcDiv;
        break;

    default:
        break;
    }

    ENH_TIM_CTRL->REG_ENH_TIMER_INTR_STS = BIT(timer_id);
    ENH_TIMx->REG_ENH_TIMER_x_MODE_CFG = ENH_TIM_TimeBaseInitStruct->TIM_Mode;
    ENH_TIMx->REG_ENH_TIMER_x_MAX_CNT = ENH_TIM_TimeBaseInitStruct->TIM_Period;

    if (ENH_TIM_TimeBaseInitStruct->TIM_OneShotEn == ENH_TIM_ONE_SHOT_DISABLE)
    {
        ENH_TIMx->REG_ENH_TIMER_x_MODE_CFG &= ~ BIT8;
    }
    else if (ENH_TIM_TimeBaseInitStruct->TIM_OneShotEn == ENH_TIM_ONE_SHOT_ENABLE)
    {
        ENH_TIMx->REG_ENH_TIMER_x_MODE_CFG |= BIT8;
    }
}

/**
  * @brief  Fills each ENH_TIM_InitStruct member with its default value.
  * @param  ENH_TIM_InitStruct : pointer to a ENH_TIM_InitTypeDef structure which will be initialized.
  * @retval None
  */
void ENH_TIM_StructInit(ENH_TIM_TimeBaseInitTypeDef *ENH_TIM_TimeBaseInitStruct)
{
//    ENH_TIM_TimeBaseInitStruct->TIM_ClockSrc = ENH_TIM_CLOCK_SOURCE_40MHZ;
    ENH_TIM_TimeBaseInitStruct->TIM_ClockSrcDiv = CLK_DIV_1;
    ENH_TIM_TimeBaseInitStruct->TIM_Mode = ENH_TIM_Mode_UserDefine;
    ENH_TIM_TimeBaseInitStruct->TIM_Period = 0xfff;
    ENH_TIM_TimeBaseInitStruct->TIM_OneShotEn = ENH_TIM_ONE_SHOT_DISABLE;
}

/**
 * \brief   Enables or disables the specified  ENH_TIM peripheral.
 * \param[in] ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \param[in] NewState: New state of the ENH_TIMx peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 */
void ENH_TIM_Cmd(ENH_TIM_TypeDef *ENH_TIMx, FunctionalState NewState)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);

    if (NewState == DISABLE)
    {
        // disable rtk etimer
        ENH_TIM_CTRL->REG_ENH_TIMER_EN_CTRL &= ~BIT(timer_id);
    }
    else
    {
        // enable rtk etimer
        ENH_TIM_CTRL->REG_ENH_TIMER_EN_CTRL |= BIT(timer_id);
    }
}

/**
 * \brief   Activates one-shot mode of the specified  ENH_TIM peripheral.
 * \param[in] ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \return  None.
 */
void ENH_TIM_OneShotActivate(ENH_TIM_TypeDef *ENH_TIMx)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);

    ENH_TIM_CTRL->REG_ENH_TIMER_ONESHOT_GO_CTRL |= BIT(timer_id);
}


/**
 * \brief       Change  ENH_TIM period value.
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \param[in]   period: Period value to be changed.
 * \return  None.
 */
void ENH_TIM_ChangePeriod(ENH_TIM_TypeDef *ENH_TIMx, uint32_t period)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    ENH_TIMx->REG_ENH_TIMER_x_MAX_CNT = period;

    return;
}

/**
 * \brief       Enables or disables the specified ENH_TIMx interrupt.
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \param[in]   NewState: New state of the ENH_TIMx peripheral.
 *              This parameter can be: ENABLE or DISABLE.
 * \return      None.
 */
void ENH_TIM_INTConfig(ENH_TIM_TypeDef *ENH_TIMx, FunctionalState NewState)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;


    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);

    if (NewState != DISABLE)
    {
        ENH_TIM_CTRL->REG_ENH_TIMER_INTR_EN_CTRL |= BIT(timer_id);
    }
    else
    {
        ENH_TIM_CTRL->REG_ENH_TIMER_INTR_EN_CTRL &= ~BIT(timer_id);
    }
}


/**
 * \brief       Get  specified ENH_TIMx interrupt is active or not .
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral
 * \return      The enhtimer_intr is wether active or not after masking.
                0 enhtimer_intr is not active after masking
                1 enhtimer_intr is active after masking
 */
bool ENH_TIM_INTActive(ENH_TIM_TypeDef *ENH_TIMx)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);

    return (ENH_TIM_CTRL->REG_ENH_TIMER_INTR & BIT(timer_id));
}
/**
 * \brief   Check whether the  ENH_TIM interrupt has occurred or not.
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \return  The new state of the ENH_TIM_IT(SET or RESET).
 */
ITStatus ENH_TIM_GetINTStatus(ENH_TIM_TypeDef *ENH_TIMx)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);


    if ((ENH_TIM_CTRL->REG_ENH_TIMER_INTR_STS & BIT(timer_id)) != (uint32_t)RESET)
    {
        bit_status = SET;
    }

    return bit_status;
}

/**
 * \brief   Clear  ENH_TIM interrupt.
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \return  None.
 */
void ENH_TIM_ClearINT(ENH_TIM_TypeDef *ENH_TIMx)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;


    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);

    ENH_TIM_CTRL->REG_ENH_TIMER_INTR_STS = BIT(timer_id);
}

/**
 * \brief   Check whether the  ENH_TIM is in operation or not.
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \return  The new state of the timer operation status (SET or RESET).
 */
ITStatus ENH_TIM_GetEnableStatus(ENH_TIM_TypeDef *ENH_TIMx)
{
    uint32_t timer_id = 0;
    uint32_t temp_reg = 0;

    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    temp_reg = (uint32_t)ENH_TIMx;
    timer_id = (temp_reg - ENH_TIMER0_BASE) / sizeof(ENH_TIM_TypeDef);

    if ((ENH_TIM_CTRL->REG_ENH_TIMER_EN_CTRL & BIT(timer_id)) != (uint32_t)RESET)
    {
        bit_status = SET;
    }

    return bit_status;
}

/**
 * \brief   Get ENH_TIMx current value when timer is running.
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral.
 * \return  The counter value.
 */
uint32_t ENH_TIM_GetCurrentValue(ENH_TIM_TypeDef *ENH_TIMx)
{
    /* Check the parameters */
    assert_param(IS_TIM_ALL_PERIPH(ENH_TIMx));

    return ENH_TIMx->REG_ENH_TIMER_x_CUR_CNT;
}


/******************* (C) COPYRIGHT 2022 Realtek Semiconductor Corporation *****END OF FILE****/
