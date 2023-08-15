/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_ramless_qspi.h
* @brief    This file provides all the LCDC Ramless QSPI firmware functions.
* @details
* @author   howie wang
* @date     2021-08-17
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL8762X_RAMLESS_QSPI_H
#define __RTL8762X_RAMLESS_QSPI_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"
#include "stdbool.h"


typedef struct
{
    __IO uint32_t RLSPI_VSYNC_CMD;
    __IO uint32_t RLSPI_VSYNC_CMD_ADDR;
    __IO uint32_t RLSPI_HSYNC_CMD_VBPORCH;
    __IO uint32_t RLSPI_HSYNC_CMD_ADDR_VBPORCH;
    __IO uint32_t RLSPI_HSYNC_CMD_VACTIVE;
    __IO uint32_t RLSPI_HSYNC_CMD_ADDR_VACTIVE;
    __IO uint32_t RLSPI_HSYNC_CMD_VFP;
    __IO uint32_t RLSPI_HSYNC_CMD_ADDR_VFP;
    __IO uint32_t RLSPI_LINE_DELAY_OUT_VACTIVE;
    __IO uint32_t RLSPI_LINE_DELAY_IN_VACTIVE;
    __IO uint32_t RLSPI_FRAME_DELAY_INFINITE;
    __I  uint32_t RSVD1[2];
    __IO uint32_t RLSPI_VERTICAL_SYNC_WIDTH;
    __IO uint32_t RLSPI_VERTICAL_ABACK_PORCH;
    __IO uint32_t RLSPI_VERTICAL_AACTIVE;
    __IO uint32_t RLSPI_VERTICAL_TOTAL_HEIGHT;
    __IO uint32_t RLSPI_HORIZONTAL_WIDTH;
} LCDC_Ramless_QSPI_TypeDef;

typedef enum
{
    VSYNC_CMD,
    VSYNC_CMD_ADDR,
    HSYNC_CMD_VBP,
    HSYNC_CMD_VBP_ADDR,
    HSYNC_CMD_VACTIVE,
    HSYNC_CMD_VACTIVE_ADDR,
    HSYNC_CMD_VFP,
    HSYNC_CMD_VFP_ADDR,
} LCDC_Ramless_QSPI_Reg_Enum;

typedef struct
{
    uint32_t VSA;       /* vertical synchronization active height (in units of horizontal scan line)*/
    uint32_t VBP;     /* accumulated Vertical back porch (in units of horizontal scan line) */
    uint32_t VFP;
    uint32_t tear_input_mux;
    uint32_t VSYNC_CMD;
    uint32_t VSYNC_CMD_ADDR;
    uint32_t HSYNC_CMD_VBP;
    uint32_t HSYNC_CMD_VBP_ADDR;
    uint32_t HSYNC_CMD_VACTIVE;
    uint32_t HSYNC_CMD_VACTIVE_ADDR;
    uint32_t HSYNC_CMD_VFP;
    uint32_t HSYNC_CMD_VFP_ADDR;
    uint32_t DUMMY_CMD;
    uint32_t width;
    uint32_t height;
    uint32_t line_delay_in_vactive;
} LCDC_RLSPI_initTypeDef;


/*============================================================================*
 *                         Registers Definitions
 *============================================================================*/
/* Peripheral: LCDC Ramless QSPI */
/* Description: LCDC Ramless QSPI register defines */

/* Register: RLSPI_VSYNC_CMD -------------------------------------------------------*/
/* Description: RLSPI_VSYNC_CMD. Offset: 0x00. Address: . */
/* RLSPI_VSYNC_CMD[23:0]: . */
#define RLSPI_VSYNC_CMD_POS                         (0)
#define RLSPI_VSYNC_CMD_MSK                         (0x7FFFFF << RLSPI_VSYNC_CMD_POS)
#define RLSPI_VSYNC_CMD_CLR                         (~RLSPI_VSYNC_CMD_MSK)

/* Register: RLSPI_VSYNC_CMD_ADDR -------------------------------------------------------*/
/* Description: RLSPI_VSYNC_CMD_ADDR. Offset: 0x04. Address: . */
/* RLSPI_VSYNC_CMD_ADDR[31:0]: . */
#define RLSPI_VSYNC_CMD_ADDR_POS                    (0)
#define RLSPI_VSYNC_CMD_ADDR_MSK                    (0xFFFFFFFF << RLSPI_VSYNC_CMD_ADDR_POS)
#define RLSPI_VSYNC_CMD_ADDR_CLR                    (~RLSPI_VSYNC_CMD_ADDR_MSK)

/* Register: RLSPI_HSYNC_CMD_VBPORCH -------------------------------------------------------*/
/* Description: RLSPI_HSYNC_CMD_VBPORCH. Offset: 0x08. Address: . */
/* RLSPI_HSYNC_CMD_VBPORCH[23:0]: . */
#define RLSPI_HSYNC_CMD_VBPORCH_POS                 (0)
#define RLSPI_HSYNC_CMD_VBPORCH_MSK                 (0xFFFFFF << RLSPI_HSYNC_CMD_VBPORCH_POS)
#define RLSPI_HSYNC_CMD_VBPORCH_CLR                 (~RLSPI_HSYNC_CMD_VBPORCH_MSK)

/* Register: RLSPI_HSYNC_CMD_ADDR_VBPORCH -------------------------------------------------------*/
/* Description: RLSPI_HSYNC_CMD_ADDR_VBPORCH. Offset: 0x0c. Address: . */
/* RLSPI_HSYNC_CMD_VBPORCH[31:0]: . */
#define RLSPI_HSYNC_CMD_ADDR_VBPORCH_POS            (0)
#define RLSPI_HSYNC_CMD_ADDR_VBPORCH_MSK            (0xFFFFFFFF << RLSPI_HSYNC_CMD_ADDR_VBPORCH_POS)
#define RLSPI_HSYNC_CMD_ADDR_VBPORCH_CLR            (~RLSPI_HSYNC_CMD_ADDR_VBPORCH_MSK)

/* Register: RLSPI_HSYNC_CMD_VACTIVE -------------------------------------------------------*/
/* Description: RLSPI_HSYNC_CMD_VACTIVE. Offset: 0x10. Address: . */
/* RLSPI_HSYNC_CMD_VACTIVE[31:0]: . */
#define RLSPI_HSYNC_CMD_VACTIVE_POS                 (0)
#define RLSPI_HSYNC_CMD_VACTIVE_MSK                 (0xFFFFFFFF << RLSPI_HSYNC_CMD_VACTIVE_POS)
#define RLSPI_HSYNC_CMD_VACTIVE_CLR                 (~RLSPI_HSYNC_CMD_VACTIVE_MSK)

/* Register: RLSPI_HSYNC_CMD_ADDR_VACTIVE -------------------------------------------------------*/
/* Description: RLSPI_HSYNC_CMD_ADDR_VACTIVE. Offset: 0x14. Address: . */
/* RLSPI_HSYNC_CMD_ADDR_VACTIVE[31:0]: . */
#define RLSPI_HSYNC_CMD_ADDR_VACTIVE_POS            (0)
#define RLSPI_HSYNC_CMD_ADDR_VACTIVE_MSK            (0xFFFFFFFF << RLSPI_HSYNC_CMD_ADDR_VACTIVE_POS)
#define RLSPI_HSYNC_CMD_ADDR_VACTIVE_CLR            (~RLSPI_HSYNC_CMD_ADDR_VACTIVE_MSK)

/* Register: RLSPI_HSYNC_CMD_VFP -------------------------------------------------------*/
/* Description: RLSPI_HSYNC_CMD_VFP. Offset: 0x18. Address: . */
/* RLSPI_HSYNC_CMD_VFP[23:0]: . */
#define RLSPI_HSYNC_CMD_VFP_POS                     (0)
#define RLSPI_HSYNC_CMD_VFP_MSK                     (0xFFFFFF << RLSPI_HSYNC_CMD_VFP_POS)
#define RLSPI_HSYNC_CMD_VFP_CLR                     (~RLSPI_HSYNC_CMD_VFP_MSK)

/* Register: RLSPI_HSYNC_CMD_ADDR_VFP -------------------------------------------------------*/
/* Description: RLSPI_HSYNC_CMD_ADDR_VFP. Offset: 0x1C. Address: . */
/* RLSPI_HSYNC_CMD_ADDR_VFP[31:0]: . */
#define RLSPI_HSYNC_CMD_ADDR_VFP_POS                (0)
#define RLSPI_HSYNC_CMD_ADDR_VFP_MSK                (0xFFFFFFFF << RLSPI_HSYNC_CMD_ADDR_VFP_POS)
#define RLSPI_HSYNC_CMD_ADDR_VFP_CLR                (~RLSPI_HSYNC_CMD_ADDR_VFP_MSK)

/* Register: RLSPI_LINE_DELAY_OUT_VACTIVE -------------------------------------------------------*/
/* Description: RLSPI_LINE_DELAY_OUT_VACTIVE. Offset: 0x20. Address: . */
/* RLSPI_LINE_DELAY_OUT_VACTIVE[31:0]: delay between each line of ramless SPI outside vertical active period. */
#define RLSPI_LINE_DELAY_OUT_VACTIVE_POS            (0)
#define RLSPI_LINE_DELAY_OUT_VACTIVE_MSK            (0xFFFFFFFF << RLSPI_LINE_DELAY_OUT_VACTIVE_POS)
#define RLSPI_LINE_DELAY_OUT_VACTIVE_CLR            (~RLSPI_LINE_DELAY_OUT_VACTIVE_MSK)

/* Register: RLSPI_LINE_DELAY_IN_VACTIVE -------------------------------------------------------*/
/* Description: RLSPI_LINE_DELAY_IN_VACTIVE. Offset: 0x24. Address: . */
/* RLSPI_LINE_DELAY_IN_VACTIVE[31:0]: delay between each line of ramless SPI outside vertical active period. */
#define RLSPI_LINE_DELAY_IN_VACTIVE_POS             (0)
#define RLSPI_LINE_DELAY_IN_VACTIVE_MSK             (0xFFFFFFFF << RLSPI_LINE_DELAY_IN_VACTIVE_POS)
#define RLSPI_LINE_DELAY_IN_VACTIVE_CLR             (~RLSPI_LINE_DELAY_IN_VACTIVE_MSK)

/* Register: RLSPI_FRAME_DELAY_INFINITE -------------------------------------------------------*/
/* Description: RLSPI_FRAME_DELAY_INFINITE. Offset: 0x28. Address: . */
/* RLSPI_FRAME_DELAY_INFINITE[31:0]: delay between each line of ramless SPI in infinite mode. */
#define RLSPI_FRAME_DELAY_INFINITE_POS              (0)
#define RLSPI_FRAME_DELAY_INFINITE_MSK              (0xFFFFFFFF << RLSPI_FRAME_DELAY_INFINITE_POS)
#define RLSPI_FRAME_DELAY_INFINITE_CLR              (~RLSPI_FRAME_DELAY_INFINITE_MSK)

/* Register: RSVD -------------------------------------------------------*/
/* Description: Control register 0. Offset: 0x2C. Address: . */
/* RSVD[31:0]: RSVD. */

/* Register: RSVD -------------------------------------------------------*/
/* Description: Control register 0. Offset: 0x30. Address: . */
/* RSVD[31:0]: RSVD. */

/* Register: RLSPI_VERTICAL_SYNC_WIDTH -------------------------------------------------------*/
/* Description: RLSPI_VERTICAL_SYNC_WIDTH. Offset: 0x34. Address: . */
/* RLSPI_VERTICAL_SYNC_WIDTH[10:0]: vertical synchronize width for each frame. */
#define RLSPI_VERTICAL_SYNC_WIDTH_POS               (0)
#define RLSPI_VERTICAL_SYNC_WIDTH_MSK               (0x7FF << RLSPI_VERTICAL_SYNC_WIDTH_POS)
#define RLSPI_VERTICAL_SYNC_WIDTH_CLR               (~RLSPI_VERTICAL_SYNC_WIDTH_MSK)

/* Register: RLSPI_VERTICAL_ABACK_PORCH -------------------------------------------------------*/
/* Description: RLSPI_VERTICAL_ABACK_PORCH. Offset: 0x38. Address: . */
/* RLSPI_VERTICAL_ABACK_PORCH[10:0]: accumulated vertical back porch. */
#define RLSPI_VERTICAL_ABACK_PORCH_POS              (0)
#define RLSPI_VERTICAL_ABACK_PORCH_MSK              (0x7FF << RLSPI_VERTICAL_ABACK_PORCH_POS)
#define RLSPI_VERTICAL_ABACK_PORCH_CLR              (~RLSPI_VERTICAL_ABACK_PORCH_MSK)

/* Register: RLSPI_VERTICAL_AACTIVE -------------------------------------------------------*/
/* Description: RLSPI_VERTICAL_AACTIVE. Offset: 0x3C. Address: . */
/* RLSPI_VERTICAL_AACTIVE[10:0]: accumulated active height. */
#define RLSPI_VERTICAL_AACTIVE_POS                  (0)
#define RLSPI_VERTICAL_AACTIVE_MSK                  (0x7FF << RLSPI_VERTICAL_AACTIVE_POS)
#define RLSPI_VERTICAL_AACTIVE_CLR                  (~RLSPI_VERTICAL_AACTIVE_MSK)

/* Register: RLSPI_VERTICAL_TOTAL_HEIGHT -------------------------------------------------------*/
/* Description: RLSPI_VERTICAL_TOTAL_HEIGHT. Offset: 0x40. Address: . */
/* RLSPI_VERTICAL_TOTAL_HEIGHT[10:0]: total height. */
#define RLSPI_VERTICAL_TOTAL_HEIGHT_POS             (0)
#define RLSPI_VERTICAL_TOTAL_HEIGHT_MSK             (0x7FF << RLSPI_VERTICAL_TOTAL_HEIGHT_POS)
#define RLSPI_VERTICAL_TOTAL_HEIGHT_CLR             (~RLSPI_VERTICAL_TOTAL_HEIGHT_MSK)

/* Register: RLSPI_HORIZONTAL_WIDTH -------------------------------------------------------*/
/* Description: RLSPI_HORIZONTAL_WIDTH. Offset: 0x4c. Address: . */
/* RLSPI_HORIZONTAL_WIDTH[11:0]: active widht. */
#define RLSPI_HORIZONTAL_WIDTH_POS                  (0)
#define RLSPI_HORIZONTAL_WIDTH_MSK                  (0xFFF << RLSPI_HORIZONTAL_WIDTH_POS)
#define RLSPI_HORIZONTAL_WIDTH_CLR                  (~RLSPI_HORIZONTAL_WIDTH_MSK)


#define RLSPI_TEAR_INPUT_FROM_DSI                   ((uint32_t)0x0 << LCDC_TEAR_INPUT_MUX_POS);
#define RLSPI_TEAR_INPUT_FROM_LCD                   ((uint32_t)0x1 << LCDC_TEAR_INPUT_MUX_POS);
#define IS_RLSPI_TEAR_INPUT(in)                     ((in == RLSPI_TEAR_INPUT_FROM_DSI) || (in == RLSPI_TEAR_INPUT_FROM_LCD)
/*============================================================================*
 *                                  Functions
 *============================================================================*/
void RLSPI_SetCmd(LCDC_Ramless_QSPI_Reg_Enum cmd, uint32_t value);

void RLSPI_StructInit(LCDC_RLSPI_initTypeDef *obj);

void RLSPI_Init(LCDC_RLSPI_initTypeDef *obj);

void RLSPI_Cmd(uint8_t state);
#endif
