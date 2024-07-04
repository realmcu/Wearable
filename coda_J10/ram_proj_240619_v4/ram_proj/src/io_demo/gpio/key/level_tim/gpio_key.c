/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     gpio_key.c
* @brief      This file provides demo code of GPIO used as a key by level trigger mode with hw timer debounce.
* @details
* @author   renee
* @date     2017-3-10
* @version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <string.h>
#include "rtl876x.h"
#include "board.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "trace.h"
#include "vector_table.h"
#include "hw_tim.h"
#include "section.h"

/** @defgroup  GPIO_KEY  GPIO KEY DEMO
    * @brief  Gpio key using hw tim to debounce implementation demo code
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup Gpio_Key_Exported_Macros Gpio Key Exported Macros
  * @brief
  * @{
  */

#define KEY_PIN                       ADC_0
#define KEY_IRQN                      GPIO0_IRQn

#define KEY_PRESS_DEBOUNCE_TIME       (30 * 1000)            //30ms
#define KEY_RELEASE_DEBOUNCE_TIME     (30 * 1000)            //30ms

/** @} */ /* End of group Gpio_Key_Exported_Macros */

void KeyIntrHandler(void);

/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @defgroup Gpio_Key_Exported_Variables Gpio Key Exported Variables
  * @brief
  * @{
  */
uint8_t key_status = 1;
uint8_t isPress = false;

T_HW_TIMER_HANDLE debounce_timer_handle = NULL;

/** @} */ /* End of group Gpio_Key_Exported_Variables */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup Gpio_Key_Exported_Functions Gpio Key Exported Functions
  * @brief
  * @{
  */

/**
* @brief  When debouncing timer of key is timeout, this function shall be called.
*
*
* @param
* @return  void
*/
RAM_TEXT_SECTION
void debounce_hw_timer_callback(T_HW_TIMER_HANDLE handle)
{
    hw_timer_stop(debounce_timer_handle);

    if (key_status != GPIOx_ReadInputDataBit(GPIOA, GPIO_GetPin(KEY_PIN)))
    {
        GPIOx_MaskINTConfig(GPIOA, GPIO_GetPin(KEY_PIN), DISABLE);
        GPIOx_INTConfig(GPIOA, GPIO_GetPin(KEY_PIN), ENABLE);
        return;
    }

    if (key_status)
    {
        GPIO->INTPOLARITY &= ~(GPIO_GetPin(KEY_PIN));
        isPress = false;
        APP_PRINT_INFO0("debounce_hw_timer_callback: Key release.");
    }
    else
    {
        GPIO->INTPOLARITY |= GPIO_GetPin(KEY_PIN);
        isPress = true;
        APP_PRINT_INFO0("debounce_hw_timer_callback: Key press.");
    }
    GPIOx_ClearINTPendingBit(GPIOA, GPIO_GetPin(KEY_PIN));
    GPIOx_MaskINTConfig(GPIOA, GPIO_GetPin(KEY_PIN), DISABLE);
    GPIOx_INTConfig(GPIOA, GPIO_GetPin(KEY_PIN), ENABLE);
}

/**
* @brief  GPIO and TIM peripheral initial function.
*
*
* @param   none.
* @return  void
*/
void key_init(void)
{
    Pinmux_Config(KEY_PIN, DWGPIO);
    Pad_Config(KEY_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    GPIO_InitTypeDef
    KeyboardButton_Param;     /* Define Mouse Button parameter structure. Mouse button is configed as GPIO. */
    GPIO_StructInit(&KeyboardButton_Param);
    RCC_PeriphClockCmd(APBPeriph_GPIOA, APBPeriph_GPIOA_CLOCK, ENABLE);

    KeyboardButton_Param.GPIO_PinBit  = GPIO_GetPin(KEY_PIN);
    KeyboardButton_Param.GPIO_Mode = GPIO_Mode_IN;
    KeyboardButton_Param.GPIO_ITCmd = ENABLE;
    KeyboardButton_Param.GPIO_ITTrigger = GPIO_INT_Trigger_LEVEL;
    KeyboardButton_Param.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    KeyboardButton_Param.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_DISABLE;
    GPIOx_Init(GPIOA, &KeyboardButton_Param);

    GPIOx_INTConfig(GPIOA, GPIO_GetPin(KEY_PIN), ENABLE);
    GPIOx_MaskINTConfig(GPIOA, GPIO_GetPin(KEY_PIN), DISABLE);

    RamVectorTableUpdate(GPIO_A0_VECTORn, KeyIntrHandler);

    /*  Enable GPIO0 IRQ  */
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = KEY_IRQN;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    debounce_timer_handle = hw_timer_create("debouce_hw_timer", KEY_PRESS_DEBOUNCE_TIME, true,
                                            debounce_hw_timer_callback);
    if (debounce_timer_handle == NULL)
    {
        APP_PRINT_TRACE0("fail to create hw timer, check hw timer usage!!");
        return;
    }
}


/**
* @brief  GPIO interrupt trigger by button is handled in this function.
*
*
* @param
* @return  void
*/
/**
* @brief  GPIO Interrupt handler
*
*
* @return  void
*/
void KeyIntrHandler(void)
{
    /*  Mask GPIO interrupt */
    GPIOx_INTConfig(GPIOA, GPIO_GetPin(KEY_PIN), DISABLE);
    GPIOx_MaskINTConfig(GPIOA, GPIO_GetPin(KEY_PIN), ENABLE);
    GPIOx_ClearINTPendingBit(GPIOA, GPIO_GetPin(KEY_PIN));

    key_status = GPIOx_ReadInputDataBit(GPIOA, GPIO_GetPin(KEY_PIN));
    APP_PRINT_INFO1("KeyIntrHandler, key_status:%d", key_status);

    if (isPress == false)
    {
        hw_timer_restart(debounce_timer_handle, KEY_PRESS_DEBOUNCE_TIME);
    }
    else
    {
        hw_timer_restart(debounce_timer_handle, KEY_RELEASE_DEBOUNCE_TIME);
    }
}

/** @} */ /* End of group Gpio_Key_Exported_Functions */
/** @} */ /* End of group GPIO_KEY */

