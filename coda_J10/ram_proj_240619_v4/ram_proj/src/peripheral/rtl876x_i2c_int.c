/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_i2c_int.c
* @brief    This file provides all the GPIO firmware internal functions.
* @details
* @author   colin
* @date     2024-06-17
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_i2c.h"
#include "pcc_reg.h"
#include "platform_utils.h"

uint8_t I2C_GetCompIndex(I2C_TypeDef *I2Cx)
{
    uint8_t compensate_index = 3;

    if (I2Cx == I2C0)
    {
        compensate_index = SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_I2C0_CLK_DIV;
    }
    else if (I2Cx == I2C1)
    {
        compensate_index = SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_I2C1_CLK_DIV;
    }
    else if (I2Cx == I2C2)
    {
        compensate_index = SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_I2C2_CLK_DIV;
    }

    return compensate_index;
}

/**
  * @brief  I2C clock divider config.
  * @param  I2Cx: where x can be 0 or 1 to select the I2C peripheral.
  * @param  ClockDiv: specifies the APB peripheral to gates its clock.
  *      this parameter can be one of the following values:
  *     @arg I2C_CLOCK_DIV_1
  *     @arg I2C_CLOCK_DIV_2
  *     @arg I2C_CLOCK_DIV_4
  *     @arg I2C_CLOCK_DIV_8
  * @retval None
  */
void I2C_ClkDivConfig(I2C_TypeDef *I2Cx, uint16_t ClockDiv)
{
    assert_param(IS_I2C_DIV(ClockDiv));

    /* Config I2C clock divider */
    if (I2Cx == I2C0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_I2C0_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_I2C0_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_I2C0_EN = 1;
    }
    else if (I2Cx == I2C1)
    {
        SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_I2C1_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_I2C1_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_I2C1_EN = 1;
    }
    else if (I2Cx == I2C2)
    {
        SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_I2C2_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_I2C2_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_238.BITS_238.BIT_SOC_ACTCK_I2C2_EN = 1;
    }

    return;
}
/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

