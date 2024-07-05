#include "stddef.h"
#include "vector_table.h"
#include "cpu_setting.h"
#include "mem_cfg_int.h"
#include "trace.h"
#include "utils.h"
#include "vector_table_auto_gen.h"

uint8_t irqMap[MAX_REGISTER_FIRST_IRQ_NUM];
P_SECOND_VECTOR_UPDATE regFuncTable[MAX_REGISTER_FIRST_IRQ_NUM];
P_SECOND_VECTOR_UPDATE gpio_regFunc;

bool RamVectorTableRegister(uint8_t irqn, P_SECOND_VECTOR_UPDATE reg_func)
{
    if ((irqn >= MAX_IRQn) || (irqn < SYSTEM_ISR_IRQn))
    {
        return false;
    }

    //gpio isr
    if (IS_GPIO_FIRST_ISR(irqn))
    {
        gpio_regFunc = reg_func;
        return true;
    }

    for (uint32_t i = 0; i < MAX_REGISTER_FIRST_IRQ_NUM; i++)
    {
        if (irqMap[i] == 0)
        {
            irqMap[i] = irqn;
        }

        if (irqMap[i] == irqn)
        {
            regFuncTable[i] = reg_func;
            return true;
        }
    }

    return false;
}

#define VECTORn_TO_IRQn(irqn)   ((irqn) - 16)
bool RamVectorTableUpdate_rom(uint32_t v_num, IRQ_Fun isr_handler)
{
    int32_t irqn = VECTORn_TO_IRQn(v_num);
    if (irqn > MAX_IRQn)
    {
        //second level irq
        P_SECOND_VECTOR_UPDATE reg_fun = NULL;
        uint8_t first_irqn = GET_FIRST_IRQn(irqn);

        if (IS_GPIO_FIRST_ISR(first_irqn))
        {
            reg_fun = gpio_regFunc;
        }
        else
        {
            for (uint32_t i = 0; i < MAX_REGISTER_FIRST_IRQ_NUM; i++)
            {
                if (irqMap[i] == first_irqn)
                {
                    reg_fun = regFuncTable[i];
                    break;
                }
            }
        }

        if (reg_fun)
        {
            reg_fun(irqn, isr_handler);
            return true;
        }

        return false;
    }
    else
    {
        //first level irq
        IRQ_Fun *pRamVectorTable = (IRQ_Fun *)(SCB->VTOR);
        if (pRamVectorTable == 0 || isr_handler == NULL)
        {
            DBG_DIRECT("Can not update vector table in ROM!");
            return false;
        }
        else
        {
            pRamVectorTable[IRQn_TO_VECTORn(irqn)] = isr_handler;
            __DMB();
            return true;
        }
    }
}

bool RamVectorTableUpdate(uint32_t v_num, IRQ_Fun isr_handler)
{
    if (patch_RamVectorTableUpdate)
    {
        bool ret;
        if (patch_RamVectorTableUpdate(v_num, isr_handler, &ret))
        {
            return ret;
        }
    }

    return RamVectorTableUpdate_rom(v_num, isr_handler);
}

