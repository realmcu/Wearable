/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_dbib.h
* @brief    The header file of the peripheral DBIB driver
* @details
* @author   boris yue
* @date     2021-08-23
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL876X_DBIB_H
#define __RTL876X_DBIB_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl_lcdc.h"
#include "rtl_lcdc_dbib_reg.h"

typedef struct
{
    uint32_t DBIB_SPEED_SEL;               /*!< Specifies the DBIB clock speed. */
    uint32_t DBIB_InitGuardTimeCmd;       /*!< Specifies the init guard time function. */
    uint32_t DBIB_InitGuardTime;          /*!< Specifies the guard time. This parameter is 0~3T of divider clock. */
    uint32_t DBIB_CmdGuardTimeCmd;        /*!< Specifies the command guard time function. */
    uint32_t DBIB_CmdGuardTime;           /*!< Specifies the guard time. This parameter is 0~3T of divider clock. */
    uint32_t DBIB_GuardTimeCmd;           /*!< Specifies the guard time function. The guard time only avaliable for hardware continuously wtite mode*/
    uint32_t DBIB_GuardTime;              /*!< Specifies the guard time. This parameter is 0~3T of divider clock. */
    uint32_t DBIB_WRDelay;                /*!< Specifies the delay time for WR. This parameter is half or one bus clock cycle. */
    uint32_t DBIB_8BitSwap;               /*!< Specifies the FIFO data format. */
    uint32_t DBIB_16BitSwap;              /*!< Specifies the FIFO data format. */
    uint32_t DBIB_TxThr;                  /*!< Specifies the TX FIFO threshold value. This value can be from 0 to 16. */
    uint32_t DBIB_TxDMACmd;               /*!< Specifies the TX DMA status in auto mode. */
    uint32_t DBIB_VsyncCmd;               /*!< Specifies the Vsync signal. */
    uint32_t DBIB_VsyncPolarity;          /*!< Specifies the Vsync trigger polarity. */
} LCDC_DBIBCfgTypeDef;




/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    DBIB_ClockDiv DBIB clock div
 * \{
 * \ingroup     DBIB_Exported_Constants
 */
#define DBIB_CLOCK_DIV_2                             ((uint32_t)0x1)
#define DBIB_CLOCK_DIV_3                             ((uint32_t)0x2)
#define DBIB_CLOCK_DIV_4                             ((uint32_t)0x3)
#define DBIB_CLOCK_DIV_5                             ((uint32_t)0x4)
#define DBIB_CLOCK_DIV_6                             ((uint32_t)0x5)
#define DBIB_CLOCK_DIV_7                             ((uint32_t)0x6)
#define DBIB_CLOCK_DIV_8                             ((uint32_t)0x7)
#define DBIB_CLOCK_DIV_64                            ((uint32_t)0x3F)
/** \} */

#define IS_DBIB_CLOCK_DIV(DIV)   (((DIV) == DBIB_CLOCK_DIV_2) || ((DIV) == DBIB_CLOCK_DIV_3) || \
                                  ((DIV) == DBIB_CLOCK_DIV_4) || ((DIV) == DBIB_CLOCK_DIV_5) || \
                                  ((DIV) == DBIB_CLOCK_DIV_6) || ((DIV) == DBIB_CLOCK_DIV_7) || \
                                  ((DIV) == DBIB_CLOCK_DIV_8))

/**
 * \defgroup    DBIB_InitGuardTime_Cmd DBIB Init Guard Time Cmd
 * \{
 * \ingroup     DBIB_Exported_Constants
 */
#define DBIB_INIT_GUARD_TIME_ENABLE                   (0)
#define DBIB_INIT_GUARD_TIME_DISABLE                  (1)
/** \} */

#define IS_DBIB_INIT_GUARD_TIME_CMD(CMD) (((CMD) == DBIB_INIT_GUARD_TIME_ENABLE) || ((CMD) == DBIB_INIT_GUARD_TIME_DISABLE))

/**
 * \defgroup    DBIB_InitGuardTime DBIB Init Guard Time
 * \{
 * \ingroup     DBIB_Exported_Constants
 */
#define DBIB_INIT_GUARD_TIME_1T                       (0)
#define DBIB_INIT_GUARD_TIME_2T                       ((uint32_t)0x1)
#define DBIB_INIT_GUARD_TIME_3T                       ((uint32_t)0x2)
#define DBIB_INIT_GUARD_TIME_4T                       ((uint32_t)0x3)
/** \} */

#define IS_DBIB_INIT_GUARD_TIME(TIME)   (((TIME) == DBIB_INIT_GUARD_TIME_1T) || ((TIME) == DBIB_INIT_GUARD_TIME_2T) || \
                                         ((TIME) == DBIB_INIT_GUARD_TIME_3T) || ((TIME) == DBIB_INIT_GUARD_TIME_4T))

/**
 * \defgroup    DBIB_CmdGuardTime_Cmd DBIB Cmd Guard Time Cmd
 * \{
 * \ingroup     DBIB_Exported_Constants
 */
#define DBIB_CMD_GUARD_TIME_ENABLE                   (0)
#define DBIB_CMD_GUARD_TIME_DISABLE                  (1)
/** \} */

#define IS_DBIB_CMD_GUARD_TIME_CMD(CMD) (((CMD) == DBIB_CMD_GUARD_TIME_ENABLE) || ((CMD) == DBIB_CMD_GUARD_TIME_DISABLE))

/**
 * \defgroup    DBIB_CmdGuardTime DBIB Cmd Guard Time
 * \{
 * \ingroup     DBIB_Exported_Constants
 */
#define DBIB_CMD_GUARD_TIME_1T                       (0)
#define DBIB_CMD_GUARD_TIME_2T                       ((uint32_t)0x1)
#define DBIB_CMD_GUARD_TIME_3T                       ((uint32_t)0x2)
#define DBIB_CMD_GUARD_TIME_4T                       ((uint32_t)0x3)
/** \} */

#define IS_DBIB_CMD_GUARD_TIME(TIME)   (((TIME) == DBIB_CMD_GUARD_TIME_1T) || ((TIME) == DBIB_CMD_GUARD_TIME_2T) || \
                                        ((TIME) == DBIB_CMD_GUARD_TIME_3T) || ((TIME) == DBIB_CMD_GUARD_TIME_4T))

/**
 * \defgroup   DBIB_GuardTime_Cmd DBIB Guard Time Cmd
 * \{
 * \ingroup    DBIB_Exported_Constants
 */

#define DBIB_GUARD_TIME_ENABLE                       (0)
#define DBIB_GUARD_TIME_DISABLE                      (1)
/** \} */

#define IS_DBIB_GUARD_TIME_CMD(CMD) (((CMD) == DBIB_GUARD_TIME_ENABLE) || ((CMD) == DBIB_GUARD_TIME_DISABLE))

/**
 * \defgroup    DBIB_GuardTime DBIB Guard Time
 * \{
 * \ingroup     DBIB_Exported_Constants
 */

#define DBIB_GUARD_TIME_1T                           (0)
#define DBIB_GUARD_TIME_2T                           ((uint32_t)0x1)
#define DBIB_GUARD_TIME_3T                           ((uint32_t)0x2)
#define DBIB_GUARD_TIME_4T                           ((uint32_t)0x3)
/** \} */

#define IS_DBIB_GUARD_TIME(TIME) (((TIME) == DBIB_GUARD_TIME_1T) || ((TIME) == DBIB_GUARD_TIME_2T) || \
                                  ((TIME) == DBIB_GUARD_TIME_3T) || ((TIME) == DBIB_GUARD_TIME_4T))
/**
 * \defgroup    DBIB_WRDelay DBIB WR Delay
 * \{
 * \ingroup     DBIB_Exported_Constants
 */
#define DBIB_WR_HALF_DELAY          ((uint32_t)0x1)
#define DBIB_WR_ONE_DELAY           ((uint32_t)0x2)
/** \} */

#define IS_DBIB_WR_DELAY_TIME(TIME) (((TIME) == DBIB_WR_HALF_DELAY) || \
                                     ((TIME) == DBIB_WR_ONE_DELAY))

/** \} */ /* End of group DBIB_Exported_Constants */

void DBIB_BypassCmdByteCmd(FunctionalState NewState);
void DBIB_ResetCS(void);
void DBIB_SetCS(void);
void DBIB_ResetDC(void);
void DBIB_SetDC(void);
void DBIB_Init(LCDC_DBIBCfgTypeDef *DBIBCfg);

/**
  * @brief  Send command in manual mode.
  * @param  cmd: command which to be sent.
  * @retval None
  */
void DBIB_SendCmd(uint8_t cmd);

/**
  * @brief  Send data in manual mode.
  * @param  pBuf: buffer address to be sent.
  * @param  len:  data length.
  * @retval None
  */
void DBIB_SendData(uint8_t *pBuf, uint32_t len);

/**
  * @brief  Receive data in manual mode.
  * @param  pBuf: buffer address to be received.
  * @param  len: data length.
  * @retval None
  */
void DBIB_ReceiveData(uint8_t *pBuf, uint32_t len);

/**
  * @brief  Send command and data buffer in manual mode.
  * @param  cmd: command which to be sent.
  * @param  pBuf: buffer address to be sent.
  * @param  len:  data length.
  * @retval None
  */
void DBIB_Write(uint8_t cmd, uint8_t *pBuf, uint32_t len);

/**
  * @brief  Send command and read data buffer in manual mode.
  * @param  cmd: command which to be sent.
  * @param  pBuf: buffer address to be sent.
  * @param  len:  data length.
  * @retval None
  */
void DBIB_Read(uint8_t cmd, uint8_t *pBuf, uint32_t len);

/**
  * @brief  Configure command sequences in auto mode.
  * @param  pCmdBuf: buffer address which store command sequence.
  * @param  len:  command length.
  * @retval None
  */
FlagStatus LCDC_DBIB_SetCmdSequence(uint8_t *pCmdBuf, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /*__RTL8762X_DBIB_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

