/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    System_rtl876x.h
  * @brief   CMSIS Device System Header file
  * @details CMSIS Device System Header File for RTL876x Device Series
  * @author
  * @date    3. March 2016
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
   * ************************************************************************************
   */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef SYSTEM_RTL876X_H
#define SYSTEM_RTL876X_H


/*============================================================================*
 *                      Headers
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>


/** @defgroup SYSTEM_RTL876X_API  System Rtl876x
  * @brief CMSIS API sets for RTL876x Device Series
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                      Types
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_types SYSTEM RTL876X Exported types
  * @{
  */
typedef void (*IRQ_Fun)(void);       /**< ISR Handler Prototype */

#define CLOCK_40MHZ 40000000
#define CLOCK_53MHZ 53333333
#define CLOCK_80MHZ 80000000
#define CLOCK_90MHZ 90000000

typedef enum _DSP_SLOW_FREQ_TYPE
{
    DSP_SLOW_1MHZ,
    DSP_SLOW_2MHZ,
    DSP_SLOW_3MHZ,
    DSP_SLOW_4MHZ,
    DSP_SLOW_5MHZ,
    DSP_SLOW_10MHZ,
    DSP_SLOW_15MHZ,
    DSP_SLOW_20MHZ,
    DSP_SLOW_40MHZ
} DSP_SLOW_FREQ_TYPE;

typedef enum _CLK_FREQ_CPU_TYPE
{
    CPU_40MHZ,
    CPU_30MHZ,
    CPU_20MHZ,
    CPU_10MHZ,
    CPU_8MHZ,
    CPU_5MHZ,
    CPU_4MHZ,
    CPU_80MHZ,
    CPU_53MHZ,
    CPU_70MHZ, // only used when PLL = 80MHz
    CPU_60MHZ, // only used when PLL = 80MHz, 240MHz
    CPU_MAX_FREQ,
} CLK_FREQ_CPU_TYPE;

typedef enum _USE_32K_CLK_SRC_TYPE
{
    USE_32K_RCOSC = 0,
    USE_32K_EXT_XTAL = 1,
    USE_40M_LP_MODE = 2,
} USE_32K_CLK_SRC_TYPE;

typedef enum _XTAL_FREQ_TYPE
{
    XTAL_16MHZ,
    XTAL_24MHZ,
    XTAL_26MHZ,
    XTAL_32MHZ,
    XTAL_38P4MHZ,
    XTAL_40MHZ,
    XTAL_MAX_FREQ,
} XTAL_FREQ_TYPE;

typedef enum _PLL_FREQ_TYPE
{
    PLL_40MHZ,
    PLL_160MHZ,
    PLL_240MHZ,
    PLL_320MHZ,
    PLL_80MHZ,
    PLL_120MHZ,
    PLL_MAX_FREQ,
} PLL_FREQ_TYPE;

typedef enum _CLK_DIV_TYPE
{
    CPU_CLK_DIV,
    DSP_CLK_DIV,
    FLASH_CLK_DIV,
    FIX_40M_CLK_DIV,
} CLK_DIV_TYPE;

typedef enum _ACTIVE_CLK_TYPE
{
    CLK_CPU,
    CLK_DSP,
    CLK_FLASH,
    CLK_40M,
} ACTIVE_CLK_TYPE;

typedef enum _BUS_SLOW_SEL_TYPE
{
    CLK_BUS_RATIO_1_1,
    CLK_BUS_RATIO_1_1_,
    CLK_BUS_RATIO_1_2,
    CLK_BUS_RATIO_1_4,
} BUS_SLOW_SEL_TYPE;

typedef enum
{
    InitialSP_VECTORn = 0,
    Reset_VECTORn,
    NMI_VECTORn,
    HardFault_VECTORn,
    MemMang_VECTORn,
    BusFault_VECTORn,
    UsageFault_VECTORn,
    RSVD0_VECTORn,
    RSVD1_VECTORn,
    RSVD2_VECTORn,
    RSVD3_VECTORn,
    SVC_VECTORn,
    DebugMonitor_VECTORn,
    RSVD4_VECTORn,
    PendSV_VECTORn,
    SysTick_VECTORn,
    System_VECTORn = 16,
    WDG_VECTORn,
    BTMAC_VECTORn,
    USB_SDIO_VECTORn,
    Timer2_VECTORn,
    Platform_VECTORn,
    I2S0_VECTORn,
    I2S1_VECTORn,
    UART2_VECTORn,
    GPIO0_VECTORn,
    GPIO1_VECTORn,
    UART1_VECTORn,
    UART0_VECTORn,
    RTC_VECTORn,
    SPI0_VECTORn,
    USB_ISOC_VECTORn,
    I2C0_VECTORn,
    I2C1_VECTORn,
    ADC_VECTORn,
    Peripheral_VECTORn,
    GDMA0_Channel0_VECTORn,
    GDMA0_Channel1_VECTORn,
    GDMA0_Channel2_VECTORn,
    GDMA0_Channel3_VECTORn,
    GDMA0_Channel4_VECTORn,
    GDMA0_Channel5_VECTORn,
    Keyscan_VECTORn,
    Qdecode_VECTORn,
    IR_VECTORn,
    DSP_VECTORn,
    GDMA0_Channel6_VECTORn,
    GDMA0_Channel7_VECTORn,
    SPIFlash_VECTORn = 48,
    GPIO2_VECTORn,
    GPIO3_VECTORn,
    GPIO4_VECTORn,
    GPIO5_VECTORn,
    Timer3_VECTORn = 53,
    Timer4_VECTORn,
    Timer5_VECTORn,
    Timer6_VECTORn,
    Timer7_VECTORn,
    MFB_DET_L_VECTORn,
    LPCOMP_VECTORn,
    VBAT_DET_VECTORn,
    ADP_DET_VECTORn,
    ASRC1_VECTORn,
    ASRC2_VECTORn,
    GPIO6_VECTORn = 64,
    GPIO7_VECTORn,
    GPIO8_VECTORn,
    GPIO9_VECTORn,
    GPIO10_VECTORn,
    GPIO11_VECTORn,
    GPIO12_VECTORn,
    GPIO13_VECTORn,
    GPIO14_VECTORn,
    GPIO15_VECTORn,
    GPIO16_VECTORn,
    GPIO17_VECTORn,
    GPIO18_VECTORn,
    GPIO19_VECTORn,
    GPIO20_VECTORn,
    GPIO21_VECTORn,
    GPIO22_VECTORn,
    GPIO23_VECTORn,
    GPIO24_VECTORn,
    GPIO25_VECTORn,
    GPIO26_VECTORn,
    GPIO27_VECTORn,
    GPIO28_VECTORn,
    GPIO29_VECTORn,
    GPIO30_VECTORn,
    GPIO31_VECTORn,
    PTA_Mailbox_VECTORn = 90,
    DSP_WDG_VECTORn,
    FlashPower_DET_VECTORn,
    Utmi_Suspend_N_VECTORn,
    RESERVED0_VECTORn,
    RESERVED1_VECTORn,
    RESERVED2_VECTORn,
    RESERVED3_VECTORn,
    SPORT0_TX_VECTORn,
    SPORT0_RX_VECTORn,
    SPORT1_TX_VECTORn,
    SPORT1_RX_VECTORn,
    ANC_AMP0_VECTORn,
    ANC_AMP1_VECTORn,
    RNG_VECTORn,
    ADP_IN_DET_VECTORn,
    ADP_OUT_DET_VECTORn,
} VECTORn_Type;

typedef enum
{
    ADP_DET_ING = 0,
    ADP_DET_IN,
    ADP_DET_OUT,
} ADP_DET_Type;

typedef enum
{
    BYPASS_EFUSE_VALID = 0,
    BYPASS_EFUSE_INVALID,
    NORMAL_EFUSE_VALID,
    NORMAL_EFUSE_INVALID,
} BOOT_Type;

typedef enum
{
    EFUSE_INVALID = 0,
    FLASH_INVALID,
} BOOT_ERROR_Type;

typedef enum
{
    LOAD_NONE                   = 0,
    LOAD_LOGICAL_PARA           = 1,
    LOAD_FT_PARA                = 2,
    LOAD_OEM_PARA               = 4,
    LOAD_ALL                    = 7,
} EFUSE_LOAD_Type;

typedef enum
{
    CHECK_NONE                  = 0,
    CHECK_EUID                  = 1,
    CHECK_SECURE_BOOT_CTRL      = 2,
    CHECK_SYSTEM_FUNC_CTRL      = 4,
    CHECK_GENERAL_FUNC_CTRL     = 8,
    CHECK_PRIVATE_FUNC_CTRL     = 16,
    CHECK_KEY_AND_PASSWORD      = 32,
    CHECK_ALL                   = 63,
} EFUSE_CHECK_Type;

typedef enum
{
    SWD_CLOSE = 0,
    SWD_NOT_DEFINE,
    SWD_PROTECT,
    SWD_OPEN,
} SWD_CONTROL_Type;

typedef union
{
    uint8_t val;
    struct
    {
        uint8_t in_stock_name: 4;
        uint8_t part_number: 3;
        uint8_t package_id_invalid: 1;
    };
} EFUSE_PACKAGE_ID_TYPE;

#define EFUSE_PACKAGE_ID_ADDR               0x18a
#define EFUSE_PACKAGE_ID_INVERSE_ADDR       0x19d
#define ENCRYPTED_CHECK_IMAGE_SIZE          DATA_RAM_PATCH_SIZE
//#define ENCRYPTED_CHECK_IMAGE_ADDR  \
//    (flash_get_bank_addr(FLASH_PATCH) + \
//     flash_get_bank_size(FLASH_PATCH) + 0x1000 - ENCRYPTED_CHECK_IMAGE_SIZE)
#define DECRYPT_CHECK_IMAGE_KEY_SIZE        16 // bytes
#define DECRYPT_CHECK_IMAGE_KEY_ADDR        (ENCRYPTED_CHECK_IMAGE_ADDR - DECRYPT_CHECK_IMAGE_KEY_SIZE)
#define EXECUTE_CHECK_IMAGE_ADDR            (DATA_RAM_START_ADDR + DATA_RAM_ROM_GLOBAL_SIZE)

#define IS_WHOLE_CHIP_RESET                 (!(btaon_fast_read(BTAON_FAST_BOOT) & BIT0))
#define MARK_NOT_FIRST_BOOT()               (btaon_fast_update(BTAON_FAST_BOOT, BIT0, BIT0))
#define INVALID_CNT_THRESHOLD               15

/* boot flow debug */
#define BOOT_STAGE_RECORD(x)                btaon_fast_update(BTAON_FAST_BOOT, 1 << (x), 1 << (x))
#define ALREADY_ENTER_SYSTEMINIT            4
#define ALREADY_LOAD_EFUSE                  5
#define ALREADY_LOAD_CHECK_PACKAGE_ID_PATCH 6
#define ALREADY_LOAD_ROM_PATCH              7
/** @} */ /* End of group SYSTEM_RTL876X_Exported_types */


/*============================================================================*
 *                      Variables
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Variables SYSTEM RTL876X Exported Variables
  * @{
  */

extern uint32_t SystemCpuClock;      /**< System Clock Frequency (Core Clock)*/
extern IRQ_Fun  RamVectorTable[];    /**< RAM Vector Table */
extern ADP_DET_Type ADP_DET_status;

/** @} */ /* End of group SYSTEM_RTL876X_Exported_Variables */

/*============================================================================*
 *                      Functions
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Functions SYSTEM RTL876X Sets Exported Functions
    * @brief
    * @{
    */

/**
 * @brief  Initialize RAM vector table to a given RAM address.
 * @param  ram_vector_addr: RAM Vector Address.
 * @retval true: Success
 *         false: Fail
 * @note   When using vector table relocation, the base address of the new vector
 *         table must be aligned to the size of the vector table extended to the
 *         next larger power of 2. In RTL8763, the base address is aligned at 0x100.
 */
extern bool RamVectorTableInit(uint32_t ram_vector_addr);

/**
 * @brief  Update ISR Handler in RAM Vector Table.
 * @param  v_num: Vector number(index)
 * @param  isr_handler: User defined ISR Handler.
 * @retval true: Success
 *         false: Fail
 */
extern bool RamVectorTableUpdate(VECTORn_Type v_num, IRQ_Fun isr_handler);

/**
 * @brief  Call the system service.
 * @param  opcode: operation code.
 * @param  parm: parameter.
 * @return none
 */
extern uint32_t SystemCall(uint32_t opcode, uint32_t parm1, uint32_t parm2);

extern void prepare_flash_for_access(void);
extern bool check_power_on_trap(bool is_whole_chip_reset);
extern void (*set_active_mode_clk_src)(void);
extern void (*set_cpu_clock)(uint8_t clk_src, uint8_t div, uint8_t div_slow);
extern void (*set_dsp_clock)(uint8_t clk_src, uint8_t div, uint8_t div_slow);
extern void (*set_flash_clock)(uint8_t clk_src, uint8_t div, uint8_t div_slow);
extern void (*set_40M_clock)(uint8_t clk_src, uint8_t div, uint8_t div_slow);
extern void (*set_adc_dac_clock)(void);
extern void (*set_MHXtal_clk_src)(bool is_enable);
extern void (*set_40M_rcosc)(bool is_enable);
extern void (*set_PLL_clk_src)(bool is_enable);
extern void (*pmu_system_init)(void);
extern void (*set_bus_slow_sel)(uint8_t bus_slow_sel);
extern uint8_t (*count_clk_40m_div)(uint8_t clk_src_40m);
extern uint8_t (*count_clk_cpu_dsp_flash_div)(uint32_t clk_src_type, uint32_t clk_rate_wanted);
extern bool check_need_mhz_xtal(void);
extern bool check_need_pll(void);
extern bool check_need_40m_rcosc(void);

extern void (*si_flow_data_init)(void);
extern void (*ft_paras_apply)(void);
extern void (*set_auto_pwm_pfm_before_power_on_sequence_restart)(void);
extern void (*set_auto_pwm_pfm_after_power_on_sequence_restart)(void);
extern void (*set_auto_pwm_pfm_reseume_from_dlps)(void);
extern void (*set_auto_pwm_pfm_before_enter_dlps)(void);
extern void turn_on_needed_clock_source(void);
extern void (*ldo_fw_handle_ldo318_ldo_pa)(bool, bool);
extern void (*ldo_fw_handle_ldo733_lq)(bool);
extern void (*ldo_fw_handle_ldo_aux_lq)(bool, bool);
extern void (*ldo_fw_handle_ldo_audio_lq)(bool, bool, bool, bool);
extern void (*ldo_fw_handle_pcut_dummy_load)(bool);
extern void (*hal_setup_hardware)(void);
extern void (*hal_setup_cpu)(void);
extern bool (*enable_cache)(void);
extern bool (*mpu_setup)(void);
extern void (*hal_setup_cpu)(void);
extern void prepare_rom_boot(void);
extern void copy_data_and_clear_bss(void);
extern void LOGUARTDriverInit(void);
extern uint8_t Hex2Char(uint8_t hex);
extern void ProgramStart(void);
extern void efuse_clear_ram_data(uint32_t begin_addr, uint32_t end_addr);
extern void efuse_set_read_protect(void);
extern void efuse_set_write_protect(void);
extern void memory_set_read_protect(void);
extern void memory_set_write_protect(void);
extern void efuse_temper_dectect(uint8_t *data, uint8_t byte_size, uint8_t num_of_zeros,
                                 uint8_t inv_num_of_zeros);
extern bool efuse_validation(EFUSE_LOAD_Type load_type, EFUSE_CHECK_Type check_type);
extern void efuse_system_general_control(void);
extern void efuse_swd_control(bool before_secure_boot);
extern void efuse_clear_private_data(void);
extern void efuse_update_parameter(void);
extern void boot_error_handler(BOOT_ERROR_Type boot_error_type);
extern void set_clk_when_wakeup(void);

extern uint8_t count_0bits(uint32_t x);
extern uint8_t count_1bits(uint32_t x);
extern uint8_t count_zeros(uint8_t *data, uint8_t byte_size);
extern bool check_identical(uint8_t data, uint8_t identical_data, uint8_t bit_mask);
extern bool check_inverse(uint8_t data, uint8_t inverse_data);
extern bool check_zero_count_and_inverse(uint8_t *data, uint8_t byte_size, uint8_t num_of_zeros,
                                         uint8_t inv_num_of_zeros);
extern uint8_t check_field(uint8_t field, uint8_t field_bit_size);
extern bool data_cmp(const void *d1, const void *d2, uint32_t length);
extern void activate_flash_power(bool is_on);

/** @} */ /* End of group SYSTEM_RTL876X_Exported_Functions */


/************************************************************************** used for vector table */
extern void Reset_Handler(void);
extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);
extern void System_Handler(void);
extern void WDG_Handler(void);
extern void BTMAC_Handler(void);
extern void M_Peripheral_ISR_Handler(void);
extern void TIM2_Handler(void);
extern void HardFault_Handler(void);
extern void I2S0_Handler(void);
extern void I2S1_Handler(void);
extern void UART2_Handler(void);
extern void GPIO0_Handler(void);
extern void GPIO1_Handler(void);
extern void UART1_Handler(void);
extern void UART0_Handler(void);
extern void RTC_Handler(void);
extern void SPI0_Handler(void);
extern void USB_ISOC_Handler(void);
extern void I2C0_Handler(void);
extern void I2C1_Handler(void);
extern void ADC_Handler(void);
extern void Peripheral_Handler(void);
extern void GDMA0_Channel0_Handler(void);
extern void GDMA0_Channel1_Handler(void);
extern void GDMA0_Channel2_Handler(void);
extern void GDMA0_Channel3_Handler(void);
extern void GDMA0_Channel4_Handler(void);
extern void GDMA0_Channel5_Handler(void);
extern void KeyScan_Handler(void);
extern void QDEC_Handler(void);
extern void IR_Handler(void);
extern void DSP_Handler(void);
extern void GDMA0_Channel6_Handler(void);
extern void GDMA0_Channel7_Handler(void);
extern void SPI_Flash_Handler(void);
extern void GPIO2_Handler(void);
extern void GPIO3_Handler(void);
extern void GPIO4_Handler(void);
extern void GPIO5_Handler(void);
extern void TIM3_Handler(void);
extern void TIM4_Handler(void);
extern void TIM5_Handler(void);
extern void TIM6_Handler(void);
extern void TIM7_Handler(void);
extern void MFB_DET_L_Handler(void);
extern void LPCOMP_Handler(void);
extern void VBAT_DET_Handler(void);
extern void ADP_DET_Handler(void);
extern void ASRC1_Handler(void);
extern void ASRC2_Handler(void);
extern void GPIO6_Handler(void);
extern void GPIO7_Handler(void);
extern void GPIO8_Handler(void);
extern void GPIO9_Handler(void);
extern void GPIO10_Handler(void);
extern void GPIO11_Handler(void);
extern void GPIO12_Handler(void);
extern void GPIO13_Handler(void);
extern void GPIO14_Handler(void);
extern void GPIO15_Handler(void);
extern void GPIO16_Handler(void);
extern void GPIO17_Handler(void);
extern void GPIO18_Handler(void);
extern void GPIO19_Handler(void);
extern void GPIO20_Handler(void);
extern void GPIO21_Handler(void);
extern void GPIO22_Handler(void);
extern void GPIO23_Handler(void);
extern void GPIO24_Handler(void);
extern void GPIO25_Handler(void);
extern void GPIO26_Handler(void);
extern void GPIO27_Handler(void);
extern void GPIO28_Handler(void);
extern void GPIO29_Handler(void);
extern void GPIO30_Handler(void);
extern void GPIO31_Handler(void);
extern void PTA_Mailbox_Handler(void);
extern void DSP_WDG_Handler(void);
extern void FlashPower_DET_Handler(void);
extern void Utmi_Suspend_N_Handler(void);
extern void RESERVED0_Handler(void);
extern void RESERVED1_Handler(void);
extern void RESERVED2_Handler(void);
extern void RESERVED3_Handler(void);
extern void SPORT0_TX_Handler(void);
extern void SPORT0_RX_Handler(void);
extern void SPORT1_TX_Handler(void);
extern void SPORT1_RX_Handler(void);
extern void ANC_AMP0_Handler(void);
extern void ANC_AMP1_Handler(void);
extern void RNG_Handler(void);
extern void ADP_IN_DET_Handler(void);
extern void ADP_OUT_DET_Handler(void);
/**************************************************************************************************/
#ifdef __cplusplus
}
#endif


/** @} */ /* End of group SYSTEM_RTL876X_API */



#endif /* SYSTEM_RTL876X_H */

