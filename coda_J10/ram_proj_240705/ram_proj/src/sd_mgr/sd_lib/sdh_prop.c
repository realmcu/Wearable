/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdh_prop.c
* @brief    This file provides sd host property firmware functions for sd library.
* @details  Adjust firmware functions according to the specified sd host IP driver.
* @author   elliot_chen
* @date     2021-01-29
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_sdio.h"
#include "sd_icfg.h"
#include "sdh_prop.h"
#ifdef TARGET_RTL87X3E
#include "os_mem.h"
#else
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portable.h"
#endif

/* Defines -------------------------------------------------------------------*/
#define SDIO_CMD_TIMEOUT                (385000UL*SD_CMD_SW_TIMEOUT/SD_MCU_CLOCK)
#define SDIO_CMD_DATA_TIMEOUT           (385000UL*SD_CMD_DATA_SW_TIMEOUT/SD_MCU_CLOCK)
#define SDH_GetResponse                 SDIO_GetResponse

/* Internal declaration ------------------------------------------------------*/
static T_SD_STATUS SDH_WaitCmdLineIdle(void);
static T_SD_STATUS SDH_WaitCmdDatComplete(void);
static T_SD_STATUS SDH_CheckCmd(uint32_t cmd);
static void SDH_SendCommand(T_SDH_CMD_CFG *p_sdh_cmd);
static T_SD_STATUS SDH_SetBusWide(uint32_t bus_wide);
static T_SD_STATUS SDH_DataConfig(T_SDH_DATA_CFG *p_sdh_data);
static void SDH_Deinit(void);
static T_SD_STATUS SDH_BusDelaySel(uint32_t bus_wide, uint8_t dat_delay);

/* Gloabls -------------------------------------------------------------------*/
/*  Points to the T_SD_HOST_PROP structure of sd host IP */
/*  The purpose of this structure is to select the specified sd host IP, such as amebia IP or synopsys IP */
const T_SD_HOST_PROP sd_host_prop =
{
    SDH_WaitCmdLineIdle,
    SDH_WaitCmdDatComplete,
    SDH_CheckCmd,
    SDH_GetResponse,
    SDH_SetBusWide,
    SDH_SendCommand,
    SDH_DataConfig,
    SDH_Deinit,
    SDH_BusDelaySel,
};

/*  The purpose of this structure is to select the specified sd host IP, such as amebia IP or synopsys IP */
const T_SD_HOST_PROP *const p_sd_host_prop = &sd_host_prop;

/* Store information of ADMA2 dersciptor table which used for ADMA2 transmission */

SDIO_ADMA2TypeDef   *SDIO_ADMA2_DescTab = NULL;

/**
  * @brief  Wait CMD line idle and issue next command.
  * @param  None.
  * @retval T_SD_STATUS: execution status.
  */
static T_SD_STATUS SDH_WaitCmdLineIdle(void)
{
    uint32_t time_out = SDIO_CMD_TIMEOUT;

    /* Check command inhibit(CMD) status */
    while (SDIO_GetFlagStatus(SDIO_FLAG_CMD_INHIBIT) == SET)
    {
        time_out--;
        if (time_out == 0)
        {
            sd_print_error("SDH_WaitCmdLineIdle: error, software timeout");
            return SD_SDIO_CMD_INHIBIT;
        }
    }
    return SD_OK;
}

/**
  * @brief  Reset command complete when get the end bit of the command response(Except Auto CMD12).
  * @param  None.
  * @retval T_SD_STATUS: execution status.
  */
static T_SD_STATUS SDH_WaitCmdDatComplete(void)
{
    uint32_t time_out = SDIO_CMD_DATA_TIMEOUT;

    /* Check command inhibit(DAT) status */
    while (SDIO_GetFlagStatus(SDIO_FLAG_CMD_DAT_INHIBIT) == SET)
    {
        time_out--;
        if (time_out == 0)
        {
            sd_print_error("SDH_WaitCmdDatComplete: error, software timeout");
            return SD_SDIO_CMD_COMPLETE;
        }
    }
    return SD_OK;
}

/**
  * @brief  Check command index in response data.
  * @param  None.
  * @retval T_SD_STATUS: execution status.
  */
static T_SD_STATUS SDH_CheckCmd(uint32_t cmd)
{
    if ((SDIO_GetResponse(SDIO_RSP2) & 0xFF) != cmd)
    {
        return SD_CMD_ERROR;
    }

    return SD_OK;
}

/**
 * @brief  Configure SD host bus wide.
 * @param bus_wide: value of SD host bus wide.
 *   This parameter can be the following values:
 *   @arg SD_BUS_WIDTH_1B: 1-bit mode.
 *   @arg SD_BUS_WIDTH_4B: 4-bit mode.
 * @return none.
 */
static T_SD_STATUS SDH_SetBusWide(uint32_t bus_wide)
{
    if (bus_wide == SD_BUS_WIDTH_1B)
    {
        SDIO_SetBusWide(SDIO_BusWide_1b);
    }
    else if (bus_wide == SD_BUS_WIDTH_4B)
    {
        SDIO_SetBusWide(SDIO_BusWide_4b);
    }
    else
    {
        return SD_INVALID_PARAMETER;
    }
    return SD_OK;
}

static T_SD_STATUS SDH_BusDelaySel(uint32_t bus_wide, uint8_t bus_delay)
{
    if (bus_delay <= 0xF)
    {
        SDIO_BusDelaySel(bus_wide, bus_delay);
    }
    else
    {
        return SD_INVALID_PARAMETER;
    }

    return SD_OK;
}

static void SDH_SendCommand(T_SDH_CMD_CFG *p_sdh_cmd)
{
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;

    SDIO_CmdInitStructure.SDIO_Argument     = p_sdh_cmd->argument;
    SDIO_CmdInitStructure.SDIO_CmdIndex     = p_sdh_cmd->cmd;
    SDIO_CmdInitStructure.SDIO_CmdType      = NORMAL;
    SDIO_CmdInitStructure.SDIO_DataPresent  = p_sdh_cmd->with_data;
    SDIO_CmdInitStructure.SDIO_CmdIdxCheck  = p_sdh_cmd->cmd_idx_check;
    SDIO_CmdInitStructure.SDIO_CmdCrcCheck  = p_sdh_cmd->cmd_crc_check;
    SDIO_CmdInitStructure.SDIO_ResponseType = p_sdh_cmd->resp_len;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
}

static T_SD_STATUS SDH_DataConfig(T_SDH_DATA_CFG *p_sdh_data)
{
    if (SDIO_ADMA2_DescTab == NULL)
    {
#ifdef TARGET_RTL87X3E
        SDIO_ADMA2_DescTab = os_mem_alloc(RAM_TYPE_BUFFER_ON, 2 * sizeof(SDIO_ADMA2TypeDef));
#else
        SDIO_ADMA2_DescTab = pvPortMalloc(2 * sizeof(SDIO_ADMA2TypeDef));
#endif
        if (SDIO_ADMA2_DescTab == NULL)
        {
            sd_print_error("SDH_DataConfig: os_mem_alloc fail");
            return SD_NO_FREE_SPACE;
        }
    }

    SDIO_DataInitTypeDef SDIO_DataInitStruct;

    //buf: for 64bit address descriptor shall be aligned to 4-byte address boundary
    //block_size: max leng is  65536  for 16bit length mode
    if (p_sdh_data->blk_count == 1)
    {
        /* Data Configure */
        SDIO_ADMA2_DescTab[0].SDIO_Address              =   p_sdh_data->buf;
        SDIO_ADMA2_DescTab[0].SDIO_Length               =   p_sdh_data->blk_size;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Valid =   1;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_End   =   1;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Int   =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Act1  =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Act2  =   1;
    }
    else if (p_sdh_data->blk_count <= 127)
    {
        SDIO_ADMA2_DescTab[0].SDIO_Address              =   p_sdh_data->buf;
        SDIO_ADMA2_DescTab[0].SDIO_Length               =   p_sdh_data->blk_count * p_sdh_data->blk_size;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Valid =   1;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_End   =   1;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Int   =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Act1  =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Act2  =   1;
    }
    else if (p_sdh_data->blk_count <= 254)
    {
        SDIO_ADMA2_DescTab[0].SDIO_Address              =   p_sdh_data->buf;
        SDIO_ADMA2_DescTab[0].SDIO_Length               =   p_sdh_data->blk_size * 127;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Valid =   1;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_End   =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Int   =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Act1  =   0;
        SDIO_ADMA2_DescTab[0].SDIO_Attribute.SDIO_Act2  =   1;

        SDIO_ADMA2_DescTab[1].SDIO_Address              =   p_sdh_data->buf + p_sdh_data->blk_size * 127 +
                                                            1;
        SDIO_ADMA2_DescTab[1].SDIO_Length               =   p_sdh_data->blk_size *
                                                            (p_sdh_data->blk_count - 127);
        SDIO_ADMA2_DescTab[1].SDIO_Attribute.SDIO_Valid =   1;
        SDIO_ADMA2_DescTab[1].SDIO_Attribute.SDIO_End   =   1;
        SDIO_ADMA2_DescTab[1].SDIO_Attribute.SDIO_Int   =   0;
        SDIO_ADMA2_DescTab[1].SDIO_Attribute.SDIO_Act1  =   0;
        SDIO_ADMA2_DescTab[1].SDIO_Attribute.SDIO_Act2  =   1;
    }
    else
    {
        return SD_READ_EXCEED_MAX_LEN;
    }

    SDIO_DataStructInit(&SDIO_DataInitStruct);
    SDIO_DataInitStruct.SDIO_Address        = (uint32_t)SDIO_ADMA2_DescTab;
    SDIO_DataInitStruct.SDIO_BlockCount     = p_sdh_data->blk_count;
    SDIO_DataInitStruct.SDIO_BlockSize      = p_sdh_data->blk_size;
    SDIO_DataInitStruct.SDIO_TransferDir    = (p_sdh_data->is_read_dir) ? SDIO_TransferDir_READ :
                                              SDIO_TransferDir_WRITE;
    SDIO_DataInitStruct.SDIO_DMAEn          = ENABLE;
    SDIO_DataConfig(&SDIO_DataInitStruct);

    return SD_OK;
}

static void SDH_Deinit(void)
{
    if (SDIO_ADMA2_DescTab != NULL)
    {
#ifdef TARGET_RTL87X3E
        os_mem_free(SDIO_ADMA2_DescTab);
#else
        vPortFree(SDIO_ADMA2_DescTab);
#endif
        SDIO_ADMA2_DescTab = NULL;
    }
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
