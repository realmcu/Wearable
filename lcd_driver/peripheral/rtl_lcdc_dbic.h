/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_dbic.h
* @brief    The header file of the peripheral DBIB driver
* @details
* @author   boris yue
* @date     2021-08-23
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL876X_DBIC_H
#define __RTL876X_DBIC_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl_lcdc.h"
#include "rtl_lcdc_dbic_reg.h"

typedef struct
{
    uint32_t DBIC_SPEED_SEL;               /*!< Specifies the DBIC clock speed. */
    uint32_t DBIC_TxThr;                  /*!< Specifies the TX FIFO threshold value. This value can be from 0 to 16. */
    uint32_t DBIC_RxThr;
    uint32_t SCPOL;
    uint32_t SCPH;
} LCDC_DBICCfgTypeDef;

#define BIT_CK_MTIMES(x)        (((x) & 0x0000001F) << 23)
#define BIT_FAST_RD(x)          (((x) & 0x00000001) << 22)
#define BIT_CMD_CH(x)           (((x) & 0x00000003) << 20)
#define BIT_DATA_CH(x)          (((x) & 0x00000003) << 18)
#define BIT_ADDR_CH(x)          (((x) & 0x00000003) << 16)
#define BIT_TMOD(x)             (((x) & 0x00000003) << 8)

#define DBIC_AUTO_MODE              ((uint32_t)0x0)
#define DBIC_USER_MODE              ((uint32_t)0x1)
#define IS_DBIC_MODE(mode)          ((mode == DBIC_AUTO_MODE) || (mode == DBIC_USER_MODE))

#define DBIC_CMD_CH_SINGLE          ((uint32_t)0x0)
#define DBIC_CMD_CH_DUAL            ((uint32_t)0x1)
#define DBIC_CMD_CH_QUAD            ((uint32_t)0x2)
#define DBIC_CMD_CH_OCTAL           ((uint32_t)0x3)
#define IS_DBIC_CMD_CH_NUM(num)     ((num == DBIC_CMD_CH_SINGLE) || (num == DBIC_CMD_CH_DUAL) ||\
                                     (num == DBIC_CMD_CH_QUAD) || (num == DBIC_CMD_CH_OCTAL))

#define DBIC_DATA_CH_SINGLE         ((uint32_t)0x0)
#define DBIC_DATA_CH_DUAL           ((uint32_t)0x1)
#define DBIC_DATA_CH_QUAD           ((uint32_t)0x2)
#define DBIC_DATA_CH_OCTAL          ((uint32_t)0x3)
#define IS_DBIC_DATA_CH_NUM(num)    ((num == DBIC_DATA_CH_SINGLE) || (num == DBIC_DATA_CH_DUAL) ||\
                                     (num == DBIC_DATA_CH_QUAD) || (num == DBIC_DATA_CH_OCTAL))

#define DBIC_ADDR_CH_SINGLE         ((uint32_t)0x0)
#define DBIC_ADDR_CH_DUAL           ((uint32_t)0x1)
#define DBIC_ADDR_CH_QUAD           ((uint32_t)0x2)
#define DBIC_ADDR_CH_OCTAL          ((uint32_t)0x3)
#define IS_DBIC_DATA_CH_NUM(num)    ((num == DBIC_DATA_CH_SINGLE) || (num == DBIC_DATA_CH_DUAL) ||\
                                     (num == DBIC_DATA_CH_QUAD) || (num == DBIC_DATA_CH_OCTAL))

#define DBIC_TMODE_TX               ((uint32_t)0x0)
#define DBIC_TMODE_RX               ((uint32_t)0x3)
#define IS_DBIC_DIR(dir)            ((dir == DBIC_TMODE_TX) || (dir == DBIC_TMODE_RX))

#define DBIC_SCPOL_LOW              ((uint32_t)0x0)
#define DBIC_SCPOL_HIGH             ((uint32_t)0x1)
#define IS_DBIC_SCPOL(pol)          ((pol == DBIC_SCPOL_LOW) || (pol == DBIC_SCPOL_HIGH))

#define DBIC_SCPH_1Edge             ((uint32_t)0x0)
#define DBIC_SCPH_2Edge             ((uint32_t)0x1)
#define IS_DBIC_SCPH(phase)         ((phase == DBIC_SCPH_1Edge) || (phase == DBIC_SCPH_2Edge))

/**
  * @brief  Enable/disable DBIC interface.
  * @param  NewState: state of DBIC
  * @retval None
  */
void DBIC_Cmd(FunctionalState NewState);

/**
  * @brief  Switch to user/auto mode.
  * @param  mode: DBIC mode
  * @retval None
  */
void DBIC_SwitchMode(uint32_t mode);

/**
  * @brief  Switch TX/RX direction .
  * @param  dir: TX/RX
  * @retval None
  */
void DBIC_SwitchDirect(uint32_t dir);

/**
  * @brief  Configure length of CMD.
  * @param  len: command length
  * @retval None
  */
void DBIC_CmdLength(uint32_t len);

/**
  * @brief  Configure length of address.
  * @param  len: address length
  * @retval None
  */
void DBIC_AddrLength(uint32_t len);

/**
  * @brief  Each bit in the register corresponds to one SPI Flash. In user mode
  *         user program the register to select target flash. .
  * @retval None
  */
void DBIC_Select(void);

/**
  * @brief  Configure num of data frames in bytes.
  * @param  len: data length
  * @retval None
  */
void DBIC_TX_NDF(uint32_t len);

/**
  * @brief  Initialize DBIC.
  * @param  DBICCfg: configuration structure of DBIC
  * @retval None
  */
void DBIC_Init(LCDC_DBICCfgTypeDef *DBICCfg);

/**
  * @brief  Send data through DBIC interface.
  * @param  buf: data to be sent.
  * @param  len:  data length.
  * @retval None
  */
void DBIC_SendBuf(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /*__RTL8762X_DBIC_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

