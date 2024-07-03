/**
************************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     platform_utils.h
* @brief    utility helper function for user application
* @author   lory_xu
* @date     2017-02
* @version  v1.0
*************************************************************************************************************
*/

#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include "patch.h"
#include "trace.h"
#include "vector_table.h"

/** @defgroup  PLATFORM_UTILS Platform Utilities
    * @brief Utility helper functions
    * @{
    */

#define RTK_STATIC_ASSERT_MSG(MSG, COUNTER) RTK_STATIC_ASSERT_MSG_(MSG, COUNTER)
#define RTK_STATIC_ASSERT_MSG_(MSG, COUNTER) static_assertion_##MSG##COUNTER
#define RTK_STATIC_ASSERT(COND,MSG) typedef char RTK_STATIC_ASSERT_MSG(MSG, __COUNTER__)[(COND)?1:-1];

#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define STATIC_ASSERT(e, m) \
    enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(!!(e)) }

#define    SIZE_OF_TYPE_EQUAL_TO(type, size, msg) \
    RTK_STATIC_ASSERT(sizeof(type) == size, msg)

#define BUILD_BUG_ON_ZERO(e)  (sizeof(struct { int _; int:-!!(e); }) - sizeof(struct { int _; }))
#define SAME_TYPE(a, b)  __builtin_types_compatible_p(typeof(a), typeof(b))
#define MUST_BE_ARRAY(a)  BUILD_BUG_ON_ZERO(SAME_TYPE((a), &(*a)))
#define ARRAY_SIZE(a) ((sizeof(a) / sizeof(*a)) + MUST_BE_ARRAY(a))
#define hex2char(val) ("0123456789abcdef"[(val) & 0xf])
#define count_1bits(x) (uint8_t)__builtin_popcount(x)
#define count_0bits(x) (32 - count_1bits(x))
#define check_inverse(data, inverse_data, mask) ((data ^ inverse_data) == mask)
#define SET_BIT(val, mask)          ((val) |=  (mask))
#define CLR_BIT(val, mask)          ((val)  &= ~(mask))

/** Calculate integer bit count of b'1 */
#define INT_BIT_COUNT(integer, count)   {               \
        count = 0;                                      \
        while (integer)                                 \
        {                                               \
            count++;                                    \
            integer &= integer - 1;                     \
        }                                               \
    }

/** Stream skip len */
#define STREAM_SKIP_LEN(s, len)     {                   \
        s += len;                                       \
    }

/** Stream to array */
#define STREAM_TO_ARRAY(a, s, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *((uint8_t *)(a) + ii) = *s++;              \
        }                                               \
    }

/** Array to stream */
#define ARRAY_TO_STREAM(s, a, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *s++ = *((uint8_t *)(a) + ii);              \
        }                                               \
    }

/** Little Endian stream to uint8 */
#define LE_STREAM_TO_UINT8(u8, s)   {                   \
        u8  = (uint8_t)(*s);                            \
        s  += 1;                                        \
    }

/** Little Endian stream to uint16 */
#define LE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 0) +            \
               ((uint16_t)(*(s + 1)) << 8);             \
        s   += 2;                                       \
    }

/** Little Endian stream to uint24 */
#define LE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16);            \
        s   += 3;                                       \
    }

/** Little Endian stream to uint32 */
#define LE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16) +           \
               ((uint32_t)(*(s + 3)) << 24);            \
        s   += 4;                                       \
    }

/** Little Endian uint8 to stream */
#define LE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Little Endian uint16 to stream */
#define LE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
    }

/** Little Endian uint24 to stream */
#define LE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >>  0);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >> 16);                  \
    }

/** Little Endian uint32 to stream */
#define LE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >>  0);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >> 24);                  \
    }

/** Little Endian array to uint8 */
#define LE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Little Endian array to uint16 */
#define LE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 0) +             \
              ((uint16_t)(*(a + 1)) << 8);              \
    }

/** Little Endian array to uint24 */
#define LE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16);             \
    }

/** Little Endian array to uint32 */
#define LE_ARRAY_TO_UINT32(u32, a) {                    \
        u32 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16) +            \
              ((uint32_t)(*(a + 3)) << 24);             \
    }

/** Little Endian uint8 to array */
#define LE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Little Endian uint16 to array */
#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

/** Little Endian uint24 to array */
#define LE_UINT24_TO_ARRAY(a, u24) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >> 16); \
    }

/** Little Endian uint32 to array */
#define LE_UINT32_TO_ARRAY(a, u32) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >> 24); \
    }

/** Big Endian stream to uint8 */
#define BE_STREAM_TO_UINT8(u8, s)   {                   \
        u8   = (uint8_t)(*(s + 0));                     \
        s   += 1;                                       \
    }

/** Big Endian stream to uint16 */
#define BE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 8) +            \
               ((uint16_t)(*(s + 1)) << 0);             \
        s   += 2;                                       \
    }

/** Big Endian stream to uint24 */
#define BE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) << 16) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) <<  0);            \
        s   += 3;                                       \
    }

/** Big Endian stream to uint32 */
#define BE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) << 24) +           \
               ((uint32_t)(*(s + 1)) << 16) +           \
               ((uint32_t)(*(s + 2)) <<  8) +           \
               ((uint32_t)(*(s + 3)) <<  0);            \
        s   += 4;                                       \
    }

/** Big Endian uint8 to stream */
#define BE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Big Endian uint16 to stream */
#define BE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
    }

/** Big Endian uint24 to stream */
#define BE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >> 16);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >>  0);                  \
    }

/** Big Endian uint32 to stream */
#define BE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >> 24);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >>  0);                  \
    }

/** Big Endian array to uint8 */
#define BE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Big Endian array to uint16 */
#define BE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 8) +             \
              ((uint16_t)(*(a + 1)) << 0);              \
    }

/** Big Endian array to uint24 */
#define BE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) << 16) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) <<  0);             \
    }

/** Big Endian array to uint32 */
#define BE_ARRAY_TO_UINT32(u32, a)  {                   \
        u32 = ((uint32_t)(*(a + 0)) << 24) +            \
              ((uint32_t)(*(a + 1)) << 16) +            \
              ((uint32_t)(*(a + 2)) <<  8) +            \
              ((uint32_t)(*(a + 3)) <<  0);             \
    }

/** Big Endian uint8 to array */
#define BE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Big Endian uint16 to array */
#define BE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 8);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 0);  \
    }

/** Big Endian uint24 to array */
#define BE_UINT24_TO_ARRAY(a, u24)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >> 16); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >>  0); \
    }

/** Big Endian uint32 to array */
#define BE_UINT32_TO_ARRAY(a, u32)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >> 24); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >>  0); \
    }

#define IRQ_RESET_CNT()         (irq_lock_nest_cnt = 0)
#define IRQ_LOCK(ptr_primask)   (irq_lock_imp(ptr_primask))
#define IRQ_UNLOCK(ptr_primask) (irq_unlock_imp(ptr_primask, __LINE__))
#define BOOT_ROM_ASSERT()                                                                               \
    do                                                                                                  \
    {                                                                                                   \
        DIRECT_LOG("BOOT ROM ASSERT! pc = 0x%x, lr = 0x%x", 2, __current_pc(), __return_address());     \
        __disable_irq();                                                                                \
        while (1);                                                                                      \
    }                                                                                                   \
    while (0)                                                                                           \


#define CONFIG_HEADER_SIZE 6

#define CLOCK_COUNT_DIFF(clk_begin, clk_end, bit_mask) (((((uint64_t)clk_end & ((uint64_t)bit_mask - 1)) | (uint64_t)bit_mask) - ((uint64_t)clk_begin & ((uint64_t)bit_mask - 1))) & ((uint64_t)bit_mask - 1))

#define CPU_CNT_BIT_NUM             BIT64(32)

typedef struct
{
    uint32_t start_addr;
    uint32_t total_size;
} T_SHM_INFO;
/*============================================================================*
 *                              Variables
 *============================================================================*/

/* For irq nesting counting only, do not modify this variable directly */
extern uint8_t irq_lock_nest_cnt;

extern volatile uint32_t timestamp_cyclecount;

extern volatile uint32_t cpu_tick_per_us;

extern T_SHM_INFO shm_info;

/**
 * @brief Busy delay for specified micro second
 * @param t   to specify t micro seconds to delay
 * @return none
 */
extern void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for specified millisecond
 * @param t   to specify t milliseconds to delay
 * @return none
 */
extern void (*platform_delay_us)(uint32_t t);

extern VOID_PATCH_FUNC set_clock_gen;
extern BOOL_PATCH_FUNC get_cfg_entry;
extern BOOL_PATCH_FUNC apply_cfg_to_mem;
extern UINT32_PATCH_FUNC parse_sys_cfg_to_mem;
extern BOOL_PATCH_FUNC image_verify;
extern VOID_PATCH_FUNC system_restore;
extern BOOL_PATCH_FUNC flash_ioctl_mp;
extern VOID_PATCH_FUNC mp_free_mem;
extern UINT32_PATCH_FUNC mp_read_cmd_patch;
extern BOOL_PATCH_FUNC mp_parse_cmd_patch;
extern VOIDPTR_PATCH_FUNC mp_alloc_mem;
extern BOOL_PATCH_FUNC patch_log_buffer;
extern VOIDPTR_PATCH_FUNC one_wire_alloc;
extern VOID_PATCH_FUNC one_wire_free;
extern bool (*patch_RamVectorTableUpdate)(uint32_t v_num, IRQ_Fun isr_handler, bool *ret);

#if (SECURE_BOOT_ENABLE == 1)
extern VOID_PATCH_FUNC boot_failed;
extern BOOL_PATCH_FUNC efuse_header_size_verification;
extern BOOL_PATCH_FUNC efuse_legality;
extern VOID_PATCH_FUNC efuse_set_read_protect;
extern VOID_PATCH_FUNC efuse_set_write_protect;
extern VOID_PATCH_FUNC efuse_set_read_protect_range;
extern VOID_PATCH_FUNC efuse_set_write_protect_range;
extern VOID_PATCH_FUNC efuse_swd_control;
extern VOID_PATCH_FUNC efuse_system_general_control;
extern VOID_PATCH_FUNC efuse_update_parameter;
extern VOID_PATCH_FUNC efuse_set_xo_range;
extern BOOL_PATCH_FUNC image_authentication;
extern BOOL_PATCH_FUNC image_decryption;
extern BOOL_PATCH_FUNC is_secure_boot;
extern BOOL_PATCH_FUNC ram_image_authentication;
#endif // (SECURE_BOOT_ENABLE == 1)
/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Functions Platform Utils Exported Functions
    * @brief
    * @{
    */

void irq_lock_imp(uint8_t *ptr_primask);
void irq_unlock_imp(uint8_t *ptr_primask, uint32_t line);
void disable_all_irq(void);
uint8_t count_zeros(uint8_t *data, uint8_t byte_size);
bool check_zero_count_and_inverse(uint8_t *data, uint8_t byte_size, uint8_t num_of_zeros,
                                  uint8_t inv_num_of_zeros);
#if (SECURE_BOOT_ENABLE == 1)
uint8_t check_field(uint8_t field, uint8_t field_bit_size);
#endif // (SECURE_BOOT_ENABLE == 1)

bool data_cmp(const void *d1, const void *d2, uint32_t length);
bool is_overlapped(uint32_t range0[2], uint32_t range1[2]);
void dump_raw_mem(uint32_t *startAddr, uint32_t size);
void dump_raw_memory_all(void);
void system_status_dump(uint32_t *stack_frame, uint32_t lr_value, uint32_t *callee_stack);
void rxi300_register_dump(void);
void cpu_snapshot_dump(void);
void cpu_register_dump(uint32_t *stack_frame, uint32_t lr_value, uint32_t *callee_stack);
void cpu_cache_setup(void);
void empty_function(void);
bool true_function(void);
uint32_t get_simple_rand_num(uint32_t max);
void log_uart_hw_init(void);
uint32_t hw_timer_get_counter(void);
uint32_t hw_timer_get_current_value(void);
void log_uart_hw_deinit(void);
void set_swd_pinmux(void);


/** @} */ /* End of group PLATFORM_UTILS_Exported_Functions */


/** @} */ /* End of group PLATFORM_UTILS */

#ifdef __cplusplus
}
#endif

#endif

