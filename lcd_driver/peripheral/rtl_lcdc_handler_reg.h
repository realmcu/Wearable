#ifndef RTL_LCDC_Handler_REG_H
#define RTL_LCDC_Handler_REG_H

#include <stdint.h>
#include <stdbool.h>
#include "rtl876x.h"

#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ================================================================================ */
/* ============= Refence: [REG]Bee3pro_Display_controller_register_table_20221130.xlsx =============== */
/* ================================================================================ */
typedef struct
{
    __IO uint32_t INTERFACE_SELECT;         /*!< 0x00 */
    __IO uint32_t FT_IN;                    /*!< 0x04 */
    __IO uint32_t FT_OUT;                   /*!< 0x08 */
    __IO uint32_t BIT_SWAP;                 /*!< 0x0C */
    __IO uint32_t TEAR_CTR;                 /*!< 0x10 */
    __IO uint32_t OPERATE_CTR;              /*!< 0x14 */
    __IO uint32_t DMA_FIFO_CTRL;            /*!< 0x18 */
    __IO uint32_t DMA_FIFO_IMR;             /*!< 0x1C */
    __IO uint32_t DMA_FIFO_SR;              /*!< 0x20 */
    __IO uint32_t DMA_FIFO_ICR;             /*!< 0x24 */
    __IO uint32_t TX_LEN;                   /*!< 0x28 */
    __IO uint32_t TX_CNT;                   /*!< 0x2C */
    __IO uint32_t RSVD;                     /*!< 0x30 */
    __IO uint32_t RX_LEN;                   /*!< 0x34 */
    __IO uint32_t RX_CNT;                   /*!< 0x38 */
    __IO uint32_t DMA_FIFO;                 /*!< 0x3C */
    __IO uint32_t AXI_MUX_MODE;             /*!< 0x40 */
    __IO uint32_t SPIC_CMD;                 /*!< 0x44 */
    __IO uint32_t SPIC_ADDR;                /*!< 0x48 */
    __IO uint32_t DMA_MULTI_BLOCK_CTRL;     /*!< 0x4C */
    __IO uint32_t DMA_MULTI_BLOCK_SIZE1;    /*!< 0x50 */
    __IO uint32_t DMA_MULTI_BLOCK_SIZE2;    /*!< 0x54 */
    __IO uint32_t DMA_FIFO_OFFSET;          /*!< 0x58 */
    __IO uint32_t DPHY_STOP_STATE_TIMEOUT;  /*!< 0x5C */
    __IO uint32_t SPIC_SCLK_PHY;            /*!< 0x60 */
    __IO uint32_t INFINITE_MODE_CTR;        /*!< 0x64 */
    __IO uint32_t SPIC_RX_FIFO_READ_SIZE;   /*!< 0x68 */
} LCDC_Handler_TypeDef;

/* 0x00
   0       R/W    display_controller_enable                       0x0
   2:1     R/W    interface_select                                0x0
   3       R/W    reset_lcd_display_module                        0x1
   31:4    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t display_controller_enable: 1;
        __IO uint32_t interface_select: 2;
        __IO uint32_t reset_lcd_display_module: 1;
        __I uint32_t reserved_0: 28;
    } b;
} INTERFACE_SELECT_t;



/* 0x04
   2:0     R/W    input_format                                    0x0
   31:3    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t input_format: 3;
        __I uint32_t reserved_0: 29;
    } b;
} FT_IN_t;



/* 0x08
   2:0     R/W    output_format                                   0x0
   31:3    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t output_format: 3;
        __I uint32_t reserved_0: 29;
    } b;
} FT_OUT_t;



/* 0x0C
   1:0     R/W    bit_swap                                        0x0
   31:2    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t bit_swap: 2;
        __I uint32_t reserved_0: 30;
    } b;
} BIT_SWAP_t;



/* 0x10
   0       R/W    tear_auto_turn_on_autowritestart                0x0
   1       R/W    tear_auto_turn_on_dma_en                        0x1
   2       R/W    tear_polarity                                   0x0
   3       R      tear                                            0x0
   4       R/W    tear_input_mux                                  0x0
   15:5    R      reserved2                                       0x0
   27:16   R/W    t2w_delay                                       0x0
   30:28   R      reserved1                                       0x0
   31      R/W    bypass_t2w_delay                                0x1
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t tear_auto_turn_on_autowritestart: 1;
        __IO uint32_t tear_auto_turn_on_dma_en: 1;
        __IO uint32_t tear_polarity: 1;
        __I uint32_t tear: 1;
        __IO uint32_t tear_input_mux: 1;
        __I uint32_t reserved_1: 11;
        __IO uint32_t t2w_delay: 12;
        __I uint32_t reserved_0: 3;
        __IO uint32_t bypass_t2w_delay: 1;
    } b;
} TEAR_CTR_t;



/* 0x14
   0       R/W    access_mode                                     0x1
   1       R/W    data_rw                                         0x0
   29:2    R      reserved                                        0x0
   30      R/W    auto_read_start                                 0x0
   31      R/W    auto_write_start                                0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t access_mode: 1;
        __IO uint32_t data_rw: 1;
        __I uint32_t reserved_0: 28;
        __IO uint32_t auto_read_start: 1;
        __IO uint32_t auto_write_start: 1;
    } b;
} OPERATE_CTR_t;



/* 0x18
   0       R/W    dma_enable                                      0x0
   7:1     R      reserved2                                       0x0
   8       R/W    dma_burst_request_only                          0x0
   15:9    R      reserved1                                       0x0
   31:16   R/W    dma_fifo_threshold                              0x8
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t dma_enable: 1;
        __I uint32_t reserved_1: 7;
        __IO uint32_t dma_burst_request_only: 1;
        __I uint32_t reserved_0: 7;
        __IO uint32_t dma_fifo_threshold: 16;
    } b;
} DMA_FIFO_CTRL_t;



/* 0x1C
   3:0     R      reserved2                                       0x0
   4       R/W    tx_fifo_transfer_error_int_msk                  0x1
   5       R/W    tx_fifo_threshold_int_msk                       0x1
   6       R/W    tx_fifo_overflow_int_msk                        0x1
   7       R/W    tx_fifo_empty_int_msk                           0x1
   8       R/W    tx_auto_done_int_msk                            0x1
   9       R/W    rx_fifo_overflow_int_msk                        0x1
   10      R/W    rx_auto_done_int_msk                            0x1
   11      R/W    tear_trigger_int_msk                            0x1
   29:12   R      reserved1                                       0x0
   30      R/W    display_controller_waveform_finish_int_msk      0x0
   31      R/W    multi_block_last_block_start_int_msk            0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __I uint32_t reserved_1: 4;
        __IO uint32_t tx_fifo_transfer_error_int_msk: 1;
        __IO uint32_t tx_fifo_threshold_int_msk: 1;
        __IO uint32_t tx_fifo_overflow_int_msk: 1;
        __IO uint32_t tx_fifo_empty_int_msk: 1;
        __IO uint32_t tx_auto_done_int_msk: 1;
        __IO uint32_t rx_fifo_overflow_int_msk: 1;
        __IO uint32_t rx_auto_done_int_msk: 1;
        __IO uint32_t tear_trigger_int_msk: 1;
        __I uint32_t reserved_0: 18;
        __IO uint32_t display_controller_waveform_finish_int_msk: 1;
        __IO uint32_t multi_block_last_block_start_int_msk: 1;
    } b;
} DMA_FIFO_IMR_t;



/* 0x20
   0       R      tx_fifo_transfer_error_int_flag                 0x0
   1       R      tx_fifo_threshold_int_flag                      0x0
   2       R      tx_fifo_overflow_int_flag                       0x0
   3       R      tx_fifo_empty_int_flag                          0x1
   4       R      tx_fifo_full_status                             0x0
   5       R      tx_fifo_empty_status                            0x1
   6       R      tx_auto_done_int                                0x0
   15:7    R      reserved2                                       0x0
   16      R      rx_fifo_overflow_int                            0x0
   17      R      rx_auto_done_int                                0x0
   18      R      rx_fifo_full_status                             0x0
   19      R      rx_fifo_empty_status                            0x1
   20      R      tear_trigger_int_status                         0x0
   29:21   R      reserved1                                       0x0
   30      R      display_controller_waveform_finish_int_status   0x0
   31      R      multi_block_last_block_start_int_status         0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __I uint32_t tx_fifo_transfer_error_int_flag: 1;
        __I uint32_t tx_fifo_threshold_int_flag: 1;
        __I uint32_t tx_fifo_overflow_int_flag: 1;
        __I uint32_t tx_fifo_empty_int_flag: 1;
        __I uint32_t tx_fifo_full_status: 1;
        __I uint32_t tx_fifo_empty_status: 1;
        __I uint32_t tx_auto_done_int: 1;
        __I uint32_t reserved_1: 9;
        __I uint32_t rx_fifo_overflow_int: 1;
        __I uint32_t rx_auto_done_int: 1;
        __I uint32_t rx_fifo_full_status: 1;
        __I uint32_t rx_fifo_empty_status: 1;
        __I uint32_t tear_trigger_int_status: 1;
        __I uint32_t reserved_0: 9;
        __I uint32_t display_controller_waveform_finish_int_status: 1;
        __I uint32_t multi_block_last_block_start_int_status: 1;
    } b;
} DMA_FIFO_SR_t;



/* 0x24
   0       R/W1C  fifo_clr                                        0x0
   1       R/W1C  tx_fifo_transfer_error_int_clr                  0x0
   2       R/W1C  tx_output_cnt_clr                               0x0
   3       R/W1C  tx_fifo_threshold_int_clr                       0x0
   4       R/W1C  tx_fifo_overflow_int_clr                        0x0
   5       R/W1C  tx_fifo_empty_int_clr                           0x0
   6       R/W1C  tx_auto_done_int_clr                            0x0
   7       R/W1C  rx_output_cnt_clr                               0x0
   8       R/W1C  rx_fifo_overflow_int_clr                        0x0
   9       R/W1C  rx_auto_done_int_clr                            0x0
   10      R/W1C  tear_trigger_int_clr                            0x0
   29:11   R      reserved1                                       0x0
   30      R/W1C  display_controller_waveform_finish_int_clr      0x0
   31      R/W1C  multi_block_last_block_start_int_clr            0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t fifo_clr: 1;
        __IO uint32_t tx_fifo_transfer_error_int_clr: 1;
        __IO uint32_t tx_output_cnt_clr: 1;
        __IO uint32_t tx_fifo_threshold_int_clr: 1;
        __IO uint32_t tx_fifo_overflow_int_clr: 1;
        __IO uint32_t tx_fifo_empty_int_clr: 1;
        __IO uint32_t tx_auto_done_int_clr: 1;
        __IO uint32_t rx_output_cnt_clr: 1;
        __IO uint32_t rx_fifo_overflow_int_clr: 1;
        __IO uint32_t rx_auto_done_int_clr: 1;
        __IO uint32_t tear_trigger_int_clr: 1;
        __I uint32_t reserved_0: 19;
        __IO uint32_t display_controller_waveform_finish_int_clr: 1;
        __IO uint32_t multi_block_last_block_start_int_clr: 1;
    } b;
} DMA_FIFO_ICR_t;



/* 0x28
   31:0    R/W    tx_output_pixel_num                             0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t tx_output_pixel_num: 32;
    } b;
} TX_LEN_t;



/* 0x2C
   31:0    R      tx_output_pixel_cnt                             0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __I uint32_t tx_output_pixel_cnt: 32;
    } b;
} TX_CNT_t;



/* 0x34
   31:0    R/W    rx_read_byte_num                                0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t rx_read_byte_num: 32;
    } b;
} RX_LEN_t;



/* 0x38
   31:0    R      rx_read_cnt                                     0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __I uint32_t rx_read_cnt: 32;
    } b;
} RX_CNT_t;



/* 0x3C
   31:0    R/W    fifo_data                                       0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t fifo_data: 32;
    } b;
} DMA_FIFO_t;



/* 0x40
   0       R/W    axi_mux_mode                                    0x0
   31:1    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t axi_mux_mode: 1;
        __I uint32_t reserved_0: 31;
    } b;
} AXI_MUX_MODE_t;



/* 0x44
   7:0     R/W    spic_cmd                                        0x0
   31:8    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t spic_cmd: 8;
        __I uint32_t reserved_0: 24;
    } b;
} SPIC_CMD_t;



/* 0x48
   23:0    R/W    spic_addr                                       0x0
   31:24   R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t spic_addr: 24;
        __I uint32_t reserved_0: 8;
    } b;
} SPIC_ADDR_t;



/* 0x4C
   0       R/W    dma_multi_block_en                              0x0
   7:1     R      reserved                                        0x0
   8       R/W    link_list_enable                                0x0
   15:9    R      reserved2                                       0x0
   26:16   R      dma_load_cnt                                    0x0
   30:27   R      reserved1                                       0x0
   31      W1C    reg_dma_load_cnt_clr                            0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t dma_multi_block_en: 1;
        __I uint32_t reserved_2: 7;
        __IO uint32_t link_list_enable: 1;
        __I uint32_t reserved_1: 7;
        __I uint32_t dma_load_cnt: 11;
        __I uint32_t reserved_0: 4;
        __IO uint32_t reg_dma_load_cnt_clr: 1;
    } b;
} DMA_MULTI_BLOCK_CTRL_t;



/* 0x50
   31:0    R/W    block_size_group_1                              0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t block_size_group_1: 32;
    } b;
} DMA_MULTI_BLOCK_SIZE1_t;



/* 0x54
   31:0    R/W    block_size_group_2                              0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t block_size_group_2: 32;
    } b;
} DMA_MULTI_BLOCK_SIZE2_t;



/* 0x58
   15:0    R      tx_fifo_offset                                  0x0
   31:16   R      rx_fifo_offset                                  0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __I uint32_t tx_fifo_offset: 16;
        __I uint32_t rx_fifo_offset: 16;
    } b;
} DMA_FIFO_OFFSET_t;



/* 0x5C
   31:0    R/W    dphy_stop_state_timeout                         0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t dphy_stop_state_timeout: 32;
    } b;
} DPHY_STOP_STATE_TIMEOUT_t;



/* 0x60
   1:0     R/W    spi_sclk_phase_shift                            0x0
   7:2     R      reserved2                                       0x0
   15:8    R/W    r_clko_dly_se                                   0x0
   31:16   R      reserved1                                       0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t spi_sclk_phase_shift: 2;
        __I uint32_t reserved_1: 6;
        __IO uint32_t r_clko_dly_se: 8;
        __I uint32_t reserved_0: 16;
    } b;
} SPIC_SCLK_PHY_t;



/* 0x64
   11:0    R/W    infinite_f2f_dmaen2wen_delay                    0x0
   15:12   R      reserved2                                       0x0
   27:16   R/W    infinite_f2f_stop2dmaen_delay                   0x0
   30:28   R      reserved1                                       0x0
   31      R/W    infinite_mode_en                                0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t infinite_f2f_dmaen2wen_delay: 12;
        __I uint32_t reserved_1: 4;
        __IO uint32_t infinite_f2f_stop2dmaen_delay: 12;
        __I uint32_t reserved_0: 3;
        __IO uint32_t infinite_mode_en: 1;
    } b;
} INFINITE_MODE_CTR_t;



/* 0x68
   1:0     R      spic_rx_fifo_read_size                          0x0
   31:2    R      reserved                                        0x0
*/
typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __I uint32_t spic_rx_fifo_read_size: 2;
        __I uint32_t reserved_0: 30;
    } b;
} SPIC_RX_FIFO_READ_SIZE_t;


#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif /* RTL_LCDC_Handler_REG_H */
