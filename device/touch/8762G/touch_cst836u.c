/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-08-04     tyustli  first version
 */

/*
 * Copyright (c) 2006-2020, Watch Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-08-04     howie  first version
 */


#include "rtl_gpio.h"
#include "rtl_rcc.h"
#include "drv_gpio.h"
#include "drv_i2c.h"
#include "drv_touch.h"
#include "touch_cst836u.h"
#include "string.h"
#include "trace.h"
#include "utils.h"
#include "board.h"


void (*touch_wakeup_indicate)(void) = NULL;

#define X_LEN      (360)
#define Y_LEN      640

#include "trace.h"
bool rtk_touch_hal_read_all(uint16_t *x, uint16_t *y, bool *pressing)
{
    uint8_t iic_write_buf[1] = {0x03};
    uint8_t buf[4] = {0};

    drv_i2c0_write(TOUCH_CST836U_ADDR, iic_write_buf, 1);
    platform_delay_us(100);

    drv_i2c0_read(TOUCH_CST836U_ADDR, buf, 4);

    if ((buf[0] & (3 << 6)) >> 6 == 0x02)
    {
        *pressing = true;
    }
    else
    {
        *pressing = false;
    }

    uint16_t x_raw = (((buf[0] & 0x0f) << 8) | buf[1]);
    uint16_t y_raw = (((buf[2] & 0x0f) << 8) | buf[3]);


    *x = y_raw;
    *y = X_LEN - x_raw;

    // *x = x_raw;
    // *y = y_raw;

    if (*pressing)
    {
        DBG_DIRECT(">raw: x %d y %d\n", x_raw, y_raw);
        // DBG_DIRECT("raw 360: x %d y %d\n", y_raw, 360 - x_raw);
        DBG_DIRECT("tp: x %d y %d\n", *x, *y);
    }

    return true;
}


void rtk_touch_hal_set_indicate(void (*indicate)(void *))
{
    drv_pin_mode(TOUCH_CST836U_INT, PIN_MODE_INPUT);
    drv_pin_attach_irq(TOUCH_CST836U_INT, PIN_IRQ_MODE_RISING_FALLING, indicate,
                       NULL);
    drv_pin_irq_enable(TOUCH_CST836U_INT, PIN_IRQ_DISABLE);
}

void rtk_touch_hal_int_config(bool enable)
{
    if (enable == true)
    {
        drv_pin_irq_enable(TOUCH_CST836U_INT, PIN_IRQ_ENABLE);
    }
    else
    {
        drv_pin_irq_enable(TOUCH_CST836U_INT, PIN_IRQ_DISABLE);
    }
}


static bool touch_enter_dlps(void)
{
    Pad_Config(TOUCH_CST836U_INT, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    System_WakeUpPinEnable(TOUCH_CST836U_INT, PAD_WAKEUP_POL_LOW, PAD_WAKEUP_DEB_DISABLE);
    return false;
}

static bool touch_exit_dlps(void)
{
    Pad_Config(TOUCH_CST836U_INT, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(TOUCH_CST836U_SCL, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    Pad_Config(TOUCH_CST836U_SDA, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);
    return false;
}

static bool touch_allowed_enter_dlps_check(void)
{
    return true;
}

static bool touch_system_wakeup_dlps_check(void)
{
    if (System_WakeUpInterruptValue(TOUCH_CST836U_INT) == SET)
    {
        Pad_ClearWakeupINTPendingBit(TOUCH_CST836U_INT);
        System_WakeUpPinDisable(TOUCH_CST836U_INT);
        DBG_DIRECT("Touch Wake up");
        if (touch_wakeup_indicate != NULL)
        {
            touch_wakeup_indicate();
        }
        return true;
    }
    return false;
}

void drv_touch_dlps_init(void)
{
    System_WakeUpPinEnable(TOUCH_CST836U_INT, PAD_WAKEUP_POL_LOW, PAD_WAKEUP_DEB_DISABLE);
//#ifdef RTK_HAL_DLPS
    drv_dlps_exit_cbacks_register("touch", touch_exit_dlps);
    drv_dlps_enter_cbacks_register("touch", touch_enter_dlps);
    drv_dlps_wakeup_cbacks_register("touch", touch_system_wakeup_dlps_check);
    drv_dlps_check_cbacks_register("touch", touch_allowed_enter_dlps_check);
//#endif
}

void drv_touch_set_wakeup_indicate(void (*wakeup_ind))
{
    touch_wakeup_indicate = wakeup_ind;
}

void rtk_touch_hal_init(void)
{
    drv_i2c0_init(TOUCH_CST836U_SCL, TOUCH_CST836U_SDA);
    drv_pin_mode(TOUCH_CST836U_RST, PIN_MODE_OUTPUT);
    drv_pin_write(TOUCH_CST836U_RST, 0);
    platform_delay_ms(10);
    drv_pin_write(TOUCH_CST836U_RST, 1);
    platform_delay_ms(120);

    uint8_t iic_write_buf[1] = {TOUCH_CST836U_CHIP_ID};
    uint8_t iic_read_buf[1] = {0x33};

    drv_i2c0_write(TOUCH_CST836U_ADDR, iic_write_buf, 1);
    platform_delay_us(100);

    drv_i2c0_read(TOUCH_CST836U_ADDR, iic_read_buf, 1);

    DBG_DIRECT("CTP ID:0x%x\r\n", iic_read_buf[0]);

    drv_touch_dlps_init();
}


#if 0
//#include "wristband_sdk_config.h"

#if 1
#include "rtl_gpio.h"
#include "rtl_rcc.h"
#include "drv_gpio.h"
#include "drv_i2c.h"
#include "drv_touch.h"
#include "drv_lcd.h"
#include "touch_gt911.h"
#include "string.h"
#include "trace.h"
#include "utils.h"

const uint8_t GT911_CFG_TBL[] =
{
    0x62, 0xE0, 0x01, 0xE0, 0x01, 0x03, 0x35, 0x00,
    0x01, 0x18, 0x1E, 0x0F, 0x60, 0x3C, 0x03, 0x0E,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
    0x1A, 0x1D, 0x14, 0x87, 0x07, 0x0E, 0x52, 0x56,
    0xB2, 0x04, 0x00, 0x00, 0x00, 0xBA, 0x02, 0x1C,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x3C, 0x69, 0x94, 0xC5, 0x02,
    0x07, 0x00, 0x00, 0x04, 0xA3, 0x3F, 0x00, 0x92,
    0x47, 0x00, 0x84, 0x4F, 0x00, 0x77, 0x58, 0x00,
    0x6B, 0x63, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x00,
    0xF0, 0x4A, 0x3A, 0xFF, 0xFF, 0x27, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1C, 0x1A, 0x18, 0x16, 0x14, 0x12, 0x10, 0x0E,
    0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x02,
    0x04, 0x06, 0x08, 0x0A, 0x0C, 0x24, 0x22, 0x21,
    0x20, 0x1F, 0x1E, 0x1D, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

uint8_t GT911_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t send[198];
    send[0] = reg >> 8;
    send[1] = reg & 0xff;
    if (len > 200)
    {
        return 0;
    }
    else
    {
        memcpy(send + 2, buf, len);
        drv_i2c0_write(TOUCH_GT911_ADDR, send, 2 + len);
    }

    return 0;
}

uint32_t GT911_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t iic_write_buf[2] = {0};
    iic_write_buf[0] = reg >> 8;
    iic_write_buf[1] = reg & 0xff;

    drv_i2c0_write(TOUCH_GT911_ADDR, iic_write_buf, 2);

    return drv_i2c0_read(TOUCH_GT911_ADDR, buf, len);
}

uint8_t GT911_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i = 0;
    buf[0] = 0;
    buf[1] = mode;
    for (i = 0; i < sizeof(GT911_CFG_TBL); i++)
    {
        buf[0] += GT911_CFG_TBL[i];
    }
    buf[0] = (~buf[0]) + 1;
    DBG_DIRECT("checksum by MCU:%x", buf[0]);


    GT911_WR_Reg(GT_CFGS_REG, (uint8_t *)GT911_CFG_TBL, sizeof(GT911_CFG_TBL));

    GT911_WR_Reg(GT_CHECK_REG, buf, 2);
    GT911_WR_Reg(0x8100, &mode, 1);
    return 0;
}

uint32_t GT911_read(uint8_t *buf, uint32_t len)
{
    uint8_t *read = buf;
    uint8_t temp[10] = {0};

    uint8_t mode = 0;
    uint8_t point_num = 0;
    uint8_t reset = 0;
    GT911_RD_Reg(GT_GSTID_REG, &mode, 1);
    //DBG_DIRECT("mode:%x",mode);
    read[0] = mode;
    if (mode & 0x80)
    {
        GT911_WR_Reg(GT_GSTID_REG, &reset, 1);// clear flags
    }
    else
    {
        //DBG_DIRECT("GT911_read");
        return 0;
    }
    point_num = mode & 0x0F;
    if (point_num == 0)
    {
        //DBG_DIRECT("GT911_read point_num");
        return 0;
    }

    len = GT911_RD_Reg(0x814F, read + 1, 7);

    GT911_WR_Reg(GT_GSTID_REG, &reset, 1);
//DBG_DIRECT("len:%d",len);
    return len;
}

bool rtk_touch_hal_read_all(uint16_t *x, uint16_t *y, bool *pressing)
{
    *pressing = false;
    uint8_t buf[10] = {0};
    *x = 480;
    *y = 480;
    static uint16_t x_old = 0;
    static uint16_t y_old = 0;
    if (GT911_read(buf, 0))
    {
        *pressing = true;
        *x = (buf[2] | (buf[3] << 8));
        *y = (buf[4] | (buf[5] << 8));
        x_old = *x;
        y_old = *y;
        return true;
    }
    else
    {
        *x = x_old;
        *y = y_old;
        return true;
    }

}
void rtk_touch_hal_set_indicate(void (*indicate)(void *))
{
    (void)indicate;
    return;
}
void rtk_touch_hal_int_config(bool enable)
{
    (void)enable;
    return;
}
bool rtk_touch_hal_power_off(void)
{
    return true;
}

bool rtk_touch_hal_power_on(void)
{
    return true;
}

bool rtk_touch_hal_dlps_check(void)
{
    return true;
}

bool rtk_touch_wake_up(void)
{
    return false;
}

void rtk_touch_dlps_init(void)
{
}

void rtk_touch_hal_init(void)
{
    DBG_DIRECT("touch_gt911_init line = %d\n", __LINE__);
    drv_i2c0_init(TOUCH_GT911_SCL, TOUCH_GT911_SDA);
    drv_pin_mode(TOUCH_GT911_INT, PIN_MODE_OUTPUT);
    drv_pin_write(TOUCH_GT911_INT, 0);
    drv_pin_mode(TOUCH_GT911_RST, PIN_MODE_OUTPUT);
    drv_pin_write(TOUCH_GT911_RST, 0);
    platform_delay_ms(10);
    drv_pin_write(TOUCH_GT911_INT, 1);
    platform_delay_ms(10);
    drv_pin_write(TOUCH_GT911_RST, 1);
    platform_delay_ms(30);
    drv_pin_mode(TOUCH_GT911_INT, PIN_MODE_INPUT);

    uint8_t iic_write_buf[2] = {0x81, 0x40};
    uint8_t iic_read_buf[5] = {0};
    platform_delay_ms(255);

    drv_i2c0_write(TOUCH_GT911_ADDR, iic_write_buf, 2);

    drv_i2c0_read(TOUCH_GT911_ADDR, iic_read_buf, 4);

    DBG_DIRECT("CTP ID:%s\r\n", iic_read_buf);

    uint8_t cmd_init[1] = {2};
    GT911_WR_Reg(GT_CTRL_REG, (uint8_t *)cmd_init, 1);
    GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)cmd_init, 1);

    DBG_DIRECT("[GT_CFGS_REG]:%x\r\n", cmd_init[0]);
    if (cmd_init[0] == 0x60)
    {
        GT911_Send_Cfg(1);
    }
    GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)cmd_init, 1);
    DBG_DIRECT("[GT_CFGS_REG]:%x\r\n", cmd_init[0]);

    uint8_t touch_read[200] = {0};

    GT911_RD_Reg(GT_CFGS_REG, (uint8_t *)touch_read, 186);

    APP_PRINT_INFO1("data = %b", TRACE_BINARY(186, touch_read));

    platform_delay_us(200);
    cmd_init[0] = 0;
    GT911_WR_Reg(GT_CTRL_REG, (uint8_t *)cmd_init, 1);

    uint8_t x_low = 0;
    uint8_t x_high = 0;
    uint8_t y_low = 0;
    uint8_t y_high = 0;

    GT911_RD_Reg(0x8146, &x_low, 1);
    GT911_RD_Reg(0x8147, &x_high, 1);
    GT911_RD_Reg(0x8148, &y_low, 1);
    GT911_RD_Reg(0x8149, &y_high, 1);
    uint16_t x = x_low + (x_high << 8);
    uint16_t y = y_low + (y_high << 8);
    DBG_DIRECT("x = %d\r\n", x);
    DBG_DIRECT("y = %d\r\n", y);
}
#endif

#endif /* BSP_USING_TOUCH */
