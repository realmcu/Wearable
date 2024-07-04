/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file         platform_rtc.c
* @brief        Platfrom RTC Functions implementation.
* @details
* @author       Po Yu Chen
* @date         2020-12-15
* @version      v0.1
*********************************************************************************************************
*/

#include "platform_rtc.h"
#include "indirect_access.h"

void platform_rtc_run_cmd(FunctionalState new_state)
{
    PF_RTC_CTRL_TYPE pf_rtc_ctrl = {.u32 = aon_indirect_read_reg_safe(PF_RTC_CTRL_REG)};
    pf_rtc_ctrl.cnt_start = new_state;
    aon_indirect_write_reg_safe(PF_RTC_CTRL_REG, pf_rtc_ctrl.u32);
}

void platform_rtc_system_wakeup_config(FunctionalState new_state)
{
    PF_RTC_CTRL_TYPE pf_rtc_ctrl = {.u32 = aon_indirect_read_reg_safe(PF_RTC_CTRL_REG)};
    pf_rtc_ctrl.wk_ie = new_state;
    aon_indirect_write_reg_safe(PF_RTC_CTRL_REG, pf_rtc_ctrl.u32);
}

void platform_rtc_nmi_enable(FunctionalState new_state)
{
    PF_RTC_CTRL_TYPE pf_rtc_ctrl = {.u32 = aon_indirect_read_reg_safe(PF_RTC_CTRL_REG)};
    pf_rtc_ctrl.nv_ie = new_state;
    aon_indirect_write_reg_safe(PF_RTC_CTRL_REG, pf_rtc_ctrl.u32);
}

void platform_rtc_comp_config(uint32_t intr_bitmap, uint32_t wkup_bitmap, FunctionalState NewState)
{
    PF_RTC_CTRL_TYPE pf_rtc_ctrl = {.u32 = aon_indirect_read_reg_safe(PF_RTC_CTRL_REG)};
    if (NewState == ENABLE)
    {
        /* Enable the selected RTC comparator interrupt */
        pf_rtc_ctrl.cmp_nv_ie |= intr_bitmap;
        pf_rtc_ctrl.cmp_wk_ie |= wkup_bitmap;
    }
    else
    {
        /* Disable the selected RTC comparator interrupt */
        pf_rtc_ctrl.cmp_nv_ie &= ~intr_bitmap;
        pf_rtc_ctrl.cmp_wk_ie &= ~wkup_bitmap;
    }
    aon_indirect_write_reg_safe(PF_RTC_CTRL_REG, pf_rtc_ctrl.u32);
}

void platform_rtc_set_comp(uint8_t index, uint32_t value)
{
    aon_indirect_write_reg_safe(PF_RTC_COMP0_REG + index * 4, value);
}

void platform_rtc_initiate_interrupt(uint32_t initiate_bitmap)
{
    PF_RTC_BACKUP_TYPE pf_rtc_backup = {.u32 = aon_indirect_read_reg_safe(PF_RTC_BACKUP_REG)};
    pf_rtc_backup.intr |= initiate_bitmap;
    aon_indirect_write_reg_safe(PF_RTC_BACKUP_REG, pf_rtc_backup.u32);
}

uint32_t platform_rtc_get_counter(void)
{
    return aon_indirect_read_reg_safe(PF_RTC_CNT_REG);
}

uint8_t platform_rtc_get_interrupt_status(void)
{
    PF_RTC_BACKUP_TYPE pf_rtc_backup = {.u32 = aon_indirect_read_reg_safe(PF_RTC_BACKUP_REG)};
    PF_RTC_INT_SR_TYPE pf_rtc_int_sr = {.u32 = aon_indirect_read_reg_safe(PF_RTC_INT_SR_REG)};
    return (pf_rtc_int_sr.cmp_nv_sr | pf_rtc_backup.intr);
}

void platform_rtc_clear_status(uint32_t initiate_bitmap, uint32_t intr_bitmap, uint32_t wkup_bitmap)
{
    PF_RTC_BACKUP_TYPE pf_rtc_backup = {.u32 = aon_indirect_read_reg_safe(PF_RTC_BACKUP_REG)};
    pf_rtc_backup.intr &= ~initiate_bitmap;
    aon_indirect_write_reg_safe(PF_RTC_BACKUP_REG, pf_rtc_backup.u32);

    PF_RTC_INT_CLR_TYPE pf_rtc_int_clr = {0};
    pf_rtc_int_clr.cmp_nv_clr = intr_bitmap;
    pf_rtc_int_clr.cmp_wk_clr = wkup_bitmap;
    aon_indirect_write_reg_safe(PF_RTC_INT_CLR_REG, pf_rtc_int_clr.u32);
    __NOP();
    __NOP();
    aon_indirect_write_reg_safe(PF_RTC_INT_CLR_REG, 0);
}

