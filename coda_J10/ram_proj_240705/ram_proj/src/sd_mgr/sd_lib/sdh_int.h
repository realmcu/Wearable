/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdh_int.h
* @brief    This file provides sd host interrupt's service routines for sd library.
* @details
* @author   elliot_chen
* @date     2021-02-01
* @version  v1.0
*********************************************************************************************************
*/

#ifndef __SDH_INT_H
#define __SDH_INT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_def.h"
#include "vector_table.h"

/* Defines ------------------------------------------------------------------*/
typedef void (*SDH_INTR_HDL_CB)(void *);

typedef enum
{
    SDH_CB_IDX_INT_VENDOR_SPECIFIC_ERR = 0x00,
    SDH_CB_IDX_INT_ADMA_ERR,
    SDH_CB_IDX_INT_AUTO_CMD12_ERR,
    SDH_CB_IDX_INT_CURRENT_LIMIT_ERR,
    SDH_CB_IDX_INT_DAT_END_BIT_ERR,
    SDH_CB_IDX_INT_DAT_CRC_ERR,
    SDH_CB_IDX_INT_DAT_TIMEOUT_ERR,
    SDH_CB_IDX_INT_CMD_INDEX_ERR,
    SDH_CB_IDX_INT_CMD_END_BIT_ERR,
    SDH_CB_IDX_INT_CMD_CRC_ERR,
    SDH_CB_IDX_INT_CMD_TIMEOUT_ERR,
    SDH_CB_IDX_INT_CARD_INSERT,
    SDH_CB_IDX_INT_CARD_REMOVAL,
    SDH_CB_IDX_INT_RW_DONE,
    SDH_CB_IDX_INT_NUM,
} T_SDH_CB_IDX;
/*
0x30
0      RW1C Command Complete          1'b00
1      RW1C Transfer Complete         1'b00
2      RW1C Block  Gap Event          1'b00
3      RW1C DMA Inerrupt              1'b00

4      RW1C Buffer Write Ready        1'b00
5      RW1C Buffer Read Ready         1'b00
6      RW1C Card  Insertion           1'b00
7      RW1C Card  Removal             1'b00

8      ROC  Card  Interrupt           1'b00
9      ROC  INT_A                     1'b00
10     ROC  INT_B                     1'b00
11     ROC  INT_C                     1'b00
12     ROC  Re-Tuning Event           1'b00
13     ROC  FX Event                  1'b00
14     Rsvd Reserved                  1'b00
15     ROC  Error  Interrupt          1'b00

*/
typedef union _T_SDH_NOR_INT_STATUS
{
    struct
    {
        uint16_t  INT_CMD_CMPL           : 1;
        uint16_t  INT_TF_CMPL            : 1;
        uint16_t  BLOCK_GAP_EVENT        : 1;
        uint16_t  INT_DMA                : 1;

        uint16_t  BUF_WRITE_RDY          : 1;
        uint16_t  BUF_READ_RDY           : 1;
        uint16_t  CARD_INSERT            : 1;
        uint16_t  CARD_REMOVAL           : 1;

        uint16_t  INT_CARD               : 1;
        uint16_t  RESERVED               : 6;
        uint16_t  INT_ERROR              : 1;

    };
    uint16_t d16;
} T_SDH_NOR_INT_STATUS;

/*
0x32
0      RW1C Command Timeout Error             1'b00
1      RW1C Command CRC     Error             1'b00
2      RW1C Command End Bit Error             1'b00
3      RW1C Command Index   Error             1'b00

4      RW1C Data    Timeout Error             1'b00
5      RW1C Data    CRC     Error             1'b00
6      RW1C Data    End Bit Error             1'b00
7      RW1C Current Limit   Error             1'b00

8      RW1C Auto CMD        Error             1'b00
9      RW1C ADMA            Error             1'b00
10     RW1C Tuning          Error             1'b00
11     RW1C Response        Error             1'b00

12:15  RW1C Vendor Specific Error             4'b00

*/
typedef union _T_SDH_ERR_INT_STATUS
{
    struct
    {
        uint16_t  INT_CMD_TIMEOUT_ERR        : 1;
        uint16_t  INT_CMD_CRC_ERR            : 1;
        uint16_t  INT_CMD_END_BIT_ERR        : 1;
        uint16_t  INT_CMD_INDEX_ERR          : 1;

        uint16_t  INT_DAT_TIMEOUT_ERR        : 1;
        uint16_t  INT_DAT_CRC_ERR            : 1;
        uint16_t  INT_DAT_END_BIT_ERR        : 1;
        uint16_t  INT_CURRENT_LIMIT_ERR      : 1;

        uint16_t  INT_AUTO_CMD12_ERR         : 1;
        uint16_t  INT_ADMA_ERR               : 1;
        uint16_t  INT_TUNING_ERR             : 1;
        uint16_t  INT_RESPONSE_ERR           : 1;

        uint16_t  INT_VENDOR_SPEC_ERR        : 4;

    };
    uint16_t d16;
} T_SDH_ERR_INT_STATUS;

/* Exported functions -------------------------------------------------------*/
void SDH_RegisterIntHandleCB(T_SDH_CB_IDX idx, SDH_INTR_HDL_CB func);
void SDH_IntModeInit(FunctionalState newState);
void SDH_TransCtrlIntConfig(FunctionalState newState);
void SDH_PlugAndErrorIntConfig(void);
void SDIO_Intr_Handler(void);
T_SD_STATUS SDIO_CheckErr_TryRecovery(void);
void SDIO_ErrStatusClearAndRecovery(void);
void SDH_SetErrorInt(bool is_enable);

/* Exported variable --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*__SDH_INT_H*/

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

