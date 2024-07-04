/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_io_dlps.h
* @brief
* @details
* @author    tifnan_ge
* @date      2015-05-18
* @version   v1.0
* *********************************************************************************************************
*/


#ifndef _RTL876X_IO_DLPS_H_
#define _RTL876X_IO_DLPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"
#include "rtl876x_bitfields.h"
#include "board.h"
#include "dlps.h"

extern void set_io_power_in_lps(bool on, LPSMode mode);

/** @addtogroup IO_DLPS IO DLPS
  * @brief IO DLPS dirver module
  * @{
  */

/** @defgroup IO_DLPS_Exported_Types IO DLPS Exported Types
  * @{
  */

typedef void (*DLPS_IO_ExitDlpsCB)(void);
typedef void (*DLPS_IO_EnterDlpsCB)(void);

/** End of group IO_DLPS_Exported_Types
  * @}
  */

/** @defgroup IO_DLPS_Exported_Constants IO DLPS Exported Constants
  * @{
  */

/** End of group IO_DLPS_Exported_Constants
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/


/** @defgroup IO_DLPS_Exported_Functions IO DLPS Exported Functions
  * @{
  */

/**
  * @brief  Register io restore function in dlps mode
  * @param  None
  * @retval None
  */
extern void DLPS_IORegister(void);

extern uint32_t DLPS_ReadWakeupPinData(void);

/**
  * @brief  Set IO power domain ON/OFF in DLPS mode
  * @note   IO power domain would change to _LQ, and certain IO(Sleep LED) would possible need more
            power which is not available under _LQ mode, and this API is used to keep the _HQ power
            even when under DLPS mode to make these IOs works as expected
  * @param  on: true to keep power on, false to let power off
  * @param  mode: DLPS mode, @ref LPSMode
  * @retval None
  */
static inline void DLPS_IOSetPower(bool on, LPSMode mode)
{
    set_io_power_in_lps(on, mode);
}

#if USE_USER_DEFINE_DLPS_EXIT_CB

extern DLPS_IO_ExitDlpsCB User_IO_ExitDlpsCB;

/**
  * @brief  Rrgister user-defined exit dlps callback function
  * @param  func: user-defined callback functon.
  * @retval None
  */
__STATIC_INLINE void DLPS_IORegUserDlpsExitCb(DLPS_IO_ExitDlpsCB func)
{
    User_IO_ExitDlpsCB = func;
}

#endif /* USE_USER_DEFINE_DLPS_EXIT_CB */

#if USE_USER_DEFINE_DLPS_ENTER_CB

extern DLPS_IO_EnterDlpsCB User_IO_EnterDlpsCB;

/**
  * @brief  Rrgister user-defined enter dlps callback function
  * @param  func: user-defined callback functon.
  * @retval None
  */
__STATIC_INLINE void DLPS_IORegUserDlpsEnterCb(DLPS_IO_EnterDlpsCB func)
{
    User_IO_EnterDlpsCB = func;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_IO_DLPS_H_ */

/** @} */ /* End of group IO_DLPS_Exported_Functions */
/** @} */ /* End of group IO_DLPS */


/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/

