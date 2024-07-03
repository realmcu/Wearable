/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdh_pwr.c
* @brief    This file provides SD host power management firmware functions for sd library.
* @details
* @author   elliot_chen
* @date     2021-02-02
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_sdio.h"
#include "sdh_pwr.h"
#ifdef TARGET_RTL87X3D
#include "pll.h"
#elif TARGET_RTL87X3E
#include "pm.h"
#endif

/* Internal Defines ----------------------------------------------------------*/
#define SDIO_HOST_INIT_TIMEOUT          ((uint32_t)0xFFFFFF)
#define   PLL4  BIT(9)

/**
  * @brief  Start or stop the SDIO Operation Clock. Please move to rtl876x_sdio.c later!
  * @param  NewState: new state of the SDIO Operation Clock.
    This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_OperationClockCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Start clock */
        SDIO->CLK_CTRL |= ((uint32_t)0x01) << 2;
    }
    else
    {
        /* Stop clock */
        SDIO->CLK_CTRL &= ~(((uint32_t)0x1) << 2);
    }
}

/**
  * @brief  Start or stop the SDI host operation clock
  * @param  NewState: new state of the SD host operation clock.
    This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDH_ClockCmd(FunctionalState NewState)
{
    return SDIO_OperationClockCmd(NewState);
}

/**
  * @brief  Get the SD host clock divider. SD clock is cpu clock / clock divider.
  * @param  None
  * @retval sdh_clk_div
  */
uint32_t SDH_GetClockDiv(void)
{
    uint32_t sdh_clk_div = sd_cfg.sd_bus_clk_sel;

#ifdef TARGET_RTL87X3E
    uint32_t cpu_freq_mhz = pm_cpu_freq_get();
    uint8_t div_factor = 1;

    if ((cpu_freq_mhz >= 60) && (sdh_clk_div <= 0x80))
    {
        div_factor = 2;

        if (sd_cfg.sd_bus_clk_sel == 0x0)
        {
            sdh_clk_div = 0x1;
        }
        else
        {
            sdh_clk_div = sd_cfg.sd_bus_clk_sel * div_factor;
        }

        if (sdh_clk_div > 0x80)
        {
            sdh_clk_div = 0x80;
        }
    }

    sd_print_info("SDH_GetClockDiv: cpu_freq_mhz %d, sdh_clk_div 0x%x", cpu_freq_mhz, sdh_clk_div);
#endif

    sdh_clk_div = sdh_clk_div << 8;

    return sdh_clk_div;
}

/**
  * @brief  Initialize SD host controller in card indentification mode.
  * @param  None.
  * @retval T_SD_STATUS: exection status.
  */
T_SD_STATUS SDH_PowerOnForIdentifyCard(void)
{
    uint32_t time_out = SDIO_HOST_INIT_TIMEOUT;
#if defined(CONFIG_SOC_SERIES_RTL8773E) || defined(IC_TYPE_BB2U)
    RCC_SDIOClockConfig(SDIO_CLOCK_SOURCE_PLL2, SDIO_CLK_DIV_2);
#endif

    /* Enable SDIO clock */
    RCC_PeriphClockCmd(APBPeriph_SD_HOST, APBPeriph_SD_HOST_CLOCK, DISABLE);
    RCC_PeriphClockCmd(APBPeriph_SD_HOST, APBPeriph_SD_HOST_CLOCK, ENABLE);
    /* Software reset host controller */
    SDIO_SoftwareReset();
    while (SDIO_GetSoftwareResetStatus() == SET)
    {
        time_out--;
        if (time_out == 0)
        {
            sd_print_error("SDH_PowerOnForIdentifyCard: SD_HOST_SW_RESET_ERROR");
            return SD_HOST_SW_RESET_ERROR;
        }
    }

    /* Enable internal clcok */
    SDIO_InternalClockCmd(ENABLE);
    time_out = SDIO_HOST_INIT_TIMEOUT;
    while (SDIO_GetInternalClockStatus() == RESET)
    {
        time_out--;
        if (time_out == 0)
        {
            sd_print_error("SDH_PowerOnForIdentifyCard: SD_HOST_INTER_CLOCK_ERROR");
            return SD_HOST_INTER_CLOCK_ERROR;
        }
    }

    /* Initialize the SDIO host peripheral */
    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_StructInit(&SDIO_InitStructure);
    SDIO_InitStructure.SDIO_TimeOut  = 0x0E;
    SDIO_InitStructure.SDIO_BusWide  = SDIO_BusWide_1b;
    SDIO_InitStructure.SDIO_ClockDiv = SDIO_CLOCK_DIV_128;
    SDIO_Init(&SDIO_InitStructure);

    /* Enable SD bus power */
    SDIO_SetBusPower(SDIO_PowerState_ON);

    return SD_OK;
}

static void SDH_ClkSelPLL4(void)
{
#ifdef TARGET_RTL87X3D

    uint32_t reg_tmp;
    pll4_enable_cko1();
    reg_tmp = *(__IO uint32_t *)(0x40000340UL);
    reg_tmp &= ~BIT(9);
    reg_tmp |= PLL4;
    *(__IO uint32_t *)(0x40000340UL) = reg_tmp;
    if (!(SDIO->CAPABILITIES_L & BIT(21)))
    {
        sd_print_error("SDH_ClkSelPLL4: high speed not support");
    }
    /* Enable highspeed */
    SDIO->HOST_CTRL &=  ~(BIT(2));
    SDIO->HOST_CTRL |=  BIT2;

#endif
}
/**
  * @brief  Initialize SD host controller in card data transfer mode.
  * @param  None.
  * @retval none.
  */
void SDH_InitForDataTransfer(void)
{
    uint32_t sdh_clk_div = SDH_GetClockDiv();

    RCC_PeriphClockCmd(APBPeriph_SD_HOST, APBPeriph_SD_HOST_CLOCK, ENABLE);

    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_StructInit(&SDIO_InitStructure);
    /* !< TMCLK * 2^SDIO_TimeOut */
    SDIO_InitStructure.SDIO_TimeOut  = 0x0E;
    SDIO_InitStructure.SDIO_BusWide  = sd_cfg.sdh_bus_width;
    if (sd_cfg.sd_bus_clk_sel >= SD_BUS_CLK_40M)
    {
        SDH_ClkSelPLL4();
    }
    SDIO_InitStructure.SDIO_ClockDiv = (sdh_clk_div & 0xff00);
    SDIO_Init(&SDIO_InitStructure);
}

/**
  * @brief  SD host controller power on and initialization afeter exit DLPS mode.
  * @param  None.
  * @retval T_SD_STATUS: exection status.
  */
T_SD_STATUS SDH_PowerOnForDataTransfer(void)
{
    uint32_t time_out = SDIO_HOST_INIT_TIMEOUT;
    uint32_t sdh_clk_div = SDH_GetClockDiv();
#if defined(CONFIG_SOC_SERIES_RTL8773E) || defined(IC_TYPE_BB2U)
    RCC_SDIOClockConfig(SDIO_CLOCK_SOURCE_PLL2, SDIO_CLK_DIV_2);
#endif

    /* Enable SDIO clock */
    RCC_PeriphClockCmd(APBPeriph_SD_HOST, APBPeriph_SD_HOST_CLOCK, DISABLE);
    RCC_PeriphClockCmd(APBPeriph_SD_HOST, APBPeriph_SD_HOST_CLOCK, ENABLE);
    /* Software reset host controller */
    SDIO_SoftwareReset();
    while (SDIO_GetSoftwareResetStatus() == SET)
    {
        time_out--;
        if (time_out == 0)
        {
            sd_print_error("SDH_PowerOnForDataTransfer: SD_HOST_SW_RESET_ERROR");
            return SD_HOST_SW_RESET_ERROR;
        }
    }

    /* Enable internal clcok */
    SDIO_InternalClockCmd(ENABLE);
    time_out = SDIO_HOST_INIT_TIMEOUT;
    while (SDIO_GetInternalClockStatus() == RESET)
    {
        time_out--;
        if (time_out == 0)
        {
            sd_print_error("SDH_PowerOnForDataTransfer: SD_HOST_INTER_CLOCK_ERROR");
            return SD_HOST_INTER_CLOCK_ERROR;
        }
    }

    /* Initialize the SDIO host peripheral */
    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_StructInit(&SDIO_InitStructure);
    SDIO_InitStructure.SDIO_TimeOut  = 0x0E;
    SDIO_InitStructure.SDIO_BusWide  = sd_cfg.sdh_bus_width;
    SDIO_InitStructure.SDIO_ClockDiv = sdh_clk_div;
    SDIO_Init(&SDIO_InitStructure);
    /* Enable SD bus power */
    SDIO_SetBusPower(SDIO_PowerState_ON);

    return SD_OK;
}

/**
  * @brief  SD host pin group selection.
  * @param  None.
  * @retval None.
  */
void SDH_PinGroupConfig(uint32_t group_index, uint32_t data_width)
{
    SDIO_PinGroupConfig(group_index);
    SDIO_DataPinConfig(data_width, ENABLE);
    SDIO_PinOutputCmd(ENABLE);
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
