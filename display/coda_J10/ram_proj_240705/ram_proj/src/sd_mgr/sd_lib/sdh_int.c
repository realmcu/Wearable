/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdh_int.c
* @brief    This file provides sd host interrupt's service routines for sd library.
* @details
* @author   elliot_chen
* @date     2021-02-01
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "rtl876x_nvic.h"
#include "rtl876x_sdio.h"
#include "trace.h"
#include "vector_table.h"
#include "platform_utils.h"
#include "sd_def.h"
#include "sdh_int.h"

/* Defines -------------------------------------------------------------------*/
#define SDIO0_VECTORn                           IRQn_TO_VECTORn(SDIO0_IRQn)

#define SDH_TRANS_INT                           SDIO_INT_TF_CMPL

#define SW_RST_FOR_DAT_LINE_MASK        BIT2
#define SW_RST_FOR_CMD_LINE_MASK        BIT1
#define SW_RST_FOR_ALL_MASK             BIT0

#define COMMAND_INHIBIT_DAT_MASK        BIT1
#define COMMAND_INHIBIT_CMD_MASK        BIT0

#define DAT3_0_LINE_SIGNAL_LEVEL_MASK   (BIT23 | BIT22 | BIT21 | BIT20)

/* Internal declaration ------------------------------------------------------*/
void SDIO_HOST_Handler(void);

/* Gloabls -------------------------------------------------------------------*/
SDH_INTR_HDL_CB sdh_intr_hdl_cb[SDH_CB_IDX_INT_NUM] = {0};
uint16_t sdio_err_status = 0;

void SDH_RegisterIntHandleCB(T_SDH_CB_IDX idx, SDH_INTR_HDL_CB func)
{
    sdh_intr_hdl_cb[idx] = func;
}

void SDH_IntModeInit(FunctionalState newState)
{
#if defined TARGET_RTL87X3D
    RamVectorTableUpdate(SDIO_HOST_VECTORn, (IRQ_Fun)SDIO_HOST_Handler);
#else
    RamVectorTableUpdate(SDIO0_VECTORn, (IRQ_Fun)SDIO_HOST_Handler);
#endif

    NVIC_InitTypeDef nvic_init_struct;
#if defined TARGET_RTL87X3D
    nvic_init_struct.NVIC_IRQChannel         = SDIO_HOST_IRQn;
#else
    nvic_init_struct.NVIC_IRQChannel         = SDIO0_IRQn;
#endif
    nvic_init_struct.NVIC_IRQChannelPriority = 5;
    nvic_init_struct.NVIC_IRQChannelCmd      = (FunctionalState)newState;
    NVIC_Init(&nvic_init_struct);
}

void SDH_TransCtrlIntConfig(FunctionalState newState)
{
    sd_print_info("SDH_TransCtrlIntConfig: newState 0x%x", newState);

    SDIO_ClearINTPendingBit(SDH_TRANS_INT);
    SDIO_INTStatusConfig(SDH_TRANS_INT, newState);
    SDIO_INTConfig(SDH_TRANS_INT, newState);
}

void SDH_PlugAndErrorIntConfig(void)
{
    /* Enable SDIO interrupt status */
    SDIO_INTStatusConfig(SDIO_CARD_INSERT | SDIO_CARD_REMOVAL, ENABLE);
    SDIO_INTConfig(SDIO_CARD_INSERT | SDIO_CARD_REMOVAL, ENABLE);

    /* Enable all error interrupts */
    SDIO_ErrrorINTStatusConfig(0x3ff, ENABLE);
    SDIO_ErrrorINTConfig(0x3ff, ENABLE);
}

void SDH_SetErrorInt(bool is_enable)
{
    //SDIO_ErrrorINTStatusConfig(0x3ff, is_enable);
    if (is_enable)
    {
#if defined TARGET_RTL87X3D
        NVIC_EnableIRQ(SDIO_HOST_IRQn);
#else
        NVIC_EnableIRQ(SDIO0_IRQn);
#endif
    }
    else
    {
#if defined TARGET_RTL87X3D
        NVIC_DisableIRQ(SDIO_HOST_IRQn);
#else
        NVIC_DisableIRQ(SDIO0_IRQn);
#endif
    }
}

void  SDIO_IrqNorIntCback(T_SDH_NOR_INT_STATUS *int_status)
{
    T_SD_STATUS status = SD_OK;

    if (int_status->INT_TF_CMPL)
    {
        sdh_intr_hdl_cb[SDH_CB_IDX_INT_RW_DONE](sd_cfg.rw_user_data);
        SDIO_ClearINTPendingBit(SDH_TRANS_INT);
    }
    /* Hot plug detection */
    if (int_status->CARD_INSERT)
    {
        status = SD_CARD_INSERT;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CARD_INSERT])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CARD_INSERT]((T_SD_STATUS *)&status);
        }
        SDIO_ClearINTPendingBit(SDIO_CARD_INSERT);
    }

    if (int_status->CARD_REMOVAL)
    {
        status = SD_CARD_REMOVAL;
        SDIO_ClearINTPendingBit(SDIO_CARD_REMOVAL);
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CARD_REMOVAL])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CARD_REMOVAL]((T_SD_STATUS *)&status);
        }
    }
    /* Error interrupt handle */
    if (int_status->INT_ERROR)
    {
        sd_print_error("SDIO_IrqNorIntCback: detect error status");
        return;
    }
}

void SDIO_ErrStatusClearAndRecovery(void)
{
    SDIO_CheckErr_TryRecovery();
    SDIO_ClearErrorINTPendingBit(SDIO_INT_ALL_ERR);
}

void  SDIO_IrqErrIntCback(T_SDH_ERR_INT_STATUS *int_status)
{
    T_SD_STATUS status = SD_OK;


    /*  Index12: Get vendor interrupt error interrupt status */
    if (int_status->INT_VENDOR_SPEC_ERR)
    {
        SDIO_ClearErrorINTPendingBit(SDIO_INT_VENDOR_SPECIFIC_ERR);
    }

    /* Index9: Get ADMA error status */
    if (int_status->INT_ADMA_ERR)
    {
        status = SD_ADMA_ERR;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_ADMA_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_ADMA_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_ADMA_ERR);
    }

    /* Index8: Get auto cmd12 error interrupt status */
    if (int_status->INT_AUTO_CMD12_ERR)
    {
        status = SD_AUTO_CMD12_ERR;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_AUTO_CMD12_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_AUTO_CMD12_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_AUTO_CMD12_ERR);
    }

    /* Index7: Get current limit interrupt status */
    if (int_status->INT_CURRENT_LIMIT_ERR)
    {
        status = SD_CURRENT_LIMIT_ERR;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CURRENT_LIMIT_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CURRENT_LIMIT_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_CURRENT_LIMIT_ERR);
    }

    /* Index6: Get data end bit status */
    if (int_status->INT_DAT_END_BIT_ERR)
    {
        status = SD_DATA_END_BIT_ERR;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_DAT_END_BIT_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_DAT_END_BIT_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_DAT_END_BIT_ERR);
    }

    /* Index5: Get data CRC error status */
    if (int_status->INT_DAT_CRC_ERR)
    {
        status = SD_DATA_CRC_FAIL;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_DAT_CRC_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_DAT_CRC_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_DAT_CRC_ERR);
    }

    /* Index4: Get data line timeout error status */
    if (int_status->INT_DAT_TIMEOUT_ERR)
    {
        status = SD_DATA_TIMEOUT;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_DAT_TIMEOUT_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_DAT_TIMEOUT_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_DAT_TIMEOUT_ERR);
    }

    /* Index3: Get command index status */
    if (int_status->INT_CMD_INDEX_ERR)
    {
        status = SD_CMD_INDEX_ERR;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_INDEX_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_INDEX_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_CMD_INDEX_ERR);
    }

    /* Index2: Get command end bit status */
    if (int_status->INT_CMD_END_BIT_ERR)
    {
        status = SD_CMD_END_BIT_ERR;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_END_BIT_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_END_BIT_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_CMD_END_BIT_ERR);
    }

    /* Index1: Get command CRC error status */
    if (int_status->INT_CMD_CRC_ERR)
    {
        status = SD_CMD_CRC_FAIL;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_CRC_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_CRC_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_CMD_CRC_ERR);
    }

    /* Index0: Get command timeout error status which has higher priority than SDIO_INT_CMD_CMPL */
    if (int_status->INT_CMD_TIMEOUT_ERR)
    {
        status = SD_CMD_RSP_TIMEOUT;
        if (sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_TIMEOUT_ERR])
        {
            sdh_intr_hdl_cb[SDH_CB_IDX_INT_CMD_TIMEOUT_ERR]((T_SD_STATUS *)&status);
        }
        SDIO_ClearErrorINTPendingBit(SDIO_INT_CMD_TIMEOUT_ERR);
    }
}

void SDIO_HOST_Handler(void)
{
    T_SDH_NOR_INT_STATUS nor_int_status ;
    T_SDH_ERR_INT_STATUS err_int_status ;
    sd_print_trace("SDIO_HOST_Handler");
    nor_int_status.d16 = SDIO_GetAllNorINTStatus();

    if (nor_int_status.d16)
    {
        SDIO_IrqNorIntCback(&nor_int_status);
    }
    err_int_status.d16 = SDIO_GetAllErrINTStatus();
    if (err_int_status.d16)
    {
        sdio_err_status = err_int_status.d16;
        sd_print_error("SDIO_HOST_Handler: err_int_status.d16 0x%x", err_int_status.d16);
        SDIO_IrqErrIntCback(&err_int_status);
    }
}

T_SD_STATUS SDIO_CheckErr_TryRecovery(void)
{
    T_SD_STATUS ret = SD_OK;
    uint32_t retry;

#ifndef ASYNC_MODE_ENABLE
    sdio_err_status = SDIO->ERR_INTR_SR;
#endif
    if (sdio_err_status == 0)
    {
        ret = SD_OK;
        goto exit_recovery;
    }

    if (sdio_err_status & 0xf) // D03-00 is set
    {
        SDIO->SW_RESET |= SW_RST_FOR_CMD_LINE_MASK;
        retry = 1000;
        while ((SDIO->SW_RESET & SW_RST_FOR_CMD_LINE_MASK) && retry--);
        if (retry == 0)
        {
            ret = SD_NON_RECOVERABLE_ERROR;
            sd_print_error("SDIO_CheckErr_TryRecovery: SD_NON_RECOVERABLE_ERROR 1");
            goto exit_recovery;
        }
        ret = SD_CMD_LINE_RECOVERABLE_ERROR;
    }

    if (sdio_err_status & 0x70) // D06-04 is set
    {
        SDIO->SW_RESET |= SW_RST_FOR_DAT_LINE_MASK;
        retry = 1000;
        while ((SDIO->SW_RESET & SW_RST_FOR_DAT_LINE_MASK) && retry--);
        if (retry == 0)
        {
            ret = SD_NON_RECOVERABLE_ERROR;
            sd_print_error("SDIO_CheckErr_TryRecovery: SD_NON_RECOVERABLE_ERROR 2");
            goto exit_recovery;
        }
        ret = SD_DAT_LINE_RECOVERABLE_ERROR;
    }

#ifndef ASYNC_MODE_ENABLE
    SDIO->ERR_INTR_SR = sdio_err_status;
#endif
    sdio_err_status = 0;

    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    SDIO_CmdInitStructure.SDIO_Argument     = 0;
    SDIO_CmdInitStructure.SDIO_CmdIndex     = 12;  // STOP_TRANSMISSION
    SDIO_CmdInitStructure.SDIO_CmdType      = NORMAL;
    SDIO_CmdInitStructure.SDIO_DataPresent  = 0;
    SDIO_CmdInitStructure.SDIO_CmdIdxCheck  = ENABLE;
    SDIO_CmdInitStructure.SDIO_CmdCrcCheck  = ENABLE;
    SDIO_CmdInitStructure.SDIO_ResponseType = RSP_LEN_48;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    retry = 385000UL * 20;
    while ((SDIO->PRESENT_STATE & (COMMAND_INHIBIT_DAT_MASK | COMMAND_INHIBIT_CMD_MASK))
           && retry--);
    if (retry == 0)
    {
        ret = SD_NON_RECOVERABLE_ERROR;
        sd_print_error("SDIO_CheckErr_TryRecovery: SD_NON_RECOVERABLE_ERROR 3");
        goto exit_recovery;
    }

#ifndef ASYNC_MODE_ENABLE
    sdio_err_status = SDIO->ERR_INTR_SR;
#endif

    if (sdio_err_status & 0x1f)
    {
        ret = SD_NON_RECOVERABLE_ERROR;
        sd_print_error("SDIO_CheckErr_TryRecovery: SD_NON_RECOVERABLE_ERROR 4");
        goto exit_recovery;
    }

    platform_delay_us(5000);

    if ((SDIO->PRESENT_STATE & DAT3_0_LINE_SIGNAL_LEVEL_MASK) != DAT3_0_LINE_SIGNAL_LEVEL_MASK)
    {
        ret = SD_NON_RECOVERABLE_ERROR;
        sd_print_error("SDIO_CheckErr_TryRecovery: SD_NON_RECOVERABLE_ERROR 5, \
                           SDIO->PRESENT_STATE 0x%x", SDIO->PRESENT_STATE);
        goto exit_recovery;
    }

exit_recovery:
#ifndef ASYNC_MODE_ENABLE
    SDIO->ERR_INTR_SR = sdio_err_status;
#endif
    sdio_err_status = 0;
    return ret;
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

