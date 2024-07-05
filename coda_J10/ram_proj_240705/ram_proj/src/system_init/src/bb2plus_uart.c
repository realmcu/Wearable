#include "hwtest_config.h"
#include "bblite_uart.h"


#define LCR_DLAB_Set                    ((uint32_t)(1 << 7))
#define LCR_DLAB_Reset                  ((uint32_t)~(1 << 7))

#define FCR_CLEAR_RX_FIFO_Set           ((uint32_t)(1 << 1))
#define FCR_CLEAR_RX_FIFO_Reset         ((uint32_t)~(1 << 1))

#define FCR_CLEAR_TX_FIFO_Set           ((uint32_t)(1 << 2))
#define FCR_CLEAR_TX_FIFO_Reset         ((uint32_t)~(1 << 2))

const UartBaudrateVal   CBaudrateTable[10] =
{
    {271, 10, 0x24A}, // BAUD_RATE_9600
    {165, 7,  0x5AD}, // BAUD_RATE_19200
    {20,  12, 0x252}, // BAUD_RATE_115200
    {10,  12, 0x252}, // BAUD_RATE_230400
    {5,   12, 0x252}, // BAUD_RATE_460800
    {4,   5,  0x3F7}, // BAUD_RATE_921600
    {2,   5,  0},     // BAUD_RATE_2000000
    {1,   8,  0x492}, // BAUD_RATE_3000000
    {1,   5,  0},     // BAUD_RATE_4000000
    {1,   1,  0x36D}, // BAUD_RATE_6000000
};


void HW_Uart_Init(void)
{
    /* RCC config */
    LOG_FUN_EN_REG |= LOG_FUN_EN_BIT_MASK;
    LOG_CLK_REG |= (LOG_ATK_BIT_MASK | LOG_SLK_BIT_MASK);

////    /* Pinmux config P2_1*/
//    *(volatile uint32_t *)(0x40000290UL) &= ~(0xff);
//    *(volatile uint32_t *)(0x40000290UL) |= (LOG_PINMUX_FUN);

    /* Pinmux config P0_3*/
//    *(volatile uint32_t *)(0x40000280UL) &= ~((0xff<<24));
//    *(volatile uint32_t *)(0x40000280UL) |= (LOG_PINMUX_FUN<<24);
//
//      *(volatile uint32_t *)(0x40000048UL) = 0x01060199;
//      *(volatile uint32_t *)(0x40000048UL) = 0x01060198;

//  tmp=  AON_Read(0x83);
//  tmp |= BIT6;
//  AON_Write(0x83,tmp);
//  tmp=  AON_Read(0x83);
//   tmp =  AON_Read(0x109);
//    tmp |= BIT6;
//    AON_Write(0x109, tmp);
//    tmp =  AON_Read(0x109);
    /* Pad config P2_0, Pinmux mode, pull none*/

    /* UART register setting */
    //clear DLAB bit
    TEST_LOG_UART->LCR &= LCR_DLAB_Reset;
    //disable all interrupt
    TEST_LOG_UART->DLH_INTCR = 0x00;
    //read to clear Line Status Reg
    TEST_LOG_UART->LSR;
    //clear FIFO
    TEST_LOG_UART->INTID_FCR |= (FCR_CLEAR_RX_FIFO_Set | FCR_CLEAR_TX_FIFO_Set);
    //set baudrate, firstly set DLAB bit
    TEST_LOG_UART->LCR |= LCR_DLAB_Set;
    //set calibration parameters(OVSR)
    TEST_LOG_UART->STSR &= ~0xF0;
    TEST_LOG_UART->STSR |= (CBaudrateTable[Log_UART_Baudrate].ovsr << 4);
    //set calibration parameters(OVSR_adj)
    TEST_LOG_UART->SPR &= (~(0x7ff << 16));
    TEST_LOG_UART->SPR |= (CBaudrateTable[Log_UART_Baudrate].ovsr_adj << 16);
    //set DLL and DLH
    TEST_LOG_UART->DLL = (CBaudrateTable[Log_UART_Baudrate].div & 0x00FF);
    TEST_LOG_UART->DLH_INTCR = ((CBaudrateTable[Log_UART_Baudrate].div & 0xFF00) >> 8);
    //after set baudrate, clear DLAB bit
    TEST_LOG_UART->LCR &= LCR_DLAB_Reset;
    //set LCR reg: No parity, 1 StopBit, Data Len Byte
    TEST_LOG_UART->LCR = 0x1;
    //set FCR reg, FIFO must enable, Rx trigger level 0x8, DMA Disable.
    TEST_LOG_UART->INTID_FCR = ((1 << 0) | (0x8 << 8));
    /* auto flow control */
    TEST_LOG_UART->MCR &= (~((1 << 5) | (1 << 1)));
#ifdef   UART_FLOW_CONTROL
    TEST_LOG_UART->MCR_REG |= ((1 << 5) | (1 << 1));
#endif

    //for test:
    serial_puts("Welcome to this magical world!\r\n");
}

void AutoTest_Uart_Init(void)
{
//    uint32_t tmp;
    /* RCC config */
    AUTOTEST_FUN_EN_REG |= AUTOTEST_FUN_EN_BIT_MASK;
    AUTOTEST_CLK_REG |= (AUTOTEST_ATK_BIT_MASK | AUTOTEST_SLK_BIT_MASK);

    /* Pinmux config P0_0 TX*/
    *(volatile uint32_t *)(0x40000280UL) &= ~(0xff << 0);
    *(volatile uint32_t *)(0x40000280UL) |= (AUTOTEST_PINMUX_FUN << 0);

    /* Pinmux config P0_1 RX*/
    *(volatile uint32_t *)(0x40000280UL) &= ~(REG_8BITS_MASK << 8);
    *(volatile uint32_t *)(0x40000280UL) |= (AUTOTEST_PINMUX_RX << 8);

//     /* Pinmux config P5_4 TX*/
//    *(volatile uint32_t *)(0x400003BCUL) &= ~(0xff<<0);
//    *(volatile uint32_t *)(0x400003BCUL) |= (AUTOTEST_PINMUX_FUN<<0);
//
//    /* Pinmux config P5_5 RX*/
//    *(volatile uint32_t *)(0x400003BCUL) &= ~(REG_8BITS_MASK << 8);
//    *(volatile uint32_t *)(0x400003BCUL) |= (AUTOTEST_PINMUX_RX << 8);

    /* Pinmux config P1_2 TX*/
//    *(volatile uint32_t *)(0x40000288UL) &= ~(0xff<<16);
//    *(volatile uint32_t *)(0x40000288UL) |= (AUTOTEST_PINMUX_FUN<<16);
//
//    /* Pinmux config P1_3 RX*/
//    *(volatile uint32_t *)(0x40000288UL) &= ~(REG_8BITS_MASK << 24);
//    *(volatile uint32_t *)(0x40000288UL) |= (AUTOTEST_PINMUX_RX << 24);

    /* Pinmux config P4_0 TX*/
//    *(volatile uint32_t *)(0x400002A4UL) &= ~(0xff<<8);
//    *(volatile uint32_t *)(0x400002A4UL) |= (AUTOTEST_PINMUX_FUN<<8);
//
//    /* Pinmux config P4_1 RX*/
//    *(volatile uint32_t *)(0x400002A4UL) &= ~(REG_8BITS_MASK << 16);
//    *(volatile uint32_t *)(0x400002A4UL) |= (AUTOTEST_PINMUX_RX << 16);

    /* Pinmux config P3_0 TX*/
//    *(volatile uint32_t *)(0x40000298UL) &= ~(0xff<<0);
//    *(volatile uint32_t *)(0x40000298UL) |= (AUTOTEST_PINMUX_FUN<<0);
//
//    /* Pinmux config P3_1 RX*/
//    *(volatile uint32_t *)(0x40000298UL) &= ~(REG_8BITS_MASK << 8);
//    *(volatile uint32_t *)(0x40000298UL) |= (AUTOTEST_PINMUX_RX << 8);

    /* UART register setting */
    //clear DLAB bit
    AUTOTEST_UART->LCR &= LCR_DLAB_Reset;
    //disable all interrupt
    AUTOTEST_UART->DLH_INTCR = 0x00;
    //read to clear Line Status Reg
    AUTOTEST_UART->LSR;
    //set baudrate, firstly set DLAB bit
    AUTOTEST_UART->LCR |= LCR_DLAB_Set;
    //set calibration parameters(OVSR)
    AUTOTEST_UART->STSR &= ~0xF0;
    AUTOTEST_UART->STSR |= (CBaudrateTable[AUTOTEST_UART_Baudrate].ovsr << 4);
    //set calibration parameters(OVSR_adj)
    AUTOTEST_UART->SPR &= (~(0x7ff << 16));
    AUTOTEST_UART->SPR |= (CBaudrateTable[AUTOTEST_UART_Baudrate].ovsr_adj << 16);
    //set DLL and DLH
    AUTOTEST_UART->DLL = (CBaudrateTable[AUTOTEST_UART_Baudrate].div & 0x00FF);
    AUTOTEST_UART->DLH_INTCR = ((CBaudrateTable[AUTOTEST_UART_Baudrate].div & 0xFF00) >> 8);
    //after set baudrate, clear DLAB bit
    AUTOTEST_UART->LCR &= LCR_DLAB_Reset;
    //set LCR reg: No parity, 1 StopBit, Data Len Byte
    AUTOTEST_UART->LCR = 0x1;
    //clear FIFO
    AUTOTEST_UART->INTID_FCR |= (FCR_CLEAR_RX_FIFO_Set | FCR_CLEAR_TX_FIFO_Set);
    //set FCR reg, FIFO must enable, Rx trigger level 14, DMA Disable.
    AUTOTEST_UART->INTID_FCR = ((1 << 0) | (0x0E << 8));
    /* auto flow control */
    AUTOTEST_UART->MCR &= (~((1 << 5) | (1 << 1)));
#ifdef   AUTOTEST_UART_FLOW_CONTROL
    AUTOTEST_UART->MCR_REG |= ((1 << 5) | (1 << 1));
#endif

    /* Interrupt RX Available config */
    AUTOTEST_UART->DLH_INTCR |= (1 << 0);
//      AUTOTEST_UART->DLH_INTCR &= ~(1 << 0);
    NVIC_SetPriority(AUTOTEST_UART_IRQ, 3);
//    NVIC_ClearPendingIRQ(AUTOTEST_UART_IRQ);
    NVIC_EnableIRQ(AUTOTEST_UART_IRQ);
//      uint8_t buffer[5]={};
//      AUTOTEST_UART->MCR |= BIT4;
//      AUTOTEST_UART->RB_THR=0x25;
//      buffer[0]=(uint8_t)AUTOTEST_UART->RB_THR;
//      while(1);
}

uint8_t AutoTest_UART_GetFlagState(AutoTest_UART_TypeDef *UARTx, uint32_t vUART_Flag)
{
    uint8_t bitstatus = 0;

    if (UARTx->LSR & vUART_Flag)
    {
        bitstatus = 1;
    }

    return bitstatus;
}

void AutoTest_UART_ReceiveData(AutoTest_UART_TypeDef *UARTx, uint8_t *pRev_Buf, uint16_t vCount)
{
    while (vCount--)
    {
        *pRev_Buf++ = (uint8_t)UARTx->RB_THR;
    }

}

void AutoTest_UART_SendData(AutoTest_UART_TypeDef *UARTx, const uint8_t *pSend_Buf, uint16_t vCount)
{
    uint8_t count;

    while (vCount / 16 > 0)
    {
        while (AutoTest_UART_GetFlagState(UARTx, AutoTest_UART_FLAG_THR_EMPTY) == 0);
        for (count = 16; count > 0; count--)
        {
            UARTx->RB_THR = *pSend_Buf++;
        }
        vCount -= 16;
    }

    while (AutoTest_UART_GetFlagState(UARTx, AutoTest_UART_FLAG_THR_EMPTY) == 0);
    while (vCount--)
    {
        UARTx->RB_THR = *pSend_Buf++;
    }

}

uint8_t BB3UART_GetFlagState(UART_InternalTypeDef *UARTx, uint32_t UART_FLAG)
{
    uint8_t bitstatus = 0;

    if (UARTx->LSR & UART_FLAG)
    {
        bitstatus = 1;
    }

    return bitstatus;
}

void BB3UART_ReceiveData(UART_InternalTypeDef *UARTx, uint8_t *outBuf, uint16_t count)
{
    while (count--)
    {
        *outBuf++ = (uint8_t)UARTx->RB_THR;
    }

    return;
}

void LogUART_SendData(const uint8_t *inBuf, uint16_t count)
{
    while (count--)
    {
        TEST_LOG_UART->RB_THR = *inBuf++;
    }
    return;
}

void BB3UART_SendData(UART_InternalTypeDef *UARTx, const uint8_t *inBuf, uint16_t count)
{
    while (count--)
    {
        UARTx->RB_THR = *inBuf++;
    }

    return;
}

void putc(UART_InternalTypeDef *UARTx, unsigned char data)
{
    while (!(UARTx->LSR & BIT5));
    UARTx->RB_THR = data;
}

void serial_puts(const char *s)
{
    while (*s)
    {
        putc(TEST_LOG_UART, *s++);
    }
}



