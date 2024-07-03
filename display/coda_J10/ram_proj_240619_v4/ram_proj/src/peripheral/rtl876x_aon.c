#include "rtl876x.h"
#include "rtl876x_aon.h"
#include "bblite_platform.h"
/* Define AON register structure in source file while it is not published */
#define REG_AON_FW_ONREG    0x48
typedef union REG_AON_FW_ONREG_S
{
    uint32_t d32;
    struct
    {
        uint32_t FW_ONREG_WSTROBE: 1; // [0]     RW FW set "0" to "1" to strobe write data
        uint32_t FW_ONREG_DATA  : 16; // [16:1]   RW
        //uint32_t FW_ONREG_RDATA  : 8; // [16:9]  RW
        uint32_t FW_ONREG_ADR    : 13; // [29:17] RW
        uint32_t RVD0             : 1; // [30] RW
        uint32_t ECC_TO          : 1; // [31] RW
    } b;
} REG_AON_FW_ONREG_S_TYPE;


uint32_t SysCpuClkUsFactor = (CPU_CLOCK / 1000000);

__asm volatile void platform_delay_us(uint32_t us)
{
    extern SysCpuClkUsFactor;
    ldr     r1, = SysCpuClkUsFactor
                  ldr     r1, [r1]                /* r0 = us, r1 = SysCpuClkUsFactor */
                  muls    r1, r0, r1              /* r1 = r0 * r1, r1 means "cpu cycles for input us" */
                  movs    r0, #3                  /* r0 = 3, (subs + bcs = one loop) needs at least 3 cycles */
                  udiv    r0, r1, r0              /* r0 = r1 / r0 = (cpu cycles for input us) / 3 = loop count */
                  cmp     r0, #1                  /* Compare r0 with 1 => if loop count == 0, exit */
                  bls     __exit_us               /* Branch if Lower or the Same */
                  subs    r0, r0, #2              /* n -= 2, it means 2 * 3 = 6 instructions */

                  __repeat_us
                  subs    r0, r0, #1              /* n --; */
                  bcs     __repeat_us             /* Branch if Carry Set */

                  __exit_us
                  bx      lr                      /* function return */
                  ALIGN
}

uint16_t btaon_fast_read(uint16_t offset)
{
    uint16_t read_data;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
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
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
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
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    uint16_t read_data;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
    read_data = (uint16_t) aon_fw_onreg.b.FW_ONREG_DATA;

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);

    return read_data;
}
uint8_t btaon_fast_read_safe_8b(uint16_t offset)
{
    /* Store register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg_orig;
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    uint16_t read_data;
    uint8_t out_data;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;
    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset & 0xFFFE;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    aon_fw_onreg.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
    read_data = (uint16_t) aon_fw_onreg.b.FW_ONREG_DATA;

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);

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

    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
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

    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
}

void btaon_fast_write_safe(uint16_t offset, uint16_t data)
{
    /* Store register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg_orig;
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;

    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset;
    aon_fw_onreg.b.FW_ONREG_DATA = data;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;

    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);
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
    aon_fw_onreg_orig.d32 = HAL_READ32(SYSTEM_REG_BASE, REG_AON_FW_ONREG);
    aon_fw_onreg_orig.b.FW_ONREG_WSTROBE = 0;

    REG_AON_FW_ONREG_S_TYPE aon_fw_onreg;

    aon_fw_onreg.d32 = 0;
    aon_fw_onreg.b.FW_ONREG_ADR = offset & 0xFFFE;
    aon_fw_onreg.b.FW_ONREG_DATA = data;
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;

    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 1;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);
    aon_fw_onreg.b.FW_ONREG_WSTROBE = 0;
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg.d32);

    /* Restore register value of REG_AON_FW_ONREG except FW_ONREG_WSTROBE */
    HAL_WRITE32(SYSTEM_REG_BASE, REG_AON_FW_ONREG, aon_fw_onreg_orig.d32);
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


