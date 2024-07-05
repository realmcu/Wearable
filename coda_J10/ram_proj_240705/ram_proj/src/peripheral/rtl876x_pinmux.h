/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_pinmux.h
* @brief
* @details
* @author    renee
* @date      2024-5-15
* @version   v1.0
* *********************************************************************************************************
*/


#ifndef _RTL876X_PINMUX_H_
#define _RTL876X_PINMUX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x.h"


/** @addtogroup PINMUX PINMUX
  * @brief Pinmux driver module
  * @{
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/


/** @defgroup PINMUX_Exported_Constants PINMUX Exported Constants
  * @{
  */

/**
  * \defgroup    PIN_Function PIN Function
  * \{
  * \ingroup     PINMUX_Exported_Constants
  */

#define IDLE_MODE                   0
#define UART4_TX                    1
#define UART4_RX                    2
#define UART4_CTS                   3
#define UART4_RTS                   4
#define I2C0_CLK                    5
#define I2C0_DAT                    6
#define I2C1_CLK                    7
#define I2C1_DAT                    8
#define PWM9_P                      9
#define PWM9_N                      10
#define PWM4                        11
#define PWM5                        12
#define PWM6                        13
#define PWM7                        14
#define PWM8                        15
#define UART3_TX                    17
#define UART3_RX                    18
#define UART3_CTS                   19
#define UART3_RTS                   20
#define QDEC_PHASE_A_X              21
#define QDEC_PHASE_B_X              22
#define QDEC_PHASE_A_Y              23
#define QDEC_PHASE_B_Y              24
#define QDEC_PHASE_A_Z              25
#define QDEC_PHASE_B_Z              26
#define UART1_TX                    27
#define UART1_RX                    28
#define UART2_TX                    29
#define UART2_RX                    30
#define UART2_CTS                   31
#define UART2_RTS                   32
#define IRDA_TX                     33
#define IRDA_RX                     34
#define UART0_TX                    35
#define UART0_RX                    36
#define UART0_CTS                   37
#define UART0_RTS                   38
#define SPI1_SS_N_0_MASTER          39
#define SPI1_SS_N_1_MASTER          40
#define SPI1_SS_N_2_MASTER          41
#define SPI1_CLK_MASTER             42
#define SPI1_MO_MASTER              43
#define SPI1_MI_MASTER              44
#define SPI_SS_N_0_SLAVE            45
#define SPI_CLK_SLAVE               46
#define SPI_SO_SLAVE                47
#define SPI_SI_SLAVE                48
#define SPI0_SS_N_0_MASTER          49
#define SPI0_CLK_MASTER             50
#define SPI0_MO_MASTER              51
#define SPI0_MI_MASTER              52
#define SWD_CLK                     56
#define SWD_DIO                     57
#define KEY_COL_0                   58
#define KEY_COL_1                   59
#define KEY_COL_2                   60
#define KEY_COL_3                   61
#define KEY_COL_4                   62
#define KEY_COL_5                   63
#define KEY_COL_6                   64
#define KEY_COL_7                   65
#define KEY_COL_8                   66
#define KEY_COL_9                   67
#define KEY_COL_10                  68
#define KEY_COL_11                  69
#define KEY_COL_12                  70
#define KEY_COL_13                  71
#define KEY_COL_14                  72
#define KEY_COL_15                  73
#define KEY_COL_16                  74
#define KEY_COL_17                  75
#define KEY_COL_18                  76
#define KEY_COL_19                  77
#define KEY_ROW_0                   78
#define KEY_ROW_1                   79
#define KEY_ROW_2                   80
#define KEY_ROW_3                   81
#define KEY_ROW_4                   82
#define KEY_ROW_5                   83
#define KEY_ROW_6                   84
#define KEY_ROW_7                   85
#define KEY_ROW_8                   86
#define KEY_ROW_9                   87
#define KEY_ROW_10                  88
#define KEY_ROW_11                  89
#define DWGPIO                      90
#define LRC_SPORT1                  91
#define BCLK_SPORT1                 92
#define ADCDAT_SPORT1               93
#define DACDAT_SPORT1               94
#define DMIC1_CLK                   96
#define DMIC1_DAT                   97
#define LRC_I_CODEC_SLAVE           98
#define BCLK_I_CODEC_SLAVE          99
#define SDI_CODEC_SLAVE             100
#define SDO_CODEC_SLAVE             101
#define LRC_I_PCM                   102
#define BCLK_I_PCM                  103
#define SDI_PCM                     104
#define SDO_PCM                     105
#define BT_COEX_I_0                 106
#define BT_COEX_I_1                 107
#define BT_COEX_I_2                 108
#define BT_COEX_I_3                 109
#define BT_COEX_O_0                 110
#define BT_COEX_O_1                 111
#define BT_COEX_O_2                 112
#define BT_COEX_O_3                 113
#define PTA_I2C_CLK_SLAVE           114
#define PTA_I2C_DAT_SLAVE           115
#define PTA_I2C_INT_OUT             116
#define DSP_GPIO_OUT                117
#define DSP_JTCK                    118
#define DSP_JTDI                    119
#define DSP_JTDO                    120
#define DSP_JTMS                    121
#define DSP_JTRST                   122
#define LRC_SPORT0                  123
#define BCLK_SPORT0                 124
#define ADCDAT_SPORT0               125
#define DACDAT_SPORT0               126
#define MCLK_M                      127
#define SPI0_SS_N_1_MASTER          128
#define SPI0_SS_N_2_MASTER          129
#define SPI2_SS_N_0_MASTER          130
#define SPI2_CLK_MASTER             131
#define SPI2_MO_MASTER              132
#define SPI2_MI_MASTER              133
#define I2C2_CLK                    134
#define I2C2_DAT                    135
#define ISO7816_RST                 136
#define ISO7816_CLK                 137
#define ISO7816_IO                  138
#define ISO7816_VCC_EN              139
#define UART5_TX                    140
#define UART5_RX                    141
#define UART5_CTS                   142
#define UART5_RTS                   143
#define DMIC2_CLK                   144
#define DMIC2_DAT                   145
#define CAN0_TX                     155
#define CAN0_RX                     156
#define CAN1_TX                     157
#define CAN1_RX                     158
#define EN_EXPA                     163
#define EN_EXLNA                    164
#define UART1_CTS                   167
#define UART1_RTS                   168
#define SPIC0_SCK                   169
#define SPIC0_CSN                   170
#define SPIC0_SIO_0                 171
#define SPIC0_SIO_1                 172
#define SPIC0_SIO_2                 173
#define SPIC0_SIO_3                 174
#define LRC_RX_CODEC_SLAVE          175
#define LRC_RX_SPORT0               176
#define LRC_RX_SPORT1               177
#define CLK_CPUDIVIDEDBY2           180
#define CLK_DSPDIVIDEDBY2           181
#define PDM_DATA                    193
#define PDM_CLK                     194
#define ANT_SW0_AOA_AOD             229
#define ANT_SW1_AOA_AOD             230
#define ANT_SW2_AOA_AOD             231
#define ANT_SW3_AOA_AOD             232
#define ANT_SW4_AOA_AOD             233
#define ANT_SW5_AOA_AOD             234
#define DIGI_DEBUG                  255


/** End of PIN_Function
  * \}
  */

/******************* Reference: BB2Ultra_pin_mux_20240621_v0.xlsx *******************/


/**
  * @brief Pinmux. (Pinmux)
  */

typedef struct                                      /*!< Pinmux Structure */
{
    __IO uint32_t
    CFG[27];                              /*!<                                            */
} PINMUX_TypeDef;

#define PINMUX_REG0_BASE                       0x40000500UL
#define PINMUX0                                ((PINMUX_TypeDef           *) PINMUX_REG0_BASE)

#define PAD_BIT_TABLE_OFFSET                   (12)
#define PAD_ITEM(reg_addr, bit_num)            ((bit_num << PAD_BIT_TABLE_OFFSET) | (reg_addr))
#define PAD_ITEM_ADDR(item)                    (item & 0xFFF)
#define PAD_ITEM_BIT_OFFSET(item)              ((item & 0xF000) >> PAD_BIT_TABLE_OFFSET)
#define PAD_ITEM_EMPTY                         (0)

/** @defgroup AON_FAST_REG_PAD_TYPE
  * @{
  */
typedef union _AON_FAST_REG_PAD_TYPE
{
    uint16_t d16;
    struct
    {
        uint16_t PAD_HS_MUX: 1;       /* 0 from aon domain , 1 from core domain */
        uint16_t PAD_HS_FUNC_SEL: 1;  /* 0 LCDC, 1 SDH*/
        uint16_t PAD_WKUP_INT_EN: 1;
        uint16_t PAD_DUMMY: 1;
        uint16_t PAD_S: 1;
        uint16_t PAD_SMT: 1;
        uint16_t PAD_E3: 1;
        uint16_t PAD_E2: 1;
        uint16_t PAD_SHDN: 1;
        uint16_t PAD_OUT_EN: 1;
        uint16_t PAD_WKPOL: 1;
        uint16_t PAD_WKEN: 1;
        uint16_t PAD_OUT_VALUE: 1;
        uint16_t PAD_PUPDC: 1;
        uint16_t PAD_PUPD: 1;
        uint16_t PAD_PU_EN: 1;
    };
} AON_FAST_REG_PAD_TYPE;

/** End of group AON_FAST_REG_PAD_TYPE
  * @}
  */

/** @defgroup AON_FAST_PAD_BIT_POS_TYPE
  * @{
  */
typedef enum _AON_FAST_PAD_POS_TYPE
{
    PAD_HS_MUX        = 0,
    PAD_HS_FUNC_SEL   = 1,
    PAD_WKUP_INT_EN   = 2,
    PAD_DUMMY         = 3,
    PAD_PINMUX_M_EN   = 4,
    PAD_SMT           = 5,
    PAD_E3            = 6,
    PAD_E2            = 7,
    PAD_SHDN          = 8,
    PAD_OUT_EN        = 9,
    PAD_WKPOL         = 10,
    PAD_WKEN          = 11,
    PAD_OUT_VALUE     = 12,
    PAD_PUPDC         = 13,
    PAD_PU            = 14,
    PAD_PU_EN         = 15,
} AON_FAST_PAD_BIT_POS_TYPE;
/** End of group AON_FAST_PAD_BIT_POS_TYPE
  * @}
  */

/** @defgroup PINMUX_Exported_Types PINMUX Exported Types
  * @{
  */

/** @defgroup PAD_Pull_Mode PAD Pull Mode
  * @{
  */

typedef enum _PAD_Pull_Mode
{
    PAD_PULL_DOWN,
    PAD_PULL_UP,
    PAD_PULL_NONE
} PAD_Pull_Mode;

typedef enum _PAD_Pull_Value
{
    PAD_PULL_HIGH,
    PAD_PULL_LOW,

} PAD_Pull_VALUE;

/** @defgroup PAD_Pull_EN PAD Pull enable
  * @{
  */

typedef enum _PAD_Pull_EN
{
    PAD_PULL_DISABLE,
    PAD_PULL_ENABLE
} PAD_Pull_EN;


/** End of group PAD_Pull_Mode
  * @}
  */

/** End of group PAD_Pull_Mode
  * @}
  */

/** @defgroup PAD_Mode PAD Mode
  * @{
  */

typedef enum _PAD_Mode
{
    PAD_SW_MODE,
    PAD_PINMUX_MODE
} PAD_Mode;

/** End of group PAD_Mode
  * @}
  */

/** @defgroup PAD_Power_Mode PAD Power Mode
  * @{
  */

typedef enum _PAD_PWR_Mode
{
    PAD_SHUTDOWN,
    PAD_IS_PWRON = 1
} PAD_PWR_Mode;

/** End of group PAD_Power_Mode
  * @}
  */

/** @defgroup PAD_Output_Config PAD Output Config
  * @{
  */

typedef enum _PAD_OUTPUT_ENABLE_Mode
{
    PAD_OUT_DISABLE,
    PAD_OUT_ENABLE
} PAD_OUTPUT_ENABLE_Mode;

/** End of group PAD_Output_Config
  * @}
  */

/** @defgroup PAD_Output_Value PAD Output Value
  * @{
  */

typedef enum _PAD_OUTPUT_VAL
{
    PAD_OUT_LOW,
    PAD_OUT_HIGH
} PAD_OUTPUT_VAL;

/** End of group PAD_Output_Value
  * @}
  */
/** @defgroup PAD_WakeUp_EN PAD WakeUp enable
  * @{
  */

typedef enum _PAD_WAKEUP_EN
{
    PAD_WAKEUP_DISABLE,
    PAD_WAKEUP_ENABLE
} PAD_WAKEUP_EN;

/** End of group PAD_WakeUp_Polarity_Value
  * @}
  */

typedef enum _SLEEP_LED_PIN
{
    SLEEP_LED_ADC_0,
    SLEEP_LED_ADC_1,
    SLEEP_LED_ADC_6,
    SLEEP_LED_ADC_7,
    SLEEP_LED_P1_0,
    SLEEP_LED_P1_1,
    SLEEP_LED_P1_4,
    SLEEP_LED_P2_0,
    SLEEP_LED_P2_1,
    SLEEP_LED_P2_2,
} SLEEP_LED_PIN;

/** @defgroup PAD_Function_Config PAD Function Config
  * @{
  */
#define PAD_AON_MUX_TYPE         (0x10)
typedef enum _PAD_FUNCTION_CONFIG_VALUE
{
    AON_GPIO = 0,
    CLK_REQ  = 4,
    XTAL_CLK = 7,
    LED0     = PAD_AON_MUX_TYPE | 0,
    LED1     = PAD_AON_MUX_TYPE | 2,
    LED2     = PAD_AON_MUX_TYPE | 4,
    LP_PWM   = PAD_AON_MUX_TYPE | 6,
} PAD_FUNCTION_CONFIG_VAL;

/** @defgroup PAD_WakeUp_Polarity_Value PAD WakeUp Polarity
  * @{
  */

typedef enum _PAD_WAKEUP_POL_VAL
{
    PAD_WAKEUP_POL_HIGH,
    PAD_WAKEUP_POL_LOW,
    PAD_WAKEUP_NONE
} PAD_WAKEUP_POL_VAL;

/** End of group PAD_WakeUp_Polarity_Value
  * @}
  */

/** @defgroup PAD_WakeUp_Debounce_En PAD WakeUp Debounce enable
  * @{
  */
typedef enum _PAD_WAKEUP_DEBOUNCE_EN
{
    PAD_WK_DEBOUNCE_DISABLE,
    PAD_WK_DEBOUNCE_ENABLE
} PAD_WAKEUP_DEBOUNCE_EN;

/**
  * @}
  */

/** @defgroup PAD_Pull_Value PAD Pull Value
  * @{
  */

typedef enum _PAD_PULL_CONFIG_VAL
{
    PAD_WEAKLY_PULL,
    PAD_STRONG_PULL
} PAD_PULL_VAL;
/**
  * @}
  */

/** @defgroup PAD_DRIVING_CURRENT PAD Driving Current Value
  * @{
  */

typedef enum _DRIVER_LEVEL
{
    MODE_4MA,
    MODE_8MA,
    MODE_12MA,
    MODE_16MA,
} T_DRIVER_LEVEL_MODE;
/** End of group PAD_DRIVING_CURRENT
  * @}
  */

/** @defgroup PAD_POWER_GROUP Pad Power Supply Volt
  * @{
  */

typedef enum _PIN_POWER_GROUP
{
    INVALID_PIN_GROUP,
    VDDIO1,
    VDDIO2,
    VDDIO3,
    VDDIO4,
} T_PIN_POWER_GROUP;

/** End of group PAD_POWER_GROUP
  * @}
  */

/** @defgroup PAD_LDO_Type PAD LDO Type
  * @{
  */

typedef enum _PAD_LDO_TYPE
{
    PAD_LDOAUX1,
    PAD_LDOAUX2
} PAD_LDO_TYPE;

/** End of group PAD_LDO_Type
  * @}
  */

/** @defgroup PAD_AON_STATUS PAD AON SETTINGS
  * @{
  */

typedef enum _PAD_AON_Status
{
    PAD_AON_OUTPUT_LOW,
    PAD_AON_OUTPUT_HIGH,
    PAD_AON_OUTPUT_DISABLE,
    PAD_AON_PINMUX_ON,
    PAD_AON_PIN_ERR
} PAD_AON_Status;
/** End of group _PAD_AON_Status
  * @}
  */

/** @defgroup WAKEUP_POLARITY PAD WAKE UP POLARITY
  * @{
  */

typedef enum _WAKEUP_POL
{
    POL_HIGH,
    POL_LOW,
} WAKEUP_POL;
/** End of group WAKEUP_POLARITY
  * @}
  */

/** @defgroup WAKEUP_ENABLE PAD WAKE UP ENABLE SET
  * @{
  */
typedef enum _WAKEUP_EN_MODE
{
    ADP_MODE,
    BAT_MODE,
    MFB_MODE,
    USB_MODE,
} WAKEUP_EN_MODE;

typedef enum _ANA_MODE
{
    PAD_ANALOG_MODE,
    PAD_DIGITAL_MODE,
} ANA_MODE;

typedef enum _PAD_HS_MUX_SEL_TYPE
{
    FROM_AON_DOMAIN,
    FROM_CORE_DOMAIN,
} PAD_HS_MUX_SEL_TYPE;

typedef enum _PAD_HS_FUNC_SEL_TYPE
{
    HS_Func0,
    HS_Func1,
} PAD_HS_FUNC_SEL_TYPE;


/** End of group PINMUX_Exported_Constants
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
  * @brief  According to the mode set to the pin , write the regster of AON which the pin coresponding .
  * @param  mode: mean one IO function, please refer to rtl876x_pinmux.h "Pin_Function_Number" part.
  *     @arg SHDN: use software mode.
  *     @arg PAD_OUT_EN: use pinmux mode.
        ......
        reference of bit of AON register mean in pinmux.h
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: value of the register bit ,0 or 1.
  * @retval None
  */

void Pad_TableConfig(AON_FAST_PAD_BIT_POS_TYPE pad_bit_set, uint8_t Pin_Num, uint8_t value);

#define Pad_WKTableConfig       Pad_TableConfig

/** @defgroup PINMUX_Exported_Functions PINMUX Exported Functions
  * @{
  */

/**
  * @brief  Reset all pin to default value.
  * @param  None.
  * @note: two SWD pins will also be reset. Please use this function carefully.
  * @retval None
  */
extern void Pinmux_Reset(void);

/**
  * @brief  Deinit the IO function of one pin.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval None
  */
extern void Pinmux_Deinit(uint8_t Pin_Num);

/**
  * @brief  Config pin to its corresponding IO function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  Pin_Func: mean one IO function, can be a value of @ref Pin_Function_Number.
  * @retval None
  */
extern void (*Pinmux_Config)(uint8_t Pin_Num, uint8_t Pin_Func);

/**
  * @brief  config the corresponding pad.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  AON_PAD_Mode: use software mode or pinmux mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_SW_MODE: use software mode.
  *     @arg PAD_PINMUX_MODE: use pinmux mode.
  * @param  AON_PAD_PwrOn: config power of pad.
  *     This parameter can be one of the following values:
  *     @arg PAD_NOT_PWRON: shutdown power of pad.
  *     @arg PAD_IS_PWRON: enable power of pad.
  * @param  AON_PAD_Pull: config pad pull mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_PULL_NONE: no pull.
  *     @arg PAD_PULL_UP: pull this pin up.
  *     @arg PAD_PULL_DOWN: pull thi pin down.
  * @param  AON_PAD_E: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_DISABLE: disable pin output.
  *     @arg PAD_OUT_ENABLE: enable pad output.
  * @param  AON_PAD_O: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_LOW: pad output low.
  *     @arg PAD_OUT_HIGH: pad output high.
  * @retval None
  */
extern void (*Pad_Config)(uint8_t Pin_Num,
                          PAD_Mode AON_PAD_Mode,
                          PAD_PWR_Mode AON_PAD_PwrOn,
                          PAD_Pull_Mode AON_PAD_Pull,
                          PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                          PAD_OUTPUT_VAL AON_PAD_O);

/**
  * @brief  config the corresponding pad.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  AON_PAD_Mode: use software mode or pinmux mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_SW_MODE: use software mode.
  *     @arg PAD_PINMUX_MODE: use pinmux mode.
  * @param  AON_PAD_PwrOn: config power of pad.
  *     This parameter can be one of the following values:
  *     @arg PAD_NOT_PWRON: shutdown power of pad.
  *     @arg PAD_IS_PWRON: enable power of pad.
  * @param  AON_PAD_Pull: config pad pull mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_PULL_NONE: no pull.
  *     @arg PAD_PULL_UP: pull this pin up.
  *     @arg PAD_PULL_DOWN: pull thi pin down.
  * @param  AON_PAD_E: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_DISABLE: disable pin output.
  *     @arg PAD_OUT_ENABLE: enable pad output.
  * @param  AON_PAD_O: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_LOW: pad output low.
  *     @arg PAD_OUT_HIGH: pad output high.
  * @param  AON_PAD_P: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_150K_PULL: pad pull 150k resistance.
  *     @arg PAD_15K_PULL: pad pull 15k resistance.
  * @retval None
  */
extern void (*Pad_ConfigExt)(uint8_t Pin_Num,
                             PAD_Mode AON_PAD_Mode,
                             PAD_PWR_Mode AON_PAD_PwrOn,
                             PAD_Pull_Mode AON_PAD_Pull,
                             PAD_OUTPUT_ENABLE_Mode AON_PAD_E,
                             PAD_OUTPUT_VAL AON_PAD_O,
                             PAD_PULL_VAL AON_PAD_P);

/**
  * @brief  Set all pins to the default state.
  * @param  None.
  * @retval None.
  */
extern void (*Pad_AllConfigDefault)(void);

/**
  * @brief  Enable pin wakeup function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  Polarity: PAD_WAKEUP_POL_HIGH--use high level wakeup, PAD_WAKEUP_POL_LOW-- use low level wakeup.
  * @retval None
  */
extern void (*System_WakeUpPinEnable)(uint8_t Pin_Num, uint8_t Polarity, uint8_t DebounceEn);

/**
  * @brief  Disable pin wakeup function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval None
  */
extern void (*System_WakeUpPinDisable)(uint8_t Pin_Num);

/**
  * @brief  Configure the adpater wake up functions.
  * @param  pol: polarity to wake up
  *            This parameter WAKEUP_POL POL_HIGH means high level POL_LOW means low level to wakeup.
  * @param  NewState: Enable or disable to wake up
  *            This parameter value is ENABLE or DISABLE.
  * @retval None
  */
extern void System_SetAdpWakeUpFunction(FunctionalState NewState, WAKEUP_POL pol);

/**
  * @brief  Configure the MFB wake up functions.
  * @param  NewState: Enable or disable to wake up
  *            This parameter value is ENABLE or DISABLE.
  * @retval None
  */
extern void System_SetMFBWakeUpFunction(FunctionalState NewState);

/**
  * @brief  Disable wake up pin interrupt function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval None
  */
extern void (*System_WakeUpInterruptDisable)(uint8_t Pin_Num);

/**
  * @brief  Disable wake up pin interrupt function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval None
  */
extern void (*System_WakeUpInterruptEnable)(uint8_t Pin_Num);
/**
  * @brief  Check wake up pin interrupt status.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval Pin interrupt status
  */
extern uint8_t (*System_WakeUpInterruptValue)(uint8_t Pin_Num);

/**
  * @brief  config the pad output function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_DISABLE: disable pin output.
  *     @arg PAD_OUT_ENABLE: enable pad output.
  * @retval None
  */
#define Pad_OutputEnableValue(Pin_Num, value) Pad_TableConfig(PAD_OUT_EN, Pin_Num, value)

/**
  * @brief  config pad pull enable or not.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg DISABLE: disable pin pull.
  *     @arg ENABLE: enable pin pull.
  * @retval None
  */
#define Pad_PullEnableValue(Pin_Num, value) Pad_TableConfig(PAD_PU_EN, Pin_Num, value)

/**
  * @brief  config pad pull mode.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pad out put function.
  *     This parameter can be one of the following values:
  *     @arg PAD_PULL_DOWN: pad pull down.
  *     @arg PAD_PULL_UP: pad pull up.
  *     @arg PAD_PULL_NONE: pad_pull none
  * @retval None
  */
extern void Pad_SetPullMode(uint8_t Pin_Num, PAD_Pull_Mode pull_mode);
#define Pad_PullEnableValue_Dir(Pin_Num, value, pull_direction)        Pad_PullConfig(Pin_Num, pull_mode)

/**
  * @brief  config pad pull up or down or none.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value : config pad PAD_Pull_Mode.
  *     This parameter can be one of the PAD_Pull_Mode type values:
  * @retval None
  */
#define Pad_PullUpOrDownValue(Pin_Num, value) Pad_TableConfig(PAD_PUPD_DIR, Pin_Num, value)

/**
  * @brief  config the pad control selected value.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: use software mode or pinmux mode.
  *     This parameter can be one of the following values:
  *     @arg PAD_SW_MODE: use software mode, aon control.
  *     @arg PAD_PINMUX_MODE: use pinmux mode, core control.
  * @retval None
  */
#define Pad_ControlSelectValue(Pin_Num, value) Pad_TableConfig(PAD_PINMUX_M_EN, Pin_Num, value)

/**
  * @brief  config the pad output value.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_LOW: pad output low.
  *     @arg PAD_OUT_HIGH: pad output high.
  * @retval None
  */
#define Pad_OutputControlValue(Pin_Num, value) Pad_TableConfig(PAD_OUT_VALUE, Pin_Num, value)


/**
  * @brief  config pad wake up function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg ENABLE: Enable pad to wake up system from DLPS.
  *     @arg DISABLE: Disable pad to wake up from DLPS.
  * @retval None
  */
#define Pad_WakeupEnableValue(Pin_Num, value) Pad_WKTableConfig(PAD_WKEN, Pin_Num, value)


/**
  * @brief  config the pad wake up polarity.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_WAKEUP_POL_HIGH: Positive level to wake up.
  *     @arg PAD_WAKEUP_POL_LOW: Negative level to wake up.
  * @retval None
  */
#define Pad_WakeupPolarityValue(Pin_Num, value) Pad_WKTableConfig(PAD_WKPOL, Pin_Num, value)

/**
  * @brief  config pad wake up interrupt.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg ENABLE: Enable pad wake up to trigger System interrupt .
  *     @arg DISABLE: Disable pad wake up to trigger System interrupt.
  * @retval None
  */
extern void Pad_WakeupInterruptEnable(uint8_t Pin_Num, uint8_t value);

/**
  * @brief  Check wake up pin interrupt status.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval Pin interrupt status
  */
extern FlagStatus Pad_WakeupInterruptValue(uint8_t Pin_Num);

/**
  * @brief  Clear pad wake up pin interrupt pending bit.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @retval None
  */
extern void Pad_ClearWakeupINTPendingBit(uint8_t Pin_Num);

/**
  * @brief  Clear all wake up pin interrupt pending bit.
  * @param  None
  * @retval None
  */
extern void Pad_ClearAllWakeupINT(void);

/**
  * @brief  config the pad output value.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_OUT_LOW: pad output low.
  *     @arg PAD_OUT_HIGH: pad output high.
  * @retval None
  */
#define Pad_PowerOrShutDownValue(Pin_Num, value) Pad_TableConfig(PAD_SHDN, Pin_Num, value)

/**
  * @brief  config the pad pull value.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
  * @param  value: config pin output level.
  *     This parameter can be one of the following values:
  *     @arg PAD_150K_PULL: pad pull 150k resistance.
  *     @arg PAD_15K_PULL: pad pull 15k resistance.
  * @retval None
  */
#define Pad_PullConfigValue(Pin_Num, value) Pad_TableConfig(PAD_PUPDC, Pin_Num, value)

/**
  * @brief  Config driving current value.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  e2_value: this parameter can be LEVEL_E2_0 and LEVEL_E2_1.
  * @param  e3_value: this parameter can be LEVEL_E3_0 and LEVEL_E3_1.
  * @retval none
  */
extern void Pad_DrivingCurrentControl(uint8_t Pin_Num, uint8_t e2_value, uint8_t e3_value);

/**
  * @brief  Config Pad Function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to Max pin number, please refer to pin_def.h "Pin_Number" part.
  * @param  PAD_FUNCTION_CONFIG_VAL: value.
  *   This parameter can be: AON_GPIO, LED0, LED1, LED2, CLK_REQ.
  * @retval None
  */
extern void Pad_FunctionConfig(uint8_t Pin_Num, PAD_FUNCTION_CONFIG_VAL value);

/**
  * @brief  Config Pad Function.
  * @param  Pin_Num: pin number.
  *     This parameter is from ADC_0 to P4_1, please refer to pin_def.h "Pin_Number" part.
  * @param  value: config value @ref PAD_FUNCTION_CONFIG_VAL
  *   This parameter can be: AON_GPIO, LED0, LED1, LED2, CLK_REQ.
  * @retval None
  */
extern void Pad_FunctionConfig(uint8_t Pin_Num, PAD_FUNCTION_CONFIG_VAL value);


/**
  * @brief  Get the Pad AON output value .
  * @param  Pin_Num:  pin number
  *            This parameter is from ADC_0 to Max pin number, please refer to pin_def.h "Pin_Number" part.
  * @retval reference PAD_AON_Status.
  */
uint8_t Pad_GetOutputCtrl(uint8_t Pin_Num);

/**
  * @brief  set the system wakeup mode  .
  * @param  mode: mode of set .
  *            This parameter reference WAKEUP_EN_MODE .
  *        pol: polarity to wake up
  *            This parameter WAKEUP_POL POL_HIGH means high level POL_LOW means low level to wakeup.
  *        NewState: Enable or disable to wake up
  *            This parameter value is ENABLE or DISABLE.
  * @retval  1 means wrong mode.
  */
extern uint8_t (*Pad_WakeUpCmd)(WAKEUP_EN_MODE mode, WAKEUP_POL pol, FunctionalState NewState);

/**
  * @brief  Config Hybrid pad analog/digital functions.
  * @param  Pin_Num: pin number.
  *     This parameter is from AUX_R to MIC3_N, please refer to pin_def.h "Pin_Number" part.
  * @param  PAD_FUNCTION_CONFIG_VAL: value.
  *   This parameter can be: PAD_ANALOG_MODE/PAD_DIGITAL_MODE.
  * @retval None
  */
void Pad_AnalogMode(uint8_t pin, ANA_MODE mode);

/**
  * @brief  Config PAD highs speed functions.
  * @param  Pin_Num: pin number.
  *    This parameter is from ADC_0 to Max pin number, please refer to pin_def.h "Pin_Number" part.
  * @param  PAD_HS_MUX_SEL_TYPE: value.
  *   This parameter can be: FROM_AON_MUX/FROM_HS_MUX.
  * @retval true: Set high speed mux success
  * @retval false: Set high speed mux failed due to invalid pinmux
  */
bool Pad_HighSpeedMuxSel(uint8_t Pin_Num, PAD_HS_MUX_SEL_TYPE Pad_Hs_Mux);

/**
  * @brief  PAD high speed mode function select.
  * @param  Pin_Num: pin number.
  *    This parameter is from ADC_0 to Max pin number, please refer to pin_def.h "Pin_Number" part.
  * @param  PAD_HS_FUNC_SEL_TYPE: Pad_HS_Func_Sel.
  *   This parameter can be: HS_Func0/HS_Func1.
  *   P4_2~P4_5: 0 Display Controller, 1 SPI0;
  *   P6_0~P6_5: 0 Display Controller, 1 SDIO0;
  *   P8_0~P8_3: 0 Display Controller, 1 SPI1;
  *   P9_0~P9_5: 0 SPIC1, 1 OPI PSRAM.
  * @retval true: Set high speed mux success
  * @retval false: Set high speed mux failed due to invalid pinmux
  */
bool Pad_HighSpeedFuncSel(uint8_t Pin_Num, PAD_HS_FUNC_SEL_TYPE Pad_HS_Func_Sel);

/**
  * @brief  Set hybrid pad digital mode or analog mode.
  * @param  Pin_Num: pin number.
  *    This parameter is from ADC_0 to Max pin number, please refer to pin_def.h "Pin_Number" part.
  * @param  mode: ANA_MODE.
  *   This parameter can be: PAD_ANALOG_MODE/PAD_DIGITAL_MODE.
  * @retval none
  */
void Pad_AnalogMode(uint8_t Pin_Num, ANA_MODE mode);

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_PINMUX_H_ */

/** @} */ /* End of group PINMUX_Exported_Functions */
/** @} */ /* End of group PINMUX */


/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/
