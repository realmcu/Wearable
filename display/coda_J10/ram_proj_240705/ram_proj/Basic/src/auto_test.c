#include "rtl8763b_uart.h"


void auto_test_init(void)
{
    AutoTest_Uart_Init();
}

uint8_t rxBuf[256];
uint8_t rxCount = 0;

void AUTOTEST_UART_Handler(void)
{
    uint8_t IID = (AUTOTEST_UART->INTID_FCR & (0x0000000E));

    switch (IID)
    {
    /* tx fifo empty */
    case 0x02:
        /* do nothing */
        break;

    /* rx data valiable */
    case 0x04:
        while (BB3UART_GetFlagState(AUTOTEST_UART, UART_FLAG_RX_DATA_RDY) == 1)
        {
            rxBuf[rxCount] = AUTOTEST_UART->RB_THR;
            rxCount++;
        }
        break;

    /*rx time out*/
    case 0x0c:
        while (BB3UART_GetFlagState(AUTOTEST_UART, UART_FLAG_RX_DATA_RDY) == 1)
        {
            rxBuf[rxCount] = AUTOTEST_UART->RB_THR;
            rxCount++;
        }
        break;

    /* receive line status interrupt */
    case 0x06:
        break;

    default:
        break;
    }
}
