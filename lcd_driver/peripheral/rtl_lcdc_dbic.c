/*
 * @Author: your name
 * @Date: 2021-10-18 14:08:27
 * @LastEditTime: 2021-11-22 19:07:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \wristband_rttd:\project\Bee3Pro\HoneyComb\sdk\src\mcu\peripheral\rtl876x_dbic.c
 */
/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl_dbic.c
* @brief    This file provides all the DBI firmware functions.
* @details
* @author   boris yue
* @date     2021-08-23
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl_lcdc_dbic.h"


void DBIC_Init(LCDC_DBICCfgTypeDef *DBICCfg)
{
    LCDC_AXIMUXMode(LCDC_FW_MODE);

    /* set cs and baudrate */
    DBIC_CTRLR0_t dbic_reg_0x00 = {.d32 = DBIC->CTRLR0};
    dbic_reg_0x00.b.user_mode = DBIC_USER_MODE;
    dbic_reg_0x00.b.scpol = DBICCfg->SCPOL;
    dbic_reg_0x00.b.scph = DBICCfg->SCPH;
    DBIC->CTRLR0 = dbic_reg_0x00.d32;

    DBIC_Select();

    DBIC_ICR_t dbic_reg_0x48 = {.d32 = DBIC->ICR};
    dbic_reg_0x48.b.icr = 0;
    DBIC->ICR = dbic_reg_0x48.d32;

    DBIC_TXFTLR_t dbic_reg_0x18 = {.d32 = DBIC->TXFTLR};
    dbic_reg_0x18.b.tft = DBICCfg->DBIC_TxThr;
    DBIC->TXFTLR = dbic_reg_0x18.d32;

    DBIC_RXFTLR_t dbic_reg_0x1c = {.d32 = DBIC->RXFTLR};
    dbic_reg_0x1c.b.rft = DBICCfg->DBIC_RxThr;
    DBIC->RXFTLR = dbic_reg_0x1c.d32;

    DBIC_BAUDR_t dbic_reg_0x14 = {.d32 = DBIC->BAUDR};
    dbic_reg_0x14.b.sckdv = DBICCfg->DBIC_SPEED_SEL;
    DBIC->BAUDR = dbic_reg_0x14.d32;
}

void DBIC_SendBuf(uint8_t *buf, uint32_t len)
{
    DBIC_Select();

    DBIC_SSIENR_t dbic_reg_0x08 = {.d32 = DBIC->SSIENR};
    dbic_reg_0x08.b.spic_en = 0;
    DBIC->SSIENR = dbic_reg_0x08.d32;//disable DBIC

    DBIC_CTRLR0_t dbic_reg_0x00 = {.d32 = DBIC->CTRLR0};
    dbic_reg_0x00.b.cmd_ch = DBIC_CH_SINGLE;
    dbic_reg_0x00.b.data_ch = DBIC_CH_SINGLE;
    dbic_reg_0x00.b.addr_ch = DBIC_CH_SINGLE;
    dbic_reg_0x00.b.tmod = DBIC_TMODE_TX;//tx mode
    DBIC->CTRLR0 = dbic_reg_0x00.d32;

    DBIC_FLUSH_FIFO_t dbic_reg_0x128 = {.d32 = DBIC->FLUSH_FIFO};
    dbic_reg_0x128.b.flush_dr_fifo = 1;
    DBIC->FLUSH_FIFO = dbic_reg_0x128.d32;

    //DBIC_TX_NDF(4);
    //LCDC_SetTxPixelLen(2);
    DBIC_CmdLength(1);
    DBIC_AddrLength(3);

    for (uint32_t i = 0; i < 4; i++)
    {
        DBIC->DR[0].byte = buf[i];
    }
    dbic_reg_0x08.b.spic_en = 1;
    DBIC->SSIENR = dbic_reg_0x08.d32;//enable DBIC

    DBIC_SR_t dbic_reg_0x28;
    do
    {
        dbic_reg_0x28.d32 = DBIC->SR;
    }
    while (dbic_reg_0x28.b.busy);  // wait bus busy

    dbic_reg_0x08.b.spic_en = 0;
    DBIC->SSIENR = dbic_reg_0x08.d32;//disable DBIC
}

void DBIC_ReceiveBuf(uint16_t addr, uint16_t data_len, uint8_t *data)
{
    uint32_t rd_num = 0;
    DBIC_SSIENR_t dbic_reg_0x08 = {.d32 = DBIC->SSIENR};
    dbic_reg_0x08.b.spic_en = 0;
    DBIC->SSIENR = dbic_reg_0x08.d32;//disable DBIC

    //set addr channel and data channel in ctrl0 register.
    DBIC_CTRLR0_t dbic_reg_0x00 = {.d32 = DBIC->CTRLR0};
    dbic_reg_0x00.b.cmd_ch = DBIC_CH_SINGLE;
    dbic_reg_0x00.b.data_ch = DBIC_CH_SINGLE;
    dbic_reg_0x00.b.addr_ch = DBIC_CH_SINGLE;
    dbic_reg_0x00.b.tmod = DBIC_TMODE_RX;
    DBIC->CTRLR0 = dbic_reg_0x00.d32;//rx mode

    //spic_set_ctrl1(data_len);
    DBIC_RX_NDF_t dbic_reg_0x04 = {.d32 = DBIC->RX_NDF};
    dbic_reg_0x04.b.rx_ndf = data_len;
    DBIC->RX_NDF = dbic_reg_0x04.d32;

    DBIC_USER_LENGTH_t dbic_reg_0x118 = {.d32 = DBIC->USER_LENGTH};
    dbic_reg_0x118.b.user_addr_length = 3;
    //set dummy_cycle in autolen register[11:0]
    dbic_reg_0x118.b.user_rd_dummy_length = 2;
    DBIC->USER_LENGTH = dbic_reg_0x118.d32;

    addr = addr << 8;
    DBIC->DR[0].word = 0x03 | __REV(addr);

    dbic_reg_0x08.b.spic_en = 1;
    DBIC->SSIENR = dbic_reg_0x08.d32;

    DBIC_SR_t dbic_reg_0x28;
    while (rd_num < data_len)
    {
        dbic_reg_0x28.d32 = DBIC->SR;
        if (dbic_reg_0x28.b.rfne)
        {
            data[rd_num++] = DBIC->DR[0].byte;
        }
    }

    dbic_reg_0x08.b.spic_en = 0;
    DBIC->SSIENR = dbic_reg_0x08.d32;
}



