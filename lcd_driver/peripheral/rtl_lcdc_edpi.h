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

#ifndef __RTL876X_EDPI_H
#define __RTL876X_EDPI_H

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

typedef enum
{
    EDPI_HSYNCS = 0,
    EDPI_VSYNCS,
    EDPI_HDES,
    EDPI_VDES,
} EDPI_SIGNAL_t;

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    EDPI_ClockDiv clock div
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_CLOCKDIV2                             ((uint32_t)0x1)               //temp
#define EDPI_CLOCKDIV3                             ((uint32_t)0x2)
#define EDPI_CLOCKDIV4                             ((uint32_t)0x3)
#define EDPI_CLOCKDIV5                             ((uint32_t)0x4)
#define EDPI_CLOCKDIV6                             ((uint32_t)0x5)
#define EDPI_CLOCKDIV7                             ((uint32_t)0x6)
#define EDPI_CLOCKDIV8                             ((uint32_t)0x7)
/** \} */

#define IS_EDPI_CLOCKDIV(DIV)   (((DIV) == EDPI_CLOCKDIV2) || ((DIV) == EDPI_CLOCKDIV3) || \
                                 ((DIV) == EDPI_CLOCKDIV4) || ((DIV) == EDPI_CLOCKDIV5) || \
                                 ((DIV) == EDPI_CLOCKDIV6) || ((DIV) == EDPI_CLOCKDIV7) || \
                                 ((DIV) == EDPI_CLOCKDIV8))


/**
 * \defgroup    EDPI_HS_POLARITY  HS POLARITY
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_HSPOL_AL                (0)            /*!< Horizontal Synchronization is active low. */
#define EDPI_HSPOL_AH                (1)            /*!< Horizontal Synchronization is active high. */
/** \} */

#define IS_EDPI_HSPOL(POLARITY)   ((POLARITY) == EDPI_HSPOL_AL) || ((POLARITY) == EDPI_HSPOL_AH)

/**
 * \defgroup    EDPI_VS_POLARITY  VS POLARITY
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_VSPOL_AL                (0)            /*!< Vertical Synchronization is active low. */
#define EDPI_VSPOL_AH                (1)            /*!< Vertical Synchronization is active high. */
/** \} */

#define IS_EDPI_VSPOL(POLARITY)   ((POLARITY) == EDPI_VSPOL_AL) || ((POLARITY) == EDPI_VSPOL_AH)

/**
 * \defgroup    EDPI_DE_POLARITY  DE POLARITY
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_DEPOL_AL                (0)            /*!< Data Enable, is active low. */
#define EDPI_DEPOL_AH                (1)            /*!< Data Enable, is active high. */
/** \} */

#define IS_EDPI_DEPOL(POLARITY)   ((POLARITY) == EDPI_DEPOL_AL) || ((POLARITY) == EDPI_DEPOL_AH)

/**
 * \defgroup    EDPI_INT_MASK
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_LIM_UNMASK            (0)              /*!< pixel clock polarity is active low. */
#define EDPI_LIM_MASK              (1)              /*!< pixel clock is active high. */
/** \} */

#define IS_EDPI_LIM(LIM)   ((LIM) == EDPI_LIM_UNMASK) || ((LIM) == EDPI_LIM_MASK)

/**
 * \defgroup    EDPI_COLOR_MAP
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_PIXELFORMAT_RGB888            (0)
#define EDPI_PIXELFORMAT_RGB565_1          ((uint32_t)0x1)   /*!<RGB565_1(R[D21:D17]G[D13:D8]B[D5:D1])*/
#define EDPI_PIXELFORMAT_RGB565_2          ((uint32_t)0x2)   /*!<RGB565_2(R[D20:D16]G[D13:D8]B[D4:D0])*/
#define EDPI_PIXELFORMAT_RGB565_3          ((uint32_t)0x3)   /*!<RGB565_3(R[D15:D11]G[D10:D5]B[D4:D0])*/
/** \} */

#define IS_EDPI_PIXELFORMAT(FORMAT)   (((FORMAT) == EDPI_PIXELFORMAT_RGB888) || ((FORMAT) == EDPI_PIXELFORMAT_RGB565_1) || \
                                       ((FORMAT) == EDPI_PIXELFORMAT_RGB565_2) || ((FORMAT) == EDPI_PIXELFORMAT_RGB565_3))

/**
 * \defgroup    EDPI_OP_MODE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_STANDARD_VIDEO_MODE            (0)             /*!< Standard Video mode. */
#define EDPI_ADAPTED_COMMAND_MODE           (1)             /*!< Adapted Command mode. */
/** \} */

#define IS_EDPI_OP_MODE(MDOE)   ((MODE) == EDPI_STANDARD_VIDEO_MODE) || ((MODE) == EDPI_ADAPTED_COMMAND_MODE)

/**
 * \defgroup    EDPI_LCD_ARC
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_LCD_ARC_TYPE123            (0)             /*!< LCD Display Driver is of Type 1 or 2or3. */
#define EDPI_LCD_ARC_TYPE4              (1)             /*!< LCD Display Driver is of Type 4. */
/** \} */

#define IS_EDPI_LCD_ARC_TYPE(TYPE)   ((TYPE) == EDPI_LCD_ARC_TYPE123) || ((TYPE) == EDPI_LCD_ARC_TYPE4)

/**
 * \defgroup    EDPI_SD_EN_FCOLOR
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_SD_EN_FCOLOR_BLACK            (0)   /*!< this bit defines the color of the 4 frames is black. */
#define EDPI_SD_EN_FCOLOR_WHITE            (1)   /*!< this bit defines the color of the 4 frames is white. */
/** \} */

#define IS_EDPI_SD_EN_FCOLOR(FCOLOR)   ((FCOLOR) == EDPI_SD_EN_FCOLOR_BLACK) || ((FCOLOR) == EDPI_SD_EN_FCOLOR_WHITE)

/**
 * \defgroup    EDPI_SD_DIS_FCOLOR
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_SD_DIS_FCOLOR_BLACK              (0)           /*!< this bit defines the color of the 12 frames is black. */
#define EDPI_SD_DIS_FCOLOR_WHITE              (1)           /*!< this bit defines the color of the 12 frames is white. */
/** \} */

#define IS_EDPI_SD_DIS_FCOLOR(FCOLOR)   ((FCOLOR) == EDPI_SD_DIS_FCOLOR_BLACK) || ((FCOLOR) == EDPI_SD_DIS_FCOLOR_WHITE)

/**
 * \defgroup    EDPI_SHUTDN_POLARITY  SD POLARITY
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_SDPOL_AL                (0)            /*!< ShutDown is active low. */
#define EDPI_SDPOL_AH                (1)            /*!< ShutDown is active high. */
/** \} */

#define IS_EDPI_SDPOL(POLARITY)   ((POLARITY) == EDPI_SDPOL_AL) || ((POLARITY) == EDPI_SDPOL_AH)

/**
 * \defgroup    EDPI_COLORMODE_POLARITY  HS POLARITY
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_CLMPOL_AL                (0)               /*!< ColorMode is active low. */
#define EDPI_CLMPOL_AH                (1)               /*!< ColorMode is active high. */
/** \} */

#define IS_EDPI_CLMPOL(POLARITY)   ((POLARITY) == EDPI_CLMPOL_AL) || ((POLARITY) == EDPI_CLMPOL_AH)

/**
 * \defgroup    EDPI_SHUTDOWN_ENABLE  SD ENABLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_SD_DISABLE                (0)            /*!< ShutDown DISABLE. */
#define EDPI_SD_ENABLE                 (1)            /*!< ShutDown ENABLE. */
/** \} */

#define IS_EDPI_SD_CMD(CMD)             ((CMD) == EDPI_SD_DISABLE) || ((CMD) == EDPI_SD_ENABLE)

/**
 * \defgroup    EDPI_COLORMODE_ENABLE  CLM ENABLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_CLM_DISABLE                (0)             /*!< ShutDown is active low. */
#define EDPI_CLM_ENABLE                 (1)             /*!< ShutDown is active high. */
/** \} */

#define IS_EDPI_CLM_CMD(CMD)   ((CMD) == EDPI_CLM_DISABLE) || ((CMD) == EDPI_CLM_ENABLE)

/**
 * \defgroup    EDPI_UPDATECFG_ENABLE  UP ENABLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_UPEN_DISABLE                (0)            /*!< ShutDown is active low. */
#define EDPI_UPEN_ENABLE                 (1)            /*!< ShutDown is active high. */
/** \} */

#define IS_EDPI_UPEN_CMD(CMD)   ((CMD) == EDPI_UPEN_DISABLE) || ((CMD) == EDPI_UPEN_ENABLE)

/**
 * \defgroup    EDPI_TEAR_REQ  CLM ENABLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_TEARREQ_AL                (0)              /*!< ShutDown is active low. */
#define EDPI_TEARREQ_AH                (1)              /*!< ShutDown is active high. */
/** \} */

#define IS_EDPI_TEARREQ_POLARITY(POLARITY)   ((POLARITY) == EDPI_TEARREQ_AL) || ((POLARITY) == EDPI_TEARREQ_AH)

/**
 * \defgroup    EDPI_HALT_POLARITY  HALT POLARITY
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_HALT_AL                (0)             /*!< ShutDown is active low. */
#define EDPI_HALT_AH                (1)             /*!< ShutDown is active high. */
/** \} */

#define IS_EDPI_HALT_POLARITY(POLARITY)   ((POLARITY) == EDPI_HALT_AL) || ((POLARITY) == EDPI_HALT_AH)

/**
 * \defgroup    EDPI_RGB_COMPATIBLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_DE_CONSTANT_NONE                (0x1)   /*!< 0x1 or 0x2: No constant value for DE, it can be drive by display controller*/
#define EDPI_DE_CONSTANT_LOW                 (0x0)      /*!< constant low for DE. */
#define EDPI_DE_CONSTANT_HIGH                (0x3)      /*!< constant high for DE */
/** \} */

#define IS_EDPI_DE_CONSTANT_VALUE(VALUE)   ((VALUE) == EDPI_DE_CONSTANT_NONE) || ((VALUE) == EDPI_DE_CONSTANT_LOW) || ((VALUE) == EDPI_DE_CONSTANT_HIGH)

/**
 * \defgroup    EDPI_RGB_COMPATIBLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_VSYNC_CONSTANT_NONE                (0x1)   /*!< 0x1 or 0x2: No constant value for VSYNC, it can be drive by display controller*/
#define EDPI_VSYNC_CONSTANT_LOW                 (0x0)      /*!< constant low for VSYNC. */
#define EDPI_VSYNC_CONSTANT_HIGH                (0x3)      /*!< constant high for VSYNC */
/** \} */

#define IS_EDPI_VSYNC_CONSTANT_VALUE(VALUE)   ((VALUE) == EDPI_VSYNC_CONSTANT_NONE) || ((VALUE) == EDPI_VSYNC_CONSTANT_LOW) || ((VALUE) == EDPI_VSYNC_CONSTANT_HIGH)

/**
 * \defgroup    EDPI_RGB_COMPATIBLE
 * \{
 * \ingroup     EDPI_Exported_Constants
 */
#define EDPI_HSYNC_CONSTANT_NONE                (0x1)   /*!< 0x1 or 0x2: No constant value for HSYNC, it can be drive by display controller*/
#define EDPI_HSYNC_CONSTANT_LOW                 (0x0)      /*!< constant low for HSYNC. */
#define EDPI_HSYNC_CONSTANT_HIGH                (0x3)      /*!< constant high for HSYNC */
/** \} */

#define IS_EDPI_HSYNC_CONSTANT_VALUE(VALUE)   ((VALUE) == EDPI_HSYNC_CONSTANT_NONE) || ((VALUE) == EDPI_HSYNC_CONSTANT_LOW)  || ((VALUE) == EDPI_HSYNC_CONSTANT_HIGH)

/** \} */ /* End of group EDPI_Exported_Constants */

void EDPI_MaskLineINTConfig(FunctionalState state);

void LCDC_ClearLineINTPendingBit(void);

uint16_t EDPI_GetLineINTPos(void);

uint16_t EDPI_GetXPos(void);

uint16_t EDPI_GetYPos(void);

void EDPI_OPMODE_CONFIG(uint32_t mode);

void EDPI_Init(LCDC_eDPICfgTypeDef *eDPICfg);

#ifdef __cplusplus
}
#endif

#endif /*__RTL8762X_EDPI_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

