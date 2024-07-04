/**
  *************************************************************************************************
  * @file    vector_table.c
  * @author
  * @version V0.0.1
  * @date    2020-07-01
  * @brief   this file includes ram vector table and corresponding APIs
  *************************************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  *************************************************************************************************/
#include "system_rtl876x.h"
#include "rtl876x.h"
#include "vector_table.h"
#include "trace.h"
#include "mem_cfg_int.h"

//#define DIRECT_LOG(...)

void Peri_Handler(void);
void GPIO_A_2_7_Handler(void);
void GPIO_A_8_15_Handler(void);
void GPIO_A_16_23_Handler(void);
void GPIO_A_24_31_Handler(void);
void GPIO_B_0_7_Handler(void);
void GPIO_B_8_15_Handler(void);
void GPIO_B_16_23_Handler(void);
void GPIO_B_24_31_Handler(void);
void RTZIP_Handler(void);
void CAN_Handler(void);
void PPE_Handler(void);
void DISPLAY_Handler(void);

void *rom_vector_table[] __attribute__((section("VECTOR"))) =
{
    (void *)(MAIN_STACK_START_ADDR + MAIN_STACK_SIZE),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManageFault_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    SVC_Handler,
    Debug_Monitor_Handler,
    Default_Handler,
    PendSV_Handler,
    SysTick_Handler,
    SYSTEM_ISR_ISR,
    WDT_ISR,
    BTMAC_ISR,
    DSP_ISR,
    RXI300_ISR_ISR,
    SPI0_ISR,
    I2C0_ISR,
    ADC_ISR,
    SPORT0_RX_ISR,
    SPORT0_TX_ISR,
    TIMER1_CH0_ISR,
    TIMER1_CH1_ISR,
    TIMER1_CH2_ISR,
    RTC_ISR,
    UART0_ISR,
    UART1_ISR,
    UART2_ISR,
    Peripheral_ISR_ISR,
    GPIO_A0_ISR,
    GPIO_A1_ISR,
    GPIO_A2_7_ISR,
    GPIO_A8_15_ISR,
    GPIO_A16_23_ISR,
    GPIO_A24_31_ISR,
    SPORT1_RX_ISR,
    SPORT1_TX_ISR,
    ADP_DET_ISR,
    VBAT_DET_ISR,
    GDMA0_Channel0_ISR,
    GDMA0_Channel1_ISR,
    GDMA0_Channel2_ISR,
    GDMA0_Channel3_ISR,
    GDMA0_Channel4_ISR,
    GDMA0_Channel5_ISR,
    GDMA0_Channel6_ISR,
    GDMA0_Channel7_ISR,
    GDMA0_Channel8_ISR,
    GPIO_B0_7_ISR,
    GPIO_B8_15_ISR,
    GPIO_B16_23_ISR,
    GPIO_B24_31_ISR,
    SPI1_ISR,
    SPI2_ISR,
    I2C1_ISR,
    I2C2_ISR,
    KEYSCAN_ISR,
    QDECODE_ISR,
    UART3_ISR,
    UART4_ISR,
    UART5_ISR,
    SPIC0_ISR,
    SPIC1_ISR,
    SPIC2_ISR,
    TIMER1_CH3_ISR,
    TIMER1_CH4_ISR,
    TIMER1_CH5_ISR,
    ASRC0_ISR,
    ASRC1_ISR,
    ETHERNET_ISR,
    ISO7816_ISR,
    SDIO0_ISR,
    SPORT2_RX_ISR,
    Zigbee_ISR,
    MJPEG_ISR,
    GDMA0_Channel9_ISR,
    GDMA0_Channel10_ISR,
    GDMA0_Channel11_ISR,
    Display_ISR,
    PPE_ISR,
    RTZIP_ISR,
    SLAVE_PORT_MONITOR_ISR,
    TIMER1_CH6_ISR,
    TIMER1_CH7_ISR,
    TIMER1_CH8_ISR,
    TIMER1_CH9_ISR,
    CAN0_ISR,
    BTMAC_WRAP_AROUND_ISR,
    PUBLIC_KEY_ENGINE_ISR,
    MFB_DET_L_ISR,
    spi_phy1_intr_ISR,
    CAN1_ISR,
    USB_ISR,
    ENDP_MULTI_PROC_INT_ISR,
    SUSPEND_N_INT_ISR,
    USB_IN_EP_INT_0_ISR,
    USB_IN_EP_INT_1_ISR,
    USB_IN_EP_INT_2_ISR,
    USB_IN_EP_INT_3_ISR,
    USB_IN_EP_INT_4_ISR,
    USB_IN_EP_INT_5_ISR,
    USB_IN_EP_INT_6_ISR,
    USB_OUT_EP_INT_0_ISR,
    USB_OUT_EP_INT_1_ISR,
    USB_OUT_EP_INT_2_ISR,
    USB_OUT_EP_INT_3_ISR,
    USB_OUT_EP_INT_4_ISR,
    USB_OUT_EP_INT_5_ISR,
    USB_OUT_EP_INT_6_ISR,
    USB_SOF_INT_ISR,
    IR_ISR,
    TRNG_ISR,
    LPC_ISR,
    PF_RTC_ISR,
    SPI_SLAVE_ISR,
};

void second_level_isr(uint32_t real_interrupt_status, int32_t vector_index)
{
    IRQ_Fun *Vectors = (IRQ_Fun *)SCB->VTOR;

    while (real_interrupt_status)
    {
        uint8_t idx = 31 - __builtin_clz(real_interrupt_status);
        Vectors[vector_index + idx]();
        real_interrupt_status &= ~BIT(idx);
    }
}

bool RamVectorTableInit(uint32_t ram_vector_addr)
{
//    IRQ_Fun *pRomVectorTable = (IRQ_Fun*)0;
//    IRQ_Fun *pRamVectorTable = (IRQ_Fun*)ram_vector_addr;
    bool ret = false;

    if ((ram_vector_addr & 0xFF) != 0)
    {
//        DIRECT_LOG("Illegal VTOR: %x", 1, ram_vector_addr);
        ret = false;
    }
    else
    {
        SCB->VTOR = ram_vector_addr;

        ret = true;
    }

    return ret;
}

bool RamVectorTableUpdate(uint32_t v_num, IRQ_Fun isr_handler)
{
    IRQ_Fun *pRamVectorTable = (IRQ_Fun *)(SCB->VTOR);
    bool ret = false;

    if (pRamVectorTable == 0)   /* Vector in ROM */
    {
//        DIRECT_LOG("Can not update vector table in ROM!", 0);
        ret = false;
    }
    else
    {
        pRamVectorTable[v_num] = isr_handler;
        __DMB();
        ret = true;
    }

    return ret;
}

