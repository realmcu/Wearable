/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "rtl876x.h"
#include "trace.h"
#include "utils.h"
#include "rtl876x_uart.h"

uint8_t log_seq_num = 0;

uint64_t trace_mask[LEVEL_NUM] =
{
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
    0xFFFFFFFFFFFFFFFF,
};

void LogUartTxData(uint8_t *data, uint32_t len)
{
//    if (boot_cfg.logDisable)
//    {
//        return;
//    }

    UART_TypeDef *log_uart;
    //UART_TypeDef *uarts[] = {LOG_UART, LOG_UART1, UART};
    UART_TypeDef *uarts[] = {UART0, UART1, UART2};

    log_uart = uarts[1];

#ifdef FOR_SIMULATION
    for (int i = 8; i < len; ++ i)
    {
        *((volatile uint8_t *)(FAKE_UART_ADDRESS)) = data[i];
    }
    *((volatile uint8_t *)(FAKE_UART_ADDRESS)) = '\n';
#else
    UART_TxData(log_uart, data, len);
#endif
}

void log_out(uint8_t  *l_msg)
{
    LogUartTxData(l_msg, l_msg[1]);
}

uint32_t sys_timestamp_get_rom(void)
{
    return (HAL_READ32(BT_VENDOR_REG_BASE, 0x17C) & 0x3FFFFFF);
}

UINT32_PATCH_FUNC sys_timestamp_get_patch = (UINT32_PATCH_FUNC)sys_timestamp_get_rom;
UINT32_PATCH_FUNC log_timestamp_get_patch = (UINT32_PATCH_FUNC)sys_timestamp_get_rom;

uint32_t sys_timestamp_get(void)
{
    return sys_timestamp_get_patch();
}

uint32_t log_timestamp_get(void)
{
    return log_timestamp_get_patch();
}

#define LOG_FORMAT_VERSION 0
void trace_header_build(uint8_t *l_msg, uint32_t info)
{
    uint32_t timestamp = log_timestamp_get();
    l_msg[0] = LOG_MESSAGE_SYNC_CODE;
    l_msg[4] = (uint8_t)LOG_FORMAT_VERSION;
    l_msg[6] = GET_TYPE(info);
    l_msg[7] = GET_SUBTYPE(info);
    memcpy(&l_msg[8], &timestamp, 4);
    l_msg[12] = GET_MODULE(info);
}

void log_direct_rom(uint32_t info, const char *fmt, ...)
{
    char  l_msg[MAX_LOG_MESSAGE_LEN];
    uint16_t log_length;
    int16_t  fmt_length;
    va_list  ap;
//    uint32_t  s;

    /**
     * Byte: Description
     * 0:    Sync(0x7E)
     * 1:    Length
     * 2:    SeqNum
     * 3:    CheckSum
     * 4:    version
     * 5:    reserved
     * 6:    type
     * 7:    subtype
     * 8-11:  Timestamp
     */

    trace_header_build((uint8_t *)l_msg, info);

    log_length = 12;

    va_start(ap, fmt);

    fmt_length = vsnprintf(l_msg + 12, MAX_LOG_MESSAGE_LEN - 12, fmt, ap);

    if (fmt_length < 0) /* error occurred */
    {
        fmt_length = 0;
    }
    else if (log_length + fmt_length > MAX_LOG_MESSAGE_LEN) /* truncated */
    {
        log_length = MAX_LOG_MESSAGE_LEN;

    }
    else
    {
        log_length += fmt_length;
    }
    va_end(ap);

    l_msg[1] = log_length;

//    s = os_lock();
    l_msg[2] = log_seq_num++;
//    os_unlock(s);

    l_msg[3] = l_msg[0] ^ l_msg[1] ^ l_msg[2];

    LogUartTxData((uint8_t *)l_msg, log_length);
}

void (*log_direct)(uint32_t info, const char *fmt, ...) = log_direct_rom;
