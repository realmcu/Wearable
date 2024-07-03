/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_gpio_int.c
* @brief    This file provides all the GPIO firmware internal functions.
* @details
* @author   colin
* @date     2024-06-17
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#include "pin_def.h"
#include "platform_utils.h"
#include "vector_table.h"

/*============================================================================*
 *                        Private Defines
 *============================================================================*/
const uint8_t GPIO_mapping_tbl[TOTAL_PIN_NUM] =
{
    0, 1, 2, 3, /*!< ADC0 ADC1 ADC2 ADC3 */
    53, 54, 55, 56, /*!< ADC4 ADC5 ADC6 ADC7 */
    7, 8, 9, 10, /*!< P1_0 P1_1 P1_2 P1_3 */
    11, 12, 15, 16, /*!< P1_4 P1_5 P2_0 P2_1 */
    17, 18, 19, 20, /*!< P2_2 P2_3 P2_4 P2_5 */
    21, 22, 23, 24, /*!< P2_6 P2_7 P3_0 P3_1 */
    4, 5, 6, 57, /*!< P3_2 P3_3 P3_4(32K_XI) P3_5(32K_XO) */
    39, 40, 41, 42, /*!< P4_0 P4_1 P4_2 P4_3 */
    43, 44, 45, 46, /*!< P4_4 P4_5 P4_6 P4_7 */
    47, 48, 49, 50, /*!< P5_0 P5_1 P5_2 P5_3 */
    13, 14, 15, 25, /*!< P5_4 P5_5 P5_6 P6_0 */
    26, 27, 28, 29, /*!< P6_1 P6_2 P6_3 P6_4 */
    30, 31, 47, 48, /*!< P6_5 P6_6 P7_0 P7_1 */
    49, 50, 51, 52, /*!< P7_2 P7_3 P7_4 P7_5 */
    53, 54, 55, 56, /*!< P7_6 P7_7 SPIC1_WP#/SIO2 SPIC1_SO */
    57, 58, 59, 60, /*!< SPIC1_CSN SPIC1_SI SPIC1_SCK SPIC1_HOLD#/SIO3 */
    58, 59, 60, 61, /*!< P8_0 P8_1 P8_2 P8_3 */
    62, 63, 51, 52, /*!< P8_4 P8_5 P8_6 P8_7 */
    25, 26, 27, 28, /*!< MIC1_P MIC1_N MIC2_P MIC2_N */
    29, 30, 31, 45, /*!< MICBIAS DAOUT1_P DAOUT1_N DAOUT2_P */
    46, 32, 33, 34, /*!< DAOUT2_N P9_0 P9_1 P9_2 */
    35, 36, 37, 38, /*!< P9_3 P9_4 P9_5 P9_6 */
    61, 62, 63, 10, /*!< P10_0 P10_1 P10_2 P10_3 */
    11, 12, 13, 14, /*!< P10_4 P10_5 P10_6 P10_7 */
    16, 17, /*!< DP DM */
};

/**
 * \brief   Get GPIO GROUP through the given pad.
 *
 * \param[in] GPIOx: Select the GPIO peripheral. \ref GPIO_Declaration
 *
 * \return  GPIOx: GPIO peripheral. \ref GPIO_Declaration
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 *
 * void gpio_demo(void)
 * {
 *      GPIO_TypeDef gpio_port = GPIO_GetPort(P0_0);
 *      //result: gpio_port is GPIOA
 * }
 *
 * \endcode
 */
GPIO_TypeDef *GPIO_GetPort(uint8_t Pin_num)
{
    /* Check the parameters */
    assert_param(IS_PIN_NUM(Pin_num));

    uint8_t GPIO_num = GPIO_mapping_tbl[Pin_num];

    if (GPIO_num < 32)
    {
        return GPIOA;
    }
    else if (GPIO_num < 64)
    {
        return GPIOB;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief get the specified GPIO pin BIT.
  * @param  Pin_num: This parameter is from ADC_0 to P9_5, please refer to rtl876x.h .
  * @retval  GPIO pin  bit for GPIO initialization.
  */
uint32_t GPIO_GetPinBit(uint8_t Pin_num)
{
    /* Check the parameters */
    assert_param(IS_PIN_NUMBER(Pin_num));

    if (Pin_num >= TOTAL_PIN_NUM)
    {
        return 0;
    }

    uint8_t GPIO_num = GPIO_mapping_tbl[Pin_num];

    if (GPIO_num >= 64)
    {
        return 0;
    }

    return BIT(GPIO_num & 0x1F);
}

/**
  * @brief get the specified GPIO pin number.
  * @param  Pin_num: This parameter is from ADC_0 to P9_5, please refer to rtl876x.h
  * @retval  GPIO pin number.
  */
uint8_t GPIO_GetNum(uint8_t Pin_num)
{
    /* Check the parameters */
    assert_param(IS_PIN_NUMBER(Pin_num));

    if (Pin_num >= TOTAL_PIN_NUM)
    {
        return 0xff;
    }

    uint8_t GPIO_num = GPIO_mapping_tbl[Pin_num];
    if (GPIO_num < 64)
    {
        return GPIO_num;
    }
    else
    {
        return 0xFF;
    }
}

static void GPIO_DebcClkEnable(GPIO_TypeDef *GPIOx, FunctionalState NewState)
{
    if (GPIOx == GPIOA)
    {
        RCC_PeriphClockCmd(APBPeriph_GPIO0_DEB, APBPeriph_GPIO0_DEB_CLOCK, NewState);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_PeriphClockCmd(APBPeriph_GPIO1_DEB, APBPeriph_GPIO1_DEB_CLOCK, NewState);
    }
}

/**
  * \brief  Enable GPIO external debounce clock.
  * \param  GPIOx: Select the GPIO peripheral. \ref GPIO_Declaration
  * \param  GPIO_Pin: GPIO_Pin can be 0 to 31.
  * \param  NewState: Disable or enable gpio debounce clock.
  * \return None.
  */
void GPIO_ExtDebCmd(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, FunctionalState NewState)
{
    if (NewState == DISABLE)
    {
        GPIOx->GPIO_EXT_DEB_FUNC_CTL = GPIOx->GPIO_EXT_DEB_FUNC_CTL & (~GPIO_Pin);
    }
    else
    {
        GPIO_DebcClkEnable(GPIOx, ENABLE);
        GPIOx->GPIO_EXT_DEB_FUNC_CTL = (GPIOx->GPIO_EXT_DEB_FUNC_CTL & (~GPIO_Pin)) | GPIO_Pin;
    }
}

/**
  * @brief  Set GPIO debounce parameters.
  * @param  GPIOx: where x can be A to B to select the GPIO peripheral.
  * @param  GPIO_Pin: GPIO_Pin can be 0 to 31.
  * @param  GPIO_DebounceClkSource: select debounce count clk source, can be S11 or 32KHz.
  * @param  GPIO_DebounceClkDiv: divider selection.
  * @param  GPIO_DebounceCntLimit: debounce time can be calculated by count limit.
  * @return None.
  */
void GPIO_ExtDebUpdate(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin,
                       GPIODebounceSrc_TypeDef GPIO_DebounceClkSource,
                       GPIODebounceDiv_TypeDef GPIO_DebounceClkDiv,
                       uint8_t GPIO_DebounceCntLimit)
{
    for (uint8_t i = 0 ; i < 32; i++)
    {
        if (GPIO_Pin & BIT(i))
        {
            GPIO_EXT_DEB_CLK_CTL_TypeDef gpio_deb = {.d32 = GPIOx->GPIO_EXT_DEB_CLK_CTL[i / 2]};
            if (i)
            {
                gpio_deb.b.GPIO_G_H_CNT_LIMIT = GPIO_DebounceCntLimit;
                gpio_deb.b.GPIO_G_H_DEB_CLK_DIV = GPIO_DebounceClkDiv;
            }
            else
            {
                gpio_deb.b.GPIO_G_L_CNT_LIMIT = GPIO_DebounceCntLimit;
                gpio_deb.b.GPIO_G_L_DEB_CLK_DIV = GPIO_DebounceClkDiv;
            }
            GPIOx->GPIO_EXT_DEB_CLK_CTL[i / 2] = gpio_deb.d32;
        }
    }

    /* Configure Debounce enable register */
    GPIOx->GPIO_EXT_DEB_FUNC_CTL = (GPIOx->GPIO_EXT_DEB_FUNC_CTL & (~GPIO_Pin)) | GPIO_Pin;

    if (!((GPIOx->GPIO_INT_LV) & GPIO_Pin))
    {
        /* Delay 8T of 32K clock to avoid level interrupt triggered before de-bounce during initialzation. */
        platform_delay_us(250);
    }
}

void GPIO_ExtPolarity(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, GPIOITPolarity_TypeDef Int_Polarity)
{
    if (Int_Polarity == GPIO_INT_POLARITY_ACTIVE_LOW)
    {
        GPIOx->GPIO_EXT_DEB_POL_CTL &= (~GPIO_Pin);
    }
    else
    {
        GPIOx->GPIO_EXT_DEB_POL_CTL = GPIOx->GPIO_EXT_DEB_POL_CTL & (~GPIO_Pin) | GPIO_Pin;
    }
}

#define TOTAL_GPIO_NUM                          (64)
#define IS_GPIO_INVALID(gpio_num)               (gpio_num >= TOTAL_GPIO_NUM)
#define SECOND_GPIO_IRQ_NUM(irqn)               ((irqn) & 0xFF)

#define GPIOA_Handler(mask) gpio_a_sub_isr(GPIOA->GPIO_INT_STS & mask)
#define GPIOB_Handler(mask) gpio_b_sub_isr(GPIOB->GPIO_INT_STS & mask)


#define imp_gpio_handler(GPIOx, gpio_num, gpio_pin, callback)   do{\
        GPIOx_ClearINTPendingBit(GPIOx, gpio_pin);\
        if (callback)\
        {\
            callback(g_gpio_isr.context[gpio_num]);\
        }\
    }while(0)


typedef void (*P_GPIO_CBACK)(uint32_t context);

typedef struct t_gpio_state
{
    uint32_t context[TOTAL_GPIO_NUM];
    P_GPIO_CBACK gpio_callback[TOTAL_GPIO_NUM];
} T_GPIO_INT_HANDLE;

static T_GPIO_INT_HANDLE g_gpio_isr;

static bool gpio_register_isr_by_gpio(uint8_t gpio_num, P_GPIO_CBACK callback, uint32_t context)
{
    if (IS_GPIO_INVALID(gpio_num))
    {
        return false;
    }

    g_gpio_isr.gpio_callback[gpio_num] = callback;
    g_gpio_isr.context[gpio_num] = context;

    return true;
}

static void gpio_a_sub_isr(uint32_t real_interrupt_status)
{
    uint8_t idx;

    while (real_interrupt_status)
    {
        idx = 31 - __builtin_clz(real_interrupt_status);
        imp_gpio_handler(GPIOA, idx, BIT(idx), g_gpio_isr.gpio_callback[idx]);
        real_interrupt_status &= ~BIT(idx);
    }
}

static void gpio_b_sub_isr(uint32_t real_interrupt_status)
{
    uint8_t idx;

    while (real_interrupt_status)
    {
        idx = 31 - __builtin_clz(real_interrupt_status);
        imp_gpio_handler(GPIOB, (idx + 32), BIT(idx), g_gpio_isr.gpio_callback[idx]);
        real_interrupt_status &= ~BIT(idx);
    }
}

void GPIOA0_Handler(void)
{
    GPIOA_Handler(0x1);
}

void GPIOA1_Handler(void)
{
    GPIOA_Handler(0x02);
}

void GPIO_A_2_7_Handler(void)
{
    GPIOA_Handler(0xFC);
}

void GPIO_A_8_15_Handler(void)
{
    GPIOA_Handler(0xFF00);
}

void GPIO_A_16_23_Handler(void)
{
    GPIOA_Handler(0xFF0000);
}

void GPIO_A_24_31_Handler(void)
{
    GPIOA_Handler(0xFF000000);
}

void GPIO_B_0_7_Handler(void)
{
    GPIOB_Handler(0xFF);
}

void GPIO_B_8_15_Handler(void)
{
    GPIOB_Handler(0xFF00);
}

void GPIO_B_16_23_Handler(void)
{
    GPIOB_Handler(0xFF0000);
}

void GPIO_B_24_31_Handler(void)
{
    GPIOB_Handler(0xFF000000);
}


static void gpio_isr_update(uint32_t irqn, IRQ_Fun isr_handler)
{
    uint8_t gpio_num = SECOND_GPIO_IRQ_NUM(irqn);
    gpio_register_isr_by_gpio(gpio_num, (P_GPIO_CBACK)isr_handler, gpio_num);
}

void gpio_int_init(void)
{
    RamVectorTableRegister(GPIO_A2_7_IRQn, gpio_isr_update);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_A0_IRQn),        GPIOA0_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_A1_IRQn),        GPIOA1_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_A2_7_IRQn),      GPIO_A_2_7_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_A8_15_IRQn),     GPIO_A_8_15_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_A16_23_IRQn),    GPIO_A_16_23_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_A24_31_IRQn),    GPIO_A_24_31_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_B0_7_IRQn),      GPIO_B_0_7_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_B8_15_IRQn),     GPIO_B_8_15_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_B16_23_IRQn),    GPIO_B_16_23_Handler);
    RamVectorTableUpdate(IRQn_TO_VECTORn(GPIO_B24_31_IRQn),    GPIO_B_24_31_Handler);
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

