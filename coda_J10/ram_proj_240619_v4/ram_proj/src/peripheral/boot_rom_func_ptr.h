#ifndef BOOT_ROM_FUNC_PTR_H
#define BOOT_ROM_FUNC_PTR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
//#include "utils.h"
//#include "rtl876x_wdg.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_uart.h"
#include "rtl876x_nvic.h"
#include "rtl876x_gdma.h"
#include "rtl876x_gpio.h"
#include "rtl876x_tim.h"

void NVIC_Init_rom(NVIC_InitTypeDef *NVIC_InitStruct);
void RCC_PeriphClockCmd_rom(uint32_t APBPeriph, uint32_t APBPeriph_Clock, FunctionalState NewState);
void GDMA_Init_rom(GDMA_ChannelTypeDef *GDMA_Channelx, GDMA_InitTypeDef *GDMA_InitStruct);
uint8_t GDMA_Cmd_rom(uint8_t GDMA_ChannelNum, FunctionalState NewState);
void Pinmux_Config_rom(uint8_t Pin_Num, uint8_t Pin_Func);
void Pad_Config_rom(uint8_t Pin_Num, PAD_Mode AON_PAD_Mode, PAD_PWR_Mode AON_PAD_PwrOn,
                    PAD_Pull_Mode AON_PAD_Pull, PAD_OUTPUT_ENABLE_Mode AON_PAD_E, PAD_OUTPUT_VAL AON_PAD_O);
void Pad_ConfigExt_rom(uint8_t Pin_Num, PAD_Mode AON_PAD_Mode, PAD_PWR_Mode AON_PAD_PwrOn,
                       PAD_Pull_Mode AON_PAD_Pull, PAD_OUTPUT_ENABLE_Mode AON_PAD_E, PAD_OUTPUT_VAL AON_PAD_O,
                       PAD_PULL_VAL AON_PAD_P);
void Pad_AllConfigDefault_rom(void);
void System_WakeUpPinEnable_rom(uint8_t Pin_Num, uint8_t Polarity, uint8_t DebounceEn);
void System_WakeUpPinDisable_rom(uint8_t Pin_Num);
void System_WakeUpInterruptEnable_rom(uint8_t Pin_Num);
void System_WakeUpInterruptDisable_rom(uint8_t Pin_Num);
uint8_t  Pad_WakeUpCmd_rom(WAKEUP_EN_MODE mode, WAKEUP_POL pol, FunctionalState NewState);
void UART_Init_rom(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);
void UART_INTConfig_rom(UART_TypeDef *UARTx, uint32_t UART_IT, FunctionalState newState);
uint8_t UART_SetBaudRate_rom(UART_TypeDef *UARTx, UartBaudRate_TypeDef baud_rate);

/**@brief patch function pointer for timer */
extern bool (*patch_TIM_ClkConfig)(TIM_TypeDef *TIMx, TIMClockSrc_TypeDef ClockSrc,
                                   TIMClockDiv_TypeDef ClockDiv);
extern bool (*patch_TIM_TimeBaseInit)(TIM_TypeDef *TIMx,
                                      TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);
extern bool (*patch_TIM_StructInit)(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);
extern bool (*patch_TIM_Cmd)(TIM_TypeDef *TIMx, FunctionalState NewState);
extern bool (*patch_TIM_ChangePeriod)(TIM_TypeDef *TIMx, uint32_t period);
extern bool (*patch_TIM_InterruptConfig)(TIM_TypeDef *TIMx, uint8_t TIM_INT,
                                         FunctionalState NewState);
extern bool (*patch_TIM_GetCurrentValue)(TIM_TypeDef *TIMx, uint32_t *p_ret);
extern bool (*patch_TIM_GetInterruptStatus)(TIM_TypeDef *TIMx, uint8_t TIM_INT, ITStatus *p_ret);
extern bool (*patch_TIM_ClearInterrupt)(TIM_TypeDef *TIMx, uint8_t TIM_INT);
extern bool (*patch_TIM_GetTimerID)(TIM_TypeDef *TIMx, uint32_t *p_ret);
extern bool (*patch_TIM_GetCase)(TIM_TypeDef *TIMx, TIMCASE_TypeDef *p_ret);
extern bool (*patch_TIM_GetTimerShareBase)(TIM_TypeDef *TIMx, TIM_ShareTypeDef **p_ret);
extern bool (*patch_TIM_PWMDZClockConfig)(PWMDZClockSrc_TypeDef ClockSrc,
                                          TIMClockDiv_TypeDef ClockDiv, FunctionalState NewState);
extern bool (*patch_TIM_LatchTrigDebClkConfig)(TIMClockDiv_TypeDef ClockDiv,
                                               FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*BOOT_ROM_FUNC_PTR_H*/

