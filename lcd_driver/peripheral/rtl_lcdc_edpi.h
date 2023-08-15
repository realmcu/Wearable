/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_dsi.h
* @brief    This file provides all the eDPI firmware functions.
* @details
* @author   boris yue
* @date     2021-09-01
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL_EDPI_H
#define __RTL_EDPI_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl_lcdc.h"
#include "rtl_lcdc_edpi_reg.h"


typedef struct
{
    uint32_t eDPI_ClockDiv;
    uint32_t eDPI_HoriSyncWidth;
    uint32_t eDPI_VeriSyncHeight;
    uint32_t eDPI_AccumulatedHBP;
    uint32_t eDPI_AccumulatedVBP;
    uint32_t eDPI_AccumulatedActiveW;
    uint32_t eDPI_AccumulatedActiveH;
    uint32_t eDPI_TotalWidth;
    uint32_t eDPI_TotalHeight;
    uint32_t eDPI_HoriSyncPolarity;
    uint32_t eDPI_VeriSyncPolarity;
    uint32_t eDPI_DataEnPolarity;
    uint32_t eDPI_LineIntMask;
    uint32_t eDPI_LineIntFlag;
    uint32_t eDPI_LineIntClr;
    uint32_t eDPI_ColorMap;
    uint32_t eDPI_OperateMode;
    uint32_t eDPI_LcdArc;
    uint32_t eDPI_ShutdnPolarity;
    uint32_t eDPI_ColorModePolarity;
    uint32_t eDPI_ShutdnEn;
    uint32_t eDPI_ColorModeEn;
    uint32_t eDPI_UpdateCfgEn;
    uint32_t eDPI_TearReq;
    uint32_t eDPI_Halt;
    uint32_t eDPI_CmdMaxLatency;
    //uint32_t eDPI_LineBufferPixelNum;
    uint32_t eDPI_LineBufferPixelThreshold;
} LCDC_eDPICfgTypeDef;


/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    EDPI_ClockDiv clock div
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_CLOCKDIV2 = 1,
    EDPI_CLOCKDIV3 = 2,
    EDPI_CLOCKDIV4 = 3,
    EDPI_CLOCKDIV5 = 4,
    EDPI_CLOCKDIV6 = 5,
    EDPI_CLOCKDIV7 = 6,
    EDPI_CLOCKDIV8 = 7,
} EDPI_CLOCKDIV_T;

#define IS_EDPI_CLOCKDIV(DIV)   (((DIV) == EDPI_CLOCKDIV2) || ((DIV) == EDPI_CLOCKDIV3) || \
                                 ((DIV) == EDPI_CLOCKDIV4) || ((DIV) == EDPI_CLOCKDIV5) || \
                                 ((DIV) == EDPI_CLOCKDIV6) || ((DIV) == EDPI_CLOCKDIV7) || \
                                 ((DIV) == EDPI_CLOCKDIV8))

#define EDPI_HSYNCS                     BIT3
#define EDPI_VSYNCS                     BIT2
#define EDPI_HDES                       BIT1
#define EDPI_VDES                       BIT0
/** \} */

#define IS_EDPI_SYNC_SIGNAL_STATUS(SIGNAL)       (((SIGNAL) == EDPI_HSYNCS)         || \
                                                  ((SIGNAL) == EDPI_VSYNCS)  || \
                                                  ((SIGNAL) == EDPI_HDES)         || \
                                                  ((SIGNAL) == EDPI_VDES))

/**
 * \defgroup    EDPI_COLOR_MAP
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_PIXELFORMAT_RGB888,
    EDPI_PIXELFORMAT_RGB565_1,
    EDPI_PIXELFORMAT_RGB565_2,
    EDPI_PIXELFORMAT_RGB565_3,
} EDPI_PIXELFORMAT_T;

#define IS_EDPI_PIXELFORMAT(FORMAT)   (((FORMAT) == EDPI_PIXELFORMAT_RGB888) || ((FORMAT) == EDPI_PIXELFORMAT_RGB565_1) || \
                                       ((FORMAT) == EDPI_PIXELFORMAT_RGB565_2) || ((FORMAT) == EDPI_PIXELFORMAT_RGB565_3))

/**
 * \defgroup    EDPI_OP_MODE
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_STANDARD_VIDEO_MODE,
    EDPI_ADAPTED_COMMAND_MODE,
} EDPI_MODE_T;

#define IS_EDPI_OP_MODE(MDOE)   ((MODE) == EDPI_STANDARD_VIDEO_MODE) || ((MODE) == EDPI_ADAPTED_COMMAND_MODE)

/**
 * \defgroup    EDPI_LCD_ARC
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_LCD_ARC_TYPE123,
    EDPI_LCD_ARC_TYPE4,
} EDPI_LCD_ARC_T;

#define IS_EDPI_LCD_ARC_TYPE(TYPE)   ((TYPE) == EDPI_LCD_ARC_TYPE123) || ((TYPE) == EDPI_LCD_ARC_TYPE4)

/**
 * \defgroup    EDPI_SD_EN_FCOLOR
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_SD_EN_FCOLOR_BLACK,
    EDPI_SD_EN_FCOLOR_WHITE,
} EDPI_SD_EN_FCOLOR_T;

#define IS_EDPI_SD_EN_FCOLOR(FCOLOR)   ((FCOLOR) == EDPI_SD_EN_FCOLOR_BLACK) || ((FCOLOR) == EDPI_SD_EN_FCOLOR_WHITE)

/**
 * \defgroup    EDPI_SD_DIS_FCOLOR
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_SD_DIS_FCOLOR_BLACK,
    EDPI_SD_DIS_FCOLOR_WHITE,
} EDPI_SD_DIS_FCOLOR_T;

#define IS_EDPI_SD_DIS_FCOLOR(FCOLOR)   ((FCOLOR) == EDPI_SD_DIS_FCOLOR_BLACK) || ((FCOLOR) == EDPI_SD_DIS_FCOLOR_WHITE)

/**
 * \defgroup    EDPI_SHUTDN_POLARITY  SD POLARITY
 *
 * \ingroup     EDPI_Exported_Constants
 */
typedef enum
{
    EDPI_AL,
    EDPI_AH,
} EDPI_ACTIVE_POL_T;

#define IS_EDPI_ACTIVE_POL(POLARITY)   ((POLARITY) == EDPI_AL) || ((POLARITY) == EDPI_AH)


typedef enum
{
    EDPI_CONSTANT_LOW,
    EDPI_CONSTANT_NONE1,
    EDPI_CONSTANT_NONE2,
    EDPI_CONSTANT_HIGH,
} EDPI_CONSTANT_T;


/** \} */ /* End of group EDPI_Exported_Constants */

__STATIC_INLINE void EDPI_MaskLineINTConfig(FunctionalState state)
{
    assert_param(IS_FUNCTIONAL_STATE(state));
    EDPI_INT_MASK_t edpi_reg_0x18 = {.d32 = EDPI->EDPI_INT_MASK};
    if (state == ENABLE)
    {
        edpi_reg_0x18.b.lim = 1;
    }
    else
    {
        edpi_reg_0x18.b.lim = 0;
    }
    EDPI->EDPI_INT_MASK = edpi_reg_0x18.d32;
}

__STATIC_INLINE void LCDC_ClearLineINTPendingBit(void)
{
    EDPI_INT_CLR_t edpi_reg_0x20 = {.d32 = EDPI->EDPI_INT_CLR};
    edpi_reg_0x20.b.clif = 0;
    EDPI->EDPI_INT_CLR = edpi_reg_0x20.d32;
}

__STATIC_INLINE uint16_t EDPI_GetLineINTPos(void)
{
    EDPI_LINE_INT_POS_t edpi_reg_0x24 = {.d32 = EDPI->EDPI_LINE_INT_POS};
    return edpi_reg_0x24.b.lipos;
}

__STATIC_INLINE uint16_t EDPI_GetXPos(void)
{
    EDPI_PIXEL_POS_t edpi_reg_0x28 = {.d32 = EDPI->EDPI_PIXEL_POS};
    return edpi_reg_0x28.b.cxpos;
}

__STATIC_INLINE uint16_t EDPI_GetYPos(void)
{
    EDPI_PIXEL_POS_t edpi_reg_0x28 = {.d32 = EDPI->EDPI_PIXEL_POS};
    return edpi_reg_0x28.b.cypos;
}

__STATIC_INLINE void EDPI_OPMODE_CONFIG(uint32_t mode)
{
    assert_param(IS_EDPI_OP_MODE(mode));
    EDPI_OP_MODE_t edpi_reg_0x34 = {.d32 = EDPI->EDPI_OP_MODE};
    edpi_reg_0x34.b.op_mode = mode;
    EDPI->EDPI_OP_MODE = edpi_reg_0x34.d32;
}

void EDPI_Init(LCDC_eDPICfgTypeDef *eDPICfg);

#ifdef __cplusplus
}
#endif

#endif /*__RTL_EDPI_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

