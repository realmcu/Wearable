#ifndef VECTOR_TABLE_AUTO_GEN_H
#define VECTOR_TABLE_AUTO_GEN_H

// *INDENT-OFF*
#include "stdint.h"
#include "stdbool.h"
#include "cmsis_compiler.h"

typedef void (*IRQ_Fun)(void);       // < ISR Handler Prototype

/* ================================================================================ */
/* ================                  IRQ_NUMBER                    ================ */
/* ======== Reference: BB2Ultra_AddrMap_IRQs_DmaPorts_20240627_v0.xlsx   =========== */
/* ================================================================================ */

typedef enum IRQn
{
    NMI_IRQn =                         -14,
    HardFault_IRQn =                   -13,
    MemManageFault_IRQn =              -12,
    BusFault_IRQn =                    -11,
    UsageFault_IRQn =                  -10,
    RSVD0_IRQn =                       -9,
    RSVD1_IRQn =                       -8,
    RSVD2_IRQn =                       -7,
    RSVD3_IRQn =                       -6,
    SVC_IRQn =                         -5,
    Debug_Monitor_IRQn =               -4,
    RSVD4_IRQn =                       -3,
    PendSV_IRQn =                      -2,
    SysTick_IRQn =                     -1,
    SYSTEM_ISR_IRQn =                  0,
    WDT_IRQn =                         1,
    BTMAC_IRQn =                       2,
    DSP_IRQn =                         3,
    RXI300_ISR_IRQn =                  4,
    SPI0_IRQn =                        5,
    I2C0_IRQn =                        6,
    ADC_IRQn =                         7,
    SPORT0_RX_IRQn =                   8,
    SPORT0_TX_IRQn =                   9,
    TIMER1_CH0_IRQn =                  10,
    TIMER1_CH1_IRQn =                  11,
    TIMER1_CH2_IRQn =                  12,
    RTC_IRQn =                         13,
    UART0_IRQn =                       14,
    UART1_IRQn =                       15,
    UART2_IRQn =                       16,
    Peripheral_ISR_IRQn =              17,
    GPIO_A0_IRQn =                     18,
    GPIO_A1_IRQn =                     19,
    GPIO_A2_7_IRQn =                   20,
    GPIO_A8_15_IRQn =                  21,
    GPIO_A16_23_IRQn =                 22,
    GPIO_A24_31_IRQn =                 23,
    SPORT1_RX_IRQn =                   24,
    SPORT1_TX_IRQn =                   25,
    ADP_DET_IRQn =                     26,
    VBAT_DET_IRQn =                    27,
    GDMA0_Channel0_IRQn =              28,
    GDMA0_Channel1_IRQn =              29,
    GDMA0_Channel2_IRQn =              30,
    GDMA0_Channel3_IRQn =              31,
    GDMA0_Channel4_IRQn =              32,
    GDMA0_Channel5_IRQn =              33,
    GDMA0_Channel6_IRQn =              34,
    GDMA0_Channel7_IRQn =              35,
    GDMA0_Channel8_IRQn =              36,
    GPIO_B0_7_IRQn =                   37,
    GPIO_B8_15_IRQn =                  38,
    GPIO_B16_23_IRQn =                 39,
    GPIO_B24_31_IRQn =                 40,
    SPI1_IRQn =                        41,
    SPI2_IRQn =                        42,
    I2C1_IRQn =                        43,
    I2C2_IRQn =                        44,
    KEYSCAN_IRQn =                     45,
    QDECODE_IRQn =                     46,
    UART3_IRQn =                       47,
    UART4_IRQn =                       48,
    UART5_IRQn =                       49,
    SPIC0_IRQn =                       50,
    SPIC1_IRQn =                       51,
    SPIC2_IRQn =                       52,
    TIMER1_CH3_IRQn =                  53,
    TIMER1_CH4_IRQn =                  54,
    TIMER1_CH5_IRQn =                  55,
    ASRC0_IRQn =                       56,
    ASRC1_IRQn =                       57,
    ETHERNET_IRQn =                    58,
    ISO7816_IRQn =                     59,
    SDIO0_IRQn =                       60,
    SPORT2_RX_IRQn =                   61,
    Zigbee_IRQn =                      62,
    MJPEG_IRQn =                       63,
    GDMA0_Channel9_IRQn =              64,
    GDMA0_Channel10_IRQn =             65,
    GDMA0_Channel11_IRQn =             66,
    Display_IRQn =                     67,
    PPE_IRQn =                         68,
    RTZIP_IRQn =                       69,
    SLAVE_PORT_MONITOR_IRQn =          70,
    TIMER1_CH6_IRQn =                  71,
    TIMER1_CH7_IRQn =                  72,
    TIMER1_CH8_IRQn =                  73,
    TIMER1_CH9_IRQn =                  74,
    CAN0_IRQn =                        75,
    BTMAC_WRAP_AROUND_IRQn =           76,
    PUBLIC_KEY_ENGINE_IRQn =           77,
    MFB_DET_L_IRQn =                   78,
    spi_phy1_intr_IRQn =               79,
    CAN1_IRQn =                        80,
    USB_IRQn =                         81,
    ENDP_MULTI_PROC_INT_IRQn =         82,
    SUSPEND_N_INT_IRQn =               83,
    USB_IN_EP_INT_0_IRQn =             84,
    USB_IN_EP_INT_1_IRQn =             85,
    USB_IN_EP_INT_2_IRQn =             86,
    USB_IN_EP_INT_3_IRQn =             87,
    USB_IN_EP_INT_4_IRQn =             88,
    USB_IN_EP_INT_5_IRQn =             89,
    USB_IN_EP_INT_6_IRQn =             90,
    USB_OUT_EP_INT_0_IRQn =            91,
    USB_OUT_EP_INT_1_IRQn =            92,
    USB_OUT_EP_INT_2_IRQn =            93,
    USB_OUT_EP_INT_3_IRQn =            94,
    USB_OUT_EP_INT_4_IRQn =            95,
    USB_OUT_EP_INT_5_IRQn =            96,
    USB_OUT_EP_INT_6_IRQn =            97,
    USB_SOF_INT_IRQn =                 98,
    IR_IRQn =                          99,
    TRNG_IRQn =                        100,
    LPC_IRQn =                         101,
    PF_RTC_IRQn =                      102,
    SPI_SLAVE_IRQn =                   103,
    MAX_IRQn,
} IRQn_Type;

#define IRQn_TO_VECTORn(irqn)   ((irqn) + 16)

extern void *RamVectorTable[];

void VTORInit(void);

/* ================================================================================ */
/* ================                    HANDLER                     ================ */
/* ======== Reference: BB2Ultra_AddrMap_IRQs_DmaPorts_20240627_v0.xlsx   =========== */
/* ================================================================================ */

__WEAK void Default_Handler(void);
__WEAK void Default_ISR(void);
__WEAK void Reset_Handler(void);
__WEAK void NMI_Handler(void);
__WEAK void HardFault_Handler(void);
__WEAK void MemManageFault_Handler(void);
__WEAK void BusFault_Handler(void);
__WEAK void UsageFault_Handler(void);
__WEAK void SVC_Handler(void);
__WEAK void Debug_Monitor_Handler(void);
__WEAK void PendSV_Handler(void);
__WEAK void SysTick_Handler(void);
__WEAK void SYSTEM_ISR_ISR(void);
__WEAK void WDT_ISR(void);
__WEAK void BTMAC_ISR(void);
__WEAK void DSP_ISR(void);
__WEAK void RXI300_ISR_ISR(void);
__WEAK void SPI0_ISR(void);
__WEAK void I2C0_ISR(void);
__WEAK void ADC_ISR(void);
__WEAK void SPORT0_RX_ISR(void);
__WEAK void SPORT0_TX_ISR(void);
__WEAK void TIMER1_CH0_ISR(void);
__WEAK void TIMER1_CH1_ISR(void);
__WEAK void TIMER1_CH2_ISR(void);
__WEAK void RTC_ISR(void);
__WEAK void UART0_ISR(void);
__WEAK void UART1_ISR(void);
__WEAK void UART2_ISR(void);
__WEAK void Peripheral_ISR_ISR(void);
__WEAK void GPIO_A0_ISR(void);
__WEAK void GPIO_A1_ISR(void);
__WEAK void GPIO_A2_7_ISR(void);
__WEAK void GPIO_A8_15_ISR(void);
__WEAK void GPIO_A16_23_ISR(void);
__WEAK void GPIO_A24_31_ISR(void);
__WEAK void SPORT1_RX_ISR(void);
__WEAK void SPORT1_TX_ISR(void);
__WEAK void ADP_DET_ISR(void);
__WEAK void VBAT_DET_ISR(void);
__WEAK void GDMA0_Channel0_ISR(void);
__WEAK void GDMA0_Channel1_ISR(void);
__WEAK void GDMA0_Channel2_ISR(void);
__WEAK void GDMA0_Channel3_ISR(void);
__WEAK void GDMA0_Channel4_ISR(void);
__WEAK void GDMA0_Channel5_ISR(void);
__WEAK void GDMA0_Channel6_ISR(void);
__WEAK void GDMA0_Channel7_ISR(void);
__WEAK void GDMA0_Channel8_ISR(void);
__WEAK void GPIO_B0_7_ISR(void);
__WEAK void GPIO_B8_15_ISR(void);
__WEAK void GPIO_B16_23_ISR(void);
__WEAK void GPIO_B24_31_ISR(void);
__WEAK void SPI1_ISR(void);
__WEAK void SPI2_ISR(void);
__WEAK void I2C1_ISR(void);
__WEAK void I2C2_ISR(void);
__WEAK void KEYSCAN_ISR(void);
__WEAK void QDECODE_ISR(void);
__WEAK void UART3_ISR(void);
__WEAK void UART4_ISR(void);
__WEAK void UART5_ISR(void);
__WEAK void SPIC0_ISR(void);
__WEAK void SPIC1_ISR(void);
__WEAK void SPIC2_ISR(void);
__WEAK void TIMER1_CH3_ISR(void);
__WEAK void TIMER1_CH4_ISR(void);
__WEAK void TIMER1_CH5_ISR(void);
__WEAK void ASRC0_ISR(void);
__WEAK void ASRC1_ISR(void);
__WEAK void ETHERNET_ISR(void);
__WEAK void ISO7816_ISR(void);
__WEAK void SDIO0_ISR(void);
__WEAK void SPORT2_RX_ISR(void);
__WEAK void Zigbee_ISR(void);
__WEAK void MJPEG_ISR(void);
__WEAK void GDMA0_Channel9_ISR(void);
__WEAK void GDMA0_Channel10_ISR(void);
__WEAK void GDMA0_Channel11_ISR(void);
__WEAK void Display_ISR(void);
__WEAK void PPE_ISR(void);
__WEAK void RTZIP_ISR(void);
__WEAK void SLAVE_PORT_MONITOR_ISR(void);
__WEAK void TIMER1_CH6_ISR(void);
__WEAK void TIMER1_CH7_ISR(void);
__WEAK void TIMER1_CH8_ISR(void);
__WEAK void TIMER1_CH9_ISR(void);
__WEAK void CAN0_ISR(void);
__WEAK void BTMAC_WRAP_AROUND_ISR(void);
__WEAK void PUBLIC_KEY_ENGINE_ISR(void);
__WEAK void MFB_DET_L_ISR(void);
__WEAK void spi_phy1_intr_ISR(void);
__WEAK void CAN1_ISR(void);
__WEAK void USB_ISR(void);
__WEAK void ENDP_MULTI_PROC_INT_ISR(void);
__WEAK void SUSPEND_N_INT_ISR(void);
__WEAK void USB_IN_EP_INT_0_ISR(void);
__WEAK void USB_IN_EP_INT_1_ISR(void);
__WEAK void USB_IN_EP_INT_2_ISR(void);
__WEAK void USB_IN_EP_INT_3_ISR(void);
__WEAK void USB_IN_EP_INT_4_ISR(void);
__WEAK void USB_IN_EP_INT_5_ISR(void);
__WEAK void USB_IN_EP_INT_6_ISR(void);
__WEAK void USB_OUT_EP_INT_0_ISR(void);
__WEAK void USB_OUT_EP_INT_1_ISR(void);
__WEAK void USB_OUT_EP_INT_2_ISR(void);
__WEAK void USB_OUT_EP_INT_3_ISR(void);
__WEAK void USB_OUT_EP_INT_4_ISR(void);
__WEAK void USB_OUT_EP_INT_5_ISR(void);
__WEAK void USB_OUT_EP_INT_6_ISR(void);
__WEAK void USB_SOF_INT_ISR(void);
__WEAK void IR_ISR(void);
__WEAK void TRNG_ISR(void);
__WEAK void LPC_ISR(void);
__WEAK void PF_RTC_ISR(void);
__WEAK void SPI_SLAVE_ISR(void);

// *INDENT-ON* 
#endif //#define VECTOR_TABLE_AUTO_GEN_H
