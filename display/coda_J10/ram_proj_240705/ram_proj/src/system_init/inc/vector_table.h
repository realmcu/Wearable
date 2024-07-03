#ifndef VECTOR_TABLE_H
#define VECTOR_TABLE_H

// *INDENT-OFF*
#include "stdint.h"
#include "stdbool.h"
#include "vector_table_auto_gen.h"

#define IRQn_TO_VECTORn(irqn)   ((irqn) + 16)
extern void *RamVectorTable[];
extern bool RamVectorTableUpdate(uint32_t v_num, IRQ_Fun isr_handler);

bool RamVectorTableInit(uint32_t ram_vector_addr);

#define MAX_REGISTER_FIRST_IRQ_NUM      (9)
#define IS_GPIO_FIRST_ISR(irqn)        ((((irqn) >= GPIO_A2_7_IRQn) && ((irqn) <= GPIO_A24_31_IRQn)) || (((irqn) >= GPIO_B0_7_IRQn) && ((irqn) <= GPIO_B24_31_IRQn)))

#define SUB_IRQ_DEF(first_irq, second_index) (((first_irq) << 8) | (second_index))
#define GET_FIRST_IRQn(sub_irq)         ((sub_irq)>>8)
typedef void (* P_SECOND_VECTOR_UPDATE)(uint32_t irqn, IRQ_Fun isr_handler);
bool RamVectorTableRegister(uint8_t irqn, P_SECOND_VECTOR_UPDATE reg_func);

// *INDENT-ON* 
#endif //#define VECTOR_TABLE_H
