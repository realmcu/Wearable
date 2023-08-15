/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl_dbib.h
* @brief    The header file of the peripheral DBIB driver
* @details
* @author   boris yue
* @date     2021-08-23
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL_DBIB_H
#define __RTL_DBIB_H

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
 *
 * \ingroup     DBIB_Exported_Constants
 */
typedef enum
{
    DBIB_CLOCK_DIV_2 = 1,
    DBIB_CLOCK_DIV_3 = 2,
    DBIB_CLOCK_DIV_4 = 3,
    DBIB_CLOCK_DIV_5 = 4,
    DBIB_CLOCK_DIV_6 = 5,
    DBIB_CLOCK_DIV_7 = 6,
    DBIB_CLOCK_DIV_8 = 7,
    DBIB_CLOCK_DIV_64 = 0x3F,
} DBIB_CLOCK_DIV_T;

#define IS_DBIB_CLOCK_DIV(DIV)   (((DIV) == DBIB_CLOCK_DIV_2) || ((DIV) == DBIB_CLOCK_DIV_3) || \
                                  ((DIV) == DBIB_CLOCK_DIV_4) || ((DIV) == DBIB_CLOCK_DIV_5) || \
                                  ((DIV) == DBIB_CLOCK_DIV_6) || ((DIV) == DBIB_CLOCK_DIV_7) || \
                                  ((DIV) == DBIB_CLOCK_DIV_8) || ((DIV) == DBIB_CLOCK_DIV_64))

/**
 * \defgroup    DBIB_InitGuardTime_Cmd DBIB Init Guard Time Cmd
 *
 * \ingroup     DBIB_Exported_Constants
 */
typedef enum
{
    DBIB_GUARD_TIME_ENABLE,
    DBIB_GUARD_TIME_BYPASS,
} DBIB_GUARD_TIME_BYPASS_T;

#define IS_DBIB_GUARD_TIME_CMD(CMD) (((CMD) == DBIB_GUARD_TIME_ENABLE) || ((CMD) == DBIB_GUARD_TIME_BYPASS))

/**
 * \defgroup    DBIB_InitGuardTime DBIB Init Guard Time
 *
 * \ingroup     DBIB_Exported_Constants
 */
typedef enum
{
    DBIB_GUARD_TIME_1T,
    DBIB_GUARD_TIME_2T,
    DBIB_GUARD_TIME_3T,
    DBIB_GUARD_TIME_4T,
} DBIB_GUARD_TIME_T;

#define IS_DBIB_GUARD_TIME(TIME)   (((TIME) == DBIB_GUARD_TIME_1T) || ((TIME) == DBIB_GUARD_TIME_2T) || \
                                    ((TIME) == DBIB_GUARD_TIME_3T) || ((TIME) == DBIB_GUARD_TIME_4T))

/**
 * \defgroup    DBIB_WRDelay DBIB WR Delay
 *
 * \ingroup     DBIB_Exported_Constants
 */
typedef enum
{
    DBIB_WR_HALF_DELAY = 1,
    DBIB_WR_ONE_DELAY = 2,
} DBIB_WR_DELAY_T;

#define IS_DBIB_WR_DELAY_TIME(TIME) (((TIME) == DBIB_WR_HALF_DELAY) || \
                                     ((TIME) == DBIB_WR_ONE_DELAY))

/** \} */ /* End of group DBIB_Exported_Constants */

__STATIC_INLINE void DBIB_BypassCmdByteCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    DBIB_CTRL0_t dbib_reg_0x00 = {.d32 = DBIB->DBIB_CTRL0};
    if (NewState == ENABLE)
    {
        dbib_reg_0x00.b.bypass_cmd = 1;
    }
    else
    {
        dbib_reg_0x00.b.bypass_cmd = 0;
    }
    DBIB->DBIB_CTRL0 = dbib_reg_0x00.d32;
}

__STATIC_INLINE void DBIB_ResetCS(void)
{
    DBIB_CFG_t dbib_reg_0x04 = {.d32 = DBIB->DBIB_CFG};
    dbib_reg_0x04.b.reg_cs_manual = 0;
    DBIB->DBIB_CFG = dbib_reg_0x04.d32;
}

__STATIC_INLINE void DBIB_SetCS(void)
{
    DBIB_CFG_t dbib_reg_0x04 = {.d32 = DBIB->DBIB_CFG};
    dbib_reg_0x04.b.reg_cs_manual = 1;
    DBIB->DBIB_CFG = dbib_reg_0x04.d32;
}

__STATIC_INLINE void DBIB_ResetDC(void)
{
    DBIB_CFG_t dbib_reg_0x04 = {.d32 = DBIB->DBIB_CFG};
    dbib_reg_0x04.b.reg_d_cb_manual = 0;
    DBIB->DBIB_CFG = dbib_reg_0x04.d32;
}

__STATIC_INLINE void DBIB_SetDC(void)
{
    DBIB_CFG_t dbib_reg_0x04 = {.d32 = DBIB->DBIB_CFG};
    dbib_reg_0x04.b.reg_d_cb_manual = 1;
    DBIB->DBIB_CFG = dbib_reg_0x04.d32;
}

void DBIB_Init(LCDC_DBIBCfgTypeDef *DBIBCfg);
void DBIB_SendCmd(uint8_t cmd);
void DBIB_SendData(uint8_t *pBuf, uint32_t len);
void DBIB_ReceiveData(uint8_t *pBuf, uint32_t len);
void DBIB_Write(uint8_t cmd, uint8_t *pBuf, uint32_t len);
void DBIB_Read(uint8_t cmd, uint8_t *pBuf, uint32_t len);
FlagStatus LCDC_DBIB_SetCmdSequence(uint8_t *pCmdBuf, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /*__RTL_DBIB_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

