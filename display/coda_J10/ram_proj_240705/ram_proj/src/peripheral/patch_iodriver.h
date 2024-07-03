/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _PATCH_IODRIVER_H_
#define _PATCH_IODRIVER_H_

#include <patch.h>

#ifdef __cplusplus
extern "C" {
#endif

/* uart patch function pointer */
extern BOOL_PATCH_FUNC patch_HCI_UART_Pin_Config;

/* cp patch function pointer */
extern BOOL_PATCH_FUNC patch_cp_burst_read;
extern BOOL_PATCH_FUNC patch_cp_signal_read;
extern BOOL_PATCH_FUNC patch_cp_write;
extern BOOL_PATCH_FUNC patch_cp_reset;
extern BOOL_PATCH_FUNC patch_cp_hw_init;

/**@brief patch function pointer for gdma */
extern BOOL_PATCH_FUNC patch_GDMA_Init;
extern BOOL_PATCH_FUNC patch_GDMA_Cmd;
extern BOOL_PATCH_FUNC patch_GDMA_INTConfig;
extern BOOL_PATCH_FUNC patch_GDMA_ClearINTPendingBit;

/**@brief patch function pointer for high speed uart */
extern BOOL_PATCH_FUNC patch_UART_Init;
extern BOOL_PATCH_FUNC patch_UART_ReceiveData;
extern BOOL_PATCH_FUNC patch_UART_SendData;
extern BOOL_PATCH_FUNC patch_UART_INTConfig;

/**@brief patch function pointer for gpio */
extern BOOL_PATCH_FUNC patch_GPIO_Init;

/**@brief patch function pointer for rcc */
extern BOOL_PATCH_FUNC patch_RCC_PeriphClockCmd;
/**@brief patch function pointer for i2c */
extern BOOL_PATCH_FUNC patch_I2C_Init;


/**@brief patch function pointer for tim and pwm */
extern BOOL_PATCH_FUNC patch_TIM_TimeBaseInit;
extern BOOL_PATCH_FUNC patch_TIM_StructInit;
extern BOOL_PATCH_FUNC patch_TIM_Cmd;
extern BOOL_PATCH_FUNC patch_TIM_PWMChangeFreqAndDuty;

/**@brief patch function pointer for hw_aes */
/**@brief patch function pointer for nvic */

/**@brief patch function pointer for pinmux */
extern BOOL_PATCH_FUNC patch_Pinmux_Config;
extern BOOL_PATCH_FUNC patch_Pad_Config;
extern BOOL_PATCH_FUNC patch_Pad_ConfigExt;
extern BOOL_PATCH_FUNC patch_Pad_AllConfigDefault;
extern BOOL_PATCH_FUNC patch_Log_SWD_DLPS_Enter;
extern BOOL_PATCH_FUNC patch_Log_SWD_DLPS_Exit;
extern BOOL_PATCH_FUNC patch_System_WakeUpPinEnable;
extern BOOL_PATCH_FUNC patch_System_WakeUpPinDisable;
extern BOOL_PATCH_FUNC patch_System_WakeUpInterruptEnable;
extern BOOL_PATCH_FUNC patch_System_WakeUpInterruptDisable;
extern BOOL_PATCH_FUNC patch_System_WakeUpInterruptValue;

/**@brief patch function pointer for rtc */
extern BOOL_PATCH_FUNC patch_RTC_WriteReg;

/**@brief patch function pointer for sleep led */
extern BOOL_PATCH_FUNC patch_SleepLed_Init;
extern BOOL_PATCH_FUNC patch_SleepLed_Cmd;
extern BOOL_PATCH_FUNC patch_SleepLed_RegisterWrite;

/**@brief patch function pointer for wdg */
extern BOOL_PATCH_FUNC patch_WDG_Config;
extern BOOL_PATCH_FUNC patch_WDG_SystemReset;

/**@brief patch function pointer for adc */
extern BOOL_PATCH_FUNC patch_ADC_Init;
extern BOOL_PATCH_FUNC patch_ADC_StructInit;
extern BOOL_PATCH_FUNC patch_ADC_INTConfig;
extern BOOL_PATCH_FUNC patch_ADC_Read;
extern BOOL_PATCH_FUNC patch_ADC_Cmd;
extern BOOL_PATCH_FUNC patch_ADC_GetCalRes;
extern BOOL_PATCH_FUNC patch_ADC_GetBypassCalRes;

/**@brief patch function pointer for sdio */
extern BOOL_PATCH_FUNC patch_SDIO_Init PATCH_POINTER_SECTION;
extern BOOL_PATCH_FUNC patch_SDIO_SetBusPower PATCH_POINTER_SECTION;
extern BOOL_PATCH_FUNC patch_SDIO_DataConfig PATCH_POINTER_SECTION;
extern BOOL_PATCH_FUNC patch_SDIO_SendCommand PATCH_POINTER_SECTION;

#ifdef __cplusplus
}
#endif

#endif /* _PATCH_IODRIVER_H_ */
