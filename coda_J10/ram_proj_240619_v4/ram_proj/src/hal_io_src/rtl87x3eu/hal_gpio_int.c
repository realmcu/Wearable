/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     hal_gpio_int.c
* @brief    This file provides all the gpio hal interrupt functions.
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/
#include <stdint.h>
#include "rtl876x.h"
#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "trace.h"
#include "vector_table.h"
#include "hal_gpio.h"
#include "hal_gpio_int.h"

#define TOTAL_HAL_GPIO_NUM                          (64)
#define TOTAL_HAL_GPIO_PORT_NUM                     (32)
#define TOTAL_HAL_PIN_NUM                           TOTAL_PIN_NUM

//#define PRINT_GPIO_LOGS
#ifdef PRINT_GPIO_LOGS
#define GPIO_PRINT_INFO(fmt, ...)                   DBG_DIRECT(fmt, __VA_ARGS__)
#else
#define GPIO_PRINT_INFO(...)
#endif

#define IS_GPIO_INVALID(gpio_num)                   ((gpio_num == 0xff) || (gpio_num >= TOTAL_HAL_GPIO_NUM))

#define SECOND_GPIO_IRQ_NUM(irqn)         ((irqn) & 0xFF)

typedef struct t_gpio_state
{
    uint32_t context[TOTAL_HAL_GPIO_NUM];
    P_GPIO_CBACK gpio_callback[TOTAL_HAL_GPIO_NUM];
} T_GPIO_INT_HANDLE;

extern T_GPIO_TYPE hal_gpio_get_type(uint8_t pin_index);

static T_GPIO_INT_HANDLE g_hal_gpio_isr;

static GPIO_TypeDef *hal_get_gpio(uint8_t gpio_num)
{
    if (gpio_num < 32)
    {
        return GPIOA;
    }
    else
    {
        return GPIOB;
    }
}

static IRQn_Type hal_gpio_get_irq_num(uint8_t p_gpio_num)
{
    if (p_gpio_num < GPIO2)
    {
        return (IRQn_Type)(GPIO_A0_IRQn + p_gpio_num - GPIO1);
    }
    else if (p_gpio_num < GPIO8)
    {
        return GPIO_A2_7_IRQn;
    }
    else if (p_gpio_num < GPIO16)
    {
        return GPIO_A16_23_IRQn;
    }
    else if (p_gpio_num < GPIO32)
    {
        return GPIO_A24_31_IRQn;
    }
    else if (p_gpio_num < GPIO40)
    {
        return GPIO_B0_7_IRQn;
    }
    else if (p_gpio_num < GPIO48)
    {
        return GPIO_B8_15_IRQn;
    }
    else if (p_gpio_num < GPIO56)
    {
        return GPIO_B16_23_IRQn;
    }
    else
    {
        return GPIO_B24_31_IRQn;
    }
}

static void hal_gpio_enable_nvic(uint8_t gpio_num, uint8_t proirity)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = hal_gpio_get_irq_num(gpio_num);
    NVIC_InitStruct.NVIC_IRQChannelPriority = proirity;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

bool hal_gpio_update_pin_table(void)
{
    return true;
}

static bool hal_gpio_register_isr_by_gpio(uint8_t gpio_num, P_GPIO_CBACK callback, uint32_t context)
{
    GPIO_TypeDef *p_gpio = hal_get_gpio(gpio_num);

    if (IS_GPIO_INVALID(gpio_num))
    {
        return false;
    }

    g_hal_gpio_isr.gpio_callback[gpio_num] = callback;
    g_hal_gpio_isr.context[gpio_num] = context;
    hal_gpio_enable_nvic(gpio_num, 4);

    return true;
}

static bool hal_gpio_get_isr_callback_by_gpio(uint8_t gpio_num, P_GPIO_CBACK *callback,
                                              uint32_t *context)
{
    GPIO_TypeDef *p_gpio = hal_get_gpio(gpio_num);

    if (IS_GPIO_INVALID(gpio_num))
    {
        return false;
    }

    *callback = g_hal_gpio_isr.gpio_callback[gpio_num];
    *context = g_hal_gpio_isr.context[gpio_num];

    return true;
}

static void imp_gpio_handler(GPIO_TypeDef *GPIOx, uint8_t gpio_num,
                             uint32_t gpio_pin,
                             P_GPIO_CBACK callback)
{
    GPIOx_ClearINTPendingBit(GPIOx, gpio_pin);
    if (callback)
    {
        callback(g_hal_gpio_isr.context[gpio_num]);
    }
    else
    {
        IO_PRINT_ERROR1("empty isr callback for gpioa%u", gpio_num);
    }
}

static void gpio_a_sub_isr(uint32_t real_interrupt_status)
{
    uint8_t idx;

    while (real_interrupt_status)
    {
        idx = 31 - __builtin_clz(real_interrupt_status);
        imp_gpio_handler(GPIOA, idx, BIT(idx), g_hal_gpio_isr.gpio_callback[idx]);
        real_interrupt_status &= ~BIT(idx);
    }
}

static void gpio_b_sub_isr(uint32_t real_interrupt_status)
{
    uint8_t idx;

    while (real_interrupt_status)
    {
        idx = 31 - __builtin_clz(real_interrupt_status);
        imp_gpio_handler(GPIOB, (idx + 32), BIT(idx), g_hal_gpio_isr.gpio_callback[idx]);
        real_interrupt_status &= ~BIT(idx);
    }
}

#define GPIOA_Handler(mask) gpio_a_sub_isr(GPIOA->INTSTATUS & mask)
#define GPIOB_Handler(mask) gpio_b_sub_isr(GPIOB->INTSTATUS & mask)

void GPIOA0_Handler(void)
{
    imp_gpio_handler(GPIOA, 0, BIT0, g_hal_gpio_isr.gpio_callback[0]);
}

void GPIOA1_Handler(void)
{
    imp_gpio_handler(GPIOA, 1, BIT1, g_hal_gpio_isr.gpio_callback[1]);
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

bool hal_gpio_register_isr_callback(uint8_t pin_index, P_GPIO_CBACK callback, uint32_t context)
{
#if 0  //(todo) add hal later
    if (hal_gpio_get_type(pin_index) == GPIO_TYPE_AUTO)
    {
        hal_pad_register_pin_wake_callback(pin_index, (P_PIN_WAKEUP_CALLBACK)callback, context);
    }
#endif
    return hal_gpio_register_isr_by_gpio(GPIO_GetNum(pin_index), callback, context);
}

bool hal_gpio_get_isr_callback(uint8_t pin_index, P_GPIO_CBACK *callback, uint32_t *context)
{
    return hal_gpio_get_isr_callback_by_gpio(GPIO_GetNum(pin_index), callback, context);
}

static void hal_gpio_isr_update(uint32_t irqn, IRQ_Fun isr_handler)
{
    uint8_t gpio_num = SECOND_GPIO_IRQ_NUM(irqn);
    hal_gpio_register_isr_by_gpio(gpio_num, (P_GPIO_CBACK)isr_handler, gpio_num);
}

void hal_gpio_int_deinit(void)
{

}

void hal_gpio_int_init(void)
{
    RamVectorTableRegister(GPIO_A2_7_IRQn, hal_gpio_isr_update);
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
