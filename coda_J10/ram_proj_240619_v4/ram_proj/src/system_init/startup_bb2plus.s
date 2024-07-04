;/*****************************************************************************/
;/*     startup_rtl8763.s: Startup file for Realtek RTL8763 device series     */
;/*****************************************************************************/
;include "mem_config.h"

;__initial_sp    EQU     0x10018000

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x0000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset

                AREA    VECTOR, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     System_Handler            ;[0]  System On interrupt
                DCD     WDG_Handler               ;[1]  Watch dog global insterrupt
                DCD     BTMAC_Handler             ;[2]  See Below Table ( an Extension of interrupt )
                DCD     M_Peripheral_ISR_Handler  ;[3]  Master Peripheral IP interrupt (usb or sdio)
                DCD     TIM2_Handler              ;[4]  Timer2 global interrupt
                DCD     HardFault_Handler         ;[5]  Platform interrupt (platfrom error interrupt)
                DCD     I2S0_Handler              ;[6]  I2S0 interrupt
                DCD     I2S1_Handler              ;[7]  I2S1 interrupt
                DCD     UART2_Handler             ;[8]  Data_Uart1 interrupt (used for DSP)
                DCD     GPIO0_Handler             ;[9]  GPIO0 interrupt
                DCD     GPIO1_Handler             ;[10] GPIO1 interrupt
                DCD     UART1_Handler             ;[11] Log_Uart interrupt (Log0)
                DCD     UART0_Handler             ;[12] Data_Uart interrupt
                DCD     RTC_Handler               ;[13] Realtime counter interrupt
                DCD     SPI0_Handler              ;[14] SPI0 interrupt
                DCD     USB_ISOC_Handler          ;[15] USB ISOC interrupt
                DCD     I2C0_Handler              ;[16] I2C0 interrupt
                DCD     0                         ;[17] I2C1 interrupt (NOT used in BBLite)
                DCD     ADC_Handler               ;[18] ADC global interrupt
                DCD     Peripheral_Handler        ;[19] See Below Table ( an Extension of interrupt )
                DCD     GDMA0_Channel0_Handler    ;[20] RTK-DMA0 channel 0 global interrupt
                DCD     GDMA0_Channel1_Handler    ;[21] RTK-DMA0 channel 1 global interrupt
                DCD     GDMA0_Channel2_Handler    ;[22] RTK-DMA0 channel 2 global interrupt
                DCD     GDMA0_Channel3_Handler    ;[23] RTK-DMA0 channel 3 global interrupt
                DCD     GDMA0_Channel4_Handler    ;[24] RTK-DMA0 channel 4 global interrupt
                DCD     GDMA0_Channel5_Handler    ;[25] RTK-DMA0 channel 5 global interrupt
                DCD     0                         ;[26] keyscan global interrupt (NOT used in BBLite)
                DCD     0                         ;[27] qdecode global interrupt (NOT used in BBLite)
                DCD     0                         ;[28] IR module global interrupt (NOT used in BBLite)
                DCD     DSP_Handler               ;[29] DSP interrupt
                DCD     GDMA0_Channel6_Handler    ;[30] RTK-DMA0 channel 6 global interrupt
                DCD     GDMA0_Channel7_Handler    ;[31] RTK-DMA0 channel 7 global interrupt
                ;Peripheral Interrupts not special interrupt
                ;Interrupt name, Interrupt status bit, Offset in vector
                DCD     SPI_Flash_Handler         ;19, 0, 48
                DCD     GPIO2_Handler             ;19, 1, 49
                DCD     GPIO3_Handler             ;19, 2, 50
                DCD     GPIO4_Handler             ;19, 3, 51
                DCD     GPIO5_Handler             ;19, 4, 52
                DCD     TIM3_Handler              ;19, 5, 53
                DCD     TIM4_Handler              ;19, 6, 54
                DCD     TIM5_Handler              ;19, 7, 55
                DCD     TIM6_Handler              ;19, 8, 56
                DCD     TIM7_Handler              ;19, 9, 57
                DCD     0                         ;19, 10, 58 (NOT used in BBLite)
                DCD     LPCOMP_Handler            ;19, 11, 59
                DCD     VBAT_DET_Handler          ;19, 12, 60
                DCD     AES_Handler               ;19, 13, 61
                DCD     ASRC1_Handler             ;19, 14, 62
                DCD     ASRC2_Handler             ;19, 15, 63
                ;GPIO 6 to 31
                ;Interrupt name, IRQ, Interrupt status bit, GPIO status bit, Offset in vector
                DCD     GPIO6_Handler             ;19, 16, 6, 64
                DCD     GPIO7_Handler             ;19, 16, 7, 65
                DCD     GPIO8_Handler             ;19, 16, 8, 66
                DCD     GPIO9_Handler             ;19, 16, 9, 67
                DCD     GPIO10_Handler            ;19, 16, 10, 68
                DCD     GPIO11_Handler            ;19, 16, 11, 69
                DCD     GPIO12_Handler            ;19, 16, 12, 70
                DCD     GPIO13_Handler            ;19, 16, 13, 71
                DCD     GPIO14_Handler            ;19, 16, 14, 72
                DCD     GPIO15_Handler            ;19, 16, 15, 73
                DCD     GPIO16_Handler            ;19, 16, 16, 74
                DCD     GPIO17_Handler            ;19, 16, 17, 75
                DCD     GPIO18_Handler            ;19, 16, 18, 76
                DCD     GPIO19_Handler            ;19, 16, 19, 77
                DCD     GPIO20_Handler            ;19, 16, 20, 78
                DCD     GPIO21_Handler            ;19, 16, 21, 79
                DCD     GPIO22_Handler            ;19, 16, 22, 80
                DCD     GPIO23_Handler            ;19, 16, 23, 81
                DCD     GPIO24_Handler            ;19, 16, 24, 82
                DCD     GPIO25_Handler            ;19, 16, 25, 83
                DCD     GPIO26_Handler            ;19, 16, 26, 84
                DCD     GPIO27_Handler            ;19, 16, 27, 85
                DCD     GPIO28_Handler            ;19, 16, 28, 86
                DCD     GPIO29_Handler            ;19, 16, 28, 87
                DCD     GPIO30_Handler            ;19, 16, 29, 88
                DCD     GPIO31_Handler            ;19, 16, 30, 89
                ;Interrupt name, IRQ, Interrupt status bit, Offset in vector
                DCD     PTA_Mailbox_Handler       ;19, 17, 90
                DCD     DSP_WDG_Handler           ;19, 18, 91
                DCD     FlashPower_DET_Handler    ;19, 19, 92
                DCD     SPORT0_TX_Handler         ;19, 25, 93
                DCD     SPORT0_RX_Handler         ;19, 26, 94
                DCD     SPORT1_TX_Handler         ;19, 27, 95
                DCD     SPORT1_RX_Handler         ;19, 28, 96
                DCD     SPDIF_Handler             ;19, 29, 97
                DCD     ADP_IN_DET_Handler        ;19, 13, 98
                DCD     ADP_OUT_DET_Handler       ;19, 12, 99
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors
                    
                AREA    RESET, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
                EXPORT Reset_Handler               [WEAK]
                IMPORT SystemInit
                IMPORT __main

                LDR    R0, =__main
                BX     R0

                ENDP ; end of Reset_Handler


                AREA    |.text|, CODE, READONLY
Default_Handler PROC
                EXPORT NMI_Handler                      [WEAK]
                EXPORT HardFault_Handler                [WEAK]
                EXPORT MemManage_Handler                [WEAK]
                EXPORT BusFault_Handler                 [WEAK]
                EXPORT UsageFault_Handler               [WEAK]
                EXPORT SVC_Handler                      [WEAK]
                EXPORT DebugMon_Handler                 [WEAK]
                EXPORT PendSV_Handler                   [WEAK]
                EXPORT SysTick_Handler                  [WEAK]
                EXPORT System_Handler                   [WEAK]
                EXPORT WDG_Handler                      [WEAK]
                EXPORT BTMAC_Handler                    [WEAK]
                EXPORT M_Peripheral_ISR_Handler         [WEAK]
                EXPORT TIM2_Handler                     [WEAK]
                EXPORT I2S0_Handler                     [WEAK]
                EXPORT I2S1_Handler                     [WEAK]
                EXPORT UART2_Handler                    [WEAK]
                EXPORT GPIO0_Handler                    [WEAK]
                EXPORT GPIO1_Handler                    [WEAK]
                EXPORT UART1_Handler                    [WEAK]
                EXPORT UART0_Handler                    [WEAK]
                EXPORT RTC_Handler                      [WEAK]
                EXPORT SPI0_Handler                     [WEAK]
                ;EXPORT SPI1_Handler                     [WEAK]
                EXPORT I2C0_Handler                     [WEAK]
                ;EXPORT I2C1_Handler                     [WEAK]
                EXPORT ADC_Handler                      [WEAK]
                EXPORT Peripheral_Handler               [WEAK]
                EXPORT GDMA0_Channel0_Handler           [WEAK]
                EXPORT GDMA0_Channel1_Handler           [WEAK]
                EXPORT GDMA0_Channel2_Handler           [WEAK]
                EXPORT GDMA0_Channel3_Handler           [WEAK]
                EXPORT GDMA0_Channel4_Handler           [WEAK]
                EXPORT GDMA0_Channel5_Handler           [WEAK]
                EXPORT GDMA0_Channel6_Handler           [WEAK]
                EXPORT GDMA0_Channel7_Handler           [WEAK]
                ;EXPORT KeyScan_Handler                  [WEAK]
                ;EXPORT QDEC_Handler                     [WEAK]
                ;EXPORT IR_Handler                       [WEAK]
                EXPORT DSP_Handler                      [WEAK]
                EXPORT MemManage_Handler                [WEAK]
                EXPORT BusFault_Handler                 [WEAK]
                EXPORT UsageFault_Handler               [WEAK]
                ;Peripheral Interrupts not special interrupt
                EXPORT SPI_Flash_Handler                [WEAK]
                EXPORT GPIO2_Handler                    [WEAK]
                EXPORT GPIO3_Handler                    [WEAK]
                EXPORT GPIO4_Handler                    [WEAK]
                EXPORT GPIO5_Handler                    [WEAK]
                EXPORT TIM3_Handler                     [WEAK]
                EXPORT TIM4_Handler                     [WEAK]
                EXPORT TIM5_Handler                     [WEAK]
                EXPORT TIM6_Handler                     [WEAK]
                EXPORT TIM7_Handler                     [WEAK]
                ;EXPORT SPI2W_Handler                    [WEAK]
                EXPORT LPCOMP_Handler                   [WEAK]
                EXPORT VBAT_DET_Handler                 [WEAK]
                EXPORT AES_Handler                      [WEAK]
                EXPORT ASRC1_Handler                    [WEAK]
                EXPORT ASRC2_Handler                    [WEAK]
                EXPORT GPIO6_Handler                    [WEAK]
                EXPORT GPIO7_Handler                    [WEAK]
                EXPORT GPIO8_Handler                    [WEAK]
                EXPORT GPIO9_Handler                    [WEAK]
                EXPORT GPIO10_Handler                   [WEAK]
                EXPORT GPIO11_Handler                   [WEAK]
                EXPORT GPIO12_Handler                   [WEAK]
                EXPORT GPIO13_Handler                   [WEAK]
                EXPORT GPIO14_Handler                   [WEAK]
                EXPORT GPIO15_Handler                   [WEAK]
                EXPORT GPIO16_Handler                   [WEAK]
                EXPORT GPIO17_Handler                   [WEAK]
                EXPORT GPIO18_Handler                   [WEAK]
                EXPORT GPIO19_Handler                   [WEAK]
                EXPORT GPIO20_Handler                   [WEAK]
                EXPORT GPIO21_Handler                   [WEAK]
                EXPORT GPIO22_Handler                   [WEAK]
                EXPORT GPIO23_Handler                   [WEAK]
                EXPORT GPIO24_Handler                   [WEAK]
                EXPORT GPIO25_Handler                   [WEAK]
                EXPORT GPIO26_Handler                   [WEAK]
                EXPORT GPIO27_Handler                   [WEAK]
                EXPORT GPIO28_Handler                   [WEAK]
                EXPORT GPIO29_Handler                   [WEAK]
                EXPORT GPIO30_Handler                   [WEAK]
                EXPORT GPIO31_Handler                   [WEAK]
                EXPORT PTA_Mailbox_Handler              [WEAK]
                EXPORT DSP_WDG_Handler                  [WEAK]
                EXPORT FlashPower_DET_Handler           [WEAK]
                EXPORT SPORT0_TX_Handler                [WEAK]
                EXPORT SPORT0_RX_Handler                [WEAK]
                EXPORT SPORT1_TX_Handler                [WEAK]
                EXPORT SPORT1_RX_Handler                [WEAK]
                EXPORT SPDIF_Handler                    [WEAK]
                EXPORT ADP_IN_DET_Handler               [WEAK]
                EXPORT ADP_OUT_DET_Handler              [WEAK]
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
System_Handler
WDG_Handler
BTMAC_Handler
M_Peripheral_ISR_Handler
TIM2_Handler
I2S0_Handler
I2S1_Handler
UART2_Handler
GPIO0_Handler
GPIO1_Handler
UART1_Handler
UART0_Handler
RTC_Handler
SPI0_Handler
USB_ISOC_Handler
;SPI1_Handler
I2C0_Handler
;I2C1_Handler
ADC_Handler
Peripheral_Handler
GDMA0_Channel0_Handler
GDMA0_Channel1_Handler
GDMA0_Channel2_Handler
GDMA0_Channel3_Handler
GDMA0_Channel4_Handler
GDMA0_Channel5_Handler
GDMA0_Channel6_Handler
GDMA0_Channel7_Handler
;KeyScan_Handler
;QDEC_Handler
;IR_Handler
DSP_Handler
;Peripheral Interrupts not special interrupt
SPI_Flash_Handler
GPIO2_Handler
GPIO3_Handler
GPIO4_Handler
GPIO5_Handler
TIM3_Handler
TIM4_Handler
TIM5_Handler
TIM6_Handler
TIM7_Handler
;SPI2W_Handler
LPCOMP_Handler
VBAT_DET_Handler
AES_Handler
ASRC1_Handler
ASRC2_Handler
GPIO6_Handler
GPIO7_Handler
GPIO8_Handler
GPIO9_Handler
GPIO10_Handler
GPIO11_Handler
GPIO12_Handler
GPIO13_Handler
GPIO14_Handler
GPIO15_Handler
GPIO16_Handler
GPIO17_Handler
GPIO18_Handler
GPIO19_Handler
GPIO20_Handler
GPIO21_Handler
GPIO22_Handler
GPIO23_Handler
GPIO24_Handler
GPIO25_Handler
GPIO26_Handler
GPIO27_Handler
GPIO28_Handler
GPIO29_Handler
GPIO30_Handler
GPIO31_Handler
PTA_Mailbox_Handler
DSP_WDG_Handler
FlashPower_DET_Handler
SPORT0_TX_Handler
SPORT0_RX_Handler
SPORT1_TX_Handler
SPORT1_RX_Handler
SPDIF_Handler
ADP_IN_DET_Handler
ADP_OUT_DET_Handler
                B      .

                ENDP

                ALIGN

; User Initial Stack
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap
                LDR     R0, = Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem + Heap_Size)
                LDR     R3, = Stack_Mem
                NOP
                BX      LR

                END
