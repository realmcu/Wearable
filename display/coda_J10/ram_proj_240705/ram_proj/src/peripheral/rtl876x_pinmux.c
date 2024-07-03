/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_pinmux.c
* @brief    This file provides all the PINMUX firmware functions.
* @details
* @author   renee
* @date     2024-05-15
* @version  v0.2
*********************************************************************************************************
*/

#include "rtl876x.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_aon_reg.h"
#include "indirect_access.h"
#include "pin_def.h"

#define PAD_WK_STS_DATA_AON_REG_7EC            (0xFFFF)
#define PAD_WK_STS_DATA_AON_REG_7EE            (0xFFFF)
#define PAD_WK_STS_DATA_AON_REG_7F0            (0xFFFF)
#define PAD_WK_STS_DATA_AON_REG_7F2            (0xFFFF)
#define PAD_WK_STS_DATA_AON_REG_7F4            (0xFFFF)
#define PAD_WK_STS_DATA_AON_REG_7F6            (0xFFFF)
#define PAD_WK_STS_DATA_AON_REG_7F8            (0xFFFF)

typedef struct _T_PAD_FUNCTION
{
    uint8_t  pin_index;
    uint8_t  reg_bit;
    uint16_t reg_offset;
} T_PAD_FUNCTION;

typedef struct _T_PINADDR_TABLE
{
    uint16_t reg[TOTAL_PIN_NUM];
} T_PIN_ADDR_TABLE;

typedef struct _T_PINMUX_INDEX_TABLE
{
    uint8_t index[TOTAL_PIN_NUM];
} T_PINMUX_INDEX_TABLE;

typedef struct T_PAD_ANALOG_ITEM
{
    uint16_t pin_index;
    uint16_t item;
} T_PAD_ANALOG_ITEM;

/**
  * \defgroup    Pinmux_Table Pinmux Table
  * \{
  */
const T_PINMUX_INDEX_TABLE pinmux_index_table =
{
    .index[ADC_0] =         0,
    .index[ADC_1] =         1,
    .index[ADC_2] =         2,
    .index[ADC_3] =         3,
    .index[ADC_4] =         4,
    .index[ADC_5] =         5,
    .index[ADC_6] =         6,
    .index[ADC_7] =         7,
    .index[P1_0] =          8,
    .index[P1_1] =          9,
    .index[P1_2] =          10,
    .index[P1_3] =          11,
    .index[P1_4] =          12,
    .index[P1_5] =          13,
    .index[P2_0] =          16,
    .index[P2_1] =          17,
    .index[P2_2] =          18,
    .index[P2_3] =          19,
    .index[P2_4] =          20,
    .index[P2_5] =          21,
    .index[P2_6] =          22,
    .index[P2_7] =          23,
    .index[P3_0] =          24,
    .index[P3_1] =          25,
    .index[P3_2] =          26,
    .index[P3_3] =          27,
    .index[P3_4] =          28,
    .index[P3_5] =          29,
    .index[P4_0] =          32,
    .index[P4_1] =          33,
    .index[P4_2] =          34,
    .index[P4_3] =          35,
    .index[P4_4] =          36,
    .index[P4_5] =          37,
    .index[P4_6] =          38,
    .index[P4_7] =          39,
    .index[P5_0] =          40,
    .index[P5_1] =          41,
    .index[P5_2] =          42,
    .index[P5_3] =          43,
    .index[P5_4] =          44,
    .index[P5_5] =          45,
    .index[P5_6] =          46,
    .index[P6_0] =          48,
    .index[P6_1] =          49,
    .index[P6_2] =          50,
    .index[P6_3] =          51,
    .index[P6_4] =          52,
    .index[P6_5] =          53,
    .index[P6_6] =          54,
    .index[P7_0] =          56,
    .index[P7_1] =          57,
    .index[P7_2] =          58,
    .index[P7_3] =          59,
    .index[P7_4] =          60,
    .index[P7_5] =          61,
    .index[P7_6] =          62,
    .index[P7_7] =          63,
    .index[P8_0] =          64,
    .index[P8_1] =          65,
    .index[P8_2] =          66,
    .index[P8_3] =          67,
    .index[P8_4] =          68,
    .index[P8_5] =          69,
    .index[P8_6] =          70,
    .index[P8_7] =          71,
    .index[P9_0] =          72,
    .index[P9_1] =          73,
    .index[P9_2] =          74,
    .index[P9_3] =          75,
    .index[P9_4] =          76,
    .index[P9_5] =          77,
    .index[P9_6] =          78,
    .index[P10_0] =         80,
    .index[P10_1] =         81,
    .index[P10_2] =         82,
    .index[P10_3] =         83,
    .index[P10_4] =         84,
    .index[P10_5] =         85,
    .index[P10_6] =         86,
    .index[P10_7] =         87,
    .index[DAOUT1_P] =      88,
    .index[DAOUT1_N] =      89,
    .index[DAOUT2_P] =      90,
    .index[DAOUT2_N] =      91,
    .index[MIC1_P] =        92,
    .index[MIC1_N] =        93,
    .index[MIC2_P] =        94,
    .index[MIC2_N] =        95,
    .index[MICBIAS] =       96,
    .index[DP] =            100,
    .index[DM] =            101,
    .index[SPIC1_SIO2] =    104,
    .index[SPIC1_SIO1] =    105,
    .index[SPIC1_CSN] =     106,
    .index[SPIC1_SIO0] =    107,
    .index[SPIC1_SCK] =     108,
    .index[SPIC1_SIO3] =    109,
};

/******************* Reference: BB2Ultra_IO_module_20240701_v1.xlsx *******************/

/**
  * \defgroup    PAD_Table Pad Table
  * \{
  */
const T_PIN_ADDR_TABLE pin_addr_table =
{
    .reg[ADC_0] =     0x1560,
    .reg[ADC_1] =     0x1562,
    .reg[ADC_2] =     0x1564,
    .reg[ADC_3] =     0x1566,
    .reg[P1_0] =      0x1568,
    .reg[P1_1] =      0x156A,
    .reg[P1_2] =      0x156C,
    .reg[P1_3] =      0x156E,
    .reg[P1_4] =      0x1570,
    .reg[P1_5] =      0x1572,
    .reg[P2_0] =      0x1578,
    .reg[P2_1] =      0x157A,
    .reg[P2_2] =      0x157C,
    .reg[P2_3] =      0x157E,
    .reg[P2_4] =      0x1580,
    .reg[P2_5] =      0x1582,
    .reg[P2_6] =      0x1584,
    .reg[P2_7] =      0x1586,
    .reg[P3_0] =      0x1588,
    .reg[P3_1] =      0x158A,
    .reg[P3_2] =      0x158C,
    .reg[P3_3] =      0x158E,
    .reg[P3_4] =      0x1590,
    .reg[P3_5] =      0x1592,
    .reg[P4_0] =      0x1594,
    .reg[P4_1] =      0x1596,
    .reg[P4_2] =      0x1598,
    .reg[P4_3] =      0x159A,
    .reg[P4_4] =      0x159C,
    .reg[P4_5] =      0x159E,
    .reg[P4_6] =      0x15A0,
    .reg[P4_7] =      0x15A2,
    .reg[P5_0] =      0x15A4,
    .reg[P5_1] =      0x15A6,
    .reg[P5_2] =      0x15A8,
    .reg[P5_3] =      0x15AA,
    .reg[P5_4] =      0x15AC,
    .reg[P5_5] =      0x15AE,
    .reg[P5_6] =      0x15B0,
    .reg[P6_0] =      0x15B4,
    .reg[P6_1] =      0x15B6,
    .reg[P6_2] =      0x15B8,
    .reg[P6_3] =      0x15BA,
    .reg[P6_4] =      0x15BC,
    .reg[P6_5] =      0x15BE,
    .reg[P6_6] =      0x15C0,
    .reg[P7_0] =      0x15C2,
    .reg[P7_1] =      0x15C4,
    .reg[P7_2] =      0x15C6,
    .reg[P7_3] =      0x15C8,
    .reg[P7_4] =      0x15CA,
    .reg[P7_5] =      0x15CC,
    .reg[P7_6] =      0x15CE,
    .reg[P8_0] =      0x15D0,
    .reg[P8_1] =      0x15D2,
    .reg[P8_2] =      0x15D4,
    .reg[P8_3] =      0x15D6,
    .reg[P8_4] =      0x15D8,
    .reg[P8_5] =      0x15DA,
    .reg[SPIC1_CSN] = 0x15E8,
    .reg[SPIC1_SCK] = 0x15EA,
    .reg[SPIC1_SIO0] = 0x15EC,
    .reg[SPIC1_SIO1] = 0x15EE,
    .reg[SPIC1_SIO2] = 0x15F0,
    .reg[SPIC1_SIO3] = 0x15F2,
    .reg[P9_2] =      0x15F4,
    .reg[P9_4] =      0x15F6,
    .reg[P9_3] =      0x15F8,
    .reg[P9_1] =      0x15FA,
    .reg[P9_0] =      0x15FC,
    .reg[P9_5] =      0x15FE,
    .reg[MIC1_P] =    0x1600,
    .reg[MIC1_N] =    0x1602,
    .reg[MIC2_P] =    0x1604,
    .reg[MIC2_N] =    0x1606,
    .reg[DAOUT1_P] =  0x160C,
    .reg[DAOUT1_N] =  0x160E,
    .reg[DAOUT2_P] =  0x1610,
    .reg[DAOUT2_N] =  0x1612,
    .reg[MICBIAS] =   0x1618,
    .reg[P10_0] =     0x161A,
    .reg[P8_6] =      0x161C,
    .reg[P8_7] =      0x161E,
    .reg[P9_6] =      0x1620,
    .reg[P10_1] =     0x1622,
    .reg[P10_2] =     0x1624,
    .reg[P10_3] =     0x1626,
    .reg[P10_4] =     0x1628,
    .reg[P10_5] =     0x162A,
    .reg[P10_6] =     0x162C,
    .reg[P10_7] =     0x162E,
    .reg[ADC_4] =     0x1630,
    .reg[ADC_5] =     0x1632,
    .reg[ADC_6] =     0x1634,
    .reg[ADC_7] =     0x1636,
    .reg[P7_7] =      0x1638,
    .reg[DP] =        0x163A,
    .reg[DM] =        0x163C,
};

const T_PIN_ADDR_TABLE pad_wake_sts_table =
{
    .reg[ADC_0] =         PAD_ITEM(0x7EC, 0),
    .reg[ADC_1] =         PAD_ITEM(0x7EC, 1),
    .reg[ADC_2] =         PAD_ITEM(0x7EC, 2),
    .reg[ADC_3] =         PAD_ITEM(0x7EC, 3),
    .reg[ADC_4] =         PAD_ITEM(0x7EC, 4),
    .reg[ADC_5] =         PAD_ITEM(0x7EC, 5),
    .reg[ADC_6] =         PAD_ITEM(0x7EC, 6),
    .reg[ADC_7] =         PAD_ITEM(0x7EC, 7),
    .reg[P1_0] =          PAD_ITEM(0x7EC, 8),
    .reg[P1_1] =          PAD_ITEM(0x7EC, 9),
    .reg[P1_2] =          PAD_ITEM(0x7EC, 10),
    .reg[P1_3] =          PAD_ITEM(0x7EC, 11),
    .reg[P1_4] =          PAD_ITEM(0x7EC, 12),
    .reg[P1_5] =          PAD_ITEM(0x7EC, 13),
    .reg[P2_0] =          PAD_ITEM(0x7EE, 0),
    .reg[P2_1] =          PAD_ITEM(0x7EE, 1),
    .reg[P2_2] =          PAD_ITEM(0x7EE, 2),
    .reg[P2_3] =          PAD_ITEM(0x7EE, 3),
    .reg[P2_4] =          PAD_ITEM(0x7EE, 4),
    .reg[P2_5] =          PAD_ITEM(0x7EE, 5),
    .reg[P2_6] =          PAD_ITEM(0x7EE, 6),
    .reg[P2_7] =          PAD_ITEM(0x7EE, 7),
    .reg[P3_0] =          PAD_ITEM(0x7EE, 8),
    .reg[P3_1] =          PAD_ITEM(0x7EE, 9),
    .reg[P3_2] =          PAD_ITEM(0x7EE, 10),
    .reg[P3_3] =          PAD_ITEM(0x7EE, 11),
    .reg[P3_4] =          PAD_ITEM(0x7EE, 12),
    .reg[P3_5] =          PAD_ITEM(0x7EE, 13),
    .reg[P4_0] =          PAD_ITEM(0x7F0, 0),
    .reg[P4_1] =          PAD_ITEM(0x7F0, 1),
    .reg[P4_2] =          PAD_ITEM(0x7F0, 2),
    .reg[P4_3] =          PAD_ITEM(0x7F0, 3),
    .reg[P4_4] =          PAD_ITEM(0x7F0, 4),
    .reg[P4_5] =          PAD_ITEM(0x7F0, 5),
    .reg[P4_6] =          PAD_ITEM(0x7F0, 6),
    .reg[P4_7] =          PAD_ITEM(0x7F0, 7),
    .reg[P5_0] =          PAD_ITEM(0x7F0, 8),
    .reg[P5_1] =          PAD_ITEM(0x7F0, 9),
    .reg[P5_2] =          PAD_ITEM(0x7F0, 10),
    .reg[P5_3] =          PAD_ITEM(0x7F0, 11),
    .reg[P5_4] =          PAD_ITEM(0x7F0, 12),
    .reg[P5_5] =          PAD_ITEM(0x7F0, 13),
    .reg[P5_6] =          PAD_ITEM(0x7F0, 14),
    .reg[P7_0] =          PAD_ITEM(0x7F2, 0),
    .reg[P7_1] =          PAD_ITEM(0x7F2, 1),
    .reg[P7_2] =          PAD_ITEM(0x7F2, 2),
    .reg[P7_3] =          PAD_ITEM(0x7F2, 3),
    .reg[P7_4] =          PAD_ITEM(0x7F2, 4),
    .reg[P7_5] =          PAD_ITEM(0x7F2, 5),
    .reg[P7_6] =          PAD_ITEM(0x7F2, 6),
    .reg[P7_7] =          PAD_ITEM(0x7F2, 7),
    .reg[P6_0] =          PAD_ITEM(0x7F2, 8),
    .reg[P6_1] =          PAD_ITEM(0x7F2, 9),
    .reg[P6_2] =          PAD_ITEM(0x7F2, 10),
    .reg[P6_3] =          PAD_ITEM(0x7F2, 11),
    .reg[P6_4] =          PAD_ITEM(0x7F2, 12),
    .reg[P6_5] =          PAD_ITEM(0x7F2, 13),
    .reg[P6_6] =          PAD_ITEM(0x7F2, 14),
    .reg[P8_0] =          PAD_ITEM(0x7F4, 0),
    .reg[P8_1] =          PAD_ITEM(0x7F4, 1),
    .reg[P8_2] =          PAD_ITEM(0x7F4, 2),
    .reg[P8_3] =          PAD_ITEM(0x7F4, 3),
    .reg[P8_4] =          PAD_ITEM(0x7F4, 4),
    .reg[P8_5] =          PAD_ITEM(0x7F4, 5),
    .reg[P8_6] =          PAD_ITEM(0x7F4, 6),
    .reg[P8_7] =          PAD_ITEM(0x7F4, 7),
    .reg[P9_0] =          PAD_ITEM(0x7F4, 8),
    .reg[P9_1] =          PAD_ITEM(0x7F4, 9),
    .reg[P9_2] =          PAD_ITEM(0x7F4, 10),
    .reg[P9_3] =          PAD_ITEM(0x7F4, 11),
    .reg[P9_4] =          PAD_ITEM(0x7F4, 12),
    .reg[P9_5] =          PAD_ITEM(0x7F4, 13),
    .reg[P9_6] =          PAD_ITEM(0x7F4, 14),
    .reg[MIC1_P] =        PAD_ITEM(0x7F6, 0),
    .reg[MIC1_N] =        PAD_ITEM(0x7F6, 1),
    .reg[MIC2_P] =        PAD_ITEM(0x7F6, 2),
    .reg[MIC2_N] =        PAD_ITEM(0x7F6, 3),
    .reg[DAOUT1_P] =      PAD_ITEM(0x7F6, 6),
    .reg[DAOUT1_N] =      PAD_ITEM(0x7F6, 7),
    .reg[DAOUT2_P] =      PAD_ITEM(0x7F6, 8),
    .reg[DAOUT2_N] =      PAD_ITEM(0x7F6, 9),
    .reg[MICBIAS] =       PAD_ITEM(0x7F6, 12),
    .reg[P10_0] =         PAD_ITEM(0x7F8, 0),
    .reg[P10_1] =         PAD_ITEM(0x7F8, 1),
    .reg[P10_2] =         PAD_ITEM(0x7F8, 2),
    .reg[P10_3] =         PAD_ITEM(0x7F8, 3),
    .reg[P10_4] =         PAD_ITEM(0x7F8, 4),
    .reg[P10_5] =         PAD_ITEM(0x7F8, 5),
    .reg[P10_6] =         PAD_ITEM(0x7F8, 6),
    .reg[P10_7] =         PAD_ITEM(0x7F8, 7),
    .reg[SPIC1_CSN] =     PAD_ITEM(0x7F8, 8),
    .reg[SPIC1_SCK] =     PAD_ITEM(0x7F8, 9),
    .reg[SPIC1_SIO0] =    PAD_ITEM(0x7F8, 10),
    .reg[SPIC1_SIO1] =    PAD_ITEM(0x7F8, 11),
    .reg[SPIC1_SIO2] =    PAD_ITEM(0x7F8, 12),
    .reg[SPIC1_SIO3] =    PAD_ITEM(0x7F8, 13),
    .reg[DP] =            PAD_ITEM(0x7F8, 14),
    .reg[DM] =            PAD_ITEM(0x7F8, 15),
};

/******************* Reference: BB2Ultra_REG_20240703_V0.xlsx *******************/

const T_PAD_FUNCTION pad_func_table[] =
{
    {ADC_0,  12, 0x744},
    {ADC_1,  9,  0x744},
    {P1_0,   6,  0x744},
    {P1_1,   3,  0x744},
    {P1_2,   0,  0x744},
    {P2_0,   12, 0x746},
    {P2_1,   9,  0x746},
    {P2_2,   6,  0x746},
    {P2_3,   3,  0x746},
    {P2_4,   0,  0x746},
    {P3_0,   12, 0x748},
    {P3_1,   9,  0x748},
};

const T_PAD_ANALOG_ITEM analog_table[] =
{
    {DAOUT1_N,  PAD_ITEM(AON_FAST_0x120, 2)},
    {DAOUT1_P,  PAD_ITEM(AON_FAST_0x120, 3)},
    {MIC2_N,  PAD_ITEM(AON_FAST_0x120, 4)},
    {MIC2_P,  PAD_ITEM(AON_FAST_0x120, 5)},
    {MIC1_N,  PAD_ITEM(AON_FAST_0x120, 6)},
    {MIC1_P,  PAD_ITEM(AON_FAST_0x120, 7)},
    {MICBIAS, PAD_ITEM(AON_FAST_0x122, 13)},
};

/******************* Reference: BB2Ultra_REG_20240621_V1.xlsx *******************/

/**
  * @brief  According to the mode set to the pin , write the regster of AON which the pin coresponding .
  * @param  mode: mean one IO function, please refer to rtl876x_pinmux.h "Pin_Function_Number" part.
  *     @arg SHDN: use software mode.
  *     @arg PAD_OUT_EN: use pinmux mode.
        ......
        reference of bit of AON register mean in pinmux.h
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  value: value of the register bit ,0 or 1.
  * @retval None
  */
void Pad_TableConfig(AON_FAST_PAD_BIT_POS_TYPE pad_bit_set, uint8_t Pin_Num, uint8_t value)
{
    AON_FAST_REG_PAD_TYPE tmpVal;
    uint16_t reg_temp;

    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return ;
    }

    reg_temp = pin_addr_table.reg[Pin_Num];

    if (reg_temp == 0)
    {
        return;
    }

    tmpVal.d16 = btaon_fast_read_safe(reg_temp);
    if (value == 0)
    {
        tmpVal.d16 &= ~BIT(pad_bit_set);
    }
    else if (value == 1)
    {
        tmpVal.d16 |=   BIT(pad_bit_set);
    }

    btaon_fast_write_safe(reg_temp, tmpVal.d16);
}

/**
  * @brief  According to the mode set to the pin , write the regster of AON which the pin coresponding .
  * @param  mode: mean one IO function, please refer to rtl876x_pinmux.h "Pin_Function_Number" part.
  *     @arg SHDN: use software mode.
  *     @arg PAD_OUT_EN: use pinmux mode.
        ......
        reference of bit of AON register mean in pinmux.h
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  value: value of the register bit ,0 or 1.
  * @retval None
  */
void Pad_TableUpdate(uint16_t bit_mask, uint8_t Pin_Num, uint8_t bit_data)
{
    AON_FAST_REG_PAD_TYPE tmpVal;
    uint16_t reg_temp;

    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return ;
    }

    reg_temp = pin_addr_table.reg[Pin_Num];

    if (reg_temp == 0)
    {
        return;
    }

    btaon_fast_update(reg_temp, bit_mask, bit_data);
}

/**
  * @brief  Config pin to its corresponding IO function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  Pin_Func: mean one IO function, please refer to rtl876x_pinmux.h "Pin_Function_Number" part.
  * @retval None
  */
void Pinmux_Config_rom(uint8_t Pin_index, uint8_t Pin_Func)
{
    uint8_t pinmux_reg_num;
    uint8_t reg_offset;

    if (Pin_index >= TOTAL_PIN_NUM)
    {
        return;
    }

    uint8_t Pin_Num = pinmux_index_table.index[Pin_index];

    pinmux_reg_num = Pin_Num >> 2;
    reg_offset = (Pin_Num & 0x03) << 3;

    PINMUX0->CFG[pinmux_reg_num] = (PINMUX0->CFG[pinmux_reg_num] & ~(0xFFU << reg_offset))
                                   | Pin_Func << reg_offset;

    return;
}

/**
  * @brief  Deinit the IO function of one pin.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval None
  */
void Pinmux_Deinit(uint8_t Pin_index)
{
    if (Pin_index >= TOTAL_PIN_NUM)
    {
        return;
    }

    uint8_t pinmux_reg_num;
    uint8_t pinmux_index = pinmux_index_table.index[Pin_index];

    pinmux_reg_num = pinmux_index >> 2;
    PINMUX0->CFG[pinmux_reg_num] &= ~(0xFFU << ((pinmux_index % 4) << 3));

    return;
}

/**
  * @brief  Reset all pin to default value.
  * @param  None.
  * @note: two SWD pins will also be reset. Please use this function carefully.
  * @retval None
  */
void Pinmux_Reset(void)
{
    uint8_t i;

    for (i = 0; i < 27; i++)
    {
        PINMUX0->CFG[i] = 0x00;
    }

    return;
}

/**
  * @brief  config the corresponding pad.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  AON_PAD_MODE: use software mode or pinmux mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_SW_MODE: use software mode.
  *     @arg PAD_PINMUX_MODE: use pinmux mode.
  * @param  AON_PAD_PwrOn: config power of pad.
  *     This parameter can be one of the following values:
  *     @arg PAD_NOT_PWRON: shutdown power of pad.
  *     @arg PAD_IS_PWRON: enable power of pad.
  * @param  AON_PAD_Pull: config pad pull mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_PULL_NONE: no pull.
  *     @arg PAD_PULL_UP: pull this pin up.
  *     @arg PAD_PULL_DOWN: pull thi pin down.
  * @param  AON_PAD_E: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_DISABLE: disable pin output.
  *     @arg PAD_OUT_ENABLE: enable pad output.
  * @param  AON_PAD_O: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_LOW: pad output low.
  *     @arg PAD_OUT_HIGH: pad output high.
  * @retval None
  */
void Pad_Config_rom(uint8_t Pin_Num,
                    PAD_Mode AON_PAD_Mode,
                    PAD_PWR_Mode AON_PAD_PwrOn,
                    PAD_Pull_Mode AON_PAD_Pull,
                    PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                    PAD_OUTPUT_VAL AON_PAD_O)
{
    AON_FAST_REG_PAD_TYPE tmpVal;
    uint16_t reg_temp;

    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return;
    }

    reg_temp = pin_addr_table.reg[Pin_Num];
    if (reg_temp == 0)
    {
        return;
    }

    tmpVal.d16 = btaon_fast_read_safe(reg_temp);

    tmpVal.PAD_S = AON_PAD_Mode;
    tmpVal.PAD_SHDN = AON_PAD_PwrOn;
    /* Pull Config */
    if (AON_PAD_Pull == PAD_PULL_NONE)
    {
        tmpVal.PAD_PU_EN = 0;
    }
    else
    {
        tmpVal.PAD_PU_EN = 1;
        tmpVal.PAD_PUPD = AON_PAD_Pull;
    }

    tmpVal.PAD_OUT_EN = AON_PAD_E;
    tmpVal.PAD_OUT_VALUE = AON_PAD_O;

    btaon_fast_write_safe(reg_temp, tmpVal.d16);
}

/**
  * @brief  config the corresponding pad.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  AON_PAD_MODE: use software mode or pinmux mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_SW_MODE: use software mode.
  *     @arg PAD_PINMUX_MODE: use pinmux mode.
  * @param  AON_PAD_PwrOn: config power of pad.
  *     This parameter can be one of the following values:
  *     @arg PAD_NOT_PWRON: shutdown power of pad.
  *     @arg PAD_IS_PWRON: enable power of pad.
  * @param  AON_PAD_Pull: config pad pull mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_PULL_NONE: no pull.
  *     @arg PAD_PULL_UP: pull this pin up.
  *     @arg PAD_PULL_DOWN: pull thi pin down.
  * @param  AON_PAD_E: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_DISABLE: disable pin output.
  *     @arg PAD_OUT_ENABLE: enable pad output.
  * @param  AON_PAD_O: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_LOW: pad output low.
  *     @arg PAD_OUT_HIGH: pad output high.
  * @retval None
  */
void Pad_ConfigExt_rom(uint8_t Pin_Num,
                       PAD_Mode AON_PAD_Mode,
                       PAD_PWR_Mode AON_PAD_PwrOn,
                       PAD_Pull_Mode AON_PAD_Pull,
                       PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                       PAD_OUTPUT_VAL AON_PAD_O,
                       PAD_PULL_VAL AON_PAD_P)
{
    AON_FAST_REG_PAD_TYPE tmpVal;
    uint16_t reg_temp;

    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return;
    }

    reg_temp = pin_addr_table.reg[Pin_Num];
    if (reg_temp == 0)
    {
        return;
    }

    tmpVal.d16 = btaon_fast_read_safe(reg_temp);

    tmpVal.PAD_S = AON_PAD_Mode;
    tmpVal.PAD_SHDN = AON_PAD_PwrOn;
    /* Pull Config */
    if (AON_PAD_Pull == PAD_PULL_NONE)
    {
        tmpVal.PAD_PU_EN = 0;
    }
    else
    {
        tmpVal.PAD_PU_EN = 1;
        tmpVal.PAD_PUPD = AON_PAD_Pull;
    }

    tmpVal.PAD_OUT_EN = AON_PAD_E;
    tmpVal.PAD_OUT_VALUE = AON_PAD_O;
    tmpVal.PAD_PUPDC = AON_PAD_P;

    btaon_fast_write_safe(reg_temp, tmpVal.d16);
}

/**
  * @brief  Set all pins to the default state.
  * @param  void.
  * @retval void.
  */

void Pad_AllConfigDefault_rom(void)
{
    uint16_t i = 0;
    /* Set Output disable, pull-none, pull down, Software mode, Output_low, wakeup_disable, Wake up polarity high */
    for (i = 0; i < TOTAL_PIN_NUM; i++)
    {
        if (pin_addr_table.reg[i] == 0x0)
        {
            continue;
        }
        Pad_ConfigExt(i, PAD_SW_MODE, PAD_SHUTDOWN, PAD_PULL_DOWN, PAD_OUT_DISABLE, PAD_OUT_LOW,
                      PAD_WEAKLY_PULL);
    }
}

/**
  * @brief  Enable pin wakeup function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  Polarity: PAD_WAKEUP_POL_HIGH--use high level wakeup, PAD_WAKEUP_POL_LOW-- use low level wakeup.
  * @retval None
  */
void System_WakeUpPinEnable_rom(uint8_t Pin_Num, uint8_t Polarity, uint8_t DebounceEn)
{
    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return;
    }

    Pad_WakeupPolarityValue(Pin_Num, Polarity);
    Pad_WakeupEnableValue(Pin_Num, 1);
}

/**
  * @brief  Disable pin wakeup function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval None
  */

void System_WakeUpPinDisable_rom(uint8_t Pin_Num)
{
    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return;
    }

    Pad_WakeupEnableValue(Pin_Num, 0);
}

/**
  * @brief  Disable wake up pin interrupt function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval None
  */
void System_WakeUpInterruptEnable_rom(uint8_t Pin_Num)
{
    Pad_TableConfig(PAD_WKUP_INT_EN, Pin_Num,  1) ;
}

/**
  * @brief  Disable wake up pin interrupt function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval None
  */
void System_WakeUpInterruptDisable_rom(uint8_t Pin_Num)
{
    Pad_TableConfig(PAD_WKUP_INT_EN, Pin_Num,  0) ;
    Pad_ClearWakeupINTPendingBit(Pin_Num);
}

/**
  * @brief  Clear all wake up pin interrupt pending bit.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part. PAD_Px_STS[x]
  * @retval None
  */
void Pad_ClearAllWakeupINT(void)
{
    btaon_fast_write_safe(0x7EC, PAD_WK_STS_DATA_AON_REG_7EC);
    btaon_fast_write_safe(0x7EE, PAD_WK_STS_DATA_AON_REG_7EE);
    btaon_fast_write_safe(0x7F0, PAD_WK_STS_DATA_AON_REG_7F0);
    btaon_fast_write_safe(0x7F2, PAD_WK_STS_DATA_AON_REG_7F2);
    btaon_fast_write_safe(0x7F4, PAD_WK_STS_DATA_AON_REG_7F4);
    btaon_fast_write_safe(0x7F6, PAD_WK_STS_DATA_AON_REG_7F6);
    btaon_fast_write_safe(0x7F8, PAD_WK_STS_DATA_AON_REG_7F8);
}

/**
  * @brief  config pad pull mode.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  value: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_PULL_DOWN: pad pull down.
  *     @arg PAD_PULL_UP: pad pull up.
  *     @arg PAD_PULL_NONE: pad_pull none
  * @retval None
  */
void Pad_SetPullMode(uint8_t Pin_Num, PAD_Pull_Mode pull_mode)
{
    uint16_t addr = pin_addr_table.reg[Pin_Num];
    if (addr == 0)
    {
        return;
    }
    AON_FAST_REG_PAD_TYPE tmpVal;

    tmpVal.d16 = btaon_fast_read_safe(addr);
    if (pull_mode == PAD_PULL_NONE)
    {
        tmpVal.PAD_PU_EN = 0;
    }
    else
    {
        tmpVal.PAD_PU_EN = 1;
    }
    tmpVal.PAD_PUPD = pull_mode;
    btaon_fast_write_safe((addr), tmpVal.d16);
}

/**
  * @brief  Check wake up pin interrupt status.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval Pin interrupt status
  */
FlagStatus Pad_WakeupInterruptValue(uint8_t Pin_Num)
{
    if (Pin_Num >= TOTAL_PIN_NUM)
    {
        return RESET;
    }

    uint16_t pad_item = pad_wake_sts_table.reg[Pin_Num];
    if (pad_item == 0)
    {
        return RESET;
    }

    if (btaon_fast_read_safe(PAD_ITEM_ADDR(pad_item)) & BIT(PAD_ITEM_BIT_OFFSET(pad_item)))
    {
        return SET;
    }

    return RESET;
}

/**
  * @brief  Clear pad wake up pin interrupt pending bit.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval None
  */
void Pad_ClearWakeupINTPendingBit(uint8_t Pin_Num)
{
    uint16_t pad_item_temp = pad_wake_sts_table.reg[Pin_Num];
    if (pad_item_temp == 0)
    {
        return;
    }

    uint16_t pad_addr = PAD_ITEM_ADDR(pad_item_temp);
    uint8_t pad_bit = PAD_ITEM_BIT_OFFSET(pad_item_temp);
    btaon_fast_write_safe(pad_addr, BIT(pad_bit));
}

/**
  * @brief  Config Pad Function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P7_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  PAD_FUNCTION_CONFIG_VAL: value.
  *   This parameter can be: AON_GPIO, LED0, LED1, LED2, CLK_REQ.
  * @retval None
  */
void Pad_FunctionConfig(uint8_t Pin_Num, PAD_FUNCTION_CONFIG_VAL value)
{
    uint8_t i = 0;

    if (value & PAD_AON_MUX_TYPE)
    {
        uint8_t offset = value & (~PAD_AON_MUX_TYPE);
        if (offset <= 6)
        {
            btaon_fast_write_safe(0x74C + offset, Pin_Num);
        }
    }
    else
    {
        uint8_t table_size = sizeof(pad_func_table) / sizeof(T_PAD_FUNCTION);

        for (i = 0; i <  table_size; i++)
        {
            if (pad_func_table[i].pin_index == Pin_Num)
            {
                break;
            }
        }

        if (i == table_size)
        {
            return;
        }

        /* Pad control mode */
        btaon_fast_update_safe(pad_func_table[i].reg_offset, 0x7 << pad_func_table[i].reg_bit, \
                               value << pad_func_table[i].reg_bit);
    }
}

/**
  * @brief  Get the Pad AON fast register value ,the register addr get through mode and pin.
  * @param  mode: mode of set .
  *            This parameter reference the define from DRIE2 to PAD_PUPDC_WE_STR in rtl876x_pinmux.h .
  *            Pin_Num:  pin number
  *            This parameter is from ADC_0 to P7_1, please refer to rtl876x.h "Pin_Number" part.
  * @retval reference PAD_Mode_Status.
  */
uint8_t  Pad_GetModeConfig(AON_FAST_PAD_BIT_POS_TYPE mode, uint8_t Pin_Num)
{
    uint16_t reg_temp;

    AON_FAST_REG_PAD_TYPE tmpVal;
    if (Pin_Num < TOTAL_PIN_NUM)
    {
        reg_temp = pin_addr_table.reg[Pin_Num];
        if (reg_temp == 0)
        {
            return PAD_AON_PIN_ERR;
        }

        tmpVal.d16 = btaon_fast_read_safe(reg_temp);

        if (tmpVal.d16 & BIT(mode))
        {
            return SET;
        }
        else
        {
            return RESET;
        }
    }
    else
    {
        return PAD_AON_PIN_ERR;
    }
}

/**
  * @brief  Get the Pad AON output value .
  * @param  Pin_Num:  pin number
  *            This parameter is from ADC_0 to H_12, please refer to rtl876x.h "Pin_Number" part.
  * @retval reference PAD_AON_Status.
  */
uint8_t Pad_GetOutputCtrl(uint8_t Pin_Num)
{
    if (Pad_GetModeConfig(PAD_OUT_EN, Pin_Num) == RESET)
    {
        return PAD_AON_OUTPUT_DISABLE;
    }
    else if (Pad_GetModeConfig(PAD_PINMUX_M_EN, Pin_Num) == SET)
    {
        return PAD_AON_PINMUX_ON;
    }
    if (Pad_GetModeConfig(PAD_OUT_VALUE, Pin_Num) == RESET)
    {
        return PAD_AON_OUTPUT_LOW;
    }
    else if (Pad_GetModeConfig(PAD_OUT_VALUE, Pin_Num) == SET)
    {
        return PAD_AON_OUTPUT_HIGH;
    }
    else
    {
        return PAD_AON_PIN_ERR;
    }
}

/**
  * @brief  set the system wakeup mode  .
  * @param  mode: mode of set .
  *            This parameter reference WAKEUP_EN_MODE .
  *        pol:  polarity to wake up
  *            This parameter WAKEUP_POL POL_HIGH means high level POL_LOW means low level to wakeup.
  *        NewState:  Enable or disable to wake up
  *            This parameter value is ENABLE or DISABLE.
  * @retval  1 means wrong mode.
  */
uint8_t  Pad_WakeUpCmd_rom(WAKEUP_EN_MODE mode, WAKEUP_POL pol, FunctionalState NewState)
{
    AON_FAST_SET_WKEN_MISC_TYPE wkup_wken = {.d16 = btaon_fast_read_safe(AON_FAST_SET_WKEN_MISC)};
    AON_FAST_SET_WKPOL_MISC_TYPE wkup_wkpol = {.d16 = btaon_fast_read_safe(AON_FAST_SET_WKPOL_MISC)};
    switch (mode)
    {
    case ADP_MODE:
        wkup_wken.ADP_WKEN = NewState;
        wkup_wkpol.ADP_WKPOL = pol;
        break;
    case BAT_MODE:
        wkup_wken.BAT_WKEN = NewState;
        wkup_wkpol.BAT_WKPOL = pol;
        break;
    case MFB_MODE:
        wkup_wken.MFB_WKEN = NewState;
        wkup_wkpol.MFB_WKPOL = !pol;
        break;
    case USB_MODE:
        wkup_wken.USB_WKEN = NewState;
        wkup_wkpol.USB_WKPOL = pol;
        break;
    default:
        return 1;
    }

    btaon_fast_write_safe(AON_FAST_SET_WKPOL_MISC, wkup_wkpol.d16);
    btaon_fast_write_safe(AON_FAST_SET_WKEN_MISC, wkup_wken.d16);

    return 0;
}

/**
  * @brief  Config PAD highs speed functions.
  * @param  Pin_Num: pin number.
  *    This parameter is from ADC_0 to Max pin number, please refer to pin_def.h "Pin_Number" part.
  * @param  PAD_HS_MUX_SEL_TYPE: value.
  *   This parameter can be: FROM_AON_MUX/FROM_HS_MUX.
  * @retval true: Set high speed mux success
  * @retval false: Set high speed mux failed due to invalid pinmux
  */
bool Pad_HighSpeedMuxSel(uint8_t Pin_Num, PAD_HS_MUX_SEL_TYPE Pad_Hs_Mux)
{
    if ((Pin_Num <= ADC_2) ||
        (Pin_Num >= P3_2 && Pin_Num <= P1_3) ||
        (Pin_Num >= P2_0 && Pin_Num <= P2_1) ||
        (Pin_Num >= P3_0 && Pin_Num <= MICBIAS) ||
        (Pin_Num == P10_0))
    {
        return false;
    }
    else
    {
        Pad_TableConfig(PAD_HS_MUX, Pin_Num, Pad_Hs_Mux);
    }

    return true;
}

/**
  * @brief  PAD high speed mode function select.
  * @param  Pin_Num: pin number.
  *    This parameter is from ADC_0 to Max pin number, please refer to rtl876x.h "Pin_Number" part.
  * @param  PAD_HS_FUNC_SEL_TYPE: Pad_HS_Func_Sel.
  *   This parameter can be: HS_Func0/HS_Func1.
  *   P4_2~P4_5: 0 Display Controller, 1 SPI0;
  *   P6_0~P6_5: 0 Display Controller, 1 SDIO0;
  *   P8_0~P8_3: 0 Display Controller, 1 SPI1;
  *   P9_0~P9_5: 0 SPIC1, 1 OPI PSRAM.
  * @retval true: Set high speed mux success
  * @retval false: Set high speed mux failed due to invalid pinmux
  */
bool Pad_HighSpeedFuncSel(uint8_t Pin_Num, PAD_HS_FUNC_SEL_TYPE Pad_HS_Func_Sel)
{
    if ((Pin_Num >= P4_2 && Pin_Num <= P4_5) ||
        (Pin_Num >= P6_0 && Pin_Num <= P6_5) ||
        (Pin_Num >= P8_0 && Pin_Num <= P8_3) ||
        (Pin_Num >= P9_0 && Pin_Num <= P9_5))
    {
        Pad_TableConfig(PAD_HS_FUNC_SEL, Pin_Num, Pad_HS_Func_Sel);
    }
    else
    {
        return false;
    }

    return true;
}

void Pad_AnalogMode(uint8_t Pin_Num, ANA_MODE mode)
{
    if ((Pin_Num < MIC1_P) | (Pin_Num > DAOUT1_N))
    {
        return;
    }
    for (uint16_t i = 0; i < sizeof(analog_table) / sizeof(T_PAD_ANALOG_ITEM); i++)
    {
        if (analog_table[i].pin_index == Pin_Num)
        {
            uint16_t pad_item = analog_table[i].item;

            if (pad_item != 0)
            {
                uint16_t addr = PAD_ITEM_ADDR(pad_item);
                uint16_t bit = BIT(PAD_ITEM_BIT_OFFSET(pad_item));

                if (mode == PAD_ANALOG_MODE)
                {
                    btaon_fast_update_safe(addr, bit, bit);
                }
                else
                {
                    btaon_fast_update_safe(addr, bit, 0);
                }
            }

            break;
        }
    }
}

