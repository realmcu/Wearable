#include <stdio.h>
#include <stdarg.h>
#include "hwtest_config.h"
#include "log_adapter.h"
#include "rtl876x_uart.h"
#include "rtl876x_rcc.h"
#include "rtl876x_pinmux.h"


#if (LOG_TO_SELECT == LOG_TO_SWD)

#elif (LOG_TO_SELECT == LOG_TO_RAM)


#error "Please configure time function required low level support."

#endif

void log_uart_hw_init(void)
{
    UART_InitTypeDef uart_init_struct;



    /* Pinmux Setting */

    Pinmux_Deinit(P2_0); /* P2_0 is assigned as the default log pin */


    /* Baudrate Setting */
    UART_StructInit(&uart_init_struct);
    UART_ComputeDiv(&uart_init_struct.div, &uart_init_struct.ovsr, &uart_init_struct.ovsr_adj,
                    (UartBaudRate_TypeDef)BAUD_RATE_2000000);

    uart_init_struct.dmaEn = UART_DMA_ENABLE;
    uart_init_struct.TxDmaEn = ENABLE;
    uart_init_struct.UART_TxWaterLevel = 8;

    uint8_t log_channel = 1;
    switch (log_channel)
    {
    case 0:
        Pinmux_Config(P2_0, UART0_TX);
        UART_HwClock(UART0, true);
        UART_Init(UART0, &uart_init_struct);
        break;

    case 1:
        Pinmux_Config(P2_0, UART1_TX);
        UART_HwClock(UART1, true);
        UART_Init(UART1, &uart_init_struct);
        break;

    case 2:
        Pinmux_Config(P2_0, UART2_TX);
        UART_HwClock(UART2, true);
        UART_Init(UART2, &uart_init_struct);
        break;
    }

    Pad_Config(P2_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_LOW);

}



void log_init(void)
{
    log_uart_hw_init();
}

void log_print(char *fmt, ...)
{
    va_list ap;
    int log_len;
    char log_msg[256] = {0};

    va_start(ap, fmt);

    log_len = vsnprintf(log_msg, 256, fmt, ap);
    if (log_len < 0)
    {
        while (1); /* error */
    }

    va_end(ap);

#if (LOG_TO_SELECT == LOG_TO_UART)
//    serial_puts(log_msg);

#elif (LOG_TO_SELECT == LOG_TO_SWD)

#elif (LOG_TO_SELECT == LOG_TO_RAM)

#else
#error "Please configure time function required low level support."

#endif
}
