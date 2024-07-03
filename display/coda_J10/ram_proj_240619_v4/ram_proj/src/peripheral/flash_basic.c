/**
 *****************************************************************************************
 *     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file    flash_basic.c
 * @brief   Flash basic implementation for ROM
 * @author  Yao-Yu
 * @date    2020-07-08
 * @version v0.1
 * ***************************************************************************************
 */

#include <string.h>

#include "rtl876x_pinmux.h"
#include "rtl8763_syson_reg.h"
#include "system_rtl876x.h"
#include "rtl876x_aon_reg.h"
#include "rtl876x_aon.h"
//#include "aon_fast_access.h"
//#include "utils.h"
//#include "otp.h"
#include "flash_basic.h"
#include "rtl876x_aon.h"

static FLASH_EXIST_LV flash_exists[FLASH_IDX_MAX] = {FLASH_EXIST_NONE};
static SPIC_TypeDef *flash_idx_spic_map[FLASH_IDX_MAX] = {SPIC0, SPIC1, SPIC2};

uint32_t FlashSrcClk = CLOCK_40MHZ;
FLASH_INFO_STRUCT *flash_info[FLASH_IDX_MAX];
FLASH_INFO_STRUCT default_flash_info;

#define FLASH_XTAL_10MHZ 0
FLASH_SETTINGS_CFG flash_setting =
{
    .flash_task_priority = 1,
    .flash_task_stack_depth = 1,    /* Stack Depth ((n + 1) * 128) Byte */
    .flash_task_wait_tick = 4,      /* Wait (n + 1) ticks */
    .flash_task_linklist_length = 0,
    .flash_dma_ch = 1,
#ifdef _IS_ASIC_
    .flash_clk = FLASH_XTAL_20MHZ,  /* Configure clock rate for flash0 */
#else
    .flash_clk = FLASH_XTAL_10MHZ,  /* Configure clock rate for flash0 */
#endif
    .ftl_page_size = 0x1000,
    .spic_baud = 1,                 /* Configure baud for SPIC0 */
    .data_addr_check_enable = 1,
    .power_mode = 0,                /* 0: HV33_HQ (3.3V), 1: AVCC_DRV (1.8V) */
    .hardfault_record_cfg = 0x7,    /* Refer to hardfault configuration in hardfault_record.h */
    .bp_lv = 0,                     /* Configure block protect level */
    .cmp_en = 0,                    /* Enable block protect range complement */
    .hpm_en = 0,                    /* Enable high performance mode or continuous read mode */
    .wait_max_retry = 10000000,     /* Maximum retry for both SPIC busy bit & flash WIP bit */
    .erase_max_retry = 10,
    .dp_release_cyc_len = 4,
    .dp_release_delay_us = 10,
    .sw_reset_delay_us = 60,        /* GD requires maximum delay, which is about 60 us */
    .wait_busy_delay_us = 1,
    .init_cmd_from_cfg = 0,         /* Enable flash startup required cmd from platform config */
    .init_query_from_cfg = 1,       /* Enable flash query info got from platform config */
    .init_cmd_rdid = 0x9F,          /* RDID cmd used for init if init_cmd_from_cfg equals to 1 */
    .init_cmd_rd_data = 0x03,       /* Data read cmd used for init if init_cmd_from_cfg equals to 1 */
    .init_cmd_dp_release = 0xAB,    /* Release DP cmd used for init if init_cmd_from_cfg equals to 1 */
    .init_cmd_en_reset = 0x66,      /* Enable reset cmd used for init if init_cmd_from_cfg equals to 1 */
    .init_cmd_reset = 0x99,         /* Reset cmd used for init if init_cmd_from_cfg equals to 1 */

};
const uint32_t flash_bp_range_table[] =
{
    FLASH_BP_LV_0,
    FLASH_BP_LV_1,
    FLASH_BP_LV_2,
    FLASH_BP_LV_3,
    FLASH_BP_LV_4,
    FLASH_BP_LV_5,
    FLASH_BP_LV_6,
    FLASH_BP_LV_7,
    FLASH_BP_LV_8,
    FLASH_BP_LV_9,
};

const FLASH_QUERY_INFO_TABLE_STRUCT flash_query_info_table[] =
{
    /* MXIC */
    {FLASH_VENDOR_MXIC,    0x2012, {      256 * 1024, 0x6, 0x1A, 0x1B, 0x5, 0xFF, 3, 0x3}}, /* MX25V2033F */
    {FLASH_VENDOR_MXIC,    0x2314, { 1 * 1024 * 1024, 0x6, 0x1A, 0x1B, 0xB, 0xFF, 5, 0xF}}, /* MX25V8035F */
    {FLASH_VENDOR_MXIC,    0x2811, {      128 * 1024, 0x6, 0x1A, 0x1B, 0xB, 0xFF, 2, 0x3}}, /* MX25R1035F */
    {FLASH_VENDOR_MXIC,    0x2812, {      256 * 1024, 0x6, 0x1A, 0x1B, 0xB, 0xFF, 3, 0x3}}, /* MX25R2035F */
    {FLASH_VENDOR_MXIC,    0x2814, { 1 * 1024 * 1024, 0x6, 0x1A, 0x1B, 0xB, 0xFF, 5, 0xF}}, /* MX25R8035F */
    {FLASH_VENDOR_MXIC,    0x2816, { 4 * 1024 * 1024, 0x6, 0x1A, 0x1B, 0xB, 0xFF, 7, 0xF}}, /* MX25R3235F */
    {FLASH_VENDOR_MXIC,    0x2817, { 8 * 1024 * 1024, 0x6, 0x1A, 0x1B, 0xB, 0xFF, 8, 0xF}}, /* MX25R6435F */

    /* GD */
    {FLASH_VENDOR_GD,      0x4014, { 1 * 1024 * 1024, 0x9, 0x0A, 0x0F, 0x5, 0x0E, 5, 0x7}}, /* GD25Q80C */
    {FLASH_VENDOR_GD,      0x4015, { 2 * 1024 * 1024, 0x9, 0x0A, 0x0F, 0x5, 0x0E, 6, 0x7}}, /* GD25Q16C */
    {FLASH_VENDOR_GD,      0x4016, { 4 * 1024 * 1024, 0x9, 0x0A, 0x0F, 0x5, 0x0E, 7, 0x7}}, /* GD25Q32C */
    {FLASH_VENDOR_GD,      0x6015, { 2 * 1024 * 1024, 0x9, 0x0A, 0x0F, 0x5, 0x0E, 6, 0x7}}, /* GD25LQ16C */
    {FLASH_VENDOR_GD,      0x6016, { 4 * 1024 * 1024, 0x9, 0x0A, 0x0F, 0x5, 0x0E, 7, 0x7}}, /* GD25LQ32D */
    {FLASH_VENDOR_GD,      0x6017, { 8 * 1024 * 1024, 0x9, 0x0A, 0x0F, 0x5, 0x0E, 7, 0x7}}, /* GD25LQ64C */

    /* Winbond */
    {FLASH_VENDOR_WINBOND, 0x4014, { 1 * 1024 * 1024, 0x9, 0x0F, 0x0F, 0x5, 0x0E, 5, 0x7}}, /* W25Q80DV/DL */
    {FLASH_VENDOR_WINBOND, 0x4015, { 2 * 1024 * 1024, 0x9, 0x0F, 0x0F, 0x5, 0x0E, 6, 0x7}}, /* W25Q16JV */
    {FLASH_VENDOR_WINBOND, 0x6012, {      256 * 1024, 0x9, 0x0F, 0x0F, 0x5, 0x0E, 3, 0x3}}, /* W25Q20EW */
    {FLASH_VENDOR_WINBOND, 0x6014, { 1 * 1024 * 1024, 0x9, 0x0F, 0x0F, 0x5, 0x0E, 5, 0x7}}, /* W25Q80EW */
};

/* *INDENT-OFF* */
static void flash_set_pad_config(FLASH_INTERFACE_TYPE type)
{
    /* By default PU SPI CSN to prevent release flash DP in low power mode */
    BTAON_FAST_0x140_TYPE aon_0x140 = {.d8[1] = btaon_fast_read_8b(BTAON_FAST_140 + 1)};
    aon_0x140.AON_PAD_SPI_CSN_PU_EN = 1;
    aon_0x140.AON_PAD_SPI_CSN_PD = 0;
    btaon_fast_write_8b(BTAON_FAST_140 + 1, aon_0x140.d8[1]);

    /* Power on flash pad */
    BTAON_FAST_BT_PWDPAD_TYPE aon_pwdpad = {.d8[0] = btaon_fast_read_8b(BTAON_FAST_BT_PWDPAD)};
    aon_pwdpad.PWDPAD1_FLASH = 0;
    btaon_fast_write_8b(BTAON_FAST_BT_PWDPAD, aon_pwdpad.d8[0]);

    /* Switch flash pad to core domain */
    BTAON_FAST_BT_PAD_SPI_FLASH_TYPE aon_pad_spi_flash = {.d8[0] = btaon_fast_read_8b(BTAON_FAST_BT_PAD_SPI_FLASH)};
    aon_pad_spi_flash.PAD_SPI_FLASH_SHDN = 1;
    aon_pad_spi_flash.AON_PAD_SPI_FLASH_S = 1;
    btaon_fast_write_8b(BTAON_FAST_BT_PAD_SPI_FLASH, aon_pad_spi_flash.d8[0]);

    /* Config MP mode flash dedicated pads polarity (support 1-bit mode only) */
    if (type == FLASH_SLAVE_EXTERNAL)
    {
        Pad_Config(P2_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(P2_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(AUX_L, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(AUX_R, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(MIC1_N, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
        Pad_Config(MIC1_P, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    }
}

static void flash_set_pin_mux(FLASH_INTERFACE_TYPE type)
{
    if (type == FLASH_MASTER_INTERNAL)
    {
        SYSBLKCTRL->u_2A8.BITS_2A8.PMUX_FLASH_EN = 0;
    }
    else if (type == FLASH_SLAVE_EXTERNAL)
    {
        SYSBLKCTRL->u_2A8.BITS_2A8.PMUX_FLASH_EN = 1;
    }
    else
    {
        return;
    }
}

static uint32_t flash_addr_order_rev(uint32_t in, uint8_t len)
{
    uint8_t *in_byte = (uint8_t *)&in;
    uint32_t out = 0;

    for (uint8_t i = 0; i < len; i++)
    {
        out |= in_byte[i] << ((len - 1 - i) * 8);
    }

    return out;
}

static void flash_load_init_cmd(FLASH_IDX_TYPE idx)
{
    /* Load default general cmd */
    flash_info[idx]->basic.cmd_rd_id = 0x9F;
    flash_info[idx]->basic.cmd_rd_sr = 0x05;
    flash_info[idx]->basic.cmd_wr_sr = 0x01;
    flash_info[idx]->basic.cmd_rd_data = 0x03;
    flash_info[idx]->basic.cmd_pp = 0x02;
    flash_info[idx]->basic.cmd_chip_e = 0x60;
    flash_info[idx]->basic.cmd_block_e = 0xD8;
    flash_info[idx]->basic.cmd_sector_e = 0x20;
    flash_info[idx]->basic.cmd_wr_en = 0x06;
    flash_info[idx]->basic.cmd_wr_en_vol = 0x50;
    flash_info[idx]->basic.cmd_wr_di = 0x04;
    flash_info[idx]->basic.cmd_dp = 0xB9;
    flash_info[idx]->basic.cmd_dp_release = 0xAB;
    flash_info[idx]->basic.cmd_en_reset = 0x66;
    flash_info[idx]->basic.cmd_reset = 0x99;
    flash_info[idx]->adv.cmd_rd_dual_o = 0x3B;
    flash_info[idx]->adv.cmd_rd_dual_io = 0xBB;
    flash_info[idx]->adv.cmd_rd_quad_o = 0x6B;
    flash_info[idx]->adv.cmd_rd_quad_io = 0xEB;
    flash_info[idx]->adv.cmd_pp_quad_i = 0x32;
    flash_info[idx]->adv.cmd_pp_quad_ii = 0x38;
    flash_info[idx]->adv.cmd_suspend = 0x75;
    flash_info[idx]->adv.cmd_resume = 0x7A;

    if ((idx == FLASH_IDX_SPIC0) & flash_setting.init_cmd_from_cfg)
    {
        flash_info[idx]->basic.cmd_rd_id = flash_setting.init_cmd_rdid;
        flash_info[idx]->basic.cmd_rd_data = flash_setting.init_cmd_rd_data;
        flash_info[idx]->basic.cmd_dp_release = flash_setting.init_cmd_dp_release;
        flash_info[idx]->basic.cmd_en_reset = flash_setting.init_cmd_en_reset;
        flash_info[idx]->basic.cmd_reset = flash_setting.init_cmd_reset;
    }
}

static void flash_push_addr_to_spic_dr(SPIC_TypeDef *SPIC, uint32_t addr, uint32_t addr_len)
{
    uint32_t addr_rev = flash_addr_order_rev(addr, addr_len);

    while (addr_len--)
    {
        spic_set_dr(SPIC, addr_rev & 0xFF, SPIC_DATA_BYTE);
        addr_rev = addr_rev >> 8;
    }
}

void flash_set_flash_exist(FLASH_IDX_TYPE idx, FLASH_EXIST_LV exist_lv)
{
    flash_exists[idx] |= exist_lv;
}

FLASH_EXIST_LV flash_get_flash_exist(FLASH_IDX_TYPE idx)
{
    return flash_exists[idx];
}

void flash_reset_flash_exist(FLASH_IDX_TYPE idx)
{
    flash_exists[idx] = FLASH_EXIST_NONE;
}

void flash_set_interface(FLASH_INTERFACE_TYPE type)
{
    flash_set_pad_config(type);
    flash_set_pin_mux(type);

    if ((type == FLASH_MASTER_INTERNAL) || (type == FLASH_SLAVE_EXTERNAL))
    {
        flash_power_ctrl(true);
    }
}

FLASH_RETURN_TYPE flash_cmd_rx_rom(FLASH_IDX_TYPE idx, FLASH_ACCESS_INFO *info, uint8_t *buf, uint8_t buf_len)
{
    if ((buf_len > 0) && (buf == NULL))
    {
        return FLASH_PARAM_INVALID;
    }

    uint32_t max_retry = flash_setting.wait_max_retry;
    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];

    if (spic_lock(SPIC) == false)
    {
        return FLASH_RET_LOCK_FAILED;
    }

    spic_enable(SPIC, false);
    spic_set_user_mode(SPIC);
    spic_set_rx_mode(SPIC);
    spic_set_cmd_len(SPIC, 1);
    spic_set_user_addr_len(SPIC, info->addr_len);
    spic_set_user_dummy_len(SPIC, info->dummy_len);
    spic_set_txndf(SPIC, 0);
    spic_set_rxndf(SPIC, buf_len);
    spic_set_multi_ch(SPIC, (SPIC_CFG_CH)info->cmd_ch, (SPIC_CFG_CH)info->addr_ch, (SPIC_CFG_CH)info->data_ch);
    spic_set_dr(SPIC, info->cmd, SPIC_DATA_BYTE);

    if (info->addr_len != 0)
    {
        flash_push_addr_to_spic_dr(SPIC, info->addr, info->addr_len);
    }

    spic_enable(SPIC, true);

    while (buf_len--)
    {
        *buf++ = spic_get_dr(SPIC, SPIC_DATA_BYTE);
    }

    while ((max_retry--) && spic_in_busy(SPIC));

    if (spic_unlock(SPIC) == false)
    {
        return FLASH_RET_UNLOCK_FAILED;
    }

    spic_set_auto_mode(SPIC);

    return max_retry ? FLASH_RET_SUCCESS : FLASH_RET_WAIT_BUSY_FAILED;
}
FLASH_RETURN_TYPE(*flash_cmd_rx)(FLASH_IDX_TYPE idx, FLASH_ACCESS_INFO *info, uint8_t *buf, uint8_t buf_len) = flash_cmd_rx_rom;

FLASH_RETURN_TYPE flash_cmd_tx_rom(FLASH_IDX_TYPE idx, FLASH_ACCESS_INFO *info, uint8_t *buf, uint8_t buf_len)
{
    if ((buf_len > 0) && (buf == NULL))
    {
        return FLASH_PARAM_INVALID;
    }

    uint32_t max_retry = flash_setting.wait_max_retry;
    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];

    if (spic_lock(SPIC) == false)
    {
        return FLASH_RET_LOCK_FAILED;
    }

    spic_enable(SPIC, false);
    spic_set_user_mode(SPIC);
    spic_set_tx_mode(SPIC);
    spic_set_cmd_len(SPIC, 1);
    spic_set_user_addr_len(SPIC, info->addr_len);
    spic_set_txndf(SPIC, buf_len);
    spic_set_multi_ch(SPIC, (SPIC_CFG_CH)info->cmd_ch, (SPIC_CFG_CH)info->addr_ch, (SPIC_CFG_CH)info->data_ch);
    spic_set_dr(SPIC, info->cmd, SPIC_DATA_BYTE);

    if (info->addr_len != 0)
    {
        flash_push_addr_to_spic_dr(SPIC, info->addr, info->addr_len);
    }

    while (buf_len--)
    {
        spic_set_dr(SPIC, *buf++, SPIC_DATA_BYTE);
    }

    spic_enable(SPIC, true);

    while ((max_retry--) && spic_in_busy(SPIC));

    if (spic_unlock(SPIC) == false)
    {
        return FLASH_RET_UNLOCK_FAILED;
    }

    spic_set_auto_mode(SPIC);

    return max_retry ? FLASH_RET_SUCCESS : FLASH_RET_WAIT_BUSY_FAILED;
}
FLASH_RETURN_TYPE(*flash_cmd_tx)(FLASH_IDX_TYPE idx, FLASH_ACCESS_INFO *info, uint8_t *buf, uint8_t buf_len) = flash_cmd_tx_rom;

FLASH_RETURN_TYPE flash_wait_busy_rom(FLASH_IDX_TYPE idx)
{
    uint8_t sr;
    FLASH_RETURN_TYPE ret;
    uint32_t max_retry = flash_setting.wait_max_retry;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    info.cmd = flash_info[idx]->basic.cmd_rd_sr;

    do
    {
        ret = flash_cmd_rx(idx, &info, &sr, 1);

        if (ret != FLASH_RET_SUCCESS)
        {
            return ret;
        }
    }
    while ((max_retry--) && (sr & BIT_FLASH_STATUS_WIP));

    ret = (max_retry ? FLASH_RET_SUCCESS : FLASH_RET_WAIT_BUSY_FAILED);

    return ret;
}
FLASH_RETURN_TYPE(*flash_wait_busy)(FLASH_IDX_TYPE idx) = flash_wait_busy_rom;

void flash_activate_rom(FLASH_IDX_TYPE idx)
{
    /* The clock source configs are the spic clock instead of spi_sclk since
       flash spi_sclk are divided from spic clock */

    switch (idx)
    {
    case FLASH_IDX_SPIC0:
        {
            BTAON_REG_0xC_TYPE aon_0xc = {.d32 = HAL_READ32(SYSTEM_REG_BASE, BTAON_REG_0xC)};
            BTAON_REG_0x14_TYPE aon_0x14 = {.d32 = HAL_READ32(SYSTEM_REG_BASE, BTAON_REG_0x14)};
            SYSBLKCTRL->u_20C.BITS_20C.r_flash_clk_src_sel_0 = 0;
            SYSBLKCTRL->u_20C.BITS_20C.r_flash_clk_src_sel_1 = !aon_0xc.POW_OSC40;
            SYSBLKCTRL->u_20C.BITS_20C.r_FLASH_CLK_SRC_EN = 1;
            SYSBLKCTRL->u_210.BITS_210.BIT_SOC_FLASH_EN = 1;
            SYSBLKCTRL->u_230.BITS_230.BIT_SOC_ACTCK_FLASH_EN = 1;
            break;
        }
    case FLASH_IDX_SPIC1:
    case FLASH_IDX_SPIC2:
    default:
        break;
    }
}
void (*flash_activate)(FLASH_IDX_TYPE idx) = flash_activate_rom;

FLASH_RETURN_TYPE flash_init(FLASH_IDX_TYPE idx, FLASH_INFO_STRUCT *info)
{
    if ((info == NULL) || (flash_info[idx] != NULL))
    {
        return FLASH_RET_INFO_NOT_RESET;
    }

    if (idx == FLASH_IDX_SPIC0)
    {
        /* Only the flash connect to spic0 requires to set interface */
        flash_set_interface(FLASH_MASTER_INTERNAL);
    }

    flash_activate(idx);

    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];
    spic_init(SPIC);
    spic_set_config(SPIC, flash_setting.wait_max_retry);

    spic_lock(SPIC);
    spic_disable_interrupt(SPIC);
    spic_unlock(SPIC);

    memset(info, 0, sizeof(FLASH_INFO_STRUCT));
    flash_info[idx] = info;

    flash_reset_flash_exist(idx);
    flash_load_init_cmd(idx);
    flash_dp_ctrl(idx, false);
    flash_sw_reset(idx);

    uint32_t rdid = flash_get_rdid(idx);

    if (rdid == 0)
    {
        return FLASH_RET_CMD_NOT_SUPPORT;
    }

    flash_set_flash_exist(idx, FLASH_EXIST_BASIC_CMD);
    flash_info[idx]->manu_id = (rdid >> 16) & 0xFF;
    flash_info[idx]->device_id = rdid & 0xFFFF;

    FLASH_RETURN_TYPE ret;

    if ((ret = flash_update_cmd(idx)) != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    if ((ret = flash_load_query_info(idx)) != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    return flash_set_auto_mode(idx, FLASH_1_BIT_MODE);
}


void test_spic_flash_auto(void)
{
    flash_init(FLASH_IDX_SPIC0, &default_flash_info);
    flash_set_bp_lv(FLASH_IDX_SPIC0, 0); 
    HAL_WRITE32(0x800000, 0, 0x04030201); 
//    HAL_WRITE32(0x800000, 4, 0x08070605); 
//    flash_set_auto_mode(FLASH_IDX_SPIC0, FLASH_4_BIT_MODE);
//    spic_enable(SPIC0,ENABLE);
}

void flash_power_ctrl(bool is_on)
{
    BTAON_FAST_RG0X_MBIAS_TYPE aon_rg0x_mbias = {.d8[0] = 0, .d8[1] = 0};
    aon_rg0x_mbias.d8[0] = btaon_fast_read_8b(BTAON_FAST_RG0X_MBIAS);
    aon_rg0x_mbias.d8[1] = btaon_fast_read_8b(BTAON_FAST_RG0X_MBIAS + 1);

    if (is_on)
    {
        aon_rg0x_mbias.PCUT_MBIAS_VG1_33 = 0;
        btaon_fast_write_8b(BTAON_FAST_RG0X_MBIAS + 1, aon_rg0x_mbias.d8[1]);
    }
    else
    {
        aon_rg0x_mbias.PCUT_MBIAS_VG2_33 = 1;
        btaon_fast_write_8b(BTAON_FAST_RG0X_MBIAS, aon_rg0x_mbias.d8[0]);
    }

    /* Flash power control delay, must > 62.5us */
    platform_delay_us(63);

    if (is_on)
    {
        aon_rg0x_mbias.PCUT_MBIAS_VG2_33 = 0;
        btaon_fast_write_8b(BTAON_FAST_RG0X_MBIAS, aon_rg0x_mbias.d8[0]);
    }
    else
    {
        aon_rg0x_mbias.PCUT_MBIAS_VG1_33 = 1;
        btaon_fast_write_8b(BTAON_FAST_RG0X_MBIAS + 1, aon_rg0x_mbias.d8[1]);
    }
}

FLASH_RETURN_TYPE flash_dp_ctrl(FLASH_IDX_TYPE idx, bool enable)
{
    uint8_t buf[7];
    FLASH_RETURN_TYPE ret = FLASH_RET_UNKNOWN;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    memset(buf, 0, sizeof(buf));

    info.cmd = flash_info[idx]->basic.cmd_dp_release;

    /* dp_release_cyc_len includes 1 cycle cmd which is configured in info variable */
    ret = flash_cmd_tx(idx, &info, buf, flash_setting.dp_release_cyc_len - 1);

    if (ret != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    platform_delay_us(flash_setting.dp_release_delay_us);

    if (enable)
    {
        info.cmd = flash_info[idx]->basic.cmd_dp;
        ret = flash_cmd_tx(idx, &info, NULL, 0);

        if (ret != FLASH_RET_SUCCESS)
        {
            return ret;
        }
    }

    return ret;
}

FLASH_RETURN_TYPE flash_sw_reset(FLASH_IDX_TYPE idx)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_UNKNOWN;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    info.cmd = flash_info[idx]->basic.cmd_en_reset;

    if ((ret = flash_cmd_tx(idx, &info, NULL, 0)) != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    info.cmd = flash_info[idx]->basic.cmd_reset;

    if ((ret = flash_cmd_tx(idx, &info, NULL, 0)) != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    platform_delay_us(flash_setting.sw_reset_delay_us);

    return ret;
}

uint32_t flash_get_rdid(FLASH_IDX_TYPE idx)
{
    uint8_t id[3];
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    info.cmd = flash_info[idx]->basic.cmd_rd_id;

    if (flash_cmd_rx(idx, &info, id, 3) != FLASH_RET_SUCCESS)
    {
        return 0;
    }

    return ((id[0] << 16) | (id[1] << 8) | id[2]);
}

FLASH_RETURN_TYPE flash_update_cmd(FLASH_IDX_TYPE idx)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_BASIC_CMD) == 0)
    {
        return FLASH_RET_DEV_NOT_SUPPORT;
    }

    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    if (flash_get_flash_exist(idx) & FLASH_EXIST_ADV_CMD)
    {
        return ret;
    }

    switch (flash_info[idx]->manu_id)
    {
    case FLASH_VENDOR_MXIC:
        ret = flash_update_mxic_cmd(idx);
        break;
    case FLASH_VENDOR_GD:
        ret = flash_update_gd_cmd(idx);
        break;
    case FLASH_VENDOR_WINBOND:
        ret = flash_update_winbond_cmd(idx);
        break;
    case FLASH_VENDOR_ESMT:
        ret = flash_update_esmt_cmd(idx);
        break;
    case FLASH_VENDOR_FUDAN:
        ret = flash_update_fudan_cmd(idx);
        break;
    default:
        ret = flash_update_new_vendor_cmd(idx);
        break;
    }

    if (ret == FLASH_RET_SUCCESS)
    {
        flash_set_flash_exist(idx, FLASH_EXIST_ADV_CMD);
    }

    return ret;
}

FLASH_RETURN_TYPE flash_update_mxic_cmd_rom(FLASH_IDX_TYPE idx)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    flash_info[idx]->basic.cmd_wr_en_vol = 0x00;

    switch (flash_info[idx]->device_id)
    {
    case 0x2012:
        flash_info[idx]->basic.cmd_dp_release = 0;
        break;
    case 0x2314:
    case 0x2811:
    case 0x2812:
    case 0x2814:
    case 0x2816:
    case 0x2817:
        flash_info[idx]->basic.cmd_dp_release = 0;
        flash_info[idx]->adv.cmd_rd_cr = 0x15;
        break;
    default:
        ret = FLASH_RET_DEV_NOT_SUPPORT;
        break;
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_update_mxic_cmd)(FLASH_IDX_TYPE idx) = flash_update_mxic_cmd_rom;

FLASH_RETURN_TYPE flash_update_gd_cmd_rom(FLASH_IDX_TYPE idx)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    flash_info[idx]->basic.cmd_wr_en_vol = 0x50;

    switch (flash_info[idx]->device_id)
    {
    case 0x4014:
    case 0x4015:
    case 0x6015:
    case 0x6016:
        break;
    case 0x4016:
        flash_info[idx]->adv.cmd_wr_sr2 = 0x31;
        break;
    default:
        ret = FLASH_RET_DEV_NOT_SUPPORT;
        break;
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_update_gd_cmd)(FLASH_IDX_TYPE idx) = flash_update_gd_cmd_rom;

FLASH_RETURN_TYPE flash_update_winbond_cmd_rom(FLASH_IDX_TYPE idx)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    flash_info[idx]->basic.cmd_wr_en_vol = 0x50;

    switch (flash_info[idx]->device_id)
    {
    case 0x4014:
    case 0x4015:
        break;
    case 0x6012:
    case 0x6014:
        flash_info[idx]->adv.cmd_wr_sr2 = 0x31;
        break;
    default:
        ret = FLASH_RET_DEV_NOT_SUPPORT;
        break;
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_update_winbond_cmd)(FLASH_IDX_TYPE idx) = flash_update_winbond_cmd_rom;

FLASH_RETURN_TYPE flash_update_esmt_cmd_rom(FLASH_IDX_TYPE idx)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    flash_info[idx]->basic.cmd_wr_en_vol = 0x50;
    flash_info[idx]->adv.cmd_rd_sr2 = 0x09;

    switch (flash_info[idx]->device_id)
    {
    case 0x3014:
        flash_info[idx]->adv.cmd_suspend = 0xB0;
        flash_info[idx]->adv.cmd_resume = 0x30;
    default:
        ret = FLASH_RET_DEV_NOT_SUPPORT;
        break;
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_update_esmt_cmd)(FLASH_IDX_TYPE idx) = flash_update_esmt_cmd_rom;

FLASH_RETURN_TYPE flash_update_fudan_cmd_rom(FLASH_IDX_TYPE idx)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    flash_info[idx]->basic.cmd_wr_en_vol = 0x50;

    switch (flash_info[idx]->device_id)
    {
    case 0x4013:
    case 0x4014:
        break;
    default:
        ret = FLASH_RET_DEV_NOT_SUPPORT;
        break;
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_update_fudan_cmd)(FLASH_IDX_TYPE idx) = flash_update_fudan_cmd_rom;

FLASH_RETURN_TYPE flash_update_new_vendor_cmd_rom(FLASH_IDX_TYPE idx)
{
    return FLASH_RET_VENDOR_NOT_SUPPORT;
}
FLASH_RETURN_TYPE(*flash_update_new_vendor_cmd)(FLASH_IDX_TYPE idx) = flash_update_new_vendor_cmd_rom;

FLASH_RETURN_TYPE flash_write_enable_rom(FLASH_IDX_TYPE idx, FLASH_WREN_MODE mode)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    info.cmd = (mode == FLASH_WREN_DATA || (flash_info[idx]->basic.cmd_wr_en_vol == 0)) ?
               flash_info[idx]->basic.cmd_wr_en : flash_info[idx]->basic.cmd_wr_en_vol;

    ret = flash_cmd_tx(idx, &info, NULL, 0);

    if (ret == FLASH_RET_SUCCESS)
    {
        ret = flash_wait_busy(idx);
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_write_enable)(FLASH_IDX_TYPE idx, FLASH_WREN_MODE mode) = flash_write_enable_rom;

FLASH_RETURN_TYPE flash_set_status_regs_rom(FLASH_IDX_TYPE idx, uint8_t *sr1, uint8_t *sr2, uint8_t *sr3)
{
    bool multi_wr_cmd = (((sr2 != NULL) && (flash_info[idx]->adv.cmd_wr_sr2 != 0)) ||
                         ((sr3 != NULL) && (flash_info[idx]->adv.cmd_wr_sr3 != 0)));

    if (multi_wr_cmd && ((flash_get_flash_exist(idx) & FLASH_EXIST_ADV_CMD) == 0))
    {
        return FLASH_RET_NOT_EXIST_ADV_CMD;
    }

    uint8_t buflen = 0;
    uint8_t *sr[3] = {sr1, sr2, sr3};
    uint8_t rd_sr[3] = {0};
    uint8_t wr_sr[3] = {0};

    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    if (sr3 != NULL)
    {
        if (multi_wr_cmd)
        {
            info.cmd = flash_info[idx]->adv.cmd_wr_sr3;
            flash_write_enable(idx, FLASH_WREN_REG);
            flash_cmd_tx(idx, &info, sr3, 1);
            flash_wait_busy(idx);
        }
        else
        {
            buflen = 3;
        }
    }

    if (sr2 != NULL)
    {
        if (multi_wr_cmd)
        {
            info.cmd = flash_info[idx]->adv.cmd_wr_sr2;
            flash_write_enable(idx, FLASH_WREN_REG);
            flash_cmd_tx(idx, &info, sr2, 1);
            flash_wait_busy(idx);
        }
        else
        {
            buflen = (buflen > 2) ? buflen : 2;
        }
    }

    if (sr1 != NULL)
    {
        if (multi_wr_cmd)
        {
            info.cmd = flash_info[idx]->basic.cmd_wr_sr;
            flash_write_enable(idx, FLASH_WREN_REG);
            flash_cmd_tx(idx, &info, sr1, 1);
            flash_wait_busy(idx);
        }
        else
        {
            buflen = (buflen > 1) ? buflen : 1;
        }
    }

    if (multi_wr_cmd)
    {
        return FLASH_RET_SUCCESS;
    }

    for (uint8_t i = 0; i < buflen; i++)
    {
        if (sr[i] == NULL)
        {
            wr_sr[i] = rd_sr[i];
        }
        else
        {
            wr_sr[i] = *sr[i];
        }
    }

    info.cmd = flash_info[idx]->basic.cmd_wr_sr;

    flash_write_enable(idx, FLASH_WREN_REG);
    flash_cmd_tx(idx, &info, wr_sr, buflen);
    flash_wait_busy(idx);

    return FLASH_RET_SUCCESS;
}
FLASH_RETURN_TYPE(*flash_set_status_regs)(FLASH_IDX_TYPE idx, uint8_t *sr1, uint8_t *sr2, uint8_t *sr3) = flash_set_status_regs_rom;

FLASH_RETURN_TYPE flash_get_status_regs_rom(FLASH_IDX_TYPE idx, uint8_t *sr1, uint8_t *sr2, uint8_t *sr3)
{
    if ((sr2 || sr3) && ((flash_get_flash_exist(idx) & FLASH_EXIST_ADV_CMD) == 0))
    {
        return FLASH_RET_NOT_EXIST_ADV_CMD;
    }

    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    if (sr1 != NULL)
    {
        info.cmd = flash_info[idx]->basic.cmd_rd_sr;
        ret = flash_cmd_rx(idx, &info, sr1, 1);
    }

    if ((sr2 != NULL) && (ret == FLASH_RET_SUCCESS))
    {
        if (flash_info[idx]->adv.cmd_rd_sr2 != 0)
        {
            info.cmd = flash_info[idx]->adv.cmd_rd_sr2;
            ret = flash_cmd_rx(idx, &info, sr2, 1);
        }
        else if (flash_info[idx]->adv.cmd_rd_cr != 0)
        {
            info.cmd = flash_info[idx]->adv.cmd_rd_cr;
            ret = flash_cmd_rx(idx, &info, sr2, 1);
        }
        else
        {
            *sr2 = 0xFF;
        }
    }

    if ((sr3 != NULL) && (ret == FLASH_RET_SUCCESS))
    {
        if (flash_info[idx]->adv.cmd_rd_sr3 != 0)
        {
            info.cmd = flash_info[idx]->adv.cmd_rd_sr3;
            ret = flash_cmd_rx(idx, &info, sr3, 1);
        }
        else if (flash_info[idx]->adv.cmd_rd_cr != 0)
        {
            uint8_t tmp[2];
            info.cmd = flash_info[idx]->adv.cmd_rd_cr;
            ret = flash_cmd_rx(idx, &info, tmp, 2);
            *sr3 = tmp[1];
        }
        else
        {
            *sr3 = 0xFF;
        }
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_get_status_regs)(FLASH_IDX_TYPE idx, uint8_t *sr1, uint8_t *sr2, uint8_t *sr3) = flash_get_status_regs_rom;

FLASH_RETURN_TYPE flash_load_query_info_rom(FLASH_IDX_TYPE idx)
{
    if (flash_get_flash_exist(idx) >= FLASH_EXIST_QUERY_INFO)
    {
        return FLASH_RET_SUCCESS;
    }

    uint8_t manu_id = flash_info[idx]->manu_id;
    uint16_t device_id = flash_info[idx]->device_id;

    if ((idx == FLASH_IDX_SPIC0) && flash_setting.init_query_from_cfg)
    {
        if ((flash_info[idx]->manu_id == flash_setting.query_info.manu_id) &&
            (flash_info[idx]->device_id == flash_setting.query_info.device_id))
        {
            flash_info[idx]->query = &flash_setting.query_info.query;
            return FLASH_RET_SUCCESS;
        }
    }

    for (uint8_t i = 0; i < FLASH_QUERY_INFO_TABLE_ENTRY_NUM; i++)
    {
        if ((manu_id == flash_query_info_table[i].manu_id) &&
            (device_id == flash_query_info_table[i].device_id))
        {
            flash_info[idx]->query = (FLASH_QUERY_INFO_STRUCT *)&flash_query_info_table[i].query;
            flash_set_flash_exist(idx, FLASH_EXIST_QUERY_INFO);
            return FLASH_RET_SUCCESS;
        }
    }

    return FLASH_RET_NOT_EXIST_QUERY_INFO;
}
FLASH_RETURN_TYPE(*flash_load_query_info)(FLASH_IDX_TYPE idx) = flash_load_query_info_rom;

FLASH_RETURN_TYPE flash_set_qe_bit(FLASH_IDX_TYPE idx, bool enable)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_QUERY_INFO) == 0)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    uint8_t *sr[3] = {NULL, NULL, NULL};
    uint8_t sr_val = 0;
    uint8_t sr_idx = flash_info[idx]->query->qebo / 8;
    uint8_t sr_bo = flash_info[idx]->query->qebo % 8;
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    sr[sr_idx] = &sr_val;
    ret = flash_get_status_regs(idx, sr[0], sr[1], sr[2]);

    if (ret != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    if (enable)
    {
        sr_val |= (1 << sr_bo);
    }
    else
    {
        sr_val &= ~(1 << sr_bo);
    }

    return flash_set_status_regs(idx, sr[0], sr[1], sr[2]);
}

FLASH_RETURN_TYPE flash_set_1_bit_mode(FLASH_IDX_TYPE idx)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_BASIC_CMD) == 0)
    {
        return FLASH_RET_NOT_EXIST_BASIC_CMD;
    }

    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];

    if (flash_info[idx]->basic.cmd_pp)
    {
        FLASH_SET_WR_CMD(idx, basic, cmd_pp, SPIC_SING_CH, SPIC_SING_CH);
        SPIC_REG_FIELD_WR(SPIC, WRITE_SINGLE, WR_CMD, flash_info[idx]->wr_cmd);
    }

    if (flash_info[idx]->basic.cmd_rd_data)
    {
        FLASH_SET_RD_CMD(idx, basic, cmd_rd_data, SPIC_SING_CH, SPIC_SING_CH);
        SPIC_REG_FIELD_WR(SPIC, READ_FAST_SINGLE, FRD_CMD, flash_info[idx]->rd_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, FRD_SINGLE, true);

        return FLASH_RET_SUCCESS;
    }

    return FLASH_RET_SET_BIT_MODE_FAILED;
}

FLASH_RETURN_TYPE flash_set_2_bit_mode(FLASH_IDX_TYPE idx)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_ADV_CMD) == 0)
    {
        return FLASH_RET_NOT_EXIST_ADV_CMD;
    }

    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];

    if (flash_info[idx]->adv.cmd_rd_dual_io)
    {
        FLASH_SET_RD_CMD(idx, adv, cmd_rd_dual_io, SPIC_DUAL_CH, SPIC_DUAL_CH);
        SPIC_REG_FIELD_WR(SPIC, READ_DUAL_ADDR_DATA, RD_DUAL_IO_CMD, flash_info[idx]->rd_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, RD_DUAL_IO, true);

        return FLASH_RET_SUCCESS;
    }

    if (flash_info[idx]->adv.cmd_rd_dual_o)
    {
        FLASH_SET_RD_CMD(idx, adv, cmd_rd_dual_o, SPIC_SING_CH, SPIC_DUAL_CH);
        SPIC_REG_FIELD_WR(SPIC, READ_DUAL_DATA, RD_DUAL_O_CMD, flash_info[idx]->rd_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, RD_DUAL_O, true);

        return FLASH_RET_SUCCESS;
    }

    return FLASH_RET_SET_BIT_MODE_FAILED;
}

FLASH_RETURN_TYPE flash_set_4_bit_mode(FLASH_IDX_TYPE idx)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_ADV_CMD) == 0)
    {
        return FLASH_RET_NOT_EXIST_ADV_CMD;
    }

    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];

    if (flash_info[idx]->adv.cmd_pp_quad_ii)
    {
        FLASH_SET_WR_CMD(idx, adv, cmd_pp_quad_ii, SPIC_QUAD_CH, SPIC_QUAD_CH);
        SPIC_REG_FIELD_WR(SPIC, WRITE_QUAD_ADDR_DATA, WR_QUAD_II_CMD, flash_info[idx]->wr_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, WR_QUAD_II, true);
    }
    else if (flash_info[idx]->adv.cmd_pp_quad_i)
    {
        FLASH_SET_WR_CMD(idx, adv, cmd_pp_quad_i, SPIC_SING_CH, SPIC_QUAD_CH);
        SPIC_REG_FIELD_WR(SPIC, WRITE_QUAD_DATA, WR_QUAD_I_CMD, flash_info[idx]->wr_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, WR_QUAD_I, true);
    }

    if (flash_info[idx]->adv.cmd_rd_quad_io)
    {
        FLASH_SET_RD_CMD(idx, adv, cmd_rd_quad_io, SPIC_QUAD_CH, SPIC_QUAD_CH);
        SPIC_REG_FIELD_WR(SPIC, READ_QUAD_ADDR_DATA, RD_QUAD_IO_CMD, flash_info[idx]->rd_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, RD_QUAD_IO, true);

        return FLASH_RET_SUCCESS;
    }

    if (flash_info[idx]->adv.cmd_rd_quad_o)
    {
        FLASH_SET_RD_CMD(idx, adv, cmd_rd_quad_o, SPIC_SING_CH, SPIC_QUAD_CH);
        SPIC_REG_FIELD_WR(SPIC, READ_QUAD_DATA, RD_QUAD_O_CMD, flash_info[idx]->rd_cmd);
        SPIC_REG_FIELD_WR(SPIC, VALID_CMD, RD_QUAD_O, true);

        return FLASH_RET_SUCCESS;
    }

    return FLASH_RET_SET_BIT_MODE_FAILED;
}

FLASH_RETURN_TYPE flash_set_bit_mode_rom(FLASH_IDX_TYPE idx, FLASH_BIT_MODE mode)
{
    FLASH_RETURN_TYPE ret = FLASH_RET_BIT_MODE_NOT_SUPPORT;

    if (mode == FLASH_1_BIT_MODE)
    {
        ret = flash_set_1_bit_mode(idx);
    }
    else if (mode == FLASH_2_BIT_MODE)
    {
        ret = flash_set_2_bit_mode(idx);
    }
    else if (mode == FLASH_4_BIT_MODE)
    {
        ret = flash_set_4_bit_mode(idx);

        if (ret == FLASH_RET_SUCCESS)
        {
            ret = flash_set_qe_bit(idx, true);
        }
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_set_bit_mode)(FLASH_IDX_TYPE idx, FLASH_BIT_MODE mode) = flash_set_bit_mode_rom;

FLASH_RETURN_TYPE flash_get_bit_mode(FLASH_IDX_TYPE idx, FLASH_BIT_MODE *mode)
{
    SPIC_CFG_CH rd_ch = flash_info[idx]->rd_ch.data;
    SPIC_CFG_CH wr_ch = flash_info[idx]->wr_ch.data;

    if (rd_ch < wr_ch)
    {
        return FLASH_RET_UNKNOWN;
    }

    *mode = (FLASH_BIT_MODE) rd_ch;

    return FLASH_RET_SUCCESS;
}

FLASH_RETURN_TYPE flash_set_auto_mode_rom(FLASH_IDX_TYPE idx, FLASH_BIT_MODE mode)
{
    SPIC_TypeDef *SPIC = flash_idx_spic_map[idx];

    if (spic_lock(SPIC) == false)
    {
        return FLASH_RET_LOCK_FAILED;
    }

    uint8_t addr_len = (flash_get_flash_size(idx) >= (1 << 24)) ? 4 : 3;
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    spic_enable(SPIC, false);

    /* Config settings */
    spic_set_baud(SPIC, flash_setting.spic_baud);
    spic_set_seq_trans(SPIC, true);
    spic_set_auto_dummy_len(SPIC, flash_info[idx]->delay.dummy_len);
    spic_set_auto_addr_len(SPIC, addr_len);
    spic_enable_dum_byte(SPIC, true);

    /* Command settings */
    ret = flash_set_bit_mode(idx, mode);

    if (spic_unlock(SPIC) == false)
    {
        return FLASH_RET_UNLOCK_FAILED;
    }

    spic_set_auto_mode(SPIC);

    return ret;
}
FLASH_RETURN_TYPE(*flash_set_auto_mode)(FLASH_IDX_TYPE idx, FLASH_BIT_MODE mode) = flash_set_auto_mode_rom;

uint32_t flash_get_flash_size(FLASH_IDX_TYPE idx)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_QUERY_INFO) == 0)
    {
        return 0;
    }

    return flash_info[idx]->query->flash_size;
}

FLASH_RETURN_TYPE flash_get_tb_bit(FLASH_IDX_TYPE idx, bool *from_bottom)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_QUERY_INFO) == 0)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    if (flash_info[idx]->query->tbbo == 0xFF)
    {
        *from_bottom = true;
        return FLASH_RET_SUCCESS;
    }

    uint8_t *sr[3] = {NULL, NULL, NULL};
    uint8_t sr_val = 0;
    uint8_t sr_idx = flash_info[idx]->query->tbbo / 8;
    uint8_t sr_bit = 1 << (flash_info[idx]->query->tbbo % 8);
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    sr[sr_idx] = &sr_val;
    ret = flash_get_status_regs(idx, sr[0], sr[1], sr[2]);

    if (ret != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    *from_bottom = ((*sr[sr_idx] & sr_bit) == sr_bit);

    return ret;
}

FLASH_RETURN_TYPE flash_set_tb_bit(FLASH_IDX_TYPE idx, bool from_bottom)
{
    if ((flash_get_flash_exist(idx) & FLASH_EXIST_QUERY_INFO) == 0)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    if (flash_info[idx]->query->tbbo == 0xFF)
    {
        return FLASH_RET_SUCCESS;
    }

    bool from_bottom_cur;
    uint8_t *sr[3] = {NULL, NULL, NULL};
    uint8_t sr_val = 0;
    uint8_t sr_idx = flash_info[idx]->query->tbbo / 8;
    uint8_t sr_bit = 1 << (flash_info[idx]->query->tbbo % 8);
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;

    sr[sr_idx] = &sr_val;
    ret = flash_get_status_regs(idx, sr[0], sr[1], sr[2]);

    if (ret != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    from_bottom_cur = ((sr_val & sr_bit) == sr_bit);

    if (from_bottom == from_bottom_cur)
    {
        return FLASH_RET_SUCCESS;
    }

    sr_val = (from_bottom ? (sr_val | sr_bit) : (sr_val & ~sr_bit));

    return flash_set_status_regs(idx, sr[0], sr[1], sr[2]);
}

FLASH_RETURN_TYPE flash_get_bp_lv_rom(FLASH_IDX_TYPE idx, uint8_t *bp_lv)
{
    if (flash_get_flash_exist(idx) < FLASH_EXIST_QUERY_INFO)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    if (flash_info[idx]->query->tbbo == 0xFF)
    {
        return FLASH_RET_CMD_NOT_SUPPORT;
    }

    uint8_t sr;
    FLASH_RETURN_TYPE ret;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    info.cmd = flash_info[idx]->basic.cmd_rd_sr;
    ret = flash_cmd_rx(idx, &info, &sr, 1);

    if (ret != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    *bp_lv = ((sr >> FLASH_BP_FIELDS_OFFSET) & flash_info[idx]->query->bp_mask);
    return FLASH_RET_SUCCESS;
}
FLASH_RETURN_TYPE(*flash_get_bp_lv)(FLASH_IDX_TYPE idx, uint8_t *bp_lv) = flash_get_bp_lv_rom;

FLASH_RETURN_TYPE flash_get_bp_all_lv(FLASH_IDX_TYPE idx, uint8_t *bp_lv)
{
    if (flash_get_flash_exist(idx) < FLASH_EXIST_QUERY_INFO)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    *bp_lv = flash_info[idx]->query->bp_all;

    return FLASH_RET_SUCCESS;
}

FLASH_RETURN_TYPE flash_set_bp_lv_rom(FLASH_IDX_TYPE idx, uint8_t bp_lv)
{
    if ((flash_get_flash_exist(idx) < FLASH_EXIST_QUERY_INFO) ||
        (bp_lv > flash_info[idx]->query->bp_all))
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    if (flash_info[idx]->query->tbbo == 0xFF)
    {
        return FLASH_RET_CMD_NOT_SUPPORT;
    }

    uint8_t sr;
    FLASH_RETURN_TYPE ret;
    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};

    info.cmd = flash_info[idx]->basic.cmd_rd_sr;
    ret = flash_cmd_rx(idx, &info, &sr, 1);

    if (ret != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    sr &= ~(flash_info[idx]->query->bp_mask << FLASH_BP_FIELDS_OFFSET);
    sr |= ((bp_lv & flash_info[idx]->query->bp_mask) << FLASH_BP_FIELDS_OFFSET);

    info.cmd = flash_info[idx]->basic.cmd_wr_sr;
    FLASH_RETURN_TYPE value;
    value = flash_cmd_tx(idx, &info, &sr, 1);
//    return flash_cmd_tx(idx, &info, &sr, 1);
    return value;
}
FLASH_RETURN_TYPE(*flash_set_bp_lv)(FLASH_IDX_TYPE idx, uint8_t bp_lv) = flash_set_bp_lv_rom;

FLASH_IDX_TYPE flash_get_idx_and_offset(uint32_t *addr)
{
    if ((*addr >= (FMC_MAIN0 | FLASH_OFFSET_TO_NON_CACHE)) &&
        (*addr < (FMC_MAIN0 | FLASH_OFFSET_TO_NON_CACHE + FLASH0_SIZE_MAX)))
    {
        *addr -= (FMC_MAIN0 | FLASH_OFFSET_TO_NON_CACHE);
        return FLASH_IDX_SPIC0;
    }
    else if ((*addr >= FMC_MAIN0) && (*addr < (FMC_MAIN0 + FLASH0_SIZE_MAX)))
    {
        *addr -= FMC_MAIN0;
        return FLASH_IDX_SPIC0;
    }
    else if ((*addr >= FMC_MAIN1) && (*addr < (FMC_MAIN1 + FLASH1_SIZE_MAX)))
    {
        *addr -= FMC_MAIN1;
        return FLASH_IDX_SPIC1;
    }
    else if ((*addr >= FMC_MAIN2) && (*addr < (FMC_MAIN2 + FLASH1_SIZE_MAX)))
    {
        *addr -= FMC_MAIN2;
        return FLASH_IDX_SPIC2;
    }
    else
    {
        return FLASH_IDX_MAX;
    }
}

FLASH_RETURN_TYPE flash_read_rom(uint32_t addr, uint8_t *data, uint32_t byte_len)
{
    if (data == NULL)
    {
        return FLASH_PARAM_INVALID;
    }

    uint32_t read_idx = 0;
    uint32_t read_byte_len;
    uint32_t remain_byte_len = byte_len;
    FLASH_RETURN_TYPE ret;
    FLASH_IDX_TYPE idx = flash_get_idx_and_offset(&addr);

    if (idx == FLASH_IDX_MAX)
    {
        return FLASH_RET_ADDR_OUT_OF_RANGE;
    }

    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};
    info.cmd = flash_info[idx]->rd_cmd;
    info.addr_len = (addr >= (1 << 24)) ? 4 : 3;
    info.cmd_ch = flash_info[idx]->rd_ch.cmd;
    info.addr_ch = flash_info[idx]->rd_ch.addr;
    info.data_ch = flash_info[idx]->rd_ch.data;
    info.dummy_len = flash_info[idx]->delay.dummy_len;

    /* FIFO includes cmd (1 byte) & addr (info.addr_len byte), rest of the
       space is for data */
    read_byte_len = SPIC_RX_FIFO_NUM - (info.addr_len + 1);

    while (remain_byte_len > read_byte_len)
    {
        info.addr = addr + read_idx;

        ret = flash_cmd_rx(idx, &info, &data[read_idx], read_byte_len);

        if (ret != FLASH_RET_SUCCESS)
        {
            return ret;
        }

        read_idx += read_byte_len;
        remain_byte_len -= read_byte_len;
    }

    info.addr = addr + read_idx;
    return flash_cmd_rx(idx, &info, &data[read_idx], remain_byte_len);
}
FLASH_RETURN_TYPE(*flash_read)(uint32_t addr, uint8_t *data, uint32_t byte_len) = flash_read_rom;

FLASH_RETURN_TYPE flash_write_rom(uint32_t addr, uint8_t *data, uint32_t byte_len)
{
    if (data == NULL)
    {
        return FLASH_PARAM_INVALID;
    }

    uint32_t write_idx = 0;
    uint32_t write_byte_len;
    uint32_t remain_byte_len = byte_len;
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;
    FLASH_IDX_TYPE idx = flash_get_idx_and_offset(&addr);

    if (idx == FLASH_IDX_MAX)
    {
        return FLASH_RET_ADDR_OUT_OF_RANGE;
    }

    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};
    info.cmd = flash_info[idx]->wr_cmd;
    info.addr_len = (addr >= (1 << 24))  ? 4 : 3;
    info.cmd_ch = flash_info[idx]->wr_ch.cmd;
    info.addr_ch = flash_info[idx]->wr_ch.addr;
    info.data_ch = flash_info[idx]->wr_ch.data;

    while (remain_byte_len > 0)
    {
        /* FIFO includes cmd (1 byte) & addr (info.addr_len byte), rest of the
           space is for data */
        write_byte_len = SPIC_TX_FIFO_NUM - (info.addr_len + 1);

        info.addr = addr + write_idx;

        if (IS_SAME_FLASH_PAGE(info.addr, info.addr + write_byte_len) == false)
        {
            write_byte_len = write_byte_len - (info.addr + write_byte_len) % FLASH_PAGE_SIZE;
        }

        if (write_byte_len > remain_byte_len)
        {
            write_byte_len = remain_byte_len;
        }

        flash_write_enable(idx, FLASH_WREN_DATA);

        if ((ret = flash_cmd_tx(idx, &info, &data[write_idx], write_byte_len)) != FLASH_RET_SUCCESS)
        {
            break;
        }

        if ((ret = flash_wait_busy(idx)) != FLASH_RET_SUCCESS)
        {
            break;
        }

        write_idx += write_byte_len;
        remain_byte_len -= write_byte_len;
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_write)(uint32_t addr, uint8_t *data, uint32_t byte_len) = flash_write_rom;

FLASH_RETURN_TYPE flash_erase_rom(uint32_t addr, FLASH_ERASE_MODE mode)
{
    uint32_t max_retry = flash_setting.erase_max_retry;
    FLASH_RETURN_TYPE ret = FLASH_RET_SUCCESS;
    FLASH_IDX_TYPE idx = flash_get_idx_and_offset(&addr);

    if (idx == FLASH_IDX_MAX)
    {
        return FLASH_RET_ADDR_OUT_OF_RANGE;
    }

    FLASH_ACCESS_INFO info = {.d32[0] = 0, .d32[1] = 0};
    info.cmd = (mode == FLASH_ERASE_SECTOR) ? flash_info[idx]->basic.cmd_sector_e :
               (mode == FLASH_ERASE_BLOCK) ? flash_info[idx]->basic.cmd_block_e :
               flash_info[idx]->basic.cmd_chip_e;

    if (mode != FLASH_ERASE_CHIP)
    {
        info.addr = addr;
        info.addr_len = (addr >= (1 << 24))  ? 4 : 3;
    }

    flash_write_enable(idx, FLASH_WREN_DATA);

    while (max_retry--)
    {
        if ((ret = flash_cmd_tx(idx, &info, NULL, 0)) == FLASH_RET_SUCCESS)
        {
            ret = flash_wait_busy(idx);
            break;
        }
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_erase)(uint32_t addr, FLASH_ERASE_MODE mode) = flash_erase_rom;

FLASH_RETURN_TYPE flash_unlock_bp_by_addr_rom(uint32_t addr, uint8_t *old_bp_lv)
{
    uint8_t bp_lv;
    uint32_t begin_addr;
    uint32_t end_addr;
    FLASH_RETURN_TYPE ret;
    FLASH_IDX_TYPE idx = flash_get_idx_and_offset(&addr);

    if (idx == FLASH_IDX_MAX)
    {
        return FLASH_RET_ADDR_OUT_OF_RANGE;
    }

    if ((flash_get_flash_exist(idx) & FLASH_EXIST_QUERY_INFO) == 0)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    if ((ret = flash_get_bp_lv(idx, old_bp_lv)) != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    if (*old_bp_lv > 0)
    {
        if (flash_info[idx]->query->tbbo == 0xFF)
        {
            bp_lv = 0;
        }
        else
        {
            bp_lv = *old_bp_lv;

            while (bp_lv)
            {
                begin_addr = flash_bp_range_table[bp_lv];
                end_addr = flash_bp_range_table[bp_lv + 1];

                if ((addr >= begin_addr) && (addr < end_addr))
                {
                    break;
                }

                bp_lv -= 1;
            }

            ret = flash_set_bp_lv(idx, bp_lv);
        }
    }

    return ret;
}
FLASH_RETURN_TYPE(*flash_unlock_bp_by_addr)(uint32_t addr, uint8_t *old_bp_lv) = flash_unlock_bp_by_addr_rom;

FLASH_RETURN_TYPE flash_unlock_access_rom(uint32_t addr, uint8_t *data, uint32_t byte_len, FLASH_ERASE_MODE mode)
{
    FLASH_IDX_TYPE idx = flash_get_idx_and_offset(&addr);

    if (idx == FLASH_IDX_MAX)
    {
        return FLASH_RET_ADDR_OUT_OF_RANGE;
    }

    if ((flash_get_flash_exist(idx) & FLASH_EXIST_QUERY_INFO) == 0)
    {
        return FLASH_RET_NOT_EXIST_QUERY_INFO;
    }

    uint8_t old_bp_lv = 0;
    FLASH_RETURN_TYPE ret;

    if ((ret = flash_unlock_bp_by_addr(addr, &old_bp_lv)) != FLASH_RET_SUCCESS)
    {
        return ret;
    }

    if ((mode == FLASH_ERASE_SECTOR) || (mode == FLASH_ERASE_BLOCK) || (mode == FLASH_ERASE_CHIP))
    {
        if ((ret = flash_erase(addr, mode)) != FLASH_RET_SUCCESS)
        {
            return ret;
        }
    }
    else
    {
        if ((ret = flash_write(addr, data, byte_len)) != FLASH_RET_SUCCESS)
        {
            return ret;
        }
    }

    return flash_set_bp_lv(idx, old_bp_lv);
}
FLASH_RETURN_TYPE(*flash_unlock_access)(uint32_t addr, uint8_t *data, uint32_t byte_len, FLASH_ERASE_MODE mode) = flash_unlock_access_rom;
