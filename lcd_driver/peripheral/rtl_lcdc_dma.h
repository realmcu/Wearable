/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file     rtl_lcdc_dma.h
* @brief    The header file of the peripheral GDMA driver.
* @details  This file provides all GDMA firmware functions.
* @author   HOWIE
* @date     2021-10-18
* @version  v1.0.2
* *********************************************************************************************************
*/

#ifndef RTL_LCDC_DMA_H
#define RTL_LCDC_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    GDMA        GDMA
 *
 * \brief       Manage the GDMA peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "rtl_lcdc_dma_reg.h"

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup   GDMA_Exported_Types  GDMA Exported Types
 * \brief
 * \ingroup    GDMA
 */

/**
 * \defgroup    GDMA_Data_Transfer_Direction GDMA Data Transfer Direction
 * \{
 * \ingroup     GDMA_Exported_Constants
 */

typedef enum
{
    LCDC_DMA_DIR_MemoryToMemory = 0x0,
    LCDC_DMA_DIR_MemoryToPeripheral = 0x1,
    LCDC_DMA_DIR_PeripheralToMemory = 0x2,
    LCDC_DMA_DIR_PeripheralToPeripheral = 0x3,
} LCDC_DMA_DIRECTION_T;
/** \} */ /** End of Group GDMA_Data_Transfer_Direction */

#define IS_GDMA_DIR(DIR) (((DIR) == GDMA_DIR_MemoryToMemory) || \
                          ((DIR) == GDMA_DIR_MemoryToPeripheral) || \
                          ((DIR) == GDMA_DIR_PeripheralToMemory) || \
                          ((DIR) == GDMA_DIR_PeripheralToPeripheral))

/**
 * \defgroup    GDMA_Source_Incremented_Mode GDMA Source Incremented Mode
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
typedef enum
{
    LCDC_DMA_SourceInc_Inc = 0x0,
    LCDC_DMA_SourceInc_Dec = 0x1,
    LCDC_DMA_SourceInc_Fix = 0x2,
} LCDC_DMA_SRC_INC_T;
/** \} */ /** End of Group GDMA_Source_Incremented_Mode */

#define IS_GDMA_SourceInc(STATE) (((STATE) == DMA_SourceInc_Inc) || \
                                  ((STATE) == DMA_SourceInc_Dec) || \
                                  ((STATE) == DMA_SourceInc_Fix))

/**
 * \defgroup    GDMA_Destination_Incremented_Mode GDMA Destination Incremented Mode
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
typedef enum
{
    LCDC_DMA_DestinationInc_Inc = 0x0,
    LCDC_DMA_DestinationInc_Dec = 0x1,
    LCDC_DMA_DestinationInc_Fix = 0x2,
} LCDC_DMA_DEST_INC_T;
/** \} */ /** End of Group GDMA_Destination_Incremented_Mode */

#define IS_GDMA_DestinationInc(STATE) (((STATE) == DMA_DestinationInc_Inc) || \
                                       ((STATE) == DMA_DestinationInc_Dec) || \
                                       ((STATE) == DMA_DestinationInc_Fix))

/**
 * \defgroup    GDMA_Data_Size GDMA Data Size
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
typedef enum
{
    LCDC_DMA_DataSize_Byte     = 0x0,
    LCDC_DMA_DataSize_HalfWord = 0x1,
    LCDC_DMA_DataSize_Word     = 0x2,
} LCDC_DMA_DATASIZE_T;
/** \} */ /** End of Group GDMA_Data_Size */

#define IS_GDMA_DATA_SIZE(SIZE) (((SIZE) == GDMA_DataSize_Byte) || \
                                 ((SIZE) == GDMA_DataSize_HalfWord) || \
                                 ((SIZE) == GDMA_DataSize_Word))

/**
 * \defgroup    GDMA_Msize GDMA Msize
 * \{
 * \ingroup     GDMA_Exported_Constants
 */
typedef enum
{
    LCDC_DMA_Msize_1   = 0x0,
    LCDC_DMA_Msize_4   = 0x1,
    LCDC_DMA_Msize_8   = 0x2,
    LCDC_DMA_Msize_16  = 0x3,
    LCDC_DMA_Msize_32  = 0x4,
    LCDC_DMA_Msize_64  = 0x5,
    LCDC_DMA_Msize_128 = 0x6,
    LCDC_DMA_Msize_256 = 0x7,
} LCDC_DMA_MSIZE_T;
/** \} */ /** End of Group GDMA_Msize */

#define LCDC_DMA_AUTO_RELOAD_SELECTED_BIT        (BIT30 | BIT31)
#define LCDC_DMA_LLP_SELECTED_BIT                (BIT27 | BIT28)

typedef struct
{
    uint8_t  LCDC_DMA_ChannelNum;               /*!< Specifies channel number for GDMA. */
    uint8_t  LCDC_DMA_DIR;              /*!< Specifies transfer direction. */
    uint32_t LCDC_DMA_BufferSize;               /*!< Specifies the buffer size(<=65535).
                                                 The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                                 or DMA_MemoryDataSize members depending in the transfer direction. */
    LCDC_DMA_SRC_INC_T LCDC_DMA_SourceInc;          /*!< Specifies whether the source address
                                                 register is incremented or not. */
    LCDC_DMA_DEST_INC_T LCDC_DMA_DestinationInc;    /*!< Specifies whether the destination address
                                                 register is incremented or not.*/
    LCDC_DMA_DATASIZE_T LCDC_DMA_SourceDataSize;    /*!< Specifies the source data width. */
    LCDC_DMA_DATASIZE_T LCDC_DMA_DestinationDataSize; /*!< Specifies the destination data width. */
    LCDC_DMA_MSIZE_T LCDC_DMA_SourceMsize;          /*!< Specifies items number to be transferred. */
    LCDC_DMA_MSIZE_T LCDC_DMA_DestinationMsize;     /*!< Specifies items number to be transferred. */
    uint32_t LCDC_DMA_SourceAddr;               /*!< Specifies the source base address for GDMA Channelx. */
    uint32_t LCDC_DMA_DestinationAddr;          /*!< Specifies the destination base address for GDMA Channelx. */
    uint32_t LCDC_DMA_ChannelPriority;          /*!< Specifies the software priority for the GDMA Channelx.
                                                 This parameter can be a value of 0~7 for GDMA1 and 0~15 for GDMA2. */
    uint32_t LCDC_DMA_Multi_Block_Mode;         /*!< Specifies the multi block transfer mode.
                                                 This parameter can be a value of \ref GDMA_Multiblock_Mode. */
    uint32_t LCDC_DMA_Multi_Block_Struct;       /*!< Pointer to the first struct of LLI. */
    uint8_t  LCDC_DMA_Multi_Block_En;           /*!< Enable or disable multi-block function. */
    uint8_t  LCDC_DMA_SourceHandshake;          /*!< Specifies the handshake index in source.
                                                 This parameter can be a value of \ref GDMA_Handshake_Type. */
    uint8_t  LCDC_DMA_DestHandshake;            /*!< Specifies the handshake index in Destination.
                                                 This parameter can be a value of \ref GDMA_Handshake_Type. */
    uint8_t  LCDC_DMA_Gather_En;                /*!< Enable or disable Gather function. NOTE:4 bytes ALIGN.*/
    uint32_t LCDC_DMA_GatherCount;              /*!< Specifies the GatherCount.NOTE:4 bytes ALIGN.*/
    uint32_t LCDC_DMA_GatherInterval;           /*!< Specifies the GatherInterval. */
    uint8_t  LCDC_DMA_Scatter_En;               /*!< Enable or disable Scatter function. */
    uint32_t LCDC_DMA_ScatterCount;             /*!< Specifies the ScatterCount. */
    uint32_t LCDC_DMA_ScatterInterval;          /*!< Specifies the ScatterInterval. */
    uint32_t LCDC_DMA_GatherCircularStreamingNum;  /*!< Specifies the GatherCircularStreamingNum. */
    uint32_t LCDC_DMA_ScatterCircularStreamingNum; /*!< Specifies the ScatterCircularStreamingNum. */
    uint8_t  LCDC_DMA_Secure_En;                /*!< Enable or disable Secure function. */
} LCDC_DMA_InitTypeDef;

void LCDC_DMA_Init(LCDC_DMA_ChannelTypeDef *LCDC_DMA_Channelx,
                   LCDC_DMA_InitTypeDef *LCDC_DMA_InitStruct);

void LCDC_DMA_StructInit(LCDC_DMA_InitTypeDef *LCDC_DMA_InitStruct);

void LCDC_DMAChannelCmd(uint8_t LCDC_DMA_Channel_Num, FunctionalState NewState);



#ifdef __cplusplus
}
#endif

#endif /* RTL_LCDC_DMA_H */


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

