/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#include <patch.h>

/**@brief patch function pointer for uart */
BOOL_PATCH_FUNC patch_HCI_UART_Pin_Config PATCH_POINTER_SECTION;

/**@brief patch function pointer for gdma */
BOOL_PATCH_FUNC patch_GDMA_Init PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_GDMA_Cmd PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_GDMA_INTConfig PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_GDMA_ClearINTPendingBit PATCH_POINTER_SECTION;

/**@brief patch function pointer for high speed uart */
BOOL_PATCH_FUNC patch_UART_Init PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_UART_ReceiveData PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_UART_SendData PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_UART_INTConfig PATCH_POINTER_SECTION;

/**@brief patch function pointer for gpio */
BOOL_PATCH_FUNC patch_GPIO_Init PATCH_POINTER_SECTION;

/**@brief patch function pointer for rcc */
BOOL_PATCH_FUNC patch_RCC_PeriphClockCmd PATCH_POINTER_SECTION;

/**@brief patch function pointer for i2c */
BOOL_PATCH_FUNC patch_I2C_Init PATCH_POINTER_SECTION;

/**@brief patch function pointer for hw_aes */
/**@brief patch function pointer for nvic */

/**@brief patch function pointer for pinmux */
BOOL_PATCH_FUNC patch_Pinmux_Config PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_Pad_Config PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_Pad_ConfigExt PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_Pad_AllConfigDefault PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_System_WakeUpPinEnable PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_System_WakeUpPinDisable PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_System_WakeUpInterruptEnable PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_System_WakeUpInterruptDisable PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_System_WakeUpInterruptValue PATCH_POINTER_SECTION;

/**@brief patch function pointer for rtc */
BOOL_PATCH_FUNC patch_RTC_WriteReg PATCH_POINTER_SECTION;

/**@brief patch function pointer for sleep led */
BOOL_PATCH_FUNC patch_SleepLed_Init PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_SleepLed_Cmd PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_SleepLed_RegisterWrite PATCH_POINTER_SECTION;

/**@brief patch function pointer for wdg */
BOOL_PATCH_FUNC patch_WDG_Config PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_WDG_SystemReset PATCH_POINTER_SECTION;

/**@brief patch function pointer for cp */
BOOL_PATCH_FUNC patch_cp_burst_read PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_cp_signal_read PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_cp_write PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_cp_reset PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_cp_hw_init PATCH_POINTER_SECTION;

/**@brief patch function pointer for adc */
BOOL_PATCH_FUNC patch_ADC_Init PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_ADC_StructInit PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_ADC_INTConfig PATCH_POINTER_SECTION;

BOOL_PATCH_FUNC patch_ADC_Read PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_ADC_Cmd PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_ADC_GetCalRes PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_ADC_GetBypassCalRes PATCH_POINTER_SECTION;

/**@brief patch function pointer for sdio */
BOOL_PATCH_FUNC patch_SDIO_Init PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_SDIO_SetBusPower PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_SDIO_DataConfig PATCH_POINTER_SECTION;
BOOL_PATCH_FUNC patch_SDIO_SendCommand PATCH_POINTER_SECTION;



