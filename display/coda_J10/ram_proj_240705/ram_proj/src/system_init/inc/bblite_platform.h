#ifndef __RTL8762C_PLATFORM_H
#define __RTL8762C_PLATFORM_H
#include "REG_BASE.h"
#include "bblite_address_map.h"

/* ----------------Configuration of the cm4 Processor and Core Peripherals---------------- */
//#define __CM4_REV                      0x0001U      /* Core revision r0p1                                                      */
//#define __MPU_PRESENT                  1            /*!< MPU present or not                                                    */
//#define __FPU_PRESENT                  1            /* FPU present                                                             */
//#define __NVIC_PRIO_BITS               3            /*!< Number of Bits used for Priority Levels                               */
//#define __Vendor_SysTickConfig         0           /*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

#define FREQ_20MHZ 20000000
#define FREQ_40MHZ 40000000



//set cpu clock
#define  CPU_CLOCK  FREQ_40MHZ

//system timer
#define SYSTEM_TIMER_INDEX 7


//NVIC table
//typedef enum IRQn
//{
//    /* -------------------  Cortex-M4 Processor Exceptions Numbers  ------------------- */
//    NonMaskableInt_IRQn           = -14,      /*  2 Non Maskable Interrupt */
//    HardFault_IRQn                = -13,      /*  3 HardFault Interrupt */
//    MemoryManagement_IRQn         = -12,      /*  4 Memory Management Interrupt */
//    BusFault_IRQn                 = -11,      /*  5 Bus Fault Interrupt */
//    UsageFault_IRQn               = -10,      /*  6 Usage Fault Interrupt */
//    SVCall_IRQn                   =  -5,      /* 11 SV Call Interrupt */
//    DebugMonitor_IRQn             =  -4,      /* 12 Debug Monitor Interrupt */
//    PendSV_IRQn                   =  -2,      /* 14 Pend SV Interrupt */
//    SysTick_IRQn                  =  -1,      /* 15 System Tick Interrupt */

//    System_IRQn = 0,             /*  [0] System Interrupt */
//    WDG_IRQn,                    /*  [1] Watch Dog Interrupt */
//    BTMAC_IRQn,                  /*  [2] BTMAC Interrupt ( an Extension of interrupt ) */
//    Timer3_IRQn,                 /*  [3] Timer3 global interrupt */
//    Timer2_IRQn,                 /*  [4] Timer2 global interrupt */
//    Platform_IRQn,               /*  [5] Platform error interrupt */
//    I2S0_TX_IRQn,                /*  [6] I2S0 TX interrupt */
//    I2S0_RX_IRQn,                /*  [7] I2S0 RX interrupt */
//    Timer4_7_IRQn,               /*  [8] Timer 4 to 7 interrupt  ( an Extension of interrupt ) */
//    GPIO4_IRQn,                  /*  [9] GPIO 4 interrupt */
//    GPIO5_IRQn,                  /*  [10] GPIO 5 interrupt */
//    UART1_IRQn,                  /*  [11] UART1 interrupt */
//    UART0_IRQn,                  /*  [12] UART0 interrupt */
//    RTC_IRQn,                    /*  [13] Realtime counter interrupt */
//    SPI0_IRQn,                   /*  [14] SPI0 interrupt */
//    SPI1_IRQn,                   /*  [15] SPI1 interrupt */
//    I2C0_IRQn,                   /*  [16] I2C0 interrupt */
//    I2C1_IRQn,                   /*  [17] I2C1 interrupt */
//    ADC_IRQn,                    /*  [18] ADC global interrupt */
//    Peripheral_IRQn,             /*  [19] Peripheral Interrupt  ( an Extension of interrupt ) */
//    GDMA0_Channel0_IRQn,         /*  [20] RTK-DMA0 channel 0 global interrupt */
//    GDMA0_Channel1_IRQn,         /*  [21] RTK-DMA0 channel 1 global interrupt */
//    GDMA0_Channel2_IRQn,         /*  [22] RTK-DMA0 channel 2 global interrupt */
//    GDMA0_Channel3_IRQn,         /*  [23] RTK-DMA0 channel 3 global interrupt */
//    GDMA0_Channel4_IRQn,         /*  [24] RTK-DMA0 channel 4 global interrupt */
//    GDMA0_Channel5_IRQn,         /*  [25] RTK-DMA0 channel 5 global interrupt */
//    GPIO_Group3_IRQn,            /*  [26] GPIO Group3 Interrupt ( an Extension of interrupt ) */
//    GPIO_Group2_IRQn,            /*  [27] GPIO Group2 Interrupt ( an Extension of interrupt )  */
//    IR_IRQn,                     /*  [28] IR Interrupt */
//    GPIO_Group1_IRQn,            /*  [29] GPIO Group1 Interrupt ( an Extension of interrupt )  */
//    GPIO_Group0_IRQn,            /*  [30] GPIO Group0 Interrupt ( an Extension of interrupt )  */
//    UART2_IRQn,                  /*  [31] UART2 interrupt */
//    /******  Bee2 Extension Interrupt Numbers ************/
//    TIMER4_IRQ                      = 8,
//    TIMER5_IRQ                      = 8,
//    TIMER6_IRQ                      = 8,
//    TIMER7_IRQ                      = 8,
//    GPIO3_IRQn                      = 26,
//    GPIO7_IRQn                      = 26,
//    GPIO11_IRQn                     = 26,
//    GPIO15_IRQn                     = 26,
//    GPIO19_IRQn                     = 26,
//    GPIO23_IRQn                     = 26,
//    GPIO27_IRQn                     = 26,
//    GPIO31_IRQn                     = 26,
//    GPIO2_IRQn                      = 27,
//    GPIO6_IRQn                      = 27,
//    GPIO10_IRQn                     = 27,
//    GPIO14_IRQn                     = 27,
//    GPIO18_IRQn                     = 27,
//    GPIO22_IRQn                     = 27,
//    GPIO26_IRQn                     = 27,
//    GPIO30_IRQn                     = 27,
//    GPIO1_IRQn                      = 29,
//    GPIO9_IRQn                      = 29,
//    GPIO13_IRQn                     = 29,
//    GPIO17_IRQn                     = 29,
//    GPIO21_IRQn                     = 29,
//    GPIO25_IRQn                     = 29,
//    GPIO29_IRQn                     = 29,
//    GPIO0_IRQn                      = 30,
//    GPIO8_IRQn                      = 30,
//    GPIO12_IRQn                     = 30,
//    GPIO16_IRQn                     = 30,
//    GPIO20_IRQn                     = 30,
//    GPIO24_IRQn                     = 30,
//    GPIO28_IRQn                     = 30,
//    SPIFLASH_IRQ                    = 52,
//    qdecode_IRQn                    = 53,
//    KeyScan_IRQn                    = 54,
//    SPI2W_IRQn                      = 55,
//    LPCOMP_IRQn                     = 56,
//    PTA_Mailbox_IRQn                = 57,
//    SPORT1_TX_IRQn                  = 58,
//    SPORT1_RX_IRQn                  = 59,
//    LCD_IRQn                        = 60
//} IRQn_Type, *PIRQn_Type;


uint8_t AON_Read(uint8_t offset);
uint8_t AON_Read_Safe(uint8_t offset);
void AON_Write(uint8_t offset, uint8_t data);
void AON_Write_Safe(uint8_t offset, uint8_t data);

#endif
