/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file      mmc.c
* @brief     This file provides all the mmc card protocol firmware functions.
* @details
* @author    elliot chen
* @date      2021-04-15
* @version   v2.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mmc.h"
#include "sd.h"
#include "sdh_prop.h"
#ifdef TARGET_RTL87X3E
#include "os_mem.h"
#else
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portable.h"
#endif

/* Private defines -----------------------------------------------------------*/
#define GET_OCR_TIMEOUT                  (287*SD_CMD_SW_TIMEOUT/SD_MCU_CLOCK)
#define MMC_ADDRESS_NULL                 ((uint32_t)0x00000000)
/* Mask for errors Card Status R1 */
#define MMC_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define MMC_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define MMC_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define MMC_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define MMC_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define MMC_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define MMC_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define MMC_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define MMC_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define MMC_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define MMC_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define MMC_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define MMC_OCR_CID_CSD_OVERWRIETE       ((uint32_t)0x00010000)
#define MMC_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define MMC_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define MMC_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define MMC_OCR_SD_APP_CMD               ((uint32_t)0x00000020)
#define MMC_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define MMC_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)
#define MMC_ALLZERO                      ((uint32_t)0x00000000)
/* Card current status */
#define MMC_CARD_IDLE                    ((uint32_t)0x00000000)
#define MMC_CARD_READY                   ((uint32_t)0x00000001)
#define MMC_CARD_IDENT                   ((uint32_t)0x00000002)
#define MMC_CARD_STBY                    ((uint32_t)0x00000003)
#define MMC_CARD_TRAN                    ((uint32_t)0x00000004)
#define MMC_CARD_DATA                    ((uint32_t)0x00000005)
#define MMC_CARD_RECEIVING               ((uint32_t)0x00000006)
#define MMC_CARD_PROGRAMMING             ((uint32_t)0x00000007)
#define MMC_CARD_DIS                     ((uint32_t)0x00000008)

typedef enum
{
    SD_RESP_NO,
    SD_RSP_LEN_136,
    SD_RSP_LEN_48,
    SD_RSP_LEN_48_CHK_BUSY
} T_SD_RSP_TYPE;

typedef union
{
    uint32_t d;
    struct
    {
        uint32_t cmd_set    : 3;
        uint32_t rsvd0      : 5;
        uint32_t value      : 8;
        uint32_t index      : 8;
        uint32_t access     : 2;
        uint32_t rsvd1      : 6;
    } b;
} T_CMD6_ARG;

/* Global variables ------------------------------------------------------------*/
T_MMC_EXT_CSD *p_ext_csd = NULL;  /* should point to dsp share ram */
/* Record card relative address */
uint32_t MMC_RCA = 0x03;
/* Card type */
uint8_t MMC_Type = MMC_TYPE_SC;

const T_SDIF_PROT_PROP mmc_prot_prop =
{
    MMC_InitCard,
    MMC_GetCardInfo,
    MMC_ReadSingleBlock,
    MMC_ReadMultiBlock,
    MMC_WriteSingleBlock,
    MMC_WriteMultiBlock,
    MMC_Earse,
    MMC_WaitCardFinishProgram,
    MMC_AsyncReadSingleBlock,
    MMC_AsyncReadMultiBlock,
    MMC_AsyncWriteSingleBlock,
    MMC_AsyncWriteMultiBlock,
    MMC_SetBlockLen,
    MMC_Deinit
};

void MMC_RegisterFops(T_SDIF_PROT_PROP **pp_sd_if_fops)
{
    *pp_sd_if_fops = (T_SDIF_PROT_PROP *)(&mmc_prot_prop);
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
  * @brief  Reset MMC.
  * @param  None.
  * @retval T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_ResetCard(void)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        sd_print_error("MMC_ResetCard: error ret = SD_CMD_INHIBIT_BEFORE_RESET ");
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
        sd_print_error("MMC_ResetCard: error ret = SD_SDIO_CMD_INHIBIT_AFTER_RESET ");
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }
    return SD_OK;
}

/**
  * @brief  Checks for error conditions for R1.
  * @param  None.
  * @retval T_SD_STATUS: MMC status.
  */
static T_SD_STATUS MMC_CheckResp1(uint32_t cmd)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        sd_print_error("MMC_CheckResp1: error = SD_SDIO_CMD_INHIBIT_AFTER_RESET ");
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    /* Check response received is of desired command */
    if (p_sd_host_prop->SDH_CheckCmd(cmd) != SD_OK)
    {
        sd_print_error("MMC_CheckResp1: cmd error = SD_CMD_ERROR ");
        return SD_CMD_ERROR;
    }

    /* Check card status */
    uint32_t response = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);

    /* Check error or not */
    if ((response & MMC_OCR_ERRORBITS) == MMC_ALLZERO)
    {
        return sd_status;
    }

    /* Check specified error */
    if (response & MMC_OCR_ADDR_OUT_OF_RANGE)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_ADDR_OUT_OF_RANGE ");
        return SD_ADDR_OUT_OF_RANGE;
    }

    if (response & MMC_OCR_ADDR_MISALIGNED)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_ADDR_MISALIGNED ");
        return SD_ADDR_MISALIGNED;
    }

    if (response & MMC_OCR_BLOCK_LEN_ERR)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_BLOCK_LEN_ERR ");
        return SD_BLOCK_LEN_ERR;
    }

    if (response & MMC_OCR_ERASE_SEQ_ERR)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_ERASE_SEQ_ERR ");
        return SD_ERASE_SEQ_ERR;
    }

    if (response & MMC_OCR_BAD_ERASE_PARAM)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_BAD_ERASE_PARAM ");
        return SD_BAD_ERASE_PARAM;
    }

    if (response & MMC_OCR_WRITE_PROT_VIOLATION)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_WRITE_PROT_VIOLATION ");
        return SD_WRITE_PROT_VIOLATION;
    }

    if (response & MMC_OCR_LOCK_UNLOCK_FAILED)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_LOCK_UNLOCK_FAILED ");
        return SD_LOCK_UNLOCK_FAILED;
    }

    if (response & MMC_OCR_COM_CRC_FAILED)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_COM_CRC_FAILED ");
        return SD_COM_CRC_FAILED;
    }

    if (response & MMC_OCR_ILLEGAL_CMD)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_ILLEGAL_CMD ");
        return SD_ILLEGAL_CMD;
    }

    if (response & MMC_OCR_CARD_ECC_FAILED)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_CARD_ECC_FAILED ");
        return SD_CARD_ECC_FAILED;
    }

    if (response & MMC_OCR_CC_ERROR)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_CC_ERROR ");
        return SD_CC_ERROR;
    }

    if (response & MMC_OCR_GENERAL_UNKNOWN_ERROR)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_GENERAL_UNKNOWN_ERROR ");
        return SD_GENERAL_UNKNOWN_ERROR;
    }

    if (response & MMC_OCR_CID_CSD_OVERWRIETE)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_CID_CSD_OVERWRITE ");
        return SD_CID_CSD_OVERWRITE;
    }

    if (response & MMC_OCR_WP_ERASE_SKIP)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_WP_ERASE_SKIP ");
        return SD_WP_ERASE_SKIP;
    }

    if (response & MMC_OCR_CARD_ECC_DISABLED)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_CARD_ECC_DISABLED ");
        return SD_CARD_ECC_DISABLED;
    }

    if (response & MMC_OCR_ERASE_RESET)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_ERASE_RESET ");
        return SD_ERASE_RESET;
    }

    if (response & MMC_OCR_AKE_SEQ_ERROR)
    {
        sd_print_error("MMC_CheckResp1: response error = SD_AKE_SEQ_ERROR ");
        return SD_AKE_SEQ_ERROR;
    }
    /*  The card will expect ACMD */
    if (!(response & MMC_OCR_SD_APP_CMD))
    {
        sd_print_error("MMC_CheckResp1: response error = SD_APP_CMD ");
        return SD_APP_CMD;
    }

    return sd_status;
}

/**
  * @brief  Checks for error conditions for R3.
  * @param  None.
  * @retval T_SD_STATUS: MMC status.
  */
static T_SD_STATUS MMC_CheckResp3(uint32_t cmd)
{
    uint32_t response = 0;
    T_SD_STATUS sd_status = SD_OK;

    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT_AFTER_RESET;
    }

    response = p_sd_host_prop->SDH_GetResponse(SDH_RSP0);
    if (((response & (0xFF000000U)) >> 24) == 0xC0U)
    {
        MMC_Type = MMC_TYPE_HC_OR_XC;
    }
    else
    {
        MMC_Type = MMC_TYPE_SC;
    }

    /* Check OCR register */
    if (!(response & BIT(31)))
    {
        sd_status = SD_CARD_BUSY;
    }
    return sd_status;
}

/**
  * @brief  Get OCR
  * @param  None.
  * @retval T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_GetOCR(void)
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
    return MMC_CheckResp3(SDIO_SEND_OP_COND);
}

/**
  * @brief  Get CID.
  * @param  none.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_GetCID(uint32_t *pBuf)
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
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_SetRCA(uint32_t addr)
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
    sd_status = MMC_CheckResp1(SDIO_SET_REL_ADDR);
    return sd_status;
}

/**
  * @brief  Set RCA.
  * @param  addr: relative card address.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_GetRCA(uint32_t *RCA)
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
T_SD_STATUS MMC_GetCSD(uint32_t addr, uint32_t *pBuf)
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
  * @note EXT_CSD can be get only after card has been selected.
  */
T_SD_STATUS MMC_GetExtCSD(uint8_t buf[512])
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Data Configure */
    T_SDH_DATA_CFG sdh_data;
    sdh_data.is_read_dir = true;
    sdh_data.buf         = (uint32_t)buf;
    sdh_data.blk_size    = 512;
    sdh_data.blk_count   = 1;
    sd_status = p_sd_host_prop->SDH_DataConfig(&sdh_data);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_GetExtCSD: SDH_DataConfig, err sd_status %d, line %d", sd_status,
                       __LINE__);
        return sd_status;
    }

    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_EXT_CSD;
    sdh_cmd.argument        = 0;
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_GetExtCSD: err sd_status %d, line %d", sd_status, __LINE__);
    }

    return sd_status;
}

/**
  * @brief  send cid.
  * @param  addr: relative acrd address.
  * @param  pBuf: address store CSD when function return.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_SendCID(uint32_t addr, uint32_t *pBuf)
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
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_SelectCmd(FunctionalState NewState)
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
        sdh_cmd.argument        = MMC_RCA << 16;
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
  * @brief  Get MMC current status.
  * @param  pBuf: address store current status when function return.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_GetCardStatus(uint32_t *pStatus)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }
    /* CMD13: Get MMC current status */
    /* CMD: SDIO_SEND_STATUS, ARG: MMC_RCA << 16,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SEND_STATUS;
    sdh_cmd.argument        = MMC_RCA << 16;
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
  * @brief  Checks if the MMC is in programming state.
  * @param  pStatus: address store current status of card.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_WaitCardFinishProgram(void)
{
    T_SD_STATUS sd_status = SD_OK;
    uint32_t retry = 8;
    uint32_t card_status = 0;

    /* Get card status */
    sd_status = MMC_GetCardStatus(&card_status);
    /* Find out programming status */
    card_status = (card_status >> 9) & 0x0F;

    /* Wait finish */
    while ((sd_status == SD_OK) && \
           ((card_status == MMC_CARD_PROGRAMMING) || (card_status == MMC_CARD_RECEIVING)))
    {
        if (retry == 0)
        {
            return SD_PROG_TIMEOUT;
        }
        retry--;

        /* Get card status */
        sd_status = MMC_GetCardStatus(&card_status);
        /* Find out programming status */
        card_status = (card_status >> 9) & 0x0F;
    }

    return sd_status;
}

/**
  * @brief  Control MMC into inactive status.
  * @param  addr: relative acrd address.
  * @retval T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_GoInactiveState(void)
{
    /* Check command inhibit status */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_CMD_INHIBIT_BEFORE_RESET;
    }

    /* CMD15: GO_IDLE_STATE */
    /* CMD: SDIO_GO_INACTIVE_STATE, ARG: MMC_RCA << 16,
    0: NO DATA, 0: NO CHECK CMD IDX, 0: NO CHECK CMD CRC, RSP: 0(SD_RESP_NO) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_GO_INACTIVE_STATE;
    sdh_cmd.argument        = MMC_RCA << 16;
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
  * @retval T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_SleepOrAwakeCmd(FunctionalState NewState)
{
    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD5, response type: R1b */
    /* CMD: SDIO_SLEEP_AWAKE, ARG: MMC_RCA << 16 | ((uint32_t)NewState << 15),
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 3(SD_RSP_LEN_48_CHK_BUSY) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SLEEP_AWAKE;
    sdh_cmd.argument        = MMC_RCA << 16 | ((uint32_t)NewState << 15);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    return MMC_CheckResp1(SDIO_SET_REL_ADDR);
}

/**
  * @brief  Handle CSD data. CSD_Tab[3:0] <<= 8;
  * @param None.
  * @retval  None.
  */
static void MMC_HandleCSDData(T_MMC_DEV_INFO *p_dev_info)
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
  * @param  p_info : pointer to a T_MMC_DEV_INFO structure.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_GetCardInfo(void *p_info)
{
    T_MMC_DEV_INFO *p_dev_info = (T_MMC_DEV_INFO *)p_info;
    T_SD_STATUS sd_status = SD_OK;
    uint8_t tmp = 0;
    MMC_HandleCSDData(p_dev_info);

    p_dev_info->rca = (uint16_t)MMC_RCA;

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
    if (MMC_TYPE_SC == MMC_Type)
    {
        p_dev_info->dev_csd.DeviceSize = (tmp & 0x03) << 10;
    }

    if (MMC_TYPE_SC == MMC_Type)
    {
        p_dev_info->card_type = MMC_TYPE_SC;
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
        p_dev_info->card_type = MMC_TYPE_HC_OR_XC;
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

    if (MMC_TYPE_SC == MMC_Type)
    {
        p_dev_info->capacity *= (1 << (p_dev_info->dev_csd.DeviceSizeMul + 2));
        p_dev_info->block_size = 1 << (p_dev_info->dev_csd.RdBlockLen);
        p_dev_info->capacity *= p_dev_info->block_size;
    }
    else
    {
        p_dev_info->block_size = 512;
        /* assume that p_ext_csd is not NULL */
        uint32_t sec_count = p_ext_csd->b.sec_count;
        sd_print_info("MMC_GetCardInfo: eMMC sector count %d", sec_count);
        p_dev_info->capacity = (uint64_t)sec_count * 512ULL;
    }

    sd_print_info("MMC_GetCardInfo: card_type: %d, DeviceSize: 0x%x, DeviceSizeMul: 0x%x, RdBlockLen: 0x%x, capacity: 0x%x, block_size: 0x%x",
                  p_dev_info->card_type, p_dev_info->dev_csd.DeviceSize, p_dev_info->dev_csd.DeviceSizeMul,
                  p_dev_info->dev_csd.RdBlockLen,
                  p_dev_info->capacity, p_dev_info->block_size);
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
  * @brief  Send APP CMD.
  * @param  none.
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_SendAppCmd(uint32_t RCA)
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

    return MMC_CheckResp1(SDIO_APP_CMD);
}

T_SD_STATUS MMC_SetHighSpeedMode(FunctionalState NewState)
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
    T_CMD6_ARG cmd6_arg = {.d = 0};
    cmd6_arg.b.access = 0x3;  // write byte
    cmd6_arg.b.index = 185;   // byte HS_TIMING in EXT_CSD
    cmd6_arg.b.value = (NewState == ENABLE) ? 1 : 0;

    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SWITCH;
    sdh_cmd.argument        = cmd6_arg.d;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    if (p_sd_host_prop->SDH_WaitCmdDatComplete() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Check response */
    if (p_sd_host_prop->SDH_CheckCmd(SDIO_SWITCH) != SD_OK)
    {
        return SD_CMD_ERROR;
    }

    if (MMC_WaitCardFinishProgram() != SD_OK)
    {
        return SD_PROG_TIMEOUT;
    }

    return sd_status;
}

/**
  * @brief  Configure bus width.
  * @retval T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_SetBusWide(uint32_t bus_width)
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
    sdh_cmd.argument        = 0x03B70000 | (bus_width << 7);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48_CHK_BUSY;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    if (p_sd_host_prop->SDH_WaitCmdDatComplete() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Check response */
    if (p_sd_host_prop->SDH_CheckCmd(SDIO_SWITCH) != SD_OK)
    {
        return SD_CMD_ERROR;
    }

    if (MMC_WaitCardFinishProgram() != SD_OK)
    {
        return SD_PROG_TIMEOUT;
    }

    /* Set host controller bus width */
    sd_status = p_sd_host_prop->SDH_SetBusWide(bus_width);

    return sd_status;
}

/**
  * @brief  Initial MMC.
  * @param  p_dev: MMC device structure pointer.
  * @retval T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_InitCard(void *p_dev)
{
    T_MMC_DEV_INFO *p_dev_info = (T_MMC_DEV_INFO *)p_dev;
    T_SD_STATUS sd_status = SD_OK;

    if (!p_ext_csd)
    {

#ifdef TARGET_RTL87X3E
        p_ext_csd = os_mem_alloc(RAM_TYPE_BUFFER_ON, 512);
#else
        p_ext_csd = pvPortMalloc(512);
#endif

        if (!p_ext_csd)
        {
            sd_print_error("MMC_InitCard: os_mem_alloc fail");
            return SD_NO_FREE_SPACE;
        }
    }


#ifdef SD_MONITOR_EN
    sd_monitor_record();
#endif
    /*Initialization delay:the longest of: 1ms, 74 clock cycles */
    /* the supply ramp up time, or the boot operation time*/
    delay_us(5000);
#ifdef SD_MONITOR_EN
    sd_monitor_record();
#endif
    /* CMD0: GO_IDLE_STATE */
    sd_status = MMC_ResetCard();
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_ResetCard error, sd_status %d", sd_status);
        return sd_status;
    }

    /* CMD1: Get OCR */
    uint32_t retry = GET_OCR_TIMEOUT;//287->100ms
    do
    {
        sd_status = MMC_GetOCR();
        retry--;
    }
    while ((sd_status == SD_CARD_BUSY) && retry);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_GetOCR error, sd_status %d", sd_status);
        return sd_status;
    }

    /* CMD2: Get any card connect to the host to send the CID */
    sd_status = MMC_GetCID(p_dev_info->CID_Tab);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_GetCID error, sd_status %d", sd_status);
        return sd_status;
    }

    /* CMD3: Set RCA */
    sd_status = MMC_SetRCA(MMC_RCA);
    if (sd_status != SD_OK)
    {
        sd_print_error("SD_InitCard: MMC_SetRCA error, sd_status %d", sd_status);
        return sd_status;
    }

    /* CMD9: Get CSD */
    sd_status = MMC_GetCSD(MMC_RCA, p_dev_info->CSD_Tab);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_GetCSD error, sd_status %d", sd_status);
        return sd_status;
    }

    /* CMD7 is uesd to put card into the transfer state */
    sd_status = MMC_SelectCmd(ENABLE);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_SelectCmd error, sd_status %d", sd_status);
        return sd_status;
    }

    sd_status = MMC_SetHighSpeedMode(ENABLE);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_SetHighSpeedMode error, sd_status %d", sd_status);
        return sd_status;
    }

    if (sd_cfg.sdh_bus_width == SD_BUS_WIDTH_4B)
    {
        sd_status = MMC_SetBusWide(SD_BUS_WIDTH_4B);
        if (sd_status != SD_OK)
        {
            sd_print_error("MMC_InitCard: MMC_SetBusWide error, sd_status %d", sd_status);
            return sd_status;
        }
    }

    sd_status = MMC_GetExtCSD(p_ext_csd->d);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_InitCard: MMC_GetExtCSD error, sd_status %d", sd_status);
        return sd_status;
    }

    p_dev_info->initialized = true;
    sd_print_info("MMC_InitCard: status: SD_OK");
    return sd_status;
}

/**
  * @brief  Set block length for standard capacity MMC.
  * @param  block_len:  block length.
  * @retval  T_SD_STATUS: execution status.
  */
T_SD_STATUS MMC_SetBlockLen(uint32_t block_len)
{
    T_SD_STATUS sd_status = SD_OK;

    /* CMD16 */
    if (MMC_TYPE_SC == MMC_Type)
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
        sd_status = MMC_CheckResp1(SDIO_SET_BLOCKLEN);
        if (sd_status != SD_OK)
        {
            sd_print_error("SD_SetBlockLen: err sd_status %d, line %d", sd_status, __LINE__);
            return sd_status;
        }
    }
    return sd_status;
}

/**
  * @brief  Set block count.
  * @param  block_count: block count.
  * @retval T_SD_STATUS: MMC status.
  */
static T_SD_STATUS MMC_SetBlockCount(uint32_t block_count)
{
    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD: SDIO_SET_BLOCKLEN, ARG: block_count,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_SET_BLOCK_COUNT;
    sdh_cmd.argument        = block_count;
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* Check response */
    if (p_sd_host_prop->SDH_CheckCmd(SDIO_SET_BLOCK_COUNT) != SD_OK)
    {
        return SD_CMD_ERROR;
    }

    return SD_OK;
}

/**
  * @brief  Read single block from a specified address in a card.
  * @param  sector: MMC card sector value
  * @param  buf: pointer to the buffer that will contain the received data
  * @param  block_size: MMC Data block size
  * @param  block_count: MMC Data block count
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_ReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
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
        sd_print_error("MMC_ReadSingleBlock: SDH_DataConfig, err sd_status %d, line %d", sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD17 */
    /* CMD: SDIO_READ_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_SINGLE_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_ReadSingleBlock: err sd_status %d, line %d", sd_status, __LINE__);
    }

    return sd_status;
}

/**
  * @brief  Read multiple block from a specified address in a card.
  * @param  sector: MMC card sector value
  * @param  buf: pointer to the buffer that will contain the received data
  * @param  block_size: MMC Data block size
  * @param  block_count: MMC Data block count
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_ReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                               uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
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
        sd_print_error("MMC_ReadMultiBlock: SDH_DataConfig, err sd_status %d, line %d", sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD18 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_READ_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_MULT_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        sd_print_error("SD_ReadMultiBlock: err sd_status %d, line %d", sd_status, __LINE__);
    }

    return sd_status;
}

/**
  * @brief  write single block to a specified address in a card.
  * @param  sector: MMC card sector value
  * @param  buf: pointer to the buffer that will contain the data to write
  * @param  block_size: MMC Data block size
  * @param  block_count: MMC Data block count
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_WriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
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
        sd_print_error("MMC_WriteSingleBlock: SDH_DataConfig, err sd_status %d, line %d", sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD24 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_SINGLE_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_WriteSingleBlock: err sd_status %d, line %d", sd_status, __LINE__);
        return sd_status;
    }

    /* Check block whether write protection */
    if (p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & MMC_OCR_WRITE_PROT_VIOLATION)
    {
        sd_print_error("MMC_WriteSingleBlock: err rsp = sd_write_protection_violation");
        return SD_WRITE_PROT_VIOLATION;
    }

    return sd_status;
}

/**
  * @brief  write multiple block to a specified address in a card.
  * @param  sector: MMC card sector value
  * @param  buf: pointer to the buffer that will contain the data to write
  * @param  block_size: MMC Data block size
  * @param  block_count: MMC Data block count
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_WriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
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
        sd_print_error("MMC_WriteMultiBlock: SDH_DataConfig, err sd_status %d, line %d", sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD25 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_MULT_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    sd_status = p_sd_host_prop->SDH_WaitCmdDatComplete();
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_WriteMultiBlock: err sd_status %d, line %d", sd_status, __LINE__);
        return sd_status;
    }

    /* Check block whether write protection */
    if (p_sd_host_prop->SDH_GetResponse(SDH_RSP0) & MMC_OCR_WRITE_PROT_VIOLATION)
    {
        sd_print_error("MMC_WriteMultiBlock: err res = sd_write_protection_violation");
        return SD_WRITE_PROT_VIOLATION;
    }

    return sd_status;
}

/**
  * @brief  Earse MMC device.
  * @param  start_addr: start address which need to erase,the addr's unit is blocksize.
  * @param  end_addr: end address which need to erase.
  * @retval  T_SD_STATUS: execution status.
  */
T_SD_STATUS MMC_Earse(uint32_t start_addr, uint32_t end_addr)
{
    T_SD_STATUS sd_status = SD_OK;

    /* Waiting for CMD line idle */
    if (p_sd_host_prop->SDH_WaitCmdLineIdle() != SD_OK)
    {
        return SD_SDIO_CMD_INHIBIT;
    }

    /* CMD35 */
    /* CMD: SDIO_ERASE_GRP_END, ARG: start_addr or start_addr * 512,
    0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_ERASE_GRP_START;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? start_addr : (start_addr * 512);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    sd_status = MMC_CheckResp1(SDIO_ERASE_GRP_START);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_Earse: err sd_status %d, line %d", sd_status, __LINE__);
        return sd_status;
    }

    /* CMD36 */
    /* CMD: SDIO_ERASE_GRP_END, ARG: end_addr or end_addr * 512,
        0: NO DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    sdh_cmd.cmd             = SDIO_ERASE_GRP_END;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? end_addr : (end_addr * 512);
    sdh_cmd.with_data       = 0;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    /* Check response */
    sd_status = MMC_CheckResp1(SDIO_ERASE_GRP_END);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_Earse: err sd_status %d, line %d", sd_status, __LINE__);
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
        sd_print_error("MMC_Earse: CMD line is not idle");
        return SD_SDIO_CMD_INHIBIT;
    }

    return sd_status;
}

/**
  * @brief  Stop transfer.
  * @param  none.
  * @retval  T_SD_STATUS: execution status.
  */
T_SD_STATUS MMC_StopTransfer(void)
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
    sd_status = MMC_CheckResp1(SDIO_STOP_TRANSMISSION);
    if (sd_status != SD_OK)
    {
        sd_print_error("MMC_StopTransfer: err sd_status %d, line %d", sd_status, __LINE__);
    }
    return sd_status;
}

/**
  * @brief  Read one block from a specified address in a card.
  * @param  sector: sector from where data are to be read
  * @param  pDescriptorTab: pointer to the buffer that will contain the received data
  * @param  blockSize: the  card Data block size
  * @retval  SD_Error: MMC status.
  */
T_SD_STATUS MMC_AsyncReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Set block count */
    sd_status = MMC_SetBlockCount(1);
    if (sd_status != SD_OK)
    {
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
        sd_print_error("MMC_AsyncReadSingleBlock: SDH_DataConfig, err sd_status %d, line %d",
                       sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD17 */
    /* CMD: SDIO_READ_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_SINGLE_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
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
  * @param  blockSize: MMC Data block size
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_AsyncReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                    uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
    {
        return SD_INVALID_PARAMETER;
    }

    /* Set block count */
    sd_status = MMC_SetBlockCount(block_count);
    if (sd_status != SD_OK)
    {
        return sd_status;
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
        sd_print_error("MMC_AsyncReadMultiBlock: SDH_DataConfig, err sd_status %d, line %d", sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD18 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_READ_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_READ_MULT_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
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
  * @param  blockSize: MMC Data block size
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_AsyncWriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Set block count */
    sd_status = MMC_SetBlockCount(1);
    if (sd_status != SD_OK)
    {
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
        sd_print_error("MMC_AsyncWriteSingleBlock: SDH_DataConfig, err sd_status %d, line %d",
                       sd_status, __LINE__);
        return sd_status;
    }

    /* CMD24 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_SINGLE_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_SINGLE_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
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
  * @param  blockSize:MMC Data block size
  * @retval  T_SD_STATUS: MMC status.
  */
T_SD_STATUS MMC_AsyncWriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                     uint16_t block_count)
{
    T_SD_STATUS sd_status = SD_OK;

    if (MMC_ADDRESS_NULL == buf)
    {
        sd_status = SD_INVALID_PARAMETER;
        return sd_status;
    }

    /* Set block count */
    sd_status = MMC_SetBlockCount(block_count);
    if (sd_status != SD_OK)
    {
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
        sd_print_error("MMC_AsyncWriteMultiBlock: SDH_DataConfig, err sd_status %d, line %d",
                       sd_status,
                       __LINE__);
        return sd_status;
    }

    /* CMD25 */
    /*SDHC block size is fixed to 512 Byte, addr offset follow the block size*/
    /* CMD: SDIO_WRITE_MULT_BLOCK, ARG: sector or sector * block_size,
        1: WITH DATA, 1: CHECK CMD IDX, 1: CHECK CMD CRC, RSP: 2(SD_RSP_LEN_48) */
    T_SDH_CMD_CFG sdh_cmd;
    sdh_cmd.cmd             = SDIO_WRITE_MULT_BLOCK;
    sdh_cmd.argument        = (MMC_TYPE_HC_OR_XC == MMC_Type) ? sector : (sector * block_size);
    sdh_cmd.with_data       = 1;
    sdh_cmd.cmd_idx_check   = ENABLE;
    sdh_cmd.cmd_crc_check   = ENABLE;
    sdh_cmd.resp_len        = SD_RSP_LEN_48;
    p_sd_host_prop->SDH_SendCommand(&sdh_cmd);

    return sd_status;
}

void MMC_Deinit(void)
{
    p_sd_host_prop->SDH_Deinit();

    if (p_ext_csd != NULL)
    {
#ifdef TARGET_RTL87X3E
        os_mem_free(p_ext_csd);
#else
        vPortFree(p_ext_csd);
#endif
        p_ext_csd = NULL;
    }
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
