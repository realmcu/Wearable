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

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "rtl_rtzip.h"
#if defined RTL87x2G
#include "rtl_gdma.h"
#include "rtl_rcc.h"
#include "rtl_nvic.h"
#else
#include "rtl876x_gdma.h"
#include "rtl876x_rcc.h"
#include "rtl876x_nvic.h"
#endif
GDMA_LLIDef RX_GDMA_LLIStruct[20];
GDMA_LLIDef TX_GDMA_LLIStruct[20];
static void RTZIP_CLKConfig(FunctionalState state)
{
    assert_param(IS_FUNCTIONAL_STATE(state));
#if defined RTL87x2G
    uint32_t *reg = (uint32_t *)(0x40002000 + 0x324);
#elif defined RTL8763EP
    uint32_t *reg = (uint32_t *)(0x40000000 + 0x210);
#endif
    uint32_t reg_value = *reg;
    if (state)
    {
#if defined RTL87x2G
        reg_value |= (BIT7 | BIT8);
#elif defined RTL8763EP
        reg_value |= BIT26;
#endif
    }
    else
    {
#if defined RTL87x2G
        reg_value &= (~(BIT7 | BIT8));
#elif defined RTL8763EP
        reg_value &= (~BIT26);
#endif
    }
    *reg = reg_value;
}

void RTZIP_TxFifoClear(void)
{
    RTZIP_CTL1_t rtzip_reg_0x04 = {.d32 = RTZIP->RTZIP_CTL1};
    rtzip_reg_0x04.b.tx_fifo_clear = 1;
    RTZIP->RTZIP_CTL1 = rtzip_reg_0x04.d32;
}

void RTZIP_RxFifoClear(void)
{
    RTZIP_CTL1_t rtzip_reg_0x04 = {.d32 = RTZIP->RTZIP_CTL1};
    rtzip_reg_0x04.b.rx_fifo_clear = 1;
    RTZIP->RTZIP_CTL1 = rtzip_reg_0x04.d32;
}

ITStatus RTZIP_GetINTStatus(uint32_t RTZIP_INT)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    //assert_param(IS_RTZIP_INT(RTZIP_INT));

    if (RTZIP->INT_STATUS & RTZIP_INT)
    {
        bit_status = SET;
    }

    return  bit_status;
}

void RTZIP_INTConfig(uint32_t RTZIP_INT, FunctionalState NewState)
{
    /* Check the parameters */
    //assert_param(IS_RTZIP_INT(RTZIP_INT_MSK));
    //assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        RTZIP->INT_ENABLE |= RTZIP_INT;
    }
    else
    {
        RTZIP->INT_ENABLE &= ~RTZIP_INT;
    }
}

void RTZIP_MaskINTConfig(uint32_t RTZIP_INT_MSK, FunctionalState NewState)
{
    /* Check the parameters */
    //assert_param(IS_RTZIP_INT(RTZIP_INT_MSK));
    //assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        RTZIP->INT_MASK |= RTZIP_INT_MSK;
    }
    else
    {
        RTZIP->INT_MASK &= ~RTZIP_INT_MSK;
    }
}

void RTZIP_ClearINTPendingBit(uint32_t RTZIP_INT)
{
    //assert_param(IS_RTZIP_INT(RTZIP_INT));
    RTZIP->INT_CLEAR |= RTZIP_INT;
}

void RTZIP_Cmd(FunctionalState state)
{
    assert_param(IS_FUNCTIONAL_STATE(state));
    RTZIP_CTL0_t rtzip_reg_0x00 = {.d32 = RTZIP->RTZIP_CTL0};
    if (state)
    {
        rtzip_reg_0x00.b.rt_zip_enable = 1;
    }
    else
    {
        rtzip_reg_0x00.b.rt_zip_enable = 0;
    }
    RTZIP->RTZIP_CTL0 = rtzip_reg_0x00.d32;
}

void RTZIP_Run(FunctionalState state)
{
    assert_param(IS_FUNCTIONAL_STATE(state));
    RTZIP_CTL0_t rtzip_reg_0x00 = {.d32 = RTZIP->RTZIP_CTL0};
    if (state)
    {
        rtzip_reg_0x00.b.rt_zip_reset_decompress_start = 1;
    }
    else
    {
        rtzip_reg_0x00.b.rt_zip_reset_decompress_start = 0;
    }
    RTZIP->RTZIP_CTL0 = rtzip_reg_0x00.d32;
}

ITStatus RTZIP_GetInternalBufferState(void)
{
    ITStatus rv = RESET;
    RTZIP_CTL1_t rtzip_reg_0x04 = {.d32 = RTZIP->RTZIP_CTL1};
    if (rtzip_reg_0x04.b.rtzip_internal_buffer_empty)
    {
        rv = SET;
    }
    return rv;
}

uint32_t RTZIP_GetDecompressLine(void)
{
    uint32_t reg_value = RTZIP->VERTICAL_COUNTER;
    return reg_value;
}

uint32_t RTZIP_GetHorizontalByteNum(void)
{
    uint32_t reg_value = RTZIP->HORIZONTAL_COUNTER;
    return reg_value;
}

uint32_t RTZIP_GetCompressedSize(void)
{
    uint32_t reg_value = RTZIP->COMPRESSED_DATA_SIZE;
    return reg_value;
}

static uint32_t RTZIP_Get_Line_Start_Address(uint32_t compressed_start_address,
                                             uint32_t line_number)
{
    // line number start from0 and ends to (raw_pic_height-1)
    // return the absolute address of line

    RTZIP_file_header *rle_file_header = ((RTZIP_file_header *)(compressed_start_address));
    if (line_number > rle_file_header->raw_pic_height)
    {
//        DBG_DIRECT("[error]line number won't be equals to or bigger than pic height");
        return 0;
    }
    else
    {
        uint32_t *line_address = (uint32_t *)(compressed_start_address + 12 + line_number * 4);
        return (line_address[0] + compressed_start_address);
    }
}


void RTZIP_Init(RTZIP_InitTypeDef *RTZIP_init_struct)
{
    RTZIP_InitTypeDef *RTZIP_struct_init = RTZIP_init_struct;
    RTZIP_CTL0_t rtzip_reg_0x00 = {.d32 = RTZIP->RTZIP_CTL0};
    rtzip_reg_0x00.b.rt_zip_algorithm = RTZIP_struct_init->algorithm_type;
    RTZIP->RTZIP_CTL0 = rtzip_reg_0x00.d32;

    RTZIP_CTL1_t rtzip_reg_0x04 = {.d32 = RTZIP->RTZIP_CTL1};
    rtzip_reg_0x04.b.head_throw_away_byte_num = RTZIP_struct_init->head_throw_away_byte_num;
    rtzip_reg_0x04.b.pic_pixel_size = RTZIP_struct_init->pic_pixel_size;
#if defined(RTL8763EP)
    rtzip_reg_0x04.b.hw_handshake_mux = RTZIP_struct_init->hw_handshake;
#endif
    RTZIP->RTZIP_CTL1 = rtzip_reg_0x04.d32;

    RTZIP->PIC_RAW_WIDTH = RTZIP_struct_init->pic_raw_width;
    RTZIP->TX_COLUMN_START = RTZIP_struct_init->tx_column_start;
    RTZIP->TX_COLUMN_END = RTZIP_struct_init->tx_column_end;
    RTZIP->DECOMPRESS_OUTPUT_PIXEL = (
                                         RTZIP_struct_init->pic_decompress_height\
                                         * (RTZIP_struct_init->tx_column_end - RTZIP_struct_init->tx_column_start + 1)
                                     );
    RTZIP->PIC_DECOMPRESS_TOTAL_PIXEL = RTZIP_struct_init->pic_raw_width *
                                        RTZIP_struct_init->pic_decompress_height;

    RLE_FASTLZ_CTL_t rtzip_reg_0x1c = {.d32 = RTZIP->RLE_FASTLZ_CTL};
    rtzip_reg_0x1c.b.pic_length1_size = RTZIP_struct_init->pic_length1_size;
    rtzip_reg_0x1c.b.pic_length2_size = RTZIP_struct_init->pic_length2_size;
    RTZIP->RLE_FASTLZ_CTL = rtzip_reg_0x1c.d32;

    YUV_SBF_CTL_t rtzip_reg_0x20 = {.d32 = RTZIP->YUV_SBF_CTL};
    rtzip_reg_0x20.b.yuv_blur_bit = RTZIP_struct_init->yuv_blur_bit;
    rtzip_reg_0x20.b.yuv_sample_type = RTZIP_struct_init->yuv_sample_type;
    RTZIP->YUV_SBF_CTL = rtzip_reg_0x20.d32;

    RTZIP->COMPRESSED_DATA_SIZE = RTZIP_struct_init->compressed_data_size;

    RX_FIFO_DMA_THRESHOLD_t rtzip_reg_0x48 = {.d32 = RTZIP->RX_FIFO_DMA_THRESHOLD};
    rtzip_reg_0x48.b.rx_fifo_dma_threshold = RTZIP_struct_init->rx_fifo_dma_threshold;
    rtzip_reg_0x48.b.rx_dma_enable = RTZIP_struct_init->rx_fifo_dma_enable;
    RTZIP->RX_FIFO_DMA_THRESHOLD = rtzip_reg_0x48.d32;

    TX_FIFO_DMA_THRESHOLD_t rtzip_reg_0x4c = {.d32 = RTZIP->TX_FIFO_DMA_THRESHOLD};
    rtzip_reg_0x4c.b.tx_fifo_dma_threshold = RTZIP_struct_init->tx_fifo_dma_threshold;
    rtzip_reg_0x4c.b.tx_dma_enable = RTZIP_struct_init->tx_fifo_dma_enable;
    RTZIP->TX_FIFO_DMA_THRESHOLD = rtzip_reg_0x4c.d32;

    RX_FIFO_INT_THRESHOLD_t rtzip_reg_0x50 = {.d32 = RTZIP->RX_FIFO_INT_THRESHOLD};
    rtzip_reg_0x50.b.rx_fifo_int_threshold = RTZIP_struct_init->rx_fifo_int_threshold;
    RTZIP->RX_FIFO_INT_THRESHOLD = rtzip_reg_0x50.d32;

    TX_FIFO_INT_THRESHOLD_t rtzip_reg_0x54 = {.d32 = RTZIP->RX_FIFO_INT_THRESHOLD};
    rtzip_reg_0x54.b.tx_fifo_int_threshold = RTZIP_struct_init->tx_fifo_int_threshold;
    RTZIP->TX_FIFO_INT_THRESHOLD = rtzip_reg_0x54.d32;
}

RTZIP_ERROR RTZIP_Decode(uint8_t *file, RTZIP_decode_range *range, RTZIP_DMA_config *dma_cfg)
{
    RTZIP_ERROR err = RTZIP_SUCCESS;
    uint32_t decompress_start_line;
    uint32_t decompress_end_line;
    uint32_t decompress_start_column;
    uint32_t decompress_end_column;
    RTZIP_CLKConfig(ENABLE);
    if (file == NULL)
    {
        return RTZIP_ERROR_NULL_INPUT;
    }
    if (dma_cfg == NULL)
    {
        return RTZIP_ERROR_INVALID_PARAM;
    }
    uint32_t compressed_data_start_address = (uint32_t)file;
    RTZIP_file_header *header = (RTZIP_file_header *)compressed_data_start_address;
    if (range != NULL)
    {
        if ((range->start_line >= header->raw_pic_height) || (range->start_column >= header->raw_pic_width))
        {
            return RTZIP_ERROR_START_EXCEED_BOUNDARY;
        }
        if ((range->start_line > range->end_line) || (range->start_column > range->end_column))
        {
            return RTZIP_ERROR_START_LARGER_THAN_END;
        }
        if ((range->end_line >= header->raw_pic_height) || (range->end_column >= header->raw_pic_width))
        {
            return RTZIP_ERROR_END_EXCEED_BOUNDARY;
        }
        decompress_start_line = range->start_line;
        decompress_end_line = range->end_line;
        decompress_start_column = range->start_column;
        decompress_end_column = range->end_column;
    }
    else
    {
        decompress_start_line = 0;
        decompress_end_line = header->raw_pic_height - 1;
        decompress_start_column = 0;
        decompress_end_column = header->raw_pic_width - 1;
    }
    if ((!IS_RTZIP_ALGORITHM(header->algorithm_type.algorithm)) ||
        (!IS_RTZIP_PIXEL_BYTES(header->algorithm_type.pixel_bytes)))
    {
        return RTZIP_ERROR_INVALID_PARAM;
    }
    uint32_t start_line_address = RTZIP_Get_Line_Start_Address(compressed_data_start_address,
                                                               decompress_start_line);
    uint32_t compressed_data_size = RTZIP_Get_Line_Start_Address(compressed_data_start_address,
                                                                 decompress_end_line + 1) - start_line_address;
    uint32_t decompressed_data_size = (decompress_end_line - decompress_start_line + 1) *
                                      (decompress_end_column - decompress_start_column + 1)\
                                      * (header->algorithm_type.pixel_bytes + 2);

    RTZIP_InitTypeDef RTZIP_struct_init;
    RTZIP_struct_init.algorithm_type            = (RTZIP_ALGORITHM)header->algorithm_type.algorithm;
    RTZIP_struct_init.head_throw_away_byte_num  = THROW_AWAY_0BYTE;
    RTZIP_struct_init.pic_pixel_size            = (RTZIP_PIXEL_SIZE)header->algorithm_type.pixel_bytes;
    RTZIP_struct_init.pic_decompress_height     = (decompress_end_line - decompress_start_line + 1);
    RTZIP_struct_init.pic_raw_width             = header->raw_pic_width;
    RTZIP_struct_init.tx_column_start           = decompress_start_column;
    RTZIP_struct_init.tx_column_end             = decompress_end_column ;
    RTZIP_struct_init.compressed_data_size      = compressed_data_size;
    RTZIP_struct_init.pic_length2_size          = (RTZIP_RLE_RUNLENGTH_SIZE)
                                                  header->algorithm_type.feature_2;
    RTZIP_struct_init.pic_length1_size          = (RTZIP_RLE_RUNLENGTH_SIZE)
                                                  header->algorithm_type.feature_1;
    RTZIP_struct_init.yuv_blur_bit              = (RTZIP_YUV_BLUR_BIT)header->algorithm_type.feature_2;
    RTZIP_struct_init.yuv_sample_type           = (RTZIP_YUV_SAMPLE_TYPE)
                                                  header->algorithm_type.feature_1;
    RTZIP_struct_init.rx_fifo_dma_enable        = (uint32_t)ENABLE;
    RTZIP_struct_init.tx_fifo_dma_enable        = (uint32_t)ENABLE;
    RTZIP_struct_init.rx_fifo_dma_threshold     = RTZIP_RX_FIFO_DEPTH / 2;
    RTZIP_struct_init.tx_fifo_dma_threshold     = RTZIP_TX_FIFO_DEPTH / 2;
    RTZIP_struct_init.rx_fifo_int_threshold     = dma_cfg->RX_FIFO_INT_threshold;
    RTZIP_struct_init.tx_fifo_int_threshold     = dma_cfg->TX_FIFO_INT_threshold;
#if defined(RTL8763EP)
    RTZIP_struct_init.hw_handshake              = RTZIP_HW_HANDSHAKE_DMA;
#endif
    RTZIP_Init(&RTZIP_struct_init);


    uint32_t DMA_compressed_data_size_word = ((compressed_data_size % 4) ?
                                              (compressed_data_size / 4 + 1) : (compressed_data_size / 4));
    uint32_t DMA_decompressed_data_size_word = ((decompressed_data_size % 4) ?
                                                (decompressed_data_size / 4 + 1) : (decompressed_data_size / 4));
    /* Configure DMA */
    RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, ENABLE);
    GDMA_InitTypeDef RX_GDMA_InitStruct;
    uint32_t rx_block_num = 0;
    if (compressed_data_size >= 65532)
    {
        rx_block_num = compressed_data_size / 65532;
        if (compressed_data_size % 65532)
        {
            rx_block_num = rx_block_num + 1;
        }
    }
    /*--------------GDMA init-----------------------------*/
    GDMA_StructInit(&RX_GDMA_InitStruct);
    RX_GDMA_InitStruct.GDMA_ChannelNum          = dma_cfg->RX_DMA_channel_num;
    RX_GDMA_InitStruct.GDMA_BufferSize          = DMA_compressed_data_size_word;
    RX_GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    RX_GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    RX_GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    RX_GDMA_InitStruct.GDMA_SourceMsize         =
        GDMA_Msize_16;                         // 8 msize for source msize
    RX_GDMA_InitStruct.GDMA_DestinationMsize    =
        GDMA_Msize_8;                         // 8 msize for destiantion msize
    RX_GDMA_InitStruct.GDMA_DestinationDataSize =
        GDMA_DataSize_Word;                   // 32 bit width for destination transaction
    RX_GDMA_InitStruct.GDMA_SourceDataSize      =
        GDMA_DataSize_Word;                   // 32 bit width for source transaction
    RX_GDMA_InitStruct.GDMA_SourceAddr          = (uint32_t)start_line_address;
    RX_GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)(&RTZIP->RX_FIFO);
#if defined RTL87x2G
    RX_GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_RTZIP_RX;
    RX_GDMA_InitStruct.GDMA_Secure_En = 1;
#elif defined RTL8763EP
    RX_GDMA_InitStruct.GDMA_DestHandshake       = 43;
#endif
    if (rx_block_num)
    {
        RX_GDMA_InitStruct.GDMA_BufferSize = 65532 / 4;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Mode = LLI_TRANSFER;
        RX_GDMA_InitStruct.GDMA_Multi_Block_En = 1;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Struct = (uint32_t)RX_GDMA_LLIStruct;
    }
    GDMA_Init(dma_cfg->RX_DMA_channel, &RX_GDMA_InitStruct);
    if (rx_block_num)
    {
        for (int i = 0; i < rx_block_num; i++)
        {
            if (i == rx_block_num - 1)
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = 0;
                /* configure low 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_LOW = (BIT(0)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                                | (GDMA_DataSize_Byte << 4)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationInc << 7)
                                                | (RX_GDMA_InitStruct.GDMA_SourceInc << 9)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                                | (RX_GDMA_InitStruct.GDMA_SourceMsize << 14)
                                                | (RX_GDMA_InitStruct.GDMA_DIR << 20));
                /* configure high 32 bit of CTL register */
                uint32_t block_size = compressed_data_size - 65532 * i;
                RX_GDMA_LLIStruct[i].CTL_HIGH = block_size;
            }
            else
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = (uint32_t)&RX_GDMA_LLIStruct[i + 1];
                /* configure low 32 bit of CTL register */
#if defined(RTL8763EP)
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->CTL_LOW;
#else
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->GDMA_CTL_LOWx;
#endif
                /* configure high 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_HIGH = 65532 / 4;
            }
        }
    }

    GDMA_InitTypeDef TX_GDMA_InitStruct;
    uint32_t tx_block_num = 0;
    if (compressed_data_size >= 65532)
    {
        tx_block_num = compressed_data_size / 65532;
        if (compressed_data_size % 65532)
        {
            tx_block_num = tx_block_num + 1;
        }
    }
    /*--------------GDMA init-----------------------------*/
    GDMA_StructInit(&TX_GDMA_InitStruct);
    TX_GDMA_InitStruct.GDMA_ChannelNum          = dma_cfg->TX_DMA_channel_num;
    TX_GDMA_InitStruct.GDMA_BufferSize          = DMA_decompressed_data_size_word;
    TX_GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_PeripheralToMemory;
    TX_GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Fix;
    TX_GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Inc;
    TX_GDMA_InitStruct.GDMA_SourceMsize         =
        GDMA_Msize_8;                         // 8 msize for source msize
    TX_GDMA_InitStruct.GDMA_DestinationMsize    =
        GDMA_Msize_8;                         // 8 msize for destiantion msize
    TX_GDMA_InitStruct.GDMA_DestinationDataSize =
        GDMA_DataSize_Word;                   // 32 bit width for destination transaction
    TX_GDMA_InitStruct.GDMA_SourceDataSize      =
        GDMA_DataSize_Word;                   // 32 bit width for source transaction
    TX_GDMA_InitStruct.GDMA_SourceAddr          = (uint32_t)(&RTZIP->TX_FIFO);
    TX_GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)dma_cfg->output_buf;
#if defined RTL87x2G
    TX_GDMA_InitStruct.GDMA_SourceHandshake     = GDMA_Handshake_RTZIP_TX;
    TX_GDMA_InitStruct.GDMA_Secure_En = 1;
#elif defined RTL8763EP
    TX_GDMA_InitStruct.GDMA_SourceHandshake     = 44;
#endif
    if (tx_block_num)
    {
        TX_GDMA_InitStruct.GDMA_BufferSize = 65532 / 4;
        TX_GDMA_InitStruct.GDMA_Multi_Block_Mode = LLI_TRANSFER;
        TX_GDMA_InitStruct.GDMA_Multi_Block_En = 1;
        TX_GDMA_InitStruct.GDMA_Multi_Block_Struct = (uint32_t)TX_GDMA_LLIStruct;
    }
    GDMA_Init(dma_cfg->TX_DMA_channel, &TX_GDMA_InitStruct);
    if (tx_block_num)
    {
        for (int i = 0; i < tx_block_num; i++)
        {
            if (i == tx_block_num - 1)
            {
                TX_GDMA_LLIStruct[i].SAR = (uint32_t)(&RTZIP->TX_FIFO);
                TX_GDMA_LLIStruct[i].DAR = (uint32_t)dma_cfg->output_buf + 65532 * i;
                TX_GDMA_LLIStruct[i].LLP = 0;
                /* configure low 32 bit of CTL register */
                TX_GDMA_LLIStruct[i].CTL_LOW = (BIT(0)
                                                | (TX_GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                                | (GDMA_DataSize_Byte << 4)
                                                | (TX_GDMA_InitStruct.GDMA_DestinationInc << 7)
                                                | (TX_GDMA_InitStruct.GDMA_SourceInc << 9)
                                                | (TX_GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                                | (TX_GDMA_InitStruct.GDMA_SourceMsize << 14)
                                                | (TX_GDMA_InitStruct.GDMA_DIR << 20));
                /* configure high 32 bit of CTL register */
                uint32_t block_size = compressed_data_size - 65532 * i;
                TX_GDMA_LLIStruct[i].CTL_HIGH = block_size;
            }
            else
            {
                TX_GDMA_LLIStruct[i].SAR = (uint32_t)(&RTZIP->TX_FIFO);
                TX_GDMA_LLIStruct[i].DAR = (uint32_t)dma_cfg->output_buf + 65532 * i;
                TX_GDMA_LLIStruct[i].LLP = (uint32_t)&TX_GDMA_LLIStruct[i + 1];
                /* configure low 32 bit of CTL register */
#if defined(RTL8763EP)
                TX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->TX_DMA_channel->CTL_LOW;
#else
                TX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->TX_DMA_channel->GDMA_CTL_LOWx;
#endif
                /* configure high 32 bit of CTL register */
                TX_GDMA_LLIStruct[i].CTL_HIGH = 65532 / 4;
            }
        }
    }

    GDMA_Cmd(dma_cfg->RX_DMA_channel_num, ENABLE);
    GDMA_Cmd(dma_cfg->TX_DMA_channel_num, ENABLE);

    RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_FINISH_INT);
    RTZIP_INTConfig(RTZIP_DECOMPRESS_FINISH_INT, ENABLE);
    RTZIP_MaskINTConfig(RTZIP_DECOMPRESS_FINISH_INT, DISABLE);

    RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_ERROR_INT);
    RTZIP_INTConfig(RTZIP_DECOMPRESS_ERROR_INT, ENABLE);
    RTZIP_MaskINTConfig(RTZIP_DECOMPRESS_ERROR_INT, DISABLE);

    RTZIP_Cmd(ENABLE);
    RTZIP_Run(ENABLE);
    while (RTZIP->RTZIP_CTL0 & BIT0);
    if (RTZIP_GetINTStatus(RTZIP_DECOMPRESS_ERROR_INT))
    {
        err =  RTZIP_ERROR_DECODE_FAIL;
    }
#if defined(RTL8763EP)
    if (GDMA0->ChEnReg & (0x01 << dma_cfg->RX_DMA_channel_num))
#else
    if (GDMA0->GDMA_ChEnReg & (0x01 << dma_cfg->RX_DMA_channel_num))
#endif
    {
        GDMA_SuspendCmd(dma_cfg->TX_DMA_channel, ENABLE);
        GDMA_SuspendCmd(dma_cfg->RX_DMA_channel, ENABLE);
#if defined(RTL8763EP)
        while (!(dma_cfg->RX_DMA_channel->CFG_LOW & BIT0));
        while (!(dma_cfg->TX_DMA_channel->CFG_LOW & BIT0));
#else
        while (!(dma_cfg->RX_DMA_channel->GDMA_CFG_LOWx & BIT0));
        while (!(dma_cfg->TX_DMA_channel->GDMA_CFG_LOWx & BIT0));
#endif
        GDMA_Cmd(dma_cfg->RX_DMA_channel_num, DISABLE);
        GDMA_Cmd(dma_cfg->TX_DMA_channel_num, DISABLE);
    }
    RTZIP_RxFifoClear();
    RTZIP_Cmd(DISABLE);
    while (!(RTZIP->RTZIP_CTL1 & BIT29));
    RTZIP_TxFifoClear();
    return err;
}

RTZIP_ERROR RTZIP_Decode_Ex(uint8_t *file, RTZIP_decode_range *range, RTZIP_DMA_config *dma_cfg,
                            RTZIP_INT_CFG_t int_cfg)
{
    uint32_t decompress_start_line;
    uint32_t decompress_end_line;
    uint32_t decompress_start_column;
    uint32_t decompress_end_column;
    RTZIP_CLKConfig(ENABLE);
    if (file == NULL)
    {
        return RTZIP_ERROR_NULL_INPUT;
    }
    if (dma_cfg == NULL)
    {
        return RTZIP_ERROR_INVALID_PARAM;
    }
    uint32_t compressed_data_start_address = (uint32_t)file;
    RTZIP_file_header *header = (RTZIP_file_header *)compressed_data_start_address;
    if (range != NULL)
    {
        if ((range->start_line >= header->raw_pic_height) || (range->start_column >= header->raw_pic_width))
        {
            return RTZIP_ERROR_START_EXCEED_BOUNDARY;
        }
        if ((range->start_line > range->end_line) || (range->start_column > range->end_column))
        {
            return RTZIP_ERROR_START_LARGER_THAN_END;
        }
        if ((range->end_line >= header->raw_pic_height) || (range->end_column >= header->raw_pic_width))
        {
            return RTZIP_ERROR_END_EXCEED_BOUNDARY;
        }
        decompress_start_line = range->start_line;
        decompress_end_line = range->end_line;
        decompress_start_column = range->start_column;
        decompress_end_column = range->end_column;
    }
    else
    {
        decompress_start_line = 0;
        decompress_end_line = header->raw_pic_height - 1;
        decompress_start_column = 0;
        decompress_end_column = header->raw_pic_width - 1;
    }
    if ((!IS_RTZIP_ALGORITHM(header->algorithm_type.algorithm)) ||
        (!IS_RTZIP_PIXEL_BYTES(header->algorithm_type.pixel_bytes)))
    {
        return RTZIP_ERROR_INVALID_PARAM;
    }

    uint32_t start_line_address = RTZIP_Get_Line_Start_Address(compressed_data_start_address,
                                                               decompress_start_line);
    uint32_t compressed_data_size = RTZIP_Get_Line_Start_Address(compressed_data_start_address,
                                                                 decompress_end_line + 1) - start_line_address;
    uint32_t decompressed_data_size = (decompress_end_line - decompress_start_line + 1) *
                                      (decompress_end_column - decompress_start_column + 1) *
                                      (header->algorithm_type.pixel_bytes + 2);

    RTZIP_InitTypeDef RTZIP_struct_init;
    RTZIP_struct_init.algorithm_type            = (RTZIP_ALGORITHM)header->algorithm_type.algorithm;
    RTZIP_struct_init.head_throw_away_byte_num  = THROW_AWAY_0BYTE;
    RTZIP_struct_init.pic_pixel_size            = (RTZIP_PIXEL_SIZE)header->algorithm_type.pixel_bytes;
    RTZIP_struct_init.pic_decompress_height     = (decompress_end_line - decompress_start_line + 1);
    RTZIP_struct_init.pic_raw_width             = header->raw_pic_width;
    RTZIP_struct_init.tx_column_start           = decompress_start_column;
    RTZIP_struct_init.tx_column_end             = decompress_end_column ;
    RTZIP_struct_init.compressed_data_size      = compressed_data_size;
    RTZIP_struct_init.pic_length2_size          = (RTZIP_RLE_RUNLENGTH_SIZE)
                                                  header->algorithm_type.feature_2;
    RTZIP_struct_init.pic_length1_size          = (RTZIP_RLE_RUNLENGTH_SIZE)
                                                  header->algorithm_type.feature_1;
    RTZIP_struct_init.yuv_blur_bit              = (RTZIP_YUV_BLUR_BIT)header->algorithm_type.feature_2;
    RTZIP_struct_init.yuv_sample_type           = (RTZIP_YUV_SAMPLE_TYPE)
                                                  header->algorithm_type.feature_1;
    RTZIP_struct_init.rx_fifo_dma_enable        = (uint32_t)ENABLE;
    RTZIP_struct_init.tx_fifo_dma_enable        = (uint32_t)ENABLE;
    RTZIP_struct_init.rx_fifo_dma_threshold     = RTZIP_RX_FIFO_DEPTH / 2;
    RTZIP_struct_init.tx_fifo_dma_threshold     = RTZIP_TX_FIFO_DEPTH / 2;
    RTZIP_struct_init.rx_fifo_int_threshold     = dma_cfg->RX_FIFO_INT_threshold;
    RTZIP_struct_init.tx_fifo_int_threshold     = dma_cfg->TX_FIFO_INT_threshold;
    RTZIP_struct_init.hw_handshake              = RTZIP_HW_HANDSHAKE_DMA;
    RTZIP_Init(&RTZIP_struct_init);


    uint32_t DMA_compressed_data_size_word = ((compressed_data_size % 4) ?
                                              (compressed_data_size / 4 + 1) : (compressed_data_size / 4));
    uint32_t DMA_decompressed_data_size_word = ((decompressed_data_size % 4) ?
                                                (decompressed_data_size / 4 + 1) : (decompressed_data_size / 4));
    /* Configure DMA */
    RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, ENABLE);
    GDMA_InitTypeDef RX_GDMA_InitStruct;
    uint32_t rx_block_num = 0;
    if (compressed_data_size >= 65532)
    {
        rx_block_num = compressed_data_size / 65532;
        if (compressed_data_size % 65532)
        {
            rx_block_num = rx_block_num + 1;
        }
    }
    /*--------------GDMA init-----------------------------*/
    GDMA_StructInit(&RX_GDMA_InitStruct);
    RX_GDMA_InitStruct.GDMA_ChannelNum          = dma_cfg->RX_DMA_channel_num;
    RX_GDMA_InitStruct.GDMA_BufferSize          = DMA_compressed_data_size_word;
    RX_GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    RX_GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    RX_GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    RX_GDMA_InitStruct.GDMA_SourceMsize         =
        GDMA_Msize_8;                         // 8 msize for source msize
    RX_GDMA_InitStruct.GDMA_DestinationMsize    =
        GDMA_Msize_8;                         // 8 msize for destiantion msize
    RX_GDMA_InitStruct.GDMA_DestinationDataSize =
        GDMA_DataSize_Word;                   // 32 bit width for destination transaction
    RX_GDMA_InitStruct.GDMA_SourceDataSize      =
        GDMA_DataSize_Word;                   // 32 bit width for source transaction
    RX_GDMA_InitStruct.GDMA_SourceAddr          = (uint32_t)start_line_address;
    RX_GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)(&RTZIP->RX_FIFO);
#if defined RTL87x2G
    RX_GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_RTZIP_RX;
    RX_GDMA_InitStruct.GDMA_Secure_En = 1;
#elif defined RTL8763EP
    RX_GDMA_InitStruct.GDMA_DestHandshake       = 43;
#endif
    if (rx_block_num)
    {
        RX_GDMA_InitStruct.GDMA_BufferSize = 65532 / 4;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Mode = LLI_TRANSFER;
        RX_GDMA_InitStruct.GDMA_Multi_Block_En = 1;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Struct = (uint32_t)RX_GDMA_LLIStruct;
    }
    GDMA_Init(dma_cfg->RX_DMA_channel, &RX_GDMA_InitStruct);
    if (rx_block_num)
    {
        for (int i = 0; i < rx_block_num; i++)
        {
            if (i == rx_block_num - 1)
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = 0;
                /* configure low 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_LOW = (BIT(0)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                                | (GDMA_DataSize_Byte << 4)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationInc << 7)
                                                | (RX_GDMA_InitStruct.GDMA_SourceInc << 9)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                                | (RX_GDMA_InitStruct.GDMA_SourceMsize << 14)
                                                | (RX_GDMA_InitStruct.GDMA_DIR << 20));
                /* configure high 32 bit of CTL register */
                uint32_t block_size = compressed_data_size - 65532 * i;
                RX_GDMA_LLIStruct[i].CTL_HIGH = block_size;
            }
            else
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = (uint32_t)&RX_GDMA_LLIStruct[i + 1];
                /* configure low 32 bit of CTL register */
#if defined(RTL8763EP)
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->CTL_LOW;
#else
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->GDMA_CTL_LOWx;
#endif
                /* configure high 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_HIGH = 65532 / 4;
            }
        }
    }

    GDMA_InitTypeDef TX_GDMA_InitStruct;
    uint32_t tx_block_num = 0;
    if (compressed_data_size >= 65532)
    {
        tx_block_num = compressed_data_size / 65532;
        if (compressed_data_size % 65532)
        {
            tx_block_num = tx_block_num + 1;
        }
    }
    /*--------------GDMA init-----------------------------*/
    GDMA_StructInit(&TX_GDMA_InitStruct);
    TX_GDMA_InitStruct.GDMA_ChannelNum          = dma_cfg->TX_DMA_channel_num;
    TX_GDMA_InitStruct.GDMA_BufferSize          = DMA_decompressed_data_size_word;
    TX_GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_PeripheralToMemory;
    TX_GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Fix;
    TX_GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Inc;
    TX_GDMA_InitStruct.GDMA_SourceMsize         =
        GDMA_Msize_8;                         // 8 msize for source msize
    TX_GDMA_InitStruct.GDMA_DestinationMsize    =
        GDMA_Msize_8;                         // 8 msize for destiantion msize
    TX_GDMA_InitStruct.GDMA_DestinationDataSize =
        GDMA_DataSize_Word;                   // 32 bit width for destination transaction
    TX_GDMA_InitStruct.GDMA_SourceDataSize      =
        GDMA_DataSize_Word;                   // 32 bit width for source transaction
    TX_GDMA_InitStruct.GDMA_SourceAddr          = (uint32_t)(&RTZIP->TX_FIFO);
    TX_GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)dma_cfg->output_buf;
#if defined RTL87x2G
    TX_GDMA_InitStruct.GDMA_SourceHandshake     = GDMA_Handshake_RTZIP_TX;
    TX_GDMA_InitStruct.GDMA_Secure_En = 1;
#elif defined RTL8763EP
    TX_GDMA_InitStruct.GDMA_SourceHandshake     = 44;
#endif
    if (rx_block_num)
    {
        RX_GDMA_InitStruct.GDMA_BufferSize = 65532 / 4;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Mode = LLI_TRANSFER;
        RX_GDMA_InitStruct.GDMA_Multi_Block_En = 1;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Struct = (uint32_t)RX_GDMA_LLIStruct;
    }
    GDMA_Init(dma_cfg->RX_DMA_channel, &RX_GDMA_InitStruct);
    if (rx_block_num)
    {
        for (int i = 0; i < rx_block_num; i++)
        {
            if (i == rx_block_num - 1)
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = 0;
                /* configure low 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_LOW = (BIT(0)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                                | (GDMA_DataSize_Byte << 4)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationInc << 7)
                                                | (RX_GDMA_InitStruct.GDMA_SourceInc << 9)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                                | (RX_GDMA_InitStruct.GDMA_SourceMsize << 14)
                                                | (RX_GDMA_InitStruct.GDMA_DIR << 20));
                /* configure high 32 bit of CTL register */
                uint32_t block_size = compressed_data_size - 65532 * i;
                RX_GDMA_LLIStruct[i].CTL_HIGH = block_size;
            }
            else
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = (uint32_t)&RX_GDMA_LLIStruct[i + 1];
                /* configure low 32 bit of CTL register */
#if defined(RTL8763EP)
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->CTL_LOW;
#else
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->GDMA_CTL_LOWx;
#endif
                /* configure high 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_HIGH = 65532 / 4;
            }
        }
    }
    GDMA_Init(dma_cfg->TX_DMA_channel, &TX_GDMA_InitStruct);

    GDMA_Cmd(dma_cfg->RX_DMA_channel_num, ENABLE);
    GDMA_Cmd(dma_cfg->TX_DMA_channel_num, ENABLE);

    if (int_cfg.b.rtzip_decompress_finish_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_FINISH_INT);
        RTZIP_INTConfig(RTZIP_DECOMPRESS_FINISH_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_DECOMPRESS_FINISH_INT, DISABLE);
    }
    if (int_cfg.b.rt_zip_decompress_error_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_ERROR_INT);
        RTZIP_INTConfig(RTZIP_DECOMPRESS_ERROR_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_DECOMPRESS_ERROR_INT, DISABLE);
    }
    if (int_cfg.b.line_decompress_finish_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_LINE_DECOMPRESS_FINISH_INT);
        RTZIP_INTConfig(RTZIP_LINE_DECOMPRESS_FINISH_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_LINE_DECOMPRESS_FINISH_INT, DISABLE);
    }
    if (int_cfg.b.rx_fifo_overflow_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_RX_FIFO_OVERFLOW_INT);
        RTZIP_INTConfig(RTZIP_RX_FIFO_OVERFLOW_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_RX_FIFO_OVERFLOW_INT, DISABLE);
    }
    if (int_cfg.b.rx_fifo_threshold_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_RX_FIFO_THRESHOLD_INT);
        RTZIP_INTConfig(RTZIP_RX_FIFO_THRESHOLD_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_RX_FIFO_THRESHOLD_INT, DISABLE);
    }
    if (int_cfg.b.tx_fifo_threshold_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_TX_FIFO_THRESHOLD_INT);
        RTZIP_INTConfig(RTZIP_TX_FIFO_THRESHOLD_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_TX_FIFO_THRESHOLD_INT, DISABLE);
    }
    if (int_cfg.b.tx_fifo_underflow_int)
    {
        RTZIP_ClearINTPendingBit(RTZIP_TX_FIFO_UNDERFLOW_INT);
        RTZIP_INTConfig(RTZIP_TX_FIFO_UNDERFLOW_INT, ENABLE);
        RTZIP_MaskINTConfig(RTZIP_TX_FIFO_UNDERFLOW_INT, DISABLE);
    }

#if defined RTL87x2G
    //TODO: wait RTL8763G platform to add rtzip defines
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = RTZIP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
#endif

    RTZIP_Cmd(ENABLE);
    RTZIP_Run(ENABLE);
    return RTZIP_SUCCESS;
}

#if defined(RTL8763EP)
RTZIP_ERROR RTZIP_Decode_Direct(uint8_t *file, RTZIP_decode_range *range, RTZIP_DMA_config *dma_cfg)
{
    uint32_t decompress_start_line;
    uint32_t decompress_end_line;
    uint32_t decompress_start_column;
    uint32_t decompress_end_column;
    RTZIP_CLKConfig(ENABLE);
    if (file == NULL)
    {
        return RTZIP_ERROR_NULL_INPUT;
    }
    if (dma_cfg == NULL)
    {
        return RTZIP_ERROR_INVALID_PARAM;
    }
    uint32_t compressed_data_start_address = (uint32_t)file;
    RTZIP_file_header *header = (RTZIP_file_header *)compressed_data_start_address;
    if (range != NULL)
    {
        if ((range->start_line >= header->raw_pic_height) || (range->start_column >= header->raw_pic_width))
        {
            return RTZIP_ERROR_START_EXCEED_BOUNDARY;
        }
        if ((range->start_line > range->end_line) || (range->start_column > range->end_column))
        {
            return RTZIP_ERROR_START_LARGER_THAN_END;
        }
        if ((range->end_line >= header->raw_pic_height) || (range->end_column >= header->raw_pic_width))
        {
            return RTZIP_ERROR_END_EXCEED_BOUNDARY;
        }
        decompress_start_line = range->start_line;
        decompress_end_line = range->end_line;
        decompress_start_column = range->start_column;
        decompress_end_column = range->end_column;
    }
    else
    {
        decompress_start_line = 0;
        decompress_end_line = header->raw_pic_height - 1;
        decompress_start_column = 0;
        decompress_end_column = header->raw_pic_width - 1;
    }
    if ((!IS_RTZIP_ALGORITHM(header->algorithm_type.algorithm)) ||
        (!IS_RTZIP_PIXEL_BYTES(header->algorithm_type.pixel_bytes)))
    {
        return RTZIP_ERROR_INVALID_PARAM;
    }

    uint32_t start_line_address = RTZIP_Get_Line_Start_Address(compressed_data_start_address,
                                                               decompress_start_line);
    uint32_t compressed_data_size = RTZIP_Get_Line_Start_Address(compressed_data_start_address,
                                                                 decompress_end_line + 1) - start_line_address;

    RTZIP_InitTypeDef RTZIP_struct_init;
    RTZIP_struct_init.algorithm_type            = (RTZIP_ALGORITHM)header->algorithm_type.algorithm;
    RTZIP_struct_init.head_throw_away_byte_num  = THROW_AWAY_0BYTE;
    RTZIP_struct_init.pic_pixel_size            = (RTZIP_PIXEL_SIZE)header->algorithm_type.pixel_bytes;
    RTZIP_struct_init.pic_decompress_height     = (decompress_end_line - decompress_start_line + 1);
    RTZIP_struct_init.pic_raw_width             = header->raw_pic_width;
    RTZIP_struct_init.tx_column_start           = decompress_start_column;
    RTZIP_struct_init.tx_column_end             = decompress_end_column ;
    RTZIP_struct_init.compressed_data_size      = compressed_data_size;
    RTZIP_struct_init.pic_length2_size          = (RTZIP_RLE_RUNLENGTH_SIZE)
                                                  header->algorithm_type.feature_2;
    RTZIP_struct_init.pic_length1_size          = (RTZIP_RLE_RUNLENGTH_SIZE)
                                                  header->algorithm_type.feature_1;
    RTZIP_struct_init.yuv_blur_bit              = (RTZIP_YUV_BLUR_BIT)header->algorithm_type.feature_2;
    RTZIP_struct_init.yuv_sample_type           = (RTZIP_YUV_SAMPLE_TYPE)
                                                  header->algorithm_type.feature_1;
    RTZIP_struct_init.rx_fifo_dma_enable        = (uint32_t)ENABLE;
    RTZIP_struct_init.tx_fifo_dma_enable        = (uint32_t)ENABLE;
    RTZIP_struct_init.rx_fifo_dma_threshold     = RTZIP_RX_FIFO_DEPTH / 2;
    RTZIP_struct_init.tx_fifo_dma_threshold     = RTZIP_TX_FIFO_DEPTH / 2;
    RTZIP_struct_init.rx_fifo_int_threshold     = dma_cfg->RX_FIFO_INT_threshold;
    RTZIP_struct_init.tx_fifo_int_threshold     = dma_cfg->TX_FIFO_INT_threshold;
    RTZIP_struct_init.hw_handshake              = RTZIP_HW_HANDSHAKE_PPE;
    RTZIP_Init(&RTZIP_struct_init);


    uint32_t DMA_compressed_data_size_word = ((compressed_data_size % 4) ?
                                              (compressed_data_size / 4 + 1) : (compressed_data_size / 4));
    /* Configure DMA */
    RCC_PeriphClockCmd(APBPeriph_GDMA, APBPeriph_GDMA_CLOCK, ENABLE);
    GDMA_InitTypeDef RX_GDMA_InitStruct;
    uint32_t block_num = 0;
    if (compressed_data_size >= 65532)
    {
        block_num = compressed_data_size / 65532;
        if (compressed_data_size % 65532)
        {
            block_num = block_num + 1;
        }
    }
    /*--------------GDMA init-----------------------------*/
    GDMA_StructInit(&RX_GDMA_InitStruct);
    RX_GDMA_InitStruct.GDMA_ChannelNum          = dma_cfg->RX_DMA_channel_num;
    RX_GDMA_InitStruct.GDMA_BufferSize          = DMA_compressed_data_size_word;
    RX_GDMA_InitStruct.GDMA_DIR                 = GDMA_DIR_MemoryToPeripheral;
    RX_GDMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    RX_GDMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    RX_GDMA_InitStruct.GDMA_SourceMsize         =
        GDMA_Msize_16;                         // 8 msize for source msize
    RX_GDMA_InitStruct.GDMA_DestinationMsize    =
        GDMA_Msize_8;                         // 8 msize for destiantion msize
    RX_GDMA_InitStruct.GDMA_DestinationDataSize =
        GDMA_DataSize_Word;                   // 32 bit width for destination transaction
    RX_GDMA_InitStruct.GDMA_SourceDataSize      =
        GDMA_DataSize_Word;                   // 32 bit width for source transaction
    RX_GDMA_InitStruct.GDMA_SourceAddr          = (uint32_t)start_line_address;
    RX_GDMA_InitStruct.GDMA_DestinationAddr     = (uint32_t)(&RTZIP->RX_FIFO);
#if defined RTL87x2G
    RX_GDMA_InitStruct.GDMA_DestHandshake       = GDMA_Handshake_RTZIP_RX;
    RX_GDMA_InitStruct.GDMA_Secure_En = 1;
#elif defined RTL8763EP
    RX_GDMA_InitStruct.GDMA_DestHandshake       = 43;
#endif
    if (block_num)
    {
        RX_GDMA_InitStruct.GDMA_BufferSize = 65532 / 4;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Mode = LLI_TRANSFER;
        RX_GDMA_InitStruct.GDMA_Multi_Block_En = 1;
        RX_GDMA_InitStruct.GDMA_Multi_Block_Struct = (uint32_t)RX_GDMA_LLIStruct;
    }
    GDMA_Init(dma_cfg->RX_DMA_channel, &RX_GDMA_InitStruct);
    if (block_num)
    {
        for (int i = 0; i < block_num; i++)
        {
            if (i == block_num - 1)
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = 0;
                /* configure low 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_LOW = (BIT(0)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationDataSize << 1)
                                                | (GDMA_DataSize_Byte << 4)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationInc << 7)
                                                | (RX_GDMA_InitStruct.GDMA_SourceInc << 9)
                                                | (RX_GDMA_InitStruct.GDMA_DestinationMsize << 11)
                                                | (RX_GDMA_InitStruct.GDMA_SourceMsize << 14)
                                                | (RX_GDMA_InitStruct.GDMA_DIR << 20));
                /* configure high 32 bit of CTL register */
                uint32_t block_size = compressed_data_size - 65532 * i;
                RX_GDMA_LLIStruct[i].CTL_HIGH = block_size;
            }
            else
            {
                RX_GDMA_LLIStruct[i].SAR = RX_GDMA_InitStruct.GDMA_SourceAddr + 65532 * i;
                RX_GDMA_LLIStruct[i].DAR = (uint32_t)(&RTZIP->RX_FIFO);
                RX_GDMA_LLIStruct[i].LLP = (uint32_t)&RX_GDMA_LLIStruct[i + 1];
                /* configure low 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_LOW = dma_cfg->RX_DMA_channel->CTL_LOW;
                /* configure high 32 bit of CTL register */
                RX_GDMA_LLIStruct[i].CTL_HIGH = 65532 / 4;
            }
        }
    }

    GDMA_Cmd(dma_cfg->RX_DMA_channel_num, ENABLE);

    RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_FINISH_INT);
    RTZIP_INTConfig(RTZIP_DECOMPRESS_FINISH_INT, ENABLE);
    RTZIP_MaskINTConfig(RTZIP_DECOMPRESS_FINISH_INT, DISABLE);

    RTZIP_ClearINTPendingBit(RTZIP_DECOMPRESS_ERROR_INT);
    RTZIP_INTConfig(RTZIP_DECOMPRESS_ERROR_INT, ENABLE);
    RTZIP_MaskINTConfig(RTZIP_DECOMPRESS_ERROR_INT, DISABLE);
    return RTZIP_SUCCESS;
}
#endif
