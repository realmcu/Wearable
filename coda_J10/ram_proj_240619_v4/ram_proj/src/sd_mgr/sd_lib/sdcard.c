/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      sdcard.c
* @brief     This file provides all the sd card protocol firmware functions.
* @details
* @author    elliot chen
* @date      2021-01-28
* @version  v2.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "sdcard.h"
#include "sd_icfg.h"
#include "sdh_prop.h"
#if defined(IC_TYPE_BB2)
//#include "os_mem.h"
#elif defined(IC_TYPE_BBPRO3)
#include <stdlib.h>
#endif

/* Private defines -----------------------------------------------------------*/
#define SD_ADDRESS_NULL                 ((uint32_t)0x00000000)
/* Mask for errors Card Status R1 */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SD_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SD_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SD_OCR_SD_APP_CMD               ((uint32_t)0x00000020)
#define SD_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SD_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)
#define SD_ALLZERO                      ((uint32_t)0x00000000)
/* Card current status */
#define SD_CARD_IDLE                    ((uint32_t)0x00000000)
#define SD_CARD_READY                   ((uint32_t)0x00000001)
#define SD_CARD_IDENT                   ((uint32_t)0x00000002)
#define SD_CARD_STBY                    ((uint32_t)0x00000003)
#define SD_CARD_TRAN                    ((uint32_t)0x00000004)
#define SD_CARD_DATA                    ((uint32_t)0x00000005)
#define SD_CARD_RECEIVING               ((uint32_t)0x00000006)
#define SD_CARD_PROGRAMMING             ((uint32_t)0x00000007)
#define SD_CARD_DIS                     ((uint32_t)0x00000008)

typedef enum
{
    SD_RESP_NO,
    SD_RSP_LEN_136,
    SD_RSP_LEN_48,
    SD_RSP_LEN_48_CHK_BUSY
} T_SD_RSP_TYPE;

/* Global variables ------------------------------------------------------------*/
/* Record card relative address */
uint32_t SD_RCA = 0x03;
/* Card type */
uint8_t SD_Type = SD_TYPE_SDSC;

const T_SDIF_PROT_PROP sd_prot_prop =
{
    SD_InitCard,
    SD_GetCardInfo,
    SD_ReadSingleBlock,
    SD_ReadMultiBlock,
    SD_WriteSingleBlock,
    SD_WriteMultiBlock,
    SD_Earse,
    SD_WaitCardFinishProgram,
    SD_AsyncReadSingleBlock,
    SD_AsyncReadMultiBlock,
    SD_AsyncWriteSingleBlock,
    SD_AsyncWriteMultiBlock,
    SD_SetBlockLen
};

void SD_RegisterFops(T_SDIF_PROT_PROP **pp_sd_if_fops)
{
    *pp_sd_if_fops = (T_SDIF_PROT_PROP *)(&sd_prot_prop);
}

static void delay_us(uint32_t t)
{
    t = t * 2;
    uint32_t i = 0;

    for (i = 0; i < t; ++i)
    {
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
        __asm volatile("nop");
    }
}

/**
  * @brief  Reset SD card.
  * @param  None.
  * @retval T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_ResetCard(void)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_ResetCard: error ret = SD_CMD_INHIBIT_BEFORE_RESET ");
        return SD_CMD_INHIBIT_BEFORE_RESET;
    }

    /* CMD0: GO_IDLE_STATE, ARGUMENT 0, NO DATA, */
    /* NO CHECK CMDIDX, NO CHECK CMD, 0: SD_RESP_NO */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_GO_IDLE_STATE;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = DISABLE;
    sdh_cmd.resp_len        = SD_RESP_NO;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_ResetCard: error ret = SD_SDIO_CMD_INHIBIT_AFTER_RESET ");
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }
    return SD_OK;
}

/**
  * @brief  Checks for error conditions for R1.
  * @param  None.
  * @retval T_SD_STATUS: sd Card status.
  */
static T_SD_STATUS SD_CheckResp1(uint32_t cmd)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: error = SD_SDIO_CMD_INHIBIT_AFTER_RESET ");
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    /* Check response received is of desired command */
    if (p_sd_host_prop->SDH_CheckCmd(cmd) != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: cmd error = SD_CMD_ERROR ");
        return SD_CMD_ERROR;
    }

    /* Check card status */
    uint32_t response = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);

    /* Check error or not */
    if ((response & SD_OCR_ERRORBITS) == SD_ALLZERO)
    {
        return sd_status;
    }

    /* Check specified error */
    if (response & SD_OCR_ADDR_OUT_OF_RANGE)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_ADDR_OUT_OF_RANGE ");
        return SD_ADDR_OUT_OF_RANGE;
    }

    if (response & SD_OCR_ADDR_MISALIGNED)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_ADDR_MISALIGNED ");
        return SD_ADDR_MISALIGNED;
    }

    if (response & SD_OCR_BLOCK_LEN_ERR)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_BLOCK_LEN_ERR ");
        return SD_BLOCK_LEN_ERR;
    }

    if (response & SD_OCR_ERASE_SEQ_ERR)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_ERASE_SEQ_ERR ");
        return SD_ERASE_SEQ_ERR;
    }

    if (response & SD_OCR_BAD_ERASE_PARAM)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_BAD_ERASE_PARAM ");
        return SD_BAD_ERASE_PARAM;
    }

    if (response & SD_OCR_WRITE_PROT_VIOLATION)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_WRITE_PROT_VIOLATION ");
        return SD_WRITE_PROT_VIOLATION;
    }

    if (response & SD_OCR_LOCK_UNLOCK_FAILED)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_LOCK_UNLOCK_FAILED ");
        return SD_LOCK_UNLOCK_FAILED;
    }

    if (response & SD_OCR_COM_CRC_FAILED)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_COM_CRC_FAILED ");
        return SD_COM_CRC_FAILED;
    }

    if (response & SD_OCR_ILLEGAL_CMD)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_ILLEGAL_CMD ");
        return SD_ILLEGAL_CMD;
    }

    if (response & SD_OCR_CARD_ECC_FAILED)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_CARD_ECC_FAILED ");
        return SD_CARD_ECC_FAILED;
    }

    if (response & SD_OCR_CC_ERROR)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_CC_ERROR ");
        return SD_CC_ERROR;
    }

    if (response & SD_OCR_GENERAL_UNKNOWN_ERROR)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_GENERAL_UNKNOWN_ERROR ");
        return SD_GENERAL_UNKNOWN_ERROR;
    }

    if (response & SD_OCR_CID_CSD_OVERWRIETE)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_CID_CSD_OVERWRITE ");
        return SD_CID_CSD_OVERWRITE;
    }

    if (response & SD_OCR_WP_ERASE_SKIP)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_WP_ERASE_SKIP ");
        return SD_WP_ERASE_SKIP;
    }

    if (response & SD_OCR_CARD_ECC_DISABLED)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_CARD_ECC_DISABLED ");
        return SD_CARD_ECC_DISABLED;
    }

    if (response & SD_OCR_ERASE_RESET)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_ERASE_RESET ");
        return SD_ERASE_RESET;
    }

    if (response & SD_OCR_AKE_SEQ_ERROR)
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_AKE_SEQ_ERROR ");
        return SD_AKE_SEQ_ERROR;
    }
    /*  The card will expect ACMD */
    if (!(response & SD_OCR_SD_APP_CMD))
    {
        //SDIO_PRINT_ERROR0("SD_CheckResp1: response error = SD_APP_CMD ");
        return SD_APP_CMD;
    }

    return sd_status;
}

/**
  * @brief  Checks for error conditions for R3.
  * @param  None.
  * @retval T_SD_STATUS: sd Card status.
  */
static T_SD_STATUS SD_CheckResp3(uint32_t cmd)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    /* Check OCR register */
    if (!(p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & BIT(31)))
    {
        sd_status = SD_CARD_BUSY;
    }
    return sd_status;
}

/**
  * @brief  Checks for error conditions for R7.
  * @param  None.
  * @retval T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_CheckResp7(void)
{
    /* Check response */
    if (p_sd_host_prop->SDH_CheckCmd(SDIO_SEND_IF_COND) != SD_OK)
    {
        return SD_CMD_ERROR;
    }
    /* Check Pattern error or not */
    if ((p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & 0xff) != 0xAA)
    {
        return SD_RSP_PATTERN_ERROR;
    }
    /* Check Supply Voltage (VHS) 0x1 */
    if (!(p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & BIT(8)))
    {
        return SD_RSP_VHS_ERROR;
    }

    return SD_OK;
}

/**
  * @brief  Get OCR
  * @param  None.
  * @retval T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_GetOCR(void)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_CMD_INHIBIT_BEFORE_RESET;
    }

    /* CMD1: SDIO_SEND_OP_COND */
    /* CMD: SDIO_SEND_OP_COND, ARG: 0xc0ff8080, */
    /* 0: NO DATA, 0: NO CHECK CMD IDX, 0: NO CHECK CMD CRC, 2: SD_RSP_LEN_48 */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_OP_COND;
    sdh_cmd.argument        = 0xc0ff8080;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = DISABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    return SD_CheckResp3(SDIO_SEND_OP_COND);
}

/**
  * @brief  Get CID.
  * @param  none.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_GetCID(uint32_t *pBuf)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_ALL_SEND_CID, ARG: 0, 0: NO DATA, */
    /* 0: NO CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 1(SD_RSP_LEN_136) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_ALL_SEND_CID;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = DISABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_136;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Store CID */
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP2);
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP4);
    *pBuf   = p_sd_host_prop->SDH_GetResponse(SDH_RSP6);

    return SD_OK;
}

/**
  * @brief  Set RCA.
  * @param  addr: relative card address.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_SetRCA(uint32_t addr)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_SET_REL_ADDR, ARG: addr << 16, 0: NO DATA, */
    /* 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SET_REL_ADDR;
    sdh_cmd.argument        = addr << 16;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    sd_status = SD_CheckResp1(SDIO_SET_REL_ADDR);
    return sd_status;
}

/**
  * @brief  Set RCA.
  * @param  addr: relative card address.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_GetRCA(uint32_t *RCA)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_SET_REL_ADDR, ARG: 0, 0: NO DATA, */
    /* 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SET_REL_ADDR;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    uint32_t rsp_data = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);
    *RCA = rsp_data >> 16;

    return sd_status;
}

/**
  * @brief  Get card-specific data.
  * @param  addr: relative acrd address.
  * @param  pBuf: address store CSD when function return.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_GetCSD(uint32_t addr, uint32_t *pBuf)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_SEND_CSD, ARG: addr << 16, 0: NO DATA, */
    /* 0: NO CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 1(SD_RSP_LEN_136) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_CSD;
    sdh_cmd.argument        = addr << 16;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_136;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Get csd */
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP2);
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP4);
    *pBuf   = p_sd_host_prop->SDH_GetResponse(SDH_RSP6);

    return SD_OK;
}

/**
  * @brief  send cid.
  * @param  addr: relative acrd address.
  * @param  pBuf: address store CSD when function return.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_SendCID(uint32_t addr, uint32_t *pBuf)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_SEND_CID, ARG: addr << 16, 0: NO DATA, */
    /* 0: NO CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 1(SD_RSP_LEN_136) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_CID;
    sdh_cmd.argument        = addr << 16;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_136;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Get csd */
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP2);
    *pBuf++ = p_sd_host_prop->SDH_GetResponse(SDH_RSP4);
    *pBuf   = p_sd_host_prop->SDH_GetResponse(SDH_RSP6);

    return SD_OK;
}
/**
  * @brief  Select or deselect the eMMC card.
  * @param  NewState: new state of the selection.
  *   This parameter can be: ENABLE or DISABLE.
  *   --ENABLE: select
  *   --DISABLE: deselect
  * @retval  T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_SelectCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    T_SDH_CMD_CFG sdh_cmd;

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Send CMD7 */
    if (NewState != DISABLE)
    {
        /* CMD: SDIO_SEL_DESEL_CARD, ARG: SD_RCA << 16, */
        /* 0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 3(SD_RSP_LEN_48_CHK_BUSY) */
        sdh_cmd.cmd             = SDIO_SEL_DESEL_CARD;
        sdh_cmd.argument        = SD_RCA << 16;
        sdh_cmd.with_data       = 0;
        sdh_cmd.cmd_idx_check   = ENABLE;
        sdh_cmd.cmd_crc_check   = ENABLE;
        sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    }
    else
    {
        /* CMD: SDIO_SEL_DESEL_CARD, ARG: 0,
        0: NO DATA, 0: NO CHECK CMD IDX, 0: NO CHECK CMD CRC, RSP: 0(SD_RESP_NO) */
        sdh_cmd.cmd             = SDIO_SEL_DESEL_CARD;
        sdh_cmd.argument        = 0;
        sdh_cmd.with_data       = 0;
        sdh_cmd.cmd_idx_check   = DISABLE;
        sdh_cmd.cmd_crc_check   = DISABLE;
        sdh_cmd.resp_len        = SD_RESP_NO;
    }
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    /* Check response */
    if (NewState != DISABLE)
    {
        if (p_sd_host_prop->SDH_CheckCmd(SDIO_SEL_DESEL_CARD) != SD_OK)
        {
            return SD_CMD_ERROR;
        }
    }

    return SD_OK;
}


/**
  * @brief  Get SD card current status.
  * @param  pBuf: address store current status when function return.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_GetCardStatus(uint32_t *pStatus)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }
    /* CMD13: Get SD card current status */
    /* CMD: SDIO_SEND_STATUS, ARG: SD_RCA << 16,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_STATUS;
    sdh_cmd.argument        = SD_RCA << 16;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }
    if (p_sd_host_prop->SDH_CheckCmd(SDIO_SEND_STATUS) != SD_OK)
    {
        return SD_CMD_ERROR;
    }
    *pStatus = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);

    return SD_OK;
}

/**
  * @brief  Checks if the SD card is in programming state.
  * @param  pStatus: address store current status of card.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_WaitCardFinishProgram(void)
{
    T_SD_STATUS sd_status = SD_OK;
    uint32_t retry = 0x01;
    uint32_t card_status = 0;

    /* Get card status */
    sd_status = SD_GetCardStatus(&card_status);
    /* Find out programming status */
    card_status = (card_status >> 9) & 0x0F;

    /* Wait finish */
    while ((sd_status == SD_OK) && \
           ((card_status == SD_CARD_PROGRAMMING) || (card_status == SD_CARD_RECEIVING)))
    {
        if (retry == 0)
        {
            return SD_PROG_TIMEOUT;
        }
        retry--;

        /* Get card status */
        sd_status = SD_GetCardStatus(&card_status);
        /* Find out programming status */
        card_status = (card_status >> 9) & 0x0F;
    }

    return sd_status;
}

/**
  * @brief  Control SD card into inactive status.
  * @param  addr: relative acrd address.
  * @retval T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_GoInactiveState(void)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_CMD_INHIBIT_BEFORE_RESET;
    }

    /* CMD15: GO_IDLE_STATE */
    /* CMD: SDIO_GO_INACTIVE_STATE, ARG: SD_RCA << 16,
    0: NO DATA, 0: NO CHECK CMD IDX, 0: NO CHECK CMD CRC, RSP: 0(SD_RESP_NO) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_GO_INACTIVE_STATE;
    sdh_cmd.argument        = SD_RCA << 16;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = DISABLE;
    sdh_cmd.resp_len        = SD_RESP_NO;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    return SD_OK;
}

/**
  * @brief  Control eMMC card into sleep or standby status.
  * @param  NewState: new state of the selection.
  *   This parameter can be: ENABLE or DISABLE.
  *   --ENABLE: control eMMC card into sleep status.
  *   --DISABLE: control eMMC card into awake status
  * @retval T_SD_STATUS: eMMC status.
  */
T_SD_STATUS SD_SleepOrAwakeCmd(FunctionalState NewState)
{
    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD5, response type: R1b */
    /* CMD: SDIO_SLEEP_AWAKE, ARG: SD_RCA << 16 | ((uint32_t)NewState << 15),
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 3(SD_RSP_LEN_48_CHK_BUSY) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SLEEP_AWAKE;
    sdh_cmd.argument        = SD_RCA << 16 | ((uint32_t)NewState << 15);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    return SD_CheckResp1(SDIO_SET_REL_ADDR);
}

/**
  * @brief CSD[3:0] <<= 8. Refer to SD Host Controller Standard Specification V2.00 P31
  * @param None.
  * @retval None.
  */
static void SD_HandleCSDData(T_SD_DEV_INFO *p_dev_info)
{
    uint8_t i = 0;

    i = sizeof(p_dev_info->CSD_Tab) / sizeof(p_dev_info->CSD_Tab[0]) - 1;
    while (i)
    {
        p_dev_info->CSD_Tab[i] = p_dev_info->CSD_Tab[i] << 8;
        p_dev_info->CSD_Tab[i] |= p_dev_info->CSD_Tab[i - 1] >> 24;
        i--;
    }

    p_dev_info->CSD_Tab[0] = (p_dev_info->CSD_Tab[0] << 8) & 0xFFFFFF00;
}

/**
  * @brief  Returns information about specific card.
  * @param  RCA: relative acrd address.
  * @param  p_dev_info : pointer to a SD_CardInfo structure.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_GetCardInfo(void *p_info)
{
    T_SD_DEV_INFO *p_dev_info = (T_SD_DEV_INFO *)p_info;
    T_SD_STATUS sd_status = SD_OK;
    uint8_t tmp = 0;

    SD_HandleCSDData(p_dev_info);
    p_dev_info->rca = (uint16_t)SD_RCA;
    /* Byte 0 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[3] & 0xFF000000) >> 24);
    p_dev_info->dev_csd.CSDStruct = (tmp & 0xC0) >> 6;
    p_dev_info->dev_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
    p_dev_info->dev_csd.Reserved1 = tmp & 0x03;
    // APP_PRINT_INFO("p_dev_info->dev_csd.CSDStruct", p_dev_info->dev_csd.CSDStruct);

    /* Byte 1 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[3] & 0x00FF0000) >> 16);
    p_dev_info->dev_csd.TAAC = tmp;

    /* Byte 2 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[3] & 0x0000FF00) >> 8);
    p_dev_info->dev_csd.NSAC = tmp;

    /* Byte 3 */
    tmp = (uint8_t)(p_dev_info->CSD_Tab[3] & 0x000000FF);
    p_dev_info->dev_csd.MaxBusClkFrec = tmp;

    /* Byte 4 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[2] & 0xFF000000) >> 24);
    p_dev_info->dev_csd.CardComdClasses = tmp << 4;

    /* Byte 5 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[2] & 0x00FF0000) >> 16);
    p_dev_info->dev_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
    p_dev_info->dev_csd.RdBlockLen = tmp & 0x0F;

    /* Byte 6 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[2] & 0x0000FF00) >> 8);
    p_dev_info->dev_csd.PartBlockRead = (tmp & 0x80) >> 7;
    p_dev_info->dev_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
    p_dev_info->dev_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
    p_dev_info->dev_csd.DSRImpl = (tmp & 0x10) >> 4;
    p_dev_info->dev_csd.Reserved2 = 0; /* Reserved */
    if (SD_TYPE_SDSC == SD_Type)
    {
        p_dev_info->dev_csd.DeviceSize = (tmp & 0x03) << 10;
    }

    if (SD_TYPE_SDSC == SD_Type)
    {
        p_dev_info->card_type = SD_TYPE_SDSC;
        /* Byte 7 */
        tmp = (uint8_t)(p_dev_info->CSD_Tab[2] & 0x000000FF);
        p_dev_info->dev_csd.DeviceSize |= (tmp) << 2;
        /* Byte 8 */
        tmp = (uint8_t)((p_dev_info->CSD_Tab[1] & 0xFF000000) >> 24);
        p_dev_info->dev_csd.DeviceSize |= (tmp & 0xC0) >> 6;
        p_dev_info->dev_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
        p_dev_info->dev_csd.MaxRdCurrentVDDMax = (tmp & 0x07);
        /* Byte 9 */
        tmp = (uint8_t)((p_dev_info->CSD_Tab[1] & 0x00FF0000) >> 16);
        p_dev_info->dev_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
        p_dev_info->dev_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
        p_dev_info->dev_csd.DeviceSizeMul = (tmp & 0x03) << 1;
        /* Byte 10 */
        tmp = (uint8_t)((p_dev_info->CSD_Tab[1] & 0x0000FF00) >> 8);
        p_dev_info->dev_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
    }
    else
    {
        p_dev_info->card_type = SD_TYPE_SDHC_OR_SDXC;
        /* Byte 7 */
        p_dev_info->dev_csd.DeviceSize = (uint8_t)(p_dev_info->CSD_Tab[2] & 0x0000003F);
        p_dev_info->dev_csd.DeviceSize <<= 8;
        /* Byte 8 */
        tmp = (p_dev_info->CSD_Tab[1] & 0xFF000000) >> 24;
        p_dev_info->dev_csd.DeviceSize |= tmp;
        p_dev_info->dev_csd.DeviceSize <<= 8;

        /* Byte 9*/
        p_dev_info->dev_csd.DeviceSize |= (p_dev_info->CSD_Tab[1] & 0x00FF0000) >> 16;
    }
    p_dev_info->capacity = (p_dev_info->dev_csd.DeviceSize + 1) ;
    if (SD_TYPE_SDSC == SD_Type)
    {
        p_dev_info->capacity *= (1 << (p_dev_info->dev_csd.DeviceSizeMul + 2));
        p_dev_info->block_size = 1 << (p_dev_info->dev_csd.RdBlockLen);
        p_dev_info->capacity *= p_dev_info->block_size;
    }
    else
    {
        p_dev_info->block_size = 512;
        p_dev_info->capacity = p_dev_info->dev_csd.DeviceSize * 512 * 1024LL;
    }

    //SDIO_PRINT_INFO5("SD_GetCardInfo: DeviceSize: 0x%x, DeviceSizeMul: 0x%x, RdBlockLen: 0x%x, capacity: 0x%x, block_size: 0x%x",
//                     p_dev_info->dev_csd.DeviceSize, p_dev_info->dev_csd.DeviceSizeMul, p_dev_info->dev_csd.RdBlockLen,
//                     p_dev_info->capacity, p_dev_info->block_size);
    /* Byte 10 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[1] & 0x0000FF00) >> 8);
    p_dev_info->dev_csd.EraseGrSize = (tmp & 0x7C) >> 2;
    p_dev_info->dev_csd.EraseGrMul = (tmp & 0x03) << 2;

    /* Byte 11 */
    tmp = (uint8_t)(p_dev_info->CSD_Tab[1] & 0x000000FF);
    p_dev_info->dev_csd.EraseGrMul |= (tmp & 0xE0) >> 5;
    p_dev_info->dev_csd.WrProtectGrSize = (tmp & 0x1F);

    /* Byte 12 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[0] & 0xFF000000) >> 24);
    p_dev_info->dev_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
    p_dev_info->dev_csd.ManDeflECC = (tmp & 0x60) >> 5;
    p_dev_info->dev_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
    p_dev_info->dev_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

    /* Byte 13 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[0] & 0x00FF0000) >> 16);
    p_dev_info->dev_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
    p_dev_info->dev_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
    p_dev_info->dev_csd.Reserved3 = 0;
    p_dev_info->dev_csd.ContentProtectAppli = (tmp & 0x01);

    /* Byte 14 */
    tmp = (uint8_t)((p_dev_info->CSD_Tab[0] & 0x0000FF00) >> 8);
    p_dev_info->dev_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
    p_dev_info->dev_csd.CopyFlag = (tmp & 0x40) >> 6;
    p_dev_info->dev_csd.PermWrProtect = (tmp & 0x20) >> 5;
    p_dev_info->dev_csd.TempWrProtect = (tmp & 0x10) >> 4;
    p_dev_info->dev_csd.FileFormat = (tmp & 0x0C) >> 2;
    p_dev_info->dev_csd.ECC = (tmp & 0x03);

    /* Byte 15 */
    tmp = (uint8_t)(p_dev_info->CSD_Tab[0] & 0x000000FF);
    p_dev_info->dev_csd.CSD_CRC = (tmp & 0xFE) >> 1;
    p_dev_info->dev_csd.Reserved4 = 1;
    return sd_status;
}

/**
  * @brief  CMD8
  * @param  addr: relative card address.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_SendIfCond(void)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {

        //SDIO_PRINT_ERROR0("SD_SendIfCond: SD_SDIO_CMD_INHIBIT");
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_SEND_IF_COND, ARG: 0x1AA,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_IF_COND;
    sdh_cmd.argument        = 0x1AA;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_SendIfCond: SD_SDIO_CMD_INHIBIT_AFTER_RESET");
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    sd_status = SD_CheckResp7();
    return sd_status;
}

/**
  * @brief  Send APP CMD.
  * @param  none.
  * @retval  T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_SendAppCmd(uint32_t RCA)
{
    /* CMD55, command response: R1 */
    /* CMD: SDIO_APP_CMD, ARG: RCA << 16,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_APP_CMD;
    sdh_cmd.argument        = RCA << 16;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    return SD_CheckResp1(SDIO_APP_CMD);
}

/**
  * @brief  ACMD41
  * @param  addr: relative card address.
  * @retval  T_SD_STATUS: SD status.
  */
/* Operating Condition (ACMD41) */
#define ACMD41_POLL_INTERVAL            10000UL
#define ACMD41_INIT_TIMEOUT             100000000UL
#define SWITCH_TO_1V8                   (1<<24)
T_SD_STATUS SD_SendOpCond(void)
{
    T_SD_STATUS sd_status = SD_OK;
    uint32_t retry = (385000UL * 1000 / SD_MCU_CLOCK); //(ACMD41_INIT_TIMEOUT / ACMD41_POLL_INTERVAL);
    uint32_t HostOCR = SD_VDD_30_31 | SD_VDD_31_32 | SD_VDD_32_33 | SD_VDD_33_34;
    uint32_t Hcs = SDHC_SUPPORT;
    uint32_t S18R = 0;//SWITCH_TO_1V8;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_SendOpCond: SD_SDIO_CMD_INHIBIT");
        return SD_SDIO_CMD_INHIBIT;
    }
    /* CMD55: Send SDIO_APP_CMD and check response */
    sd_status = SD_SendAppCmd(0);
    if (sd_status != SD_OK)
    {
        return sd_status;
    }

    /* ACMD41: SDIO_CMD_SEND_OP_COND, ARG: 0,
    0: NO DATA, 0: NO CHECK CMD IDX, 0: NO CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_CMD_SEND_OP_COND;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = DISABLE;
    sdh_cmd.cmd_crc_check   = DISABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_SendOpCond: SD_SDIO_CMD_INHIBIT");
        return SD_SDIO_CMD_INHIBIT;
    }
    uint32_t OCR = p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & 0xFFFFFF;
    p_sd_host_prop->SDH_GetResponse(SDH_RSP2);

    while (--retry)
    {
        /* Send SDIO_APP_CMD and check response */
        sd_status = SD_SendAppCmd(0);
        if (sd_status != SD_OK)
        {
            return sd_status;
        }

        /* Inquiry ACMD41 */
        /* CMD: SDIO_CMD_SEND_OP_COND, ARG: (Hcs << 30) | HostOCR | S18R,
        0: NO DATA, 0: NO CHECK CMD IDX, 0: NO CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
        T_SDH_CMD_CFG sdh_cmd;
        sdh_cmd.cmd             = SDIO_CMD_SEND_OP_COND;
        sdh_cmd.argument        = (Hcs << 30) | HostOCR | S18R;
        sdh_cmd.with_data       = 0;
        sdh_cmd.cmd_idx_check   = DISABLE;
        sdh_cmd.cmd_crc_check   = DISABLE;
        sdh_cmd.resp_len        = SD_RSP_LEN_48;
        p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

        /* Waiting for CMD line idle */
        if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
        {
            return SD_SDIO_CMD_INHIBIT;
        }
        /* Check SD card status */
        if (p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & BIT(31))
        {
            if (p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & BIT30)
            {
                SD_Type = SD_TYPE_SDHC_OR_SDXC;
                //SDIO_PRINT_ERROR0("SD_SendOpCond: SD type = SD_TYPE_SDHC_OR_SDXC");
            }
            else
            {
                SD_Type = SD_TYPE_SDSC;
                //SDIO_PRINT_ERROR0("SD_SendOpCond: SD type = SD_TYPE_SDSC");
            }
            break;
        }
        p_sd_host_prop->SDH_GetResponse(SDH_RSP2);
    }

    return sd_status;

    /* Check response */
    // return SD_CheckResp3(SDIO_SEND_OP_COND);
}
#include "sd.h"
/**
  * @note RawCSDTab is the csd data before invoking SD_HandleCSDData()
  */
T_SD_STATUS SD_SetHighSpeedMode(FunctionalState NewState, const uint32_t RawCSDTab[4])
{
    T_SD_STATUS sd_status = SD_OK;
    T_CMD6_ARG arg = {.d32 = 0};

    if (NewState != ENABLE) // TD: change to default speed mode
    {
        return SD_OK;
    }

    uint32_t ccc = (RawCSDTab[2] & (0xfff00000UL >> 8)) >> 12;
    if (!(ccc & BIT10)) // not support cmd6
    {
        //SDIO_PRINT_INFO0("SD_SetHighSpeedMode: not support cmd6");
        return SD_OK;
    }

#if defined(IC_TYPE_BB2)
    // uint8_t *pbuf = os_mem_alloc(OS_MEM_TYPE_BUFFER, 512 / 8); // cmd 6 will return 512 bits data
    uint8_t pbuf[64] = {0};

#elif defined(IC_TYPE_BBPRO3)
    uint8_t *pbuf = malloc(512 / 8);
#endif
//    if (pbuf == NULL)
//    {
//        return SD_OK;  // swicthing to high speed mode is not mandatory
//    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = true;
    sdh_data.buf         = (uint32_t)pbuf;
    sdh_data.blk_size    = 512 / 8;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_SetHighSpeedMode: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
//                          __LINE__);
        goto clean_up;
    }

    /* CMD6 */
    arg.d32 = 0;
    arg.b.AcessMode = 0x1;
    arg.b.CommandSystem = 0xf;
    arg.b.DriveStrength = 0xf;
    arg.b.PowerLimit = 0xf;
    arg.b.RsvdForGroup5 = 0xf;
    arg.b.RsvdForGroup6 = 0xf;
    arg.b.Mode = 0;

    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SWITCH;
    sdh_cmd.argument        = arg.d32;
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    sd_print_binary_data(pbuf, 64);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_SetHighSpeedMode: err sd_status = 0x%x, line:%d", sd_status, __LINE__);
        goto clean_up;
    }

    if ((pbuf[16] & 0xf) != 0x1)
    {
        //SDIO_PRINT_INFO0("SD_SetHighSpeedMode: not support high speed mode");
        goto clean_up;
    }

    /* Data Configure */
    sdh_data.is_read_dir = true;
    sdh_data.buf         = (uint32_t)pbuf;
    sdh_data.blk_size    = 512 / 8;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_SetHighSpeedMode: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
//                          __LINE__);
        goto clean_up;
    }

    /* CMD6 */
    arg.d32 = 0;
    arg.b.AcessMode = 0x1;
    arg.b.CommandSystem = 0xf;
    arg.b.DriveStrength = 0xf;
    arg.b.PowerLimit = 0xf;
    arg.b.RsvdForGroup5 = 0xf;
    arg.b.RsvdForGroup6 = 0xf;
    arg.b.Mode = 1;

    sdh_cmd.cmd             = SDIO_SWITCH;
    sdh_cmd.argument        = arg.d32;
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_SetHighSpeedMode: err sd_status = 0x%x, line:%d", sd_status, __LINE__);
        goto clean_up;
    }

clean_up:
#if defined(IC_TYPE_BB2)
    //  os_mem_free(pbuf);
#elif defined(IC_TYPE_BBPRO3)
    free(pbuf);
#endif
    return sd_status;
}

/**
  * @brief  Configure bus width.
  * @retval T_SD_STATUS: SD status.
  */
T_SD_STATUS SD_SetBusWide(uint32_t bus_width)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD6 */
    /* CMD: SDIO_SWITCH, ARG: bus_width,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 3(SD_RSP_LEN_48_CHK_BUSY) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SWITCH;
    sdh_cmd.argument        = bus_width;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Check response */
    if (p_sd_host_prop->SDH_CheckCmd(SDIO_SWITCH) != SD_OK)
    {
        return SD_CMD_ERROR;
    }

    /* Set host controller bus width */
    sd_status = p_sd_host_prop->SDH_SetBusWide(bus_width);

    return sd_status;
}

/**
  * @brief  Initial SD device.
  * @param  p_dev: SD device structure pointer.
  * @retval T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_InitCard(void *p_dev)
{
    T_SD_DEV_INFO *p_dev_info = (T_SD_DEV_INFO *)p_dev;
    T_SD_STATUS sd_status = SD_OK;

    /*Initialization delay:the longest of: 1ms, 74 clock cycles */
    /* the supply ramp up time, or the boot operation time*/
    delay_us(50000);
    /* CMD0: GO_IDLE_STATE */
    sd_status = SD_ResetCard();
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_ResetCard error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* CMD8: Send SD Memory Card interface condition */
    sd_status = SD_SendIfCond();
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: CMD8 reponse error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* ACMD41: identify the operating voltage of the card send CMD55 first */
    sd_status = SD_SendOpCond();
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_SendOpCond CMD41 error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* CMD2: Get any card connect to the host to send the CID */
    sd_status = SD_GetCID(p_dev_info->CID_Tab);
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_GetCID error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* CMD3: Get RCA */
    sd_status = SD_GetRCA(&SD_RCA);
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_GetRCA error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* CMD9: Get CSD */
    sd_status = SD_GetCSD(SD_RCA, p_dev_info->CSD_Tab);
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_GetCSD error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* CMD10: Addressed card sends its card identification(CID) */
    sd_status = SD_SendCID(SD_RCA, p_dev_info->CID_Tab);
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_SendCID error, sd_status:%d", sd_status);
        return sd_status;
    }

    /* CMD7 is uesd to put card into the transfer state */
    sd_status = SD_SelectCmd(ENABLE);
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_SelectCmd error, sd_status:%d", sd_status);
        return sd_status;
    }

    sd_status = SD_SetHighSpeedMode(ENABLE, p_dev_info->CSD_Tab);
    if (sd_status != SD_OK)
    {
        DBG_DIRECT("SD_InitCard: SD_SetHighSpeedMode error, sd_status:%d", sd_status);
        return sd_status;
    }

    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        /* ACMD6 : Set to 4-bit mode */
        sd_status = SD_SendAppCmd(SD_RCA);
        if (sd_status != SD_OK)
        {
            DBG_DIRECT("SD_InitCard: SD_SendAppCmd error, sd_status:%d", sd_status);
            return sd_status;
        }

        sd_status = SD_SetBusWide(SD_BUS_WIDTH_4B);
        if (sd_status != SD_OK)
        {
            DBG_DIRECT("SD_InitCard: SD_SetBusWide error, sd_status:%d", sd_status);
            return sd_status;
        }
    }

    p_dev_info->initialized = true;
    //SDIO_PRINT_INFO0("SD_InitCard: status: SD_OK");
    return sd_status;
}

/**
  * @brief  Set block length for standard capacity SD card.
  * @param  block_len:  block length.
  * @retval  T_SD_STATUS: execution status.
  */
T_SD_STATUS SD_SetBlockLen(uint32_t block_len)
{
    T_SD_STATUS sd_status = SD_OK;

    /* CMD16 */
    if (SD_TYPE_SDSC == SD_Type)
    {
        /* CMD: SDIO_SET_BLOCKLEN, ARG: block_len,
        0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
        T_SDH_CMD_CFG sdh_cmd;
        sdh_cmd.cmd             = SDIO_SET_BLOCKLEN;
        sdh_cmd.argument        = block_len;
        sdh_cmd.with_data       = 0;
        sdh_cmd.cmd_idx_check   = ENABLE;
        sdh_cmd.cmd_crc_check   = ENABLE;
        sdh_cmd.resp_len        = SD_RSP_LEN_48;
        p_sd_host_prop->SDH_SendCommand(&sdh_cmd);
        /* Check response */
        sd_status = SD_CheckResp1(SDIO_SET_BLOCKLEN);
        if (sd_status != SD_OK)
        {
            //SDIO_PRINT_ERROR2("SD_SetBlockLen: sd_status = 0x%x, line:%d", sd_status, __LINE__);
            return sd_status;
        }
    }
    return sd_status;
}

/**
  * @brief  Read one block from a specified address in a card.
  * @param  sector: sector from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain the received data
  * @param  blockSize: the SD card Data block size
  * @retval  SD_Error: SD Card status.
  */
T_SD_STATUS SD_ReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = true;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_ReadSingleBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                  __LINE__);
        return sd_status;
    }

    /* CMD17 */
    /* CMD: SDIO_READ_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_SINGLE_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_ReadSingleBlock: err sd_status = 0x%x, line:%d", sd_status, __LINE__);
    }

    return sd_status;
}

/**
  * @brief  Read multiple block from a specified address in a card.
  * @param  addr: Address from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain the received data
  * @param  blockSize: the SD card Data block size
  * @retval  T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_ReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                              uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        return SD_INVALID_PARAMETER;
    }

    /* Data Configure blockCount is limit to 65536 Byte*/
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = true;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = block_count;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_ReadMultiBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                 __LINE__);
        return sd_status;
    }

    /* CMD18 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_READ_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_MULT_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_ReadMultiBlock: err sd_status = 0x%x, line:%d", sd_status, __LINE__);
    }

    return sd_status;
}

/**
  * @brief  Read one block from a specified address in a card.
  * @param  addr: Address from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain data to be written.
  * @param  blockSize: the SD card Data block size
  * @retval  SD_Error: SD Card Error code.
  */
T_SD_STATUS SD_WriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = false;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_WriteSingleBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                 __LINE__);
        return sd_status;
    }

    /* CMD24 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_SINGLE_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_WriteSingleBlock: err sd_status = 0x%x, line:%d", sd_status, __LINE__);
        return sd_status;
    }

    /* Check block whether write protection */
    if (p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & SD_OCR_WRITE_PROT_VIOLATION)
    {
        //SDIO_PRINT_ERROR0("SD_WriteSingleBlock: err rsp = sd_write_protection_violation");
        return SD_WRITE_PROT_VIOLATION;
    }

    return sd_status;
}

/**
  * @brief  Read multiple block from a specified address in a card.
  * @param  addr: Address from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain data to be written.
  * @param  blockSize: the SD card Data block size
  * @retval  SD_Error: SD Card Error code.
  */
T_SD_STATUS SD_WriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                               uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = false;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = block_count;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_WriteMultiBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                 __LINE__);
        return sd_status;
    }

    /* CMD25 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_MULT_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_WriteMultiBlock: err sd_status = 0x%x, line:%d", sd_status, __LINE__);
        return sd_status;
    }

    /* Check block whether write protection */
    if (p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & SD_OCR_WRITE_PROT_VIOLATION)
    {
        //SDIO_PRINT_ERROR0("SD_WriteMultiBlock: err res = sd_write_protection_violation");
        return SD_WRITE_PROT_VIOLATION;
    }

    return sd_status;
}

/**
  * @brief  Earse SD card.
  * @param  startAddr:  Start address which need to erase,the addr's unit is blocksize.
  * @param  endAddr: End address which need to erase.
  * @retval  T_SD_STATUS: execution status.
  */
T_SD_STATUS SD_Earse(uint32_t start_addr, uint32_t end_addr)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD32 */
    /* CMD: SDIO_SD_ERASE_GRP_START, ARG: start_addr or start_addr * 512,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SD_ERASE_GRP_START;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? start_addr : (start_addr * 512);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    sd_status = SD_CheckResp1(SDIO_SD_ERASE_GRP_START);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_Earse: sd_status = 0x%x, line:%d", sd_status, __LINE__);
        return sd_status;
    }

    /* CMD33 */
    /* CMD: SDIO_SD_ERASE_GRP_END, ARG: end_addr or end_addr * 512,
        0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    sdh_cmd.cmd             = SDIO_SD_ERASE_GRP_END;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? end_addr : (end_addr * 512);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    sd_status = SD_CheckResp1(SDIO_SD_ERASE_GRP_END);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_Earse: sd_status = 0x%x, line:%d", sd_status, __LINE__);
        return sd_status;
    }

    /* CMD38 */
    /* CMD: SDIO_ERASE, ARG: 0,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 3(SD_RSP_LEN_48_CHK_BUSY) */
    sdh_cmd.cmd             = SDIO_ERASE;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        //SDIO_PRINT_ERROR0("SD_Earse: CMD line is not idle");
        return SD_SDIO_CMD_INHIBIT;
    }

    return sd_status;
}

/**
  * @brief  Stop transfer.
  * @param  none.
  * @retval  T_SD_STATUS: execution status.
  */
T_SD_STATUS SD_StopTransfer(void)
{
    T_SD_STATUS sd_status = SD_OK;

    /* CMD12, command response: R1b */
    /* CMD: SDIO_STOP_TRANSMISSION, ARG: 0,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_STOP_TRANSMISSION;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    sd_status = SD_CheckResp1(SDIO_STOP_TRANSMISSION);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_StopTransfer: sd_status = 0x%x, line:%d", sd_status, __LINE__);
    }
    return sd_status;
}

/**
  * @brief  Read one block from a specified address in a card.
  * @param  sector: sector from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain the received data
  * @param  blockSize: the  card Data block size
  * @retval  SD_Error: SD Card status.
  */
T_SD_STATUS SD_AsyncReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = true;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_AsyncReadSingleBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                    __LINE__);
        return sd_status;
    }

    /* CMD17 */
    /* CMD: SDIO_READ_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_SINGLE_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    return sd_status;
}

/**
  * @brief  Read multiple block from a specified address in a card.
  * @param  addr: Address from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain the received data
  * @param  blockSize: the SD card Data block size
  * @retval  T_SD_STATUS: SD Card status.
  */
T_SD_STATUS SD_AsyncReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                   uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        return SD_INVALID_PARAMETER;
    }

    /* Data Configure blockCount is limit to 65536 Byte*/
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = true;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = block_count;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_AsyncReadMultiBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                   __LINE__);
        return sd_status;
    }

    /* CMD18 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_READ_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_MULT_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    return sd_status;
}

/**
  * @brief  Read one block from a specified address in a card.
  * @param  addr: Address from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain data to be written.
  * @param  blockSize: the SD card Data block size
  * @retval  SD_Error: SD Card Error code.
  */
T_SD_STATUS SD_AsyncWriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = false;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_AsyncWriteSingleBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //                   __LINE__);
        return sd_status;
    }

    /* CMD24 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_SINGLE_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    return sd_status;
}

/**
  * @brief  Read multiple block from a specified address in a card.
  * @param  addr: Address from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain data to be written.
  * @param  blockSize: the SD card Data block size
  * @retval  SD_Error: SD Card Error code.
  */
T_SD_STATUS SD_AsyncWriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                    uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (SD_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = false;
    sdh_data.buf         = buf;
    sdh_data.blk_size    = block_size;
    sdh_data.blk_count   = block_count;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        //SDIO_PRINT_ERROR2("SD_AsyncWriteMultiBlock: SDH_DataConfig, eror status= 0x%x, line:%d", sd_status,
        //            __LINE__);
        return sd_status;
    }

    /* CMD25 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_MULT_BLOCK;
    sdh_cmd.argument        = (SD_TYPE_SDHC_OR_SDXC == SD_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    return sd_status;
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
