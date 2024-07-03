/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      hw_tim_internal.h
* @brief
* @details
* @author
* @date
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _HW_TIM_INTERNAL_
#define _HW_TIM_INTERNAL_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "pwm.h"
#include "vector_table.h"
#include "rtl876x.h"

#if defined(IC_TYPE_BBPRO3) || (TARGET_RTL8773DO == 1) || (TARGET_RTL8773DFL == 1)
#else
#define count_1bits(x) (uint8_t)__builtin_popcount(x)
#endif

/** @addtogroup HW_TIM_INTERNAL HW TIM INTERNAL
  * @brief HW TIM driver module
  * @{
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/


/** @defgroup HW_TIM_INTERNAL_Exported_constants HW TIM Internal Exported Constants
  * @{
  */
#if defined(IC_TYPE_BBPRO3) || (TARGET_RTL8773DO == 1) || (TARGET_RTL8773DFL == 1)
#define HW_TIMER_DYNAMIC_MASK                       (0xF700 | BIT5 | BIT2 | BIT3 | BIT4 | BIT6)               /* only TIM8/TIM9 is not used in ROM/APP/FWK for now */
#define HW_TIMER_MASK                               (0xFFFC)               /* TIM2~TIM11, TIM0/TIM1 reserved for lowstack, not handled in driver */

#define TOTAL_HW_TIMER_NUM                          (16)

#define hw_timer_get_reg_base_from_id(id)           (TIM_TypeDef *)((id < 8) ? (TIM0_REG_BASE + 0x14 * id) : (TIM8_REG_BASE + 0x14 * (id - 8)))
#define hw_timer_get_pwm_func(id)                   ((id < 8) ? (timer_pwm0 + id) : ((id < 12) ? (PWM8 + (id - 8)) : (PWM12 + (id - 12))))
#else /* RTL87X3E or RTL87X3G */
#define HW_TIMER_DYNAMIC_MASK                       (BIT6 | BIT5 | BIT4 | BIT3 | BIT2)   /* Not used in ROM */
#define HW_TIMER_MASK                               (0xFC)                /* TIM2~TIM11, TIM0/TIM1 reserved for lowstack, not handled in driver */

#define TOTAL_HW_TIMER_NUM                          (8)

#define hw_timer_get_reg_base_from_id(id)           (TIM_TypeDef *)(TIM0_REG_BASE + 0x14 * id)
#define hw_timer_get_pwm_func(id)                   (TIMER_PWM0 + id)
#endif

#define PF_RTC_GET_CURRENT_COUNT()                  HAL_READ32(RTC_REG_BASE, 0x58)
#define US_TO_TICK_THRESHOLD                        (0x3FFFFFFF)
#define US_TO_PF_RTC_TICK(us)                       (((us) > US_TO_TICK_THRESHOLD)?(((us)/125) << 2):(((us) << 2)/125))
#define PF_RTC_CNT_TO_US_OVERFLOW_THRESHOLD         (0x83126E9)
#define PF_RTC_TICK_TO_US(count)                    (((count) > PF_RTC_CNT_TO_US_OVERFLOW_THRESHOLD)?0xFFFFFFFF:(((count) * 31) + ((count) >> 2)))
#define RTC_INTERVAL_CAL(before_cnt, after_cnt)     (((before_cnt) > (after_cnt))?(0xFFFFFFFFUL - (before_cnt) + (after_cnt)):((after_cnt) - (before_cnt)))


typedef enum
{
    HW_TIMER_ID_EMPTY,
    HW_TIMER_ID_1,
    HW_TIMER_ID_2,
    HW_TIMER_ID_3,
    HW_TIMER_ID_4,
    HW_TIMER_ID_5,
    HW_TIMER_ID_6,
    HW_TIMER_ID_7,
#if defined(IC_TYPE_BBPRO3) || (TARGET_RTL8773DO == 1) || (TARGET_RTL8773DFL == 1)
    HW_TIMER_ID_8,
    HW_TIMER_ID_9,
    HW_TIMER_ID_10,
    HW_TIMER_ID_11,
    HW_TIMER_ID_12,
    HW_TIMER_ID_13,
    HW_TIMER_ID_14,
    HW_TIMER_ID_15,
#endif
    HW_TIMER_ID_MAX,
} T_HW_TIMER_ID;

/** End of group HW_TIM_INTERNAL_Exported_constants
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/


/** @defgroup HW_TIM_INTERNAL_Exported_Types HW TIM Internal Exported Types
  * @{
  */

typedef struct
{
    uint32_t REG_LoadCount;
    uint32_t REG_ControlReg;
    uint32_t REG_LoadCount2;
    uint32_t REG_CurrentValue;
} T_TIM_RESTORE_REG;

#if defined(IC_TYPE_BBPRO3) || (TARGET_RTL8773DO == 1) || (TARGET_RTL8773DFL == 1)
extern uint32_t pf_rtc_dlps_store_cnt;
#endif

/** End of group HW_TIM_INTERNAL_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/


/** @defgroup HW_TIM_INTERNAL_Exported_Functions HW TIM Internal Exported Functions
  * @{
  */

/**
  * @brief  The execution content of ISR handler of the specified timer.
  * @param  id: hw timer id.
  * @retval none
  */
extern void hw_timer_handler_imp(T_HW_TIMER_ID id);

/**
  * @brief  Update isr handler of the specified timer.
  * @param  id: hw timer id.
  * @retval true    success
  *         false   fail
  */
extern bool hw_timer_update_isr(VECTORn_Type v_num, IRQ_Fun isr_handler, bool is_xip);

#if defined(IC_TYPE_BB2) || (TARGET_RTL8753EFE == 1) || (TARGET_RTL8753EFE_VS == 1) || (TARGET_RTL8773EFE == 1) || \
(TARGET_RTL8763EFL == 1) || (TARGET_RTL8763EWM == 1) || (TARGET_RTL8773EFL == 1) || (TARGET_RTL8763ESE == 1) || \
(TARGET_RTL8773EFE_AVP == 1) || (TARGET_RTL8753EFE_VS_AVP == 1) || (IC_TYPE_BB2_CCUT == 1) || \
(TARGET_RTL8753EFE_AVP == 1) || (TARGET_RTL8763ESE_AVP == 1)

/**
  * @brief  Get isr handler of the specified timer.
  * @param  none
  * @retval none
  */
extern void hal_timer_handler(void);

/**
  * @brief  Get isr handler of the specified timer.
  * @param  id: hw timer id.
  * @retval none
  */
#define hw_timer_get_isr(id)  hal_timer_handler
#else
/**
  * @brief  Get isr handler of the specified timer.
  * @param  id: hw timer id.
  * @retval none
  */
extern IRQ_Fun hw_timer_get_isr(T_HW_TIMER_ID id);
#endif

/**
  * @brief  Enable clock of the specified timer.
  * @param  id: hw timer id.
  * @retval none
  */
extern void hw_timer_rcc_clock_cmd(T_HW_TIMER_ID id);

/**
  * @brief  Get the vector number of the specified timer.
  * @param  id: hw timer id.
  * @retval Vector number
  */
extern VECTORn_Type hw_timer_get_vectorn_from_id(T_HW_TIMER_ID id);

/**
  * @brief  Get the interrupt number of the specified timer.
  * @param  id: hw timer id.
  * @retval Interrupt number
  */
extern IRQn_Type hw_timer_get_irqn_from_id(T_HW_TIMER_ID id);

/**
  * @brief  Select PWM clock source divider.
  * @param  source: PWM clock source.
  * @retval PWM clock source divider.
  */
extern uint8_t pwm_src_div_mapping(T_PWM_CLOCK_SOURCE source);

/**
  * @brief  Restore register value of the specified timer.
  * @param  id: hw timer id.
  * @param  buf: restore register value
  * @param  sleep_time_us: sleep time
  * @retval none
  */
extern void hw_timer_pm_exit(T_HW_TIMER_ID id, T_TIM_RESTORE_REG *buf, uint32_t sleep_time_us);

#ifdef __cplusplus
}
#endif

#endif /* _HW_TIM_INTERNAL_ */

/** @} */ /* End of group HW_TIM_INTERNAL_Exported_Functions */
/** @} */ /* End of group HW_TIM_INTERNAL */

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
