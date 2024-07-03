/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_if.c
* @brief    This file provides operation interface for SD library.
* @details
* @author   elliot_chen
* @date     2021-06-03
* @version  v2.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "mmc.h"
#include "sdh_int.h"
#include "sdh_pwr.h"
#include "sdcard.h"
#include "sd_icfg.h"
#include "sd_if.h"

/* Defines -------------------------------------------------------------------*/
typedef struct
{
    T_SD_DEV_INFO sd_info;
    T_MMC_DEV_INFO mmc_info;
} T_SD_IF_DEV;

typedef enum
{
    SD_IF_SYNC_MODE,
    SD_IF_ASYNC_MODE,
} T_SD_IF_MODE;

typedef struct
{
    T_SD_IF_TYPE sd_if_type;
    T_SD_IF_DEV dev;
    uint8_t use_flag;
    T_SD_IF_MODE mode;
    T_SDIF_PROT_PROP *p_prot_fops;
} T_SD_IF;

/* Globals -------------------------------------------------------------------*/
T_SD_IF sd_if;

T_SD_STATUS sd_if_bind(T_SD_IF_TYPE sd_if_type, T_SD_IF *p_sd_if, void **pp_dev_info)
{
    /* Check device status */
    if (p_sd_if->use_flag == true)
    {
        if (sd_if_type != p_sd_if->sd_if_type)
        {
            sd_print_error("sd_if_bind: sd_if_type violation %d", sd_if_type);
            return SD_IF_VIOLATION;
        }
    }

    /* Select device */
    if (sd_if_type == SD_IF_SD_CARD)
    {
        /* Check initialized status */
        if (p_sd_if->dev.sd_info.initialized)
        {
            *pp_dev_info = (void *)(&(p_sd_if->dev.sd_info));
            return SD_OK;
        }
        *pp_dev_info = (void *)(&(p_sd_if->dev.sd_info));
        SD_RegisterFops(&(p_sd_if->p_prot_fops));
    }
    else if (sd_if_type == SD_IF_MMC)
    {
        /* Check initialized status */
        if (p_sd_if->dev.mmc_info.initialized)
        {
            return SD_OK;
        }
        *pp_dev_info = (void *)(&(p_sd_if->dev.mmc_info));
        MMC_RegisterFops(&(p_sd_if->p_prot_fops));
    }
    else
    {
        sd_print_error("sd_if_bind: invalid sd_if_type %d",  sd_if_type);
        return SD_INVALID_PARAMETER;
    }

    return SD_OK;
}
extern T_SD_STATUS sd_if_transfer_start(void);
T_SD_STATUS sd_if_init(T_SD_IF_TYPE sd_if_type)
{
    uint32_t fail_line = 0;
    void *p_dev_info = 0;
    T_SD_STATUS ret = SD_OK;

    ret = sd_if_bind(sd_if_type, &sd_if, &p_dev_info);
    if (ret != SD_OK)
    {
        return ret;
    }

    /* update sd_if status */
    sd_if.sd_if_type = sd_if_type;
    sd_if.use_flag = true;

    ret = SDH_PowerOnForIdentifyCard();
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

#ifdef ASYNC_MODE_ENABLE
    SDH_IntModeInit(ENABLE);
    SDH_PlugAndErrorIntConfig();
#endif

    ret = sd_if.p_prot_fops->sd_if_init_dev(p_dev_info);
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

    /* Update device information */
    ret = sd_if.p_prot_fops->sd_if_get_dev_info(p_dev_info);

    /* Initialize the SDIO host peripheral */
    SDH_InitForDataTransfer();

    sd_if.p_prot_fops->sd_if_set_bus_delay(p_dev_info);

    return ret;

fail:
    sd_print_error("sd_if_init: err ret %d, fail line %d", ret, fail_line);
    return ret;
}

void sd_if_deinit(void)
{
    memset(&sd_if, 0, sizeof(T_SD_IF));
    SD_Deinit();
    // TODO
}

T_SD_STATUS sd_if_read(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num)
{
    T_SD_STATUS ret = SD_OK;
    T_SDIF_PROT_PROP *p_sd_prot_prop = sd_if.p_prot_fops;

    uint8_t retry = 3;
    do
    {
        /* Check status */
        ret = p_sd_prot_prop->sd_if_wait_card_finish_program();
        if (ret != SD_OK)
        {
            sd_print_error("sd_if_read: err ret %d", ret);
            return ret;
        }

        if (blk_num == 1)
        {
            ret = p_sd_prot_prop->sd_if_read_single_block(sector, buf, blk_size);
        }
        else
        {
            ret = p_sd_prot_prop->sd_if_read_multi_block(sector, buf, blk_size, blk_num);
        }
        if (ret != SD_OK)
        {
            sd_print_error("sd_if_read: err ret %d", ret);
            ret = SDIO_CheckErr_TryRecovery();
        }
    }
    while (--retry && (ret == SD_CMD_LINE_RECOVERABLE_ERROR || ret == SD_DAT_LINE_RECOVERABLE_ERROR));

    return ret;
}

T_SD_STATUS sd_if_write(uint32_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num)
{
    T_SD_STATUS ret = SD_OK;
    T_SDIF_PROT_PROP *p_sd_prot_prop = sd_if.p_prot_fops;

    uint8_t retry = 3;
    do
    {
        /* Check status */
        ret = p_sd_prot_prop->sd_if_wait_card_finish_program();
        if (ret != SD_OK)
        {
            sd_print_error("sd_if_write: err ret %d", ret);
            return ret;
        }

        if (blk_num == 1)
        {
            ret = p_sd_prot_prop->sd_if_write_single_block(sector, buf, blk_size);
        }
        else
        {
            ret = p_sd_prot_prop->sd_if_write_multi_block(sector, buf, blk_size, blk_num);
        }
        if (ret != SD_OK)
        {
            sd_print_error("sd_if_write: err ret %d", ret);
            ret = SDIO_CheckErr_TryRecovery();
        }
    }
    while (--retry && (ret == SD_CMD_LINE_RECOVERABLE_ERROR || ret == SD_DAT_LINE_RECOVERABLE_ERROR));

    return ret;
}

T_SD_STATUS sd_if_erase(uint32_t start_addr, uint32_t end_addr)
{
    uint32_t fail_line = 0;
    T_SD_STATUS ret = SD_OK;
    T_SDIF_PROT_PROP *p_sd_prot_prop = sd_if.p_prot_fops;

    /* Check status */
    ret = p_sd_prot_prop->sd_if_wait_card_finish_program();
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

    ret = p_sd_prot_prop->sd_if_earse(start_addr, end_addr);
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

#if 0
    uint32_t retry = 0xFFFFFFFF;
    while (retry--)
    {
        ret = p_sd_prot_prop->sd_if_wait_card_finish_program();
        if ((ret == SD_OK) || (retry == 0))
        {
            sd_print_info("sd_if_erase: ret = 0x%x", 1, ret);
            break;
        }
    }
#endif
    return ret;

fail:
    sd_print_error("sd_if_erase: err ret %d, fail line %d", ret, fail_line);
    return ret;
}

#ifdef ASYNC_MODE_ENABLE

T_SD_STATUS sd_if_transfer_start(void)
{
    sd_if.mode = SD_IF_ASYNC_MODE;
    SDH_TransCtrlIntConfig(ENABLE);

    return SD_OK;
}

T_SD_STATUS sd_if_async_read(uint64_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num)
{
    uint32_t fail_line = 0;
    T_SD_STATUS ret = SD_OK;
    T_SDIF_PROT_PROP *p_sd_prot_prop = sd_if.p_prot_fops;

    /* Check status */
    ret = p_sd_prot_prop->sd_if_wait_card_finish_program();
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

    ret = sd_if_transfer_start();
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

    if (blk_num == 1)
    {
        ret = p_sd_prot_prop->sd_if_async_read_single_block(sector, buf, blk_size);
    }
    else
    {
        ret = p_sd_prot_prop->sd_if_async_read_multi_block(sector, buf, blk_size, blk_num);
    }
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }
    return ret;

fail:
    sd_print_error("sd_if_async_read: err ret %d, fail line %d", ret, fail_line);
    return ret;
}

T_SD_STATUS sd_if_async_write(uint64_t sector, uint32_t buf, uint16_t blk_size, uint16_t blk_num)
{
    uint32_t fail_line = 0;
    T_SD_STATUS ret = SD_OK;
    T_SDIF_PROT_PROP *p_sd_prot_prop = sd_if.p_prot_fops;

    /* Check status */
    ret = p_sd_prot_prop->sd_if_wait_card_finish_program();
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

    ret = sd_if_transfer_start();
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }

    if (blk_num == 1)
    {
        ret = p_sd_prot_prop->sd_if_async_write_single_block(sector, buf, blk_size);
    }
    else
    {
        ret = p_sd_prot_prop->sd_if_async_write_multi_block(sector, buf, blk_size, blk_num);
    }
    if (ret != SD_OK)
    {
        fail_line = __LINE__;
        goto fail;
    }
    return ret;
fail:
    sd_print_error("sd_if_async_write: err ret %d, fail line %d", ret, fail_line);
    return ret;
}
#endif

T_SD_STATUS sd_if_set_block_len(uint32_t block_len)
{
    return sd_if.p_prot_fops->sd_if_set_block_len(block_len);
}

/**
  * @brief  Checks if the MMC is in programming state.
  * @param  pStatus: address store current status of card.
* @retval  T_SD_STATUS: device status which can be SD_PROG_TIMEOUT(receiving or programming) or SD_OK(idle status).
  */
T_SD_STATUS sd_if_check_program_status(void)
{
    return sd_if.p_prot_fops->sd_if_wait_card_finish_program();
}

uint64_t sd_if_get_dev_capacity(void)
{
    uint64_t capacity = 0;
    T_SD_IF_TYPE sd_if_type = sd_if.sd_if_type;

    if (sd_if_type == SD_IF_SD_CARD)
    {
        capacity = sd_if.dev.sd_info.capacity;
    }
    else if (sd_if_type == SD_IF_MMC)
    {
        capacity = sd_if.dev.mmc_info.capacity;
    }

    sd_print_info("sd_if_get_dev_capacity: capacity 0x%x, capacity + 1 0x%x, sd_if_type 0x%x",
                  *(uint32_t *)&capacity, *((uint32_t *)&capacity + 1), sd_if_type);
    return capacity;
}

uint32_t sd_if_get_dev_block_num(void)
{
    uint32_t card_blk_cnt = sd_if_get_dev_capacity() / 0x200;

    sd_print_info("sd_if_get_dev_block_num: card_blk_cnt 0x%x", card_blk_cnt);
    return card_blk_cnt;
}

uint32_t sd_if_get_dev_block_size(void)
{
    uint32_t block_size = 0;
    T_SD_IF_TYPE sd_if_type = sd_if.sd_if_type;

    if (sd_if_type == SD_IF_SD_CARD)
    {
        block_size = sd_if.dev.sd_info.block_size;
    }
    else if (sd_if_type == SD_IF_MMC)
    {
        block_size = sd_if.dev.mmc_info.block_size;
    }
    sd_print_info("sd_if_get_dev_block_size: block_size 0x%x, sd_if_type 0x%x", block_size,
                  sd_if_type);

    return block_size;
}

void sd_if_pin_group_cfg(uint32_t group_index)
{
    SDH_PinGroupConfig(group_index, sd_cfg.sdh_bus_width);
}

T_SD_STATUS sd_if_sdh_pow_on(void)
{
    return  SDH_PowerOnForDataTransfer();
}

T_SD_IF_TYPE sd_if_get_dev_type(void)
{
    T_SD_IF_TYPE sd_if_type = sd_if.sd_if_type;

    sd_print_info("sd_if_get_dev_type: sd_if_type 0x%x", sd_if_type);
    return sd_if_type;
}

void sd_if_sdh_clk_cmd(bool NewState)
{
    SDH_ClockCmd((FunctionalState)NewState);
}

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
