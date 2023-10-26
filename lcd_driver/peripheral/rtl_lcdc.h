/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl_lcdc.h
* @brief    This file provides all the LCDC HANDLER firmware functions.
* @details
* @author   howie wang
* @date     2021-08-17
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL_LCDC_H
#define __RTL_LCDC_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl_lcdc_dma.h"
#include "rtl_lcdc_handler_reg.h"
#include "rtl_lcdc_dma_ll_reg.h"

typedef enum
{
    CKO1_PLL1_VCORE4 = 0,
    CKO1_PLL4_VCORE4 = 1
} CLK_DISPLAY_SRC_MUX0;

typedef enum
{
    CKO1_PLL2_VCORE4 = 0,
    FROM_CLK_DISPLAY_SRC_MUX0 = 1
} CLK_DISPLAY_SRC_MUX1;

typedef enum
{
    NO_CLOCK = 0,
    FRO_CLK_DISPLAY_SRC_MUX1 = 1
} CLK_DISPLAY_MUX_CG_EN;

typedef enum
{
    LCDC_DIV_DISABLE = 0,
    LCDC_DIV_ENABLE = 1
} LCDC_DIV_EN;

typedef enum
{
    LCDC_DIV_1_DIV = 0,
    LCDC_DIV_2_DIV = 1,
    LCDC_DIV_4_DIV = 2,
    LCDC_DIV_8_DIV = 3,
    LCDC_DIV_16_DIV = 4,
    LCDC_DIV_32_DIV = 5,
    LCDC_DIV_40_DIV = 6,
    LCDC_DIV_64_DIV = 7,
} LCDC_DIV_SEL;


typedef struct
{
    uint32_t LCDC_Interface;
    uint32_t LCDC_RamlessEn;
    uint32_t LCDC_GroupSel;
    uint32_t LCDC_PixelInputFarmat;
    uint32_t LCDC_PixelOutpuFarmat;
    uint32_t LCDC_PixelBitSwap;
    uint32_t LCDC_TeEn;
    uint32_t LCDC_TePolarity;
    uint32_t LCDC_TeInputMux;
    uint32_t LCDC_OperateMode;
    uint32_t LCDC_OperateDirection;
    uint32_t LCDC_DmaThreshold;
    uint32_t LCDC_DmaIntMask;
    uint32_t LCDC_TxLen;
    uint32_t LCDC_InfiniteModeEn;
} LCDC_InitTypeDef;

typedef struct
{
    __IO uint32_t GRP1_SAR;
    __IO uint32_t GRP1_DAR;
    __IO uint32_t GRP1_LLP;
    __IO uint32_t GRP1_CTL0;
    __IO uint32_t GRP1_CTL1;
    __I  uint32_t RSVD0[3];
    __IO uint32_t GRP1_SAR_OFFSET;
    __IO uint32_t GRP1_DAR_OFFSET;
    __IO uint32_t GRP2_SAR_OFFSET;
    __IO uint32_t GRP2_DAR_OFFSET;
    __IO uint32_t GRP2_SAR;
    __IO uint32_t GRP2_DAR;
    __IO uint32_t GRP2_LLP;
    __IO uint32_t GRP2_CTL0;
    __IO uint32_t GRP2_CTL1;
    __IO uint32_t DMA_CFG;
    __I  uint32_t RSVD1[2];
    __IO uint32_t GRP1_SAR_DMA;
    __IO uint32_t GRP1_DAR_DMA;
    __IO uint32_t GRP1_LLP_DMA;
    __IO uint32_t GRP1_CTL0_DMA;
    __IO uint32_t GRP1_CTL1_DMA;
    __I  uint32_t RSVD2[3];
    __IO uint32_t GRP2_SAR_DMA;
    __IO uint32_t GRP2_DAR_DMA;
    __IO uint32_t GRP2_LLP_DMA;
    __IO uint32_t GRP2_CTL0_DMA;
    __IO uint32_t GRP2_CTL1_DMA;
    __I  uint32_t RSVD3[2];
    __IO uint32_t GRP1_SAR_FOR_INFINITE_MODE;
    __IO uint32_t GRP2_SAR_FOR_INFINITE_MODE;
} LCDC_DMA_LinkListTypeDef;

typedef struct
{
    uint32_t g1_source_addr;
    uint32_t g2_source_addr;
    uint32_t g1_destination_addr;
    uint32_t g2_destination_addr;
    uint16_t g1_buffersize;
    uint16_t g2_buffersize;
    uint32_t g1_sar_offset;
    uint32_t g2_sar_offset;
    uint32_t g1_dar_offset;
    uint32_t g2_dar_offset;
    uint32_t g1_LLP;
    uint32_t g2_LLP;
} LCDC_DMALLI_InitTypeDef;



/*==============================================================*


*===============================================================*/
#undef  DISPLAY_CTRL_REG_BASE

#if defined RTL87x2G

#define DISPLAY_CTRL_REG_BASE              0x40023000UL

#elif defined RTL8763EP

#define DISPLAY_CTRL_REG_BASE                0x40017000UL

#endif
#define LCDC_DMA_CHANNEL_REG_BASE           (DISPLAY_CTRL_REG_BASE + 0)
#define LCDC_DMA_Channel0_BASE              (DISPLAY_CTRL_REG_BASE + 0x000)
#define LCDC_DMA_REG_BASE                   (DISPLAY_CTRL_REG_BASE + 0x2c0)
#define DBIC_REG_BASE                       (DISPLAY_CTRL_REG_BASE + 0x400)
#define LCDC_DMA_LINKLIST_REG_BASE          (DISPLAY_CTRL_REG_BASE + 0x600)
#define EDPI_REG_BASE                       (DISPLAY_CTRL_REG_BASE + 0x700)
#define DBIB_REG_BASE                       (DISPLAY_CTRL_REG_BASE + 0x800)
#define LCDC_HANDLER_REG_BASE               (DISPLAY_CTRL_REG_BASE + 0x900)
#define RAMLESS_QSPI_REG_BASE               (DISPLAY_CTRL_REG_BASE + 0xA00)//0x40023A00UL

#define LCDC_DMA_Channel0                   ((LCDC_DMA_ChannelTypeDef *) LCDC_DMA_Channel0_BASE)
#define LCDC_DMA_BASE                       ((LCDC_DMA_TypeDef *) LCDC_DMA_REG_BASE)
#define DBIC                                ((LCDC_DBIC_TypeDef *)DBIC_REG_BASE)
#define LCDC_DMA_LINKLIST                   ((LCDC_DMA_LinkList_TypeDef *)LCDC_DMA_LINKLIST_REG_BASE)
#define EDPI                                ((LCDC_EDPI_TypeDef *)EDPI_REG_BASE)
#define DBIB                                ((LCDC_DBIB_TypeDef *)DBIB_REG_BASE)
#define LCDC_HANDLER                        ((LCDC_Handler_TypeDef *)LCDC_HANDLER_REG_BASE)
#define RAMLESS_QSPI                        ((LCDC_Ramless_QSPI_TypeDef *)RAMLESS_QSPI_REG_BASE)

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    LCDC_HANDLER_Exported_Constants Macro Definitions
 *
 * \ingroup     LCDC HANDLER
 */
#define LCDC_DMA_ENABLE             BIT0

#define LCDC_TX_CNT

#define LCDC_DMA_RX_FIFO_OFFSET             ((uint32_t)(0xFFFF << 16))
#define LCDC_DMA_TX_FIFO_OFFSET             ((uint32_t)(0xFFFF))

/**
 * \defgroup    LCDC_Interface LCDC interface
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_IF_eDPI,
    LCDC_IF_DBIB,
    LCDC_IF_DBIC,
    LCDC_IF_DPI
} LCDC_IF_T;

#define IS_LCDC_IF_SEL(IF) (((IF) == LCDC_IF_eDPI) || ((IF) == LCDC_IF_DBIB) || \
                            ((IF) == LCDC_IF_DBIC) || ((IF) == LCDC_IF_DPI))

/**
 * \defgroup    LCDC_PixelInputFarmat lcdc pixel input format
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_INPUT_ARGB8888,
    LCDC_INPUT_BGR565,
    LCDC_INPUT_RGB565,
    LCDC_INPUT_RGB888,
    LCDC_INPUT_ABGR8888,
} LCDC_INPUT_FORMAT_T;

#define IS_LCDC_INPUT_FORMAT(FORMAT) (((FORMAT) == LCDC_INPUT_ARGB8888) || ((FORMAT) == LCDC_INPUT_BGR565) || \
                                      ((FORMAT) == LCDC_INPUT_RGB565) || ((FORMAT) == LCDC_INPUT_RGB888) || \
                                      ((FORMAT) == LCDC_INPUT_ABGR8888))

/**
 * \defgroup    LCDC_PixelOutputFarmat lcdc pixel output format
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_OUTPUT_RGB888,
    LCDC_OUTPUT_RGB565,
    LCDC_OUTPUT_BGR888,
    LCDC_OUTPUT_BGR565,
} LCDC_OUTPUT_FORMAT_T;

#define IS_LCDC_OUTPUT_FORMAT(FORMAT) ((FORMAT) == LCDC_OUTPUT_RGB888) || ((FORMAT) == LCDC_OUTPUT_RGB565) || ((FORMAT) == LCDC_OUTPUT_BGR565)

/**
 * \defgroup    LCDC_PixelBitSwap lcdc pixel bit swap
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_SWAP_BYPASS,
    LCDC_SWAP_8BIT,
    LCDC_SWAP_16BIT,
    LCDC_SWAP_8BIT_16BIT,
} LCDC_SWAP_BIT_T;

#define IS_LCDC_SWAP(SWAP) (((SWAP) == LCDC_SWAP_BYPASS) || ((SWAP) == LCDC_SWAP_8BIT) || \
                            ((SWAP) == LCDC_SWAP_16BIT) || ((SWAP) == LCDC_SWAP_8BIT_16BIT))
/**
 * \defgroup    LCDC_TearSignalValid lcdc tear signal valid
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_TE_SIGNAL_VALID_LOW,
    LCDC_TE_SIGNAL_VALID_HIGH,
} LCDC_TE_SIGNAL_VALID_T;

#define IS_LCDC_TE_SIGNAL_VALID(VALID) ((VALID) == LCDC_TE_SIGNAL_VALID_HIGH) || ((VALID) == LCDC_TE_SIGNAL_VALID_LOW)

/**
 * \defgroup    LCDC_TePolarity lcdc te polarity
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_TE_EDGE_RISING,
    LCDC_TE_EDGE_FALLING
} LCDC_TE_EDGE_T;

#define IS_LCDC_TE_POLARITY(POLARITY) ((POLARITY) == LCDC_TE_EDGE_RISING) || ((POLARITY) == LCDC_TE_EDGE_FALLING)

/**
 * \defgroup    LCDC_TePolarity lcdc te polarity
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_TE_LCD_INPUT,
    LCDC_TE_DSIHOST_INPUT
} LCDC_INPUT_MUX_T;

#define IS_LCDC_TE_INPUTMUX(INPUT) ((INPUT) == LCDC_TE_LCD_INPUT) || ((INPUT) == LCDC_TE_DSIHOST_INPUT)


/**
 * \defgroup    LCDC_OperateMode lcdc mode
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_AUTO_MODE,
    LCDC_MANUAL_MODE,
} LCDC_MODE_T;

#define IS_LCDC_MODE(MODE) ((MODE) == LCDC_AUTO_MODE) || ((MODE) == LCDC_MANUAL_MODE)

/**
 * \defgroup    LCDC_OperateDirection lcdc direction
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_TX_MODE,
    LCDC_RX_MODE,
} LCDC_DIRECTION_T;

#define IS_LCDC_DIR(DIR) ((DIR) == LCDC_TX_MODE) || ((DIR) == LCDC_RX_MODE)

/**
 * \defgroup    LCDC_Interrupts_Mask_Definition lcdc Interrupts Mask Definition
 *
 * \ingroup     LCDC_Exported_Constants
 */
#define LCDC_INT_MASK_MULTI_BLOCK_START                 BIT31
#define LCDC_INT_MASK_WAVEFORM_FINISH                   BIT30
#define LCDC_INT_MASK_TEAR_TTRIGGER                     BIT11
#define LCDC_INT_MASK_RX_AUTO_DONE                      BIT10
#define LCDC_INT_MASK_RX_FIFO_OVERFLOW                  BIT9
#define LCDC_INT_MASK_TX_AUTO_DONE                      BIT8
#define LCDC_INT_MASK_TX_FIFO_EMPTY                     BIT7
#define LCDC_INT_MASK_TX_FIFO_OVERFLOW                  BIT6
#define LCDC_INT_MASK_TX_FIFO_THRESHOLDR                BIT5
#define LCDC_INT_MASK_TX_TRANSFER_ERROR                 BIT4

#define IS_LCDC_INT_MSK_CONFIG(MASK)   (((MASK) == LCDC_INT_MASK_TEAR_TTRIGGER)            || \
                                        ((MASK) == LCDC_INT_MASK_RX_AUTO_DONE)     || \
                                        ((MASK) == LCDC_INT_MASK_RX_FIFO_OVERFLOW)            || \
                                        ((MASK) == LCDC_INT_MASK_TX_AUTO_DONE)     || \
                                        ((MASK) == LCDC_INT_MASK_TX_FIFO_EMPTY)         || \
                                        ((MASK) == LCDC_INT_MASK_TX_FIFO_OVERFLOW)            || \
                                        ((MASK) == LCDC_INT_MASK_TX_FIFO_THRESHOLDR)        || \
                                        ((MASK) == LCDC_INT_MASK_TX_TRANSFER_ERROR))

/**
 * \defgroup    LCDC_Interrupts_Definition lcdc Interrupts status
 *
 * \ingroup     LCDC_Exported_Constants
 */
#define LCDC_INT_STATUS_MULTIBLOCK_LASTBLOCK_START              BIT31
#define LCDC_INT_STATUS_WAVEFORM_FINISH                         BIT30
#define LCDC_INT_STATUS_TEAR_TRIGGER                            BIT20
#define LCDC_INT_STATUS_RX_FIFO_EMPTY                           BIT19
#define LCDC_INT_STATUS_RX_FIFO_FULL                            BIT18
#define LCDC_INT_RX_AUTO_DONE                                   BIT17
#define LCDC_INT_RX_FIFO_OVERFLOW                               BIT16
#define LCDC_INT_TX_AUTO_DONE                                   BIT6
#define LCDC_INT_STATUS_TX_FIFO_EMPTY                           BIT5
#define LCDC_INT_STATUS_TX_FIFO_FULL                            BIT4

#define IS_LCDC_INT_STATUS(STATUS)       (((STATUS) == LCDC_INT_STATUS_MULTIBLOCK_LASTBLOCK_START)         || \
                                          ((STATUS) == LCDC_INT_STATUS_WAVEFORM_FINISH)  || \
                                          ((STATUS) == LCDC_INT_STATUS_TEAR_TRIGGER)         || \
                                          ((STATUS) == LCDC_INT_STATUS_RX_FIFO_EMPTY)  || \
                                          ((STATUS) == LCDC_INT_STATUS_RX_FIFO_FULL)      || \
                                          ((STATUS) == LCDC_INT_RX_AUTO_DONE)         || \
                                          ((STATUS) == LCDC_INT_RX_FIFO_OVERFLOW)           || \
                                          ((STATUS) == LCDC_INT_TX_AUTO_DONE)          || \
                                          ((STATUS) == LCDC_INT_STATUS_TX_FIFO_EMPTY)  || \
                                          ((STATUS) == LCDC_INT_STATUS_TX_FIFO_FULL))

/**
 * \defgroup    LCDC_Interrupts_Definition lcdc Interrupts flag
 *
 * \ingroup     LCDC_Exported_Constants
 */
#define LCDC_INT_FLAG_STATUS_TX_FIFO_EMPTY                      BIT3
#define LCDC_INT_FLAG_STATUS_TX_FIFO_OVERFLOW                   BIT2
#define LCDC_INT_FLAG_STATUS_TX_FIFO_THESHOLD                   BIT1
#define LCDC_INT_FLAG_STATUS_TX_FIFO_TRANSFER_ERROR             BIT0

#define IS_LCDC_FLAG_STATUS(FLAG)       (((FLAG) == LCDC_INT_FLAG_STATUS_TX_FIFO_EMPTY)         || \
                                         ((FLAG) == LCDC_INT_FLAG_STATUS_TX_FIFO_OVERFLOW)  || \
                                         ((FLAG) == LCDC_INT_FLAG_STATUS_TX_FIFO_THESHOLD)         || \
                                         ((FLAG) == LCDC_INT_FLAG_STATUS_TX_FIFO_TRANSFER_ERROR))

/**
 * \defgroup    LCDC_Interrupts_Clear_Status lcdc Interrupts Clear
 *
 * \ingroup     LCDC_Exported_Constants
 */
#define LCDC_CLR_MULTIBLOCK_LASTBLOCK_START                     BIT31
#define LCDC_CLR_WAVEFORM_FINISH                                BIT30
#define LCDC_CLR_TEAR_TRIGGER                                   BIT10
#define LCDC_CLR_RX_AUTO_DONE                                   BIT9
#define LCDC_CLR_RX_FIFO_OVERFLOW                               BIT8
#define LCDC_CLR_RX_OUTPUT_CNT                                  BIT7
#define LCDC_CLR_TX_AUTO_DONE                                   BIT6
#define LCDC_CLR_TX_FIFO_EMPTY                                  BIT5
#define LCDC_CLR_TX_FIFO_OVERFLOW                               BIT4
#define LCDC_CLR_TX_FIFO_THESHOLD                               BIT3
#define LCDC_CLR_TX_OUTPUT_CNT                                  BIT2
#define LCDC_CLR_TX_FIFO_TRANSFER_ERROR                         BIT1
#define LCDC_CLR_FIFO                                           BIT0

#define IS_LCDC_INT_CLEAR(CLR)           (((CLR) == LCDC_CLR_MULTIBLOCK_LASTBLOCK_START) || \
                                          ((CLR) == LCDC_CLR_WAVEFORM_FINISH) || \
                                          ((CLR) == LCDC_CLR_TEAR_TRIGGER) || \
                                          ((CLR) == LCDC_CLR_RX_AUTO_DONE) || \
                                          ((CLR) == LCDC_CLR_RX_FIFO_OVERFLOW) || \
                                          ((CLR) == LCDC_CLR_RX_OUTPUT_CNT) || \
                                          ((CLR) == LCDC_CLR_TX_AUTO_DONE) || \
                                          ((CLR) == LCDC_CLR_TX_FIFO_EMPTY) || \
                                          ((CLR) == LCDC_CLR_TX_FIFO_OVERFLOW) || \
                                          ((CLR) == LCDC_CLR_TX_FIFO_THESHOLD) || \
                                          ((CLR) == LCDC_CLR_TX_OUTPUT_CNT) || \
                                          ((CLR) == LCDC_CLR_TX_FIFO_TRANSFER_ERROR) || \
                                          ((CLR) == LCDC_CLR_FIFO))

/**
 * \defgroup    SPIC_RX_FIFO_READ_SIZE
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    SPIC_RX_FIFO_READ_SIZE_BYTE,
    SPIC_RX_FIFO_READ_SIZE_HALFWORD,
    SPIC_RX_FIFO_READ_SIZE_WORD
} LCDC_SPIC_READ_SIZE_T;

#define IS_SPIC_RX_FIFO_READ_SIZE(SIZE) (((SIZE) == SPIC_RX_FIFO_READ_SIZE_BYTE) || \
                                         ((SIZE) == SPIC_RX_FIFO_READ_SIZE_HALFWORD) || \
                                         ((SIZE) == SPIC_RX_FIFO_READ_SIZE_WORD))

/**
 * \defgroup    DMA_Multiblock_Mode DMA Multi-block Mode
 * \{
 * \ingroup     DMA_Exported_Constants
 */
#define AUTO_RELOAD_WITH_CONTIGUOUS_SAR     (BIT30)
#define AUTO_RELOAD_WITH_CONTIGUOUS_DAR     (BIT31)
#define AUTO_RELOAD_TRANSFER                (BIT30 | BIT31)
#define LLI_WITH_CONTIGUOUS_SAR             (BIT27)
#define LLI_WITH_AUTO_RELOAD_SAR            (BIT27 | BIT30)
#define LLI_WITH_CONTIGUOUS_DAR             (BIT28)
#define LLI_WITH_AUTO_RELOAD_DAR            (BIT28 | BIT31)
#define LLI_TRANSFER                        (BIT27 | BIT28)
/** \} */

#define IS_DMA_MULTIBLOCKMODE(MODE) (((MODE) == AUTO_RELOAD_WITH_CONTIGUOUS_SAR) || ((MODE) == AUTO_RELOAD_WITH_CONTIGUOUS_DAR)\
                                     ||((MODE) == AUTO_RELOAD_TRANSFER) || ((MODE) == LLI_WITH_CONTIGUOUS_SAR)\
                                     ||((MODE) == LLI_WITH_AUTO_RELOAD_SAR) || ((MODE) == LLI_WITH_CONTIGUOUS_DAR)\
                                     ||((MODE) == LLI_WITH_AUTO_RELOAD_DAR) || ((MODE) == LLI_TRANSFER))

/**
 * \def DMA_Multiblock_Select_Bit Multi-Block Select Bit
 *
 */
#define LCDC_DMA_LINK_G1_WP         BIT31
#define LCDC_DMA_LINK_G2_WP         BIT30


/**
 * \defgroup    LCDC_AXIMUXmode lcdc direction
 *
 * \ingroup     LCDC_Exported_Constants
 */
typedef enum
{
    LCDC_FW_MODE,
    LCDC_HW_MODE,
} LCDC_AXI_MUX_MODE_T;

/** \} */ /* End of group LCDC_HANDLER_Exported_Constants */

__STATIC_INLINE void LCDC_LCD_SET_RST(bool reset)
{
    LCDC_HANDLER_INTERFACE_SELECT_t handler_reg_0x00 = {.d32 = LCDC_HANDLER->INTERFACE_SELECT};
    if (reset)
    {
        handler_reg_0x00.b.reset_lcd_display_module = 0;
    }
    else
    {
        handler_reg_0x00.b.reset_lcd_display_module = 1;
    }
    LCDC_HANDLER->INTERFACE_SELECT = handler_reg_0x00.d32;
}

__STATIC_INLINE void LCDC_Cmd(FunctionalState NewState)
{
    LCDC_HANDLER_INTERFACE_SELECT_t handler_reg_0x00 = {.d32 = LCDC_HANDLER->INTERFACE_SELECT};
    if (NewState)
    {
        handler_reg_0x00.b.display_controller_enable = 1;
    }
    else
    {
        handler_reg_0x00.b.display_controller_enable = 0;
    }
    LCDC_HANDLER->INTERFACE_SELECT = handler_reg_0x00.d32;
}

__STATIC_INLINE void LCDC_SwitchIF(uint32_t InterFace)
{
    assert_param(IS_LCDC_IF_SEL(InterFace));

    LCDC_HANDLER_INTERFACE_SELECT_t handler_reg_0x00 = {.d32 = LCDC_HANDLER->INTERFACE_SELECT};
    handler_reg_0x00.b.interface_select = InterFace;
    LCDC_HANDLER->INTERFACE_SELECT = handler_reg_0x00.d32;
}

__STATIC_INLINE void LCDC_SwitchMode(uint32_t mode)
{
    assert_param(IS_LCDC_MODE(mode));
    LCDC_HANDLER_OPERATE_CTR_t handler_reg_0x14 = {.d32 = LCDC_HANDLER->OPERATE_CTR};
    handler_reg_0x14.b.access_mode = mode;
    LCDC_HANDLER->OPERATE_CTR = handler_reg_0x14.d32;
}

__STATIC_INLINE void LCDC_SwitchDirect(uint32_t dir)
{
    assert_param(IS_LCDC_DIR(dir));
    LCDC_HANDLER_OPERATE_CTR_t handler_reg_0x14 = {.d32 = LCDC_HANDLER->OPERATE_CTR};
    handler_reg_0x14.b.data_rw = dir;
    LCDC_HANDLER->OPERATE_CTR = handler_reg_0x14.d32;
}

__STATIC_INLINE void LCDC_DmaCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    LCDC_HANDLER_DMA_FIFO_CTRL_t handler_reg_0x18 = {.d32 = LCDC_HANDLER->DMA_FIFO_CTRL};
    if (NewState == ENABLE)
    {
        handler_reg_0x18.b.dma_enable = 1;
    }
    else
    {
        handler_reg_0x18.b.dma_enable = 0;
    }
    LCDC_HANDLER->DMA_FIFO_CTRL = handler_reg_0x18.d32;
}

__STATIC_INLINE void LCDC_TeCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    LCDC_HANDLER_TEAR_CTR_t handler_reg_0x10 = {.d32 = LCDC_HANDLER->TEAR_CTR};
    if (NewState == ENABLE)
    {
        handler_reg_0x10.b.tear_auto_turn_on_autowritestart = 1;
    }
    else
    {
        handler_reg_0x10.b.tear_auto_turn_on_autowritestart = 0;
    }
    LCDC_HANDLER->TEAR_CTR = handler_reg_0x10.d32;
}

__STATIC_INLINE void LCDC_AutoWriteCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    LCDC_HANDLER_OPERATE_CTR_t handler_reg_0x14 = {.d32 = LCDC_HANDLER->OPERATE_CTR};
    if (NewState == ENABLE)
    {
        handler_reg_0x14.b.auto_write_start = 1;
    }
    else
    {
        handler_reg_0x14.b.auto_write_start = 0;
    }
    LCDC_HANDLER->OPERATE_CTR = handler_reg_0x14.d32;
}

__STATIC_INLINE void LCDC_AutoReadCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    LCDC_HANDLER_OPERATE_CTR_t handler_reg_0x14 = {.d32 = LCDC_HANDLER->OPERATE_CTR};
    if (NewState == ENABLE)
    {
        handler_reg_0x14.b.auto_read_start = 1;
    }
    else
    {
        handler_reg_0x14.b.auto_read_start = 0;
    }
    LCDC_HANDLER->OPERATE_CTR = handler_reg_0x14.d32;
}

__STATIC_INLINE void LCDC_WriteFIFO(uint32_t data)
{
    LCDC_HANDLER->DMA_FIFO = data;
}

__STATIC_INLINE uint32_t LCDC_ReadFIFO(void)
{
    return LCDC_HANDLER->DMA_FIFO;
}

__STATIC_INLINE void LCDC_ClearDmaFifo(void)
{
    LCDC_HANDLER_DMA_FIFO_ICR_t handler_reg_0x24 = {.d32 = LCDC_HANDLER->DMA_FIFO_ICR};
    handler_reg_0x24.b.fifo_clr = 1;
    LCDC_HANDLER->DMA_FIFO_ICR = handler_reg_0x24.d32;
}

__STATIC_INLINE void LCDC_ClearINTPendingBit(uint32_t LCDC_CLEAR_INT)
{
    /* Check the parameters */
    assert_param(IS_LCDC_INT_CLEAR(LCDC_CLEAR_INT));

    //LCDC_HANDLER->DMA_FIFO_ICR &= LCDC_CLEAR_INT;
    LCDC_HANDLER->DMA_FIFO_ICR = LCDC_CLEAR_INT;
}

__STATIC_INLINE uint32_t LCDC_SetTxPixelLen(uint32_t len)
{
    return LCDC_HANDLER->TX_LEN = len;
}

__STATIC_INLINE uint32_t LCDC_GetTxPixelCnt(void)
{
    return LCDC_HANDLER->TX_CNT;
}

__STATIC_INLINE void LCDC_ClearTxPixelCnt(void)
{
    LCDC_HANDLER_DMA_FIFO_ICR_t handler_reg_0x24 = {.d32 = LCDC_HANDLER->DMA_FIFO_ICR};
    handler_reg_0x24.b.tx_output_cnt_clr = 1;
    LCDC_HANDLER->DMA_FIFO_ICR = handler_reg_0x24.d32;
}

__STATIC_INLINE uint32_t LCDC_SetRxByteLen(uint32_t len)
{
    return LCDC_HANDLER->RX_LEN = len;
}

__STATIC_INLINE uint32_t LCDC_GetRxByteCnt(void)
{
    return LCDC_HANDLER->RX_LEN;
}

__STATIC_INLINE uint32_t LCDC_GetRxCounter(void)
{
    return LCDC_HANDLER->RX_CNT;
}

__STATIC_INLINE void LCDC_ClearRxCounter(void)
{
    LCDC_HANDLER_DMA_FIFO_ICR_t handler_reg_0x24 = {.d32 = LCDC_HANDLER->DMA_FIFO_ICR};
    handler_reg_0x24.b.rx_output_cnt_clr = 1;
    LCDC_HANDLER->DMA_FIFO_ICR = handler_reg_0x24.d32;
}

__STATIC_INLINE void LCDC_AXIMUXMode(uint32_t mode)
{
    assert_param(IS_LCDC_AXI_MUX_MODE(mode));
    LCDC_HANDLER_AXI_MUX_MODE_t handler_reg_0x40 = {.d32 = LCDC_HANDLER->AXI_MUX_MODE};
    handler_reg_0x40.b.axi_mux_mode = mode;
    LCDC_HANDLER->AXI_MUX_MODE = handler_reg_0x40.d32;
}

__STATIC_INLINE void LCDC_SPICCmd(uint8_t cmd)
{
    LCDC_HANDLER_SPIC_CMD_t handler_reg_0x44 = {.d32 = LCDC_HANDLER->SPIC_CMD};
    handler_reg_0x44.b.spic_cmd = cmd;
    LCDC_HANDLER->SPIC_CMD = handler_reg_0x44.d32;
}

__STATIC_INLINE void LCDC_SPICAddr(uint32_t addr)
{
    LCDC_HANDLER_SPIC_ADDR_t handler_reg_0x44 = {.d32 = 0};
    handler_reg_0x44.b.spic_addr = addr;
    LCDC_HANDLER->SPIC_ADDR = handler_reg_0x44.d32;
}

__STATIC_INLINE uint32_t LCDC_SPICRXFIFOReadSize(void)
{
    LCDC_HANDLER_SPIC_RX_FIFO_READ_SIZE_t handler_reg_0x68 = {.d32 = LCDC_HANDLER->SPIC_RX_FIFO_READ_SIZE};
    return handler_reg_0x68.b.spic_rx_fifo_read_size;
}

__STATIC_INLINE uint32_t LCDC_DMA_LOAD_CNT(void)
{
    LCDC_HANDLER_DMA_MULTI_BLOCK_CTRL_t handler_reg_0x4c = {.d32 = LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL};
    return (handler_reg_0x4c.b.dma_load_cnt);
}

__STATIC_INLINE void LCDC_DMA_MultiBlockCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    LCDC_HANDLER_DMA_MULTI_BLOCK_CTRL_t handler_reg_0x4c = {.d32 = LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL};
    if (NewState == ENABLE)
    {
        handler_reg_0x4c.b.dma_multi_block_en = 1;
    }
    else
    {
        handler_reg_0x4c.b.dma_multi_block_en = 0;
    }
    LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL = handler_reg_0x4c.d32;
}

__STATIC_INLINE void LCDC_DMA_LinkListCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    LCDC_HANDLER_DMA_MULTI_BLOCK_CTRL_t handler_reg_0x4c = {.d32 = LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL};
    if (NewState == ENABLE)
    {
        handler_reg_0x4c.b.link_list_enable = 1;
    }
    else
    {
        handler_reg_0x4c.b.link_list_enable = 0;
    }
    LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL = handler_reg_0x4c.d32;
}

__STATIC_INLINE void LCDC_DMA_LOAD_CNT_CLR(void)
{
    LCDC_HANDLER_DMA_MULTI_BLOCK_CTRL_t handler_reg_0x4c = {.d32 = LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL};
    handler_reg_0x4c.b.reg_dma_load_cnt_clr = 1;
    LCDC_HANDLER->DMA_MULTI_BLOCK_CTRL = handler_reg_0x4c.d32;
}

__STATIC_INLINE void LCDC_SET_GROUP1_BLOCKSIZE(uint32_t size)
{
    LCDC_HANDLER_DMA_MULTI_BLOCK_SIZE1_t handler_reg_0x50 = {.d32 = LCDC_HANDLER->DMA_MULTI_BLOCK_SIZE1};
    handler_reg_0x50.b.block_size_group_1 = size;
    LCDC_HANDLER->DMA_MULTI_BLOCK_SIZE1 = handler_reg_0x50.d32;
}

__STATIC_INLINE void LCDC_SET_GROUP2_BLOCKSIZE(uint32_t size)
{
    LCDC_HANDLER_DMA_MULTI_BLOCK_SIZE2_t handler_reg_0x54 = {.d32 = LCDC_HANDLER->DMA_MULTI_BLOCK_SIZE2};
    handler_reg_0x54.b.block_size_group_2 = size;
    LCDC_HANDLER->DMA_MULTI_BLOCK_SIZE2 = handler_reg_0x54.d32;
}

__STATIC_INLINE void LCDC_SET_INFINITE_ADDR(uint32_t addr1, uint32_t addr2)
{
    LCDC_DMA_LINKLIST->GRP1_SAR_FOR_INFINITE_MODE = addr1;
    LCDC_DMA_LINKLIST->GRP2_SAR_FOR_INFINITE_MODE = addr2;
}

__STATIC_INLINE void LCDC_DMA_SetSourceAddress(LCDC_DMA_ChannelTypeDef *LCDC_DMA_Channelx,
                                               uint32_t Address)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));

    LCDC_DMA_Channelx->LCDC_DMA_SARx = Address;
}

__STATIC_INLINE void LCDC_DMA_SetBufferSize(LCDC_DMA_ChannelTypeDef *LCDC_DMA_Channelx,
                                            uint32_t buffer_size)
{
    /* Check the parameters */
    assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));

    /* configure high 32 bit of CTL register */
    LCDC_DMA_Channelx->LCDC_DMA_CTL_HIGHx = buffer_size;
}

void LCDC_Init(LCDC_InitTypeDef *LCDC_Init);

void LCDC_WriteCmd(uint8_t cmd);

void LCDC_WriteData(uint8_t data);

void LCDC_DMA_LinkList_Init(LCDC_DMALLI_InitTypeDef *LCDC_DMA_LLIConfig, void *LCDC_DMA_Init);

void LCDC_Clock_Cfg(FunctionalState state);

#ifdef __cplusplus
}
#endif

#endif /*__RTL_LCDC_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

