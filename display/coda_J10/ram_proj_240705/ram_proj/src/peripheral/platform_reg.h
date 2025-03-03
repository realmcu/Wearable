#ifndef PLATFORM_REG_H
#define PLATFORM_REG_H
#include <stdint.h>
#include "address_map.h"

/* Auto gen based on BB2Ultra_vendor_reg_20240605_v2.xlsx */

typedef struct
{
    /* 0x0000       0x4000_6000
        15:0    R/W    Wdt_divfactor                       1
        23:16   R/W    Wdt_en_byte                         0
        24      W      Wdt_clear                           1
        28:25   R/W    Cnt_limit                           0
        29      R/W    wdtaon_en                           1
        30      R/W    Wdt_mode                            0
        31      R/W1C  Wdt_to                              0
    */
    union
    {
        __IO uint32_t REGWATCH_DOG_TIMER;
        struct
        {
            __IO uint32_t Wdt_divfactor: 16;
            __IO uint32_t Wdt_en_byte: 8;
            __IO uint32_t Wdt_clear: 1;
            __IO uint32_t Cnt_limit: 4;
            __IO uint32_t wdtaon_en: 1;
            __IO uint32_t Wdt_mode: 1;
            __IO uint32_t Wdt_to: 1;
        } BITS_000;
    } u_000;

    /* 0x0004       0x4000_6004
        0       R/W1C  MBIAS_MFB_DET_L                     0
        1       R/W1C  mailbox_int                         0
        2       R/W1C  utmi_suspend_n                      0
        3       R/W1C  dig_trda_int_r                      0
        4       R/W1C  rng_int                             0
        5       R/W1C  psram_intr                          0
        6       R/W1C  dig_lpcomp_int_r                    0
        7       R/W1C  timer_intr[5]                       0
        8       R/W1C  timer_intr[6]                       0
        9       R/W1C  timer_intr[7]                       0
        10      R      reserved                            0
        11      R/W1C  dig_lpcomp_int                      0
        12      R/W1C  MBIAS_VBAT_DET_L                    0
        13      R/W1C  MBIAS_ADP_DET_L                     0
        14      R/W1C  HW_ASRC_ISR1                        0
        15      R/W1C  HW_ASRC_ISR2                        0
        16      R/W1C  gpio_intr[31:6]                     0
        17      R      reserved                            0
        18      R/W1C  dsp_wdt_to_mcu_intr                 0
        19      R/W1C  flash_pwr_intr                      0
        20      R      reserved                            0
        24:21   R      reserved                            0
        25      R/W1C  sp0_intr_tx                         0
        26      R/W1C  sp0_intr_rx                         0
        27      R/W1C  sp1_intr_tx                         0
        28      R/W1C  sp1_intr_rx                         0
        29      R      reserved                            0
        30      R      reserved                            0
        31      R      reserved                            0
    */
    union
    {
        __IO uint32_t REG_LOW_PRI_INT_STATUS;
        struct
        {
            __IO uint32_t MBIAS_MFB_DET_L: 1;
            __IO uint32_t mailbox_int: 1;
            __IO uint32_t utmi_suspend_n: 1;
            __IO uint32_t dig_trda_int_r: 1;
            __IO uint32_t rng_int: 1;
            __IO uint32_t psram_intr: 1;
            __IO uint32_t dig_lpcomp_int_r: 1;
            __IO uint32_t timer_intr_5: 1;
            __IO uint32_t timer_intr_6: 1;
            __IO uint32_t timer_intr_7: 1;
            __I uint32_t RESERVED_6: 1;
            __IO uint32_t dig_lpcomp_int: 1;
            __IO uint32_t MBIAS_VBAT_DET_L: 1;
            __IO uint32_t MBIAS_ADP_DET_L: 1;
            __IO uint32_t HW_ASRC_ISR1: 1;
            __IO uint32_t HW_ASRC_ISR2: 1;
            __IO uint32_t gpio_intr_31_6: 1;
            __I uint32_t RESERVED_5: 1;
            __IO uint32_t dsp_wdt_to_mcu_intr: 1;
            __IO uint32_t flash_pwr_intr: 1;
            __I uint32_t RESERVED_4: 1;
            __I uint32_t RESERVED_3: 4;
            __IO uint32_t sp0_intr_tx: 1;
            __IO uint32_t sp0_intr_rx: 1;
            __IO uint32_t sp1_intr_tx: 1;
            __IO uint32_t sp1_intr_rx: 1;
            __I uint32_t RESERVED_2: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 1;
        } BITS_004;
    } u_004;

    /* 0x0008       0x4000_6008
        31:0    R/W    int_mode                            32'hffffffff
    */
    union
    {
        __IO uint32_t REG_LOW_PRI_INT_MODE;
        struct
        {
            __IO uint32_t int_mode: 32;
        } BITS_008;
    } u_008;

    /* 0x000C       0x4000_600c
        31:0    R/W    int_en                              32'h0
    */
    union
    {
        __IO uint32_t REG_LOW_PRI_INT_EN;
        struct
        {
            __IO uint32_t int_en: 32;
        } BITS_00C;
    } u_00C;

    /* 0x0010       0x4000_6010
        0       R      timer_intr1andtimer_intr0           0
        1       R      bluewiz_intr_r                      0
        2       R      hci_dma_intr                        0
        3       R      btverdor_reg_intr                   0
        4       R      RSVD                                0
        31:5    R      RSVD                                0
    */
    union
    {
        __IO uint32_t BT_MAC_interrupt;
        struct
        {
            __I uint32_t timer_intr1andtimer_intr0: 1;
            __I uint32_t bluewiz_intr_r: 1;
            __I uint32_t hci_dma_intr: 1;
            __I uint32_t btverdor_reg_intr: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 27;
        } BITS_010;
    } u_010;

    /* 0x0014       0x4000_6014
        0       R      otg_intr                            0
        1       R      sdio_host_intr                      0
        15:2    R      RSVD                                0
        16      R      rxi300_intr                         0
        17      R      rdp_intr                            0
        23:18   R      RSVD                                0
        24      RW     rxi300_intr_en                      1
        25      RW     rdp_intr_en                         0
        31:26   R      RSVD                                0
    */
    union
    {
        __IO uint32_t REG_PLATFORM_INT_CTRL;
        struct
        {
            __I uint32_t otg_intr: 1;
            __I uint32_t sdio_host_intr: 1;
            __I uint32_t RESERVED_2: 14;
            __I uint32_t rxi300_intr: 1;
            __I uint32_t rdp_intr: 1;
            __I uint32_t RESERVED_1: 6;
            __IO uint32_t rxi300_intr_en: 1;
            __IO uint32_t rdp_intr_en: 1;
            __I uint32_t RESERVED_0: 6;
        } BITS_014;
    } u_014;

    /* 0x0018       0x4000_6018
        31:0    RW     int_pol                             0
    */
    union
    {
        __IO uint32_t Interrupt_edge_option;
        struct
        {
            __IO uint32_t int_pol: 32;
        } BITS_018;
    } u_018;

    /* 0x001C       0x4000_601c
        11:0    RW     BIT_DEBUG_SEL                       0
        15:12   R      RSVD                                0
        16      RW     pke_clk_always_disable              1'h0
        17      RW     pke_clk_always_enable               1'h0
        18      RW     ppe_clk_always_disable              1'h0
        19      RW     ppe_clk_always_enable               1'h0
        28:20   R      RSVD                                0
        29      RW     spi_speed_up_sim                    0
        30      R      RSVD                                0
        31      RW     uartlog_sir_inv                     0
    */
    union
    {
        __IO uint32_t REG_DEBUG_SEL;
        struct
        {
            __IO uint32_t BIT_DEBUG_SEL: 12;
            __I uint32_t RESERVED_2: 4;
            __IO uint32_t pke_clk_always_disable: 1;
            __IO uint32_t pke_clk_always_enable: 1;
            __IO uint32_t ppe_clk_always_disable: 1;
            __IO uint32_t ppe_clk_always_enable: 1;
            __I uint32_t RESERVED_1: 9;
            __IO uint32_t spi_speed_up_sim: 1;
            __I uint32_t RESERVED_0: 1;
            __IO uint32_t uartlog_sir_inv: 1;
        } BITS_01C;
    } u_01C;

    /* 0x0020       0x4000_6020
        0       R/W    BIT_I2C0_TX_BIT_SWAP_EN             0
        1       R/W    BIT_I2C0_RX_BIT_SWAP_EN             0
        2       R/W    BIT_I2C1_TX_BIT_SWAP_EN             0
        3       R/W    BIT_I2C1_RX_BIT_SWAP_EN             0
        4       R/W    BIT_I2C2_TX_BIT_SWAP_EN             0
        5       R/W    BIT_I2C2_RX_BIT_SWAP_EN             0
        7:6     R      RSVD                                0
        8       R/W    BYTE_SPI0_TX_BYTE_SWAP_EN           0
        9       R/W    BYTE_SPI0_RX_BYTE_SWAP_EN           0
        15:10   R      RSVD                                0
        16      R/W    BIT_SPI0_TX_BIT_SWAP_EN             0
        17      R/W    BIT_SPI0_RX_BIT_SWAP_EN             0
        28:18   R      RSVD                                0
        29      W1O    dummy                               0
        30      W1O    reg_efuse_wp                        0
        31      W1O    reg_efuse_rp                        0
    */
    union
    {
        __IO uint32_t REG_DATA_FIFO_SWAP_CTRL;
        struct
        {
            __IO uint32_t BIT_I2C0_TX_BIT_SWAP_EN: 1;
            __IO uint32_t BIT_I2C0_RX_BIT_SWAP_EN: 1;
            __IO uint32_t BIT_I2C1_TX_BIT_SWAP_EN: 1;
            __IO uint32_t BIT_I2C1_RX_BIT_SWAP_EN: 1;
            __IO uint32_t BIT_I2C2_TX_BIT_SWAP_EN: 1;
            __IO uint32_t BIT_I2C2_RX_BIT_SWAP_EN: 1;
            __I uint32_t RESERVED_3: 2;
            __IO uint32_t BYTE_SPI0_TX_BYTE_SWAP_EN: 1;
            __IO uint32_t BYTE_SPI0_RX_BYTE_SWAP_EN: 1;
            __I uint32_t RESERVED_2: 6;
            __IO uint32_t BIT_SPI0_TX_BIT_SWAP_EN: 1;
            __IO uint32_t BIT_SPI0_RX_BIT_SWAP_EN: 1;
            __I uint32_t RESERVED_1: 11;
            __IO uint32_t RESERVED_0: 1;
            __IO uint32_t reg_efuse_wp: 1;
            __IO uint32_t reg_efuse_rp: 1;
        } BITS_020;
    } u_020;

    /* 0x0024       0x4000_6024
        5:0     R/W    reg_timer1_9_4_dma_en               0
        7:6     R      RSVD                                0
        8       R      RSVD                                0
        9       R      RSVD                                0
        10      R/W    reg_fwpi_enable                     1
        11      R/W    spic_icg_disable                    0
        12      R/W    ic_icg_disable                      0
        13      R/W    dmac_clk_always_disable             0
        14      R/W    dmac_clk_always_enable              0
        15      R/W    rxi300_auto_icg_en                  1
        23:16   R      RSVD                                0
        24      R/W    spic0_mem_wr_en                     0
        25      R/W    spic1_mem_wr_en                     0
        26      R/W    spic2_mem_wr_en                     0
        27      R/W    spic3_mem_wr_en                     0
        28      R/W    psram_mem_wr_en                     0
        30:29   R      RSVD                                0
        31      R/W    dsp_dma_int_mask_n                  1
    */
    union
    {
        __IO uint32_t REG_DMAC_CLK_CTRL;
        struct
        {
            __IO uint32_t reg_timer1_9_4_dma_en: 6;
            __I uint32_t RESERVED_4: 2;
            __I uint32_t RESERVED_3: 1;
            __I uint32_t RESERVED_2: 1;
            __IO uint32_t reg_fwpi_enable: 1;
            __IO uint32_t spic_icg_disable: 1;
            __IO uint32_t ic_icg_disable: 1;
            __IO uint32_t dmac_clk_always_disable: 1;
            __IO uint32_t dmac_clk_always_enable: 1;
            __IO uint32_t rxi300_auto_icg_en: 1;
            __I uint32_t RESERVED_1: 8;
            __IO uint32_t spic0_mem_wr_en: 1;
            __IO uint32_t spic1_mem_wr_en: 1;
            __IO uint32_t spic2_mem_wr_en: 1;
            __IO uint32_t spic3_mem_wr_en: 1;
            __IO uint32_t psram_mem_wr_en: 1;
            __I uint32_t RESERVED_0: 2;
            __IO uint32_t dsp_dma_int_mask_n: 1;
        } BITS_024;
    } u_024;

    /* 0x0028       0x4000_6028
        31:0    R      reserved                            0
    */
    __IO uint32_t REG_0x0028;

    /* 0x002C       0x4000_602c
        0       R/W    r_hw_alu_en                         0
        3:1     R/W    r_hw_alu_mode                       0
        4       R/W    r_hw_alu_trig                       0
        30:5    R      reserved                            0
        31      R      hw_alu_done                         0
    */
    union
    {
        __IO uint32_t REG_CORDIC_CTRL;
        struct
        {
            __IO uint32_t r_hw_alu_en: 1;
            __IO uint32_t r_hw_alu_mode: 3;
            __IO uint32_t r_hw_alu_trig: 1;
            __I uint32_t RESERVED_0: 26;
            __I uint32_t hw_alu_done: 1;
        } BITS_02C;
    } u_02C;

    /* 0x0030       0x4000_6030
        15:0    R/W    r_hw_alu_in_a                       0
        31:16   R/W    r_hw_alu_in_b                       0
    */
    union
    {
        __IO uint32_t REG_CORDIC_IN0;
        struct
        {
            __IO uint32_t r_hw_alu_in_a: 16;
            __IO uint32_t r_hw_alu_in_b: 16;
        } BITS_030;
    } u_030;

    /* 0x0034       0x4000_6034
        15:0    R/W    r_hw_alu_in_c                       0
        31:16   R/W    r_hw_alu_in_d                       0
    */
    union
    {
        __IO uint32_t REG_CORDIC_IN1;
        struct
        {
            __IO uint32_t r_hw_alu_in_c: 16;
            __IO uint32_t r_hw_alu_in_d: 16;
        } BITS_034;
    } u_034;

    /* 0x0038       0x4000_6038
        15:0    R      hw_alu_out_a                        0
        31:16   R      hw_alu_out_b                        0
    */
    union
    {
        __IO uint32_t REG_CORDIC_OUT0;
        struct
        {
            __I uint32_t hw_alu_out_a: 16;
            __I uint32_t hw_alu_out_b: 16;
        } BITS_038;
    } u_038;

    /* 0x003C       0x4000_603c
        15:0    R      hw_alu_out_c                        0
        31:16   R      hw_alu_out_d                        0
    */
    union
    {
        __IO uint32_t REG_CORDIC_OUT1;
        struct
        {
            __I uint32_t hw_alu_out_c: 16;
            __I uint32_t hw_alu_out_d: 16;
        } BITS_03C;
    } u_03C;

    __IO uint32_t RSVD_0x40[20];

    /* 0x0090       0x4000_6090
        31:0    R/W    OUT_DATA0                           0
    */
    union
    {
        __IO uint32_t REG_I2C_MAILBOX_OUT_DATA0;
        struct
        {
            __IO uint32_t OUT_DATA0: 32;
        } BITS_090;
    } u_090;

    /* 0x0094       0x4000_6094
        31:0    R/W    OUT_DATA1                           0
    */
    union
    {
        __IO uint32_t REG_I2C_MAILBOX_OUT_DATA1;
        struct
        {
            __IO uint32_t OUT_DATA1: 32;
        } BITS_094;
    } u_094;

    /* 0x0098       0x4000_6098
        31:0    R      IN_DATA0                            0
    */
    union
    {
        __IO uint32_t REG_I2C_MAILBOX_IN_DATA0;
        struct
        {
            __I uint32_t IN_DATA0: 32;
        } BITS_098;
    } u_098;

    /* 0x009C       0x4000_609c
        31:0    R      IN_DATA1                            0
    */
    union
    {
        __IO uint32_t REG_I2C_MAILBOX_IN_DATA1;
        struct
        {
            __I uint32_t IN_DATA1: 32;
        } BITS_09C;
    } u_09C;

    /* 0x00A0       0x4000_60a0
        0       R/W1C  outbox_rdy_r                        0
        1       R/W    out_empty_inten                     0
        2       R/W    out_int_mode                        0
        3       R/W1C  out_edge_sts                        0
        4       R      rx_data_crc_ok                      0
        15:5    R      RSVD                                0
        16      R/W1C  inbox_rdy_r                         0
        17      R/W    in_rdy_inten                        0
        30:18   R      RSVD                                0
        31      R/W    pta_i2c_en                          0
    */
    union
    {
        __IO uint32_t REG_I2C_MAILBOX_CTRL;
        struct
        {
            __IO uint32_t outbox_rdy_r: 1;
            __IO uint32_t out_empty_inten: 1;
            __IO uint32_t out_int_mode: 1;
            __IO uint32_t out_edge_sts: 1;
            __I uint32_t rx_data_crc_ok: 1;
            __I uint32_t RESERVED_1: 11;
            __IO uint32_t inbox_rdy_r: 1;
            __IO uint32_t in_rdy_inten: 1;
            __I uint32_t RESERVED_0: 13;
            __IO uint32_t pta_i2c_en: 1;
        } BITS_0A0;
    } u_0A0;

    /* 0x00A4       0x4000_60a4
        31:0    R      RSVD                                0
    */
    __IO uint32_t REG_MCU_BT_TIME_STAMP;

    /* 0x00A8       0x4000_60a8
        31:0    R      RSVD                                0
    */
    __IO uint32_t REG_MCU_BT_CLOCK_COMPARE;

    /* 0x00AC       0x4000_60ac
        7:0     R/W    BIT_EFUSE_BURN_GNT                  8'h0
        23:8    R      RSVD                                16'h0
        28:24   R/W    EF_SCAN_END[8:4]                    0
        31:29   R      RSVD                                3'h0
    */
    union
    {
        __IO uint32_t REG_EFUSE_PGPWD;
        struct
        {
            __IO uint32_t BIT_EFUSE_BURN_GNT: 8;
            __I uint32_t RESERVED_1: 16;
            __IO uint32_t EF_SCAN_END_8_4: 5;
            __I uint32_t RESERVED_0: 3;
        } BITS_0AC;
    } u_0AC;

    /* 0x00B0       0x4000_60b0
        7:0     R/W    Data7-0                             8'h0
        16:8    R/W    RegAddr8-0                          9'h0
        17      R      RSVD                                0
        18      R/W    Alden                               0
        19      R      RSVD                                0
        23:20   R/W    setup_time                          4'h6
        26:24   R/W    VDDQ_time                           3'h1
        27      R/W    RD_time                             0
        30:28   R/W    PG_time
                                3'h2
        31      R/W    Flag                                0
    */
    union
    {
        __IO uint32_t REG_EFUSE_CTRL;
        struct
        {
            __IO uint32_t Data7_0: 8;
            __IO uint32_t RegAddr8_0: 9;
            __I uint32_t RESERVED_1: 1;
            __IO uint32_t Alden: 1;
            __I uint32_t RESERVED_0: 1;
            __IO uint32_t setup_time: 4;
            __IO uint32_t VDDQ_time: 3;
            __IO uint32_t RD_time: 1;
            __IO uint32_t PG_time
            : 3;
            __IO uint32_t Flag: 1;
        } BITS_0B0;
    } u_0B0;

    /* 0x00B4       0x4000_60b4
        6:0     R/WAC  Thres6-0                            7'h0
        7       R/W    EFSCAN_FAIL                         0
        9:8     R/W    EFCELL_SEL                          2'b0
        10      R/W    r_PWC_EV2EF                         0
        11      R/W    EFPGMEN_FORCE                       0
        15:12   R/W    EF_SCAN_EADR                        0
        24:16   R/W    EF_SCAN_SADR                        0
        25      R/W    EFPD_SEL                            0
        26      R/W    EFCRES_SEL                          0
        31:27   R/W    DUMMY                               5'b00011
    */
    union
    {
        __IO uint32_t REG_EFUSE_TEST;
        struct
        {
            uint32_t Thres6_0: 7;
            __IO uint32_t EFSCAN_FAIL: 1;
            __IO uint32_t EFCELL_SEL: 2;
            __IO uint32_t r_PWC_EV2EF: 1;
            __IO uint32_t EFPGMEN_FORCE: 1;
            __IO uint32_t EF_SCAN_EADR: 4;
            __IO uint32_t EF_SCAN_SADR: 9;
            __IO uint32_t EFPD_SEL: 1;
            __IO uint32_t EFCRES_SEL: 1;
            __IO uint32_t RESERVED_0: 5;
        } BITS_0B4;
    } u_0B4;

    __IO uint32_t RSVD_0xb8[19];

    /* 0x0104       0x4000_6104
        31:0    R      RSVD                                0
    */
    __IO uint32_t REG_0x0104;

    __IO uint32_t RSVD_0x108[16];

    /* 0x0148       0x4000_6148
        0       RW     reg_cm33_cfgfpu                     1'h1
        1       RW     reg_cm33_cfgdsp                     1'h1
        2       RW     reg_cm33_cfgsecext                  1'h0
        3       RW     reg_cm33_mpunsdisable               1'h0
        4       RW     reg_cm33_mpusdisable                1'h1
        5       RW     reg_cm33_saudisable                 1'h1
        6       RW     reg_cm33_locksvtaircr               1'h1
        7       RW     reg_cm33_locknsvtor                 1'h1
        8       RW     reg_cm33_locksmpu                   1'h1
        9       RW     reg_cm33_locknsmpu                  1'h1
        10      RW     reg_cm33_locksau                    1'h1
        29:11   R      RSVD                                19'h0
        30      R      reg_cm33_wicenack                   1'h1
        31      RW     reg_cm33_wicenreq                   1'h1
    */
    union
    {
        __IO uint32_t REG_CM33_CFG_CTRL;
        struct
        {
            __IO uint32_t reg_cm33_cfgfpu: 1;
            __IO uint32_t reg_cm33_cfgdsp: 1;
            __IO uint32_t reg_cm33_cfgsecext: 1;
            __IO uint32_t reg_cm33_mpunsdisable: 1;
            __IO uint32_t reg_cm33_mpusdisable: 1;
            __IO uint32_t reg_cm33_saudisable: 1;
            __IO uint32_t reg_cm33_locksvtaircr: 1;
            __IO uint32_t reg_cm33_locknsvtor: 1;
            __IO uint32_t reg_cm33_locksmpu: 1;
            __IO uint32_t reg_cm33_locknsmpu: 1;
            __IO uint32_t reg_cm33_locksau: 1;
            __I uint32_t RESERVED_0: 19;
            __I uint32_t reg_cm33_wicenack: 1;
            __IO uint32_t reg_cm33_wicenreq: 1;
        } BITS_148;
    } u_148;

    /* 0x014c       0x4000_614c
        24:0    R/W    reg_cm33_initsvtor                  25'h0000000
        31:25   R      RSVD                                7'h0
    */
    union
    {
        __IO uint32_t REG_CM33_INITSVTOR;
        struct
        {
            __IO uint32_t reg_cm33_initsvtor: 25;
            __I uint32_t RESERVED_0: 7;
        } BITS_14c;
    } u_14c;

    /* 0x0150       0x4000_6150
        24:0    R/W    reg_cm33_initnsvtor                 25'h0000000
        31:25   R      RSVD                                7'h0
    */
    union
    {
        __IO uint32_t REG_CM33_INITSVTOR;
        struct
        {
            __IO uint32_t reg_cm33_initnsvtor: 25;
            __I uint32_t RESERVED_0: 7;
        } BITS_150;
    } u_150;

    /* 0x0154       0x4000_6154
        7:0     R      reg_cm33_currpri                    8'h0
        16:8    R      reg_cm33_intnum                     9'h0
        19:17   R      reg_cm33_codehint                   3'h0
        20      R      reg_cm33_currns                     1'h0
        21      R      reg_cm33_lockup                     1'h0
        22      R      reg_cm33_txev                       1'h0
        23      R      reg_cm33_halted                     1'h0
        24      R      reg_cm33_dbgrestarted               1'h1
        25      R      reg_cm33_fpixc                      1'h0
        26      R      reg_cm33_fpidc                      1'h0
        27      R      reg_cm33_fpofc                      1'h0
        28      R      reg_cm33_fpufc                      1'h0
        29      R      reg_cm33_fpdzc                      1'h0
        30      R      reg_cm33_fpioc                      1'h0
        31      R      RSVD                                1'h0
    */
    union
    {
        __IO uint32_t REG_CM33_STS;
        struct
        {
            __I uint32_t reg_cm33_currpri: 8;
            __I uint32_t reg_cm33_intnum: 9;
            __I uint32_t reg_cm33_codehint: 3;
            __I uint32_t reg_cm33_currns: 1;
            __I uint32_t reg_cm33_lockup: 1;
            __I uint32_t reg_cm33_txev: 1;
            __I uint32_t reg_cm33_halted: 1;
            __I uint32_t reg_cm33_dbgrestarted: 1;
            __I uint32_t reg_cm33_fpixc: 1;
            __I uint32_t reg_cm33_fpidc: 1;
            __I uint32_t reg_cm33_fpofc: 1;
            __I uint32_t reg_cm33_fpufc: 1;
            __I uint32_t reg_cm33_fpdzc: 1;
            __I uint32_t reg_cm33_fpioc: 1;
            __I uint32_t RESERVED_0: 1;
        } BITS_154;
    } u_154;

    /* 0x0158       0x4000_6158
        31:0    R      reg_cm33_version                    32'hCF020001
    */
    union
    {
        __IO uint32_t REG_CM33_VERSION;
        struct
        {
            __I uint32_t reg_cm33_version: 32;
        } BITS_158;
    } u_158;

    __IO uint32_t RSVD_0x15c[73];

    /* 0x0280       0x4000_6280
        9:0     R/W    EFUSE_WP_START                      10'h1a6
        11:10   R      RSVD                                2'h0
        21:12   R/W    EFUSE_WP_END                        10'h1ff
        29:22   R      RSVD                                8'h0
        30      W1O    EFUSE_WP_LOCK                       1'h0
        31      R/W    EFUSE_WP_EN                         1'h0
    */
    union
    {
        __IO uint32_t efuse_write_protect;
        struct
        {
            __IO uint32_t EFUSE_WP_START: 10;
            __I uint32_t RESERVED_1: 2;
            __IO uint32_t EFUSE_WP_END: 10;
            __I uint32_t RESERVED_0: 8;
            __IO uint32_t EFUSE_WP_LOCK: 1;
            __IO uint32_t EFUSE_WP_EN: 1;
        } BITS_280;
    } u_280;

    /* 0x0284       0x4000_6284
        9:0     R/W    EFUSE_RP_START                      10'h1a6
        11:10   R      RSVD                                2'h0
        21:12   R/W    EFUSE_RP_END                        10'h1ff
        29:22   R      RSVD                                8'h0
        30      W1O    EFUSE_RP_LOCK                       1'h0
        31      R/W    EFUSE_RP_EN                         1'h0
    */
    union
    {
        __IO uint32_t efuse_read_protect;
        struct
        {
            __IO uint32_t EFUSE_RP_START: 10;
            __I uint32_t RESERVED_1: 2;
            __IO uint32_t EFUSE_RP_END: 10;
            __I uint32_t RESERVED_0: 8;
            __IO uint32_t EFUSE_RP_LOCK: 1;
            __IO uint32_t EFUSE_RP_EN: 1;
        } BITS_284;
    } u_284;

    /* 0x0288       0x4000_6288
        0       R/W    data_ram_err_flag_en                1'h0
        1       R/W    data_ram_err_int_en                 1'h0
        2       R/W    data_ram_rd_flag_clr                1'h0
        3       R/W    data_ram_err_int_clr                1'h0
        31:4    R      RSVD                                28'h0
    */
    union
    {
        __IO uint32_t REG_DATA_RAM_ERR_0;
        struct
        {
            __IO uint32_t data_ram_err_flag_en: 1;
            __IO uint32_t data_ram_err_int_en: 1;
            __IO uint32_t data_ram_rd_flag_clr: 1;
            __IO uint32_t data_ram_err_int_clr: 1;
            __I uint32_t RESERVED_0: 28;
        } BITS_288;
    } u_288;

    /* 0x028C       0x4000_628c
        17:0    R      data_ram_err_flag                   18'h0
        18      R      data_ram_err_int                    1'h0
        31:19   R      RSVD                                13'h0
    */
    union
    {
        __IO uint32_t REG_DATA_RAM_ERR_1;
        struct
        {
            __I uint32_t data_ram_err_flag: 18;
            __I uint32_t data_ram_err_int: 1;
            __I uint32_t RESERVED_0: 13;
        } BITS_28C;
    } u_28C;

    __IO uint32_t RSVD_0x290[28];

    /* 0x0300       0x4000_6300
        0       R/W    SYS_TIMER_RST_SEL                   1'h1
        1       R/W    SYS_TIMER_MANUAL_RSTB               1'h0
        31:2    R      RSVD                                30'h0
    */
    union
    {
        __IO uint32_t REG_SYS_TIMER_CTL;
        struct
        {
            __IO uint32_t SYS_TIMER_RST_SEL: 1;
            __IO uint32_t SYS_TIMER_MANUAL_RSTB: 1;
            __I uint32_t RESERVED_0: 30;
        } BITS_300;
    } u_300;

    /* 0x0304       0x4000_6304
        31:0    R      SYS_TIMER_CNT                       32'h0
    */
    union
    {
        __IO uint32_t REG_SYS_TIMER_CNT;
        struct
        {
            __I uint32_t SYS_TIMER_CNT: 32;
        } BITS_304;
    } u_304;

    /* 0x0308       0x4000_6308
        31:0    R      RSVD                                32'h0
    */
    __IO uint32_t REG_SYS_TIMER_RSVD;

    /* 0x030C       0x4000_630c
        31:0    R      SYS_TIMER_VERSION                   32'h2306270A
    */
    union
    {
        __IO uint32_t REG_SYS_TIMER_VERSION;
        struct
        {
            __I uint32_t SYS_TIMER_VERSION: 32;
        } BITS_30C;
    } u_30C;

    /* 0x310        0x4000_6310
        7:0     R/W    timer1_9_gpio_trig_sel              8'hFF
        23:8    R/W    timer1_9_gpio_trig_deb_cnt_limit    16'h1
        24      R/W    timer1_9_gpio_trig_deb_en           1'h0
        25      R      timer1_9_gpio_trig_deb_sts          1'h0
        31:26   R      RSVD                                6'h0
    */
    union
    {
        __IO uint32_t REG_TIMER1_9_GPIO_TRIG_CTRL;
        struct
        {
            __IO uint32_t timer1_9_gpio_trig_sel: 8;
            __IO uint32_t timer1_9_gpio_trig_deb_cnt_limit: 16;
            __IO uint32_t timer1_9_gpio_trig_deb_en: 1;
            __I uint32_t timer1_9_gpio_trig_deb_sts: 1;
            __I uint32_t RESERVED_0: 6;
        } BITS_310;
    } u_310;

    __IO uint32_t RSVD_0x314[123];

    /* 0x500        0x4000_6500
        0       R/W    cm33_rom_bist_rstn                  1'h0
        1       R/W    cm33_rom_bist_mode                  1'h0
        30:2    R      reserved                            29'h0
        31      R      cm33_rom_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_0;
        struct
        {
            __IO uint32_t cm33_rom_bist_rstn: 1;
            __IO uint32_t cm33_rom_bist_mode: 1;
            __I uint32_t RESERVED_0: 29;
            __I uint32_t cm33_rom_bist_done: 1;
        } BITS_500;
    } u_500;

    /* 0x504        0x4000_6504
        11:0    R/W    cm33_rom_bist_grp_en                12'h0
        31:12   R      reserved                            20'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_1;
        struct
        {
            __IO uint32_t cm33_rom_bist_grp_en: 12;
            __I uint32_t RESERVED_0: 20;
        } BITS_504;
    } u_504;

    /* 0x508        0x4000_6508
        0       R/W    pke_imem_bist_rstn                  1'h0
        1       R/W    pke_imem_bist_mode                  1'h0
        30:2    R      reserved                            29'h0
        31      R      pke_imem_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_2;
        struct
        {
            __IO uint32_t pke_imem_bist_rstn: 1;
            __IO uint32_t pke_imem_bist_mode: 1;
            __I uint32_t RESERVED_0: 29;
            __I uint32_t pke_imem_bist_done: 1;
        } BITS_508;
    } u_508;

    /* 0x50c        0x4000_650c
        0:0     R/W    pke_imem_bist_grp_en                1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_3;
        struct
        {
            __IO uint32_t pke_imem_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_50c;
    } u_50c;

    /* 0x510        0x4000_6510
        0       R/W    dsp_rom_bist_rstn                   1'h0
        1       R/W    dsp_rom_bist_mode                   1'h0
        30:2    R      reserved                            29'h0
        31      R      dsp_rom_bist_done                   1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_4;
        struct
        {
            __IO uint32_t dsp_rom_bist_rstn: 1;
            __IO uint32_t dsp_rom_bist_mode: 1;
            __I uint32_t RESERVED_0: 29;
            __I uint32_t dsp_rom_bist_done: 1;
        } BITS_510;
    } u_510;

    /* 0x514        0x4000_6514
        9:0     R/W    dsp_rom_bist_grp_en                 10'h0
        31:10   R      reserved                            22'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_5;
        struct
        {
            __IO uint32_t dsp_rom_bist_grp_en: 10;
            __I uint32_t RESERVED_0: 22;
        } BITS_514;
    } u_514;

    __IO uint32_t RSVD_0x518[10];

    /* 0x540        0x4000_6540
        31:0    R      cm33_rom_misr_dataout_0_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_0;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_0_31_00: 32;
        } BITS_540;
    } u_540;

    /* 0x544        0x4000_6544
        31:0    R      cm33_rom_misr_dataout_0_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_1;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_0_63_32: 32;
        } BITS_544;
    } u_544;

    /* 0x548        0x4000_6548
        31:0    R      cm33_rom_misr_dataout_1_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_2;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_1_31_00: 32;
        } BITS_548;
    } u_548;

    /* 0x54c        0x4000_654c
        31:0    R      cm33_rom_misr_dataout_1_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_3;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_1_63_32: 32;
        } BITS_54c;
    } u_54c;

    /* 0x550        0x4000_6550
        31:0    R      cm33_rom_misr_dataout_2_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_4;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_2_31_00: 32;
        } BITS_550;
    } u_550;

    /* 0x554        0x4000_6554
        31:0    R      cm33_rom_misr_dataout_2_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_5;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_2_63_32: 32;
        } BITS_554;
    } u_554;

    /* 0x558        0x4000_6558
        31:0    R      cm33_rom_misr_dataout_3_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_6;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_3_31_00: 32;
        } BITS_558;
    } u_558;

    /* 0x55c        0x4000_655c
        31:0    R      cm33_rom_misr_dataout_3_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_7;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_3_63_32: 32;
        } BITS_55c;
    } u_55c;

    /* 0x560        0x4000_6560
        31:0    R      cm33_rom_misr_dataout_4_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_8;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_4_31_00: 32;
        } BITS_560;
    } u_560;

    /* 0x564        0x4000_6564
        31:0    R      cm33_rom_misr_dataout_4_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_9;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_4_63_32: 32;
        } BITS_564;
    } u_564;

    /* 0x568        0x4000_6568
        31:0    R      cm33_rom_misr_dataout_5_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_10;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_5_31_00: 32;
        } BITS_568;
    } u_568;

    /* 0x56c        0x4000_656c
        31:0    R      cm33_rom_misr_dataout_5_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_11;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_5_63_32: 32;
        } BITS_56c;
    } u_56c;

    /* 0x570        0x4000_6570
        31:0    R      cm33_rom_misr_dataout_6_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_12;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_6_31_00: 32;
        } BITS_570;
    } u_570;

    /* 0x574        0x4000_6574
        31:0    R      cm33_rom_misr_dataout_6_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_13;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_6_63_32: 32;
        } BITS_574;
    } u_574;

    /* 0x578        0x4000_6578
        31:0    R      cm33_rom_misr_dataout_7_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_14;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_7_31_00: 32;
        } BITS_578;
    } u_578;

    /* 0x57c        0x4000_657c
        31:0    R      cm33_rom_misr_dataout_7_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_15;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_7_63_32: 32;
        } BITS_57c;
    } u_57c;

    /* 0x580        0x4000_6580
        31:0    R      cm33_rom_misr_dataout_8_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_16;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_8_31_00: 32;
        } BITS_580;
    } u_580;

    /* 0x584        0x4000_6584
        31:0    R      cm33_rom_misr_dataout_8_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_17;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_8_63_32: 32;
        } BITS_584;
    } u_584;

    /* 0x588        0x4000_6588
        31:0    R      cm33_rom_misr_dataout_9_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_18;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_9_31_00: 32;
        } BITS_588;
    } u_588;

    /* 0x58c        0x4000_658c
        31:0    R      cm33_rom_misr_dataout_9_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_19;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_9_63_32: 32;
        } BITS_58c;
    } u_58c;

    /* 0x590        0x4000_6590
        31:0    R      cm33_rom_misr_dataout_10_31_00      32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_20;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_10_31_00: 32;
        } BITS_590;
    } u_590;

    /* 0x594        0x4000_6594
        31:0    R      cm33_rom_misr_dataout_10_63_32      32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_21;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_10_63_32: 32;
        } BITS_594;
    } u_594;

    /* 0x598        0x4000_6598
        31:0    R      cm33_rom_misr_dataout_11_31_00      32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_22;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_11_31_00: 32;
        } BITS_598;
    } u_598;

    /* 0x59c        0x4000_659c
        31:0    R      cm33_rom_misr_dataout_11_63_32      32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_23;
        struct
        {
            __I uint32_t cm33_rom_misr_dataout_11_63_32: 32;
        } BITS_59c;
    } u_59c;

    /* 0x5a0        0x4000_65a0
        31:0    R      pke_imem_misr_dataout_0_31_00       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_24;
        struct
        {
            __I uint32_t pke_imem_misr_dataout_0_31_00: 32;
        } BITS_5a0;
    } u_5a0;

    /* 0x5a4        0x4000_65a4
        31:0    R      pke_imem_misr_dataout_0_63_32       32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_25;
        struct
        {
            __I uint32_t pke_imem_misr_dataout_0_63_32: 32;
        } BITS_5a4;
    } u_5a4;

    /* 0x5a8        0x4000_65a8
        31:0    R      dsp_rom_misr_dataout_0_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_26;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_0_31_00: 32;
        } BITS_5a8;
    } u_5a8;

    /* 0x5ac        0x4000_65ac
        31:0    R      dsp_rom_misr_dataout_0_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_27;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_0_63_32: 32;
        } BITS_5ac;
    } u_5ac;

    /* 0x5b0        0x4000_65b0
        31:0    R      dsp_rom_misr_dataout_1_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_28;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_1_31_00: 32;
        } BITS_5b0;
    } u_5b0;

    /* 0x5b4        0x4000_65b4
        31:0    R      dsp_rom_misr_dataout_1_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_29;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_1_63_32: 32;
        } BITS_5b4;
    } u_5b4;

    /* 0x5b8        0x4000_65b8
        31:0    R      dsp_rom_misr_dataout_2_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_30;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_2_31_00: 32;
        } BITS_5b8;
    } u_5b8;

    /* 0x5bc        0x4000_65bc
        31:0    R      dsp_rom_misr_dataout_2_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_31;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_2_63_32: 32;
        } BITS_5bc;
    } u_5bc;

    /* 0x5c0        0x4000_65c0
        31:0    R      dsp_rom_misr_dataout_3_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_32;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_3_31_00: 32;
        } BITS_5c0;
    } u_5c0;

    /* 0x5c4        0x4000_65c4
        31:0    R      dsp_rom_misr_dataout_3_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_33;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_3_63_32: 32;
        } BITS_5c4;
    } u_5c4;

    /* 0x5c8        0x4000_65c8
        31:0    R      dsp_rom_misr_dataout_4_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_34;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_4_31_00: 32;
        } BITS_5c8;
    } u_5c8;

    /* 0x5cc        0x4000_65cc
        31:0    R      dsp_rom_misr_dataout_4_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_35;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_4_63_32: 32;
        } BITS_5cc;
    } u_5cc;

    /* 0x5d0        0x4000_65d0
        31:0    R      dsp_rom_misr_dataout_5_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_36;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_5_31_00: 32;
        } BITS_5d0;
    } u_5d0;

    /* 0x5d4        0x4000_65d4
        31:0    R      dsp_rom_misr_dataout_5_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_37;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_5_63_32: 32;
        } BITS_5d4;
    } u_5d4;

    /* 0x5d8        0x4000_65d8
        31:0    R      dsp_rom_misr_dataout_6_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_38;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_6_31_00: 32;
        } BITS_5d8;
    } u_5d8;

    /* 0x5dc        0x4000_65dc
        31:0    R      dsp_rom_misr_dataout_6_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_39;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_6_63_32: 32;
        } BITS_5dc;
    } u_5dc;

    /* 0x5e0        0x4000_65e0
        31:0    R      dsp_rom_misr_dataout_7_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_40;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_7_31_00: 32;
        } BITS_5e0;
    } u_5e0;

    /* 0x5e4        0x4000_65e4
        31:0    R      dsp_rom_misr_dataout_7_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_41;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_7_63_32: 32;
        } BITS_5e4;
    } u_5e4;

    /* 0x5e8        0x4000_65e8
        31:0    R      dsp_rom_misr_dataout_8_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_42;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_8_31_00: 32;
        } BITS_5e8;
    } u_5e8;

    /* 0x5ec        0x4000_65ec
        31:0    R      dsp_rom_misr_dataout_8_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_43;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_8_63_32: 32;
        } BITS_5ec;
    } u_5ec;

    /* 0x5f0        0x4000_65f0
        31:0    R      dsp_rom_misr_dataout_9_31_00        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_44;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_9_31_00: 32;
        } BITS_5f0;
    } u_5f0;

    /* 0x5f4        0x4000_65f4
        31:0    R      dsp_rom_misr_dataout_9_63_32        32'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_ROM_DATA_45;
        struct
        {
            __I uint32_t dsp_rom_misr_dataout_9_63_32: 32;
        } BITS_5f4;
    } u_5f4;

    __IO uint32_t RSVD_0x5f8[2];

    /* 0x600        0x4000_6600
        0       R/W    cm33_ram_bist_rstn                  1'h0
        1       R/W    cm33_ram_bist_mode                  1'h0
        2       R/W    cm33_ram_drf_bist_mode              1'h0
        3       R/W    cm33_ram_drf_test_resume            1'h0
        4       R/W    cm33_ram_bist_loop_mode             1'h0
        5       R/W    cm33_ram_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      cm33_ram_drf_start_pause            1'h0
        30      R      cm33_ram_drf_bist_done              1'h0
        31      R      cm33_ram_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_0;
        struct
        {
            __IO uint32_t cm33_ram_bist_rstn: 1;
            __IO uint32_t cm33_ram_bist_mode: 1;
            __IO uint32_t cm33_ram_drf_bist_mode: 1;
            __IO uint32_t cm33_ram_drf_test_resume: 1;
            __IO uint32_t cm33_ram_bist_loop_mode: 1;
            __IO uint32_t cm33_ram_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t cm33_ram_drf_start_pause: 1;
            __I uint32_t cm33_ram_drf_bist_done: 1;
            __I uint32_t cm33_ram_bist_done: 1;
        } BITS_600;
    } u_600;

    /* 0x604        0x4000_6604
        7:0     R/W    cm33_ram_bist_grp_en                8'h0
        31:8    R      reserved                            24'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_1;
        struct
        {
            __IO uint32_t cm33_ram_bist_grp_en: 8;
            __I uint32_t RESERVED_0: 24;
        } BITS_604;
    } u_604;

    /* 0x608        0x4000_6608
        7:0     R      cm33_ram_bist_fail                  8'h0
        31:8    R      reserved                            24'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_2;
        struct
        {
            __I uint32_t cm33_ram_bist_fail: 8;
            __I uint32_t RESERVED_0: 24;
        } BITS_608;
    } u_608;

    /* 0x60c        0x4000_660c
        7:0     R      cm33_ram_drf_bist_fail              8'h0
        31:8    R      reserved                            24'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_3;
        struct
        {
            __I uint32_t cm33_ram_drf_bist_fail: 8;
            __I uint32_t RESERVED_0: 24;
        } BITS_60c;
    } u_60c;

    /* 0x610        0x4000_6610
        0       R/W    cm33_mem_bist_rstn                  1'h0
        1       R/W    cm33_mem_bist_mode                  1'h0
        2       R/W    cm33_mem_drf_bist_mode              1'h0
        3       R/W    cm33_mem_drf_test_resume            1'h0
        4       R/W    cm33_mem_bist_loop_mode             1'h0
        5       R/W    cm33_mem_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      cm33_mem_drf_start_pause            1'h0
        30      R      cm33_mem_drf_bist_done              1'h0
        31      R      cm33_mem_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_4;
        struct
        {
            __IO uint32_t cm33_mem_bist_rstn: 1;
            __IO uint32_t cm33_mem_bist_mode: 1;
            __IO uint32_t cm33_mem_drf_bist_mode: 1;
            __IO uint32_t cm33_mem_drf_test_resume: 1;
            __IO uint32_t cm33_mem_bist_loop_mode: 1;
            __IO uint32_t cm33_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t cm33_mem_drf_start_pause: 1;
            __I uint32_t cm33_mem_drf_bist_done: 1;
            __I uint32_t cm33_mem_bist_done: 1;
        } BITS_610;
    } u_610;

    /* 0x614        0x4000_6614
        9:0     R/W    cm33_mem_bist_grp_en                10'h0
        31:10   R      reserved                            22'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_5;
        struct
        {
            __IO uint32_t cm33_mem_bist_grp_en: 10;
            __I uint32_t RESERVED_0: 22;
        } BITS_614;
    } u_614;

    /* 0x618        0x4000_6618
        9:0     R      cm33_mem_bist_fail                  10'h0
        31:10   R      reserved                            22'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_6;
        struct
        {
            __I uint32_t cm33_mem_bist_fail: 10;
            __I uint32_t RESERVED_0: 22;
        } BITS_618;
    } u_618;

    /* 0x61c        0x4000_661c
        9:0     R      cm33_mem_drf_bist_fail              10'h0
        31:10   R      reserved                            22'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_7;
        struct
        {
            __I uint32_t cm33_mem_drf_bist_fail: 10;
            __I uint32_t RESERVED_0: 22;
        } BITS_61c;
    } u_61c;

    /* 0x620        0x4000_6620
        0       R/W    data_sram0_bist_rstn                1'h0
        1       R/W    data_sram0_bist_mode                1'h0
        2       R/W    data_sram0_drf_bist_mode            1'h0
        3       R/W    data_sram0_drf_test_resume          1'h0
        4       R/W    data_sram0_bist_loop_mode           1'h0
        5       R/W    data_sram0_dyn_read_en              1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      data_sram0_drf_start_pause          1'h0
        30      R      data_sram0_drf_bist_done            1'h0
        31      R      data_sram0_bist_done                1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_8;
        struct
        {
            __IO uint32_t data_sram0_bist_rstn: 1;
            __IO uint32_t data_sram0_bist_mode: 1;
            __IO uint32_t data_sram0_drf_bist_mode: 1;
            __IO uint32_t data_sram0_drf_test_resume: 1;
            __IO uint32_t data_sram0_bist_loop_mode: 1;
            __IO uint32_t data_sram0_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t data_sram0_drf_start_pause: 1;
            __I uint32_t data_sram0_drf_bist_done: 1;
            __I uint32_t data_sram0_bist_done: 1;
        } BITS_620;
    } u_620;

    /* 0x624        0x4000_6624
        7:0     R/W    data_sram0_bist_grp_en              8'h0
        31:8    R      reserved                            24'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_9;
        struct
        {
            __IO uint32_t data_sram0_bist_grp_en: 8;
            __I uint32_t RESERVED_0: 24;
        } BITS_624;
    } u_624;

    /* 0x628        0x4000_6628
        7:0     R      data_sram0_bist_fail                8'h0
        31:8    R      reserved                            24'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_10;
        struct
        {
            __I uint32_t data_sram0_bist_fail: 8;
            __I uint32_t RESERVED_0: 24;
        } BITS_628;
    } u_628;

    /* 0x62c        0x4000_662c
        7:0     R      data_sram0_drf_bist_fail            8'h0
        31:8    R      reserved                            24'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_11;
        struct
        {
            __I uint32_t data_sram0_drf_bist_fail: 8;
            __I uint32_t RESERVED_0: 24;
        } BITS_62c;
    } u_62c;

    /* 0x630        0x4000_6630
        0       R/W    buffer_ram_bist_rstn                1'h0
        1       R/W    buffer_ram_bist_mode                1'h0
        2       R/W    buffer_ram_drf_bist_mode            1'h0
        3       R/W    buffer_ram_drf_test_resume          1'h0
        4       R/W    buffer_ram_bist_loop_mode           1'h0
        5       R/W    buffer_ram_dyn_read_en              1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      buffer_ram_drf_start_pause          1'h0
        30      R      buffer_ram_drf_bist_done            1'h0
        31      R      buffer_ram_bist_done                1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_12;
        struct
        {
            __IO uint32_t buffer_ram_bist_rstn: 1;
            __IO uint32_t buffer_ram_bist_mode: 1;
            __IO uint32_t buffer_ram_drf_bist_mode: 1;
            __IO uint32_t buffer_ram_drf_test_resume: 1;
            __IO uint32_t buffer_ram_bist_loop_mode: 1;
            __IO uint32_t buffer_ram_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t buffer_ram_drf_start_pause: 1;
            __I uint32_t buffer_ram_drf_bist_done: 1;
            __I uint32_t buffer_ram_bist_done: 1;
        } BITS_630;
    } u_630;

    /* 0x634        0x4000_6634
        3:0     R/W    buffer_ram_bist_grp_en              4'h0
        31:4    R      reserved                            28'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_13;
        struct
        {
            __IO uint32_t buffer_ram_bist_grp_en: 4;
            __I uint32_t RESERVED_0: 28;
        } BITS_634;
    } u_634;

    /* 0x638        0x4000_6638
        3:0     R      buffer_ram_bist_fail                4'h0
        31:4    R      reserved                            28'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_14;
        struct
        {
            __I uint32_t buffer_ram_bist_fail: 4;
            __I uint32_t RESERVED_0: 28;
        } BITS_638;
    } u_638;

    /* 0x63c        0x4000_663c
        3:0     R      buffer_ram_drf_bist_fail            4'h0
        31:4    R      reserved                            28'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_15;
        struct
        {
            __I uint32_t buffer_ram_drf_bist_fail: 4;
            __I uint32_t RESERVED_0: 28;
        } BITS_63c;
    } u_63c;

    /* 0x640        0x4000_6640
        0       R/W    icache_bist_rstn                    1'h0
        1       R/W    icache_bist_mode                    1'h0
        2       R/W    icache_drf_bist_mode                1'h0
        3       R/W    icache_drf_test_resume              1'h0
        4       R/W    icache_bist_loop_mode               1'h0
        5       R/W    icache_dyn_read_en                  1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      icache_drf_start_pause              1'h0
        30      R      icache_drf_bist_done                1'h0
        31      R      icache_bist_done                    1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_16;
        struct
        {
            __IO uint32_t icache_bist_rstn: 1;
            __IO uint32_t icache_bist_mode: 1;
            __IO uint32_t icache_drf_bist_mode: 1;
            __IO uint32_t icache_drf_test_resume: 1;
            __IO uint32_t icache_bist_loop_mode: 1;
            __IO uint32_t icache_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t icache_drf_start_pause: 1;
            __I uint32_t icache_drf_bist_done: 1;
            __I uint32_t icache_bist_done: 1;
        } BITS_640;
    } u_640;

    /* 0x644        0x4000_6644
        8:0     R/W    icache_bist_grp_en                  9'h0
        31:9    R      reserved                            23'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_17;
        struct
        {
            __IO uint32_t icache_bist_grp_en: 9;
            __I uint32_t RESERVED_0: 23;
        } BITS_644;
    } u_644;

    /* 0x648        0x4000_6648
        8:0     R      icache_bist_fail                    9'h0
        31:9    R      reserved                            23'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_18;
        struct
        {
            __I uint32_t icache_bist_fail: 9;
            __I uint32_t RESERVED_0: 23;
        } BITS_648;
    } u_648;

    /* 0x64c        0x4000_664c
        8:0     R      icache_drf_bist_fail                9'h0
        31:9    R      reserved                            23'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_19;
        struct
        {
            __I uint32_t icache_drf_bist_fail: 9;
            __I uint32_t RESERVED_0: 23;
        } BITS_64c;
    } u_64c;

    /* 0x650        0x4000_6650
        0       R/W    arm_ahb_cache_bist_rstn             1'h0
        1       R/W    arm_ahb_cache_bist_mode             1'h0
        2       R/W    arm_ahb_cache_drf_bist_mode         1'h0
        3       R/W    arm_ahb_cache_drf_test_resume       1'h0
        4       R/W    arm_ahb_cache_bist_loop_mode        1'h0
        5       R/W    arm_ahb_cache_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      arm_ahb_cache_drf_start_pause       1'h0
        30      R      arm_ahb_cache_drf_bist_done         1'h0
        31      R      arm_ahb_cache_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_20;
        struct
        {
            __IO uint32_t arm_ahb_cache_bist_rstn: 1;
            __IO uint32_t arm_ahb_cache_bist_mode: 1;
            __IO uint32_t arm_ahb_cache_drf_bist_mode: 1;
            __IO uint32_t arm_ahb_cache_drf_test_resume: 1;
            __IO uint32_t arm_ahb_cache_bist_loop_mode: 1;
            __IO uint32_t arm_ahb_cache_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t arm_ahb_cache_drf_start_pause: 1;
            __I uint32_t arm_ahb_cache_drf_bist_done: 1;
            __I uint32_t arm_ahb_cache_bist_done: 1;
        } BITS_650;
    } u_650;

    /* 0x654        0x4000_6654
        8:0     R/W    arm_ahb_cache_bist_grp_en           9'h0
        31:9    R      reserved                            23'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_21;
        struct
        {
            __IO uint32_t arm_ahb_cache_bist_grp_en: 9;
            __I uint32_t RESERVED_0: 23;
        } BITS_654;
    } u_654;

    /* 0x658        0x4000_6658
        8:0     R      arm_ahb_cache_bist_fail             9'h0
        31:9    R      reserved                            23'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_22;
        struct
        {
            __I uint32_t arm_ahb_cache_bist_fail: 9;
            __I uint32_t RESERVED_0: 23;
        } BITS_658;
    } u_658;

    /* 0x65c        0x4000_665c
        8:0     R      arm_ahb_cache_drf_bist_fail         9'h0
        31:9    R      reserved                            23'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_23;
        struct
        {
            __I uint32_t arm_ahb_cache_drf_bist_fail: 9;
            __I uint32_t RESERVED_0: 23;
        } BITS_65c;
    } u_65c;

    /* 0x660        0x4000_6660
        0       R/W    pke_mmem_bist_rstn                  1'h0
        1       R/W    pke_mmem_bist_mode                  1'h0
        2       R/W    pke_mmem_drf_bist_mode              1'h0
        3       R/W    pke_mmem_drf_test_resume            1'h0
        4       R/W    pke_mmem_bist_loop_mode             1'h0
        5       R/W    pke_mmem_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      pke_mmem_drf_start_pause            1'h0
        30      R      pke_mmem_drf_bist_done              1'h0
        31      R      pke_mmem_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_24;
        struct
        {
            __IO uint32_t pke_mmem_bist_rstn: 1;
            __IO uint32_t pke_mmem_bist_mode: 1;
            __IO uint32_t pke_mmem_drf_bist_mode: 1;
            __IO uint32_t pke_mmem_drf_test_resume: 1;
            __IO uint32_t pke_mmem_bist_loop_mode: 1;
            __IO uint32_t pke_mmem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t pke_mmem_drf_start_pause: 1;
            __I uint32_t pke_mmem_drf_bist_done: 1;
            __I uint32_t pke_mmem_bist_done: 1;
        } BITS_660;
    } u_660;

    /* 0x664        0x4000_6664
        1:0     R/W    pke_mmem_bist_grp_en                2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_25;
        struct
        {
            __IO uint32_t pke_mmem_bist_grp_en: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_664;
    } u_664;

    /* 0x668        0x4000_6668
        1:0     R      pke_mmem_bist_fail                  2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_26;
        struct
        {
            __I uint32_t pke_mmem_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_668;
    } u_668;

    /* 0x66c        0x4000_666c
        1:0     R      pke_mmem_drf_bist_fail              2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_27;
        struct
        {
            __I uint32_t pke_mmem_drf_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_66c;
    } u_66c;

    /* 0x670        0x4000_6670
        0       R/W    pke_tmem_bist_rstn                  1'h0
        1       R/W    pke_tmem_bist_mode                  1'h0
        2       R/W    pke_tmem_drf_bist_mode              1'h0
        3       R/W    pke_tmem_drf_test_resume            1'h0
        4       R/W    pke_tmem_bist_loop_mode             1'h0
        5       R/W    pke_tmem_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      pke_tmem_drf_start_pause            1'h0
        30      R      pke_tmem_drf_bist_done              1'h0
        31      R      pke_tmem_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_28;
        struct
        {
            __IO uint32_t pke_tmem_bist_rstn: 1;
            __IO uint32_t pke_tmem_bist_mode: 1;
            __IO uint32_t pke_tmem_drf_bist_mode: 1;
            __IO uint32_t pke_tmem_drf_test_resume: 1;
            __IO uint32_t pke_tmem_bist_loop_mode: 1;
            __IO uint32_t pke_tmem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t pke_tmem_drf_start_pause: 1;
            __I uint32_t pke_tmem_drf_bist_done: 1;
            __I uint32_t pke_tmem_bist_done: 1;
        } BITS_670;
    } u_670;

    /* 0x674        0x4000_6674
        1:0     R/W    pke_tmem_bist_grp_en                2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_29;
        struct
        {
            __IO uint32_t pke_tmem_bist_grp_en: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_674;
    } u_674;

    /* 0x678        0x4000_6678
        1:0     R      pke_tmem_bist_fail                  2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_30;
        struct
        {
            __I uint32_t pke_tmem_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_678;
    } u_678;

    /* 0x67c        0x4000_667c
        1:0     R      pke_tmem_drf_bist_fail              2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_31;
        struct
        {
            __I uint32_t pke_tmem_drf_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_67c;
    } u_67c;

    /* 0x680        0x4000_6680
        0       R/W    dsp_ram0_bist_rstn                  1'h0
        1       R/W    dsp_ram0_bist_mode                  1'h0
        2       R/W    dsp_ram0_drf_bist_mode              1'h0
        3       R/W    dsp_ram0_drf_test_resume            1'h0
        4       R/W    dsp_ram0_bist_loop_mode             1'h0
        5       R/W    dsp_ram0_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      dsp_ram0_drf_start_pause            1'h0
        30      R      dsp_ram0_drf_bist_done              1'h0
        31      R      dsp_ram0_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_32;
        struct
        {
            __IO uint32_t dsp_ram0_bist_rstn: 1;
            __IO uint32_t dsp_ram0_bist_mode: 1;
            __IO uint32_t dsp_ram0_drf_bist_mode: 1;
            __IO uint32_t dsp_ram0_drf_test_resume: 1;
            __IO uint32_t dsp_ram0_bist_loop_mode: 1;
            __IO uint32_t dsp_ram0_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t dsp_ram0_drf_start_pause: 1;
            __I uint32_t dsp_ram0_drf_bist_done: 1;
            __I uint32_t dsp_ram0_bist_done: 1;
        } BITS_680;
    } u_680;

    /* 0x684        0x4000_6684
        13:0    R/W    dsp_ram0_bist_grp_en                14'h0
        31:14   R      reserved                            18'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_33;
        struct
        {
            __IO uint32_t dsp_ram0_bist_grp_en: 14;
            __I uint32_t RESERVED_0: 18;
        } BITS_684;
    } u_684;

    /* 0x688        0x4000_6688
        13:0    R      dsp_ram0_bist_fail                  14'h0
        31:14   R      reserved                            18'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_34;
        struct
        {
            __I uint32_t dsp_ram0_bist_fail: 14;
            __I uint32_t RESERVED_0: 18;
        } BITS_688;
    } u_688;

    /* 0x68c        0x4000_668c
        13:0    R      dsp_ram0_drf_bist_fail              14'h0
        31:14   R      reserved                            18'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_35;
        struct
        {
            __I uint32_t dsp_ram0_drf_bist_fail: 14;
            __I uint32_t RESERVED_0: 18;
        } BITS_68c;
    } u_68c;

    /* 0x690        0x4000_6690
        0       R/W    dsp_ram1_bist_rstn                  1'h0
        1       R/W    dsp_ram1_bist_mode                  1'h0
        2       R/W    dsp_ram1_drf_bist_mode              1'h0
        3       R/W    dsp_ram1_drf_test_resume            1'h0
        4       R/W    dsp_ram1_bist_loop_mode             1'h0
        5       R/W    dsp_ram1_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      dsp_ram1_drf_start_pause            1'h0
        30      R      dsp_ram1_drf_bist_done              1'h0
        31      R      dsp_ram1_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_36;
        struct
        {
            __IO uint32_t dsp_ram1_bist_rstn: 1;
            __IO uint32_t dsp_ram1_bist_mode: 1;
            __IO uint32_t dsp_ram1_drf_bist_mode: 1;
            __IO uint32_t dsp_ram1_drf_test_resume: 1;
            __IO uint32_t dsp_ram1_bist_loop_mode: 1;
            __IO uint32_t dsp_ram1_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t dsp_ram1_drf_start_pause: 1;
            __I uint32_t dsp_ram1_drf_bist_done: 1;
            __I uint32_t dsp_ram1_bist_done: 1;
        } BITS_690;
    } u_690;

    /* 0x694        0x4000_6694
        13:0    R/W    dsp_ram1_bist_grp_en                14'h0
        31:14   R      reserved                            18'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_37;
        struct
        {
            __IO uint32_t dsp_ram1_bist_grp_en: 14;
            __I uint32_t RESERVED_0: 18;
        } BITS_694;
    } u_694;

    /* 0x698        0x4000_6698
        13:0    R      dsp_ram1_bist_fail                  14'h0
        31:14   R      reserved                            18'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_38;
        struct
        {
            __I uint32_t dsp_ram1_bist_fail: 14;
            __I uint32_t RESERVED_0: 18;
        } BITS_698;
    } u_698;

    /* 0x69c        0x4000_669c
        13:0    R      dsp_ram1_drf_bist_fail              14'h0
        31:14   R      reserved                            18'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_39;
        struct
        {
            __I uint32_t dsp_ram1_drf_bist_fail: 14;
            __I uint32_t RESERVED_0: 18;
        } BITS_69c;
    } u_69c;

    /* 0x6a0        0x4000_66a0
        0       R/W    sdio_ram_bist_rstn                  1'h0
        1       R/W    sdio_ram_bist_mode                  1'h0
        2       R/W    sdio_ram_drf_bist_mode              1'h0
        3       R/W    sdio_ram_drf_test_resume            1'h0
        4       R/W    sdio_ram_bist_loop_mode             1'h0
        5       R/W    sdio_ram_dyn_read_en                1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      sdio_ram_drf_start_pause            1'h0
        30      R      sdio_ram_drf_bist_done              1'h0
        31      R      sdio_ram_bist_done                  1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_40;
        struct
        {
            __IO uint32_t sdio_ram_bist_rstn: 1;
            __IO uint32_t sdio_ram_bist_mode: 1;
            __IO uint32_t sdio_ram_drf_bist_mode: 1;
            __IO uint32_t sdio_ram_drf_test_resume: 1;
            __IO uint32_t sdio_ram_bist_loop_mode: 1;
            __IO uint32_t sdio_ram_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t sdio_ram_drf_start_pause: 1;
            __I uint32_t sdio_ram_drf_bist_done: 1;
            __I uint32_t sdio_ram_bist_done: 1;
        } BITS_6a0;
    } u_6a0;

    /* 0x6a4        0x4000_66a4
        0:0     R/W    sdio_ram_bist_grp_en                1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_41;
        struct
        {
            __IO uint32_t sdio_ram_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6a4;
    } u_6a4;

    /* 0x6a8        0x4000_66a8
        0:0     R      sdio_ram_bist_fail                  1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_42;
        struct
        {
            __I uint32_t sdio_ram_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6a8;
    } u_6a8;

    /* 0x6ac        0x4000_66ac
        0:0     R      sdio_ram_drf_bist_fail              1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_43;
        struct
        {
            __I uint32_t sdio_ram_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6ac;
    } u_6ac;

    /* 0x6b0        0x4000_66b0
        0       R/W    can_ram_0_bist_rstn                 1'h0
        1       R/W    can_ram_0_bist_mode                 1'h0
        2       R/W    can_ram_0_drf_bist_mode             1'h0
        3       R/W    can_ram_0_drf_test_resume           1'h0
        4       R/W    can_ram_0_bist_loop_mode            1'h0
        5       R/W    can_ram_0_dyn_read_en               1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      can_ram_0_drf_start_pause           1'h0
        30      R      can_ram_0_drf_bist_done             1'h0
        31      R      can_ram_0_bist_done                 1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_44;
        struct
        {
            __IO uint32_t can_ram_0_bist_rstn: 1;
            __IO uint32_t can_ram_0_bist_mode: 1;
            __IO uint32_t can_ram_0_drf_bist_mode: 1;
            __IO uint32_t can_ram_0_drf_test_resume: 1;
            __IO uint32_t can_ram_0_bist_loop_mode: 1;
            __IO uint32_t can_ram_0_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t can_ram_0_drf_start_pause: 1;
            __I uint32_t can_ram_0_drf_bist_done: 1;
            __I uint32_t can_ram_0_bist_done: 1;
        } BITS_6b0;
    } u_6b0;

    /* 0x6b4        0x4000_66b4
        0:0     R/W    can_ram_0_bist_grp_en               1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_45;
        struct
        {
            __IO uint32_t can_ram_0_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6b4;
    } u_6b4;

    /* 0x6b8        0x4000_66b8
        0:0     R      can_ram_0_bist_fail                 1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_46;
        struct
        {
            __I uint32_t can_ram_0_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6b8;
    } u_6b8;

    /* 0x6bc        0x4000_66bc
        0:0     R      can_ram_0_drf_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_47;
        struct
        {
            __I uint32_t can_ram_0_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6bc;
    } u_6bc;

    /* 0x6c0        0x4000_66c0
        0       R/W    can_ram_1_bist_rstn                 1'h0
        1       R/W    can_ram_1_bist_mode                 1'h0
        2       R/W    can_ram_1_drf_bist_mode             1'h0
        3       R/W    can_ram_1_drf_test_resume           1'h0
        4       R/W    can_ram_1_bist_loop_mode            1'h0
        5       R/W    can_ram_1_dyn_read_en               1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      can_ram_1_drf_start_pause           1'h0
        30      R      can_ram_1_drf_bist_done             1'h0
        31      R      can_ram_1_bist_done                 1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_48;
        struct
        {
            __IO uint32_t can_ram_1_bist_rstn: 1;
            __IO uint32_t can_ram_1_bist_mode: 1;
            __IO uint32_t can_ram_1_drf_bist_mode: 1;
            __IO uint32_t can_ram_1_drf_test_resume: 1;
            __IO uint32_t can_ram_1_bist_loop_mode: 1;
            __IO uint32_t can_ram_1_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t can_ram_1_drf_start_pause: 1;
            __I uint32_t can_ram_1_drf_bist_done: 1;
            __I uint32_t can_ram_1_bist_done: 1;
        } BITS_6c0;
    } u_6c0;

    /* 0x6c4        0x4000_66c4
        0:0     R/W    can_ram_1_bist_grp_en               1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_49;
        struct
        {
            __IO uint32_t can_ram_1_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6c4;
    } u_6c4;

    /* 0x6c8        0x4000_66c8
        0:0     R      can_ram_1_bist_fail                 1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_50;
        struct
        {
            __I uint32_t can_ram_1_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6c8;
    } u_6c8;

    /* 0x6cc        0x4000_66cc
        0:0     R      can_ram_1_drf_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_51;
        struct
        {
            __I uint32_t can_ram_1_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6cc;
    } u_6cc;

    /* 0x6d0        0x4000_66d0
        0       R/W    display_128x32_bist_rstn            1'h0
        1       R/W    display_128x32_bist_mode            1'h0
        2       R/W    display_128x32_drf_bist_mode        1'h0
        3       R/W    display_128x32_drf_test_resume      1'h0
        4       R/W    display_128x32_bist_loop_mode       1'h0
        5       R/W    display_128x32_dyn_read_en          1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      display_128x32_drf_start_pause      1'h0
        30      R      display_128x32_drf_bist_done        1'h0
        31      R      display_128x32_bist_done            1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_52;
        struct
        {
            __IO uint32_t display_128x32_bist_rstn: 1;
            __IO uint32_t display_128x32_bist_mode: 1;
            __IO uint32_t display_128x32_drf_bist_mode: 1;
            __IO uint32_t display_128x32_drf_test_resume: 1;
            __IO uint32_t display_128x32_bist_loop_mode: 1;
            __IO uint32_t display_128x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t display_128x32_drf_start_pause: 1;
            __I uint32_t display_128x32_drf_bist_done: 1;
            __I uint32_t display_128x32_bist_done: 1;
        } BITS_6d0;
    } u_6d0;

    /* 0x6d4        0x4000_66d4
        0:0     R/W    display_128x32_bist_grp_en          1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_53;
        struct
        {
            __IO uint32_t display_128x32_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6d4;
    } u_6d4;

    /* 0x6d8        0x4000_66d8
        0:0     R      display_128x32_bist_fail            1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_54;
        struct
        {
            __I uint32_t display_128x32_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6d8;
    } u_6d8;

    /* 0x6dc        0x4000_66dc
        0:0     R      display_128x32_drf_bist_fail        1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_55;
        struct
        {
            __I uint32_t display_128x32_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6dc;
    } u_6dc;

    /* 0x6e0        0x4000_66e0
        0       R/W    display_672x24_bist_rstn            1'h0
        1       R/W    display_672x24_bist_mode            1'h0
        2       R/W    display_672x24_drf_bist_mode        1'h0
        3       R/W    display_672x24_drf_test_resume      1'h0
        4       R/W    display_672x24_bist_loop_mode       1'h0
        5       R/W    display_672x24_dyn_read_en          1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      display_672x24_drf_start_pause      1'h0
        30      R      display_672x24_drf_bist_done        1'h0
        31      R      display_672x24_bist_done            1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_56;
        struct
        {
            __IO uint32_t display_672x24_bist_rstn: 1;
            __IO uint32_t display_672x24_bist_mode: 1;
            __IO uint32_t display_672x24_drf_bist_mode: 1;
            __IO uint32_t display_672x24_drf_test_resume: 1;
            __IO uint32_t display_672x24_bist_loop_mode: 1;
            __IO uint32_t display_672x24_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t display_672x24_drf_start_pause: 1;
            __I uint32_t display_672x24_drf_bist_done: 1;
            __I uint32_t display_672x24_bist_done: 1;
        } BITS_6e0;
    } u_6e0;

    /* 0x6e4        0x4000_66e4
        0:0     R/W    display_672x24_bist_grp_en          1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_57;
        struct
        {
            __IO uint32_t display_672x24_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6e4;
    } u_6e4;

    /* 0x6e8        0x4000_66e8
        0:0     R      display_672x24_bist_fail            1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_58;
        struct
        {
            __I uint32_t display_672x24_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6e8;
    } u_6e8;

    /* 0x6ec        0x4000_66ec
        0:0     R      display_672x24_drf_bist_fail        1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_59;
        struct
        {
            __I uint32_t display_672x24_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6ec;
    } u_6ec;

    /* 0x6f0        0x4000_66f0
        0       R/W    jpeg_tc_mem_bist_rstn               1'h0
        1       R/W    jpeg_tc_mem_bist_mode               1'h0
        2       R/W    jpeg_tc_mem_drf_bist_mode           1'h0
        3       R/W    jpeg_tc_mem_drf_test_resume         1'h0
        4       R/W    jpeg_tc_mem_bist_loop_mode          1'h0
        5       R/W    jpeg_tc_mem_dyn_read_en             1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      jpeg_tc_mem_drf_start_pause         1'h0
        30      R      jpeg_tc_mem_drf_bist_done           1'h0
        31      R      jpeg_tc_mem_bist_done               1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_60;
        struct
        {
            __IO uint32_t jpeg_tc_mem_bist_rstn: 1;
            __IO uint32_t jpeg_tc_mem_bist_mode: 1;
            __IO uint32_t jpeg_tc_mem_drf_bist_mode: 1;
            __IO uint32_t jpeg_tc_mem_drf_test_resume: 1;
            __IO uint32_t jpeg_tc_mem_bist_loop_mode: 1;
            __IO uint32_t jpeg_tc_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t jpeg_tc_mem_drf_start_pause: 1;
            __I uint32_t jpeg_tc_mem_drf_bist_done: 1;
            __I uint32_t jpeg_tc_mem_bist_done: 1;
        } BITS_6f0;
    } u_6f0;

    /* 0x6f4        0x4000_66f4
        0:0     R/W    jpeg_tc_mem_bist_grp_en             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_61;
        struct
        {
            __IO uint32_t jpeg_tc_mem_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6f4;
    } u_6f4;

    /* 0x6f8        0x4000_66f8
        0:0     R      jpeg_tc_mem_bist_fail               1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_62;
        struct
        {
            __I uint32_t jpeg_tc_mem_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6f8;
    } u_6f8;

    /* 0x6fc        0x4000_66fc
        0:0     R      jpeg_tc_mem_drf_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_63;
        struct
        {
            __I uint32_t jpeg_tc_mem_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_6fc;
    } u_6fc;

    /* 0x700        0x4000_6700
        0       R/W    jpeg_mb_mem_bist_rstn               1'h0
        1       R/W    jpeg_mb_mem_bist_mode               1'h0
        2       R/W    jpeg_mb_mem_drf_bist_mode           1'h0
        3       R/W    jpeg_mb_mem_drf_test_resume         1'h0
        4       R/W    jpeg_mb_mem_bist_loop_mode          1'h0
        5       R/W    jpeg_mb_mem_dyn_read_en             1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      jpeg_mb_mem_drf_start_pause         1'h0
        30      R      jpeg_mb_mem_drf_bist_done           1'h0
        31      R      jpeg_mb_mem_bist_done               1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_64;
        struct
        {
            __IO uint32_t jpeg_mb_mem_bist_rstn: 1;
            __IO uint32_t jpeg_mb_mem_bist_mode: 1;
            __IO uint32_t jpeg_mb_mem_drf_bist_mode: 1;
            __IO uint32_t jpeg_mb_mem_drf_test_resume: 1;
            __IO uint32_t jpeg_mb_mem_bist_loop_mode: 1;
            __IO uint32_t jpeg_mb_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t jpeg_mb_mem_drf_start_pause: 1;
            __I uint32_t jpeg_mb_mem_drf_bist_done: 1;
            __I uint32_t jpeg_mb_mem_bist_done: 1;
        } BITS_700;
    } u_700;

    /* 0x704        0x4000_6704
        0:0     R/W    jpeg_mb_mem_bist_grp_en             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_65;
        struct
        {
            __IO uint32_t jpeg_mb_mem_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_704;
    } u_704;

    /* 0x708        0x4000_6708
        0:0     R      jpeg_mb_mem_bist_fail               1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_66;
        struct
        {
            __I uint32_t jpeg_mb_mem_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_708;
    } u_708;

    /* 0x70c        0x4000_670c
        0:0     R      jpeg_mb_mem_drf_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_67;
        struct
        {
            __I uint32_t jpeg_mb_mem_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_70c;
    } u_70c;

    /* 0x710        0x4000_6710
        0       R/W    jpeg_pp_mem_bist_rstn               1'h0
        1       R/W    jpeg_pp_mem_bist_mode               1'h0
        2       R/W    jpeg_pp_mem_drf_bist_mode           1'h0
        3       R/W    jpeg_pp_mem_drf_test_resume         1'h0
        4       R/W    jpeg_pp_mem_bist_loop_mode          1'h0
        5       R/W    jpeg_pp_mem_dyn_read_en             1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      jpeg_pp_mem_drf_start_pause         1'h0
        30      R      jpeg_pp_mem_drf_bist_done           1'h0
        31      R      jpeg_pp_mem_bist_done               1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_68;
        struct
        {
            __IO uint32_t jpeg_pp_mem_bist_rstn: 1;
            __IO uint32_t jpeg_pp_mem_bist_mode: 1;
            __IO uint32_t jpeg_pp_mem_drf_bist_mode: 1;
            __IO uint32_t jpeg_pp_mem_drf_test_resume: 1;
            __IO uint32_t jpeg_pp_mem_bist_loop_mode: 1;
            __IO uint32_t jpeg_pp_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t jpeg_pp_mem_drf_start_pause: 1;
            __I uint32_t jpeg_pp_mem_drf_bist_done: 1;
            __I uint32_t jpeg_pp_mem_bist_done: 1;
        } BITS_710;
    } u_710;

    /* 0x714        0x4000_6714
        0:0     R/W    jpeg_pp_mem_bist_grp_en             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_69;
        struct
        {
            __IO uint32_t jpeg_pp_mem_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_714;
    } u_714;

    /* 0x718        0x4000_6718
        0:0     R      jpeg_pp_mem_bist_fail               1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_70;
        struct
        {
            __I uint32_t jpeg_pp_mem_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_718;
    } u_718;

    /* 0x71c        0x4000_671c
        0:0     R      jpeg_pp_mem_drf_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_71;
        struct
        {
            __I uint32_t jpeg_pp_mem_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_71c;
    } u_71c;

    /* 0x720        0x4000_6720
        0       R/W    jpeg_huff_mem_bist_rstn             1'h0
        1       R/W    jpeg_huff_mem_bist_mode             1'h0
        2       R/W    jpeg_huff_mem_drf_bist_mode         1'h0
        3       R/W    jpeg_huff_mem_drf_test_resume       1'h0
        4       R/W    jpeg_huff_mem_bist_loop_mode        1'h0
        5       R/W    jpeg_huff_mem_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      jpeg_huff_mem_drf_start_pause       1'h0
        30      R      jpeg_huff_mem_drf_bist_done         1'h0
        31      R      jpeg_huff_mem_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_72;
        struct
        {
            __IO uint32_t jpeg_huff_mem_bist_rstn: 1;
            __IO uint32_t jpeg_huff_mem_bist_mode: 1;
            __IO uint32_t jpeg_huff_mem_drf_bist_mode: 1;
            __IO uint32_t jpeg_huff_mem_drf_test_resume: 1;
            __IO uint32_t jpeg_huff_mem_bist_loop_mode: 1;
            __IO uint32_t jpeg_huff_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t jpeg_huff_mem_drf_start_pause: 1;
            __I uint32_t jpeg_huff_mem_drf_bist_done: 1;
            __I uint32_t jpeg_huff_mem_bist_done: 1;
        } BITS_720;
    } u_720;

    /* 0x724        0x4000_6724
        0:0     R/W    jpeg_huff_mem_bist_grp_en           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_73;
        struct
        {
            __IO uint32_t jpeg_huff_mem_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_724;
    } u_724;

    /* 0x728        0x4000_6728
        0:0     R      jpeg_huff_mem_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_74;
        struct
        {
            __I uint32_t jpeg_huff_mem_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_728;
    } u_728;

    /* 0x72c        0x4000_672c
        0:0     R      jpeg_huff_mem_drf_bist_fail         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_75;
        struct
        {
            __I uint32_t jpeg_huff_mem_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_72c;
    } u_72c;

    /* 0x730        0x4000_6730
        0       R/W    jpeg_qmat_mem_bist_rstn             1'h0
        1       R/W    jpeg_qmat_mem_bist_mode             1'h0
        2       R/W    jpeg_qmat_mem_drf_bist_mode         1'h0
        3       R/W    jpeg_qmat_mem_drf_test_resume       1'h0
        4       R/W    jpeg_qmat_mem_bist_loop_mode        1'h0
        5       R/W    jpeg_qmat_mem_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      jpeg_qmat_mem_drf_start_pause       1'h0
        30      R      jpeg_qmat_mem_drf_bist_done         1'h0
        31      R      jpeg_qmat_mem_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_76;
        struct
        {
            __IO uint32_t jpeg_qmat_mem_bist_rstn: 1;
            __IO uint32_t jpeg_qmat_mem_bist_mode: 1;
            __IO uint32_t jpeg_qmat_mem_drf_bist_mode: 1;
            __IO uint32_t jpeg_qmat_mem_drf_test_resume: 1;
            __IO uint32_t jpeg_qmat_mem_bist_loop_mode: 1;
            __IO uint32_t jpeg_qmat_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t jpeg_qmat_mem_drf_start_pause: 1;
            __I uint32_t jpeg_qmat_mem_drf_bist_done: 1;
            __I uint32_t jpeg_qmat_mem_bist_done: 1;
        } BITS_730;
    } u_730;

    /* 0x734        0x4000_6734
        0:0     R/W    jpeg_qmat_mem_bist_grp_en           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_77;
        struct
        {
            __IO uint32_t jpeg_qmat_mem_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_734;
    } u_734;

    /* 0x738        0x4000_6738
        0:0     R      jpeg_qmat_mem_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_78;
        struct
        {
            __I uint32_t jpeg_qmat_mem_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_738;
    } u_738;

    /* 0x73c        0x4000_673c
        0:0     R      jpeg_qmat_mem_drf_bist_fail         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_79;
        struct
        {
            __I uint32_t jpeg_qmat_mem_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_73c;
    } u_73c;

    /* 0x740        0x4000_6740
        0       R/W    jpeg_coef_mem_bist_rstn             1'h0
        1       R/W    jpeg_coef_mem_bist_mode             1'h0
        2       R/W    jpeg_coef_mem_drf_bist_mode         1'h0
        3       R/W    jpeg_coef_mem_drf_test_resume       1'h0
        4       R/W    jpeg_coef_mem_bist_loop_mode        1'h0
        5       R/W    jpeg_coef_mem_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      jpeg_coef_mem_drf_start_pause       1'h0
        30      R      jpeg_coef_mem_drf_bist_done         1'h0
        31      R      jpeg_coef_mem_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_80;
        struct
        {
            __IO uint32_t jpeg_coef_mem_bist_rstn: 1;
            __IO uint32_t jpeg_coef_mem_bist_mode: 1;
            __IO uint32_t jpeg_coef_mem_drf_bist_mode: 1;
            __IO uint32_t jpeg_coef_mem_drf_test_resume: 1;
            __IO uint32_t jpeg_coef_mem_bist_loop_mode: 1;
            __IO uint32_t jpeg_coef_mem_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t jpeg_coef_mem_drf_start_pause: 1;
            __I uint32_t jpeg_coef_mem_drf_bist_done: 1;
            __I uint32_t jpeg_coef_mem_bist_done: 1;
        } BITS_740;
    } u_740;

    /* 0x744        0x4000_6744
        1:0     R/W    jpeg_coef_mem_bist_grp_en           2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_81;
        struct
        {
            __IO uint32_t jpeg_coef_mem_bist_grp_en: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_744;
    } u_744;

    /* 0x748        0x4000_6748
        1:0     R      jpeg_coef_mem_bist_fail             2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_82;
        struct
        {
            __I uint32_t jpeg_coef_mem_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_748;
    } u_748;

    /* 0x74c        0x4000_674c
        1:0     R      jpeg_coef_mem_drf_bist_fail         2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_83;
        struct
        {
            __I uint32_t jpeg_coef_mem_drf_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_74c;
    } u_74c;

    /* 0x750        0x4000_6750
        0       R/W    bt_mac_1024x16_bist_rstn            1'h0
        1       R/W    bt_mac_1024x16_bist_mode            1'h0
        2       R/W    bt_mac_1024x16_drf_bist_mode        1'h0
        3       R/W    bt_mac_1024x16_drf_test_resume      1'h0
        4       R/W    bt_mac_1024x16_bist_loop_mode       1'h0
        5       R/W    bt_mac_1024x16_dyn_read_en          1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_1024x16_drf_start_pause      1'h0
        30      R      bt_mac_1024x16_drf_bist_done        1'h0
        31      R      bt_mac_1024x16_bist_done            1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_84;
        struct
        {
            __IO uint32_t bt_mac_1024x16_bist_rstn: 1;
            __IO uint32_t bt_mac_1024x16_bist_mode: 1;
            __IO uint32_t bt_mac_1024x16_drf_bist_mode: 1;
            __IO uint32_t bt_mac_1024x16_drf_test_resume: 1;
            __IO uint32_t bt_mac_1024x16_bist_loop_mode: 1;
            __IO uint32_t bt_mac_1024x16_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_1024x16_drf_start_pause: 1;
            __I uint32_t bt_mac_1024x16_drf_bist_done: 1;
            __I uint32_t bt_mac_1024x16_bist_done: 1;
        } BITS_750;
    } u_750;

    /* 0x754        0x4000_6754
        0:0     R/W    bt_mac_1024x16_bist_grp_en          1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_85;
        struct
        {
            __IO uint32_t bt_mac_1024x16_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_754;
    } u_754;

    /* 0x758        0x4000_6758
        0:0     R      bt_mac_1024x16_bist_fail            1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_86;
        struct
        {
            __I uint32_t bt_mac_1024x16_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_758;
    } u_758;

    /* 0x75c        0x4000_675c
        0:0     R      bt_mac_1024x16_drf_bist_fail        1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_87;
        struct
        {
            __I uint32_t bt_mac_1024x16_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_75c;
    } u_75c;

    /* 0x760        0x4000_6760
        0       R/W    bt_mac_512x16_0_bist_rstn           1'h0
        1       R/W    bt_mac_512x16_0_bist_mode           1'h0
        2       R/W    bt_mac_512x16_0_drf_bist_mode       1'h0
        3       R/W    bt_mac_512x16_0_drf_test_resume     1'h0
        4       R/W    bt_mac_512x16_0_bist_loop_mode      1'h0
        5       R/W    bt_mac_512x16_0_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_512x16_0_drf_start_pause     1'h0
        30      R      bt_mac_512x16_0_drf_bist_done       1'h0
        31      R      bt_mac_512x16_0_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_88;
        struct
        {
            __IO uint32_t bt_mac_512x16_0_bist_rstn: 1;
            __IO uint32_t bt_mac_512x16_0_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_0_drf_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_0_drf_test_resume: 1;
            __IO uint32_t bt_mac_512x16_0_bist_loop_mode: 1;
            __IO uint32_t bt_mac_512x16_0_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_512x16_0_drf_start_pause: 1;
            __I uint32_t bt_mac_512x16_0_drf_bist_done: 1;
            __I uint32_t bt_mac_512x16_0_bist_done: 1;
        } BITS_760;
    } u_760;

    /* 0x764        0x4000_6764
        0:0     R/W    bt_mac_512x16_0_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_89;
        struct
        {
            __IO uint32_t bt_mac_512x16_0_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_764;
    } u_764;

    /* 0x768        0x4000_6768
        0:0     R      bt_mac_512x16_0_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_90;
        struct
        {
            __I uint32_t bt_mac_512x16_0_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_768;
    } u_768;

    /* 0x76c        0x4000_676c
        0:0     R      bt_mac_512x16_0_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_91;
        struct
        {
            __I uint32_t bt_mac_512x16_0_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_76c;
    } u_76c;

    /* 0x770        0x4000_6770
        0       R/W    bt_mac_512x16_1_bist_rstn           1'h0
        1       R/W    bt_mac_512x16_1_bist_mode           1'h0
        2       R/W    bt_mac_512x16_1_drf_bist_mode       1'h0
        3       R/W    bt_mac_512x16_1_drf_test_resume     1'h0
        4       R/W    bt_mac_512x16_1_bist_loop_mode      1'h0
        5       R/W    bt_mac_512x16_1_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_512x16_1_drf_start_pause     1'h0
        30      R      bt_mac_512x16_1_drf_bist_done       1'h0
        31      R      bt_mac_512x16_1_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_92;
        struct
        {
            __IO uint32_t bt_mac_512x16_1_bist_rstn: 1;
            __IO uint32_t bt_mac_512x16_1_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_1_drf_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_1_drf_test_resume: 1;
            __IO uint32_t bt_mac_512x16_1_bist_loop_mode: 1;
            __IO uint32_t bt_mac_512x16_1_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_512x16_1_drf_start_pause: 1;
            __I uint32_t bt_mac_512x16_1_drf_bist_done: 1;
            __I uint32_t bt_mac_512x16_1_bist_done: 1;
        } BITS_770;
    } u_770;

    /* 0x774        0x4000_6774
        0:0     R/W    bt_mac_512x16_1_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_93;
        struct
        {
            __IO uint32_t bt_mac_512x16_1_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_774;
    } u_774;

    /* 0x778        0x4000_6778
        0:0     R      bt_mac_512x16_1_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_94;
        struct
        {
            __I uint32_t bt_mac_512x16_1_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_778;
    } u_778;

    /* 0x77c        0x4000_677c
        0:0     R      bt_mac_512x16_1_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_95;
        struct
        {
            __I uint32_t bt_mac_512x16_1_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_77c;
    } u_77c;

    /* 0x780        0x4000_6780
        0       R/W    bt_mac_512x16_2_bist_rstn           1'h0
        1       R/W    bt_mac_512x16_2_bist_mode           1'h0
        2       R/W    bt_mac_512x16_2_drf_bist_mode       1'h0
        3       R/W    bt_mac_512x16_2_drf_test_resume     1'h0
        4       R/W    bt_mac_512x16_2_bist_loop_mode      1'h0
        5       R/W    bt_mac_512x16_2_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_512x16_2_drf_start_pause     1'h0
        30      R      bt_mac_512x16_2_drf_bist_done       1'h0
        31      R      bt_mac_512x16_2_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_96;
        struct
        {
            __IO uint32_t bt_mac_512x16_2_bist_rstn: 1;
            __IO uint32_t bt_mac_512x16_2_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_2_drf_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_2_drf_test_resume: 1;
            __IO uint32_t bt_mac_512x16_2_bist_loop_mode: 1;
            __IO uint32_t bt_mac_512x16_2_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_512x16_2_drf_start_pause: 1;
            __I uint32_t bt_mac_512x16_2_drf_bist_done: 1;
            __I uint32_t bt_mac_512x16_2_bist_done: 1;
        } BITS_780;
    } u_780;

    /* 0x784        0x4000_6784
        0:0     R/W    bt_mac_512x16_2_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_97;
        struct
        {
            __IO uint32_t bt_mac_512x16_2_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_784;
    } u_784;

    /* 0x788        0x4000_6788
        0:0     R      bt_mac_512x16_2_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_98;
        struct
        {
            __I uint32_t bt_mac_512x16_2_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_788;
    } u_788;

    /* 0x78c        0x4000_678c
        0:0     R      bt_mac_512x16_2_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_99;
        struct
        {
            __I uint32_t bt_mac_512x16_2_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_78c;
    } u_78c;

    /* 0x790        0x4000_6790
        0       R/W    bt_mac_512x16_3_bist_rstn           1'h0
        1       R/W    bt_mac_512x16_3_bist_mode           1'h0
        2       R/W    bt_mac_512x16_3_drf_bist_mode       1'h0
        3       R/W    bt_mac_512x16_3_drf_test_resume     1'h0
        4       R/W    bt_mac_512x16_3_bist_loop_mode      1'h0
        5       R/W    bt_mac_512x16_3_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_512x16_3_drf_start_pause     1'h0
        30      R      bt_mac_512x16_3_drf_bist_done       1'h0
        31      R      bt_mac_512x16_3_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_100;
        struct
        {
            __IO uint32_t bt_mac_512x16_3_bist_rstn: 1;
            __IO uint32_t bt_mac_512x16_3_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_3_drf_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_3_drf_test_resume: 1;
            __IO uint32_t bt_mac_512x16_3_bist_loop_mode: 1;
            __IO uint32_t bt_mac_512x16_3_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_512x16_3_drf_start_pause: 1;
            __I uint32_t bt_mac_512x16_3_drf_bist_done: 1;
            __I uint32_t bt_mac_512x16_3_bist_done: 1;
        } BITS_790;
    } u_790;

    /* 0x794        0x4000_6794
        0:0     R/W    bt_mac_512x16_3_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_101;
        struct
        {
            __IO uint32_t bt_mac_512x16_3_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_794;
    } u_794;

    /* 0x798        0x4000_6798
        0:0     R      bt_mac_512x16_3_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_102;
        struct
        {
            __I uint32_t bt_mac_512x16_3_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_798;
    } u_798;

    /* 0x79c        0x4000_679c
        0:0     R      bt_mac_512x16_3_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_103;
        struct
        {
            __I uint32_t bt_mac_512x16_3_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_79c;
    } u_79c;

    /* 0x7a0        0x4000_67a0
        0       R/W    bt_mac_512x16_4_bist_rstn           1'h0
        1       R/W    bt_mac_512x16_4_bist_mode           1'h0
        2       R/W    bt_mac_512x16_4_drf_bist_mode       1'h0
        3       R/W    bt_mac_512x16_4_drf_test_resume     1'h0
        4       R/W    bt_mac_512x16_4_bist_loop_mode      1'h0
        5       R/W    bt_mac_512x16_4_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_512x16_4_drf_start_pause     1'h0
        30      R      bt_mac_512x16_4_drf_bist_done       1'h0
        31      R      bt_mac_512x16_4_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_104;
        struct
        {
            __IO uint32_t bt_mac_512x16_4_bist_rstn: 1;
            __IO uint32_t bt_mac_512x16_4_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_4_drf_bist_mode: 1;
            __IO uint32_t bt_mac_512x16_4_drf_test_resume: 1;
            __IO uint32_t bt_mac_512x16_4_bist_loop_mode: 1;
            __IO uint32_t bt_mac_512x16_4_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_512x16_4_drf_start_pause: 1;
            __I uint32_t bt_mac_512x16_4_drf_bist_done: 1;
            __I uint32_t bt_mac_512x16_4_bist_done: 1;
        } BITS_7a0;
    } u_7a0;

    /* 0x7a4        0x4000_67a4
        0:0     R/W    bt_mac_512x16_4_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_105;
        struct
        {
            __IO uint32_t bt_mac_512x16_4_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7a4;
    } u_7a4;

    /* 0x7a8        0x4000_67a8
        0:0     R      bt_mac_512x16_4_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_106;
        struct
        {
            __I uint32_t bt_mac_512x16_4_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7a8;
    } u_7a8;

    /* 0x7ac        0x4000_67ac
        0:0     R      bt_mac_512x16_4_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_107;
        struct
        {
            __I uint32_t bt_mac_512x16_4_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7ac;
    } u_7ac;

    /* 0x7b0        0x4000_67b0
        0       R/W    bt_mac_128x16_bist_rstn             1'h0
        1       R/W    bt_mac_128x16_bist_mode             1'h0
        2       R/W    bt_mac_128x16_drf_bist_mode         1'h0
        3       R/W    bt_mac_128x16_drf_test_resume       1'h0
        4       R/W    bt_mac_128x16_bist_loop_mode        1'h0
        5       R/W    bt_mac_128x16_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_128x16_drf_start_pause       1'h0
        30      R      bt_mac_128x16_drf_bist_done         1'h0
        31      R      bt_mac_128x16_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_108;
        struct
        {
            __IO uint32_t bt_mac_128x16_bist_rstn: 1;
            __IO uint32_t bt_mac_128x16_bist_mode: 1;
            __IO uint32_t bt_mac_128x16_drf_bist_mode: 1;
            __IO uint32_t bt_mac_128x16_drf_test_resume: 1;
            __IO uint32_t bt_mac_128x16_bist_loop_mode: 1;
            __IO uint32_t bt_mac_128x16_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_128x16_drf_start_pause: 1;
            __I uint32_t bt_mac_128x16_drf_bist_done: 1;
            __I uint32_t bt_mac_128x16_bist_done: 1;
        } BITS_7b0;
    } u_7b0;

    /* 0x7b4        0x4000_67b4
        0:0     R/W    bt_mac_128x16_bist_grp_en           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_109;
        struct
        {
            __IO uint32_t bt_mac_128x16_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7b4;
    } u_7b4;

    /* 0x7b8        0x4000_67b8
        0:0     R      bt_mac_128x16_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_110;
        struct
        {
            __I uint32_t bt_mac_128x16_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7b8;
    } u_7b8;

    /* 0x7bc        0x4000_67bc
        0:0     R      bt_mac_128x16_drf_bist_fail         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_111;
        struct
        {
            __I uint32_t bt_mac_128x16_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7bc;
    } u_7bc;

    /* 0x7c0        0x4000_67c0
        0       R/W    bt_mac_128x32_0_bist_rstn           1'h0
        1       R/W    bt_mac_128x32_0_bist_mode           1'h0
        2       R/W    bt_mac_128x32_0_drf_bist_mode       1'h0
        3       R/W    bt_mac_128x32_0_drf_test_resume     1'h0
        4       R/W    bt_mac_128x32_0_bist_loop_mode      1'h0
        5       R/W    bt_mac_128x32_0_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_128x32_0_drf_start_pause     1'h0
        30      R      bt_mac_128x32_0_drf_bist_done       1'h0
        31      R      bt_mac_128x32_0_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_112;
        struct
        {
            __IO uint32_t bt_mac_128x32_0_bist_rstn: 1;
            __IO uint32_t bt_mac_128x32_0_bist_mode: 1;
            __IO uint32_t bt_mac_128x32_0_drf_bist_mode: 1;
            __IO uint32_t bt_mac_128x32_0_drf_test_resume: 1;
            __IO uint32_t bt_mac_128x32_0_bist_loop_mode: 1;
            __IO uint32_t bt_mac_128x32_0_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_128x32_0_drf_start_pause: 1;
            __I uint32_t bt_mac_128x32_0_drf_bist_done: 1;
            __I uint32_t bt_mac_128x32_0_bist_done: 1;
        } BITS_7c0;
    } u_7c0;

    /* 0x7c4        0x4000_67c4
        0:0     R/W    bt_mac_128x32_0_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_113;
        struct
        {
            __IO uint32_t bt_mac_128x32_0_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7c4;
    } u_7c4;

    /* 0x7c8        0x4000_67c8
        0:0     R      bt_mac_128x32_0_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_114;
        struct
        {
            __I uint32_t bt_mac_128x32_0_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7c8;
    } u_7c8;

    /* 0x7cc        0x4000_67cc
        0:0     R      bt_mac_128x32_0_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_115;
        struct
        {
            __I uint32_t bt_mac_128x32_0_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7cc;
    } u_7cc;

    /* 0x7d0        0x4000_67d0
        0       R/W    bt_mac_128x32_1_bist_rstn           1'h0
        1       R/W    bt_mac_128x32_1_bist_mode           1'h0
        2       R/W    bt_mac_128x32_1_drf_bist_mode       1'h0
        3       R/W    bt_mac_128x32_1_drf_test_resume     1'h0
        4       R/W    bt_mac_128x32_1_bist_loop_mode      1'h0
        5       R/W    bt_mac_128x32_1_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_128x32_1_drf_start_pause     1'h0
        30      R      bt_mac_128x32_1_drf_bist_done       1'h0
        31      R      bt_mac_128x32_1_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_116;
        struct
        {
            __IO uint32_t bt_mac_128x32_1_bist_rstn: 1;
            __IO uint32_t bt_mac_128x32_1_bist_mode: 1;
            __IO uint32_t bt_mac_128x32_1_drf_bist_mode: 1;
            __IO uint32_t bt_mac_128x32_1_drf_test_resume: 1;
            __IO uint32_t bt_mac_128x32_1_bist_loop_mode: 1;
            __IO uint32_t bt_mac_128x32_1_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_128x32_1_drf_start_pause: 1;
            __I uint32_t bt_mac_128x32_1_drf_bist_done: 1;
            __I uint32_t bt_mac_128x32_1_bist_done: 1;
        } BITS_7d0;
    } u_7d0;

    /* 0x7d4        0x4000_67d4
        0:0     R/W    bt_mac_128x32_1_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_117;
        struct
        {
            __IO uint32_t bt_mac_128x32_1_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7d4;
    } u_7d4;

    /* 0x7d8        0x4000_67d8
        0:0     R      bt_mac_128x32_1_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_118;
        struct
        {
            __I uint32_t bt_mac_128x32_1_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7d8;
    } u_7d8;

    /* 0x7dc        0x4000_67dc
        0:0     R      bt_mac_128x32_1_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_119;
        struct
        {
            __I uint32_t bt_mac_128x32_1_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7dc;
    } u_7dc;

    /* 0x7e0        0x4000_67e0
        0       R/W    bt_mac_216x32_bist_rstn             1'h0
        1       R/W    bt_mac_216x32_bist_mode             1'h0
        2       R/W    bt_mac_216x32_drf_bist_mode         1'h0
        3       R/W    bt_mac_216x32_drf_test_resume       1'h0
        4       R/W    bt_mac_216x32_bist_loop_mode        1'h0
        5       R/W    bt_mac_216x32_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_216x32_drf_start_pause       1'h0
        30      R      bt_mac_216x32_drf_bist_done         1'h0
        31      R      bt_mac_216x32_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_120;
        struct
        {
            __IO uint32_t bt_mac_216x32_bist_rstn: 1;
            __IO uint32_t bt_mac_216x32_bist_mode: 1;
            __IO uint32_t bt_mac_216x32_drf_bist_mode: 1;
            __IO uint32_t bt_mac_216x32_drf_test_resume: 1;
            __IO uint32_t bt_mac_216x32_bist_loop_mode: 1;
            __IO uint32_t bt_mac_216x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_216x32_drf_start_pause: 1;
            __I uint32_t bt_mac_216x32_drf_bist_done: 1;
            __I uint32_t bt_mac_216x32_bist_done: 1;
        } BITS_7e0;
    } u_7e0;

    /* 0x7e4        0x4000_67e4
        0:0     R/W    bt_mac_216x32_bist_grp_en           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_121;
        struct
        {
            __IO uint32_t bt_mac_216x32_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7e4;
    } u_7e4;

    /* 0x7e8        0x4000_67e8
        0:0     R      bt_mac_216x32_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_122;
        struct
        {
            __I uint32_t bt_mac_216x32_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7e8;
    } u_7e8;

    /* 0x7ec        0x4000_67ec
        0:0     R      bt_mac_216x32_drf_bist_fail         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_123;
        struct
        {
            __I uint32_t bt_mac_216x32_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7ec;
    } u_7ec;

    /* 0x7f0        0x4000_67f0
        0       R/W    bt_mac_640x32_bist_rstn             1'h0
        1       R/W    bt_mac_640x32_bist_mode             1'h0
        2       R/W    bt_mac_640x32_drf_bist_mode         1'h0
        3       R/W    bt_mac_640x32_drf_test_resume       1'h0
        4       R/W    bt_mac_640x32_bist_loop_mode        1'h0
        5       R/W    bt_mac_640x32_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_640x32_drf_start_pause       1'h0
        30      R      bt_mac_640x32_drf_bist_done         1'h0
        31      R      bt_mac_640x32_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_124;
        struct
        {
            __IO uint32_t bt_mac_640x32_bist_rstn: 1;
            __IO uint32_t bt_mac_640x32_bist_mode: 1;
            __IO uint32_t bt_mac_640x32_drf_bist_mode: 1;
            __IO uint32_t bt_mac_640x32_drf_test_resume: 1;
            __IO uint32_t bt_mac_640x32_bist_loop_mode: 1;
            __IO uint32_t bt_mac_640x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_640x32_drf_start_pause: 1;
            __I uint32_t bt_mac_640x32_drf_bist_done: 1;
            __I uint32_t bt_mac_640x32_bist_done: 1;
        } BITS_7f0;
    } u_7f0;

    /* 0x7f4        0x4000_67f4
        0:0     R/W    bt_mac_640x32_bist_grp_en           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_125;
        struct
        {
            __IO uint32_t bt_mac_640x32_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7f4;
    } u_7f4;

    /* 0x7f8        0x4000_67f8
        0:0     R      bt_mac_640x32_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_126;
        struct
        {
            __I uint32_t bt_mac_640x32_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7f8;
    } u_7f8;

    /* 0x7fc        0x4000_67fc
        0:0     R      bt_mac_640x32_drf_bist_fail         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_127;
        struct
        {
            __I uint32_t bt_mac_640x32_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_7fc;
    } u_7fc;

    /* 0x800        0x4000_6800
        0       R/W    bt_mac_96x32_bist_rstn              1'h0
        1       R/W    bt_mac_96x32_bist_mode              1'h0
        2       R/W    bt_mac_96x32_drf_bist_mode          1'h0
        3       R/W    bt_mac_96x32_drf_test_resume        1'h0
        4       R/W    bt_mac_96x32_bist_loop_mode         1'h0
        5       R/W    bt_mac_96x32_dyn_read_en            1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_96x32_drf_start_pause        1'h0
        30      R      bt_mac_96x32_drf_bist_done          1'h0
        31      R      bt_mac_96x32_bist_done              1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_128;
        struct
        {
            __IO uint32_t bt_mac_96x32_bist_rstn: 1;
            __IO uint32_t bt_mac_96x32_bist_mode: 1;
            __IO uint32_t bt_mac_96x32_drf_bist_mode: 1;
            __IO uint32_t bt_mac_96x32_drf_test_resume: 1;
            __IO uint32_t bt_mac_96x32_bist_loop_mode: 1;
            __IO uint32_t bt_mac_96x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_96x32_drf_start_pause: 1;
            __I uint32_t bt_mac_96x32_drf_bist_done: 1;
            __I uint32_t bt_mac_96x32_bist_done: 1;
        } BITS_800;
    } u_800;

    /* 0x804        0x4000_6804
        0:0     R/W    bt_mac_96x32_bist_grp_en            1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_129;
        struct
        {
            __IO uint32_t bt_mac_96x32_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_804;
    } u_804;

    /* 0x808        0x4000_6808
        0:0     R      bt_mac_96x32_bist_fail              1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_130;
        struct
        {
            __I uint32_t bt_mac_96x32_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_808;
    } u_808;

    /* 0x80c        0x4000_680c
        0:0     R      bt_mac_96x32_drf_bist_fail          1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_131;
        struct
        {
            __I uint32_t bt_mac_96x32_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_80c;
    } u_80c;

    /* 0x810        0x4000_6810
        0       R/W    bt_mac_144x32_bist_rstn             1'h0
        1       R/W    bt_mac_144x32_bist_mode             1'h0
        2       R/W    bt_mac_144x32_drf_bist_mode         1'h0
        3       R/W    bt_mac_144x32_drf_test_resume       1'h0
        4       R/W    bt_mac_144x32_bist_loop_mode        1'h0
        5       R/W    bt_mac_144x32_dyn_read_en           1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_mac_144x32_drf_start_pause       1'h0
        30      R      bt_mac_144x32_drf_bist_done         1'h0
        31      R      bt_mac_144x32_bist_done             1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_132;
        struct
        {
            __IO uint32_t bt_mac_144x32_bist_rstn: 1;
            __IO uint32_t bt_mac_144x32_bist_mode: 1;
            __IO uint32_t bt_mac_144x32_drf_bist_mode: 1;
            __IO uint32_t bt_mac_144x32_drf_test_resume: 1;
            __IO uint32_t bt_mac_144x32_bist_loop_mode: 1;
            __IO uint32_t bt_mac_144x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_mac_144x32_drf_start_pause: 1;
            __I uint32_t bt_mac_144x32_drf_bist_done: 1;
            __I uint32_t bt_mac_144x32_bist_done: 1;
        } BITS_810;
    } u_810;

    /* 0x814        0x4000_6814
        0:0     R/W    bt_mac_144x32_bist_grp_en           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_133;
        struct
        {
            __IO uint32_t bt_mac_144x32_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_814;
    } u_814;

    /* 0x818        0x4000_6818
        0:0     R      bt_mac_144x32_bist_fail             1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_134;
        struct
        {
            __I uint32_t bt_mac_144x32_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_818;
    } u_818;

    /* 0x81c        0x4000_681c
        0:0     R      bt_mac_144x32_drf_bist_fail         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_135;
        struct
        {
            __I uint32_t bt_mac_144x32_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_81c;
    } u_81c;

    /* 0x820        0x4000_6820
        0       R/W    zb_mac_ram32x32_bist_rstn           1'h0
        1       R/W    zb_mac_ram32x32_bist_mode           1'h0
        2       R/W    zb_mac_ram32x32_drf_bist_mode       1'h0
        3       R/W    zb_mac_ram32x32_drf_test_resume     1'h0
        4       R/W    zb_mac_ram32x32_bist_loop_mode      1'h0
        5       R/W    zb_mac_ram32x32_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      zb_mac_ram32x32_drf_start_pause     1'h0
        30      R      zb_mac_ram32x32_drf_bist_done       1'h0
        31      R      zb_mac_ram32x32_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_136;
        struct
        {
            __IO uint32_t zb_mac_ram32x32_bist_rstn: 1;
            __IO uint32_t zb_mac_ram32x32_bist_mode: 1;
            __IO uint32_t zb_mac_ram32x32_drf_bist_mode: 1;
            __IO uint32_t zb_mac_ram32x32_drf_test_resume: 1;
            __IO uint32_t zb_mac_ram32x32_bist_loop_mode: 1;
            __IO uint32_t zb_mac_ram32x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t zb_mac_ram32x32_drf_start_pause: 1;
            __I uint32_t zb_mac_ram32x32_drf_bist_done: 1;
            __I uint32_t zb_mac_ram32x32_bist_done: 1;
        } BITS_820;
    } u_820;

    /* 0x824        0x4000_6824
        3:0     R/W    zb_mac_ram32x32_bist_grp_en         4'h0
        31:4    R      reserved                            28'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_137;
        struct
        {
            __IO uint32_t zb_mac_ram32x32_bist_grp_en: 4;
            __I uint32_t RESERVED_0: 28;
        } BITS_824;
    } u_824;

    /* 0x828        0x4000_6828
        3:0     R      zb_mac_ram32x32_bist_fail           4'h0
        31:4    R      reserved                            28'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_138;
        struct
        {
            __I uint32_t zb_mac_ram32x32_bist_fail: 4;
            __I uint32_t RESERVED_0: 28;
        } BITS_828;
    } u_828;

    /* 0x82c        0x4000_682c
        3:0     R      zb_mac_ram32x32_drf_bist_fail       4'h0
        31:4    R      reserved                            28'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_139;
        struct
        {
            __I uint32_t zb_mac_ram32x32_drf_bist_fail: 4;
            __I uint32_t RESERVED_0: 28;
        } BITS_82c;
    } u_82c;

    /* 0x830        0x4000_6830
        0       R/W    zb_mac_ram40x32_bist_rstn           1'h0
        1       R/W    zb_mac_ram40x32_bist_mode           1'h0
        2       R/W    zb_mac_ram40x32_drf_bist_mode       1'h0
        3       R/W    zb_mac_ram40x32_drf_test_resume     1'h0
        4       R/W    zb_mac_ram40x32_bist_loop_mode      1'h0
        5       R/W    zb_mac_ram40x32_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      zb_mac_ram40x32_drf_start_pause     1'h0
        30      R      zb_mac_ram40x32_drf_bist_done       1'h0
        31      R      zb_mac_ram40x32_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_140;
        struct
        {
            __IO uint32_t zb_mac_ram40x32_bist_rstn: 1;
            __IO uint32_t zb_mac_ram40x32_bist_mode: 1;
            __IO uint32_t zb_mac_ram40x32_drf_bist_mode: 1;
            __IO uint32_t zb_mac_ram40x32_drf_test_resume: 1;
            __IO uint32_t zb_mac_ram40x32_bist_loop_mode: 1;
            __IO uint32_t zb_mac_ram40x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t zb_mac_ram40x32_drf_start_pause: 1;
            __I uint32_t zb_mac_ram40x32_drf_bist_done: 1;
            __I uint32_t zb_mac_ram40x32_bist_done: 1;
        } BITS_830;
    } u_830;

    /* 0x834        0x4000_6834
        1:0     R/W    zb_mac_ram40x32_bist_grp_en         2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_141;
        struct
        {
            __IO uint32_t zb_mac_ram40x32_bist_grp_en: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_834;
    } u_834;

    /* 0x838        0x4000_6838
        1:0     R      zb_mac_ram40x32_bist_fail           2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_142;
        struct
        {
            __I uint32_t zb_mac_ram40x32_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_838;
    } u_838;

    /* 0x83c        0x4000_683c
        1:0     R      zb_mac_ram40x32_drf_bist_fail       2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_143;
        struct
        {
            __I uint32_t zb_mac_ram40x32_drf_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_83c;
    } u_83c;

    /* 0x840        0x4000_6840
        0       R/W    zb_mac_ram16x32_bist_rstn           1'h0
        1       R/W    zb_mac_ram16x32_bist_mode           1'h0
        2       R/W    zb_mac_ram16x32_drf_bist_mode       1'h0
        3       R/W    zb_mac_ram16x32_drf_test_resume     1'h0
        4       R/W    zb_mac_ram16x32_bist_loop_mode      1'h0
        5       R/W    zb_mac_ram16x32_dyn_read_en         1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      zb_mac_ram16x32_drf_start_pause     1'h0
        30      R      zb_mac_ram16x32_drf_bist_done       1'h0
        31      R      zb_mac_ram16x32_bist_done           1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_144;
        struct
        {
            __IO uint32_t zb_mac_ram16x32_bist_rstn: 1;
            __IO uint32_t zb_mac_ram16x32_bist_mode: 1;
            __IO uint32_t zb_mac_ram16x32_drf_bist_mode: 1;
            __IO uint32_t zb_mac_ram16x32_drf_test_resume: 1;
            __IO uint32_t zb_mac_ram16x32_bist_loop_mode: 1;
            __IO uint32_t zb_mac_ram16x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t zb_mac_ram16x32_drf_start_pause: 1;
            __I uint32_t zb_mac_ram16x32_drf_bist_done: 1;
            __I uint32_t zb_mac_ram16x32_bist_done: 1;
        } BITS_840;
    } u_840;

    /* 0x844        0x4000_6844
        0:0     R/W    zb_mac_ram16x32_bist_grp_en         1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_145;
        struct
        {
            __IO uint32_t zb_mac_ram16x32_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_844;
    } u_844;

    /* 0x848        0x4000_6848
        0:0     R      zb_mac_ram16x32_bist_fail           1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_146;
        struct
        {
            __I uint32_t zb_mac_ram16x32_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_848;
    } u_848;

    /* 0x84c        0x4000_684c
        0:0     R      zb_mac_ram16x32_drf_bist_fail       1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_147;
        struct
        {
            __I uint32_t zb_mac_ram16x32_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_84c;
    } u_84c;

    /* 0x850        0x4000_6850
        0       R/W    bt_modem_rx_bist_rstn               1'h0
        1       R/W    bt_modem_rx_bist_mode               1'h0
        2       R/W    bt_modem_rx_drf_bist_mode           1'h0
        3       R/W    bt_modem_rx_drf_test_resume         1'h0
        4       R/W    bt_modem_rx_bist_loop_mode          1'h0
        5       R/W    bt_modem_rx_dyn_read_en             1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      bt_modem_rx_drf_start_pause         1'h0
        30      R      bt_modem_rx_drf_bist_done           1'h0
        31      R      bt_modem_rx_bist_done               1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_148;
        struct
        {
            __IO uint32_t bt_modem_rx_bist_rstn: 1;
            __IO uint32_t bt_modem_rx_bist_mode: 1;
            __IO uint32_t bt_modem_rx_drf_bist_mode: 1;
            __IO uint32_t bt_modem_rx_drf_test_resume: 1;
            __IO uint32_t bt_modem_rx_bist_loop_mode: 1;
            __IO uint32_t bt_modem_rx_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t bt_modem_rx_drf_start_pause: 1;
            __I uint32_t bt_modem_rx_drf_bist_done: 1;
            __I uint32_t bt_modem_rx_bist_done: 1;
        } BITS_850;
    } u_850;

    /* 0x854        0x4000_6854
        4:0     R/W    bt_modem_rx_bist_grp_en             5'h0
        31:5    R      reserved                            27'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_149;
        struct
        {
            __IO uint32_t bt_modem_rx_bist_grp_en: 5;
            __I uint32_t RESERVED_0: 27;
        } BITS_854;
    } u_854;

    /* 0x858        0x4000_6858
        4:0     R      bt_modem_rx_bist_fail               5'h0
        31:5    R      reserved                            27'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_150;
        struct
        {
            __I uint32_t bt_modem_rx_bist_fail: 5;
            __I uint32_t RESERVED_0: 27;
        } BITS_858;
    } u_858;

    /* 0x85c        0x4000_685c
        4:0     R      bt_modem_rx_drf_bist_fail           5'h0
        31:5    R      reserved                            27'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_151;
        struct
        {
            __I uint32_t bt_modem_rx_drf_bist_fail: 5;
            __I uint32_t RESERVED_0: 27;
        } BITS_85c;
    } u_85c;

    /* 0x860        0x4000_6860
        0       R/W    gmac_ram512x32_bist_rstn            1'h0
        1       R/W    gmac_ram512x32_bist_mode            1'h0
        2       R/W    gmac_ram512x32_drf_bist_mode        1'h0
        3       R/W    gmac_ram512x32_drf_test_resume      1'h0
        4       R/W    gmac_ram512x32_bist_loop_mode       1'h0
        5       R/W    gmac_ram512x32_dyn_read_en          1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      gmac_ram512x32_drf_start_pause      1'h0
        30      R      gmac_ram512x32_drf_bist_done        1'h0
        31      R      gmac_ram512x32_bist_done            1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_152;
        struct
        {
            __IO uint32_t gmac_ram512x32_bist_rstn: 1;
            __IO uint32_t gmac_ram512x32_bist_mode: 1;
            __IO uint32_t gmac_ram512x32_drf_bist_mode: 1;
            __IO uint32_t gmac_ram512x32_drf_test_resume: 1;
            __IO uint32_t gmac_ram512x32_bist_loop_mode: 1;
            __IO uint32_t gmac_ram512x32_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t gmac_ram512x32_drf_start_pause: 1;
            __I uint32_t gmac_ram512x32_drf_bist_done: 1;
            __I uint32_t gmac_ram512x32_bist_done: 1;
        } BITS_860;
    } u_860;

    /* 0x864        0x4000_6864
        1:0     R/W    gmac_ram512x32_bist_grp_en          2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_153;
        struct
        {
            __IO uint32_t gmac_ram512x32_bist_grp_en: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_864;
    } u_864;

    /* 0x868        0x4000_6868
        1:0     R      gmac_ram512x32_bist_fail            2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_154;
        struct
        {
            __I uint32_t gmac_ram512x32_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_868;
    } u_868;

    /* 0x86c        0x4000_686c
        1:0     R      gmac_ram512x32_drf_bist_fail        2'h0
        31:2    R      reserved                            30'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_155;
        struct
        {
            __I uint32_t gmac_ram512x32_drf_bist_fail: 2;
            __I uint32_t RESERVED_0: 30;
        } BITS_86c;
    } u_86c;

    /* 0x870        0x4000_6870
        0       R/W    usb_ram_bist_rstn                   1'h0
        1       R/W    usb_ram_bist_mode                   1'h0
        2       R/W    usb_ram_drf_bist_mode               1'h0
        3       R/W    usb_ram_drf_test_resume             1'h0
        4       R/W    usb_ram_bist_loop_mode              1'h0
        5       R/W    usb_ram_dyn_read_en                 1'h0
        6       R      reserved                            1'h0
        28:7    R      reserved                            22'h0
        29      R      usb_ram_drf_start_pause             1'h0
        30      R      usb_ram_drf_bist_done               1'h0
        31      R      usb_ram_bist_done                   1'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_156;
        struct
        {
            __IO uint32_t usb_ram_bist_rstn: 1;
            __IO uint32_t usb_ram_bist_mode: 1;
            __IO uint32_t usb_ram_drf_bist_mode: 1;
            __IO uint32_t usb_ram_drf_test_resume: 1;
            __IO uint32_t usb_ram_bist_loop_mode: 1;
            __IO uint32_t usb_ram_dyn_read_en: 1;
            __I uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 22;
            __I uint32_t usb_ram_drf_start_pause: 1;
            __I uint32_t usb_ram_drf_bist_done: 1;
            __I uint32_t usb_ram_bist_done: 1;
        } BITS_870;
    } u_870;

    /* 0x874        0x4000_6874
        0:0     R/W    usb_ram_bist_grp_en                 1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_157;
        struct
        {
            __IO uint32_t usb_ram_bist_grp_en: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_874;
    } u_874;

    /* 0x878        0x4000_6878
        0:0     R      usb_ram_bist_fail                   1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_158;
        struct
        {
            __I uint32_t usb_ram_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_878;
    } u_878;

    /* 0x87c        0x4000_687c
        0:0     R      usb_ram_drf_bist_fail               1'h0
        31:1    R      reserved                            31'h0
    */
    union
    {
        __IO uint32_t REG_MBIST_RAM_159;
        struct
        {
            __I uint32_t usb_ram_drf_bist_fail: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_87c;
    } u_87c;


} SoC_VENDOR_REG_TypeDef;

#define VENDOR_REG_BASE                 SOC_VENDOR_REG_BASE
#define SoC_VENDOR                      ((SoC_VENDOR_REG_TypeDef   *) VENDOR_REG_BASE)

#endif //#define PLATFORM_REG_H