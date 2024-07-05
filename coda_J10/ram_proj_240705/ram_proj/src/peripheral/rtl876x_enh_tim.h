/**
*********************************************************************************************************
*               Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_enh_tim.h
* \brief    The header file of the peripheral ENHANCE TIMER driver.
* \details  This file provides all TIMER firmware functions.
* \author   arthur
* \date     2022-10-20
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_ENH_TIM_H_
#define _RTL876X_ENH_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    ENH_TIM         ENH_TIM
 * \brief       Manage the ENH_TIM peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "utils.h"

/*============================================================================*
 *                         Types
 *============================================================================*/
typedef enum _ENH_TIM_ID_TypeDef
{
    ENH_TIM_0_ID,
    ENH_TIM_1_ID,
    ENH_TIM_2_ID,
    ENH_TIM_3_ID,
//  ENH_TIM_4_ID,
//  ENH_TIM_5_ID,
//  ENH_TIM_6_ID,
//  ENH_TIM_7_ID,
//  ENH_TIM_8_ID,
//  ENH_TIM_9_ID,
//  ENH_TIM_10_ID,
//  ENH_TIM_11_ID,
//  ENH_TIM_12_ID,
//  ENH_TIM_13_ID,
//  ENH_TIM_14_ID,
//  ENH_TIM_15_ID,
//  ENH_TIM_16_ID,
//  ENH_TIM_17_ID,
//  ENH_TIM_18_ID,
//  ENH_TIM_19_ID,
//  ENH_TIM_20_ID,
//  ENH_TIM_21_ID,
//  ENH_TIM_22_ID,
//  ENH_TIM_23_ID,
//  ENH_TIM_24_ID,
//  ENH_TIM_25_ID,
//  ENH_TIM_26_ID,
//  ENH_TIM_27_ID,
//  ENH_TIM_28_ID,
//  ENH_TIM_29_ID,
//  ENH_TIM_30_ID,
} ENH_TIM_ID_TypeDef;

typedef enum _ENH_TIM_CLK_DIV_SEL
{
    CLK_DIV_1,
    CLK_DIV_2,
    CLK_DIV_4,
    CLK_DIV_8,
    CLK_DIV_16,
    CLK_DIV_32,
    CLK_DIV_64,
} ENH_TIM_CLK_DIV_SEL_TypeDef;

/**
 * \defgroup    ENH_TIM_Exported_Types Init Params Struct
 *
 * \ingroup     ENH_TIM
 */

/**
 * \brief       ENH_TIM init structure definition.
 *
 * \ingroup     ENH_TIM_Exported_Types
 */
typedef struct
{
//    uint32_t TIM_ClockSrc;          /*!< Specifies the clock source.
//                                        This parameter can be a value of \ref ENH_TIM_Clock_Source*/
    uint32_t TIM_ClockSrcDiv;        /*!< Specifies the clock source div.
                                        This parameter can be a value of \ref ENH_TIM_Clock_Divider*/
    uint32_t TIM_Mode;              /*!< Specifies the counter mode.
                                        This parameter can be a value of \ref ENH_TIM_Mode. */
    uint32_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
                                        Auto-Reload Register at the next update event.
                                        This parameter must range from 0x1 to 0xFFFFFFFE. */
    uint32_t TIM_OneShotEn;        /*!< Specifies the one shot mode.
                                            This parameter can be a value of \ref ENH_TIM_One_Shot*/
} ENH_TIM_TimeBaseInitTypeDef;


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    ENH_TIM_Exported_constants  Macro Definitions
 *
 * \ingroup     ENH_TIM
 */

#define IS_TIM_ALL_PERIPH(PERIPH)   (((PERIPH) == ENH_TIM0) || \
                                     ((PERIPH) == ENH_TIM1) || \
                                     ((PERIPH) == ENH_TIM2) || \
                                     ((PERIPH) == ENH_TIM3) )


/**
 * \defgroup    ENH_TIM_Mode ENH_TIM Mode
 * \{
 * \ingroup     ENH_TIM_Exported_constants
 */
#define ENH_TIM_Mode_FreeRun                    ((uint32_t)0x0)
#define ENH_TIM_Mode_UserDefine                     ((uint32_t)0x2)

/** \} */
#define IS_TIM_MODE(MODE) (((MODE) == ENH_TIM_Mode_FreeRun) || \
                           ((MODE) == ENH_TIM_Mode_UserDefine))

/**
 * \defgroup    ENH_TIM_One_Shot ENH_TIM One Shot
 * \{
 * \ingroup     ENH_TIM_Exported_constants
 */
#define ENH_TIM_ONE_SHOT_DISABLE                ((uint32_t)0x0)
#define ENH_TIM_ONE_SHOT_ENABLE                 ((uint32_t)0x1)

/** \} */
#define IS_TIM_OneShotEn(MODE)     (((MODE) == ENH_TIM_ONE_SHOT_DISABLE) || \
                                    ((MODE) == ENH_TIM_ONE_SHOT_ENABLE))

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    ENH_TIM_Exported_Functions  Peripheral APIs
 * \{
 * \ingroup     ENH_TIM
 */

/**
 * \brief   Initialize the TIMx time base unit peripheral according to
 *          the specified parameters in ENH_TIM_TimeBaseInitStruct.
 * \param[in] TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \param[in] ENH_TIM_TimeBaseInitStruct: Pointer to a ENH_TIM_TimeBaseInitTypeDef
 *            structure that contains the configuration information for the selected ENH_TIM peripheral.
 * \return  None.
 */
void ENH_TIM_TimeBaseInit(ENH_TIM_TypeDef *TIMx,
                          ENH_TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Fills each ENH_TIM_TimeBaseInitStruct member with its default value.
 * \param[in] ENH_TIM_TimeBaseInitStruct: Pointer to a ENH_TIM_TimeBaseInitTypeDef structure which will be initialized.
 * \return  None.
 */
void ENH_TIM_StructInit(ENH_TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);

/**
 * \brief   Enables or disables the specified ENH_TIM peripheral.
 * \param[in] TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \param[in] NewState: New state of the TIMx peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 */
void ENH_TIM_Cmd(ENH_TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief   Activates one-shot mode of the specified ENH_TIM peripheral.
 * \param[in] TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \return  None.
 */
void ENH_TIM_OneShotActivate(ENH_TIM_TypeDef *TIMx);

/**
 * \brief       Enables or disables the specified TIMx interrupt.
 * \param[in]   TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \param[in]   NewState: New state of the TIMx peripheral.
 *              This parameter can be: ENABLE or DISABLE.
 * \return      None.
 */
void ENH_TIM_INTConfig(ENH_TIM_TypeDef *TIMx, FunctionalState NewState);

/**
 * \brief       Change ENH_TIM period value.
 * \param[in]   TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \param[in]   period: Period value to be changed.
 * \return  None.
 */
void ENH_TIM_ChangePeriod(ENH_TIM_TypeDef *TIMx, uint32_t period);

/**
 * \brief   Get TIMx current value when timer is running.
 * \param[in]   TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \return  The counter value.
 */
uint32_t ENH_TIM_GetCurrentValue(ENH_TIM_TypeDef *TIMx);

/**
 * \brief   Check whether the ENH_TIM interrupt has occurred or not.
 * \param[in]   TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \return  The new state of the ENH_TIM_IT(SET or RESET).
 */
ITStatus ENH_TIM_GetINTStatus(ENH_TIM_TypeDef *TIMx);

/**
 * \brief   Clear ENH_TIM interrupt.
 * \param[in]   TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \return  None.
 */
void ENH_TIM_ClearINT(ENH_TIM_TypeDef *TIMx);

/**
 * \brief   Check whether the ENH_TIM is in operation or not.
 * \param[in]   TIMx: Where TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the TIMx peripheral.
 * \return  The new state of the timer operation status (SET or RESET).
 */
ITStatus ENH_TIM_GetEnableStatus(ENH_TIM_TypeDef *TIMx);

/**
 * \brief       Get  specified ENH_TIMx interrupt is active or not .
 * \param[in]   ENH_TIMx: Where ENH_TIMx can be ENH_TIM0 to ENH_TIMx( x max timer) to select the ENH_TIMx peripheral
 * \return      The enhtimer_intr is wether active or not after masking.
                0 enhtimer_intr is not active after masking
                1 enhtimer_intr is active after masking
 */
bool ENH_TIM_INTActive(ENH_TIM_TypeDef *ENH_TIMx);
/** \} */ /* End of group ENH_TIM_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /*_rtl876x_enh_tim_H_*/


/******************* (C) COPYRIGHT 2022 Realtek Semiconductor Corporation *****END OF FILE****/
