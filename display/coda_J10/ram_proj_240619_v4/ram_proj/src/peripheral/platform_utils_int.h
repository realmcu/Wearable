/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      platform_utils_int.h
   * @brief     utils header file for internal project
   * @details
   * @author
   * @date
   * @version
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
  *                   Define to prevent recursive inclusion
  *============================================================================*/
#ifndef _PLATFORM_UTILS_INT_H_
#define _PLATFORM_UTILS_INT_H_


/*============================================================================*
  *                               Header Files
  *============================================================================*/
//#include "platform_utils.h"
#include "FreeRTOS.h"

/*============================================================================*
  *                                   Types
  *============================================================================*/


/*============================================================================*
  *                                Functions
  *============================================================================*/


/************************************ assert ************************************/

#define PLATFORM_ASSERT(x) configASSERT(x)
#define EMPTY_FUNCTION  empty_function

extern void empty_function(void);

#ifdef _NEW_SW_DELAY_TO_SUPPORT_VARIED_CPU_CLK_RATE_

extern volatile void (*platform_delay_ms)(uint32_t t);
extern volatile void (*platform_delay_us)(uint32_t t);
void sw_delay_func_reinit(uint32_t SysCpuClk);

#else

void platform_delay_ms(uint32_t t);
void platform_delay_us(uint32_t t);

#endif

#endif
