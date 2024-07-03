#include "rtl876x.h"//new include
#include "bblite_platform.h"
//#include "core_cm4.h"
#include <string.h>
#include "system_rtl876x.h"
#include "vector_table_auto_gen.h"


void SystemInit(void)
{

    __disable_irq();

    // #if (__FPU_USED == 1)
    SCB->CPACR |= 0xF << 20; //Enable full access to the FPU
    //  #endif

    VTORInit();

    extern void gpio_int_init(void);
    gpio_int_init();

//      /* enable share ram */
//      //enable share ram clock and function
//    *(uint32_t *)0x40000244 |= 0x1;
//    *(uint32_t *)0x40000210 |= 0x4;
//    *(uint32_t *)0x40000218 = 0x8E000000;
//    *(uint32_t *)0x40000238 = 0x00004000;


    return;
}

extern int main(void);
void Reset_Handler(void)
{
    SystemInit();

    copy_data_and_clear_bss();

    main();
}


//add platform clock setting here

void Set_platform_Clock(void)
{

}

void HardFault_Handler_C(uint32_t *hardfault_args, uint32_t lr_value,
                         uint32_t *callee_saved_registers)
{
    while (1);
}
