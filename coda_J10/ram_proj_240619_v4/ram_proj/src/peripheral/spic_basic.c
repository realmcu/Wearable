/**
 *****************************************************************************************
 *     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file    spic_basic.c
 * @brief   RXI-312 SPI controller basic implementation for ROM
 * @author  Yao-Yu
 * @date    2020-07-08
 * @version v0.1
 * ***************************************************************************************
 */

#include <stddef.h>
#include <string.h>

//#include "utils.h"
//#include "trace.h"
#include "spic_basic.h"
uint8_t irq_lock_nest_cnt = 0;
#define IRQ_RESET_CNT() (irq_lock_nest_cnt = 0)

/* disable irq with increasing */
#define IRQ_LOCK()                                                      \
    do                                                                  \
    {                                                                   \
        __disable_irq();                                                \
        irq_lock_nest_cnt++;                                            \
    } while (0)

/* enable irq with checking counter */
#define DIRECT_LOG(...)
#define IRQ_UNLOCK()                                                    \
    do                                                                  \
    {                                                                   \
        if (irq_lock_nest_cnt == 0)                                     \
        {                                                               \
            DIRECT_LOG(UNLOCK_FAIL, 2, __LINE__, irq_lock_nest_cnt);    \
            while (1);                                                  \
        }                                                               \
        irq_lock_nest_cnt--;                                            \
        if (irq_lock_nest_cnt == 0)                                     \
        {                                                               \
            __enable_irq();                                             \
        }                                                               \
    } while (0)

static SPIC_CFG_STRUCT spic_config_struct[SPIC_NUM];

bool spic_init(SPIC_TypeDef *SPIC)
{
    SPIC_CFG_STRUCT *config = spic_get_config(SPIC);

    if (config == NULL)
    {
        return false;
    }

    memset(config->d32, 0, sizeof(config->d32));

    if (SPIC == SPIC0)
    {
        SYSBLKCTRL->u_2A8.BITS_2A8.SPIC_MASTER_EN = 1;
    }
    else if (SPIC == SPIC1)
    {

    }
    else if (SPIC == SPIC2)
    {

    }
    else
    {
        return false;
    }

    return true;
}

SPIC_CFG_STRUCT *spic_get_config(SPIC_TypeDef *SPIC)
{
    if (SPIC == SPIC0)
    {
        return &spic_config_struct[0];
    }
    else if (SPIC == SPIC1)
    {
        return &spic_config_struct[1];
    }
    else if (SPIC == SPIC2)
    {
        return &spic_config_struct[2];
    }
    else
    {
        return false;
    }
}

bool spic_set_config(SPIC_TypeDef *SPIC, uint32_t max_retry)
{
    SPIC_CFG_STRUCT *config = spic_get_config(SPIC);

    if (config == NULL)
    {
        return false;
    }

    config->max_retry = max_retry;

    return true;
}

bool spic_in_busy(SPIC_TypeDef *SPIC)
{
    return (SPIC_REG_FIELD_RD(SPIC, SSIENR, SPIC_EN) ||
            SPIC_REG_FIELD_RD(SPIC, SR, BUSY) ||
            SPIC_REG_FIELD_RD(SPIC, SR, TXE));
}

bool spic_lock(SPIC_TypeDef *SPIC)
{
    SPIC_CFG_STRUCT *config = spic_get_config(SPIC);

    if (config == NULL)
    {
        return false;
    }

    uint32_t max_retry = config->max_retry;

    /* Wait until SPIC is not busy for max_retry */
    while (max_retry)
    {
        IRQ_LOCK();

        if (spic_in_busy(SPIC))
        {
            IRQ_UNLOCK();
        }
        else
        {
            break;
        }

        max_retry--;
    }

    return max_retry ? true : false;
}

bool spic_unlock(SPIC_TypeDef *SPIC)
{
    IRQ_UNLOCK();
    return (spic_get_config(SPIC) != NULL);
}

void spic_enable(SPIC_TypeDef *SPIC, bool enable)
{
    SPIC_REG_FIELD_WR(SPIC, SSIENR, SPIC_EN, enable);
}

void spic_enable_dum_byte(SPIC_TypeDef *SPIC, bool enable)
{
    SPIC_REG_FIELD_WR(SPIC, VALID_CMD, DUM_EN, enable);
}

void spic_disable_interrupt(SPIC_TypeDef *SPIC)
{
    SPIC_REG_WR(SPIC, IMR, 0);
}

void spic_set_rx_mode(SPIC_TypeDef *SPIC)
{
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, TMOD, 3);
}

void spic_set_tx_mode(SPIC_TypeDef *SPIC)
{
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, TMOD, 0);
}

void spic_set_auto_mode(SPIC_TypeDef *SPIC)
{
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, USER_MODE, 0);
}

void spic_set_user_mode(SPIC_TypeDef *SPIC)
{
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, USER_MODE, 1);
}

void spic_set_rxndf(SPIC_TypeDef *SPIC, uint32_t ndf)
{
    SPIC_REG_FIELD_WR(SPIC, RX_NDF, RX_NDF, ndf);
}

void spic_set_txndf(SPIC_TypeDef *SPIC, uint32_t ndf)
{
    SPIC_REG_FIELD_WR(SPIC, TX_NDF, TX_NDF, ndf);
}

void spic_set_dr(SPIC_TypeDef *SPIC, uint32_t data, SPIC_CFG_DATA_LEN len)
{
    if (len == SPIC_DATA_BYTE)
    {
        SPIC_REG_WR(SPIC, DR[0].BYTE, data & 0xFF);
    }
    else if (len == SPIC_DATA_HALF)
    {
        SPIC_REG_WR(SPIC, DR[0].HALF, rtk_cpu_to_le16(data & 0xFFFF));
    }
    else
    {
        SPIC_REG_WR(SPIC, DR[0].WORD, rtk_cpu_to_le32(data));
    }
}

uint32_t spic_get_dr(SPIC_TypeDef *SPIC, SPIC_CFG_DATA_LEN len)
{
    if (len == SPIC_DATA_BYTE)
    {
        return (SPIC_REG_RD(SPIC, DR[0].BYTE) & 0xFF);
    }
    else if (len == SPIC_DATA_HALF)
    {
        return (rtk_cpu_to_le16(SPIC_REG_RD(SPIC, DR[0].HALF) & 0xFFFF));
    }
    else
    {
        return SPIC_REG_RD(SPIC, DR[0].WORD);
    }
}

void spic_set_cmd_len(SPIC_TypeDef *SPIC, uint8_t len)
{
    SPIC_REG_FIELD_WR(SPIC, USER_LENGTH, USER_CMD_LENGTH, len);
}

void spic_set_user_addr_len(SPIC_TypeDef *SPIC, uint8_t len)
{
    SPIC_REG_FIELD_WR(SPIC, USER_LENGTH, USER_ADDR_LENGTH, len);
}

void spic_set_auto_addr_len(SPIC_TypeDef *SPIC, uint8_t len)
{
    SPIC_REG_FIELD_WR(SPIC, AUTO_LENGTH, AUTO_ADDR_LENGTH, len);
}

void spic_set_delay_len(SPIC_TypeDef *SPIC, uint8_t delay_len)
{
    SPIC_REG_FIELD_WR(SPIC, AUTO_LENGTH, IN_PHYSICAL_CYC, delay_len);
}

void spic_set_user_dummy_len(SPIC_TypeDef *SPIC, uint8_t dummy_len)
{
    SPIC_REG_FIELD_WR(SPIC, USER_LENGTH, USER_RD_DUMMY_LENGTH, dummy_len);
}

void spic_set_auto_dummy_len(SPIC_TypeDef *SPIC, uint8_t dummy_len)
{
    SPIC_REG_FIELD_WR(SPIC, AUTO_LENGTH, RD_DUMMY_LENGTH, dummy_len);
}

void spic_set_baud(SPIC_TypeDef *SPIC, uint16_t baud)
{
    SPIC_REG_FIELD_WR(SPIC, BAUDR, SCKDV, baud);
}

uint16_t spic_get_baud(SPIC_TypeDef *SPIC)
{
    return SPIC_REG_FIELD_RD(SPIC, BAUDR, SCKDV);
}

void spic_set_multi_ch(SPIC_TypeDef *SPIC, SPIC_CFG_CH cmd, SPIC_CFG_CH addr, SPIC_CFG_CH data)
{
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, CMD_CH, cmd);
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, ADDR_CH, addr);
    SPIC_REG_FIELD_WR(SPIC, CTRLR0, DATA_CH, data);
}

void spic_set_seq_trans(SPIC_TypeDef *SPIC, bool enable)
{
    SPIC_REG_FIELD_WR(SPIC, VALID_CMD, SEQ_TRANS_EN, enable);
}
