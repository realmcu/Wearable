#include "indirect_access.h"
#include "platform_rtc.h"
#include "rtl876x.h"
#include "rtl876x_rtc.h"

uint16_t indirect_access_read_32k_reg(uint16_t offset)
{
    uint16_t read_data;

    PON_F00_REG_S_TYPE pon_f00_orig;
    pon_f00_orig.d32 = HAL_READ32(BT_VENDOR_REG_BASE, PON_F00_REG);
    pon_f00_orig.ind_32k_rw = 0;

    PON_F00_REG_S_TYPE pon_f00;

    pon_f00.d32 = HAL_READ32(BT_VENDOR_REG_BASE, PON_F00_REG);
    pon_f00.ind_32k_rw = 0;
    pon_f00.ind_32k_addr = offset;
    HAL_WRITE32(BT_VENDOR_REG_BASE, PON_F00_REG, pon_f00.d32);

    pon_f00.d32 = HAL_READ32(BT_VENDOR_REG_BASE, PON_F00_REG);
    read_data = (uint16_t) pon_f00.ind_32k_wdata;

    HAL_WRITE32(BT_VENDOR_REG_BASE, PON_F00_REG, pon_f00_orig.d32);

    return read_data;
}

void indirect_access_write_32k_reg(uint16_t offset, uint16_t wdata)
{
    PON_F00_REG_S_TYPE pon_f00_orig;
    pon_f00_orig.d32 = HAL_READ32(BT_VENDOR_REG_BASE, PON_F00_REG);
    pon_f00_orig.ind_32k_rw = 0;

    PON_F00_REG_S_TYPE pon_f00;

    pon_f00.d32 = HAL_READ32(BT_VENDOR_REG_BASE, PON_F00_REG);
    pon_f00.ind_32k_rw = 0;
    pon_f00.ind_32k_addr = offset;
    pon_f00.ind_32k_wdata = wdata;
    HAL_WRITE32(BT_VENDOR_REG_BASE, PON_F00_REG, pon_f00.d32);

    pon_f00.ind_32k_rw = 1;
    HAL_WRITE32(BT_VENDOR_REG_BASE, PON_F00_REG, pon_f00.d32);

    HAL_WRITE32(BT_VENDOR_REG_BASE, PON_F00_REG, pon_f00_orig.d32);
}



uint16_t btaon_fast_read(uint16_t offset)
{
    uint16_t read_data;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    read_data = (uint16_t) aon_fw_onreg.b.FW_ONREG_DATA;
    return read_data;
}
uint8_t btaon_fast_read_8b(uint16_t offset)
{
    uint16_t read_data;
    uint8_t out_data;
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset & 0xFFFE;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    read_data = (uint16_t) aon_fw_onreg.b.FW_ONREG_DATA;
    if (offset & 0x01)
    {
        out_data = (uint8_t)((read_data >> 8) & 0xFF);
    }
    else
    {
        out_data = (uint8_t)(read_data & 0xFF);
    }
    return out_data;
}

uint16_t btaon_fast_read_safe(uint16_t offset)
{
    /* Store register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg_orig;
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    uint16_t read_data;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    read_data = (uint16_t) aon_fw_onreg.b.FW_ONREG_DATA;

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);

    return read_data;
}
uint8_t btaon_fast_read_safe_8b(uint16_t offset)
{
    /* Store register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg_orig;
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    uint16_t read_data;
    uint8_t out_data;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset & 0xFFFE;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    read_data = (uint16_t) aon_fw_onreg.b.FW_ONREG_DATA;

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);

    if (offset & 0x01)
    {
        out_data = (uint8_t)((read_data >> 8) & 0xFF);
    }
    else
    {
        out_data = (uint8_t)(read_data & 0xFF);
    }
    return out_data;

}

void btaon_fast_write(uint16_t offset, uint16_t data)
{
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_DATA = data;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;

    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
}
void btaon_fast_write_8b(uint16_t offset, uint8_t write_data)
{
    uint16_t read_val, data;
    read_val = btaon_fast_read(offset & 0xFFFE);
    if (offset & 0x01)
    {
        data = read_val & 0x00FF;
        data |= ((uint16_t)write_data << 8);
    }
    else
    {
        data = read_val & 0xFF00;
        data |= ((uint16_t)write_data);
    }
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset & 0xFFFE;
    aon_fw_onreg.b.FW_ONREG_DATA = data;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;

    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
}

void btaon_fast_write_safe(uint16_t offset, uint16_t data)
{
    /* Store register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg_orig;
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;

    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_DATA = data;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;

    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);
}
void btaon_fast_write_safe_8b(uint16_t offset, uint8_t write_data)
{
    uint16_t read_val, data;
    read_val = btaon_fast_read_safe(offset & 0xFFFE);
    if (offset & 0x01)
    {
        data = read_val & 0x00FF;
        data |= ((uint16_t)write_data << 8);
    }
    else
    {
        data = read_val & 0xFF00;
        data |= ((uint16_t)write_data);
    }
    /* Store register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg_orig;
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSON_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;

    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset & 0xFFFE;
    aon_fw_onreg.b.FW_ONREG_DATA = data;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;

    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSON_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);
}

void btaon_fast_update(uint16_t offset, uint16_t mask, uint16_t data)
{
    uint16_t temp;
    temp = btaon_fast_read(offset) & ~mask;
    btaon_fast_write(offset, (temp | (data & mask)));
}
void btaon_fast_update_8b(uint16_t offset, uint8_t mask, uint8_t data)
{
    uint8_t temp;
    temp = btaon_fast_read_8b(offset) & ~mask;
    btaon_fast_write_8b(offset, (temp | (data & mask)));
}

void btaon_fast_update_safe(uint16_t offset, uint16_t mask, uint16_t data)
{
    uint16_t temp;
    temp = btaon_fast_read_safe(offset) & ~mask;
    btaon_fast_write_safe(offset, (temp | (data & mask)));
}
void btaon_fast_update_safe_8b(uint16_t offset, uint8_t mask, uint8_t data)
{
    uint8_t temp;
    temp = btaon_fast_read_safe_8b(offset) & ~mask;
    btaon_fast_write_safe_8b(offset, (temp | (data & mask)));
}

void aon_indirect_write_reg_safe(uint32_t offset, uint32_t data)
{
    /* Store register value of RTC_FAST_WDATA and RTC_FAST_ADDR */
    uint32_t rtc_fast_wdata_ori = HAL_READ32(RTC_REG_BASE, RTC_FAST_WDATA);
    uint32_t rtc_fast_addr_ori = HAL_READ32(RTC_REG_BASE, RTC_FAST_ADDR);

    /* Write data */
    HAL_WRITE32(RTC_REG_BASE, RTC_FAST_WDATA, data);
    /* Write RTC register address. Only offset */
    HAL_WRITE32(RTC_REG_BASE, RTC_FAST_ADDR, offset);
    HAL_WRITE32(RTC_REG_BASE, RTC_WR_STROBE, 1);

    /* Restore register value of REG_RTC_FAST_WDATA and REG_RTC_FAST_ADDR */
    HAL_WRITE32(RTC_REG_BASE, RTC_FAST_WDATA, rtc_fast_wdata_ori);
    HAL_WRITE32(RTC_REG_BASE, RTC_FAST_ADDR, rtc_fast_addr_ori);
}

uint32_t aon_indirect_read_reg_safe(uint32_t offset)
{
    return HAL_READ32(RTC_REG_BASE, offset);
}



