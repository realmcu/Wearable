#include "time_adapter.h"
#include "trace.h"
#include "rtl876x_gdma.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_aon.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "app_task.h"
#include "vector_table.h"

#include "jpeg_coda.h"
#include "jpeg_heap.h"
void _system_init_(void)
{
    __enable_irq();
    *(uint32_t *)(0x40006024) |= 0xFFFF00;
    btaon_fast_write_safe(0x744, 0x00);
    extern void log_uart_hw_init(void);
    log_uart_hw_init();

    *(uint32_t *)(0x40006024) |= BIT29 | BIT30; //force clock always on  fix uart baudrate
}

void irq_init()
{
//    NVIC_InitTypeDef NVIC_InitStruct;
//    NVIC_InitStruct.NVIC_IRQChannel = RTZIP_VECTORn;
//    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
//    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStruct);
}

void mydelay()
{
    for (uint32_t i = 0; i < 9999; i++)
    {}
}

int main(void)
{
    irq_init();
    __enable_irq();
    _system_init_();
//  init_timer();
//  delay_us(100);
//  Pad_Config(P2_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);

    DBG_DIRECT("Hello");

    Pad_Config(P2_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    jpg_tlsf_init();
    coda_init();


    Pad_Config(P2_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    // CODA_Test(3);
    CODA_Test(4);
    DBG_DIRECT("Done");

//    task_init();
//    sw_timer_init();

//    vTaskStartScheduler();


    // {
    //     DBG_DIRECT(" *gpio_demo start");
    //     while(1)
    //     {
    //          Pad_Config(P2_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    //          mydelay();
    //  Pad_Config(P2_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    //          mydelay();
    //     }
    // }




    while (1)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
    return 0;
}


