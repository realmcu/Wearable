#ifndef _RTL8762C_UART_H_
#define _RTL8762C_UART_H_
//#include "core_cm4.h"
#include "rtl876x.h"//new include
#ifdef __cplusplus
extern "C" {
#endif

/*UART Structure*/
typedef struct
{
    __IO  uint32_t DLL;
    __IO  uint32_t DLH_INTCR;
    __IO  uint32_t INTID_FCR;
    __IO  uint32_t LCR;
    __IO  uint32_t MCR;     //0x10
    __I   uint32_t LSR;
    __I   uint32_t MSR;
    __IO  uint32_t SPR;
    __IO  uint32_t STSR;    //0x20
    __IO  uint32_t RB_THR;
    __IO  uint32_t MISCR;
    __IO  uint32_t TXPLSR;
    __IO  uint32_t RSVD0;           //0x30
    __IO  uint32_t BaudMONR;
    __IO  uint32_t RSVD1;
    __I   uint32_t DBG_UART;
    __IO  uint32_t RX_IDLE_INTTCR;  //0x40
    __IO  uint32_t RX_IDLE_SR;
    __IO  uint32_t RXIDLE_INTCR;
    __I   uint32_t FIFO_LEVEL;
} AutoTest_UART_TypeDef;


typedef struct                                      /*!< UART Structure                                                        */
{
    __IO  uint32_t DLL;
    __IO  uint32_t DLH_INTCR;
    __IO  uint32_t INTID_FCR;
    __IO  uint32_t LCR;
    __IO  uint32_t MCR;     //0x10
    __I   uint32_t LSR;
    __I   uint32_t MSR;
    __IO  uint32_t SPR;
    __IO  uint32_t STSR;    //0x20
    __IO  uint32_t RB_THR;
    __IO  uint32_t MISCR;
    __IO  uint32_t TXPLSR;
    __IO  uint32_t RSVD0;           //0x30
    __IO  uint32_t BaudMONR;
    __IO  uint32_t RSVD1;
    __I   uint32_t DBG_UART;
    __IO  uint32_t RX_IDLE_INTTCR;  //0x40
    __IO  uint32_t RX_IDLE_SR;
    __IO  uint32_t RXIDLE_INTCR;
    __I   uint32_t FIFO_LEVEL;
} UART_InternalTypeDef;

/* Baudrate Table */
typedef enum
{
    AT_BAUD_RATE_9600,
    AT_BAUD_RATE_19200,
    AT_BAUD_RATE_115200,
    AT_BAUD_RATE_230400,
    AT_BAUD_RATE_460800,
    AT_BAUD_RATE_921600,
    AT_BAUD_RATE_2000000,
    AT_BAUD_RATE_3000000,
    AT_BAUD_RATE_4000000,
    AT_BAUD_RATE_6000000
} AT_UartBaudRate_TypeDef;

#define  RTK_UART_0 0
#define  RTK_UART_1 1

/* UART Control Settings */
#define  LOG_UART_INDEX                   RTK_UART_1/* 0: RTK_UART_0, 1: RTK_UART_1 */
#define  AUTOTEST_UART_INDEX              RTK_UART_0  /* 0: RTK_UART_0, 1: RTK_UART_1   */
//#define  LOG_UART_INDEX                   RTK_UART_0/* 0: RTK_UART_0, 1: RTK_UART_1   */
//#define  AUTOTEST_UART_INDEX              RTK_UART_1  /* 0: RTK_UART_0, 1: RTK_UART_1 */

/* Mode Set */

#define Log_UART_Baudrate             (AT_BAUD_RATE_115200)
#define AUTOTEST_UART_Baudrate        (AT_BAUD_RATE_2000000)
//#define UART_FLOW_CONTROL





/* Register defines */
#if (LOG_UART_INDEX == RTK_UART_0) //Data UART
#define    TEST_LOG_UART            ((UART_InternalTypeDef            *) RTK_UART0_INTERNAL_REG_BASE)
#define    LOG_FUN_EN_REG           PERI_FUNC0_EN_REG_218
#define    LOG_FUN_EN_BIT_MASK      BIT0
#define    LOG_CLK_REG              PERI_CLK_CTRL0_REG_234
#define    LOG_ATK_BIT_MASK         SYSBLK_ACTCK_UART0DATA_EN_Msk
#define    LOG_SLK_BIT_MASK         SYSBLK_SLPCK_UART0DATA_EN_Msk
#define    LOG_UART_BASE_ADDR       UART_REG_INTERNAL_BASE
#define    LOG_PINMUX_FUN           (35)

#elif (LOG_UART_INDEX == RTK_UART_1) //LOG_UART
#define    TEST_LOG_UART            ((UART_InternalTypeDef            *) RTK_UART1_INTERNAL_REG_BASE)
#define    LOG_FUN_EN_REG           SOC_FUNC_EN_REG
#define    LOG_FUN_EN_BIT_MASK      BIT12
#define    LOG_CLK_REG              CLK_CTRL_REG
#define    LOG_ATK_BIT_MASK         SYSBLK_ACTCK_LOGUART_EN_Msk
#define    LOG_SLK_BIT_MASK         SYSBLK_SLPCK_LOGUART_EN_Msk
#define    LOG_UART_BASE_ADDR       LOG_UART_INTERNAL_REG_BASE
#define    LOG_PINMUX_FUN           (27)


#endif

/* Register defines */
#if (AUTOTEST_UART_INDEX == RTK_UART_0) //Data UART
#define    AUTOTEST_UART                 ((UART_InternalTypeDef *) RTK_UART0_INTERNAL_REG_BASE)
#define    AUTOTEST_FUN_EN_REG           PERI_FUNC0_EN_REG_218
#define    AUTOTEST_FUN_EN_BIT_MASK      BIT0
#define    AUTOTEST_CLK_REG              PERI_CLK_CTRL0_REG_234
#define    AUTOTEST_ATK_BIT_MASK         SYSBLK_ACTCK_UART0DATA_EN_Msk
#define    AUTOTEST_SLK_BIT_MASK         SYSBLK_SLPCK_UART0DATA_EN_Msk
#define    AUTOTEST_UART_BASE_ADDR       UART_REG_INTERNAL_BASE
#define    AUTOTEST_PINMUX_FUN           (35)
#define    AUTOTEST_PINMUX_RX            (36)
#define    AUTOTEST_UART_IRQ             (UART0_IRQn)
#define    AUTOTEST_UART_Handler         UART0_Handler

#elif (AUTOTEST_UART_INDEX == RTK_UART_1) //LOG_UART
#define    AUTOTEST_UART                ((UART_InternalTypeDef *) RTK_UART1_INTERNAL_REG_BASE)
#define    AUTOTEST_FUN_EN_REG           SOC_FUNC_EN_REG
#define    AUTOTEST_FUN_EN_BIT_MASK      BIT12
#define    AUTOTEST_CLK_REG              CLK_CTRL_REG
#define    AUTOTEST_ATK_BIT_MASK         SYSBLK_ACTCK_LOGUART_EN_Msk
#define    AUTOTEST_SLK_BIT_MASK         SYSBLK_SLPCK_LOGUART_EN_Msk
#define    AUTOTEST_UART_BASE_ADDR       LOG_UART_INTERNAL_REG_BASE
#define    AUTOTEST_PINMUX_FUN           (27)
#define    AUTOTEST_PINMUX_RX            (28)
#define    AUTOTEST_UART_IRQ             (UART1_IRQn)
#define    AUTOTEST_UART_Handler         UART1_Handler



#endif

typedef struct
{
    uint16_t div;
    uint16_t ovsr;
    uint16_t ovsr_adj;
} AutoTest_UART_BaudrateValTypeDef;

#define AutoTest_UART_FLAG_INT_PEND                         ((uint16_t)(1 << 0))
#define AutoTest_UART_FLAG_RX_DATA_RDY                      ((uint16_t)(1 << 0))
#define AutoTest_UART_FLAG_RX_OVERRUN                       ((uint16_t)(1 << 1))
#define AutoTest_UART_FLAG_PARTY_ERR                        ((uint16_t)(1 << 2))
#define AutoTest_UART_FLAG_FRAME_ERR                        ((uint16_t)(1 << 3))
#define AutoTest_UART_FLAG_BREAK_ERR                        ((uint16_t)(1 << 4))
#define AutoTest_UART_FLAG_THR_EMPTY                        ((uint16_t)(1 << 5))     //Transmitter Holding Register or Transmitter FIFO empty
#define AutoTest_UART_FLAG_THR_TSR_EMPTY                    ((uint16_t)(1 << 6))     //Transmitter Holding Register(or tx FIFO) and Transmitter shift Register both empty
#define AutoTest_UART_FLAG_RX_FIFO_ERR                      ((uint16_t)(1 << 7))
#define AutoTest_UART_FLAG_RX_IDLE                          ((uint16_t)(1 << 8))     //Only to show difference cause the address of UART RX Ilde flag is isolate

typedef struct
{
    uint16_t div;
    uint16_t ovsr;
    uint16_t ovsr_adj;
} UartBaudrateVal;

#define UART_FLAG_INT_PEND                         ((uint16_t)(1 << 0))
#define UART_FLAG_RX_DATA_RDY                      ((uint16_t)(1 << 0))
#define UART_FLAG_RX_OVERRUN                       ((uint16_t)(1 << 1))
#define UART_FLAG_PARTY_ERR                        ((uint16_t)(1 << 2))
#define UART_FLAG_FRAME_ERR                        ((uint16_t)(1 << 3))
#define UART_FLAG_BREAK_ERR                        ((uint16_t)(1 << 4))
#define UART_FLAG_THR_EMPTY                        ((uint16_t)(1 << 5))     //Transmitter Holding Register or Transmitter FIFO empty
#define UART_FLAG_THR_TSR_EMPTY                    ((uint16_t)(1 << 6))     //Transmitter Holding Register(or tx FIFO) and Transmitter shift Register both empty
#define UART_FLAG_RX_FIFO_ERR                      ((uint16_t)(1 << 7))
#define UART_FLAG_RX_IDLE                          ((uint16_t)(1 << 8))     //Only to show difference cause the address of UART RX Ilde flag is isolate

void LogUART_SendData(const uint8_t *inBuf, uint16_t count);
void serial_puts(const char *s);
void HW_Uart_Init(void);
uint8_t BB3UART_GetFlagState(UART_InternalTypeDef *UARTx, uint32_t UART_FLAG);
void BB3UART_ReceiveData(UART_InternalTypeDef *UARTx, uint8_t *outBuf, uint16_t count);
void BB3UART_SendData(UART_InternalTypeDef *UARTx, const uint8_t *inBuf, uint16_t count);
void AutoTest_Uart_Init(void);
void HW_Uart_Init(void);

uint8_t AutoTest_UART_GetFlagState(AutoTest_UART_TypeDef *UARTx, uint32_t vUART_Flag);
void AutoTest_UART_ReceiveData(AutoTest_UART_TypeDef *UARTx, uint8_t *pRev_Buf, uint16_t vCount);
void AutoTest_UART_SendData(AutoTest_UART_TypeDef *UARTx, const uint8_t *pSend_Buf,
                            uint16_t vCount);

#ifdef __cplusplus
}
#endif

#endif /* _RTL8763B_UART_H_ */

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/



