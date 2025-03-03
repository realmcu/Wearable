#ifndef PCC_REG_H
#define PCC_REG_H
#include <stdint.h>

/* ================================================================================ */
/* ================            System Block Control            ================ */
/* ================================================================================ */

/* Auto gen based on BB2Ultra_IO_module_20240626_v2.xlsx */

typedef struct
{
    /* 0x0200       0x4000_0200
        0       R/W    r_cpu_slow_en                           1'b0
        1       R/W    r_cpu_slow_opt_wfi                      1'b0
        2       R/W    r_cpu_slow_opt_dsp                      1'b0
        3       R/W    r_dsp_slow_en                           1'b0
        4       R/W    r_dsp_slow_opt_dsp                      1'b0
        5       R/W    r_auto_dsp_fast_clk_en                  1'b0
        6       R/W    r_clk_cpu_f1m_en                        1'b0
        7       R/W    r_clk_cpu_32k_en                        1'b0
        8       R/W    r_aon_rd_opt                            1'b1
        10:9    R/W    RESERVED9                               2'b0
        18:11   R/W    r_dsp_fast_clk_ext_num                  8'b0
        25:19   R/W    r_bt_ahb_wait_cnt                       7'b0010001
        26      R/W    r_cpu_slow_opt_at_tx                    1'b1
        27      R/W    r_cpu_slow_opt_at_rx                    1'b1
        31:28   R/W    r_cpu_low_rate_valid_num                4'h3
    */
    union
    {
        __IO uint32_t SYS_CLK_SEL;
        struct
        {
            __IO uint32_t r_cpu_slow_en: 1;
            __IO uint32_t r_cpu_slow_opt_wfi: 1;
            __IO uint32_t r_cpu_slow_opt_dsp: 1;
            __IO uint32_t r_dsp_slow_en: 1;
            __IO uint32_t r_dsp_slow_opt_dsp: 1;
            __IO uint32_t r_auto_dsp_fast_clk_en: 1;
            __IO uint32_t r_clk_cpu_f1m_en: 1;
            __IO uint32_t r_clk_cpu_32k_en: 1;
            __IO uint32_t r_aon_rd_opt: 1;
            __IO uint32_t RESERVED9: 2;
            __IO uint32_t r_dsp_fast_clk_ext_num: 8;
            __IO uint32_t r_bt_ahb_wait_cnt: 7;
            __IO uint32_t r_cpu_slow_opt_at_tx: 1;
            __IO uint32_t r_cpu_slow_opt_at_rx: 1;
            __IO uint32_t r_cpu_low_rate_valid_num: 4;
        } BITS_200;
    } u_200;

    /* 0x0204       0x4000_0204
        3:0     R      RF_RL_ID                                4'h0
        7:4     R      RF_RTL_ID                               4'h0
        8       R/W    r_btaon_acc_no_block                    1'b0
        9       R/W    r_cpu_slow_opt_spic0                    1'b0
        10      R/W    r_cpu_slow_opt_spic1                    1'b0
        11      R/W    r_cpu_slow_opt_spic2                    1'b0
        12      R/W    r_cpu_slow_opt_spi0                     1'b0
        13      R/W    r_dsp_clk_src_pll_sel                   1'b0
        14      R/W    r_dsp_auto_slow_filter_en               1'b1
        15      R/W    RESERVED33                              1'b0
        16      R/W    reserved0204_16                         1'b0
        17      R/W    reserved0204_17                         1'b0
        18      R/W    r_auto_slow_opt                         1'b0
        19      R/W    r_cpu_slow_opt_dma                      1'b0
        20      R/W    r_cpu_slow_opt_sdio0                    1'b0
        21      R/W    reserved0204_21                         1'b0
        22      R/W    r_cpu_slow_opt_bt_sram_1                1'b0
        23      R/W    r_cpu_slow_opt_bt_sram_2                1'b0
        24      R/W    r_dsp_slow_opt_dspram_wbuf              1'b0
        25      R/W    r_cpu_slow_opt_dspram_wbuf              1'b0
        26      R/W    r_dsp_slow_opt_at_tx                    1'b1
        27      R/W    r_dsp_slow_opt_at_rx                    1'b1
        31:28   R/W    r_dsp_low_rate_valid_num                4'h3
    */
    union
    {
        __IO uint32_t SLOW_CTRL;
        struct
        {
            __I uint32_t RF_RL_ID: 4;
            __I uint32_t RF_RTL_ID: 4;
            __IO uint32_t r_btaon_acc_no_block: 1;
            __IO uint32_t r_cpu_slow_opt_spic0: 1;
            __IO uint32_t r_cpu_slow_opt_spic1: 1;
            __IO uint32_t r_cpu_slow_opt_spic2: 1;
            __IO uint32_t r_cpu_slow_opt_spi0: 1;
            __IO uint32_t r_dsp_clk_src_pll_sel: 1;
            __IO uint32_t r_dsp_auto_slow_filter_en: 1;
            __IO uint32_t RESERVED33: 1;
            __IO uint32_t reserved0204_16: 1;
            __IO uint32_t reserved0204_17: 1;
            __IO uint32_t r_auto_slow_opt: 1;
            __IO uint32_t r_cpu_slow_opt_dma: 1;
            __IO uint32_t r_cpu_slow_opt_sdio0: 1;
            __IO uint32_t reserved0204_21: 1;
            __IO uint32_t r_cpu_slow_opt_bt_sram_1: 1;
            __IO uint32_t r_cpu_slow_opt_bt_sram_2: 1;
            __IO uint32_t r_dsp_slow_opt_dspram_wbuf: 1;
            __IO uint32_t r_cpu_slow_opt_dspram_wbuf: 1;
            __IO uint32_t r_dsp_slow_opt_at_tx: 1;
            __IO uint32_t r_dsp_slow_opt_at_rx: 1;
            __IO uint32_t r_dsp_low_rate_valid_num: 4;
        } BITS_204;
    } u_204;

    /* 0x0208       0x4000_0208
        3:0     R/W    r_cpu_div_sel                           4'h4
        7:4     R/W    r_cpu_div_sel_slow                      4'h4
        8       R/W    r_cpu_div_en                            1'b1
        9       R/W    r_CPU_CLK_SRC_EN                        1'b1
        10      R/W    r_cpu_auto_slow_filter_en               1'b1
        11      R/W    r_cpu_auto_slow_force_update            1'b0
        12      R/W    r_cpu_pll_clk_cg_en                     1'b1
        13      R/W    r_cpu_xtal_clk_cg_en                    1'b1
        14      R/W    r_cpu_osc40_clk_cg_en                   1'b1
        15      R/W    r_cpu_div_en_slow                       1'b1
        19:16   R/W    r_dsp_div_sel                           4'h0
        23:20   R/W    r_dsp_div_sel_slow                      4'h0
        24      R/W    r_dsp_div_en                            1'b0
        25      R/W    r_DSP_CLK_SRC_EN                        1'b1
        26      R/W    r_dsp_clk_src_sel_1                     1'b1
        27      R/W    r_dsp_clk_src_sel_0                     1'b0
        28      R/W    r_dsp_pll_clk_cg_en                     1'b0
        29      R/W    r_dsp_xtal_clk_cg_en                    1'b0
        30      R/W    r_dsp_osc40_clk_cg_en                   1'b0
        31      R/W    r_dsp_div_en_slow                       1'b0
    */
    union
    {
        __IO uint32_t REG_0x208;
        struct
        {
            __IO uint32_t r_cpu_div_sel: 4;
            __IO uint32_t r_cpu_div_sel_slow: 4;
            __IO uint32_t r_cpu_div_en: 1;
            __IO uint32_t r_CPU_CLK_SRC_EN: 1;
            __IO uint32_t r_cpu_auto_slow_filter_en: 1;
            __IO uint32_t r_cpu_auto_slow_force_update: 1;
            __IO uint32_t r_cpu_pll_clk_cg_en: 1;
            __IO uint32_t r_cpu_xtal_clk_cg_en: 1;
            __IO uint32_t r_cpu_osc40_clk_cg_en: 1;
            __IO uint32_t r_cpu_div_en_slow: 1;
            __IO uint32_t r_dsp_div_sel: 4;
            __IO uint32_t r_dsp_div_sel_slow: 4;
            __IO uint32_t r_dsp_div_en: 1;
            __IO uint32_t r_DSP_CLK_SRC_EN: 1;
            __IO uint32_t r_dsp_clk_src_sel_1: 1;
            __IO uint32_t r_dsp_clk_src_sel_0: 1;
            __IO uint32_t r_dsp_pll_clk_cg_en: 1;
            __IO uint32_t r_dsp_xtal_clk_cg_en: 1;
            __IO uint32_t r_dsp_osc40_clk_cg_en: 1;
            __IO uint32_t r_dsp_div_en_slow: 1;
        } BITS_208;
    } u_208;

    /* 0x020C       0x4000_020c
        3:0     R/W    r_flash_div_sel                         4'h0
        4       R/W    r_flash_div_en                          1'h0
        5       R/W    r_FLASH_CLK_SRC_EN                      1'h1
        6       R/W    r_flash_clk_src_sel_1                   1'h0
        7       R/W    r_flash_clk_src_sel_0                   1'h0
        8       R/W    r_flash_mux_1_clk_cg_en                 1'h1
        9       R/W    r_rng_sfosc_sel                         1'h0
        12:10   R/W    r_rng_sfosc_div_sel                     3'h0
        13      R/W    r_flash_clk_src_pll_sel_0               1'h0
        14      R/W    r_flash_clk_src_pll_sel_1               1'h0
        15      R/W    RESERVED78                              1'h0
        18:16   R/W    r_40m_div_sel                           3'h0
        19      R/W    RESERVED76                              1'h0
        20      R/W    r_40m_div_en                            1'h0
        21      R/W    r_CLK_40M_DIV_CG_EN                     1'h1
        22      R/W    r_CLK_40M_SRC_EN                        1'h1
        23      R/W    r_40m_clk_src_sel_1                     1'h0
        24      R/W    r_40m_clk_src_sel_0                     1'h1
        25      R/W    r_f40m_pll_clk_cg_en                    1'h0
        26      R/W    r_CLK_40M_SRC_DIV_EN                    1'h1
        27      R/W    r_CLK_20M_SRC_EN                        1'h0
        28      R/W    r_CLK_10M_SRC_EN                        1'h1
        29      R/W    r_CLK_5M_SRC_EN                         1'h0
        31:30   R/W    RESERVED65                              2'h0
    */
    union
    {
        __IO uint32_t REG_0x20C;
        struct
        {
            __IO uint32_t r_flash_div_sel: 4;
            __IO uint32_t r_flash_div_en: 1;
            __IO uint32_t r_FLASH_CLK_SRC_EN: 1;
            __IO uint32_t r_flash_clk_src_sel_1: 1;
            __IO uint32_t r_flash_clk_src_sel_0: 1;
            __IO uint32_t r_flash_mux_1_clk_cg_en: 1;
            __IO uint32_t r_rng_sfosc_sel: 1;
            __IO uint32_t r_rng_sfosc_div_sel: 3;
            __IO uint32_t r_flash_clk_src_pll_sel_0: 1;
            __IO uint32_t r_flash_clk_src_pll_sel_1: 1;
            __IO uint32_t RESERVED78: 1;
            __IO uint32_t r_40m_div_sel: 3;
            __IO uint32_t RESERVED76: 1;
            __IO uint32_t r_40m_div_en: 1;
            __IO uint32_t r_CLK_40M_DIV_CG_EN: 1;
            __IO uint32_t r_CLK_40M_SRC_EN: 1;
            __IO uint32_t r_40m_clk_src_sel_1: 1;
            __IO uint32_t r_40m_clk_src_sel_0: 1;
            __IO uint32_t r_f40m_pll_clk_cg_en: 1;
            __IO uint32_t r_CLK_40M_SRC_DIV_EN: 1;
            __IO uint32_t r_CLK_20M_SRC_EN: 1;
            __IO uint32_t r_CLK_10M_SRC_EN: 1;
            __IO uint32_t r_CLK_5M_SRC_EN: 1;
            __IO uint32_t RESERVED65: 2;
        } BITS_20C;
    } u_20C;

    /* 0x0210       0x4000_0210
        0       R/W    RESERVED121                             1'h0
        1       R/W    RESERVED120                             1'h0
        2       R/W    BIT_SOC_BTBUS_EN                        1'b1
        3       R/W    RESERVED118                             1'h0
        4       R/W    BIT_SOC_FLASH_EN                        1'b1
        5       R/W    BIT_SOC_FLASH_1_EN                      1'h0
        6       R/W    BIT_SOC_FLASH_2_EN                      1'h0
        7       R/W    BIT_SOC_UART0_EN                        1'h0
        8       R/W    BIT_SOC_UART1_EN                        1'h0
        9       R/W    BIT_SOC_UART2_EN                        1'h0
        10      R/W    BIT_SOC_UART3_EN                        1'h0
        11      R/W    BIT_SOC_UART4_EN                        1'h0
        12      R/W    BIT_SOC_UART5_EN                        1'h0
        13      R/W    BIT_SOC_GDMA0_EN                        1'h0
        14      R/W    BIT_SOC_SDH_EN                          1'h0
        15      R/W    BIT_SOC_USB_EN                          1'h0
        16      R/W    BIT_SOC_TIMER0_EN                       1'h0
        17      R/W    RESERVED104                             1'h0
        18      R/W    BIT_SOC_SWR_SS_EN                       1'h0
        19      R/W    BIT_SOC_PDCK_EN                         1'h0
        20      R/W    BIT_SOC_AAC_XTAL_EN                     1'h0
        21      R/W    BIT_SOC_TIMER1_PWM_EN                   1'h0
        22      R/W    BIT_SOC_TIMER1_EN                       1'h0
        23      R/W    BIT_SOC_CAN0_EN                         1'h0
        24      R/W    BIT_SOC_PPE_EN                          1'h0
        25      R/W    BIT_SOC_PKE_EN                          1'h0
        26      R/W    BIT_SOC_RTZIP_EN                        1'h0
        27      R/W    BIT_SOC_CAN1_EN                         1'h0
        28      R/W    BIT_SOC_JPEG_EN                         1'h0
        29      R/W    BIT_SOC_GMAC_EN                         1'h0
        30      R/W    BIT_SOC_ZIGBEE_EN                       1'h0
        31      R/W    BIT_SOC_TIMER1_DEB_EN                   5'h0
    */
    union
    {
        __IO uint32_t SOC_FUNC_EN;
        struct
        {
            __IO uint32_t RESERVED121: 1;
            __IO uint32_t RESERVED120: 1;
            __IO uint32_t BIT_SOC_BTBUS_EN: 1;
            __IO uint32_t RESERVED118: 1;
            __IO uint32_t BIT_SOC_FLASH_EN: 1;
            __IO uint32_t BIT_SOC_FLASH_1_EN: 1;
            __IO uint32_t BIT_SOC_FLASH_2_EN: 1;
            __IO uint32_t BIT_SOC_UART0_EN: 1;
            __IO uint32_t BIT_SOC_UART1_EN: 1;
            __IO uint32_t BIT_SOC_UART2_EN: 1;
            __IO uint32_t BIT_SOC_UART3_EN: 1;
            __IO uint32_t BIT_SOC_UART4_EN: 1;
            __IO uint32_t BIT_SOC_UART5_EN: 1;
            __IO uint32_t BIT_SOC_GDMA0_EN: 1;
            __IO uint32_t BIT_SOC_SDH_EN: 1;
            __IO uint32_t BIT_SOC_USB_EN: 1;
            __IO uint32_t BIT_SOC_TIMER0_EN: 1;
            __IO uint32_t RESERVED104: 1;
            __IO uint32_t BIT_SOC_SWR_SS_EN: 1;
            __IO uint32_t BIT_SOC_PDCK_EN: 1;
            __IO uint32_t BIT_SOC_AAC_XTAL_EN: 1;
            __IO uint32_t BIT_SOC_TIMER1_PWM_EN: 1;
            __IO uint32_t BIT_SOC_TIMER1_EN: 1;
            __IO uint32_t BIT_SOC_CAN0_EN: 1;
            __IO uint32_t BIT_SOC_PPE_EN: 1;
            __IO uint32_t BIT_SOC_PKE_EN: 1;
            __IO uint32_t BIT_SOC_RTZIP_EN: 1;
            __IO uint32_t BIT_SOC_CAN1_EN: 1;
            __IO uint32_t BIT_SOC_JPEG_EN: 1;
            __IO uint32_t BIT_SOC_GMAC_EN: 1;
            __IO uint32_t BIT_SOC_ZIGBEE_EN: 1;
            __IO uint32_t BIT_SOC_TIMER1_DEB_EN: 1;
        } BITS_210;
    } u_210;

    /* 0x0214       0x4000_0214
        2:0     R/W    sdio0_clk_div                           3'h0
        3       R/W    sdio0_clk_div_en                        1'h0
        4       R/W    sdio0_clk_src_en                        1'h0
        5       R/W    sdio0_clk_sel0                          1'h0
        6       R/W    sdio0_clk_sel1                          1'h0
        7       R/W    RESERVED139                             1'h0
        10:8    R/W    DISP_CLK_DIV                            3'h0
        11      R/W    DISP_CLK_DIV_EN                         1'h0
        12      R/W    DISP_CLK_SRC_EN                         1'h0
        13      R/W    DISP_CLK_SEL0                           1'h0
        14      R/W    DISP_CLK_SEL1                           1'h0
        15      R/W    DISP_PLL_CLK_SEL                        1'h0
        18:16   R/W    GMAC_CLK_125M_DIV                       1'h0
        19      R/W    GMAC_CLK_125M_DIV_EN                    1'h0
        20      R/W    GMAC_CLK_125M_CLK_SRC_EN                1'h0
        21      R/W    GMAC_CLK_125M_CLK_SEL                   1'h0
        23:22   R/W    RESERVED128                             2'h0
        26:24   R/W    GMAC_CLK_50M_DIV                        1'h0
        27      R/W    GMAC_CLK_50M_DIV_EN                     1'h0
        28      R/W    GMAC_CLK_50M_CLK_SRC_EN                 1'h0
        29      R/W    GMAC_CLK_50M_CLK_SEL                    1'h0
        31:30   R/W    RESERVED123                             16'h0
    */
    union
    {
        __IO uint32_t REG_0x0214;
        struct
        {
            __IO uint32_t sdio0_clk_div: 3;
            __IO uint32_t sdio0_clk_div_en: 1;
            __IO uint32_t sdio0_clk_src_en: 1;
            __IO uint32_t sdio0_clk_sel0: 1;
            __IO uint32_t sdio0_clk_sel1: 1;
            __IO uint32_t RESERVED139: 1;
            __IO uint32_t DISP_CLK_DIV: 3;
            __IO uint32_t DISP_CLK_DIV_EN: 1;
            __IO uint32_t DISP_CLK_SRC_EN: 1;
            __IO uint32_t DISP_CLK_SEL0: 1;
            __IO uint32_t DISP_CLK_SEL1: 1;
            __IO uint32_t DISP_PLL_CLK_SEL: 1;
            __IO uint32_t GMAC_CLK_125M_DIV: 3;
            __IO uint32_t GMAC_CLK_125M_DIV_EN: 1;
            __IO uint32_t GMAC_CLK_125M_CLK_SRC_EN: 1;
            __IO uint32_t GMAC_CLK_125M_CLK_SEL: 1;
            __IO uint32_t RESERVED128: 2;
            __IO uint32_t GMAC_CLK_50M_DIV: 3;
            __IO uint32_t GMAC_CLK_50M_DIV_EN: 1;
            __IO uint32_t GMAC_CLK_50M_CLK_SRC_EN: 1;
            __IO uint32_t GMAC_CLK_50M_CLK_SEL: 1;
            __IO uint32_t RESERVED123: 2;
        } BITS_214;
    } u_214;

    /* 0x0218       0x4000_0218
        1:0     R/W    RESERVED173                             2'h0
        2       R/W    BIT_PERI_AES_EN                         1'h0
        3       R/W    BIT_PERI_RNG_EN                         1'h0
        4       R/W    BIT_PERI_SIMC_EN                        1'h0
        5       R/W    BIT_PERI_LCD_EN                         1'h0
        6       R/W    BIT_PERI_SHA256_EN                      1'h0
        7       R/W    BIT_PERI_SM3_EN                         1'h0
        8       R/W    BIT_PERI_SPI0_EN                        1'h0
        9       R/W    BIT_PERI_SPI1_EN                        1'h0
        10      R/W    BIT_PERI_IRRC_EN                        1'h0
        11      R/W    BIT_PERI_SPI2_EN                        1'h0
        15:12   R/W    RESERVED162                             4'h0
        16      R/W    BIT_PERI_I2C0_EN                        1'h0
        17      R/W    BIT_PERI_I2C1_EN                        1'h0
        18      R/W    BIT_PERI_QDEC_EN                        1'h0
        19      R/W    BIT_PERI_KEYSCAN_EN                     1'h0
        20      R/W    BIT_PERI_I2C2_EN                        1'h0
        21      R/W    RESERVED156                             1'h0
        22      R/W    BIT_PERI_PSRAM_EN                       1'h0
        23      R/W    RESERVED154                             1'h0
        24      R/W    BIT_PERI_SPI0_S_EN                      1'h0
        25      R/W    BIT_DSP_CORE_EN                         1'b1
        26      R/W    BIT_DSP_H2D_D2H                         1'b1
        27      R/W    BIT_DSP_MEM_EN                          1'b1
        28      R/W    BIT_ASRC_EN                             1'h0
        29      R/W    BIT_DSP_WDT_EN                          1'h0
        30      R/W    BIT_EFUSE_EN                            1'h0
        31      R/W    BIT_DATA_MEM_EN                         1'h0
    */
    union
    {
        __IO uint32_t SOC_PERI_FUNC0_EN;
        struct
        {
            __IO uint32_t RESERVED173: 2;
            __IO uint32_t BIT_PERI_AES_EN: 1;
            __IO uint32_t BIT_PERI_RNG_EN: 1;
            __IO uint32_t BIT_PERI_SIMC_EN: 1;
            __IO uint32_t BIT_PERI_LCD_EN: 1;
            __IO uint32_t BIT_PERI_SHA256_EN: 1;
            __IO uint32_t BIT_PERI_SM3_EN: 1;
            __IO uint32_t BIT_PERI_SPI0_EN: 1;
            __IO uint32_t BIT_PERI_SPI1_EN: 1;
            __IO uint32_t BIT_PERI_IRRC_EN: 1;
            __IO uint32_t BIT_PERI_SPI2_EN: 1;
            __IO uint32_t RESERVED162: 4;
            __IO uint32_t BIT_PERI_I2C0_EN: 1;
            __IO uint32_t BIT_PERI_I2C1_EN: 1;
            __IO uint32_t BIT_PERI_QDEC_EN: 1;
            __IO uint32_t BIT_PERI_KEYSCAN_EN: 1;
            __IO uint32_t BIT_PERI_I2C2_EN: 1;
            __IO uint32_t RESERVED156: 1;
            __IO uint32_t BIT_PERI_PSRAM_EN: 1;
            __IO uint32_t RESERVED154: 1;
            __IO uint32_t BIT_PERI_SPI0_S_EN: 1;
            __IO uint32_t BIT_DSP_CORE_EN: 1;
            __IO uint32_t BIT_DSP_H2D_D2H: 1;
            __IO uint32_t BIT_DSP_MEM_EN: 1;
            __IO uint32_t BIT_ASRC_EN: 1;
            __IO uint32_t BIT_DSP_WDT_EN: 1;
            __IO uint32_t BIT_EFUSE_EN: 1;
            __IO uint32_t BIT_DATA_MEM_EN: 1;
        } BITS_218;
    } u_218;

    /* 0x021C       0x4000_021c
        0       R/W    BIT_PERI_ADC_EN                         1'h0
        7:1     R/W    RESERVED178                             7'h0
        8       R/W    BIT_PERI_GPIO_EN                        1'h0
        9       R/W    BIT_PERI_GPIO1_EN                       1'h0
        31:10   R/W    RESERVED175                             22'h0
    */
    union
    {
        __IO uint32_t SOC_PERI_FUNC1_EN;
        struct
        {
            __IO uint32_t BIT_PERI_ADC_EN: 1;
            __IO uint32_t RESERVED178: 7;
            __IO uint32_t BIT_PERI_GPIO_EN: 1;
            __IO uint32_t BIT_PERI_GPIO1_EN: 1;
            __IO uint32_t RESERVED175: 22;
        } BITS_21C;
    } u_21C;

    /* 0x0220       0x4000_0220
        0       R/W    r_PON_FEN_AUDIO                         1'h0
        1       R/W    r_PON_FEN_SPORT0                        1'h0
        2       R/W    r_PON_FEN_SPORT1                        1'h0
        3       R/W    RESERVED191                             1'h0
        4       R/W    r_CLK_EN_AUDIO                          1'h0
        5       R/W    r_CLK_EN_SPORT0                         1'h0
        6       R/W    r_CLK_EN_SPORT1                         1'h0
        7       R/W    RESERVED187                             1'h0
        8       R/W    r_CLK_EN_SPORT_40M                      1'h0
        9       R/W    r_CLK_EN_SI                             1'h1
        10      R/W    r_PON_FEN_SPORT2                        1'h1
        11      R/W    RESERVED183                             1'h0
        12      R/W    r_CLK_EN_SPORT2                         1'h1
        31:13   R/W    RESERVED181                             19'h0
    */
    union
    {
        __IO uint32_t SOC_AUDIO_IF_EN;
        struct
        {
            __IO uint32_t r_PON_FEN_AUDIO: 1;
            __IO uint32_t r_PON_FEN_SPORT0: 1;
            __IO uint32_t r_PON_FEN_SPORT1: 1;
            __IO uint32_t RESERVED191: 1;
            __IO uint32_t r_CLK_EN_AUDIO: 1;
            __IO uint32_t r_CLK_EN_SPORT0: 1;
            __IO uint32_t r_CLK_EN_SPORT1: 1;
            __IO uint32_t RESERVED187: 1;
            __IO uint32_t r_CLK_EN_SPORT_40M: 1;
            __IO uint32_t r_CLK_EN_SI: 1;
            __IO uint32_t r_PON_FEN_SPORT2: 1;
            __IO uint32_t RESERVED183: 1;
            __IO uint32_t r_CLK_EN_SPORT2: 1;
            __IO uint32_t RESERVED181: 19;
        } BITS_220;
    } u_220;

    /* 0x0224       0x4000_0224
        2:0     R/W    r_SPORT0_PLL_CLK_SEL                    3'b100
        3       R/W    r_SPORT0_EXT_CODEC                      1'h0
        6:4     R/W    r_SPORT1_PLL_CLK_SEL                    3'b001
        7       R/W    r_CODEC_STANDALONE                      1'h0
        15:8    R/W    r_PLL_DIV0_SETTING                      8'h0
        23:16   R/W    r_PLL_DIV1_SETTING                      8'h0
        24      R/W    R_SPORT0_PLL_SRC_CLK_SEL                1'h0
        25      R/W    R_SPORT1_PLL_SRC_CLK_SEL                1'h0
        31:26   R/W    RESERVED196                             6'h0
    */
    union
    {
        __IO uint32_t SOC_AUDIO_CLK_CTRL_A;
        struct
        {
            __IO uint32_t r_SPORT0_PLL_CLK_SEL: 3;
            __IO uint32_t r_SPORT0_EXT_CODEC: 1;
            __IO uint32_t r_SPORT1_PLL_CLK_SEL: 3;
            __IO uint32_t r_CODEC_STANDALONE: 1;
            __IO uint32_t r_PLL_DIV0_SETTING: 8;
            __IO uint32_t r_PLL_DIV1_SETTING: 8;
            __IO uint32_t R_SPORT0_PLL_SRC_CLK_SEL: 1;
            __IO uint32_t R_SPORT1_PLL_SRC_CLK_SEL: 1;
            __IO uint32_t RESERVED196: 6;
        } BITS_224;
    } u_224;

    /* 0x0228       0x4000_0228
        2:0     R/W    RESERVED217                             3'h1
        3       R/W    r_SPORT0_MCLK_OUT                       1'h0
        4       R/W    r_SPORT1_MCLK_OUT                       1'h0
        5       R/W    r_SPORT2_MCLK_OUT                       1'h0
        6       R/W    RESERVED213                             1'h0
        7       R/W    r_AUDIO_CLK_FROM_PLL                    1'h0
        8       R/W    r_SPORT1_EXT_CODEC                      1'h0
        12:9    R/W    RESERVED210                             4'b0
        15:13   R/W    r_SPORT2_PLL_CLK_SEL                    3'b0
        18:16   R/W    RESERVED208                             3'b0
        19      R/W    r_SPORT2_EXT_CODEC                      1'h0
        31:20   R/W    RESERVED206                             12'b0
    */
    union
    {
        __IO uint32_t SOC_AUDIO_CLK_CTRL_B;
        struct
        {
            __IO uint32_t RESERVED217: 3;
            __IO uint32_t r_SPORT0_MCLK_OUT: 1;
            __IO uint32_t r_SPORT1_MCLK_OUT: 1;
            __IO uint32_t r_SPORT2_MCLK_OUT: 1;
            __IO uint32_t RESERVED213: 1;
            __IO uint32_t r_AUDIO_CLK_FROM_PLL: 1;
            __IO uint32_t r_SPORT1_EXT_CODEC: 1;
            __IO uint32_t RESERVED210: 4;
            __IO uint32_t r_SPORT2_PLL_CLK_SEL: 3;
            __IO uint32_t RESERVED208: 3;
            __IO uint32_t r_SPORT2_EXT_CODEC: 1;
            __IO uint32_t RESERVED206: 12;
        } BITS_228;
    } u_228;

    /* 0x022C       0x4000_022c
        31:0    R/W    RESERVED219                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x022C;
        struct
        {
            __IO uint32_t RESERVED219: 32;
        } BITS_22C;
    } u_22C;

    /* 0x0230       0x4000_0230
        0       R/W    R_CKE_GPIO0_DEB                         1'h1
        1       R/W    R_CKE_GPIO1_DEB                         1'h0
        2       R/W    BIT_SOC_CKE_PLFM                        1'h1
        3       R/W    r_CKE_CTRLAP                            1'h1
        4       R/W    BIT_CKE_BUS_RAM_SLP                     1'h0
        5       R/W    BIT_CKE_BT_VEN                          1'h1
        6       R/W    BIT_SOC_ACTCK_VENDOR_REG_EN             1'h0
        7       R/W    BIT_SOC_SLPCK_VENDOR_REG_EN             1'h0
        8       R/W    BIT_SOC_ACTCK_FLASH_EN                  1'h1
        9       R/W    BIT_SOC_SLPCK_FLASH_EN                  1'h0
        13:10   R/W    RESERVED238                             4'h0
        14      R/W    BIT_SOC_ACTCK_TIMER0_EN                 1'h0
        15      R/W    BIT_SOC_SLPCK_TIMER0_EN                 1'h0
        16      R/W    BIT_SOC_ACTCK_GDMA0_EN                  1'h0
        17      R/W    BIT_SOC_SLPCK_GDMA0_EN                  1'h0
        18      R/W    BIT_SOC_ACTCK_FLASH1_EN                 1'h0
        19      R/W    BIT_SOC_SLPCK_FLASH1_EN                 1'h0
        20      R/W    BIT_SOC_ACTCK_FLASH2_EN                 1'h0
        21      R/W    BIT_SOC_SLPCK_FLASH2_EN                 1'h0
        22      R/W    BIT_SOC_ACTCK_GPIO1_EN                  1'h0
        23      R/W    BIT_SOC_SLPCK_GPIO1_EN                  1'h0
        24      R/W    BIT_SOC_ACTCK_GPIO_EN                   1'h0
        25      R/W    BIT_SOC_SLPCK_GPIO_EN                   1'h0
        26      R/W    BIT_SOC_ACTCK_SDH_EN                    1'h0
        27      R/W    BIT_SOC_SLPCK_SDH_EN                    1'h0
        28      R/W    RESERVED223                             1'h0
        29      R/W    RESERVED222                             1'h0
        31:30   R/W    RESERVED221                             2'h0
    */
    union
    {
        __IO uint32_t PESOC_CLK_CTRL;
        struct
        {
            __IO uint32_t R_CKE_GPIO0_DEB: 1;
            __IO uint32_t R_CKE_GPIO1_DEB: 1;
            __IO uint32_t BIT_SOC_CKE_PLFM: 1;
            __IO uint32_t r_CKE_CTRLAP: 1;
            __IO uint32_t BIT_CKE_BUS_RAM_SLP: 1;
            __IO uint32_t BIT_CKE_BT_VEN: 1;
            __IO uint32_t BIT_SOC_ACTCK_VENDOR_REG_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_VENDOR_REG_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_FLASH_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_FLASH_EN: 1;
            __IO uint32_t RESERVED238: 4;
            __IO uint32_t BIT_SOC_ACTCK_TIMER0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_TIMER0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_GDMA0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_GDMA0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_FLASH1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_FLASH1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_FLASH2_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_FLASH2_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_GPIO1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_GPIO1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_GPIO_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_GPIO_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_SDH_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SDH_EN: 1;
            __IO uint32_t RESERVED223: 1;
            __IO uint32_t RESERVED222: 1;
            __IO uint32_t RESERVED221: 2;
        } BITS_230;
    } u_230;

    /* 0x0234       0x4000_0234
        1:0     R/W    RESERVED280                             2'h0
        2       R/W    BIT_SOC_ACTCK_SPI0_S                    1'h0
        3       R/W    BIT_SOC_SLPCK_SPI0_S                    1'h0
        4       R/W    BIT_CKE_MODEM                           1'h1
        5       R/W    BIT_CKE_CAL32K                          1'h0
        6       R/W    BIT_CKE_SWR_SS                          1'h0
        7       R/W    RESERVED274                             1'h0
        8       R/W    BIT_CKE_RNG                             1'h0
        9       R/W    BIT_CKE_PDCK                            1'h0
        10      R/W    BIT_CKE_AAC_XTAL                        1'h0
        11      R/W    BIT_CKE_SHA256                          1'h0
        12      R/W    BIT_CKE_SM3                             1'h0
        13      R/W    RESERVED268                             1'h0
        14      R/W    BIT_SOC_ACTCK_TIMER1                    1'h0
        15      R/W    BIT_SOC_SLPCK_TIMER1                    1'h0
        16      R/W    BIT_SOC_ACTCK_SPI0_EN                   1'h0
        17      R/W    BIT_SOC_SLPCK_SPI0_EN                   1'h0
        18      R/W    BIT_SOC_ACTCK_SPI1_EN                   1'h0
        19      R/W    BIT_SOC_SLPCK_SPI1_EN                   1'h0
        20      R/W    BIT_SOC_ACTCK_IRRC                      1'h0
        21      R/W    BIT_SOC_SLPCK_IRRC                      1'h0
        22      R/W    BIT_SOC_ACTCK_SPI2_EN                   1'h0
        23      R/W    BIT_SOC_SLPCK_SPI2_EN                   1'h0
        24      R/W    BIT_SOC_ACTCK_CAN0                      1'h0
        25      R/W    BIT_SOC_SLPCK_CAN0                      1'h0
        26      R/W    BIT_SOC_ACTCK_PPE                       1'h0
        27      R/W    BIT_SOC_SLPCK_PPE                       1'h0
        28      R/W    BIT_SOC_ACTCK_PKE                       1'h0
        29      R/W    BIT_SOC_SLPCK_PKE                       1'h0
        30      R/W    BIT_SOC_ACTCK_RTZIP                     1'h0
        31      R/W    BIT_SOC_SLPCK_RTZIP                     1'h0
    */
    union
    {
        __IO uint32_t PESOC_PERI_CLK_CTRL0;
        struct
        {
            __IO uint32_t RESERVED280: 2;
            __IO uint32_t BIT_SOC_ACTCK_SPI0_S: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI0_S: 1;
            __IO uint32_t BIT_CKE_MODEM: 1;
            __IO uint32_t BIT_CKE_CAL32K: 1;
            __IO uint32_t BIT_CKE_SWR_SS: 1;
            __IO uint32_t RESERVED274: 1;
            __IO uint32_t BIT_CKE_RNG: 1;
            __IO uint32_t BIT_CKE_PDCK: 1;
            __IO uint32_t BIT_CKE_AAC_XTAL: 1;
            __IO uint32_t BIT_CKE_SHA256: 1;
            __IO uint32_t BIT_CKE_SM3: 1;
            __IO uint32_t RESERVED268: 1;
            __IO uint32_t BIT_SOC_ACTCK_TIMER1: 1;
            __IO uint32_t BIT_SOC_SLPCK_TIMER1: 1;
            __IO uint32_t BIT_SOC_ACTCK_SPI0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_SPI1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_IRRC: 1;
            __IO uint32_t BIT_SOC_SLPCK_IRRC: 1;
            __IO uint32_t BIT_SOC_ACTCK_SPI2_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SPI2_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_CAN0: 1;
            __IO uint32_t BIT_SOC_SLPCK_CAN0: 1;
            __IO uint32_t BIT_SOC_ACTCK_PPE: 1;
            __IO uint32_t BIT_SOC_SLPCK_PPE: 1;
            __IO uint32_t BIT_SOC_ACTCK_PKE: 1;
            __IO uint32_t BIT_SOC_SLPCK_PKE: 1;
            __IO uint32_t BIT_SOC_ACTCK_RTZIP: 1;
            __IO uint32_t BIT_SOC_SLPCK_RTZIP: 1;
        } BITS_234;
    } u_234;

    /* 0x0238       0x4000_0238
        0       R/W    BIT_SOC_ACTCK_I2C0_EN                   1'h0
        1       R/W    BIT_SOC_SLPCK_I2C0_EN                   1'h0
        2       R/W    BIT_SOC_ACTCK_I2C1_EN                   1'h0
        3       R/W    BIT_SOC_SLPCK_I2C1_EN                   1'h0
        4       R/W    BIT_SOC_ACTCK_QDEC_EN                   1'h0
        5       R/W    BIT_SOC_SLPCK_QDEC_EN                   1'h0
        6       R/W    BIT_SOC_ACTCK_KEYSCAN_EN                1'h0
        7       R/W    BIT_SOC_SLPCK_KEYSCAN_EN                1'h0
        8       R/W    BIT_SOC_ACTCK_AES_EN                    1'h0
        9       R/W    BIT_SOC_SLPCK_AES_EN                    1'h0
        10      R/W    BIT_SOC_ACTCK_SIMC_EN                   1'h0
        11      R/W    BIT_SOC_SLPCK_SIMC_EN                   1'h0
        12      R/W    BIT_SOC_ACTCK_I2C2_EN                   1'h0
        13      R/W    BIT_SOC_SLPCK_I2C2_EN                   1'h0
        14      R/W    BIT_SOC_ACTCK_DATA_MEM0_EN              1'h0
        15      R/W    BIT_SOC_SLPCK_DATA_MEM0_EN              1'h0
        16      R/W    BIT_SOC_ACTCK_DATA_MEM1_EN              1'h0
        17      R/W    BIT_SOC_SLPCK_DATA_MEM1_EN              1'h0
        18      R/W    BIT_SOC_ACTCK_LCD_EN                    1'h0
        19      R/W    BIT_SOC_SLPCK_LCD_EN                    1'h0
        20      R/W    BIT_SOC_ACTCKE_ASRC                     1'h0
        21      R/W    BIT_SOC_SLPCKE_ASRC                     1'h0
        22      R/W    BIT_SOC_ACTCKE_DSP_MEM                  1'h0
        23      R/W    BIT_SOC_SLPCKE_DSP_MEM                  1'h0
        24      R/W    BIT_SOC_ACTCK_ADC_EN                    1'h0
        25      R/W    BIT_SOC_SLPCK_ADC_EN                    1'h0
        26      R/W    BIT_SOC_ACTCKE_H2D_D2H                  1'h0
        27      R/W    BIT_SOC_SLPCKE_H2D_D2H                  1'h0
        28      R/W    BIT_SOC_ACTCKE_DSP                      1'h0
        29      R/W    BIT_SOC_SLPCKE_DSP                      1'h0
        30      R/W    BIT_SOC_CKE_DSP_WDT                     1'h0
        31      R/W    BIT_SOC_CLK_EFUSE                       1'h0
    */
    union
    {
        __IO uint32_t PESOC_PERI_CLK_CTRL1;
        struct
        {
            __IO uint32_t BIT_SOC_ACTCK_I2C0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_I2C0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_I2C1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_I2C1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_QDEC_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_QDEC_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_KEYSCAN_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_KEYSCAN_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_AES_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_AES_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_SIMC_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_SIMC_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_I2C2_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_I2C2_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_DATA_MEM0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_DATA_MEM0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_DATA_MEM1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_DATA_MEM1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_LCD_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_LCD_EN: 1;
            __IO uint32_t BIT_SOC_ACTCKE_ASRC: 1;
            __IO uint32_t BIT_SOC_SLPCKE_ASRC: 1;
            __IO uint32_t BIT_SOC_ACTCKE_DSP_MEM: 1;
            __IO uint32_t BIT_SOC_SLPCKE_DSP_MEM: 1;
            __IO uint32_t BIT_SOC_ACTCK_ADC_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_ADC_EN: 1;
            __IO uint32_t BIT_SOC_ACTCKE_H2D_D2H: 1;
            __IO uint32_t BIT_SOC_SLPCKE_H2D_D2H: 1;
            __IO uint32_t BIT_SOC_ACTCKE_DSP: 1;
            __IO uint32_t BIT_SOC_SLPCKE_DSP: 1;
            __IO uint32_t BIT_SOC_CKE_DSP_WDT: 1;
            __IO uint32_t BIT_SOC_CLK_EFUSE: 1;
        } BITS_238;
    } u_238;

    /* 0x023C       0x4000_023c
        0       R/W    r_epalna_od                             1'h0
        1       R/W    r_spic_dbg_dis                          1'h0
        3:2     R/W    r_SPIC_MON_SEL                          2'h0
        13:4    R/W    RESERVED333                             10'h0
        14      R/W    BIT_SOC_ACTCK_ZIGBEE_EN                 1'h0
        15      R/W    BIT_SOC_SLPCK_ZIGBEE_EN                 1'h0
        16      R/W    BIT_SOC_ACTCK_JPEG_EN                   1'h0
        17      R/W    BIT_SOC_SLPCK_JPEG_EN                   1'h0
        18      R/W    BIT_SOC_ACTCK_UART0_EN                  1'h0
        19      R/W    BIT_SOC_SLPCK_UART0_EN                  1'h0
        20      R/W    BIT_SOC_ACTCK_UART1_EN                  1'h0
        21      R/W    BIT_SOC_SLPCK_UART1_EN                  1'h0
        22      R/W    BIT_SOC_ACTCK_UART2_EN                  1'h0
        23      R/W    BIT_SOC_SLPCK_UART2_EN                  1'h0
        24      R/W    BIT_SOC_ACTCK_UART3_EN                  1'h0
        25      R/W    BIT_SOC_SLPCK_UART3_EN                  1'h0
        26      R/W    BIT_SOC_ACTCK_UART4_EN                  1'h0
        27      R/W    BIT_SOC_SLPCK_UART4_EN                  1'h0
        28      R/W    BIT_SOC_ACTCK_UART5_EN                  1'h0
        29      R/W    BIT_SOC_SLPCK_UART5_EN                  1'h0
        30      R/W    BIT_SOC_ACTCK_CAN1_EN                   1'h0
        31      R/W    BIT_SOC_SLPCK_CAN1_EN                   1'h0
    */
    union
    {
        __IO uint32_t REG_TESTMODE_SEL_RF;
        struct
        {
            __IO uint32_t r_epalna_od: 1;
            __IO uint32_t r_spic_dbg_dis: 1;
            __IO uint32_t r_SPIC_MON_SEL: 2;
            __IO uint32_t RESERVED333: 10;
            __IO uint32_t BIT_SOC_ACTCK_ZIGBEE_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_ZIGBEE_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_JPEG_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_JPEG_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART0_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART0_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART1_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART2_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART2_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART3_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART3_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART4_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART4_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_UART5_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_UART5_EN: 1;
            __IO uint32_t BIT_SOC_ACTCK_CAN1_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_CAN1_EN: 1;
        } BITS_23C;
    } u_23C;

    /* 0x0240       0x4000_0240
        31:0    R      RESERVED338                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x0240;
        struct
        {
            __I uint32_t RESERVED338: 32;
        } BITS_240;
    } u_240;

    /* 0x0244       0x4000_0244
        0       R/W    BIT_SOC_ACTCK_BTBUS_EN                  1'h1
        1       R/W    BIT_SOC_SLPCK_BTBUS_EN                  1'h1
        31:2    R/W    RESERVED340                             30'h0
    */
    union
    {
        __IO uint32_t OFF_MEM_PWR_CTRL;
        struct
        {
            __IO uint32_t BIT_SOC_ACTCK_BTBUS_EN: 1;
            __IO uint32_t BIT_SOC_SLPCK_BTBUS_EN: 1;
            __IO uint32_t RESERVED340: 30;
        } BITS_244;
    } u_244;

    /* 0x0248       0x4000_0248
        7:0     R/W    RESERVED346                             8'h0
        10:8    R/W    r_swr_ss_div_sel                        3'h0
        31:11   R/W    RESERVED344                             21'h0
    */
    union
    {
        __IO uint32_t REG_0x248;
        struct
        {
            __IO uint32_t RESERVED346: 8;
            __IO uint32_t r_swr_ss_div_sel: 3;
            __IO uint32_t RESERVED344: 21;
        } BITS_248;
    } u_248;

    /* 0x024C       0x4000_024c
        31:0    R      RESERVED348                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x024C;
        struct
        {
            __I uint32_t RESERVED348: 32;
        } BITS_24C;
    } u_24C;

    /* 0x0250       0x4000_0250
        31:0    R      RESERVED350                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x0250;
        struct
        {
            __I uint32_t RESERVED350: 32;
        } BITS_250;
    } u_250;

    /* 0x0254       0x4000_0254
        0       R/W    DSP_RUN_STALL                           1'h1
        1       R/W    DSP_STAT_VECTOR_SEL                     1'h0
        2       R/W    RESERVED361                             1'h0
        14:3    R/W    DUMMY                                   12'h0
        15      R/W    RESERVED359                             1'h0
        19:16   R/W    r_cpu_low_rate_valid_num1               4'h3
        23:20   R/W    r_dsp_low_rate_valid_num1               4'h3
        24      R/W    r_cpu_auto_slow_filter1_en              1'h0
        25      R/W    r_dsp_auto_slow_filter1_en              1'h0
        29:26   R/W    RESERVED354                             4'h0
        30      R      RESERVED353                             1'h0
        31      R/W    RESERVED352                             1'h0
    */
    union
    {
        __IO uint32_t REG_0x254;
        struct
        {
            __IO uint32_t DSP_RUN_STALL: 1;
            __IO uint32_t DSP_STAT_VECTOR_SEL: 1;
            __IO uint32_t RESERVED361: 1;
            __IO uint32_t RESERVED_0: 12;
            __IO uint32_t RESERVED359: 1;
            __IO uint32_t r_cpu_low_rate_valid_num1: 4;
            __IO uint32_t r_dsp_low_rate_valid_num1: 4;
            __IO uint32_t r_cpu_auto_slow_filter1_en: 1;
            __IO uint32_t r_dsp_auto_slow_filter1_en: 1;
            __IO uint32_t RESERVED354: 4;
            __I uint32_t RESERVED353: 1;
            __IO uint32_t RESERVED352: 1;
        } BITS_254;
    } u_254;

    /* 0x0258       0x4000_0258
        1:0     R/W    CORE0_TUNE_OCP_RES                      2'h0
        4:2     R/W    CORE0_TUNE_PWM_R3                       3'h0
        7:5     R/W    CORE0_TUNE_PWM_R2                       3'h0
        10:8    R/W    CORE0_TUNE_PWM_R1                       3'h0
        13:11   R/W    CORE0_TUNE_PWM_C3                       3'h0
        16:14   R/W    CORE0_TUNE_PWM_C2                       3'h0
        19:17   R/W    CORE0_TUNE_PWM_C1                       3'h0
        20      R/W    CORE0_BYPASS_PWM_BYPASS_RoughSS         1'h0
        22:21   R/W    CORE0_BYPASS_PWM_TUNE_RoughSS           2'h0
        25:23   R/W    CORE0_BYPASS_PWM_TUNE_VCL               3'h0
        28:26   R/W    CORE0_BYPASS_PWM_TUNE_VCH               3'h0
        29      R/W    CORE0_X4_PWM_COMP_IB                    1'h0
        30      R/W    CORE0_X4_POW_PWM_CLP                    1'h0
        31      R/W    CORE0_X4_TUNE_VDIV_Bit0                 1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR0_31_0;
        struct
        {
            __IO uint32_t CORE0_TUNE_OCP_RES: 2;
            __IO uint32_t CORE0_TUNE_PWM_R3: 3;
            __IO uint32_t CORE0_TUNE_PWM_R2: 3;
            __IO uint32_t CORE0_TUNE_PWM_R1: 3;
            __IO uint32_t CORE0_TUNE_PWM_C3: 3;
            __IO uint32_t CORE0_TUNE_PWM_C2: 3;
            __IO uint32_t CORE0_TUNE_PWM_C1: 3;
            __IO uint32_t CORE0_BYPASS_PWM_BYPASS_RoughSS: 1;
            __IO uint32_t CORE0_BYPASS_PWM_TUNE_RoughSS: 2;
            __IO uint32_t CORE0_BYPASS_PWM_TUNE_VCL: 3;
            __IO uint32_t CORE0_BYPASS_PWM_TUNE_VCH: 3;
            __IO uint32_t CORE0_X4_PWM_COMP_IB: 1;
            __IO uint32_t CORE0_X4_POW_PWM_CLP: 1;
            __IO uint32_t CORE0_X4_TUNE_VDIV_Bit0: 1;
        } BITS_258;
    } u_258;

    /* 0x025C       0x4000_025c
        6:0     R/W    CORE0_X4_TUNE_VDIV_Bit7_Bit1            7'h0
        14:7    R/W    CORE0_BYPASS_PWM_TUNE_POS_VREFPFM       8'h0
        17:15   R/W    CORE0_BYPASS_PWM_TUNE_POS_VREFOCP       3'h0
        18      R/W    CORE0_FPWM                              1'h0
        19      R/W    CORE0_POW_PFM                           1'h0
        20      R/W    CORE0_POW_PWM                           1'h0
        21      R/W    CORE0_POW_VDIV                          1'h0
        23:22   R/W    CORE0_XTAL_OV_RATIO                     2'h0
        26:24   R/W    CORE0_XTAL_OV_UNIT                      3'h0
        29:27   R/W    CORE0_XTAL_OV_MODE                      3'h0
        30      R/W    CORE0_EN_POWERMOS_DR8X                  1'h0
        31      R/W    CORE0_SEL_OCP_TABLE                     1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR0_63_32;
        struct
        {
            __IO uint32_t CORE0_X4_TUNE_VDIV_Bit7_Bit1: 7;
            __IO uint32_t CORE0_BYPASS_PWM_TUNE_POS_VREFPFM: 8;
            __IO uint32_t CORE0_BYPASS_PWM_TUNE_POS_VREFOCP: 3;
            __IO uint32_t CORE0_FPWM: 1;
            __IO uint32_t CORE0_POW_PFM: 1;
            __IO uint32_t CORE0_POW_PWM: 1;
            __IO uint32_t CORE0_POW_VDIV: 1;
            __IO uint32_t CORE0_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE0_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE0_XTAL_OV_MODE: 3;
            __IO uint32_t CORE0_EN_POWERMOS_DR8X: 1;
            __IO uint32_t CORE0_SEL_OCP_TABLE: 1;
        } BITS_25C;
    } u_25C;

    /* 0x0260       0x4000_0260
        1:0     R/W    CORE4_TUNE_OCP_RES                      2'h0
        4:2     R/W    CORE4_TUNE_PWM_R3                       3'h0
        7:5     R/W    CORE4_TUNE_PWM_R2                       3'h0
        10:8    R/W    CORE4_TUNE_PWM_R1                       3'h0
        13:11   R/W    CORE4_TUNE_PWM_C3                       3'h0
        16:14   R/W    CORE4_TUNE_PWM_C2                       3'h0
        19:17   R/W    CORE4_TUNE_PWM_C1                       3'h0
        20      R/W    CORE4_BYPASS_PWM_BYPASS_RoughSS         1'h0
        22:21   R/W    CORE4_BYPASS_PWM_TUNE_RoughSS           2'h0
        25:23   R/W    CORE4_BYPASS_PWM_TUNE_VCL               3'h0
        28:26   R/W    CORE4_BYPASS_PWM_TUNE_VCH               3'h0
        29      R/W    CORE4_X4_PWM_COMP_IB                    1'h0
        30      R/W    CORE4_X4_POW_PWM_CLP                    1'h0
        31      R/W    CORE4_X4_TUNE_VDIV_Bit0                 1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR4_31_0;
        struct
        {
            __IO uint32_t CORE4_TUNE_OCP_RES: 2;
            __IO uint32_t CORE4_TUNE_PWM_R3: 3;
            __IO uint32_t CORE4_TUNE_PWM_R2: 3;
            __IO uint32_t CORE4_TUNE_PWM_R1: 3;
            __IO uint32_t CORE4_TUNE_PWM_C3: 3;
            __IO uint32_t CORE4_TUNE_PWM_C2: 3;
            __IO uint32_t CORE4_TUNE_PWM_C1: 3;
            __IO uint32_t CORE4_BYPASS_PWM_BYPASS_RoughSS: 1;
            __IO uint32_t CORE4_BYPASS_PWM_TUNE_RoughSS: 2;
            __IO uint32_t CORE4_BYPASS_PWM_TUNE_VCL: 3;
            __IO uint32_t CORE4_BYPASS_PWM_TUNE_VCH: 3;
            __IO uint32_t CORE4_X4_PWM_COMP_IB: 1;
            __IO uint32_t CORE4_X4_POW_PWM_CLP: 1;
            __IO uint32_t CORE4_X4_TUNE_VDIV_Bit0: 1;
        } BITS_260;
    } u_260;

    /* 0x0264       0x4000_0264
        6:0     R/W    CORE4_X4_TUNE_VDIV_Bit7_Bit1            7'h0
        14:7    R/W    CORE4_BYPASS_PWM_TUNE_POS_VREFPFM       8'h0
        17:15   R/W    CORE4_BYPASS_PWM_TUNE_POS_VREFOCP       3'h0
        18      R/W    CORE4_FPWM                              1'h0
        19      R/W    CORE4_POW_PFM                           1'h0
        20      R/W    CORE4_POW_PWM                           1'h0
        21      R/W    CORE4_POW_VDIV                          1'h0
        23:22   R/W    CORE4_XTAL_OV_RATIO                     2'h0
        26:24   R/W    CORE4_XTAL_OV_UNIT                      3'h0
        29:27   R/W    CORE4_XTAL_OV_MODE                      3'h0
        30      R/W    CORE4_EN_POWERMOS_DR8X                  1'h0
        31      R/W    CORE4_SEL_OCP_TABLE                     1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR4_63_32;
        struct
        {
            __IO uint32_t CORE4_X4_TUNE_VDIV_Bit7_Bit1: 7;
            __IO uint32_t CORE4_BYPASS_PWM_TUNE_POS_VREFPFM: 8;
            __IO uint32_t CORE4_BYPASS_PWM_TUNE_POS_VREFOCP: 3;
            __IO uint32_t CORE4_FPWM: 1;
            __IO uint32_t CORE4_POW_PFM: 1;
            __IO uint32_t CORE4_POW_PWM: 1;
            __IO uint32_t CORE4_POW_VDIV: 1;
            __IO uint32_t CORE4_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE4_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE4_XTAL_OV_MODE: 3;
            __IO uint32_t CORE4_EN_POWERMOS_DR8X: 1;
            __IO uint32_t CORE4_SEL_OCP_TABLE: 1;
        } BITS_264;
    } u_264;

    /* 0x0268       0x4000_0268
        1:0     R/W    CORE5_TUNE_OCP_RES                      2'h0
        4:2     R/W    CORE5_TUNE_PWM_R3                       3'h0
        7:5     R/W    CORE5_TUNE_PWM_R2                       3'h0
        10:8    R/W    CORE5_TUNE_PWM_R1                       3'h0
        13:11   R/W    CORE5_TUNE_PWM_C3                       3'h0
        16:14   R/W    CORE5_TUNE_PWM_C2                       3'h0
        19:17   R/W    CORE5_TUNE_PWM_C1                       3'h0
        20      R/W    CORE5_BYPASS_PWM_BYPASS_RoughSS         1'h0
        22:21   R/W    CORE5_BYPASS_PWM_TUNE_RoughSS           2'h0
        25:23   R/W    CORE5_BYPASS_PWM_TUNE_VCL               3'h0
        28:26   R/W    CORE5_BYPASS_PWM_TUNE_VCH               3'h0
        29      R/W    CORE5_X4_PWM_COMP_IB                    1'h0
        30      R/W    CORE5_X4_POW_PWM_CLP                    1'h0
        31      R/W    CORE5_X4_TUNE_VDIV_Bit0                 1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR5_31_0;
        struct
        {
            __IO uint32_t CORE5_TUNE_OCP_RES: 2;
            __IO uint32_t CORE5_TUNE_PWM_R3: 3;
            __IO uint32_t CORE5_TUNE_PWM_R2: 3;
            __IO uint32_t CORE5_TUNE_PWM_R1: 3;
            __IO uint32_t CORE5_TUNE_PWM_C3: 3;
            __IO uint32_t CORE5_TUNE_PWM_C2: 3;
            __IO uint32_t CORE5_TUNE_PWM_C1: 3;
            __IO uint32_t CORE5_BYPASS_PWM_BYPASS_RoughSS: 1;
            __IO uint32_t CORE5_BYPASS_PWM_TUNE_RoughSS: 2;
            __IO uint32_t CORE5_BYPASS_PWM_TUNE_VCL: 3;
            __IO uint32_t CORE5_BYPASS_PWM_TUNE_VCH: 3;
            __IO uint32_t CORE5_X4_PWM_COMP_IB: 1;
            __IO uint32_t CORE5_X4_POW_PWM_CLP: 1;
            __IO uint32_t CORE5_X4_TUNE_VDIV_Bit0: 1;
        } BITS_268;
    } u_268;

    /* 0x026C       0x4000_026c
        6:0     R/W    CORE5_X4_TUNE_VDIV_Bit7_Bit1            7'h0
        14:7    R/W    CORE5_BYPASS_PWM_TUNE_POS_VREFPFM       8'h0
        17:15   R/W    CORE5_BYPASS_PWM_TUNE_POS_VREFOCP       3'h0
        18      R/W    CORE5_FPWM                              1'h0
        19      R/W    CORE5_POW_PFM                           1'h0
        20      R/W    CORE5_POW_PWM                           1'h0
        21      R/W    CORE5_POW_VDIV                          1'h0
        23:22   R/W    CORE5_XTAL_OV_RATIO                     2'h0
        26:24   R/W    CORE5_XTAL_OV_UNIT                      3'h0
        29:27   R/W    CORE5_XTAL_OV_MODE                      3'h0
        30      R/W    CORE5_EN_POWERMOS_DR8X                  1'h0
        31      R/W    CORE5_SEL_OCP_TABLE                     1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR5_63_32;
        struct
        {
            __IO uint32_t CORE5_X4_TUNE_VDIV_Bit7_Bit1: 7;
            __IO uint32_t CORE5_BYPASS_PWM_TUNE_POS_VREFPFM: 8;
            __IO uint32_t CORE5_BYPASS_PWM_TUNE_POS_VREFOCP: 3;
            __IO uint32_t CORE5_FPWM: 1;
            __IO uint32_t CORE5_POW_PFM: 1;
            __IO uint32_t CORE5_POW_PWM: 1;
            __IO uint32_t CORE5_POW_VDIV: 1;
            __IO uint32_t CORE5_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE5_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE5_XTAL_OV_MODE: 3;
            __IO uint32_t CORE5_EN_POWERMOS_DR8X: 1;
            __IO uint32_t CORE5_SEL_OCP_TABLE: 1;
        } BITS_26C;
    } u_26C;

    /* 0x0270       0x4000_0270
        0       R/W    CORE0_EN_HVD17_LOWIQ                    1'h0
        4:1     R/W    CORE0_TUNE_HVD17_IB                     4'h0
        5       R/W    CORE0_X4_PFM_COMP_IB                    1'h0
        8:6     R/W    CORE0_TUNE_PFM_VREFOCPPFM               3'h0
        14:9    R/W    CORE0_TUNE_SAW_ICLK                     6'h0
        15      R/W    RESERVED455                             1'h0
        16      R/W    CORE4_EN_HVD17_LOWIQ                    1'h0
        20:17   R/W    CORE4_TUNE_HVD17_IB                     4'h0
        21      R/W    CORE4_X4_PFM_COMP_IB                    1'h0
        24:22   R/W    CORE4_TUNE_PFM_VREFOCPPFM               3'h0
        30:25   R/W    CORE4_TUNE_SAW_ICLK                     6'h0
        31      R/W    RESERVED449                             1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR4_79_64_AUTO_SW_PAR0_79_64;
        struct
        {
            __IO uint32_t CORE0_EN_HVD17_LOWIQ: 1;
            __IO uint32_t CORE0_TUNE_HVD17_IB: 4;
            __IO uint32_t CORE0_X4_PFM_COMP_IB: 1;
            __IO uint32_t CORE0_TUNE_PFM_VREFOCPPFM: 3;
            __IO uint32_t CORE0_TUNE_SAW_ICLK: 6;
            __IO uint32_t RESERVED455: 1;
            __IO uint32_t CORE4_EN_HVD17_LOWIQ: 1;
            __IO uint32_t CORE4_TUNE_HVD17_IB: 4;
            __IO uint32_t CORE4_X4_PFM_COMP_IB: 1;
            __IO uint32_t CORE4_TUNE_PFM_VREFOCPPFM: 3;
            __IO uint32_t CORE4_TUNE_SAW_ICLK: 6;
            __IO uint32_t RESERVED449: 1;
        } BITS_270;
    } u_270;

    /* 0x0274       0x4000_0274
        19:0    R/W    r_dss_data_in                           20'h0
        22:20   R/W    r_dss_ro_sel                            3'h0
        23      R/W    r_dss_wire_sel                          1'h0
        24      R/W    r_dss_clk_en                            1'h0
        25      R/W    r_dss_speed_en                          1'h0
        26      R/W    r_FEN_DSS                               1'h0
        31:27   R/W    RESERVED462                             5'h0
    */
    union
    {
        __IO uint32_t REG_DSS_CTRL;
        struct
        {
            __IO uint32_t r_dss_data_in: 20;
            __IO uint32_t r_dss_ro_sel: 3;
            __IO uint32_t r_dss_wire_sel: 1;
            __IO uint32_t r_dss_clk_en: 1;
            __IO uint32_t r_dss_speed_en: 1;
            __IO uint32_t r_FEN_DSS: 1;
            __IO uint32_t RESERVED462: 5;
        } BITS_274;
    } u_274;

    /* 0x0278       0x4000_0278
        19:0    R      bset_dss_count_out                      20'h0
        20      R      bset_dss_wsort_go                       1'h0
        21      R      bset_dss_ready                          1'h0
        31:22   R      RESERVED470                             10'h0
    */
    union
    {
        __IO uint32_t REG_BEST_DSS_RD;
        struct
        {
            __I uint32_t bset_dss_count_out: 20;
            __I uint32_t bset_dss_wsort_go: 1;
            __I uint32_t bset_dss_ready: 1;
            __I uint32_t RESERVED470: 10;
        } BITS_278;
    } u_278;

    /* 0x027C       0x4000_027c
        19:0    R      dss_ir_count_out                        20'h0
        20      R      dss_ir_wsort_go                         1'h0
        21      R      dss_ir_ready                            1'h0
        31:22   R      RESERVED475                             10'h0
    */
    union
    {
        __IO uint32_t REG_BEST_DSS_IR_RD;
        struct
        {
            __I uint32_t dss_ir_count_out: 20;
            __I uint32_t dss_ir_wsort_go: 1;
            __I uint32_t dss_ir_ready: 1;
            __I uint32_t RESERVED475: 10;
        } BITS_27C;
    } u_27C;

    __IO uint32_t RSVD_0x280[9];

    /* 0x02A4       0x4000_02a4
        31:0    R/W    RESERVED480                             32'h0
    */
    union
    {
        __IO uint32_t REG_GPIOE_dummy;
        struct
        {
            __IO uint32_t RESERVED480: 32;
        } BITS_2A4;
    } u_2A4;

    /* 0x02A8       0x4000_02a8
        3:0     R/W    PMUX_TEST_MODE                          4'h1
        6:4     R/W    RESERVED503                             3'h0
        7       R/W    PMUX_TEST_MODE_EN                       1'h0
        8       R/W    PMUX_DBG_INF_EN                         1'h0
        9       R/W    PMUX_DBG_FLASH_INF_EN                   1'h0
        10      R/W    PMUX_SPI_DMA_REQ_EN                     1'h0
        11      R/W    PMUX_OPI_EN                             1'h0
        12      R/W    PMUX_LCD_EN                             1'h0
        13      R/W    PMUX_LCD_VSYNC_DIS                      1'h0
        14      R/W    PMUX_LCD_RD_DIS                         1'h0
        15      R/W    PMUX_LCD_VSYNC_IO_SEL                   1'h0
        19:16   R/W    PMUX_DBG_MODE_SEL                       4'h2
        20      R/W    PMUX_FLASH_EXTS_FT_EN                   1'h0
        22:21   R/W    RESERVED491                             2'h0
        23      R/W    r_dbg_cpu_dsp_clk_en                    1'h0
        24      R/W    SPIC_MASTER_EN                          1'h0
        25      R/W    SPIC1_MASTER_EN                         1'h0
        26      R/W    SPIC2_MASTER_EN                         1'h0
        27      R/W    PMUX_FLASH_EXTC_MP_EN                   1'h0
        28      R/W    PMUX_DIG_SMUX_EN                        1'h0
        29      R/W    RESERVED484                             1'h0
        30      R/W    RESERVED483                             1'h0
        31      R/W    RESERVED482                             1'h0
    */
    union
    {
        __IO uint32_t REG_TEST_MODE;
        struct
        {
            __IO uint32_t PMUX_TEST_MODE: 4;
            __IO uint32_t RESERVED503: 3;
            __IO uint32_t PMUX_TEST_MODE_EN: 1;
            __IO uint32_t PMUX_DBG_INF_EN: 1;
            __IO uint32_t PMUX_DBG_FLASH_INF_EN: 1;
            __IO uint32_t PMUX_SPI_DMA_REQ_EN: 1;
            __IO uint32_t PMUX_OPI_EN: 1;
            __IO uint32_t PMUX_LCD_EN: 1;
            __IO uint32_t PMUX_LCD_VSYNC_DIS: 1;
            __IO uint32_t PMUX_LCD_RD_DIS: 1;
            __IO uint32_t PMUX_LCD_VSYNC_IO_SEL: 1;
            __IO uint32_t PMUX_DBG_MODE_SEL: 4;
            __IO uint32_t PMUX_FLASH_EXTS_FT_EN: 1;
            __IO uint32_t RESERVED491: 2;
            __IO uint32_t r_dbg_cpu_dsp_clk_en: 1;
            __IO uint32_t SPIC_MASTER_EN: 1;
            __IO uint32_t SPIC1_MASTER_EN: 1;
            __IO uint32_t SPIC2_MASTER_EN: 1;
            __IO uint32_t PMUX_FLASH_EXTC_MP_EN: 1;
            __IO uint32_t PMUX_DIG_SMUX_EN: 1;
            __IO uint32_t RESERVED484: 1;
            __IO uint32_t RESERVED483: 1;
            __IO uint32_t RESERVED482: 1;
        } BITS_2A8;
    } u_2A8;

    /* 0x02AC       0x4000_02ac
        0       R/W    r_PMUX_UART0_1_W_EN                     1'h0
        1       R/W    r_PMUX_UART1_1_W_EN                     1'h0
        2       R/W    r_PMUX_UART2_1_W_EN                     1'h0
        3       R/W    r_PMUX_UART3_1_W_EN                     1'h0
        4       R/W    r_PMUX_UART4_1_W_EN                     1'h0
        5       R/W    r_PMUX_UART5_1_W_EN                     1'h0
        7:6     R/W    RESERVED523                             2'h0
        8       R/W    r_SPIC0_PULL_SEL_SIO0_PULL_CTRL         1'h0
        9       R/W    r_SPIC0_PULL_SEL_SIO1_PULL_CTRL         1'h0
        10      R/W    r_SPIC0_PULL_SEL_SIO2_PULL_CTRL         1'h0
        11      R/W    r_SPIC0_PULL_SEL_SIO3_PULL_CTRL         1'h0
        12      R/W    r_SPIC1_PULL_SEL_SIO0_PULL_CTRL         1'h0
        13      R/W    r_SPIC1_PULL_SEL_SIO1_PULL_CTRL         1'h0
        14      R/W    r_SPIC1_PULL_SEL_SIO2_PULL_CTRL         1'h0
        15      R/W    r_SPIC1_PULL_SEL_SIO3_PULL_CTRL         1'h0
        16      R/W    r_SPIC2_PULL_SEL_SIO0_PULL_CTRL         1'h0
        17      R/W    r_SPIC2_PULL_SEL_SIO1_PULL_CTRL         1'h0
        18      R/W    r_SPIC2_PULL_SEL_SIO2_PULL_CTRL         1'h0
        19      R/W    r_SPIC2_PULL_SEL_SIO3_PULL_CTRL         1'h0
        20      R/W    r_SPIC3_PULL_SEL_SIO0_PULL_CTRL         1'h0
        21      R/W    r_SPIC3_PULL_SEL_SIO1_PULL_CTRL         1'h0
        22      R/W    r_SPIC3_PULL_SEL_SIO2_PULL_CTRL         1'h0
        23      R/W    r_SPIC3_PULL_SEL_SIO3_PULL_CTRL         1'h0
        31:24   R/W    RESERVED506                             8'h0
    */
    union
    {
        __IO uint32_t REG_TEST_MODE;
        struct
        {
            __IO uint32_t r_PMUX_UART0_1_W_EN: 1;
            __IO uint32_t r_PMUX_UART1_1_W_EN: 1;
            __IO uint32_t r_PMUX_UART2_1_W_EN: 1;
            __IO uint32_t r_PMUX_UART3_1_W_EN: 1;
            __IO uint32_t r_PMUX_UART4_1_W_EN: 1;
            __IO uint32_t r_PMUX_UART5_1_W_EN: 1;
            __IO uint32_t RESERVED523: 2;
            __IO uint32_t r_SPIC0_PULL_SEL_SIO0_PULL_CTRL: 1;
            __IO uint32_t r_SPIC0_PULL_SEL_SIO1_PULL_CTRL: 1;
            __IO uint32_t r_SPIC0_PULL_SEL_SIO2_PULL_CTRL: 1;
            __IO uint32_t r_SPIC0_PULL_SEL_SIO3_PULL_CTRL: 1;
            __IO uint32_t r_SPIC1_PULL_SEL_SIO0_PULL_CTRL: 1;
            __IO uint32_t r_SPIC1_PULL_SEL_SIO1_PULL_CTRL: 1;
            __IO uint32_t r_SPIC1_PULL_SEL_SIO2_PULL_CTRL: 1;
            __IO uint32_t r_SPIC1_PULL_SEL_SIO3_PULL_CTRL: 1;
            __IO uint32_t r_SPIC2_PULL_SEL_SIO0_PULL_CTRL: 1;
            __IO uint32_t r_SPIC2_PULL_SEL_SIO1_PULL_CTRL: 1;
            __IO uint32_t r_SPIC2_PULL_SEL_SIO2_PULL_CTRL: 1;
            __IO uint32_t r_SPIC2_PULL_SEL_SIO3_PULL_CTRL: 1;
            __IO uint32_t r_SPIC3_PULL_SEL_SIO0_PULL_CTRL: 1;
            __IO uint32_t r_SPIC3_PULL_SEL_SIO1_PULL_CTRL: 1;
            __IO uint32_t r_SPIC3_PULL_SEL_SIO2_PULL_CTRL: 1;
            __IO uint32_t r_SPIC3_PULL_SEL_SIO3_PULL_CTRL: 1;
            __IO uint32_t RESERVED506: 8;
        } BITS_2AC;
    } u_2AC;

    /* 0x02B0       0x4000_02b0
        0       R      SPI_FLASH_SEL                           1'h0
        31:1    R/W    RESERVED531                             31'h0
    */
    union
    {
        __IO uint32_t REG_0x2B0;
        struct
        {
            __I uint32_t SPI_FLASH_SEL: 1;
            __IO uint32_t RESERVED531: 31;
        } BITS_2B0;
    } u_2B0;

    /* 0x02B4       0x4000_02b4
        0       R/W    PMUX_SDIO_EN                            1'h0
        1       R/W    PMUX_SDIO_SEL                           1'h0
        3:2     R/W    RESERVED536                             2'h0
        11:4    R/W    PMUX_SDIO_PIN_EN                        8'h0
        31:12   R/W    RESERVED534                             20'h0
    */
    union
    {
        __IO uint32_t REG_0x2B4;
        struct
        {
            __IO uint32_t PMUX_SDIO_EN: 1;
            __IO uint32_t PMUX_SDIO_SEL: 1;
            __IO uint32_t RESERVED536: 2;
            __IO uint32_t PMUX_SDIO_PIN_EN: 8;
            __IO uint32_t RESERVED534: 20;
        } BITS_2B4;
    } u_2B4;

    /* 0x02B8       0x4000_02b8
        0       R/W    SPIC1_QPI_EN                            1'h0
        1       R/W    SPIC0_DDR_MODE_EN                       1'h0
        2       R/W    SPIC2_DDR_MODE_EN                       1'h0
        3       R/W    SPIC0_SCLK_SHIFT_EN                     1'h0
        4       R/W    SPIC2_SCLK_SHIFT_EN                     1'h0
        31:5    R/W    RESERVED540                             27'h0
    */
    union
    {
        __IO uint32_t REG_SPI_OPI_PHY_Ctrl;
        struct
        {
            __IO uint32_t SPIC1_QPI_EN: 1;
            __IO uint32_t SPIC0_DDR_MODE_EN: 1;
            __IO uint32_t SPIC2_DDR_MODE_EN: 1;
            __IO uint32_t SPIC0_SCLK_SHIFT_EN: 1;
            __IO uint32_t SPIC2_SCLK_SHIFT_EN: 1;
            __IO uint32_t RESERVED540: 27;
        } BITS_2B8;
    } u_2B8;

    /* 0x02BC       0x4000_02bc
        31:0    R/W    RESERVED547                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02BC;
        struct
        {
            __IO uint32_t RESERVED547: 32;
        } BITS_2BC;
    } u_2BC;

    /* 0x02C0       0x4000_02c0
        1:0     R/W    CORE6_TUNE_OCP_RES                      2'h0
        4:2     R/W    CORE6_TUNE_PWM_R3                       3'h0
        7:5     R/W    CORE6_TUNE_PWM_R2                       3'h0
        10:8    R/W    CORE6_TUNE_PWM_R1                       3'h0
        13:11   R/W    CORE6_TUNE_PWM_C3                       3'h0
        16:14   R/W    CORE6_TUNE_PWM_C2                       3'h0
        19:17   R/W    CORE6_TUNE_PWM_C1                       3'h0
        20      R/W    CORE6_BYPASS_PWM_BYPASS_RoughSS         1'h0
        22:21   R/W    CORE6_BYPASS_PWM_TUNE_RoughSS           2'h0
        25:23   R/W    CORE6_BYPASS_PWM_TUNE_VCL               3'h0
        28:26   R/W    CORE6_BYPASS_PWM_TUNE_VCH               3'h0
        29      R/W    CORE6_X4_PWM_COMP_IB                    1'h0
        30      R/W    CORE6_X4_POW_PWM_CLP                    1'h0
        31      R/W    CORE6_X4_TUNE_VDIV_Bit0                 1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR6_31_0;
        struct
        {
            __IO uint32_t CORE6_TUNE_OCP_RES: 2;
            __IO uint32_t CORE6_TUNE_PWM_R3: 3;
            __IO uint32_t CORE6_TUNE_PWM_R2: 3;
            __IO uint32_t CORE6_TUNE_PWM_R1: 3;
            __IO uint32_t CORE6_TUNE_PWM_C3: 3;
            __IO uint32_t CORE6_TUNE_PWM_C2: 3;
            __IO uint32_t CORE6_TUNE_PWM_C1: 3;
            __IO uint32_t CORE6_BYPASS_PWM_BYPASS_RoughSS: 1;
            __IO uint32_t CORE6_BYPASS_PWM_TUNE_RoughSS: 2;
            __IO uint32_t CORE6_BYPASS_PWM_TUNE_VCL: 3;
            __IO uint32_t CORE6_BYPASS_PWM_TUNE_VCH: 3;
            __IO uint32_t CORE6_X4_PWM_COMP_IB: 1;
            __IO uint32_t CORE6_X4_POW_PWM_CLP: 1;
            __IO uint32_t CORE6_X4_TUNE_VDIV_Bit0: 1;
        } BITS_2C0;
    } u_2C0;

    /* 0x02C4       0x4000_02c4
        6:0     R/W    CORE6_X4_TUNE_VDIV_Bit7_Bit1            7'h0
        14:7    R/W    CORE6_BYPASS_PWM_TUNE_POS_VREFPFM       8'h0
        17:15   R/W    CORE6_BYPASS_PWM_TUNE_POS_VREFOCP       3'h0
        18      R/W    CORE6_FPWM                              1'h0
        19      R/W    CORE6_POW_PFM                           1'h0
        20      R/W    CORE6_POW_PWM                           1'h0
        21      R/W    CORE6_POW_VDIV                          1'h0
        23:22   R/W    CORE6_XTAL_OV_RATIO                     2'h0
        26:24   R/W    CORE6_XTAL_OV_UNIT                      3'h0
        29:27   R/W    CORE6_XTAL_OV_MODE                      3'h0
        30      R/W    CORE6_EN_POWERMOS_DR8X                  1'h0
        31      R/W    CORE6_SEL_OCP_TABLE                     1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR6_63_32;
        struct
        {
            __IO uint32_t CORE6_X4_TUNE_VDIV_Bit7_Bit1: 7;
            __IO uint32_t CORE6_BYPASS_PWM_TUNE_POS_VREFPFM: 8;
            __IO uint32_t CORE6_BYPASS_PWM_TUNE_POS_VREFOCP: 3;
            __IO uint32_t CORE6_FPWM: 1;
            __IO uint32_t CORE6_POW_PFM: 1;
            __IO uint32_t CORE6_POW_PWM: 1;
            __IO uint32_t CORE6_POW_VDIV: 1;
            __IO uint32_t CORE6_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE6_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE6_XTAL_OV_MODE: 3;
            __IO uint32_t CORE6_EN_POWERMOS_DR8X: 1;
            __IO uint32_t CORE6_SEL_OCP_TABLE: 1;
        } BITS_2C4;
    } u_2C4;

    /* 0x02C8       0x4000_02c8
        1:0     R/W    CORE7_TUNE_OCP_RES                      2'h0
        4:2     R/W    CORE7_TUNE_PWM_R3                       3'h0
        7:5     R/W    CORE7_TUNE_PWM_R2                       3'h0
        10:8    R/W    CORE7_TUNE_PWM_R1                       3'h0
        13:11   R/W    CORE7_TUNE_PWM_C3                       3'h0
        16:14   R/W    CORE7_TUNE_PWM_C2                       3'h0
        19:17   R/W    CORE7_TUNE_PWM_C1                       3'h0
        20      R/W    CORE7_BYPASS_PWM_BYPASS_RoughSS         1'h0
        22:21   R/W    CORE7_BYPASS_PWM_TUNE_RoughSS           2'h0
        25:23   R/W    CORE7_BYPASS_PWM_TUNE_VCL               3'h0
        28:26   R/W    CORE7_BYPASS_PWM_TUNE_VCH               3'h0
        29      R/W    CORE7_X4_PWM_COMP_IB                    1'h0
        30      R/W    CORE7_X4_POW_PWM_CLP                    1'h0
        31      R/W    CORE7_X4_TUNE_VDIV_Bit0                 1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR7_31_0;
        struct
        {
            __IO uint32_t CORE7_TUNE_OCP_RES: 2;
            __IO uint32_t CORE7_TUNE_PWM_R3: 3;
            __IO uint32_t CORE7_TUNE_PWM_R2: 3;
            __IO uint32_t CORE7_TUNE_PWM_R1: 3;
            __IO uint32_t CORE7_TUNE_PWM_C3: 3;
            __IO uint32_t CORE7_TUNE_PWM_C2: 3;
            __IO uint32_t CORE7_TUNE_PWM_C1: 3;
            __IO uint32_t CORE7_BYPASS_PWM_BYPASS_RoughSS: 1;
            __IO uint32_t CORE7_BYPASS_PWM_TUNE_RoughSS: 2;
            __IO uint32_t CORE7_BYPASS_PWM_TUNE_VCL: 3;
            __IO uint32_t CORE7_BYPASS_PWM_TUNE_VCH: 3;
            __IO uint32_t CORE7_X4_PWM_COMP_IB: 1;
            __IO uint32_t CORE7_X4_POW_PWM_CLP: 1;
            __IO uint32_t CORE7_X4_TUNE_VDIV_Bit0: 1;
        } BITS_2C8;
    } u_2C8;

    /* 0x02CC       0x4000_02cc
        6:0     R/W    CORE7_X4_TUNE_VDIV_Bit7_Bit1            7'h0
        14:7    R/W    CORE7_BYPASS_PWM_TUNE_POS_VREFPFM       8'h0
        17:15   R/W    CORE7_BYPASS_PWM_TUNE_POS_VREFOCP       3'h0
        18      R/W    CORE7_FPWM                              1'h0
        19      R/W    CORE7_POW_PFM                           1'h0
        20      R/W    CORE7_POW_PWM                           1'h0
        21      R/W    CORE7_POW_VDIV                          1'h0
        23:22   R/W    CORE7_XTAL_OV_RATIO                     2'h0
        26:24   R/W    CORE7_XTAL_OV_UNIT                      3'h0
        29:27   R/W    CORE7_XTAL_OV_MODE                      3'h0
        30      R/W    CORE7_EN_POWERMOS_DR8X                  1'h0
        31      R/W    CORE7_SEL_OCP_TABLE                     1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR7_63_32;
        struct
        {
            __IO uint32_t CORE7_X4_TUNE_VDIV_Bit7_Bit1: 7;
            __IO uint32_t CORE7_BYPASS_PWM_TUNE_POS_VREFPFM: 8;
            __IO uint32_t CORE7_BYPASS_PWM_TUNE_POS_VREFOCP: 3;
            __IO uint32_t CORE7_FPWM: 1;
            __IO uint32_t CORE7_POW_PFM: 1;
            __IO uint32_t CORE7_POW_PWM: 1;
            __IO uint32_t CORE7_POW_VDIV: 1;
            __IO uint32_t CORE7_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE7_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE7_XTAL_OV_MODE: 3;
            __IO uint32_t CORE7_EN_POWERMOS_DR8X: 1;
            __IO uint32_t CORE7_SEL_OCP_TABLE: 1;
        } BITS_2CC;
    } u_2CC;

    /* 0x02D0       0x4000_02d0
        3:0     R/W    flash1_div_sel                          4'h0
        4       R/W    flash1_div_en                           1'h0
        5       R/W    FLASH1_CLK_SRC_EN                       1'h0
        6       R/W    flash1_clk_src_sel_1                    1'h0
        7       R/W    flash1_clk_src_sel_0                    1'h0
        8       R/W    flash1_mux_1_clk_cg_en                  1'h0
        12:9    R/W    flash2_div_sel                          4'h0
        13      R/W    flash2_div_en                           1'h0
        14      R/W    FLASH2_CLK_SRC_EN                       1'h0
        15      R/W    flash2_clk_src_sel_1                    1'h0
        16      R/W    flash2_clk_src_sel_0                    1'h0
        17      R/W    flash2_mux_1_clk_cg_en                  1'h0
        26:18   R/W    RESERVED610                             9'h0
        27      R/W    R_FLASH1_CLK_SRC_PLL_SEL_0              1'h0
        28      R/W    R_FLASH2_CLK_SRC_PLL_SEL_0              1'h0
        29      R/W    R_FLASH1_CLK_SRC_PLL_SEL_1              1'h0
        30      R/W    R_FLASH2_CLK_SRC_PLL_SEL_1              1'h0
        31      R/W    RESERVED605                             1'h0
    */
    union
    {
        __IO uint32_t REG_0x2D0;
        struct
        {
            __IO uint32_t flash1_div_sel: 4;
            __IO uint32_t flash1_div_en: 1;
            __IO uint32_t FLASH1_CLK_SRC_EN: 1;
            __IO uint32_t flash1_clk_src_sel_1: 1;
            __IO uint32_t flash1_clk_src_sel_0: 1;
            __IO uint32_t flash1_mux_1_clk_cg_en: 1;
            __IO uint32_t flash2_div_sel: 4;
            __IO uint32_t flash2_div_en: 1;
            __IO uint32_t FLASH2_CLK_SRC_EN: 1;
            __IO uint32_t flash2_clk_src_sel_1: 1;
            __IO uint32_t flash2_clk_src_sel_0: 1;
            __IO uint32_t flash2_mux_1_clk_cg_en: 1;
            __IO uint32_t RESERVED610: 9;
            __IO uint32_t R_FLASH1_CLK_SRC_PLL_SEL_0: 1;
            __IO uint32_t R_FLASH2_CLK_SRC_PLL_SEL_0: 1;
            __IO uint32_t R_FLASH1_CLK_SRC_PLL_SEL_1: 1;
            __IO uint32_t R_FLASH2_CLK_SRC_PLL_SEL_1: 1;
            __IO uint32_t RESERVED605: 1;
        } BITS_2D0;
    } u_2D0;

    /* 0x02D4       0x4000_02d4
        0       R/W    USB_DIV_SEL0                            1'h0
        1       R/W    USB_CLK_SRC_EN                          1'h0
        2       R/W    USB_CLK_SEL                             1'h0
        3       R/W    USB_CLK_EN                              1'h0
        5:4     R/W    USB_DIV_SEL1                            2'h0
        31:6    R/W    RESERVED624                             26'h0
    */
    union
    {
        __IO uint32_t REG_0x2D4;
        struct
        {
            __IO uint32_t USB_DIV_SEL0: 1;
            __IO uint32_t USB_CLK_SRC_EN: 1;
            __IO uint32_t USB_CLK_SEL: 1;
            __IO uint32_t USB_CLK_EN: 1;
            __IO uint32_t USB_DIV_SEL1: 2;
            __IO uint32_t RESERVED624: 26;
        } BITS_2D4;
    } u_2D4;

    /* 0x02D8       0x4000_02d8
        31:0    R      RESERVED631                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x02D8;
        struct
        {
            __I uint32_t RESERVED631: 32;
        } BITS_2D8;
    } u_2D8;

    /* 0x02DC       0x4000_02dc
        0       R/W    CORE5_EN_HVD17_LOWIQ                    1'h0
        4:1     R/W    CORE5_TUNE_HVD17_IB                     4'h0
        5       R/W    CORE5_X4_PFM_COMP_IB                    1'h0
        8:6     R/W    CORE5_TUNE_PFM_VREFOCPPFM               3'h0
        14:9    R/W    CORE5_TUNE_SAW_ICLK                     6'h0
        15      R/W    RESERVED639                             1'h0
        16      R/W    CORE6_EN_HVD17_LOWIQ                    1'h0
        20:17   R/W    CORE6_TUNE_HVD17_IB                     4'h0
        21      R/W    CORE6_X4_PFM_COMP_IB                    1'h0
        24:22   R/W    CORE6_TUNE_PFM_VREFOCPPFM               3'h0
        30:25   R/W    CORE6_TUNE_SAW_ICLK                     6'h0
        31      R/W    RESERVED633                             1'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR6_79_64_AUTO_SW_PAR5_79_64;
        struct
        {
            __IO uint32_t CORE5_EN_HVD17_LOWIQ: 1;
            __IO uint32_t CORE5_TUNE_HVD17_IB: 4;
            __IO uint32_t CORE5_X4_PFM_COMP_IB: 1;
            __IO uint32_t CORE5_TUNE_PFM_VREFOCPPFM: 3;
            __IO uint32_t CORE5_TUNE_SAW_ICLK: 6;
            __IO uint32_t RESERVED639: 1;
            __IO uint32_t CORE6_EN_HVD17_LOWIQ: 1;
            __IO uint32_t CORE6_TUNE_HVD17_IB: 4;
            __IO uint32_t CORE6_X4_PFM_COMP_IB: 1;
            __IO uint32_t CORE6_TUNE_PFM_VREFOCPPFM: 3;
            __IO uint32_t CORE6_TUNE_SAW_ICLK: 6;
            __IO uint32_t RESERVED633: 1;
        } BITS_2DC;
    } u_2DC;

    /* 0x02E0       0x4000_02e0
        31:0    R/W    RESERVED646                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02E0;
        struct
        {
            __IO uint32_t RESERVED646: 32;
        } BITS_2E0;
    } u_2E0;

    /* 0x02E4       0x4000_02e4
        31:0    R/W    RESERVED648                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02E4;
        struct
        {
            __IO uint32_t RESERVED648: 32;
        } BITS_2E4;
    } u_2E4;

    /* 0x02E8       0x4000_02e8
        31:0    R/W    RESERVED650                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02E8;
        struct
        {
            __IO uint32_t RESERVED650: 32;
        } BITS_2E8;
    } u_2E8;

    /* 0x02EC       0x4000_02ec
        31:0    R/W    RESERVED652                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02EC;
        struct
        {
            __IO uint32_t RESERVED652: 32;
        } BITS_2EC;
    } u_2EC;

    /* 0x02F0       0x4000_02f0
        31:0    R/W    RESERVED654                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02F0;
        struct
        {
            __IO uint32_t RESERVED654: 32;
        } BITS_2F0;
    } u_2F0;

    /* 0x02F4       0x4000_02f4
        31:0    R/W    RESERVED656                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02F4;
        struct
        {
            __IO uint32_t RESERVED656: 32;
        } BITS_2F4;
    } u_2F4;

    /* 0x02F8       0x4000_02f8
        31:0    R/W    RESERVED658                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02F8;
        struct
        {
            __IO uint32_t RESERVED658: 32;
        } BITS_2F8;
    } u_2F8;

    /* 0x02FC       0x4000_02fc
        31:0    R/W    RESERVED660                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x02FC;
        struct
        {
            __IO uint32_t RESERVED660: 32;
        } BITS_2FC;
    } u_2FC;

    /* 0x0300       0x4000_0300
        7:0     R/W    PON_PERI_DLYSEL_SPIM                    8'h0
        15:8    R/W    PON_PERI_DLYSEL_SPIM1                   8'h0
        23:16   R/W    PON_PERI_DLYSEL_SPIM2                   8'h0
        31:24   R/W    RESERVED662                             8'h0
    */
    union
    {
        __IO uint32_t PON_PERI_DLYSEL_CTRL;
        struct
        {
            __IO uint32_t PON_PERI_DLYSEL_SPIM: 8;
            __IO uint32_t PON_PERI_DLYSEL_SPIM1: 8;
            __IO uint32_t PON_PERI_DLYSEL_SPIM2: 8;
            __IO uint32_t RESERVED662: 8;
        } BITS_300;
    } u_300;

    /* 0x0304       0x4000_0304
        31:0    R/W    RESERVED667                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x0304;
        struct
        {
            __IO uint32_t RESERVED667: 32;
        } BITS_304;
    } u_304;

    /* 0x0308       0x4000_0308
        0       R/W    PON_SPI0_MST                            1'h0
        1       R/W    PON_SPI0_BRIDGE_EN                      1'h0
        2       R/W    PON_SPI1_BRIDGE_EN                      1'h0
        3       R/W    PON_SPI2_BRIDGE_EN                      1'h0
        4       R/W    PON_SPI1_H2S_BRG_EN                     1'h0
        5       R/W    PON_SPI_SLV_BRIDGE_EN                   1'h0
        31:6    R/W    RESERVED669                             26'h0
    */
    union
    {
        __IO uint32_t REG_0x308;
        struct
        {
            __IO uint32_t PON_SPI0_MST: 1;
            __IO uint32_t PON_SPI0_BRIDGE_EN: 1;
            __IO uint32_t PON_SPI1_BRIDGE_EN: 1;
            __IO uint32_t PON_SPI2_BRIDGE_EN: 1;
            __IO uint32_t PON_SPI1_H2S_BRG_EN: 1;
            __IO uint32_t PON_SPI_SLV_BRIDGE_EN: 1;
            __IO uint32_t RESERVED669: 26;
        } BITS_308;
    } u_308;

    /* 0x030C       0x4000_030c
        31:0    R      RESERVED677                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x030C;
        struct
        {
            __I uint32_t RESERVED677: 32;
        } BITS_30C;
    } u_30C;

    /* 0x0310       0x4000_0310
        31:0    R      RESERVED679                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x0310;
        struct
        {
            __I uint32_t RESERVED679: 32;
        } BITS_310;
    } u_310;

    /* 0x0314       0x4000_0314
        31:0    R      RESERVED681                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x0314;
        struct
        {
            __I uint32_t RESERVED681: 32;
        } BITS_314;
    } u_314;

    /* 0x0318       0x4000_0318
        31:0    R/W    RESERVED683                             32'h0
    */
    union
    {
        __IO uint32_t CPU_INTER_ENABLE;
        struct
        {
            __IO uint32_t RESERVED683: 32;
        } BITS_318;
    } u_318;

    /* 0x031C       0x4000_031c
        31:0    R      RESERVED685                             32'hEAEAEAEA
    */
    union
    {
        __IO uint32_t REG_0x031C;
        struct
        {
            __I uint32_t RESERVED685: 32;
        } BITS_31C;
    } u_31C;

    /* 0x0320       0x4000_0320
        31:0    R/W    RESERVED687                             32'he0342a43
    */
    union
    {
        __IO uint32_t REG_0x0320;
        struct
        {
            __IO uint32_t RESERVED687: 32;
        } BITS_320;
    } u_320;

    /* 0x0324       0x4000_0324
        31:0    R/W    RESERVED689                             32'h2419e6ab
    */
    union
    {
        __IO uint32_t REG_0x0324;
        struct
        {
            __IO uint32_t RESERVED689: 32;
        } BITS_324;
    } u_324;

    /* 0x0328       0x4000_0328
        31:0    R/W    RESERVED691                             32'he0000000
    */
    union
    {
        __IO uint32_t REG_0x0328;
        struct
        {
            __IO uint32_t RESERVED691: 32;
        } BITS_328;
    } u_328;

    /* 0x032C       0x4000_032c
        3:0     R/W    OSC40_FSET                              4'h8
        31:4    R/W    RESERVED693                             28'h0001090
    */
    union
    {
        __IO uint32_t REG_OSC40_FSET;
        struct
        {
            __IO uint32_t OSC40_FSET: 4;
            __IO uint32_t RESERVED693: 28;
        } BITS_32C;
    } u_32C;

    /* 0x0330       0x4000_0330
        0       RW     rst_n_aac                               1'b0
        1       RW     offset_plus                             1'b0
        7:2     RW     XAAC_GM_offset                          6'h0
        8       RW     GM_STEP                                 1'b0
        14:9    RW     GM_INIT                                 6'h3F
        17:15   RW     XTAL_CLK_SET                            3'b101
        23:18   RW     GM_STUP                                 6'h3F
        29:24   RW     GM_MANUAL                               6'h1F
        30      RW     r_EN_XTAL_AAC_DIGI                      1'b0
        31      RW     r_EN_XTAL_AAC_TRIG                      1'b0
    */
    union
    {
        __IO uint32_t REG_0x330;
        struct
        {
            __IO uint32_t rst_n_aac: 1;
            __IO uint32_t offset_plus: 1;
            __IO uint32_t XAAC_GM_offset: 6;
            __IO uint32_t GM_STEP: 1;
            __IO uint32_t GM_INIT: 6;
            __IO uint32_t XTAL_CLK_SET: 3;
            __IO uint32_t GM_STUP: 6;
            __IO uint32_t GM_MANUAL: 6;
            __IO uint32_t r_EN_XTAL_AAC_DIGI: 1;
            __IO uint32_t r_EN_XTAL_AAC_TRIG: 1;
        } BITS_330;
    } u_330;

    /* 0x0334       0x4000_0334
        0       R      XAAC_BUSY                               1'b0
        1       R      XAAC_READY                              1'b0
        7:2     R      XTAL_GM_OUT                             6'h1F
        11:8    R      xaac_curr_state                         4'h0
        12      R/W    EN_XTAL_AAC_GM                          1'b0
        13      R/W    EN_XTAL_AAC_PKDET                       1'b0
        14      R      XTAL_PKDET_OUT                          1'b0
        15      R/W    RESERVED708                             1'b0
        31:16   R/W    RESERVED707                             16'h0129
    */
    union
    {
        __IO uint32_t AAC_CTRL_1;
        struct
        {
            __I uint32_t XAAC_BUSY: 1;
            __I uint32_t XAAC_READY: 1;
            __I uint32_t XTAL_GM_OUT: 6;
            __I uint32_t xaac_curr_state: 4;
            __IO uint32_t EN_XTAL_AAC_GM: 1;
            __IO uint32_t EN_XTAL_AAC_PKDET: 1;
            __I uint32_t XTAL_PKDET_OUT: 1;
            __IO uint32_t RESERVED708: 1;
            __IO uint32_t RESERVED707: 16;
        } BITS_334;
    } u_334;

    /* 0x0338       0x4000_0338
        0       R/W    disable_pll_pre_gating                  1'b0
        15:1    R/W    RESERVED726                             15'h0
        16      R      XTAL32K_OK                              1'h0
        17      R      OSC32K_OK                               1'h0
        22:18   R      XTAL_CTRL_DEBUG_OUT[4:0]                5'h0
        23      R      BT_READY_PLL2                           1'h0
        24      R      BT_READY_PLL1                           1'h0
        25      R      XTAL_OK                                 1'h0
        27:26   R      XTAL_CTRL_DEBUG_OUT[6:5]                3'h0
        28      R      BT_READY_PLL3                           1'h0
        31:29   R      XTAL_MODE_O                             3'h0
    */
    union
    {
        __IO uint32_t REG_0x338;
        struct
        {
            __IO uint32_t disable_pll_pre_gating: 1;
            __IO uint32_t RESERVED726: 15;
            __I uint32_t XTAL32K_OK: 1;
            __I uint32_t OSC32K_OK: 1;
            __I uint32_t XTAL_CTRL_DEBUG_OUT_4_0: 5;
            __I uint32_t BT_READY_PLL2: 1;
            __I uint32_t BT_READY_PLL1: 1;
            __I uint32_t XTAL_OK: 1;
            __I uint32_t XTAL_CTRL_DEBUG_OUT_6_5: 2;
            __I uint32_t BT_READY_PLL3: 1;
            __I uint32_t XTAL_MODE_O: 3;
        } BITS_338;
    } u_338;

    /* 0x033C       0x4000_033c
        0       R/W    resetn                                  1'b0
        1       R/W    EN_XTAL_PDCK_DIGI                       1'b0
        2       R/W    PDCK_SEARCH_MODE                        1'b0
        4:3     R/W    PDCK_WAIT_CYC[1:0]                      2'b01
        9:5     R/W    VREF_MANUAL[4:0]                        5'h1F
        14:10   R/W    VREF_INIT[4:0]                          5'h1F
        16:15   R/W    XTAL_PDCK_UNIT[1:0]                     2'b01
        21:17   R/W    XPDCK_VREF_SEL[4:0]                     5'h2
        22      R/W    PDCK_LPOW                               1'b0
        27:23   R/W    RESERVED730                             5'h0
        31:28   R      pdck_state[3:0]                         4'h0
    */
    union
    {
        __IO uint32_t XTAL_PDCK;
        struct
        {
            __IO uint32_t resetn: 1;
            __IO uint32_t EN_XTAL_PDCK_DIGI: 1;
            __IO uint32_t PDCK_SEARCH_MODE: 1;
            __IO uint32_t PDCK_WAIT_CYC_1_0: 2;
            __IO uint32_t VREF_MANUAL_4_0: 5;
            __IO uint32_t VREF_INIT_4_0: 5;
            __IO uint32_t XTAL_PDCK_UNIT_1_0: 2;
            __IO uint32_t XPDCK_VREF_SEL_4_0: 5;
            __IO uint32_t PDCK_LPOW: 1;
            __IO uint32_t RESERVED730: 5;
            __I uint32_t pdck_state_3_0: 4;
        } BITS_33C;
    } u_33C;

    /* 0x0340       0x4000_0340
        31:0    R/W    RESERVED741                             32'h0
    */
    union
    {
        __IO uint32_t USB_ISO_INT_CTRL;
        struct
        {
            __IO uint32_t RESERVED741: 32;
        } BITS_340;
    } u_340;

    /* 0x0344       0x4000_0344
        31:0    R/W    RESERVED743                             32'h0
    */
    union
    {
        __IO uint32_t REG_0x344;
        struct
        {
            __IO uint32_t RESERVED743: 32;
        } BITS_344;
    } u_344;

    /* 0x0348       0x4000_0348
        8:0     R/W    RESERVED748                             9'h0
        9       R/W    r_irrc_clk_sel                          1'b0
        10      R/W    r_irrc_clk_src_pll_sel                  1'b0
        31:11   R      RESERVED745                             21'h0
    */
    union
    {
        __IO uint32_t REG_0x348;
        struct
        {
            __IO uint32_t RESERVED748: 9;
            __IO uint32_t r_irrc_clk_sel: 1;
            __IO uint32_t r_irrc_clk_src_pll_sel: 1;
            __I uint32_t RESERVED745: 21;
        } BITS_348;
    } u_348;

    __IO uint32_t RSVD_0x34c[4];

    /* 0x035C       0x4000_035c
        2:0     R/W    RESERVED764                             3'h0
        4:3     R/W    BIT_PERI_UART0_CLK_DIV                  2'h0
        6:5     R/W    BIT_PERI_UART1_CLK_DIV                  2'h0
        8:7     R/W    BIT_PERI_UART2_CLK_DIV                  2'h0
        10:9    R/W    BIT_PERI_UART3_CLK_DIV                  2'h0
        12:11   R/W    BIT_PERI_UART4_CLK_DIV                  2'h0
        14:13   R/W    BIT_PERI_UART5_CLK_DIV                  2'h0
        16:15   R/W    BIT_PERI_I2C0_CLK_DIV                   2'h0
        18:17   R/W    BIT_PERI_I2C1_CLK_DIV                   2'h0
        21:19   R/W    BIT_PERI_SPI0_CLK_DIV                   3'h0
        24:22   R/W    BIT_PERI_SPI1_CLK_DIV                   3'h0
        27:25   R/W    BIT_PERI_SPI2_CLK_DIV                   3'h0
        29:28   R/W    BIT_PERI_I2C2_CLK_DIV                   2'h0
        30      R/W    r_spi1_clk_src_pll_sel                  1'b0
        31      R/W    RESERVED750                             1'b0
    */
    union
    {
        __IO uint32_t REG_PERI_GTIMER_CLK_SRC1;
        struct
        {
            __IO uint32_t RESERVED764: 3;
            __IO uint32_t BIT_PERI_UART0_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART1_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART2_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART3_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART4_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART5_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_I2C0_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_I2C1_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_SPI0_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_SPI1_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_SPI2_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_I2C2_CLK_DIV: 2;
            __IO uint32_t r_spi1_clk_src_pll_sel: 1;
            __IO uint32_t RESERVED750: 1;
        } BITS_35C;
    } u_35C;

    /* 0x0360       0x4000_0360
        7:0     R/W    RESERVED779                             8'h0
        8       R/W    BIT_TIMER_CLK_32K_EN                    1'h1
        9       R/W    RESERVED777                             1'h1
        10      R/W    RESERVED776                             1'h0
        11      R/W    RESERVED775                             1'h0
        12      R/W    RESERVED774                             1'h0
        13      R/W    RESERVED773                             1'h0
        15:14   R/W    RESERVED772                             2'h0
        18:16   R/W    RESERVED771                             3'h0
        21:19   R/W    RESERVED770                             3'h0
        24:22   R/W    RESERVED769                             3'h0
        27:25   R/W    RESERVED768                             3'h0
        30:28   R/W    RESERVED767                             3'h0
        31      R/W    RESERVED766                             1'h0
    */
    union
    {
        __IO uint32_t REG_PERI_GTIMER_CLK_SRC0;
        struct
        {
            __IO uint32_t RESERVED779: 8;
            __IO uint32_t BIT_TIMER_CLK_32K_EN: 1;
            __IO uint32_t RESERVED777: 1;
            __IO uint32_t RESERVED776: 1;
            __IO uint32_t RESERVED775: 1;
            __IO uint32_t RESERVED774: 1;
            __IO uint32_t RESERVED773: 1;
            __IO uint32_t RESERVED772: 2;
            __IO uint32_t RESERVED771: 3;
            __IO uint32_t RESERVED770: 3;
            __IO uint32_t RESERVED769: 3;
            __IO uint32_t RESERVED768: 3;
            __IO uint32_t RESERVED767: 3;
            __IO uint32_t RESERVED766: 1;
        } BITS_360;
    } u_360;

    /* 0x0364       0x4000_0364
        15:0    R      RESERVED782                             16'h0
        31:16   R/W    RESERVED781                             16'h0
    */
    union
    {
        __IO uint32_t REG_PERI_PWM2_DZONE_CTRL;
        struct
        {
            __I uint32_t RESERVED782: 16;
            __IO uint32_t RESERVED781: 16;
        } BITS_364;
    } u_364;

    /* 0x0368       0x4000_0368
        15:0    R      RESERVED785                             16'h0
        31:16   R/W    RESERVED784                             16'h0
    */
    union
    {
        __IO uint32_t REG_PERI_PWM3_DZONE_CTRL;
        struct
        {
            __I uint32_t RESERVED785: 16;
            __IO uint32_t RESERVED784: 16;
        } BITS_368;
    } u_368;

    /* 0x036C       0x4000_036c
        2:0     R/W    ETIMER0_0_CLK_DIV_SEL                   3'h0
        3       R/W    ETIMER0_0_CLK_DIV_EN                    1'h0
        6:4     R/W    ETIMER0_1_CLK_DIV_SEL                   3'h0
        7       R/W    ETIMER0_1_CLK_DIV_EN                    1'h0
        23:8    R/W    RESERVED792                             16'h0
        25:24   R/W    CAN0_LP_SCAN_DIV_SEL                    2'h0
        26      R/W    CAN0_SCAN_DIV_SEL                       1'h0
        28:27   R/W    CAN1_LP_SCAN_DIV_SEL                    2'h0
        29      R/W    CAN1_SCAN_DIV_SEL                       1'h0
        31:30   R/W    RESERVED787                             2'h0
    */
    union
    {
        __IO uint32_t REG_PERI_ETIMER0_CAN_DIV;
        struct
        {
            __IO uint32_t ETIMER0_0_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER0_0_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER0_1_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER0_1_CLK_DIV_EN: 1;
            __IO uint32_t RESERVED792: 16;
            __IO uint32_t CAN0_LP_SCAN_DIV_SEL: 2;
            __IO uint32_t CAN0_SCAN_DIV_SEL: 1;
            __IO uint32_t CAN1_LP_SCAN_DIV_SEL: 2;
            __IO uint32_t CAN1_SCAN_DIV_SEL: 1;
            __IO uint32_t RESERVED787: 2;
        } BITS_36C;
    } u_36C;

    /* 0x0370       0x4000_0370
        2:0     R/W    ETIMER1_0_CLK_DIV_SEL                   3'h0
        3       R/W    ETIMER1_0_CLK_DIV_EN                    1'h0
        6:4     R/W    ETIMER1_1_CLK_DIV_SEL                   3'h0
        7       R/W    ETIMER1_1_CLK_DIV_EN                    1'h0
        10:8    R/W    ETIMER1_2_CLK_DIV_SEL                   3'h0
        11      R/W    ETIMER1_2_CLK_DIV_EN                    1'h0
        14:12   R/W    ETIMER1_3_CLK_DIV_SEL                   3'h0
        15      R/W    ETIMER1_3_CLK_DIV_EN                    1'h0
        18:16   R/W    ETIMER1_4_CLK_DIV_SEL                   3'h0
        19      R/W    ETIMER1_4_CLK_DIV_EN                    1'h0
        22:20   R/W    ETIMER1_5_CLK_DIV_SEL                   3'h0
        23      R/W    ETIMER1_5_CLK_DIV_EN                    1'h0
        26:24   R/W    ETIMER1_6_CLK_DIV_SEL                   3'h0
        27      R/W    ETIMER1_6_CLK_DIV_EN                    1'h0
        30:28   R/W    ETIMER1_7_CLK_DIV_SEL                   3'h0
        31      R/W    ETIMER1_7_CLK_DIV_EN                    1'h0
    */
    union
    {
        __IO uint32_t REG_PERI_ETIMER1_DIV_G0;
        struct
        {
            __IO uint32_t ETIMER1_0_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_0_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_1_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_1_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_2_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_2_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_3_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_3_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_4_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_4_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_5_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_5_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_6_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_6_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_7_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_7_CLK_DIV_EN: 1;
        } BITS_370;
    } u_370;

    /* 0x0374       0x4000_0374
        2:0     R/W    ETIMER1_8_CLK_DIV_SEL                   3'h0
        3       R/W    ETIMER1_8_CLK_DIV_EN                    1'h0
        4       R/W    ETIMER1_8_CLK_SEL0                      1'h0
        5       R/W    ETIMER1_8_CLK_SEL1                      1'h0
        6       R/W    ETIMER1_8_PLL_CLK_SEL                   1'h0
        7       R/W    RESERVED826                             1'h0
        10:8    R/W    ETIMER1_9_CLK_DIV_SEL                   3'h0
        11      R/W    ETIMER1_9_CLK_DIV_EN                    1'h0
        14:12   R/W    ETIMER1_9_PWM_CLK_DIV_SEL               3'h0
        15      R/W    ETIMER1_9_PWM_CLK_DIV_EN                1'h0
        16      R/W    ETIMER1_9_PWM_CLK_EN                    1'h0
        17      R/W    ETIMER1_9_PWM_CLK_SEL                   1'h0
        19:18   R/W    RESERVED819                             1'h0
        22:20   R/W    ETIMER1_9_DEB_CLK_DIV_SEL               3'h0
        23      R/W    ETIMER1_9_DEB_CLK_DIV_EN                1'h0
        24      R/W    ETIMER1_9_DEB_CLK_CLK_EN                1'h0
        31:25   R/W    RESERVED815                             7'h0
    */
    union
    {
        __IO uint32_t REG_PERI_ETIMER1_G1_DIV;
        struct
        {
            __IO uint32_t ETIMER1_8_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_8_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_8_CLK_SEL0: 1;
            __IO uint32_t ETIMER1_8_CLK_SEL1: 1;
            __IO uint32_t ETIMER1_8_PLL_CLK_SEL: 1;
            __IO uint32_t RESERVED826: 1;
            __IO uint32_t ETIMER1_9_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_9_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_9_PWM_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_9_PWM_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_9_PWM_CLK_EN: 1;
            __IO uint32_t ETIMER1_9_PWM_CLK_SEL: 1;
            __IO uint32_t RESERVED819: 2;
            __IO uint32_t ETIMER1_9_DEB_CLK_DIV_SEL: 3;
            __IO uint32_t ETIMER1_9_DEB_CLK_DIV_EN: 1;
            __IO uint32_t ETIMER1_9_DEB_CLK_CLK_EN: 1;
            __IO uint32_t RESERVED815: 7;
        } BITS_374;
    } u_374;

    __IO uint32_t RSVD_0x378[5];

    /* 0x038C       0x4000_038c
        31:0    R/W    SWR_SS_LUT_2                            32'h0
    */
    union
    {
        __IO uint32_t REG_0x38C;
        struct
        {
            __IO uint32_t SWR_SS_LUT_2: 32;
        } BITS_38C;
    } u_38C;

    /* 0x0390       0x4000_0390
        31:0    R/W    SWR_SS_LUT_3                            32'h0
    */
    union
    {
        __IO uint32_t REG_0x390;
        struct
        {
            __IO uint32_t SWR_SS_LUT_3: 32;
        } BITS_390;
    } u_390;

    /* 0x0394       0x4000_0394
        31:0    R/W    SWR_SS_LUT_4                            32'h0
    */
    union
    {
        __IO uint32_t REG_0x394;
        struct
        {
            __IO uint32_t SWR_SS_LUT_4: 32;
        } BITS_394;
    } u_394;

    /* 0x0398       0x4000_0398
        31:0    R/W    SWR_SS_LUT_5                            32'h0
    */
    union
    {
        __IO uint32_t REG_0x398;
        struct
        {
            __IO uint32_t SWR_SS_LUT_5: 32;
        } BITS_398;
    } u_398;

    /* 0x039C       0x4000_039c
        15:0    R/W    SWR_SS_CONFIG                           16'h0
        31:16   R/W    RESERVED841                             16'h0
    */
    union
    {
        __IO uint32_t REG_0x39C;
        struct
        {
            __IO uint32_t SWR_SS_CONFIG: 16;
            __IO uint32_t RESERVED841: 16;
        } BITS_39C;
    } u_39C;

    /* 0x03A0       0x4000_03a0
        31:0    R/W    SWR_SS_LUT_0                            32'h0
    */
    union
    {
        __IO uint32_t REG_0x3A0;
        struct
        {
            __IO uint32_t SWR_SS_LUT_0: 32;
        } BITS_3A0;
    } u_3A0;

    /* 0x03A4       0x4000_03a4
        31:0    R/W    SWR_SS_LUT_1                            32'h0
    */
    union
    {
        __IO uint32_t REG_0x3A4;
        struct
        {
            __IO uint32_t SWR_SS_LUT_1: 32;
        } BITS_3A4;
    } u_3A4;

    __IO uint32_t RSVD_0x3a8[20];

    /* 0x03F8       0x4000_03f8
        0       R/W    CORE7_EN_HVD17_LOWIQ                    1'h0
        4:1     R/W    CORE7_TUNE_HVD17_IB                     4'h0
        5       R/W    CORE7_X4_PFM_COMP_IB                    1'h0
        8:6     R/W    CORE7_TUNE_PFM_VREFOCPPFM               3'h0
        14:9    R/W    CORE7_TUNE_SAW_ICLK                     6'h0
        31:15   R/W    RESERVED848                             17'h0
    */
    union
    {
        __IO uint32_t AUTO_SW_PAR7_79_64;
        struct
        {
            __IO uint32_t CORE7_EN_HVD17_LOWIQ: 1;
            __IO uint32_t CORE7_TUNE_HVD17_IB: 4;
            __IO uint32_t CORE7_X4_PFM_COMP_IB: 1;
            __IO uint32_t CORE7_TUNE_PFM_VREFOCPPFM: 3;
            __IO uint32_t CORE7_TUNE_SAW_ICLK: 6;
            __IO uint32_t RESERVED848: 17;
        } BITS_3F8;
    } u_3F8;

    __IO uint32_t RSVD_0x3fc[65];

    /* 0x0500       0x4000_0500
        7:0     R/W    PMUX_GPIO_ADC_0                         8'h0
        15:8    R/W    PMUX_GPIO_ADC_1                         8'h0
        23:16   R/W    PMUX_GPIO_ADC_2                         8'h0
        31:24   R/W    PMUX_GPIO_ADC_3                         8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO_ADC_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_ADC_0: 8;
            __IO uint32_t PMUX_GPIO_ADC_1: 8;
            __IO uint32_t PMUX_GPIO_ADC_2: 8;
            __IO uint32_t PMUX_GPIO_ADC_3: 8;
        } BITS_500;
    } u_500;

    /* 0x0504       0x4000_0504
        7:0     R/W    PMUX_GPIO_ADC_4                         8'h0
        15:8    R/W    PMUX_GPIO_ADC_5                         8'h0
        23:16   R/W    PMUX_GPIO_ADC_6                         8'h0
        31:24   R/W    PMUX_GPIO_ADC_7                         8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO_ADC_4_7;
        struct
        {
            __IO uint32_t PMUX_GPIO_ADC_4: 8;
            __IO uint32_t PMUX_GPIO_ADC_5: 8;
            __IO uint32_t PMUX_GPIO_ADC_6: 8;
            __IO uint32_t PMUX_GPIO_ADC_7: 8;
        } BITS_504;
    } u_504;

    /* 0x0508       0x4000_0508
        7:0     R/W    PMUX_GPIO_P1_0                          8'h38
        15:8    R/W    PMUX_GPIO_P1_1                          8'h39
        23:16   R/W    PMUX_GPIO_P1_2                          8'h0
        31:24   R/W    PMUX_GPIO_P1_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO1_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P1_0: 8;
            __IO uint32_t PMUX_GPIO_P1_1: 8;
            __IO uint32_t PMUX_GPIO_P1_2: 8;
            __IO uint32_t PMUX_GPIO_P1_3: 8;
        } BITS_508;
    } u_508;

    /* 0x050C       0x4000_050c
        7:0     R/W    PMUX_GPIO_P1_4                          8'h0
        15:8    R/W    PMUX_GPIO_P1_5                          8'h0
        31:16   R/W    RESERVED870                             16'h0
    */
    union
    {
        __IO uint32_t REG_GPIO1_4_5;
        struct
        {
            __IO uint32_t PMUX_GPIO_P1_4: 8;
            __IO uint32_t PMUX_GPIO_P1_5: 8;
            __IO uint32_t RESERVED870: 16;
        } BITS_50C;
    } u_50C;

    /* 0x0510       0x4000_0510
        7:0     R/W    PMUX_GPIO_P2_0                          8'h18
        15:8    R/W    PMUX_GPIO_P2_1                          8'h0
        23:16   R/W    PMUX_GPIO_P2_2                          8'h0
        31:24   R/W    PMUX_GPIO_P2_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO2_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P2_0: 8;
            __IO uint32_t PMUX_GPIO_P2_1: 8;
            __IO uint32_t PMUX_GPIO_P2_2: 8;
            __IO uint32_t PMUX_GPIO_P2_3: 8;
        } BITS_510;
    } u_510;

    /* 0x0514       0x4000_0514
        7:0     R/W    PMUX_GPIO_P2_4                          8'h0
        15:8    R/W    PMUX_GPIO_P2_5                          8'h0
        23:16   R/W    PMUX_GPIO_P2_6                          8'h0
        31:24   R/W    PMUX_GPIO_P2_7                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO2_4_7;
        struct
        {
            __IO uint32_t PMUX_GPIO_P2_4: 8;
            __IO uint32_t PMUX_GPIO_P2_5: 8;
            __IO uint32_t PMUX_GPIO_P2_6: 8;
            __IO uint32_t PMUX_GPIO_P2_7: 8;
        } BITS_514;
    } u_514;

    /* 0x0518       0x4000_0518
        7:0     R/W    PMUX_GPIO_P3_0                          8'h24
        15:8    R/W    PMUX_GPIO_P3_1                          8'h23
        23:16   R/W    PMUX_GPIO_P3_2                          8'h0
        31:24   R/W    PMUX_GPIO_P3_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO3_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P3_0: 8;
            __IO uint32_t PMUX_GPIO_P3_1: 8;
            __IO uint32_t PMUX_GPIO_P3_2: 8;
            __IO uint32_t PMUX_GPIO_P3_3: 8;
        } BITS_518;
    } u_518;

    /* 0x051C       0x4000_051c
        7:0     R/W    PMUX_GPIO_P3_4                          8'h0
        15:8    R/W    PMUX_GPIO_P3_5                          8'h0
        31:16   R/W    RESERVED889                             16'h0
    */
    union
    {
        __IO uint32_t REG_GPIO3_4_5;
        struct
        {
            __IO uint32_t PMUX_GPIO_P3_4: 8;
            __IO uint32_t PMUX_GPIO_P3_5: 8;
            __IO uint32_t RESERVED889: 16;
        } BITS_51C;
    } u_51C;

    /* 0x0520       0x4000_0520
        7:0     R/W    PMUX_GPIO_P4_0                          8'h0
        15:8    R/W    PMUX_GPIO_P4_1                          8'h0
        23:16   R/W    PMUX_GPIO_P4_2                          8'h0
        31:24   R/W    PMUX_GPIO_P4_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO4_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P4_0: 8;
            __IO uint32_t PMUX_GPIO_P4_1: 8;
            __IO uint32_t PMUX_GPIO_P4_2: 8;
            __IO uint32_t PMUX_GPIO_P4_3: 8;
        } BITS_520;
    } u_520;

    /* 0x0524       0x4000_0524
        7:0     R/W    PMUX_GPIO_P4_4                          8'h0
        15:8    R/W    PMUX_GPIO_P4_5                          8'h0
        23:16   R/W    PMUX_GPIO_P4_6                          8'h0
        31:24   R/W    PMUX_GPIO_P4_7                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO4_4_7;
        struct
        {
            __IO uint32_t PMUX_GPIO_P4_4: 8;
            __IO uint32_t PMUX_GPIO_P4_5: 8;
            __IO uint32_t PMUX_GPIO_P4_6: 8;
            __IO uint32_t PMUX_GPIO_P4_7: 8;
        } BITS_524;
    } u_524;

    /* 0x0528       0x4000_0528
        7:0     R/W    PMUX_GPIO_P5_0                          8'h0
        15:8    R/W    PMUX_GPIO_P5_1                          8'h0
        23:16   R/W    PMUX_GPIO_P5_2                          8'h0
        31:24   R/W    PMUX_GPIO_P5_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO5_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P5_0: 8;
            __IO uint32_t PMUX_GPIO_P5_1: 8;
            __IO uint32_t PMUX_GPIO_P5_2: 8;
            __IO uint32_t PMUX_GPIO_P5_3: 8;
        } BITS_528;
    } u_528;

    /* 0x052C       0x4000_052c
        7:0     R/W    PMUX_GPIO_P5_4                          8'h0
        15:8    R/W    PMUX_GPIO_P5_5                          8'h0
        23:16   R/W    PMUX_GPIO_P5_6                          8'h0
        31:24   R/W    RESERVED908                             8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO5_4_6;
        struct
        {
            __IO uint32_t PMUX_GPIO_P5_4: 8;
            __IO uint32_t PMUX_GPIO_P5_5: 8;
            __IO uint32_t PMUX_GPIO_P5_6: 8;
            __IO uint32_t RESERVED908: 8;
        } BITS_52C;
    } u_52C;

    /* 0x0530       0x4000_0530
        7:0     R/W    PMUX_GPIO_P6_0                          8'h0
        15:8    R/W    PMUX_GPIO_P6_1                          8'h0
        23:16   R/W    PMUX_GPIO_P6_2                          8'h0
        31:24   R/W    PMUX_GPIO_P6_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO6_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P6_0: 8;
            __IO uint32_t PMUX_GPIO_P6_1: 8;
            __IO uint32_t PMUX_GPIO_P6_2: 8;
            __IO uint32_t PMUX_GPIO_P6_3: 8;
        } BITS_530;
    } u_530;

    /* 0x0534       0x4000_0534
        7:0     R/W    PMUX_GPIO_P6_4                          8'h0
        15:8    R/W    PMUX_GPIO_P6_5                          8'h0
        23:16   R/W    PMUX_GPIO_P6_6                          8'h0
        31:24   R/W    RESERVED918                             8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO6_4_6;
        struct
        {
            __IO uint32_t PMUX_GPIO_P6_4: 8;
            __IO uint32_t PMUX_GPIO_P6_5: 8;
            __IO uint32_t PMUX_GPIO_P6_6: 8;
            __IO uint32_t RESERVED918: 8;
        } BITS_534;
    } u_534;

    /* 0x0538       0x4000_0538
        7:0     R/W    PMUX_GPIO_P7_0                          8'h0
        15:8    R/W    PMUX_GPIO_P7_1                          8'h0
        23:16   R/W    PMUX_GPIO_P7_2                          8'h0
        31:24   R/W    PMUX_GPIO_P7_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO7_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P7_0: 8;
            __IO uint32_t PMUX_GPIO_P7_1: 8;
            __IO uint32_t PMUX_GPIO_P7_2: 8;
            __IO uint32_t PMUX_GPIO_P7_3: 8;
        } BITS_538;
    } u_538;

    /* 0x053C       0x4000_053c
        7:0     R/W    PMUX_GPIO_P7_4                          8'h0
        15:8    R/W    PMUX_GPIO_P7_5                          8'h0
        23:16   R/W    PMUX_GPIO_P7_6                          8'h0
        31:24   R/W    PMUX_GPIO_P7_7                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO7_4_7;
        struct
        {
            __IO uint32_t PMUX_GPIO_P7_4: 8;
            __IO uint32_t PMUX_GPIO_P7_5: 8;
            __IO uint32_t PMUX_GPIO_P7_6: 8;
            __IO uint32_t PMUX_GPIO_P7_7: 8;
        } BITS_53C;
    } u_53C;

    /* 0x0540       0x4000_0540
        7:0     R/W    PMUX_GPIO_P8_0                          8'h0
        15:8    R/W    PMUX_GPIO_P8_1                          8'h0
        23:16   R/W    PMUX_GPIO_P8_2                          8'h0
        31:24   R/W    PMUX_GPIO_P8_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO8_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P8_0: 8;
            __IO uint32_t PMUX_GPIO_P8_1: 8;
            __IO uint32_t PMUX_GPIO_P8_2: 8;
            __IO uint32_t PMUX_GPIO_P8_3: 8;
        } BITS_540;
    } u_540;

    /* 0x0544       0x4000_0544
        7:0     R/W    PMUX_GPIO_P8_4                          8'h0
        15:8    R/W    PMUX_GPIO_P8_5                          8'h0
        23:16   R/W    PMUX_GPIO_P8_6                          8'h0
        31:24   R/W    PMUX_GPIO_P8_7                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO8_4_7;
        struct
        {
            __IO uint32_t PMUX_GPIO_P8_4: 8;
            __IO uint32_t PMUX_GPIO_P8_5: 8;
            __IO uint32_t PMUX_GPIO_P8_6: 8;
            __IO uint32_t PMUX_GPIO_P8_7: 8;
        } BITS_544;
    } u_544;

    /* 0x0548       0x4000_0548
        7:0     R/W    PMUX_GPIO_P9_0                          8'h0
        15:8    R/W    PMUX_GPIO_P9_1                          8'h0
        23:16   R/W    PMUX_GPIO_P9_2                          8'h0
        31:24   R/W    PMUX_GPIO_P9_3                          8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO9_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P9_0: 8;
            __IO uint32_t PMUX_GPIO_P9_1: 8;
            __IO uint32_t PMUX_GPIO_P9_2: 8;
            __IO uint32_t PMUX_GPIO_P9_3: 8;
        } BITS_548;
    } u_548;

    /* 0x054C       0x4000_054c
        7:0     R/W    PMUX_GPIO_P9_4                          8'h0
        15:8    R/W    PMUX_GPIO_P9_5                          8'h0
        23:16   R/W    PMUX_GPIO_P9_6                          8'h0
        31:24   R/W    RESERVED948                             8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO9_4_6;
        struct
        {
            __IO uint32_t PMUX_GPIO_P9_4: 8;
            __IO uint32_t PMUX_GPIO_P9_5: 8;
            __IO uint32_t PMUX_GPIO_P9_6: 8;
            __IO uint32_t RESERVED948: 8;
        } BITS_54C;
    } u_54C;

    /* 0x0550       0x4000_0550
        7:0     R/W    PMUX_GPIO_P10_0                         8'h0
        15:8    R/W    PMUX_GPIO_P10_1                         8'h0
        23:16   R/W    PMUX_GPIO_P10_2                         8'h0
        31:24   R/W    PMUX_GPIO_P10_3                         8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO10_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_P10_0: 8;
            __IO uint32_t PMUX_GPIO_P10_1: 8;
            __IO uint32_t PMUX_GPIO_P10_2: 8;
            __IO uint32_t PMUX_GPIO_P10_3: 8;
        } BITS_550;
    } u_550;

    /* 0x0554       0x4000_0554
        7:0     R/W    PMUX_GPIO_P10_4                         8'h0
        15:8    R/W    PMUX_GPIO_P10_5                         8'h0
        23:16   R/W    PMUX_GPIO_P10_6                         8'h0
        31:24   R/W    PMUX_GPIO_P10_7                         8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO10_4_7;
        struct
        {
            __IO uint32_t PMUX_GPIO_P10_4: 8;
            __IO uint32_t PMUX_GPIO_P10_5: 8;
            __IO uint32_t PMUX_GPIO_P10_6: 8;
            __IO uint32_t PMUX_GPIO_P10_7: 8;
        } BITS_554;
    } u_554;

    /* 0x0558       0x4000_0558
        7:0     R/W    PMUX_GPIO_H_0                           8'h0
        15:8    R/W    PMUX_GPIO_H_1                           8'h0
        23:16   R/W    PMUX_GPIO_H_2                           8'h0
        31:24   R/W    PMUX_GPIO_H_3                           8'h0
    */
    union
    {
        __IO uint32_t REG_GPIOH_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_H_0: 8;
            __IO uint32_t PMUX_GPIO_H_1: 8;
            __IO uint32_t PMUX_GPIO_H_2: 8;
            __IO uint32_t PMUX_GPIO_H_3: 8;
        } BITS_558;
    } u_558;

    /* 0x055C       0x4000_055c
        7:0     R/W    PMUX_GPIO_H_4                           8'h2e
        15:8    R/W    PMUX_GPIO_H_5                           8'h2f
        23:16   R/W    PMUX_GPIO_H_6                           8'h30
        31:24   R/W    PMUX_GPIO_H_7                           8'h2d
    */
    union
    {
        __IO uint32_t REG_GPIOH_4_6;
        struct
        {
            __IO uint32_t PMUX_GPIO_H_4: 8;
            __IO uint32_t PMUX_GPIO_H_5: 8;
            __IO uint32_t PMUX_GPIO_H_6: 8;
            __IO uint32_t PMUX_GPIO_H_7: 8;
        } BITS_55C;
    } u_55C;

    /* 0x0560       0x4000_0560
        7:0     R/W    PMUX_GPIO_H_8                           8'h0
        31:8    R/W    RESERVED973                             24'h0
    */
    union
    {
        __IO uint32_t REG_GPIO_USB_0_1;
        struct
        {
            __IO uint32_t PMUX_GPIO_H_8: 8;
            __IO uint32_t RESERVED973: 24;
        } BITS_560;
    } u_560;

    /* 0x0564       0x4000_0564
        7:0     R/W    PMUX_GPIO_USB_0                         8'h0
        15:8    R/W    PMUX_GPIO_USB_1                         8'h0
        31:16   R/W    RESERVED976                             15'h0
    */
    union
    {
        __IO uint32_t REG_GPIO_USB_0_1;
        struct
        {
            __IO uint32_t PMUX_GPIO_USB_0: 8;
            __IO uint32_t PMUX_GPIO_USB_1: 8;
            __IO uint32_t RESERVED976: 16;
        } BITS_564;
    } u_564;

    /* 0x0568       0x4000_0568
        7:0     R/W    PMUX_GPIO_SPIC1_0                       8'h0
        15:8    R/W    PMUX_GPIO_SPIC1_1                       8'h0
        23:16   R/W    PMUX_GPIO_SPIC1_2                       8'h0
        31:24   R/W    PMUX_GPIO_SPIC1_3                       8'h0
    */
    union
    {
        __IO uint32_t REG_GPIO_SPIC1_0_3;
        struct
        {
            __IO uint32_t PMUX_GPIO_SPIC1_0: 8;
            __IO uint32_t PMUX_GPIO_SPIC1_1: 8;
            __IO uint32_t PMUX_GPIO_SPIC1_2: 8;
            __IO uint32_t PMUX_GPIO_SPIC1_3: 8;
        } BITS_568;
    } u_568;

    /* 0x056C       0x4000_056c
        7:0     R/W    PMUX_GPIO_SPIC1_4                       8'h0
        15:8    R/W    PMUX_GPIO_SPIC1_5                       8'h0
        31:16   R/W    RESERVED985                             16'h0
    */
    union
    {
        __IO uint32_t REG_GPIO_SPIC1_4_5;
        struct
        {
            __IO uint32_t PMUX_GPIO_SPIC1_4: 8;
            __IO uint32_t PMUX_GPIO_SPIC1_5: 8;
            __IO uint32_t RESERVED985: 16;
        } BITS_56C;
    } u_56C;


} SYS_BLKCTRL_TypeDef;

#define SYSBLKCTRL_REG_BASE             0x40000200UL
#define SYSBLKCTRL                      ((SYS_BLKCTRL_TypeDef      *) SYSBLKCTRL_REG_BASE)

#endif //#define PCC_REG_H