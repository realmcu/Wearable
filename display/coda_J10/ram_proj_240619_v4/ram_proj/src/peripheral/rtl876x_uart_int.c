/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl_uart_int.c
* @brief    This file provides all the UART firmware internal functions.
* @details
* @author   colin
* @date     2024-06-13
* @version  v1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_uart.h"
#include "pcc_reg.h"
#include "platform_utils.h"

/**
  * @brief  TX data to tx FIFO.
  * @param  UARTx: selected UART peripheral.
  * @param  data: buffer to be written to Tx FIFO.
  * @param  len: len of data to be written.
  * @return None
  */
void UART_TxData(UART_TypeDef *UARTx, uint8_t *data, uint32_t len)
{
    uint32_t blkcount  = len / UART_TX_FIFO_SIZE;
    uint32_t remainder = len % UART_TX_FIFO_SIZE;
    uint32_t i = 0;

    for (i = 0; i < blkcount; ++i)
    {
        UART_SendData(UARTx, data + UART_TX_FIFO_SIZE * i, UART_TX_FIFO_SIZE);
        /* wait tx fifo empty */
        while (UART_GetFlagState(UARTx, UART_FLAG_TX_EMPTY) != SET);
    }

    /* send left bytes */
    if (remainder)
    {
        UART_SendData(UARTx, data + UART_TX_FIFO_SIZE * i, remainder);
        /* wait tx fifo empty */
        while (UART_GetFlagState(UARTx, UART_FLAG_TX_EMPTY) != SET);
    }
}

/**
 * @brief   UART one wire config.
 * @param   UARTx: UART peripheral selected.
 * @param   NewState: UART one wire config is set or not.
 *          This parameter can be one of the following values:
 *          @arg true: UART one wire config is set.
 *          @arg false: UART one wire config is unset.
 * @return  None.
 */
void UART_OneWireConfig(UART_TypeDef *UARTx, bool is_enable)
{
    if (UARTx == UART0)
    {
        SYSBLKCTRL->u_2AC.BITS_2AC.r_PMUX_UART0_1_W_EN = is_enable;
    }
    else if (UARTx == UART1)
    {
        SYSBLKCTRL->u_2AC.BITS_2AC.r_PMUX_UART1_1_W_EN = is_enable;
    }
    else if (UARTx == UART2)
    {
        SYSBLKCTRL->u_2AC.BITS_2AC.r_PMUX_UART2_1_W_EN = is_enable;
    }
    else if (UARTx == UART3)
    {
        SYSBLKCTRL->u_2AC.BITS_2AC.r_PMUX_UART3_1_W_EN = is_enable;
    }
    else if (UARTx == UART4)
    {
        SYSBLKCTRL->u_2AC.BITS_2AC.r_PMUX_UART4_1_W_EN = is_enable;
    }
    else if (UARTx == UART5)
    {
        SYSBLKCTRL->u_2AC.BITS_2AC.r_PMUX_UART5_1_W_EN = is_enable;
    }
}

/**
  * @brief  UART clock divider config.
  * @param  UARTx: selected UART peripheral.
  * @param  ClockDiv: specifies the APB peripheral to gates its clock.
  *      this parameter can be one of the following values:
  *     @arg UART_CLOCK_DIV_1
  *     @arg UART_CLOCK_DIV_2
  *     @arg UART_CLOCK_DIV_4
  *     @arg UART_CLOCK_DIV_16
  * @retval None
  */
void UART_ClkDivConfig(UART_TypeDef *UARTx, uint16_t ClockDiv)
{
    assert_param(IS_UART_DIV(ClockDiv));

    /* Config UART clock divider */
    if (UARTx == UART0)
    {
        /* disable clock first */
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART0_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_UART0_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART0_EN = 1;
    }
    else if (UARTx == UART1)
    {
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART1_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_UART1_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART1_EN = 1;
    }
    else if (UARTx == UART2)
    {
        /* disable clock first */
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART2_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_UART2_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART2_EN = 1;
    }
    else if (UARTx == UART3)
    {
        /* disable clock first */
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART3_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_UART3_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART3_EN = 1;
    }
    else if (UARTx == UART4)
    {
        /* disable clock first */
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART4_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_UART4_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART4_EN = 1;
    }
    else if (UARTx == UART5)
    {
        /* disable clock first */
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART5_EN = 0;
        platform_delay_us(1);

        SYSBLKCTRL->u_35C.BITS_35C.BIT_PERI_UART5_CLK_DIV = ClockDiv;

        platform_delay_us(1);
        SYSBLKCTRL->u_23C.BITS_23C.BIT_SOC_ACTCK_UART5_EN = 1;
    }

    return;
}

const static uint32_t uart_baudrate_map[] =
{
    1200, 4800, 7200, 9600, 14400, 19200, 28800, 38400, 57600, 76800, 115200, 128000, 153600, 230400, 460800, 500000, 921600, 1000000, 1382400, 1444400, 1500000, 1843200, 2000000, 3000000, 4000000
};

/**
  *@brief  baudrate convert to UartBaudRate_TypeDef
  *@param  baudrate: select UART uint32_t baudrate
  *@retval Unsupport baudrate return 0xff
  */
UartBaudRate_TypeDef UART_ConvUartBaudRate(uint32_t baudrate)
{
    for (uint8_t i = 0; i < sizeof(uart_baudrate_map) / sizeof(uint32_t); i++)
    {
        if (uart_baudrate_map[i] == baudrate)
        {
            return (UartBaudRate_TypeDef)i;
        }
    }
    return (UartBaudRate_TypeDef)0xff;
}

/**
  *@brief  UartBaudRate_TypeDef convert to baudrate
  *@param  baudrate: select UART UartBaudRate_TypeDef baudrate
  *@retval Unsupport baudrate return 0
  */
uint32_t UART_ConvRateValue(UartBaudRate_TypeDef baudrate)
{
    if (baudrate >= sizeof(uart_baudrate_map) / sizeof(uint32_t))
    {
        return 0;
    }
    return uart_baudrate_map[baudrate];
}

/**
  *@brief  According to baudrate get UART param, start bit + 8 data bit + stop bit
  *@param[out]  div: div for setting baudrate
  *@param[out]  ovsr: ovsr for setting baudrate
  *@param[out]  ovsr_adj: ovsr_adj for setting baudrate
  *@param[in]   rate: select UART UartBaudRate_TypeDef baudrate
  *@retval Unsupport baudrate return false
  */
bool UART_ComputeDiv(uint16_t *div, uint16_t *ovsr, uint16_t *ovsr_adj, UartBaudRate_TypeDef rate)
{
    switch (rate)
    {
    case BAUD_RATE_1200:
        *div = 0x91b;
        *ovsr = 0x9;
        *ovsr_adj = 0x112;
        break;
    case BAUD_RATE_4800:
        *div = 0x682;
        *ovsr = 0x0;
        *ovsr_adj = 0x0;
        break;
    case BAUD_RATE_7200:
        *div = 0x457;
        *ovsr = 0x0;
        *ovsr_adj = 0x0;
        break;
    case BAUD_RATE_9600:
        *div = 0x341;
        *ovsr = 0x0;
        *ovsr_adj = 0x0;
        break;
    case BAUD_RATE_14400:
        *div = 0xc5;
        *ovsr = 0x9;
        *ovsr_adj = 0x10;
        break;
    case BAUD_RATE_19200:
        *div = 0x150;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_28800:
        *div = 0xe0;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_38400:
        *div = 0xa8;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_57600:
        *div = 0x70;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_76800:
        *div = 0x54;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_115200:
        *div = 0x38;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_128000:
        *div = 25;
        *ovsr = 7;
        *ovsr_adj = 0x555;
        break;
    case BAUD_RATE_153600:
        *div = 15;
        *ovsr = 12;
        *ovsr_adj = 0x252;
        break;
    case BAUD_RATE_230400:
        *div = 0x1c;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_460800:
        *div = 0xe;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_500000:
        *div = 0x8;
        *ovsr = 0x5;
        *ovsr_adj = 0x0;
        break;
    case BAUD_RATE_921600:
        *div = 0x7;
        *ovsr = 0x1;
        *ovsr_adj = 0x3;
        break;
    case BAUD_RATE_1000000:
        *div = 0x8;
        *ovsr = 0x0;
        *ovsr_adj = 0x0;
        break;
    case BAUD_RATE_1382400:
        *div = 0x2;
        *ovsr = 0x9;
        *ovsr_adj = 0x2AA;
        break;
    case BAUD_RATE_1444400:
        *div = 0x2;
        *ovsr = 0x8;
        *ovsr_adj = 0x3f7;
        break;
    case BAUD_RATE_1500000:
        *div = 0x2;
        *ovsr = 0x8;
        *ovsr_adj = 0x92;
        break;
    case BAUD_RATE_1843200:
        *div = 0x2;
        *ovsr = 0x5;
        *ovsr_adj = 0x3F7;
        break;
    case BAUD_RATE_2000000:
        *div = 0x4;
        *ovsr = 0x0;
        *ovsr_adj = 0x0;
        break;
    case BAUD_RATE_3000000:
        *div = 0x1;
        *ovsr = 0x8;
        *ovsr_adj = 0x92;
        break;
    case BAUD_RATE_4000000:
        *div = 0x1;
        *ovsr = 0x5;
        *ovsr_adj = 0;
        break;
    default:
        return false;
    }
    return true;
}

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/

