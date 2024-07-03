/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_lppwm.h
* @brief
* @details
* @author
* @date
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_LPPWM_H_
#define _RTL876X_LPPWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"

/** @addtogroup 87x3e_LPPWM LPPWM
  * @brief LPPWM driver module
  * @{
  */

/** @defgroup LPPWM_Exported_Types LPPWM Exported Types
  * @{
  */

/**
 * @brief LPPWM initialize parameters
 *
 * LPPWM initialize parameters
 */
typedef struct
{
    uint8_t polarity;           /*!< Specifies the LPPWM polarity.
                                         This parameter can be a value of @ref LPPWM_Output_polarity */
    uint16_t period_high;     /*!< Specifies the LPPWM period high.
                                         This parameter can be a value of 0-65535 */
    uint16_t period_low;      /*!< Specifies the LPPWM period low.
                                         This parameter can be a value of 0-65535 */
} LPPWM_InitTypeDef;

/** End of group LPPWM_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/


/** @defgroup LPPWM_Exported_Constants LPPWM Exported Constants
  * @{
  */

/** @defgroup LPPWM_Output_polarity LPPWM output polarity
  * @{
  */

typedef enum
{
    LPPWM_OUTPUT_NORMAL = 0x0,
    LPPWM_OUTPUT_INVERT = 0x1,
} LPPWMOutputPolarity_TypeDef;

#define IS_LPPWM_OUTPUT_MODE(MODE)    (((MODE) == LPPWM_OUTPUT_NORMAL) || \
                                       ((MODE) == LPPWM_OUTPUT_INVERT))
/** End of group LPPWM_Output_polarity
  * @}
  */

/** End of group LPPWM_Exported_Constants
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/


/** @defgroup LPPWM_Exported_Functions LPPWM Exported Functions
  * @{
  */

/**
 * rtl876x_lppwm.h
 *
 * \brief   Clear all the LPPWM registers to their default reset values.
 *
 * \param   None.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lppwm_init(void)
 * {
 *     LPPWM_Reset();
 * }
 * \endcode
 */
void LPPWM_Reset(void);


/**
 * rtl876x_lppwm.h
 *
 * \brief  Initialize the LPPWM according to the specified
 *         parameters in the LPPWM_InitStruct.
 *
 * \param[in] LPPWM_InitStruct: Pointer to a LPPWM_InitTypeDef structure that
 *            contains the configuration information for the LPPWM.
 *
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lppwm_init(void)
 * {
 *     LPPWM_Reset();
 *     LPPWM_InitTypeDef LPPWM_InitStruct;
 *     LPPWM_StructInit(&LPPWM_InitStruct);
 *     LPPWM_InitStruct.polarity                    = LPPWM_OUTPUT_NORMAL;
 *     LPPWM_InitStruct.period_high                 = 32;
 *     LPPWM_InitStruct.period_low                  = 32;
 *     LPPWM_Init(&LPPWM_InitStruct);
 *     LPPWM_Cmd(ENABLE);
 * }
 * \endcode
 */
void LPPWM_Init(LPPWM_InitTypeDef *LPPWM_InitStruct);


/**
 * rtl876x_lppwm.h
 *
 * \brief   Fill each LPPWM_InitStruct member with its default value.
 *
 * \param[in] LPPWM_InitStruct: Pointer to a LPPWM_InitTypeDef structure which will be initialized.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lppwm_init(void)
 * {
 *     LPPWM_Reset();
 *     LPPWM_InitTypeDef LPPWM_InitStruct;
 *     LPPWM_StructInit(&LPPWM_InitStruct);
 *     LPPWM_InitStruct.polarity                    = LPPWM_OUTPUT_NORMAL;
 *     LPPWM_InitStruct.period_high                 = 32;
 *     LPPWM_InitStruct.period_low                  = 32;
 *     LPPWM_Init(&LPPWM_InitStruct);
 *     LPPWM_Cmd(ENABLE);
 * }
 * \endcode
 */
void LPPWM_StructInit(LPPWM_InitTypeDef *LPPWM_InitStruct);


/**
 * rtl876x_lppwm.h
 *
 * \brief   Enable or disable the LPPWM.
 *
 * \param[in] NewState: New state of the LPPWM.
 *            This parameter can be one of the following values:
 *            \arg ENABLE: Enable the LPPWM.
 *            \arg DISABLE: Disable the LPPWM.
 *
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_lppwm_init(void)
 * {
 *     LPPWM_Cmd(ENABLE);
 * }
 * \endcode
 */
void LPPWM_Cmd(FunctionalState NewState);


/** @} */ /* End of group LPPWM_Exported_Functions */
/** @} */ /* End of group 87x3e_LPPWM */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_LPPWM_H_ */

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/

