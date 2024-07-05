/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_spi_int.c
* @brief    This file provides all the SPI firmware internal functions.
* @details
* @author
* @date     2024-06-13
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_spi.h"
#include "platform_utils.h"

void SPI_SPI1AHBBridgeConfig(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        /* Open SPI1 AHB bridge*/
        SYSBLKCTRL->u_308.BITS_308.PON_SPI1_H2S_BRG_EN = 1;
    }
    else
    {
        /* Close SPI1 AHB bridge*/
        SYSBLKCTRL->u_308.BITS_308.PON_SPI1_H2S_BRG_EN = 0;
    }
}

void SPI_ConfigAPHBridge(SPI_TypeDef *SPIx)
{
    if (SPIx == SPI0)
    {
        /*Open SPI0 APB Bridge for high speed*/
        SYSBLKCTRL->u_308.BITS_308.PON_SPI0_BRIDGE_EN = 1;
    }
    else if (SPIx == SPI1)
    {
        SYSBLKCTRL->u_308.BITS_308.PON_SPI1_BRIDGE_EN = 1;
    }
    else if (SPIx == SPI2)
    {
        SYSBLKCTRL->u_308.BITS_308.PON_SPI2_BRIDGE_EN = 1;
    }
    else if (SPIx == SPI_SLAVE)
    {
        SYSBLKCTRL->u_308.BITS_308.PON_SPI_SLV_BRIDGE_EN = 1;
    }
}

/**
  * @brief  SPI clock source switch.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  ClockSource: specifies the APB peripheral to gates its clock.
  *      this parameter can be one of the following values:
  *     @arg SPI_CLOCK_SOURCE_PLL
  *     @arg SPI_CLOCK_SOURCE_40M
  * @retval None
  */
void SPI_ClkSourceSwitch(SPI_TypeDef *SPIx, uint16_t ClockSource)
{
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    assert_param(IS_SPI_SOR(ClockSource));

    // /* Switch spi clock source */
    // if ((SPIx == SPI1) || (SPIx == SPI1_HS))
    // {
    //     /* disable clock first */
    //     SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI1_EN = 0;
    //     platform_delay_us(1);

    //     SYSBLKCTRL->u_35C.BITS_35C.r_spi0_clk_src_pll_sel = ClockSource;

    //     platform_delay_us(1);
    //     SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI1_EN = 1;
    // }
    return;
}

/**
  * \brief  SPI clock divider config.
  * \param  SPIx: Select the SPI peripheral. \ref SPI_Declaration
  * \param  ClockDiv: specifies the APB peripheral to gates its clock. \ref Clock_Divider
  * \return None
  */
void SPI_ClkDivConfig(SPI_TypeDef *SPIx, uint16_t ClockDiv)
{
    assert_param(IS_SPI_CLK_DIV(ClockDiv));

    /* Config SPI clock divider */
    if (SPIx == SPI0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI0_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_SPI0_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI0_EN = 1;
    }
    else if (SPIx == SPI1)
    {
        /* disable clock first */
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI1_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_SPI1_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI1_EN = 1;

    }
    else if (SPIx == SPI2)
    {
        /* disable clock first */
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI2_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_SPI2_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_234.BITS_234.BIT_SOC_ACTCK_SPI2_EN = 1;
    }
    return;
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

