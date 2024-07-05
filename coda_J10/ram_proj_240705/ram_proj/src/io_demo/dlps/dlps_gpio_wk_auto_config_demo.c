/**
*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     dlps_gpio_wk_auto_config_demo.c
* @brief    This file provides demo code of GPIO wakeup from DLPS mode by auto config.
* @details
* @author   colin
* @date     2023-04-11
* @version  v1.0
*********************************************************************************************************
*/



/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include "trace.h"
#include "io_dlps.h"
#include "pm.h"
#include "section.h"
#include "hal_gpio.h"
#include "hal_gpio_int.h"

/** @defgroup  DLPS_GPIO_WK_AUTO_CONFIG_DEMO  DLPS GPIO INTERRUPT
    * @brief  DLPS GPIO wakeup implementation demo code
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup DLPS_GPIO_Exported_Macros DLPS GPIO wakeup Exported Macros
  * @brief
  * @{
  */
#define TEST_Pin                      P0_0
#define GPIO_Test_Pin                 GPIO_GetPin(TEST_Pin)
#define GPIO_IRQ                      GPIO0_IRQn
/** @} */ /* End of group DLPS_GPIO_Exported_Macros */

void dlps_gpio_wk_init(void);

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup DLPS_GPIO_wakeup_Exported_Functions
  * @brief
  * @{
  */
/**
  * @brief  read GPIO input.
  * @param   No parameter.
  * @return  void
  */
void gpio_test(void)
{
    T_GPIO_LEVEL gpio_level = hal_gpio_get_input_level(TEST_Pin);

    IO_PRINT_INFO1("gpio level =0x%x\n", gpio_level);
}

/**
* @brief  GPIO interrupt handler function.
* @param   No parameter.
* @return  void
*/
ISR_TEXT_SECTION
void gpio_handler(uint32_t context)
{
    uint8_t pin_index = (uint32_t)context;
    T_GPIO_LEVEL gpio_level = hal_gpio_get_input_level(pin_index);

    IO_PRINT_INFO1("gpio_handler level %d", gpio_level);

    // add user code here
}

void driver_gpio_init(void)
{
    hal_gpio_init();
    hal_gpio_int_init();
    hal_gpio_set_debounce_time(30);

    /* GPIO of the AUTO type will automatically configure the wake function when enter DLPS */
    hal_gpio_init_pin(TEST_Pin, GPIO_TYPE_AUTO, GPIO_DIR_INPUT, GPIO_PULL_UP);

    hal_gpio_set_up_irq(TEST_Pin, GPIO_IRQ_EDGE, GPIO_IRQ_ACTIVE_LOW, true);

    hal_gpio_register_isr_callback(TEST_Pin, gpio_handler, TEST_Pin);

    hal_gpio_irq_enable(TEST_Pin);
}

void dlps_store(void)
{
    DBG_DIRECT("enter dlps!!");
}


void dlps_restore(void)
{
    IO_PRINT_INFO0("exit dlps!!");
}

void dlps_gpio_wk_init(void)
{
    /*IO register  store restore callback register*/
    io_dlps_register();
    /*user callback fuciton register*/
    io_dlps_register_enter_cb(dlps_store);
    io_dlps_register_exit_cb(dlps_restore);

    /*set btmac to dsm mode*/
    bt_power_mode_set(BTPOWER_DEEP_SLEEP);
    /*set platfrom as dlps mode*/
    power_mode_set(POWER_DLPS_MODE);
}

/**
  * @brief  demo code of operation about GPIO.
  * @param   No parameter.
  * @return  void
  */
void dlps_gpio_wk_demo(void)
{
    /* Initialize GPIO peripheral */
    driver_gpio_init();

    /* GPIO function */
    gpio_test();

    /* dlps gpio wakup init */
    dlps_gpio_wk_init();
}
/** @} */ /* End of group DLPS_GPIO_wkup_Exported_Functions */
/** @} */ /* End of group DLPS_GPIO_WK_AUTO_CONFIG_DEMO */
