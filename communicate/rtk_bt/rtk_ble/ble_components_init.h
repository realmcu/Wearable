/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file ble_components_init.h
  * @brief app components
  * @details application startup and close
  * @author howie_wang@realsil.com.cn
  * @date
  * @version 1.0
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __BLE_COMPONENTS_H__
#define __BLE_COMPONENTS_H__
#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*
 *                        Header Files
 *============================================================================*/


/** @defgroup APP APP
  * @brief
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup APP_Exported_Types APP Exported Types
  * @brief
  * @{
  */

typedef int (*ble_init_fn_t)(void);

/** End of APP_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/
/** @defgroup APP_Exported_Constants APP Exported Constants
  * @brief
  * @{
  */


/** End of APP_Exported_Constants
  * @}
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup APP_Exported_Macros APP Exported Macros
  * @brief
  * @{
  */

#define BLE_APP_USED                     __attribute__((used))
#define BLE_APP_SECTION(x)                  __attribute__((section(x)))


#define BLE_APP_INIT_EXPORT(fn, level)                                                       \
    BLE_APP_USED const ble_init_fn_t __ble_init_##fn BLE_APP_SECTION(".ble_fn." level) = fn

/* board init routines will be called in board_init() function */
#define BLE_INIT_BOARD_EXPORT(fn)           BLE_APP_INIT_EXPORT(fn, "1")

/* pre/device/component/env/app init routines will be called in init_thread */
/* components pre-initialization (pure software initilization) */
#define BLE_INIT_PREV_EXPORT(fn)            BLE_APP_INIT_EXPORT(fn, "2")
/* device initialization */
#define BLE_INIT_DEVICE_EXPORT(fn)          BLE_APP_INIT_EXPORT(fn, "3")
/* components initialization (dfs, lwip, ...) */
#define BLE_INIT_COMPONENT_EXPORT(fn)       BLE_APP_INIT_EXPORT(fn, "4")
/* environment initialization (mount disk, ...) */
#define BLE_INIT_ENV_EXPORT(fn)             BLE_APP_INIT_EXPORT(fn, "5")
/* appliation initialization (rtble application etc ...) */
#define BLE_INIT_APP_EXPORT(fn)             BLE_APP_INIT_EXPORT(fn, "6")


/** End of APP_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Variables
 *============================================================================*/
/** @defgroup APP_Exported_Variables APP Exported Variables
  * @brief
  * @{
  */


/** End of APP_Exported_Variables
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup APP_Exported_Functions APP Exported Functions
  * @brief
  * @{
  */

/**
 * @brief
 *
 */
void ble_components_init(void);

/** End of APP_Exported_Functions
  * @}
  */

/** End of APP
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif















