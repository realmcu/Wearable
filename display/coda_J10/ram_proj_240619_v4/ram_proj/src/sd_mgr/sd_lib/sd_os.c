/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sd_os.c
* @brief    This file provides sd os operation for sd library.
* @details
* @author   elliot_chen
* @date     2021-05-11
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "sd.h"
#ifdef ASYNC_MODE_ENABLE
#include "os_msg.h"
#include "sd_os.h"

/* Internal Defines ----------------------------------------------------------*/
#define SD_OS_IO_MSG_NUM               0x40
#define SD_OS_EVT_NUM                  0x40
#define EVENT_IO_TO_SD                 0x0B

/* Globals -------------------------------------------------------------------*/
void *p_sd_queue_evt_handle;
void *p_sd_queue_io_handle;

uint8_t send_sdh_msg_to_task(T_IO_MSG *p_msg)
{
    uint32_t fail_line = 0;
    uint8_t event = EVENT_IO_TO_SD;

    if (os_msg_send(p_sd_queue_io_handle, p_msg, 0) == false)
    {
        fail_line = __LINE__;
        goto fail;
    }
    if (os_msg_send(p_sd_queue_evt_handle, &event, 0) == false)
    {
        fail_line = __LINE__;
        goto fail;
    }
    return 1;

fail:
    SDIO_PRINT_ERROR1("send msg to sd task fail: fail line %d", fail_line);
    return 0;
}

uint8_t recv_sdh_msg(void)
{
    uint8_t event;

    if (os_msg_recv(p_sd_queue_evt_handle, &event, 0xFFFFFFFF) == true)
    {
        if (event == EVENT_IO_TO_SD)
        {
            T_IO_MSG sd_msg;
            if (os_msg_recv(p_sd_queue_io_handle, &sd_msg, 0) == true)
            {
                return 1;
            }
        }
    }

    return 0;
}

uint8_t sd_create_queue(void)
{
    uint32_t fail_line = 0;
    if (!os_msg_queue_create(&p_sd_queue_io_handle, "ioQ", SD_OS_IO_MSG_NUM, sizeof(T_IO_MSG)))
    {
        fail_line = __LINE__;
        goto fail;
    }

    if (!os_msg_queue_create(&p_sd_queue_evt_handle, "evtQ", SD_OS_EVT_NUM, sizeof(uint8_t)))
    {
        fail_line = __LINE__;
        goto fail;
    }

    return 1;

fail:
    SDIO_PRINT_ERROR1("sd_create_queue: create failure, fail line %d", fail_line);
    return 0;
}
#endif

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

