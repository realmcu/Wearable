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
#include "platform_utils.h"

#ifdef IC_TYPE_BBPRO3
#include "pll.h"
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
        platform_delay_us(200);
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
  * @brief  Initialize SD host controller in card indentification mode.
  * @param  None.
  * @retval T_SD_STATUS: exection status.
  */
T_SD_STATUS SDH_PowerOnForIdentifyCard(void)
{
    uint32_t time_out = SDIO_HOST_INIT_TIMEOUT;

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
            //SDIO_PRINT_ERROR0("SDH_PowerOnForIdentifyCard: ret = SD_HOST_SW_RESET_ERROR");
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
            //SDIO_PRINT_ERROR0("SDH_PowerOnForIdentifyCard: ret = SD_HOST_INTER_CLOCK_ERROR");
            return SD_HOST_INTER_CLOCK_ERROR;
        }
    }

    /* Initialize the SDIO host peripheral */
    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_StructInit(&SDIO_InitStructure);
    SDIO_InitStructure.SDIO_TimeOut  = 0x0E;
    SDIO_InitStructure.SDIO_BusWide  = SDIO_BusWide_1b;
    SDIO_InitStructure.SDIO_ClockDiv = SDIO_CLOCK_DIV_128;
    //SDIO_PRINT_INFO2("SDH_PowerOnForIdentifyCard: SDH_CLOCK_DIV = 0x%x sd_cfg.sdh_bus_width =%d",SDIO_CLOCK_DIV_128,SDIO_BusWide_1b);

    SDIO_Init(&SDIO_InitStructure);

    /* Enable SD bus power */
    SDIO_SetBusPower(SDIO_PowerState_ON);

    return SD_OK;
}

static void SDH_ClkSelPLL4(void)
{
#ifdef IC_TYPE_BBPRO3

    uint32_t reg_tmp;
    pll4_enable_cko1();
    reg_tmp = *(__IO uint32_t *)(0x40000340UL);
    reg_tmp &= ~BIT(9);
    reg_tmp |= PLL4;
    *(__IO uint32_t *)(0x40000340UL) = reg_tmp;
    if (!(SDIO->CAPABILITIES_L & BIT(21)))
    {
        //SDIO_PRINT_ERROR0("HIGH speed not support");
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
    RCC_PeriphClockCmd(APBPeriph_SD_HOST, APBPeriph_SD_HOST_CLOCK, ENABLE);

    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_StructInit(&SDIO_InitStructure);
    /* !< TMCLK * 2^SDIO_TimeOut */
    SDIO_InitStructure.SDIO_TimeOut  = 0x0E;
    SDIO_InitStructure.SDIO_BusWide  = sd_cfg.sdh_bus_width;
    if (SDH_CLOCK_DIV >= SD_BUS_CLK_40M)
    {
        SDH_ClkSelPLL4();
    }
    SDIO_InitStructure.SDIO_ClockDiv = (SDH_CLOCK_DIV & 0xff00);
    //SDIO_PRINT_INFO2("SDH_InitForDataTransfer: SDH_CLOCK_DIV = 0x%x sd_cfg.sdh_bus_width =%d",SDH_CLOCK_DIV,sd_cfg.sdh_bus_width);

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
            //SDIO_PRINT_ERROR0("SDH_PowerOnForDataTransfer: ret = SD_HOST_SW_RESET_ERROR");
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
            //SDIO_PRINT_ERROR0("SDH_PowerOnForDataTransfer: ret = SD_HOST_INTER_CLOCK_ERROR");
            return SD_HOST_INTER_CLOCK_ERROR;
        }
    }

    /* Initialize the SDIO host peripheral */
    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_StructInit(&SDIO_InitStructure);
    SDIO_InitStructure.SDIO_TimeOut  = 0x0E;
    SDIO_InitStructure.SDIO_BusWide  = sd_cfg.sdh_bus_width;
    SDIO_InitStructure.SDIO_ClockDiv = SDH_CLOCK_DIV;
    //SDIO_PRINT_INFO2("SDH_PowerOnForDataTransfer: SDH_CLOCK_DIV = 0x%x sd_cfg.sdh_bus_width =%d",SDH_CLOCK_DIV,sd_cfg.sdh_bus_width);

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
