#include "boot_rom_func_ptr.h"

/* Patch Pointers in boot ROM */

#if 0
// spic0 init needs
void(*set_clock_gen)(ACTIVE_CLK_TYPE type,
                     ACTIVE_CLK_SRC_TYPE clk_src) = (void(*)(ACTIVE_CLK_TYPE type,
                                                             ACTIVE_CLK_SRC_TYPE clk_src))empty_function;

// flash function patch pointer in mploader
bool (*flash_ioctl_mp)(uint32_t, FLASH_NOR_IDX_TYPE, uint32_t, uint32_t, uint32_t) = NULL;

// image verify function pointer
bool (*image_verify)(uint32_t img_addr, IMG_ID img_id)                     = NULL;

// parse occd function pointers
bool (*get_cfg_entry)(void *hdr, int32_t *remain_size, void *entry)         = NULL;
bool (*apply_cfg_to_mem)(void *cfg_entry, uint8_t *mem, uint32_t size)     = NULL;
uint32_t (*parse_sys_cfg_to_mem)(PARSING_PARAMETER_FORMAT parse_info,
                                 uint16_t id, uint8_t *mem,
                                 uint32_t size) = (uint32_t(*)(PARSING_PARAMETER_FORMAT parse_info,
                                                               uint16_t id,
                                                               uint8_t *mem,
                                                               uint32_t size))true_function;

// bank switch used in boot patch
bool (*is_ota_support_bank_switch)(void)        = NULL;

// malloc/free functions in mploader
void (*mp_free_mem)(void *ptr, int size)        = NULL;
void *(*mp_alloc_mem)(uint32_t size)             = NULL;

//mp loader command read/parse patch
uint32_t (*mp_read_cmd_patch)(uint16_t cmd_byte, uint8_t *p_cmd_buf)  = NULL;
bool (*mp_parse_cmd_patch)(uint8_t *cmd, uint16_t cmdlen) = NULL;

// malloc/free functions in 1-wire
void (*one_wire_free)(void *ptr)                   = NULL;
void *(*one_wire_alloc)(uint32_t size)              = NULL;

// patch pointer of log buffer
bool (*patch_log_buffer)(uint8_t subtype, uint8_t *l_msg) = NULL;
bool (*patch_check_log_permition)(uint8_t module, bool *ret)       = NULL;
bool (*patch_log_trace_header_build)(uint8_t *l_msg)    = NULL;
#endif
// patch pointer of nvic driver
void (*NVIC_Init)(NVIC_InitTypeDef *NVIC_InitStruct) = NVIC_Init_rom;

// patch pointer of rcc driver
void (*RCC_PeriphClockCmd)(uint32_t APBPeriph, uint32_t APBPeriph_Clock,
                           FunctionalState NewState) = RCC_PeriphClockCmd_rom;

/**@brief patch function pointer for timer */
bool (*patch_TIM_ClkConfig)(TIM_TypeDef *TIMx, TIMClockSrc_TypeDef ClockSrc,
                            TIMClockDiv_TypeDef ClockDiv);
bool (*patch_TIM_TimeBaseInit)(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);
bool (*patch_TIM_StructInit)(TIM_TimeBaseInitTypeDef *TIM_TimeBaseInitStruct);
bool (*patch_TIM_Cmd)(TIM_TypeDef *TIMx, FunctionalState NewState);
bool (*patch_TIM_ChangePeriod)(TIM_TypeDef *TIMx, uint32_t period);
bool (*patch_TIM_InterruptConfig)(TIM_TypeDef *TIMx, uint8_t TIM_INT, FunctionalState NewState);
bool (*patch_TIM_GetCurrentValue)(TIM_TypeDef *TIMx, uint32_t *p_ret);
bool (*patch_TIM_GetInterruptStatus)(TIM_TypeDef *TIMx, uint8_t TIM_INT, ITStatus *p_ret);
bool (*patch_TIM_ClearInterrupt)(TIM_TypeDef *TIMx, uint8_t TIM_INT);
bool (*patch_TIM_GetTimerID)(TIM_TypeDef *TIMx, uint32_t *p_ret);
bool (*patch_TIM_GetCase)(TIM_TypeDef *TIMx, TIMCASE_TypeDef *p_ret);
bool (*patch_TIM_GetTimerShareBase)(TIM_TypeDef *TIMx, TIM_ShareTypeDef **p_ret);
bool (*patch_TIM_PWMDZClockConfig)(PWMDZClockSrc_TypeDef ClockSrc, TIMClockDiv_TypeDef ClockDiv,
                                   FunctionalState NewState);
bool (*patch_TIM_LatchTrigDebClkConfig)(TIMClockDiv_TypeDef ClockDiv, FunctionalState NewState);

// patch pointer of gdma driver
void (*GDMA_Init)(GDMA_ChannelTypeDef *GDMA_Channelx,
                  GDMA_InitTypeDef *GDMA_InitStruct) = GDMA_Init_rom;
uint8_t (*GDMA_Cmd)(uint8_t GDMA_ChannelNum, FunctionalState NewState) = GDMA_Cmd_rom;

#if 0
void (*GDMA_StructInit_lowerstack)(GDMA_InitTypeDef *GDMA_InitStruct) =
    (VOID_PATCH_FUNC)GDMA_StructInit;
#endif

// patch pointer of pinmux driver
void (*Pinmux_Config)(uint8_t Pin_Num, uint8_t Pin_Func) = Pinmux_Config_rom;
void (*Pad_Config)(uint8_t Pin_Num,
                   PAD_Mode AON_PAD_Mode,
                   PAD_PWR_Mode AON_PAD_PwrOn,
                   PAD_Pull_Mode AON_PAD_Pull,
                   PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                   PAD_OUTPUT_VAL AON_PAD_O) = Pad_Config_rom;
void (*Pad_ConfigExt)(uint8_t Pin_Num,
                      PAD_Mode AON_PAD_Mode,
                      PAD_PWR_Mode AON_PAD_PwrOn,
                      PAD_Pull_Mode AON_PAD_Pull,
                      PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                      PAD_OUTPUT_VAL AON_PAD_O,
                      PAD_PULL_VAL AON_PAD_P) = Pad_ConfigExt_rom;
void (*Pad_AllConfigDefault)(void) = Pad_AllConfigDefault_rom;
void (*System_WakeUpPinEnable)(uint8_t Pin_Num, uint8_t Polarity,
                               uint8_t DebounceEn) = System_WakeUpPinEnable_rom;
void (*System_WakeUpPinDisable)(uint8_t Pin_Num) = System_WakeUpPinDisable_rom;
FlagStatus(*System_WakeUpInterruptValue)(uint8_t Pin_Num) = Pad_WakeupInterruptValue;
void (*System_WakeUpInterruptDisable)(uint8_t Pin_Num) = System_WakeUpInterruptDisable_rom;
void (*System_WakeUpInterruptEnable)(uint8_t Pin_Num)  = System_WakeUpInterruptEnable_rom;
uint8_t (*Pad_WakeUpCmd)(WAKEUP_EN_MODE mode, WAKEUP_POL pol,
                         FunctionalState NewState) = Pad_WakeUpCmd_rom;

// patch pointer of uart driver
void (*UART_Init)(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct) = UART_Init_rom;
uint8_t (*UART_SetBaudRate)(UART_TypeDef *UARTx,
                            UartBaudRate_TypeDef baud_rate) = UART_SetBaudRate_rom;
void (*UART_INTConfig)(UART_TypeDef *UARTx, uint32_t UART_IT,
                       FunctionalState newState) = UART_INTConfig_rom;

#if (SECURE_BOOT_ENABLE == 1)
void(*boot_failed)(void) = NULL;
bool (*efuse_header_size_verification)(void)  = NULL;
bool (*efuse_legality)(void)                  = NULL;
void (*efuse_set_read_protect)(void)          = NULL;
void (*efuse_set_write_protect)(void)         = NULL;
void (*efuse_set_read_protect_range)(bool enable, bool lock, uint16_t begin,
                                     uint16_t end)    = NULL;
void (*efuse_set_write_protect_range)(bool enable, bool lock, uint16_t begin,
                                      uint16_t end)   = NULL;
void (*efuse_swd_control)(bool before_secure_boot) = NULL;
void (*efuse_system_general_control)(void)    = NULL;
void (*efuse_update_parameter)(void)          = NULL;
bool (*image_authentication)(uint32_t img_addr, IMG_ID img_id) = NULL;
bool (*image_decryption)(T_IMG_HEADER_FORMAT *header, uint32_t enc_len)                = NULL;
bool (*is_secure_boot)(void)                 = NULL;
bool (*ram_image_authentication)(uint32_t addr, uint32_t size, IMG_ID img_id) = NULL;
#endif // (SECURE_BOOT_ENABLE == 1)

//// patch pointer of efuse driver
//bool (*efuse_one_byte_read)(uint16_t addr, uint8_t *data) = efuse_one_byte_read_rom;
//bool (*efuse_one_byte_write)(uint16_t addr, uint8_t data) = efuse_one_byte_write_rom;

// patch pointer of vector table update
bool (*patch_RamVectorTableUpdate)(uint32_t v_num, IRQ_Fun isr_handler, bool *ret) = NULL;
