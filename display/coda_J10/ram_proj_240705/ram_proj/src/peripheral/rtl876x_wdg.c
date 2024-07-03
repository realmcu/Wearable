/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_wdg.c
* @brief    This file provides all the Watch Dog firmware functions.
* @details
* @author   Lory_xu
* @date     2016-06-12
* @version  v0.1
*********************************************************************************************************
*/
#include "rtl876x_wdg.h"
#include "rtl876x_rcc.h"
#include "trace.h"
#include "utils.h"

void WDG_ClockEnable(void)
{
    RCC_PeriphClockCmd(APBPeriph_VENDOR_REG, APBPeriph_VENDOR_REG_CLOCK, ENABLE);
    SYSBLKCTRL->u_360.BITS_360.BIT_TIMER_CLK_32K_EN = 1;
}

/**
divfactor: 16Bit: 32.768k/(1+divfactor)
cnt_limit: 2^(cnt_limit+1) - 1 ; max 11~15 = 0xFFF
            0: 0x001
            1: 0x003
            2: 0x007
            3: 0x00F
            4: 0x01F
            5: 0x03F
            6: 0x07F
            7: 0x0FF
            8: 0x1FF
            9: 0x3FF
            10: 0x7FF
            11~15: 0xFFF
Wdg_mode: 0: interrupt cpu
          1: reset all except AON
          2: reset core domain
          3: reset all
*/
#define WDG_MAX_TIMEROUT_MS 8192000
bool WDG_Calculate_Config(uint32_t timeout_ms, uint16_t *div_factor, uint8_t *cnt_limit)
{
    if (timeout_ms > WDG_MAX_TIMEROUT_MS)
    {
        return false;
    }

    if (timeout_ms == 0)
    {
        *cnt_limit = 0;
        *div_factor = 1;
    }
    else if (timeout_ms < 1000)
    {
        *cnt_limit = 0;
        *div_factor = 32 * timeout_ms - 1;
    }
    else
    {
        *cnt_limit = 15;
        *div_factor = timeout_ms / 128 - 1;
    }
    return true;
}

void WDG_Config(uint16_t div_factor, uint8_t  cnt_limit, T_WDG_MODE  wdg_mode)
{
    T_WATCH_DOG_TIMER_REG wdg_ctrl_value;

    if (div_factor == 0)
    {
        div_factor = 1;
    }

    wdg_ctrl_value.d32 = WDG->WDG_CTL;

    wdg_ctrl_value.b.div_factor = div_factor;
    wdg_ctrl_value.b.cnt_limit = cnt_limit;
    wdg_ctrl_value.b.wdg_mode = wdg_mode;

    WDG->WDG_CTL = wdg_ctrl_value.d32;
}

void WDG_Set_Core(T_WDG_TYPE type)
{
    T_WATCH_DOG_TIMER_REG wdg_ctrl_value;

    wdg_ctrl_value.d32 = WDG->WDG_CTL;

    if (type != WDG_TYPE_RESTART)
    {
        wdg_ctrl_value.b.en_byte = 0xA5 * type;
        wdg_ctrl_value.b.timeout = 1; // W1C;
    }

    wdg_ctrl_value.b.clear = 1;


    WDG->WDG_CTL = wdg_ctrl_value.d32;
}

void WDG_Disable_Core()
{
    WDG_Set_Core(WDG_TYPE_DISABLE);
}

void WDG_Kick_Core()
{
    WDG_Set_Core(WDG_TYPE_RESTART);
}

VOID_PATCH_FUNC wdg_disable = (VOID_PATCH_FUNC)WDG_Disable_Core;
VOID_PATCH_FUNC wdg_kick = (VOID_PATCH_FUNC)WDG_Kick_Core;

bool WDG_Start_Core(uint32_t ms, T_WDG_MODE  wdg_mode)
{
    uint16_t div_factor;
    uint8_t cnt_limit;
    if (WDG_Calculate_Config(ms, &div_factor, &cnt_limit))
    {
        WDG_ClockEnable();
        WDG_Config(div_factor, cnt_limit, wdg_mode);
        WDG_Enable();
        return true;
    }
    return false;
}

void WDG_SystemReset_Dump(T_WDG_MODE wdg_mode, uint32_t dump_size)
{
    __disable_irq();

    // uint32_t sp = __current_sp();
    // uint32_t lr = __return_address();

    // DIRECT_LOG("wdt reset(%d), sb = %d, id = 0x%x, bk = %d, lr = 0x%x, sp = 0x%x", 6, wdg_mode,
    //            boot_cfg.flash_layout.secure_boot_failed_line, secure_boot_image_id, ota_bank_num, lr, sp);

    // dump_raw_mem((uint32_t *)(sp & ~(31)), dump_size);
}

bool (*wdg_start)(uint32_t ms, T_WDG_MODE  wdg_mode) = WDG_Start_Core;
void (*chip_reset)(T_WDG_MODE wdg_mode) = WDG_SystemReset;
void WDG_SystemReset(T_WDG_MODE wdg_mode)
{
    WDG_SystemReset_Dump(wdg_mode, 512);

    WDG_Start_Core(0, wdg_mode);

    while (1); /* wait until reset */
}

