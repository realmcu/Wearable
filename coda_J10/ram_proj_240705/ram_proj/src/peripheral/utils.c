#include <stdlib.h>
#include <stddef.h>
#include "rtl876x.h"
#include "vector_table.h"
#include "utils.h"
#include "trace.h"
#include "mem_cfg_int.h"
//#include "boot_cfg.h"
//#include "efuse_table.h"
////#include "efuse_driver.h"
//#include "rtl876x_wdg.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_uart.h"
#include "rtl876x_aon_reg.h"
#include "indirect_access.h"
#include "rtl876x_tim.h"

//#define UNLOCK_FAIL "IRQ_UNLOCK assert!! line = %d, nesting cnt = %d, primask = %d"

//T_SHM_INFO shm_info;

//void irq_lock_imp(uint8_t *ptr_primask)
//{
//    /* Record current primask for restoring in irq_unlock_imp */
//    *ptr_primask = __get_PRIMASK();

//    __disable_irq();
//    irq_lock_nest_cnt++;
//}

//void irq_unlock_imp(uint8_t *ptr_primask, uint32_t line)
//{
//    /* Asymmetrical API calling and illegal primask setting detection */
//    if ((irq_lock_nest_cnt == 0) || ((irq_lock_nest_cnt > 0) && (__get_PRIMASK() == 0)))
//    {
//        DIRECT_LOG(UNLOCK_FAIL, 3, line, irq_lock_nest_cnt, __get_PRIMASK());
//        BOOT_ROM_ASSERT();
//    }

//    irq_lock_nest_cnt--;

//    /* Restore primask */
//    if (*ptr_primask == 0)
//    {
//        __enable_irq();
//    }
//}

///* bb2 has only 64 external IRQs */
//void disable_all_irq(void)
//{
//    /* clear enable bits */
//    NVIC->ICER[0] = 0xFFFFFFFF;
//    NVIC->ICER[1] = 0xFFFFFFFF;

//    /* clear pending bits */
//    NVIC->ICPR[0] = 0xFFFFFFFF;
//    NVIC->ICPR[1] = 0xFFFFFFFF;
//}

//uint8_t count_zeros(uint8_t *data, uint8_t byte_size)
//{
//    uint8_t zero_count = 0;

//    while (byte_size >= 4)
//    {
//        zero_count += count_0bits(*(uint32_t *)data);
//        data += 4;
//        byte_size -= 4;
//    }

//    zero_count += count_0bits((0xffffffff << (byte_size << 3)) | (*(uint32_t *)data));

//    return zero_count;
//}

//bool check_zero_count_and_inverse(uint8_t *data, uint8_t byte_size, uint8_t num_of_zeros,
//                                  uint8_t inv_num_of_zeros)
//{
//    return (count_zeros(data, byte_size) == num_of_zeros &&
//            check_inverse(num_of_zeros, inv_num_of_zeros, 0xFF));
//}

//#if (SECURE_BOOT_ENABLE == 1)
//uint8_t check_field(uint8_t field, uint8_t field_bit_size)
//{
//    uint8_t data_bit_size = field_bit_size >> 1;
//    uint8_t data_bit_mask = ~((0xff >> data_bit_size) << data_bit_size);
//    uint8_t data = (field & data_bit_mask);
//    uint8_t copy = (field >> data_bit_size);

//    if (data != copy)
//    {
//        boot_failed(RESET_ALL);
//    }

//    return data;
//}
//#endif // (SECURE_BOOT_ENABLE == 1)

//// *INDENT-OFF*
//asm bool data_cmp(const void *d1, const void *d2, uint32_t length)
//{
//    PUSH     {r4 - r7, lr}  /* function start */
//    MOVS     r6, #0         /* r6 = result = 0 */
//    MOVS     r7, #0         /* r7 = i = 0 */

//__loop_length
//    LDRB     r4, [r0, r7]   /* r4 = d1[i] */
//    LDRB     r5, [r1, r7]   /* r5 = d2[i] */
//    EOR      r4, r4, r5     /* r4 = d1[i] ^ d2[i] */
//    ORRS     r6, r6, r4     /* result |= r4 */
//    ADDS     r7, r7, #1     /* i ++ */
//    CMP      r7, r2         /* if (i < length) */
//    BCC      __loop_length  /* branch to __loop_length when (i < length) */

//    CBZ      r6, __cmp_equal /* if (result == 0) goto __cmp_equal */
//    MOVS     r0, #0         /* set r0 to return result */
//    POP      {r4 - r7, pc}  /* function return */

//__cmp_equal
//    MOVS     r0, #1         /* set r0 to return result */
//    POP      {r4 - r7, pc}  /* function return */
//}
//// *INDENT-ON*

//bool is_overlapped(uint32_t range0[2], uint32_t range1[2])
//{
//    return ((range0[0] <= range1[1]) && (range1[0] <= range0[1]));
//}

//void empty_function(void)
//{
//    return;
//}

//bool true_function(void)
//{
//    return true;
//}

///************************************ irq control *******************************/
///* For irq nesting counting only, do not modify this variable directly */
//uint8_t irq_lock_nest_cnt = 0;

///************************************ mem dump ************************************/
//void dump_raw_mem(uint32_t *startAddr, uint32_t size)
//{
//    uint8_t l_msg[128] = {0};
//    uint16_t i = 0;

//    if (size & 31)
//    {
//        size = (size + 31) & ((uint32_t)~31);
//    }

//    while (size)
//    {
//        if ((((uint32_t)startAddr >> 2) & 0x7) == 0)
//        {
//            i = 0;
//            l_msg[i] = '0';
//            l_msg[++i] = 'x';
//            for (int j = 28; j >= 0; j -= 4)
//            {
//                l_msg[++i] = hex2char((uint32_t)startAddr >> j);
//            }
//            l_msg[++i] = ':';
//            l_msg[++i] = ' ';
//        }

//        for (int j = 28; j >= 0; j -= 4)
//        {
//            l_msg[++i] = hex2char(*startAddr >> j);
//        }

//        l_msg[++i] = ' ';

//        if ((((uint32_t)startAddr >> 2) & 0x7) == 0x7)
//        {
//            l_msg[++i] = '\n';
//            LogUartTxData(l_msg, i + 1);
//        }

//        startAddr += 1;
//        size -= 4;
//    }
//}

//void dump_raw_memory_all(void)
//{
//    DIRECT_LOG("[Mem Dump]SRAM from addr 0x%x \n", 1, ITCM1_ADDR);
//    dump_raw_mem((uint32_t *)ITCM1_ADDR, ITCM1_SIZE);
//    DIRECT_LOG("[Mem Dump]SRAM from addr 0x%x \n", 1, DTCM0_ADDR);
//    dump_raw_mem((uint32_t *)DTCM0_ADDR, DTCM0_SIZE);
//    DIRECT_LOG("[Mem Dump]SRAM from addr 0x%x \n", 1, DTCM1_ADDR);
//    dump_raw_mem((uint32_t *)DTCM1_ADDR, DTCM1_SIZE);
//    DIRECT_LOG("[Mem Dump]SRAM from addr 0x%x \n", 1, EXT0_ADDR);
//    dump_raw_mem((uint32_t *)EXT0_ADDR, EXT0_SIZE);

//    if (shm_info.total_size)
//    {
//        wdg_kick();
//        DIRECT_LOG("[Mem Dump]SRAM from addr 0x%x \n", 1, shm_info.start_addr);
//        dump_raw_mem((uint32_t *)shm_info.start_addr, shm_info.total_size);
//    }

//    DIRECT_LOG("Mem Dump Done.\n", 0);
//}

//void rxi300_register_dump(void)
//{
//    DIRECT_LOG("RXI300 REG DUMP:PLD0 = 0x%x, PLD1 = 0x%x, ID = 0x%x, ADR0 = 0x%x, ADR1 = 0x%x, CODE = 0x%x",
//               6,
//               RXI300->u_200.ELR_i_PLD0, RXI300->u_204.ELR_i_PLD1, RXI300->u_208.ELR_i_ID,
//               RXI300->u_20C.ELR_i_ADR0, RXI300->u_210.ELR_i_ADR1, RXI300->u_230.ELR_i_CODE);
//    RXI300->u_23C.BITS_23C.ELR_INTR_CLR = 1;
//}

//void cpu_snapshot_dump(void)
//{
//#if (_IS_BB_SERIES_ == 1)
//    if (!check_field(efuse_on_ram.map.cpu_register_dump_when_boot, 2))
//    {
//        return;
//    }
//#endif // (_IS_BB_SERIES_ == 1)
//    DIRECT_LOG("PC:  0x%04x%04x", 2, btaon_fast_read_safe(AON_FAST_0x18A),
//               btaon_fast_read_safe(AON_FAST_0x188));
//    DIRECT_LOG("LR:  0x%04x%04x", 2, btaon_fast_read_safe(AON_FAST_0x18E),
//               btaon_fast_read_safe(AON_FAST_0x18C));
//    DIRECT_LOG("PSR: 0x%04x%04x", 2, btaon_fast_read_safe(AON_FAST_0x192),
//               btaon_fast_read_safe(AON_FAST_0x190));
//}

//void cpu_register_dump(uint32_t *stack_frame, uint32_t lr_value, uint32_t *callee_stack)
//{
//    /* r0~r3, r12, lr, pc, psr are in interrupt stack frame (stack_frame[0 ~ 7]) */
//    /* r4~r11 are pushed by codes, EXC_RETURN is also input by codes */

//    /* Determine whether an application task has used the FPU, using bit[4] of EXC_RETURN.
//     * EXC_RETURN[4] = 1 (8 words) or 0 (26 words).Always 1 when the floating unit is unavailable.
//     */
//    uint8_t stack_frame_size = (lr_value & BIT4) ? (8 * 4) : (26 * 4);

//    /* If the double-word stack alignment feature was enabled, and the value of the
//     * stack pointer was not aligned to double-word boundary, a padding spaces is inserted
//     * to the stack to force the stack pointer to be aligned to double-word location.
//     * xPSR[9] = 1 (padding exists) or 0 (no padding)
//     */
//    uint8_t padding = (stack_frame[7] & BIT9) ? 4 : 0;

//    /* find last SP before ISR happened */
//    uint32_t old_sp = (uint32_t)stack_frame + stack_frame_size + padding;

//    DIRECT_LOG("R0 = 0x%X, R1 = 0x%X, R2 = 0x%X, R3 = 0x%X", 4,
//               stack_frame[0], stack_frame[1], stack_frame[2], stack_frame[3]);
//    DIRECT_LOG("R4 = 0x%X, R5 = 0x%X, R6 = 0x%X, R7 = 0x%X", 4,
//               callee_stack[0], callee_stack[1], callee_stack[2], callee_stack[3]);
//    DIRECT_LOG("R8 = 0x%X, R9 = 0x%X, R10 = 0x%X, R11 = 0x%X", 4,
//               callee_stack[4], callee_stack[5], callee_stack[6], callee_stack[7]);
//    DIRECT_LOG("R12 = 0x%X, LR = 0x%X, PC = 0x%X, PSR = 0x%X, SP = 0x%X", 5,
//               stack_frame[4], stack_frame[5], stack_frame[6], stack_frame[7], old_sp);
//    DIRECT_LOG("MSP = 0x%X, PSP = 0x%x, PRIMASK = 0x%X, BASEPRI = 0x%X, EXC_RETURN = 0x%X", 5,
//               __get_MSP(), __get_PSP(), __get_PRIMASK(), __get_BASEPRI(), lr_value);

//    DIRECT_LOG("CFSR = 0x%X, HFSR = 0x%X, DFSR = 0x%X, AFSR = 0x%X", 4,
//               SCB->CFSR, SCB->HFSR, SCB->DFSR, SCB->AFSR);
//    if (SCB->CFSR & 0x0080)
//    {
//        DIRECT_LOG(" MMFAR = 0x%X", 1, SCB->MMFAR);
//    }

//    if (SCB->CFSR & 0x8000)
//    {
//        DIRECT_LOG(" BFAR = 0x%X", 1, SCB->BFAR);
//    }

//    if (SCB->CFSR & 0x10000)
//    {
//        DIRECT_LOG("Mem addr: 0x%X, value = 0x%X", 2, stack_frame[6], *(uint32_t *)stack_frame[6]);
//    }
//}

//void cpu_cache_setup(void)
//{
//    SCB_OnOffICache(boot_cfg.icache_enable);

//    SCB_OnOffDCache(boot_cfg.dcache_enable);

//    AON_FAST_RG0X_MEMORY_TYPE aon_0x580 = {.d16 = btaon_fast_read_safe(AON_FAST_RG0X_MEMORY)};
//    aon_0x580.reg_ic_data_to_itcm1_en = !boot_cfg.icache_enable && boot_cfg.set_icache_to_itcm1;
//    btaon_fast_write_safe(AON_FAST_RG0X_MEMORY, aon_0x580.d16);
//}

//void system_status_dump(uint32_t *stack_frame, uint32_t lr_value, uint32_t *callee_stack)
//{
//    extern VOID_PATCH_FUNC log_output;
//    extern void log_out(uint8_t  *l_msg);
//    log_output = (VOID_PATCH_FUNC)log_out;

//    if (boot_cfg.hardfault_print_buf_log)
//    {
//        extern VOID_PATCH_FUNC buf_output;
//        buf_output();
//    }

//    switch (__get_IPSR() & 0xFF)
//    {
//    case HardFault_VECTORn:
//        {
//            DIRECT_LOG("Hard Fault Error!", 0);
//            if ((stack_frame[7] & 0xFF) == RXI300_VECTORn)
//            {
//                rxi300_register_dump();
//            }
//            break;
//        }

//    case DebugMonitor_VECTORn:
//        DIRECT_LOG("Debug monitor Interrupt!", 0);
//        break;

//    case WDG_VECTORn:
//        DIRECT_LOG("Watchdog Interrupt!", 0);
//        cpu_register_dump(stack_frame, lr_value, callee_stack);
//        dump_raw_memory_all();
//        return;

//    case RXI300_VECTORn:
//        rxi300_register_dump();
//        break;
//    }

//    cpu_register_dump(stack_frame, lr_value, callee_stack);

//    wdg_start(8000, RESET_ALL);

//    dump_raw_memory_all();

//    if (boot_cfg.resetWhenHardFault)
//    {
//        WDG_SystemReset(RESET_ALL);
//    }
//    else
//    {
//        wdg_disable();
//    }
//}
volatile uint32_t loop_cycle_count = 4;
volatile uint32_t cpu_tick_per_us = (40000000 / 1000000);
/* *INDENT-OFF* */
__attribute__((naked)) void delay_us_asm(uint32_t us)
{
    __asm volatile
    (
        "ldr     r1, = cpu_tick_per_us   \n"
        "ldr     r1, [r1]                \n"/* r0 = us, r1 = cpu_tick_per_us */ 
        "muls    r1, r0, r1              \n"/* r1 = r0 * r1 = "cpu cycles for input us" */ 
        "ldr     r0, = loop_cycle_count  \n"/* r0 = loop_cycle_count (subs + bcs = one loop) cycles */ 
        "ldr     r0, [r0]                \n"
        "udiv    r0, r1, r0              \n"/* r0 = r1 / r0 = "loop count  for input us" */ 
        "cmp     r0, #1                  \n"/* Compare r0 with 1 => if loop count == 0, exit */ 
        "bls     __exit_us               \n"/* Branch if Lower or the Same */
        "subs    r0, r0, #2              \n"/* n -= 2, it means 2 * 3 = 6 instructions */ 

        "nop.n                           \n"
        "nop.n                           \n"
        " .align 4                       \n"
    "__repeat_us:"
        "subs    r0, r0, #1              \n"/* n --; */
        "bcs     __repeat_us             \n"/* Branch if Carry Set */
        "nop.n                           \n"
        "nop.n                           \n"

    "__exit_us:"
        "bx      lr                      \n"/* function return */
        ".align"
    );
}

__attribute__((naked)) void delay_ms_asm(uint32_t ms)
{
    __asm volatile
    (
        "ldr     r1, = cpu_tick_per_us   \n"
        "ldr     r1, [r1]                \n"/* r0 = us, r1 = cpu_tick_per_us */
        "ldr     r2, = 0x3E8             \n"/* r2 = 1000 */
        "muls    r1, r2, r1              \n"/* r1 = r2 * r1 */
        "muls    r1, r0, r1              \n"/* r1 = r0 * r1 */
        "ldr     r0, = loop_cycle_count  \n"/* r0 = loop_cycle_count */
        "ldr     r0, [r0]                \n"
        "udiv    r0, r1, r0              \n"/* r0 = r1/r0 */
        "cmp     r0, #1                  \n"/* Compare r0 with 1 */
        "bls     __exit_ms               \n"/* Branch if Lower or the Same */
        "subs    r0, r0, #2              \n"/* n -= 2 */

        "nop.n                           \n"
        "nop.n                           \n"
        ".align 4                        \n"
    "__repeat_ms:                        \n"
        "subs    r0, r0, #1              \n"/* n --; */
        "bcs     __repeat_ms             \n"/* Branch if Carry Set */
        "nop.n\n"
        "nop.n\n"

    "__exit_ms:                          \n"
        "bx      lr                      \n"/* function return */
        ".align                          \n"
    );
}
/* *INDENT-ON* */
/* *INDENT-ON* */
void (*platform_delay_us)(uint32_t t) = delay_us_asm;
void (*platform_delay_ms)(uint32_t t) = delay_ms_asm;
