/* Keil style config file for HWTest */

#ifndef _HWTEST_CONFIG_H_
#define _HWTEST_CONFIG_H_


#ifdef __cplusplus
extern "C" {
#endif


/* RTL8763B platform function */
#include "bblite_platform.h"

/* select timer function required low-level support */
#define DELAY_USE_SYSTICK  0
#define DELAY_USE_HWTIMER  1
#define DELAY_USE_OS_DELAY 2

#define DELAY_USE_SELECT   DELAY_USE_HWTIMER

/* select log function required low-level support */
#define LOG_TO_UART  0
#define LOG_TO_SWD   1
#define LOG_TO_RAM   2

#define LOG_TO_SELECT LOG_TO_UART

// /* UART Control Settings */
//#define  LOG_UART_INDEX             LOG_TO_UART  /* 1: Data UART, 2: LOG_UART, 3: LOG_UART1  */
///* Mode Set *///#define Log_UART_Baudrate         (BAUD_RATE_115200)
////#define UART_FLOW_CONTROL


#ifdef __cplusplus
}
#endif

#endif /* _HWTEST_CONFIG_H_ */
