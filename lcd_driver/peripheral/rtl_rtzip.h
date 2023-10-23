/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_lcdc.h
* @brief    This file provides all the LCDC HANDLER firmware functions.
* @details
* @author   astor zhang
* @date     2022-07-29
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL8762X_RTZIP_H
#define __RTL8762X_RTZIP_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"
#include "stdbool.h"
#if defined RTL8762G
#include "rtl_gdma.h"
#else
#include "rtl876x_gdma.h"
#endif
#include "rtl_rtzip_reg.h"

typedef enum
{
    RTZIP_RLE = 0x00,
    RTZIP_FASTLZ = 0x01,
    RTZIP_YUV_SAMPLE_BLUR_FASTLZ = 0x02,
    RTZIP_YUV_SAMPLE_BLUR = 0x03,
} RTZIP_ALGORITHM;

typedef enum
{
    YUV444_SAMPLE = 0x00,
    YUV422_SAMPLE = 0x01,
    YUV411_SAMPLE = 0x02,
} RTZIP_YUV_SAMPLE_TYPE;

typedef enum
{
    YUV_BLUR_0BIT = 0x00,
    YUV_BLUR_1BIT = 0x01,
    YUV_BLUR_2BIT = 0x02,
    YUV_BLUR_4BIT = 0x03,
} RTZIP_YUV_BLUR_BIT;

typedef enum
{
    PIXEL_SIZE_16BIT = 0x00,
    PIXEL_SIZE_24BIT = 0x01,
    PIXEL_SIZE_32BIT = 0x02,
} RTZIP_PIXEL_SIZE;

typedef enum
{
    THROW_AWAY_0BYTE = 0x00,
    THROW_AWAY_1BYTE = 0x01,
    THROW_AWAY_2BYTE = 0x02,
    THROW_AWAY_3BYTE = 0x03,
} RTZIP_THROW_AWAY_SIZE;

typedef enum
{
    RUN_LENGTH_SIZE_0BYTE = 0x00,
    RUN_LENGTH_SIZE_1BYTE = 0x01,
    RUN_LENGTH_SIZE_2BYTE = 0x02,
} RTZIP_RLE_RUNLENGTH_SIZE;

typedef enum
{
    RTZIP_SUCCESS = 0x0,
    RTZIP_ERROR_NULL_INPUT,
    RTZIP_ERROR_DECODE_FAIL,
    RTZIP_ERROR_START_EXCEED_BOUNDARY,
    RTZIP_ERROR_END_EXCEED_BOUNDARY,
    RTZIP_ERROR_START_LARGER_THAN_END,
    RTZIP_ERROR_INVALID_PARAM,
} RTZIP_ERROR;

typedef enum
{
    RTZIP_HW_HANDSHAKE_DMA = 0x0,
    RTZIP_HW_HANDSHAKE_PPE = 0x1,
} RTZIP_HW_HANDSHAKE;

typedef struct
{
    struct
    {
        uint8_t algorithm: 2;
        uint8_t feature_1: 2;
        uint8_t feature_2: 2;
        uint8_t pixel_bytes: 2;
    } algorithm_type;
    uint8_t reserved[3];
    uint32_t raw_pic_width;
    uint32_t raw_pic_height;
} RTZIP_file_header;

typedef struct
{
    uint32_t start_line;
    uint32_t end_line;
    uint32_t start_column;
    uint32_t end_column;
} RTZIP_decode_range;

typedef struct
{
    uint32_t RX_DMA_channel_num;
    GDMA_ChannelTypeDef *RX_DMA_channel;
    uint32_t RX_FIFO_INT_threshold;
    uint32_t TX_DMA_channel_num;
    GDMA_ChannelTypeDef *TX_DMA_channel;
    uint32_t TX_FIFO_INT_threshold;
    uint32_t *output_buf;
} RTZIP_DMA_config;

typedef struct
{
    RTZIP_ALGORITHM algorithm_type;
    RTZIP_THROW_AWAY_SIZE head_throw_away_byte_num;
    RTZIP_PIXEL_SIZE pic_pixel_size;
    RTZIP_RLE_RUNLENGTH_SIZE pic_length2_size;
    RTZIP_RLE_RUNLENGTH_SIZE pic_length1_size;
    RTZIP_YUV_BLUR_BIT yuv_blur_bit;
    RTZIP_YUV_SAMPLE_TYPE yuv_sample_type;
    RTZIP_HW_HANDSHAKE hw_handshake;
    uint32_t pic_decompress_height;
    uint32_t pic_raw_width;
    uint32_t tx_column_start;
    uint32_t tx_column_end;
    uint32_t compressed_data_size;
    uint32_t rx_fifo_dma_enable;
    uint32_t tx_fifo_dma_enable;
    uint8_t rx_fifo_dma_threshold;
    uint8_t tx_fifo_dma_threshold;
    uint8_t tx_fifo_int_threshold;
    uint8_t rx_fifo_int_threshold;
} RTZIP_InitTypeDef;

typedef union
{
    uint32_t d32;
    uint8_t d8[4];
    struct
    {
        __IO uint32_t rt_zip_decompress_error_int: 1;
        __IO uint32_t tx_fifo_underflow_int: 1;
        __IO uint32_t tx_fifo_threshold_int: 1;
        __IO uint32_t rx_fifo_overflow_int: 1;
        __IO uint32_t rx_fifo_threshold_int: 1;
        __IO uint32_t line_decompress_finish_int: 1;
        __IO uint32_t rtzip_decompress_finish_int: 1;
        __I uint32_t reserved_0: 25;
    } b;
} RTZIP_INT_CFG_t;

#if defined RTL8763EP
#undef RTZIP_REG_BASE
#define RTZIP_REG_BASE                  0x40090000UL
#endif
#define RTZIP                           ((RTZIP_TypeDef *)RTZIP_REG_BASE)

#define RTZIP_TX_FIFO_WIDTH             32
#define RTZIP_RX_FIFO_WIDTH             32
#define RTZIP_TX_FIFO_DEPTH             16
#define RTZIP_RX_FIFO_DEPTH             16

/**
 * \defgroup    RTZIP_Interrupts_Definition RTZIP Interrupts Definition
 * \{
 * \ingroup     RTZIP_Exported_Constants
 */
#define RTZIP_DECOMPRESS_FINISH_INT                                 ((uint32_t)BIT(6))
#define RTZIP_LINE_DECOMPRESS_FINISH_INT                            ((uint32_t)BIT(5))
#define RTZIP_RX_FIFO_THRESHOLD_INT                                 ((uint32_t)BIT(4))
#define RTZIP_RX_FIFO_OVERFLOW_INT                                  ((uint32_t)BIT(3))
#define RTZIP_TX_FIFO_THRESHOLD_INT                                 ((uint32_t)BIT(2))
#define RTZIP_TX_FIFO_UNDERFLOW_INT                                  ((uint32_t)BIT(1))
#define RTZIP_DECOMPRESS_ERROR_INT                                  ((uint32_t)BIT(0))
/** \} */

#define IS_RTZIP_INT(INT)              (((INT) == RTZIP_DECOMPRESS_FINISH_INT)         || \
                                        ((INT) == RTZIP_LINE_DECOMPRESS_FINISH_INT)  || \
                                        ((INT) == RTZIP_RX_FIFO_THRESHOLD_INT)         || \
                                        ((INT) == RTZIP_RX_FIFO_OVERFLOW_INT)  || \
                                        ((INT) == RTZIP_TX_FIFO_THRESHOLD_INT)      || \
                                        ((INT) == RTZIP_TX_FIFO_UNDERFLOW_INT)         || \
                                        ((INT) == RTZIP_DECOMPRESS_ERROR_INT)

#define RTZIP_ALGO_RLE                      (0)
#define RTZIP_ALGO_FASTLZ                   (BIT(0))
#define RTZIP_ALGO_YUV_BLUR_FASTLZ          (BIT(1))
#define RTZIP_ALGO_YUV_BLUR                 (BIT(1)|BIT(0))
#define IS_RTZIP_ALGORITHM(ALGO)             (((ALGO) == RTZIP_ALGO_RLE) || ((ALGO) == RTZIP_ALGO_FASTLZ)      ||\
                                              ((ALGO) == RTZIP_ALGO_YUV_BLUR_FASTLZ) || ((ALGO) == RTZIP_ALGO_YUV_BLUR))

#define RTZIP_PIXEL_16BIT                   (0)
#define RTZIP_PIXEL_24BIT                   (BIT(0))
#define RTZIP_PIXEL_32BIT                   (BIT(1))
#define IS_RTZIP_PIXEL_BYTES(NUM)           (((NUM) == RTZIP_PIXEL_16BIT) || ((NUM) == RTZIP_PIXEL_24BIT) || ((NUM) == RTZIP_PIXEL_32BIT))

#define RTZIP_RLE_1BYTE_LEN                 (BIT(0))
#define RTZIP_RLE_2BYTE_LEN                 (BIT(1))
#define IS_RTZIP_RLE_BYTE_LEN(LEN)          (((LEN) == RTZIP_RLE_1BYTE_LEN) || ((LEN) == RTZIP_RLE_2BYTE_LEN))

#define RTZIP_YUV_BLUR_0BIT                 (0)
#define RTZIP_YUV_BLUR_1BIT                 (BIT(0))
#define RTZIP_YUV_BLUR_2BIT                 (BIT(1))
#define RTZIP_YUV_BLUR_4BIT                 (BIT(0)|BIT(1))
#define IS_RTZIP_YUB_BLUR_BIT(NUM)          (((NUM) == RTZIP_YUV_BLUR_0BIT) || ((NUM) == RTZIP_YUV_BLUR_1BIT)      ||\
                                             ((NUM) == RTZIP_YUV_BLUR_2BIT) || ((NUM) == RTZIP_YUV_BLUR_4BIT))

#define RTZIP_YUV_TYPE_444                  (0)
#define RTZIP_YUV_TYPE_422                  (BIT(0))
#define RTZIP_YUV_TYPE_411                  (BIT(1))
#define IS_RTZIP_YUV_TYPE(TYPE)             (((TYPE) == RTZIP_YUV_TYPE_444) || ((TYPE) == RTZIP_YUV_TYPE_422) || ((TYPE) == RTZIP_YUV_TYPE_411))

/*============================================================================*
 *                               Functions
 *============================================================================*/
/**
 * @brief  Clear TX FIFO
 */
void RTZIP_TxFifoClear(void);

/**
 * @brief  Clear RX FIFO
 */
void RTZIP_RxFifoClear(void);

/**
 * @brief  Clear internal memory
 */
void RTZIP_AllMemClear(void);

/**
 * @brief  Enable/Disable RTZIP interrupt
 * @param[in] RTZIP_INT_EN         interrupts to be enabled.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        RTZIP_INT_CFG_t internal_int_cfg = {.d32 = 0};
        internal_int_cfg.b.rtzip_decompress_finish_int = 1;
        internal_int_cfg.b.rt_zip_decompress_error_int = 1;
        RTZIP_INTConfig(internal_int_cfg);
    }
 * \endcode
 */
void RTZIP_INTConfig(uint32_t RTZIP_INT_EN, FunctionalState state);

/**
 * @brief  Get interrupt status of RTZIP
 * @param[in] RTZIP_INT         interrupts to be obtained.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        RTZIP_INT_CFG_t internal_int_cfg = {.d32 = 0};
        internal_int_cfg.b.rtzip_decompress_finish_int = 1;
        internal_int_cfg.b.rt_zip_decompress_error_int = 1;
        RTZIP_INT_CFG_t ret_int = RTZIP_GetINTStatus(internal_int_cfg);
        if(rev.b.rtzip_decompress_finish_int)
        {
            ...
        }
    }
 * \endcode
 */
ITStatus RTZIP_GetINTStatus(uint32_t RTZIP_INT);

/**
 * @brief  Mask/Unmask RTZIP interrupt
 * @param[in] RTZIP_INT_MSK         interrupts to be masked.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        RTZIP_INT_CFG_t internal_int_cfg = {.d32 = 0};
        internal_int_cfg.b.rtzip_decompress_finish_int = 1;
        internal_int_cfg.b.rt_zip_decompress_error_int = 1;
        RTZIP_MaskINTConfig(internal_int_cfg);
    }
 * \endcode
 */
void RTZIP_MaskINTConfig(uint32_t RTZIP_INT_MSK, FunctionalState state);

/**
 * @brief  Clear pending RTZIP interrupt
 * @param[in] RTZIP_INT         interrupts to be cleared.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        RTZIP_INT_CFG_t internal_int_cfg = {.d32 = 0};
        internal_int_cfg.b.rtzip_decompress_finish_int = 1;
        internal_int_cfg.b.rt_zip_decompress_error_int = 1;
        RTZIP_MaskINTConfig(internal_int_cfg);
    }
 * \endcode
 */
void RTZIP_ClearINTPendingBit(uint32_t RTZIP_INT);

/**
 * @brief  Get RTZIP internal buffer state
 * @return buffer state
 * @retval 0        Not empty.
 * @retval 1        Empty.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        ITStatus ret = RTZIP_GetInternalBufferState();
        if(ret)
        {
            ...
        }
    }
 * \endcode
 */
ITStatus RTZIP_GetInternalBufferState(void);

/**
 * @brief  Get vertical counter of decompress process
 * @return counter value starts from 0
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        uint32_t cnt = RTZIP_GetDecompressLine();
        DBG_DIRECT("%d lines has been decompressed", cnt + 1);
    }
 * \endcode
 */
uint32_t RTZIP_GetDecompressLine(void);

/**
 * @brief  Get horizontal counter of decompress process
 * @return counter value starts from 0
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        uint32_t cnt = RTZIP_GetHorizontalByteNum();
        DBG_DIRECT("%d pixels has been decompressed", cnt + 1);
    }
 * \endcode
 */
uint32_t RTZIP_GetHorizontalByteNum(void);

/**
 * @brief  the size of compressed data which DMA will transmit from FLASH, measured in byte
 * @return data size
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        uint32_t data_size = RTZIP_GetCompressedSize();
        DBG_DIRECT("%d bytes will be decompressed", data_size);
    }
 * \endcode
 */
uint32_t RTZIP_GetCompressedSize(void);

/**
 * @brief  RTZIP decode function, decode file from memory/flash
 * @param[in] file          address of source.
 * @param[in] range         decode range.
 * @param[in] dma_cfg       determine which DMA channel is selected.
 * @return operation result
 * @retval RTZIP_SUCCESS    Operation success.
 * @retval others           Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test_code(void){
        RCC_PeriphClockCmd(APBPeriph_RTZIP, APBPeriph_RTZIP_CLOCK, ENABLE);
        RTZIP_file_header* header = ( RTZIP_file_header*)file_data;
        DBG_DIRECT("algo = %d, blur %d, YUV %d", header->algorithm_type.algorithm, header->algorithm_type.feature_2, header->algorithm_type.feature_1);
        RTZIP_decode_range range = {0, header->raw_pic_height - 1, 0, header->raw_pic_width - 1};
        RTZIP_DMA_config dma_cfg;
        dma_cfg.output_buf = (uint32_t*)buf;
        dma_cfg.RX_DMA_channel_num = 0;
        dma_cfg.TX_DMA_channel_num = 1;
        dma_cfg.RX_DMA_channel = GDMA_Channel0;
        dma_cfg.TX_DMA_channel = GDMA_Channel1;
        RTZIP_Decode(file_data, range, dma_cfg);
    }
 * \endcode
 */
RTZIP_ERROR RTZIP_Decode(uint8_t *file, RTZIP_decode_range *range, RTZIP_DMA_config *dma_cfg);

/**
 * @brief  RTZIP decode function with interrupt, decode file from memory/flash
 * @param[in] file          address of source.
 * @param[in] range         decode range.
 * @param[in] dma_cfg       determine which DMA channel is selected.
 * @return operation result
 * @retval RTZIP_SUCCESS    Operation started.
 * @retval others           Operation aborted.
 *
 * <b>Example usage</b>
 * \code{.c}
    void RTZIP_Handler(void){
        if(RTZIP_GetINTStatus(RTZIP_DECOMPRESS_FINISH_INT_STATUS_MSK) == SET){
            RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_FINISH_INT_STATUS_MSK);
            DBG_DIRECT("RTZIP_IRQHandler line = %d\n",__LINE__);
            for(int i = 0; i < sizeof(decode_file_data); i++){
                if(decode_file_data[i] != buf[i]){

                    DBG_DIRECT("Decode Error at raw[%d] %x || out[%d] %x ",i, decode_file_data[i], i ,buf[i]);
                }
            }
            DBG_DIRECT("Decompress start bit is : %d", RTZIP->RTZIP_CTL0 & BIT0);
        }
        if(RTZIP_GetINTStatus(RTZIP_DECOMPRESS_ERROR_INT_STATUS_MSK) == SET){
            RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_ERROR_INT_STATUS_MSK);
            DBG_DIRECT("RTZIP_IRQHandler line = %d\n",__LINE__);
        }
    }
    void test_code(void){
        RCC_PeriphClockCmd(APBPeriph_RTZIP, APBPeriph_RTZIP_CLOCK, ENABLE);
        RTZIP_file_header* header = ( RTZIP_file_header*)file_data;
        DBG_DIRECT("algo = %d, blur %d, YUV %d", header->algorithm_type.algorithm, header->algorithm_type.feature_2, header->algorithm_type.feature_1);
        RTZIP_decode_range range = {0, header->raw_pic_height - 1, 0, header->raw_pic_width - 1};
        RTZIP_DMA_config dma_cfg;
        dma_cfg.output_buf = (uint32_t*)buf;
        dma_cfg.RX_DMA_channel_num = 0;
        dma_cfg.TX_DMA_channel_num = 1;
        dma_cfg.RX_DMA_channel = GDMA_Channel0;
        dma_cfg.TX_DMA_channel = GDMA_Channel1;
        RTZIP_INT_CFG_t int_cfg = {.d32 = 0};
        int_cfg.b.rtzip_decompress_finish_int = 1;
        int_cfg.b.rt_zip_decompress_error_int = 1;
        RTZIP_Decode_Ex(file_data, range, dma_cfg, int_cfg);
    }
 * \endcode
 */
RTZIP_ERROR RTZIP_Decode_Ex(uint8_t *file, RTZIP_decode_range *range, RTZIP_DMA_config *dma_cfg,
                            RTZIP_INT_CFG_t int_cfg);

void RTZIP_Cmd(FunctionalState state);
void RTZIP_Run(FunctionalState state);
void RTZIP_Init(RTZIP_InitTypeDef *RTZIP_init_struct);
#if defined(RTL8763EP)
RTZIP_ERROR RTZIP_Decode_Direct(uint8_t *file, RTZIP_decode_range *range,
                                RTZIP_DMA_config *dma_cfg);
#endif
#endif
