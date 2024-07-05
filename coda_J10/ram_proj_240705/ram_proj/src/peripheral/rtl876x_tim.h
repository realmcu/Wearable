/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_tim.h
* \brief    The header file of the peripheral TIMER driver.
* \details  This file provides all TIMER firmware functions.
* \author
* \date
* \version  v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef RTL876X_TIM_H
#define RTL876X_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x_tim_def.h"

/** \defgroup TIM         TIM
  * \brief
  * \{
  */
/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup TIM_Exported_Constants TIM Exported Constants
  * \brief
  * \{
  */

/**
 * \defgroup    TIM_CASE TIM CASE
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_CASE1 = 1,
    TIM_CASE2 = 2,
    TIM_CASE3 = 3,
    TIM_CASE4 = 4,
    TIM_CASE5 = 5,
    TIM_CASE6 = 6,
    TIM_CASE7 = 7,
} TIMCASE_TypeDef;

/** End of TIM_CASE
  * \}
  */

/**
 * \defgroup    PWM_DeadZone_Clock_Source PWM DeadZone Clock Source
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    PWM_CK_32K_TIMER = 0x0,
    PWM_CK_TIM1_CH9_DIVIDER_TIMER = 0x1,
} PWMDZClockSrc_TypeDef;

#define IS_TIM_PWM_DZ_SOURCE(PERIPH)     (((PERIPH) == PWM_CK_32K_TIMER) || \
                                          ((PERIPH) == PWM_CK_TIM1_CH9_DIVIDER_TIMER))

/** End of PWM_DeadZone_Clock_Source
  * \}
  */

/**
 * \defgroup    TIM_Mode TIM Mode
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_Mode_FreeRun = 0x0,
    TIM_Mode_UserDefine_Auto = 0x1,
    TIM_Mode_UserDefine_Manual = 0x2,
} TIMMode_TypeDef;

#define IS_TIM_MODE(mode) (((mode) == TIM_Mode_FreeRun) || \
                           ((mode) == TIM_Mode_UserDefine_Auto) || \
                           ((mode) == TIM_Mode_UserDefine_Manual))

/** End of TIM_Mode
  * \}
  */

/**
 * \defgroup    TIM_PWM_Mode_Enable TIM PWM Mode Enable
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define IS_TIM_PWM_En(mode) (((mode) == ENABLE) || \
                             ((mode) == DISABLE))

/** End of TIM_PWM_Mode_Enable
  * \}
  */

/**
 * \defgroup    PWM_DeadZone_Enable PWM DeadZone Enable
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define IS_TIM_PWM_DeadZone_En(mode) (((mode) == ENABLE) || \
                                      ((mode) == DISABLE))

/** End of PWM_DeadZone_Enable
  * \}
  */

/**
 * \defgroup    PWM_DeadZone_Stop_State PWM DeadZone Stop State
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    PWM_STOP_AT_LOW = 0x0,
    PWM_STOP_AT_HIGH = 0x1,
} PWMDZStopState_TypeDef;

#define IS_PWM_STOP_STATE(STATE) (((STATE) == PWM_STOP_AT_LOW) || \
                                  ((STATE) == PWM_STOP_AT_HIGH))

/** End of PWM_DeadZone_Stop_State
  * \}
  */

/**
 * \defgroup    TIM_Latch_Trigger_Mode TIM Latch Trigger Mode
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_LATCH_TRIGGER_RISING_EDGE = 0x00,
    TIM_LATCH_TRIGGER_FALLING_EDGE = 0x01,
    TIM_LATCH_TRIGGER_BOTH_EDGE = 0x02,
} TIMLatchTriggleMode_TypeDef;

#define IS_TIM_LATCH_TRIG_Mode(mode) (((mode) == TIM_LATCH_TRIGGER_BOTH_EDGE) || \
                                      ((mode) == TIM_LATCH_TRIGGER_FALLING_EDGE) || \
                                      ((mode) == TIM_LATCH_TRIGGER_RISING_EDGE))

/** End of TIM_Latch_Trigger_Mode
  * \}
  */

/**
 * \defgroup    TIM_DMA_TARGET TIM DMA TARGET
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_DMA_CCR_FIFO = 0x00,
    TIM_DMA_LC_FIFO = 0x01,
} TIMDmaTarget_TypeDef;

#define IS_TIM_DMA_TARGET(mode) (((mode) == TIM_DMA_CCR_FIFO) || \
                                 ((mode) == TIM_DMA_LC_FIFO))

/** End of TIM_DMA_TARGET
  * \}
  */

/**
 * \defgroup    TIM_PWM_Polarity TIM PWM Polarity
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_PWM_START_WITH_LOW = 0x00,
    TIM_PWM_START_WITH_HIGH = 0x01,
} TIMPWMPolarity_TypeDef;

#define IS_TIM_PWM_POLARITY(Pol) (((Pol) == TIM_PWM_START_WITH_HIGH) || \
                                  ((Pol) == TIM_PWM_START_WITH_LOW))

/** End of TIM_PWM_Polarity
  * \}
  */

/**
 * \defgroup    TIM_Interrupts TIM Interrupts
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define TIM_INT_STS                            (1 << 0)
#define TIM_INT_LATCH_CNT_FIFO_FULL            (1 << 1)
#define TIM_INT_LATCH_CNT_FIFO_THD             (1 << 2)
#define TIM_INT_PAUSE                          (1 << 3)

#define IS_TIM_INT(INT) (((INT) == TIM_INT_STS) || \
                         ((INT) == TIM_INT_LATCH_CNT_FIFO_FULL) || \
                         ((INT) == TIM_INT_LATCH_CNT_FIFO_THD) || \
                         ((INT) == TIM_INT_PAUSE))

/** End of TIM_Interrupts
  * \}
  */

#if TIM_SUPPORT_PWM_SRC_SELECT
/**
 * \brief       TIM PWM reference selection
 *
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_DZ_REF_PWMPN = 0x0,
    TIM_DZ_REF_PWMNN = 0x1,
    TIM_DZ_REF_PWMPP = 0x2,
    TIM_DZ_REF_PWMNP = 0x3,
} TIMPWMDZRef_TypeDef;
#endif

/**
 * \brief       TIM PWM Open Drain
 *
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    PWM_PUSH_PULL = 0x0,
    PWM_OPEN_DRAIN = 0x1,
} PWMOpenDrain_TypeDef;

/**
 * \defgroup    TIM_Flag TIM Flag
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define TIM_FLAG_CCR_FIFO_EMPTY                 0
#define TIM_FLAG_CCR_FIFO_FULL                  1
#define TIM_FLAG_LATCH_CNT_FIFO_EMPTY           2

#define IS_TIM_FLAG(flag) (((flag) == TIM_FLAG_CCR_FIFO_EMPTY) || \
                           ((flag) == TIM_FLAG_CCR_FIFO_FULL) || \
                           ((flag) == TIM_FLAG_LATCH_CNT_FIFO_EMPTY))

/** End of TIM_Flag
  * \}
  */

/**
 * \defgroup    TIM_FIFO_Clear_Flag TIM FIFO Clear Flag
 * \{
 * \ingroup     TIM_Exported_Constants
 */
#define TIM_FIFO_CLR_CCR                 (0)
#define TIM_FIFO_CLR_CNT                 (8)

/** End of TIM_FIFO_Clear_Flag
  * \}
  */

/**
     * \defgroup    TIM_Clock_Source TIM Clock Source
     * \{
     * TIML_CH8 supports CK_PLL1_TIMER and CK_PLL2_TIMER and CK_PLL3_TIMER and CK_40M_TIMER.
     * The other channels of timer only support CK_40M_TIMER.
     * \ingroup     TIM_Exported_Constants
     */
typedef enum
{
    CK_PLL2_TIMER = 0x0,
    CK_PLL3_TIMER = 0x1,
    CK_PLL1_TIMER = 0x2,
    CK_40M_TIMER  = 0x3,
} TIMClockSrc_TypeDef;

#define IS_TIM_CLK_SOURCE(PERIPH)     (((PERIPH) == CK_PLL2_TIMER) || \
                                       ((PERIPH) == CK_PLL3_TIMER) || \
                                       ((PERIPH) == CK_PLL1_TIMER) || \
                                       ((PERIPH) == CK_40M_TIMER))

/** End of TIM_Clock_Source
  * \}
  */

/**
 * \defgroup    TIM_Clock_Divider TIM Clock Divider
 * \{
 * \ingroup     TIM_Exported_Constants
 */
typedef enum
{
    TIM_CLOCK_DIVIDER_1 = 0x0,
    TIM_CLOCK_DIVIDER_2 = 0x1,
    TIM_CLOCK_DIVIDER_4 = 0x2,
    TIM_CLOCK_DIVIDER_8 = 0x3,
    TIM_CLOCK_DIVIDER_16 = 0x4,
    TIM_CLOCK_DIVIDER_32 = 0x5,
    TIM_CLOCK_DIVIDER_40 = 0x6,
    TIM_CLOCK_DIVIDER_64 = 0x7,
} TIMClockDiv_TypeDef;

#define IS_TIM_CLK_DIV(DIV) (((DIV) == TIM_CLOCK_DIVIDER_1) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_2) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_4) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_8) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_16) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_32) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_40) || \
                             ((DIV) == TIM_CLOCK_DIVIDER_64))

/** End of TIM_Clock_Divider
  * \}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** \defgroup TIM_Exported_Types TIM Exported Types
  * \brief
  * \{
  */

/**
 * \brief       TIM init structure definition.
 *
 * \ingroup     TIM_Exported_Types
 */
typedef struct
{
    TIMClockSrc_TypeDef TIM_ClockSrc;            /*!< Specifies the clock source. */

    TIMClockDiv_TypeDef TIM_SOURCE_DIV;          /*!< Specifies the clock source divider. */

    TIMMode_TypeDef TIM_Mode;                    /*!< Specifies the counter mode. */

    FunctionalState TIM_PWM_En;                  /*!< Specifies the PWM mode.
                                                         This parameter can be a value of DISABLE or ENABLE. */
    TIMPWMPolarity_TypeDef TIM_PWMStartPolarity; /*!< Specifies timer toggle output polarity
                                                         for user-define PWM mode.*/
    uint32_t TIM_Period;                         /*!< Specifies timer max counter value for user-define mode.
                                                         This parameter must range from 1 to 2^32-2. */

    uint32_t TIM_PWM_High_Count;                 /*!< Specifies the PWM High Count.
                                                         This parameter must range from 0 to TIM_Period. */

    PWMDZClockSrc_TypeDef PWM_Deazone_ClockSrc;  /*!< Specifies the pwm deazone clock source. */

    TIMClockDiv_TypeDef PWM_Deadzone_DIV;        /*!< Specifies the pwm deazone clock source divider. */

    uint32_t PWM_Deazone_Size;                   /*!< Size of deadzone time, DeadzoneTime=deadzone_size / deadzone_clock.
                                                         This parameter must range from 1 to 0xff. */

    FunctionalState PWMDeadZone_En;              /*!< PWM Deadzone enable.
                                                         This parameter can be a value of ENABLE or DISABLE. */

    PWMDZStopState_TypeDef PWM_Stop_State_P;     /*!< Specifies the PWM P stop state. */

    PWMDZStopState_TypeDef PWM_Stop_State_N;     /*!< Specifies the PWM N stop state. */

    FunctionalState
    PWM_DZ_INV_P;                /*!< Specifies ENHTIM invertion of pwm_p reference selected by ref_sel */

    FunctionalState
    PWM_DZ_INV_N;                /*!< Specifies ENHTIM invertion of pwm_n reference selected by ref_sel */

    PWMOpenDrain_TypeDef PWM_OpenDrain;          /*!< Specifies enable PWM open drain. */

    FunctionalState TIM_LatchCountEn[3];         /*!< Specifies enbale TIM Latch_cnt.
                                                         This parameter can be a value of DISABLE or ENABLE. */
    TIMLatchTriggleMode_TypeDef
    TIM_LatchCountTrigger[3];                    /*!< Specifies TIM counter latch trigger mode. */

    uint16_t TIM_LatchCountThd;                  /*!< Specifies TIM latched counter fifo threshold.
                                                         This parameter can be value of  1~7. */

    uint16_t TIM_LatchTriggerPad;                /*!< Specifies TIM counter latch trigger pad.
                                                         This parameter can be a value of P0_0 to DAOUT_N. */

    uint16_t TIM_LatchTriggerDebSize;            /*!< Size of GPIO trigger(latch counter 0) debounce time
                                                         This parameter must range from 0 to 65535. */

    FunctionalState TIM_LatchTriggerDebEn;       /*!< GPIO trigger(latch counter 0) debounce enable.
                                                         This parameter can be a value of ENABLE or DISABLE. */

    TIMClockDiv_TypeDef
    TIM_LatchTrigDebClkDiv;  /*!< Specifies the GPIO trigger(latch counter 0) debounce clock source divider. */

    FunctionalState TIM_DmaEn;                   /*!< Specifies enable TIM DMA.
                                                         This parameter can be a value of DISABLE or ENABLE. */

    TIMDmaTarget_TypeDef TIM_DmaTragget;         /*!< Specifies TIM DMA target. */

    uint8_t TIM_OneShotEn;                       /*!< Specifies the one shot mode.
                                                         This parameter can be a value of DISABLE or ENABLE.*/

} TIM_TimeBaseInitTypeDef;

/** End of TIM_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup TIM_Exported_Functions TIM Exported Functions
  * \brief
  * \{
  */

/**
  * @brief  Enable or Disable TIM peripheral clock.
  * @param  is_enable:
  *  @args  true:  enable hw timer hw clock
  *  @args  false: disable hw timer hw clock
  * @retval None
  */
void TIM_HwClock(bool is_enable);

/**
 * \brief   Initialize the TIMx time base unit peripheral according to
 *          the specified parameters in TIM_TimeBaseInitStruct.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in] TIM_TimeBaseInitStruct: Pointer to a TIM_TimeBaseInitTypeDef
 *            structure that contains the configuration information for the selected TIM peripheral.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1 ;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 * }
 * \endcode
 */
void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Fills each TIM_InitStruct member with its default value.
 *
 * \param[in] TIM_TimeBaseInitStruct: Pointer to a TIM_TimeBaseInitTypeDef structure which will be initialized.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 * }
 * \endcode
 */
void TIM_StructInit(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Enables or disables the specified TIM peripheral.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in] NewState: New state of the TIMx peripheral.
 *            This parameter can be: ENABLE or DISABLE.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 *     TIM_Cmd(TIM6, ENABLE);
 * }
 * \endcode
 */
void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief       Enables or disables the specified TIMx interrupt.
 *
 * \param[in]   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in]   NewState: New state of the TIMx peripheral.
 *              This parameter can be: ENABLE or DISABLE.
 *
 * \return      None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_PWM_En = DISABLE;
 *     TIM_InitStruct.TIM_Period = 1000000 - 1;
 *     TIM_InitStruct.TIM_Mode = TIM_Mode_UserDefine;
 *     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);
 *     TIM_ClearInterrupt(TIM6);
 *     TIM_InterruptConfig(TIM6, ENABLE);
 */
void TIM_InterruptConfig(TIM_TypeDef *TIMx, uint8_t TIM_INT, FunctionalState NewState);

/**
 * \brief       Change TIM period value.
 *
 * \param[in]   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in]   period: Period value to be changed.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t new_period = 1000000 - 1;
 *     TIM_Cmd(TIM6, DISABLE);
 *     TIM_ChangePeriod(TIM6, new_period);
 *
 * }
 * \endcode
 */
void TIM_ChangePeriod(TIM_TypeDef *TIMx, uint32_t period);

/**
 * \brief   Change PWM freq and duty
 *
 * \param[in]   TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 * \param[in]   period: Specifies the period value.
 * \param[in]  compare: Specifies the capture/compare value.
 *
 * \return  None.
 */
void TIM_PWMChangeFreqAndDuty(TIM_TypeDef *TIMx, uint32_t period, uint32_t high_count);

/**
 * \brief   Get TIMx current value when timer is running.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  The counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     uint32_t cur_value = TIM_GetCurrentValue(TIM6);
 * }
 * \endcode
 */
uint32_t TIM_GetCurrentValue(TIM_TypeDef *TIMx);

/**
 * \brief   Check whether the TIM interrupt has occurred or not.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  The new state of the TIM_IT(SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     ITStatus int_status = TIM_GetInterruptStatus(TIM6);
 * }
 * \endcode
 */
ITStatus TIM_GetInterruptStatus(TIM_TypeDef *TIMx, uint8_t TIM_INT);

/**
 * \brief   Check whether the TIM is in operation or not.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  The new state of the timer operation status (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     ITStatus intstatus = TIM_GetOperationStatus(TIM6);
 * }
 * \endcode
 */
ITStatus TIM_GetOperationStatus(TIM_TypeDef *TIMx);

/**
 * \brief Get TIMx elapsed value when timer is running.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return The elapsed counter value.
 */
uint32_t TIM_GetElapsedValue(TIM_TypeDef *TIMx);

/**
 * \brief   Clear TIM interrupt.
 *
 * \param[in] TIMx: Select the TIM peripheral. \ref TIMER_Declaration
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void timer_demo(void)
 * {
 *     TIM_ClearInterrupt(TIM6);
 * }
 * \endcode
 */
void TIM_ClearInterrupt(TIM_TypeDef *TIMx, uint8_t TIM_INT);

/**
 * \brief   PWM complementary output emergency stop and resume.
 *          PWM_P emergency stop level state is configured by PWM_Stop_State_P,
 *          PWM_N emergency stop level state is configured by PWM_Stop_State_N.
 *
 * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
 * \param[in] NewState: New state of complementary output.
 *            \ref DISABLE: Resume PWM complementary output.
 *            \ref ENABLE: PWM complementary output emergency stop.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void board_pwm_init(void)
 * {
 *     Pad_Config(P0_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
 *     Pad_Config(P0_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
 *                PAD_OUT_HIGH);
 *     Pad_Config(P2_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
 *                PAD_OUT_HIGH);
 *
 *     Pinmux_Config(P0_1, PWM_OUT_PIN_PINMUX);
 *     Pinmux_Config(P0_2, PWM_OUT_P_PIN_PINMUX);
 *     Pinmux_Config(P2_2, PWM_OUT_N_PIN_PINMUX);
 * }
 *
 * void driver_pwm_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     TIM_TimeBaseInitTypeDef TIM_InitStruct;
 *     TIM_StructInit(&TIM_InitStruct);
 *     TIM_InitStruct.TIM_Mode             = TIM_Mode_UserDefine;
 *     TIM_InitStruct.TIM_PWM_En           = ENABLE;
 *     TIM_InitStruct.TIM_PWM_High_Count   = PWM_HIGH_COUNT;
 *     TIM_InitStruct.TIM_PWM_Low_Count    = PWM_LOW_COUNT;
 *     TIM_InitStruct.PWM_Stop_State_P     = PWM_STOP_AT_HIGH;
 *     TIM_InitStruct.PWM_Stop_State_N     = PWM_STOP_AT_LOW;
 *     TIM_InitStruct.PWMDeadZone_En       = ENABLE;  //enable to use pwn p/n output
 *     TIM_InitStruct.PWM_Deazone_Size     = 255;
 *     TIM_TimeBaseInit(PWM2, &TIM_InitStruct);
 *
 *     TIM_Cmd(PWM2, ENABLE);
 * }
 *
 * void pwm_demo(void)
 * {
 *    board_pwm_init();
 *    driver_pwm_init();
 *    //Add delay.
 *    TIM_PWMComplOutputEMCmd(PWM2,ENABLE);
 * }
 * \endcode
 */
void TIM_PWMComplOutputEMCmd(TIM_TypeDef *TIMx, FunctionalState NewState);

/**
  * \brief  Activates one-shot mode of the specified TIM peripheral.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \return None.
  */
#if TIM_SUPPORT_ONESHOT_CMD
void TIM_OneShotActive(TIM_TypeDef *TIMx);

ITStatus TIM_GetOneshotStatus(TIM_TypeDef *TIMx);
#endif

/**
  * \brief  Read TIMx latch counter0 fifo data.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  length: Latch count0 fifo length, max 32.
  * \pBuf   pBuf: FIFO data out buffer.
  * \return None.
  */
void TIM_ReadLatchCountFIFO(TIM_TypeDef *TIMx, uint32_t *pBuf, uint8_t length);

/**
  * \brief  Change TIM pwm_phase_shift count
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  PhaseShiftCnt:This parameter can be 0 to ccr;
  * \return None
  */
#if TIM_SUPPORT_PWM_PHASE_SHIFT
void TIM_PhaseShiftCnt(TIM_TypeDef *TIMx, uint32_t PhaseShiftCnt);
#endif

/**
  * \brief  Set Max Count value.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  count: Max counter value for user-define PWM mode (leagel value: 1 ~ 2^32-2).
  * \return None
  */
void TIM_SetMaxCount(TIM_TypeDef *TIMx, uint32_t count);

/**
  * \brief  Set TIMx capture/compare value for user-define PWM manual mode.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  value: User-defined TIM capture/compare value for PWM manual mode,
  *         ranging from 0 to 2^31.
  * \return None
  */
void TIM_SetCCValue(TIM_TypeDef *TIMx, uint32_t value);

/**
  * \brief  Set TIMx capture/compare value for user-define PWM auto mode.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  value:  User-defined TIM capture/compare value for PWM auto mode,
  *         ranging from 0 to 2^31.
  * \return None
  */
void TIM_WriteCCFIFO(TIM_TypeDef *TIMx, uint32_t value);

/**
  * \brief  Get TIM latch counter0 fifo length.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \return FIFO data length.
  */
uint8_t TIM_GetLatchCountFIFOLength(TIM_TypeDef *TIMx);

/**
  * \brief  Get TIM toggle state
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \return The new state of toggle state(true or false).
  */
bool TIM_GetToggleState(TIM_TypeDef *TIMx);

/**
  * \brief  Check whether the specified TIM flag is set.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  TIM_FLAG: Specifies the flag to check.
  * \return The new state of TIM_FLAG (SET or RESET).
  */
FlagStatus TIM_GetFIFOFlagStatus(TIM_TypeDef *TIMx, uint32_t TIM_FLAG);

/**
  * \brief  Clear capture/compare or latch count0 fifo.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  FIFO_CLR: Specifies the FIFO type which to be clear.
  * \return FIFO data length.
  */
void TIM_ClearFIFO(TIM_TypeDef *TIMx, uint8_t FIFO_CLR);

#if TIM_SUPPORT_PWM_SRC_SELECT
/**
 * \brief  TIM PWMP/N Source Select.
 * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
 * \param  PWMSrcSel: State of the TIMx PWMP/N.
 * \return None.
 */
void TIM_PWMDZRefSel(TIM_TypeDef *TIMx, TIMPWMDZRef_TypeDef PWMSrcSel);
#endif

/**
  * \brief  Enable/disable to pause timer counter.
  * \param  TIMx: Select the TIM peripheral. \ref TIM_Declaration
  * \param  NewState: New state of the TIMx peripheral.
  * \return None.
  */
void TIM_PauseCmd(TIM_TypeDef *TIMx, FunctionalState NewState);

void TIM_ClkConfig(TIM_TypeDef *TIMx, TIMClockSrc_TypeDef ClockSrc,
                   TIMClockDiv_TypeDef ClockDiv);

void TIM_PWMDZClockConfig(PWMDZClockSrc_TypeDef ClockSrc, TIMClockDiv_TypeDef ClockDiv,
                          FunctionalState NewState);

void TIM_LatchTrigDebClkConfig(TIMClockDiv_TypeDef ClockDiv, FunctionalState NewState);

/** End of TIM_Exported_Functions
  * \}
  */

/** End of TIM
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /*_RTL876X_TIM_H_*/


/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/
