/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     main.c
* @brief    This file provides demo code of SPI comunication in DLPS situation. Please add file of board.h
* @details
* @author   elliot chen
* @date     2015-06-04
* @version  v0.1
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "string.h"
#include "rtl876x_rcc.h"
#include "rtl876x_spi.h"
#include "rtl876x_nvic.h"
#include "rtl876x_pinmux.h"
#include "trace.h"
#include "io_dlps.h"
#include "pm.h"
#include "os_sched.h"
#include "os_task.h"
#include "os_msg.h"

/** @defgroup  SPI_DLPS_DEMO  SPI DLPS DEMO
    * @brief
    * @{
    */


/* Global variables------------------------------------------------------------*/
uint8_t IO_DEMO_EVENT_SYSTEM_WAKE = 0x01;

//xQueueHandle DemoIOEventQueue;
void *io_queue_handle;
void *io_test_task_handle;

/* Demo code-------------------------------------------------------------------*/
void RCC_Configuration(void)
{
    /* turn on SPI clock */
    RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);
}

void SPI_PINMUXConfiguration(void)
{
    Pinmux_Config(SPI0_SCK, SPI0_CLK_MASTER);
    Pinmux_Config(SPI0_MOSI, SPI0_MO_MASTER);
    Pinmux_Config(SPI0_MISO, SPI0_MI_MASTER);
    Pinmux_Config(SPI0_CS, SPI0_SS_N_0_MASTER);
}

void SPI_PADConfiguration(void)
{
    Pad_Config(SPI0_SCK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(SPI0_MOSI, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(SPI0_MISO, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(SPI0_CS, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
}

void SPI_Configuration(void)
{
    SPI_InitTypeDef  SPI_InitStructure;

    SPI_DeInit(SPI0);
    RCC_Configuration();
    SPI_PINMUXConfiguration();
    SPI_PADConfiguration();

    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction   = SPI_Direction_FullDuplex;
    SPI_InitStructure.SPI_Mode        = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize    = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL        = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA        = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_BaudRatePrescaler  = 100;
    SPI_InitStructure.SPI_FrameFormat = SPI_Frame_Motorola;

    SPI_Init(SPI0, &SPI_InitStructure);
    SPI_Cmd(SPI0, ENABLE);
}

/*--------------------------SPI demo code----------------------------------*/
void SPI_DemoCode(void)
{
    uint8_t SPI_WriteBuf[16] = {0x01, 0x02, 0x00};
    uint8_t SPI_ReadBuf[16] = {0, 0, 0, 0};
    uint8_t idx = 0;
    uint8_t len = 0;

    /*---------------read flash ID--------------*/
    SPI_WriteBuf[0] = 0x9f;
    SPI_SendBuffer(SPI0, SPI_WriteBuf, 4);

    /*Waiting for SPI data transfer to end*/
    while (SPI_GetFlagState(SPI0, SPI_FLAG_BUSY));

    /*read ID number of flash GD25Q20*/
    len = SPI_GetRxFIFOLen(SPI0);
    for (idx = 0; idx < len; idx++)
    {
        SPI_ReadBuf[idx] = SPI_ReceiveData(SPI0);
        APP_PRINT_INFO2("SPI_ReadBuf[%d] = 0x%x", idx, SPI_ReadBuf[idx]);
    }
}
/*--------------------- Io test task-------------------------*/
void IOTestTask(void *pParameters)
{
    uint8_t event;
    uint8_t idx = 0;

    while (true)
    {
        if (os_msg_recv(io_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            /* system wakeup event */
            if (event == IO_DEMO_EVENT_SYSTEM_WAKE)
            {
                APP_PRINT_INFO0("IO test-->Task is running!");
                /*-------print SPI register-----------------*/
                for (idx = 0; idx < 13; idx++)
                {
                    APP_PRINT_INFO2("SPI0_StoreReg[%d] = 0x%2x!", idx, *((uint32_t *)(SPI0_REG_BASE + idx * 4)));
                }
                SPI_DemoCode();
            }
        }
    }
}

void DlpsExitCallback(void)
{
    APP_PRINT_INFO0("DlpsExitCallback");

    uint8_t event = IO_DEMO_EVENT_SYSTEM_WAKE;

    if (os_msg_send(io_queue_handle, &event, 0) == false)
    {
        APP_PRINT_ERROR0("Send queue error!!!!");
    }
}

/**
* @brief  PwrMgr_Init() contains the setting about power mode.
*
* @param   No parameter.
* @return  void
*/
void PwrMgr_Init(void)
{
    io_dlps_register();
    io_dlps_register_exit_cb(DlpsExitCallback);
    bt_power_mode_set(BTPOWER_DEEP_SLEEP);
    power_mode_set(POWER_DLPS_MODE);
}

int main(void)
{
    /*enable DLPS*/
    PwrMgr_Init();
    /*SPI init and read GD25Q20 ID*/
    SPI_Configuration();
    SPI_DemoCode();

    /* create event queue and message queue */
    os_msg_queue_create(&io_queue_handle, "spiQ", IO_DEMO_EVENT_QUEUE_SIZE, sizeof(uint8_t));
    os_task_create(&io_test_task_handle, "IO driver test", IOTestTask, NULL, IOTest_STACK_SIZE, 2);

    os_sched_start();
}

/** @} */ /* End of group SPI_DLPS_DEMO */

