/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl_dbic.h
* @brief    The header file of the peripheral DBIB driver
* @details
* @author   boris yue
* @date     2021-08-23
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL_DBIC_H
#define __RTL_DBIC_H

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

typedef enum
{
    DBIC_AUTO_MODE,
    DBIC_USER_MODE,
} DBIC_MODE_T;

typedef enum
{
    DBIC_CH_SINGLE,
    DBIC_CH_DUAL,
    DBIC_CH_QUAD,
    DBIC_CH_OCTAL,
} DBIC_CH_T;

typedef enum
{
    DBIC_TMODE_TX = 0,
    DBIC_TMODE_RX = 3,
} DBIC_TMODE_T;

typedef enum
{
    DBIC_SCPOL_LOW,
    DBIC_SCPOL_HIGH,
} DBIC_SCPOL_T;

typedef enum
{
    DBIC_SCPH_1Edge,
    DBIC_SCPH_2Edge,
} DBIC_SCPH_T;

#if 0
#define BIT_CK_MTIMES(x)        (((x) & 0x0000001F) << 23)
#define BIT_FAST_RD(x)          (((x) & 0x00000001) << 22)
#define BIT_CMD_CH(x)           (((x) & 0x00000003) << 20)
#define BIT_DATA_CH(x)          (((x) & 0x00000003) << 18)
#define BIT_ADDR_CH(x)          (((x) & 0x00000003) << 16)
#define BIT_TMOD(x)             (((x) & 0x00000003) << 8)
#define BIT_SCPOL               (0x00000001 << 7)
#define BIT_SCPH                (0x00000001 << 6)

#define BIT_TXSIM               (0x00000001 << 9)
#define BIT_SEQ_EN              (0x00000001 << 3)
/********************  Bits definition for SPIC_SR register  *******************/
#define BIT_TXE                 (0x00000001 << 5)
#define BIT_RFF                 (0x00000001 << 4)
#define BIT_RFNE                (0x00000001 << 3)
#define BIT_TFE                 (0x00000001 << 2)
#define BIT_TFNF                    (0x00000001 << 1)
#define BIT_BUSY                    (0x00000001)
#define BIT_RD_DUMMY_LENGTH(x)  (((x) & 0x00000fff))
#endif

__STATIC_INLINE void DBIC_Cmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    DBIC_SSIENR_t dbic_reg_0x08 = {.d32 = DBIC->SSIENR};
    if (NewState == ENABLE)
    {
        dbic_reg_0x08.b.spic_en = 1;//enable
    }
    else
    {
        dbic_reg_0x08.b.spic_en = 0;
    }
    DBIC->SSIENR = dbic_reg_0x08.d32;
}

__STATIC_INLINE void DBIC_SwitchMode(uint32_t mode)
{
    assert_param(IS_DBIC_MODE(mode));
    DBIC_CTRLR0_t dbic_reg_0x00 = {.d32 = DBIC->CTRLR0};
    dbic_reg_0x00.b.user_mode = mode;
    DBIC->CTRLR0 = dbic_reg_0x00.d32;
}

__STATIC_INLINE void DBIC_SwitchDirect(uint32_t dir)
{
    assert_param(IS_DBIC_DIR(dir));
    DBIC_CTRLR0_t dbic_reg_0x00 = {.d32 = DBIC->CTRLR0};
    dbic_reg_0x00.b.tmod = dir;
    DBIC->CTRLR0 = dbic_reg_0x00.d32;
}

__STATIC_INLINE void DBIC_CmdLength(uint32_t len)
{
    DBIC_USER_LENGTH_t dbic_reg_0x118 = {.d32 = DBIC->USER_LENGTH};
    dbic_reg_0x118.b.user_cmd_lenght = len;
    DBIC->USER_LENGTH = dbic_reg_0x118.d32;
}

__STATIC_INLINE void DBIC_AddrLength(uint32_t len)
{
    DBIC_USER_LENGTH_t dbic_reg_0x118 = {.d32 = DBIC->USER_LENGTH};
    dbic_reg_0x118.b.user_addr_length = len;
    DBIC->USER_LENGTH = dbic_reg_0x118.d32;
}

__STATIC_INLINE void DBIC_Select()
{
    DBIC_SER_t dbic_reg_0x10 = {.d32 = DBIC->SER};
    dbic_reg_0x10.b.ser = 1;
    DBIC->SER = dbic_reg_0x10.d32;
}

__STATIC_INLINE void DBIC_TX_NDF(uint32_t len)
{
    DBIC_TX_NDF_t dbic_reg_0x130 = {.d32 = DBIC->TX_NDF};
    dbic_reg_0x130.b.tx_ndf = len;
    DBIC->TX_NDF = dbic_reg_0x130.d32;
}

void DBIC_Init(LCDC_DBICCfgTypeDef *DBICCfg);
void DBIC_SendBuf(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /*__RTL_DBIC_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

