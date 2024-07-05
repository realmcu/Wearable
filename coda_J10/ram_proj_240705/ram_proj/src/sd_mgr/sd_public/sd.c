/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd.c
* @brief    This file provides application functions for sd library.
* @details
* @author   elliot_chen
* @date     2021-02-01
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "rtl876x_sdio.h"
#include "sdcard.h"

#include "sdh_int.h"
#include "sd_if.h"
#include "sd.h"

#ifdef SD_MONITOR_EN
#include "sd_monitor.h"
#endif

/* Defines -------------------------------------------------------------------*/
T_SD_CONFIG       sd_cfg;
/*
typedef enum
{
    SDH_CLK,
    SDH_CMD,
    SDH_D0,
    SDH_D1,
    SDH_D2,
    SDH_D3,
    SDH_WT_PROT,
    SDH_CD,
} T_SD_PIN_NUM;
*/
#ifdef TARGET_RTL87X3D
const uint8_t   sdio_pin_group0[8] = {P1_2, P1_3, P1_4, P1_5, P1_6, P1_7, P3_0, P3_1};
const uint8_t   sdio_pin_group1[8] = {P4_2, P4_3, P4_4, P4_5, P4_6, P4_7, P6_0, P6_1};
#endif

#ifdef TARGET_RTL87X3E
const uint8_t   sdio_pin_group0[8] = {P5_0, P5_1, P5_2, P5_3, P5_4, P5_5, P5_7, P5_6};
const uint8_t   sdio_pin_group1[8] = {P6_0, P6_1, P6_2, P6_3, P6_4, P6_5, P2_0, P2_3};
#endif

#if defined(CONFIG_SOC_SERIES_RTL8773E) || defined(IC_TYPE_BB2U)
const uint8_t   sdio_pin_group0[8] = {P5_0, P5_1, P5_2, P5_3, P5_4, P5_5, 0xFF, P5_6};
const uint8_t   sdio_pin_group1[8] = {P6_0, P6_1, P6_2, P6_3, P6_4, P6_5, P2_0, P2_3};
#endif

static uint8_t  *sdio_pin = NULL;

/* Gloabls -------------------------------------------------------------------*/
void sd_config_init(T_SD_CONFIG *app_sd_cfg)
{
    memcpy(&sd_cfg, app_sd_cfg, sizeof(T_SD_CONFIG));

    if (sd_cfg.sdh_group == GROUP_0)
    {
        sdio_pin = (uint8_t *)&sdio_pin_group0;
    }
    else
    {
        sdio_pin = (uint8_t *)&sdio_pin_group1;
    }
}

void sd_handle_card_insert_evt(void *param)
{
    //T_SD_STATUS *p_status = (T_SD_STATUS *)param;

    // Add application code here, please send message to task
}

void sd_handle_card_removal_evt(void *param)
{
    //T_SD_STATUS *p_status = (T_SD_STATUS *)param;

    // Add application code here, please send message to task
}

void sd_recovery_from_transfer_error(void *param)
{
    //T_SD_STATUS *p_status = (T_SD_STATUS *)param;

    // Add application code here
    // Send stop transfer cmd and retry, please send message to task
}

void sd_intr_msg_handle_init(void)
{
    SDH_RegisterIntHandleCB(SDH_CB_IDX_INT_CARD_INSERT, sd_handle_card_insert_evt);
    SDH_RegisterIntHandleCB(SDH_CB_IDX_INT_CARD_REMOVAL, sd_handle_card_removal_evt);
    SDH_RegisterIntHandleCB(SDH_CB_IDX_INT_ADMA_ERR, sd_recovery_from_transfer_error);
    SDH_RegisterIntHandleCB(SDH_CB_IDX_INT_RW_DONE, (SDH_INTR_HDL_CB)sd_cfg.rw_cback);
}

#if defined(CONFIG_SOC_SERIES_RTL8773E) || defined(IC_TYPE_BB2U)
static void sd_high_speed_set(void)
{
    Pad_HighSpeedMuxSel(sdio_pin[SDH_CLK], FROM_CORE_DOMAIN);
    Pad_HighSpeedMuxSel(sdio_pin[SDH_CMD], FROM_CORE_DOMAIN);
    Pad_HighSpeedMuxSel(sdio_pin[SDH_D0], FROM_CORE_DOMAIN);
    Pad_HighSpeedMuxSel(sdio_pin[SDH_D1], FROM_CORE_DOMAIN);
    Pad_HighSpeedMuxSel(sdio_pin[SDH_D2], FROM_CORE_DOMAIN);
    Pad_HighSpeedMuxSel(sdio_pin[SDH_D3], FROM_CORE_DOMAIN);
}

static void sd_pad_sdh_func_sel(void)
{
    Pad_HighSpeedFuncSel(sdio_pin[SDH_CLK], HS_Func1);
    Pad_HighSpeedFuncSel(sdio_pin[SDH_CMD], HS_Func1);
    Pad_HighSpeedFuncSel(sdio_pin[SDH_D0], HS_Func1);
    Pad_HighSpeedFuncSel(sdio_pin[SDH_D1], HS_Func1);
    Pad_HighSpeedFuncSel(sdio_pin[SDH_D2], HS_Func1);
    Pad_HighSpeedFuncSel(sdio_pin[SDH_D3], HS_Func1);
}
#endif

void sd_board_init(void)
{
    Pad_Config(sdio_pin[SDH_CLK], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pad_Config(sdio_pin[SDH_CMD], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

#if defined(CONFIG_SOC_SERIES_RTL8773E) || defined(IC_TYPE_BB2U)
    sd_high_speed_set();
    if (sd_cfg.sdh_group == GROUP_1)
    {
        sd_pad_sdh_func_sel();
    }
#endif

    if (sd_cfg.sd_power_en)
    {
        if (sd_cfg.sd_power_high_active)
        {
            Pad_Config(sd_cfg.sd_power_pin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                       PAD_OUT_HIGH);
        }
        else
        {
            Pad_Config(sd_cfg.sd_power_pin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE,
                       PAD_OUT_LOW);
        }
    }

    Pad_Config(sdio_pin[SDH_D0], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

    /* The host shall pull-up all DAT0-3 lines by RDAT,
       even if the host uses the SD Card as 1 bit-mode-only in SD mode. */
    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        Pad_Config(sdio_pin[SDH_D1], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
        Pad_Config(sdio_pin[SDH_D2], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
        Pad_Config(sdio_pin[SDH_D3], PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
    }
    else
    {
        Pad_Config(sdio_pin[SDH_D1], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
        Pad_Config(sdio_pin[SDH_D2], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
        Pad_Config(sdio_pin[SDH_D3], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                   PAD_OUT_HIGH);
    }

    /* bbpro3 evb sdio need PAD_STRONG_PULL or mutiblock r/w error */
    Pad_PullConfigValue(sdio_pin[SDH_CLK], PAD_STRONG_PULL);
    Pad_PullConfigValue(sdio_pin[SDH_CMD], PAD_STRONG_PULL);
    Pad_PullConfigValue(sdio_pin[SDH_D0], PAD_STRONG_PULL);
    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        Pad_PullConfigValue(sdio_pin[SDH_D1], PAD_STRONG_PULL);
        Pad_PullConfigValue(sdio_pin[SDH_D2], PAD_STRONG_PULL);
        Pad_PullConfigValue(sdio_pin[SDH_D3], PAD_STRONG_PULL);
    }
    Pinmux_Deinit(sdio_pin[SDH_CLK]);
    Pinmux_Deinit(sdio_pin[SDH_CMD]);
    Pinmux_Deinit(sdio_pin[SDH_D0]);

    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        Pinmux_Deinit(sdio_pin[SDH_D1]);
        Pinmux_Deinit(sdio_pin[SDH_D2]);
        Pinmux_Deinit(sdio_pin[SDH_D3]);
    }

    sd_if_pin_group_cfg((uint32_t)sd_cfg.sdh_group);

#ifdef SD_MONITOR_EN
    sd_monitor_init();
#endif
    sd_intr_msg_handle_init();
}

T_SD_STATUS sd_card_init(void)
{
    return sd_if_init((T_SD_IF_TYPE)sd_cfg.sd_if_type);
}

void sd_init(void)
{
    sd_board_init();
    sd_if_init((T_SD_IF_TYPE)sd_cfg.sd_if_type);
}

void sd_deinit(void)
{
    if (sd_cfg.sd_power_en)
    {
        if (sd_cfg.sd_power_high_active)
        {
            Pad_Config(sd_cfg.sd_power_pin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE,
                       PAD_OUT_LOW);
        }
        else
        {
            Pad_Config(sd_cfg.sd_power_pin, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE,
                       PAD_OUT_HIGH);
        }
    }

    sd_if_deinit();
    Pad_Config(sdio_pin[SDH_CLK], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(sdio_pin[SDH_CMD], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(sdio_pin[SDH_D0], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE,
               PAD_OUT_LOW);

    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        Pad_Config(sdio_pin[SDH_D1], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE,
                   PAD_OUT_LOW);
        Pad_Config(sdio_pin[SDH_D2], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE,
                   PAD_OUT_LOW);
        Pad_Config(sdio_pin[SDH_D3], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_DISABLE,
                   PAD_OUT_LOW);
    }

    sd_sdh_clk_cmd(false);
}

void sd_suspend(void)
{
    Pad_Config(sdio_pin[SDH_CLK], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(sdio_pin[SDH_CMD], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(sdio_pin[SDH_D0], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        Pad_Config(sdio_pin[SDH_D1], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
        Pad_Config(sdio_pin[SDH_D2], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
        Pad_Config(sdio_pin[SDH_D3], PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    }

    sd_sdh_clk_cmd(false);
}

void sd_resume(void)
{
    sd_board_init();
    sd_if_sdh_pow_on();
}

void sd_print_binary_data(uint8_t *p, uint32_t len)
{
    if (len < 120)
    {
        sd_print_bin("sd_print_binary_data: buf: %b", \
                     TRACE_BINARY(len, p));
    }
    else
    {
        uint8_t print_loop = len / 120;
        uint8_t i = 0;
        while (print_loop--)
        {
            sd_print_bin("sd_print_binary_data: buf[%d]: %b", \
                         i, TRACE_BINARY(120, (p + 120 * i)));
            i++;
        }
        sd_print_bin("sd_print_binary_data: buf[%d]: %b", \
                     i, TRACE_BINARY(len % 120, (p + 120 * i)));
    }
}

T_SD_STATUS sd_erase(uint32_t start_addr, uint32_t end_addr)
{
    return sd_if_erase(start_addr, end_addr);
}

T_SD_STATUS sd_read(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num)
{
    return  sd_if_read(sector,  buf,  blk_size,  blk_num);
}

T_SD_STATUS sd_write(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num)
{
    return sd_if_write(sector,  buf,  blk_size,  blk_num);
}

T_SD_STATUS sd_set_block_len(uint32_t block_len)
{
    return sd_if_set_block_len(block_len);
}

void sd_sdh_clk_cmd(bool NewState)
{
    return  sd_if_sdh_clk_cmd(NewState);
}

T_SD_STATUS sd_check_program_status(void)
{
    return sd_if_check_program_status();
}

uint32_t sd_get_dev_block_size(void)
{
    return sd_if_get_dev_block_size();
}

uint64_t sd_get_dev_capacity(void)
{
    return sd_if_get_dev_capacity();
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

