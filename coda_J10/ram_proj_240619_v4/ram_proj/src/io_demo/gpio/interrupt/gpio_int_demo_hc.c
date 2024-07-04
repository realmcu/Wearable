/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     gpio_int_demo.c
* @brief    This file provides demo code of GPIO interrupt mode.
* @details
* @author   renee
* @date     2017-06-23
* @version  v1.0
*********************************************************************************************************
*/



/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include "rtl876x_rcc.h"
#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "vector_table.h"

/** @defgroup  GPIO_INT_DEMO  GPIO INTERRUPT DEMO
    * @brief  Gpio interrupt implementation demo code
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup Gpio_Interrupt_Exported_Macros Gpio Interrupt Exported Macros
  * @brief
  * @{
  */

#define TEST_Pin                    ADC_2
#define GPIO_Test_Pin               GPIO_GetPin(TEST_Pin)
#define GPIO_IRQ                    GPIO2_IRQn
/** @} */ /* End of group Gpio_Interrupt_Exported_Macros */

void gpio_handler(void);

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup Gpio_Interrupt_Exported_Functions Gpio Interrupt Exported Functions
  * @brief
  * @{
  */
/**
 * @brief  initialization of pinmux settings and pad settings.
 * @param   No parameter.
 * @return  void  */
void board_gpio_init(void)
{
    Pad_Config(TEST_Pin, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    Pinmux_Config(TEST_Pin, DWGPIO);
}

/**
  * @brief  Initialize GPIO peripheral.
  * @param   No parameter.
  * @return  void
  */
void driver_gpio_init(void)
{
    /* turn on GPIO clock */
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_PinBit  = GPIO_Test_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_ITCmd = ENABLE;
    GPIO_InitStruct.GPIO_ITTrigger = GPIO_INT_Trigger_EDGE;
    GPIO_InitStruct.GPIO_ITPolarity = GPIO_INT_POLARITY_ACTIVE_LOW;
    GPIO_InitStruct.GPIO_ITDebounce = GPIO_INT_DEBOUNCE_ENABLE;
    GPIO_InitStruct.GPIO_DebounceTime = 30;
    GPIOx_Init(GPIOA, &GPIO_InitStruct);
    GPIOx_MaskINTConfig(GPIOA, GPIO_Test_Pin, DISABLE);
    GPIOx_INTConfig(GPIOA, GPIO_Test_Pin, ENABLE);

    RamVectorTableUpdate(GPIOA2_VECTORn, gpio_handler);

    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = GPIO_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/**
  * @brief  Initialize GPIO peripheral.
  * @param   No parameter.
  * @return  void
  */
void gpio_test(void)
{
    GPIOx_ReadInputData(GPIOA);
}

/**
  * @brief  demo code of operation about GPIO.
  * @param   No parameter.
  * @return  void
  */
void gpio_demo(void)
{
    /* Configure PAD and pinmux firstly! */
    board_gpio_init();

    /* Initialize GPIO peripheral */
    driver_gpio_init();

    /* GPIO function */
    gpio_test();

}

/**
* @brief  GPIO interrupt handler function.
* @param   No parameter.
* @return  void
*/
void gpio_handler(void)
{
    GPIOx_MaskINTConfig(GPIOA, GPIO_Test_Pin, ENABLE);

    // add user code here

    GPIOx_ClearINTPendingBit(GPIOA, GPIO_Test_Pin);
    GPIOx_MaskINTConfig(GPIOA, GPIO_Test_Pin, DISABLE);
}

/** @} */ /* End of group Gpio_Interrupt_Exported_Functions */
/** @} */ /* End of group GPIO_INT_DEMO */

