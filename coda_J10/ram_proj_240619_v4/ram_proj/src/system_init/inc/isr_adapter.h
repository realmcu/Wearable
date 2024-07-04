/*
 * This file is used to rename IRQ number and ISR name.
 * This remapping is not a must, it is used to reduce the modifies to AutoTest
 * code.
 * Please find the below URL for the IRQ and ISR naming rule:
 * https://wiki.realtek.com/display/Bee1/CM4+Interrupt+Priority+Config+Reference
 */

#ifndef _ISR_ADAPTER_H_
#define _ISR_ADAPTER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Compatible IRQ Number defninitions to Bee1(RTL8762A) */
//#define SYSTEM_ON_IRQ      System_IRQn
//#define WDG_IRQ            WDG_IRQn

//#define TIM2_IRQ          Timer2_IRQn
//#define TIM3_IRQ          Timer3_IRQn
//#define TIM4_7_IRQ        Timer4_7_IRQn
//#define TIM4_IRQ          TIMER4_IRQ
//#define TIM5_IRQ          TIMER5_IRQ
//#define TIM6_IRQ          TIMER6_IRQ
//#define TIM7_IRQ          TIMER7_IRQ
//
//#define GPIO0_IRQ          GPIO0_IRQn
//#define GPIO1_IRQ          GPIO1_IRQn
//#define Log_Uart_IRQn      UART1_IRQn
//#define Data_Uart_IRQn     UART0_IRQn
//#define Data_Uart1_IRQn    UART2_IRQn
//#define RTC_IRQ            RTC_IRQn
//#define SPI0_IRQ           SPI0_IRQn
//#define SPI1_IRQ           SPI1_IRQn
//#define I2C0_IRQ           I2C0_IRQn
//#define I2C1_IRQ           I2C1_IRQn
//#define ADC_IRQ            ADC_IRQn
//#define PERIPHERAL_IRQ     Peripheral_IRQn
//#define GDMA0_CHANNEL0_IRQ GDMA0_Channel0_IRQn
//#define GDMA0_CHANNEL1_IRQ GDMA0_Channel1_IRQn
//#define GDMA0_CHANNEL2_IRQ GDMA0_Channel2_IRQn
//#define GDMA0_CHANNEL3_IRQ GDMA0_Channel3_IRQn
//#define GDMA0_CHANNEL4_IRQ GDMA0_Channel4_IRQn
//#define KEYSCAN_IRQ        KeyScan_IRQn
//#define QDECODE_IRQ        Qdecode_IRQn
//#define IR_IRQ             IR_IRQn
//#define EFLASH_IRQ
//#define SPI2WIRE_IRQ       SPI2W_IRQn
//#define ANALOG_IRQ
//#define SPIFLASH_IRQ       SPIC_IRQn
//#define GPIO2_IRQ          GPIO2_IRQn
//#define GPIO3_IRQ          GPIO3_IRQn
//#define GPIO4_IRQ          GPIO4_IRQn
//#define GPIO5_IRQ          GPIO5_IRQn

//#define GPIO6To31IRQ       GPIO6To31_IRQn

/* Compatible IRQ Handler definitions to Bee1(RTL8762A) */
//#define M_Peripheral_ISR_Handler M_Peripheral_Handler

//#define TIM2_Handler            Timer2_Handler
//#define TIM3_Handler            Timer3_Handler
//#define TIM4_Handler            Timer4_Handler
//#define TIM5_Handler            Timer5_Handler
//#define TIM6_Handler            Timer6_Handler
//#define TIM7_Handler            Timer7_Handler

//#define crossbar_int_handler     Crossbar_Handler
//#define Log_Uart_Handler         UART1_Handler
//#define Data_Uart_Handler        UART0_Handler
//#define SPIFLASHIntrHandler      SPIC_Handler
//#define Gpio2IntrHandler         GPIO2_Handler
//#define Gpio3IntrHandler         GPIO3_Handler
//#define Gpio4IntrHandler         GPIO4_Handler
//#define Gpio5IntrHandler         GPIO5_Handler
//#define Timer3IntrHandler        TIM3_Handler
//#define Timer4IntrHandler        TIM4_Handler
//#define Timer5IntrHandler        TIM5_Handler
//#define Timer6IntrHandler        TIM6_Handler
//#define Timer7IntrHandler        TIM7_Handler
//#define Gpio6IntrHandler         GPIO6_Handler
//#define Gpio7IntrHandler         GPIO7_Handler
//#define Gpio8IntrHandler         GPIO8_Handler
//#define Gpio9IntrHandler         GPIO9_Handler
//#define Gpio10IntrHandler        GPIO10_Handler
//#define Gpio11IntrHandler        GPIO11_Handler
//#define Gpio12IntrHandler        GPIO12_Handler
//#define Gpio13IntrHandler        GPIO13_Handler
//#define Gpio14IntrHandler        GPIO14_Handler
//#define Gpio15IntrHandler        GPIO15_Handler
//#define Gpio16IntrHandler        GPIO16_Handler
//#define Gpio17IntrHandler        GPIO17_Handler
//#define Gpio18IntrHandler        GPIO18_Handler
//#define Gpio19IntrHandler        GPIO19_Handler
//#define Gpio20IntrHandler        GPIO20_Handler
//#define Gpio21IntrHandler        GPIO21_Handler
//#define Gpio22IntrHandler        GPIO22_Handler
//#define Gpio23IntrHandler        GPIO23_Handler
//#define Gpio24IntrHandler        GPIO24_Handler
//#define Gpio25IntrHandler        GPIO25_Handler
//#define Gpio26IntrHandler        GPIO26_Handler
//#define Gpio27IntrHandler        GPIO27_Handler
//#define Gpio28IntrHandler        GPIO28_Handler
//#define Gpio29IntrHandler        GPIO29_Handler
//#define Gpio30IntrHandler        GPIO30_Handler
//#define Gpio31IntrHandler        GPIO31_Handler

#ifdef __cplusplus
}
#endif

#endif /* _ISR_RENAME_H_ */
