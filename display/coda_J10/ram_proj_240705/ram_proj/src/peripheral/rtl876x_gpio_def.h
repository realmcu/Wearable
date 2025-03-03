/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_gpio_def.h
* \brief    GPIO Registers Structures Section
* \details
* \author   colin
* \date     2024-06-17
* \version  v1.0
* *********************************************************************************************************
*/

#ifndef RTL_GPIO_DEF_H
#define RTL_GPIO_DEF_H

#include <stdint.h>
#include <stdbool.h>
#include "rtl876x.h"

#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */

/*============================================================================*
 *                         GPIO Features
 *============================================================================*/
#define GPIO_SUPPORT_SET_CONTROL_MODE                  (1)
#define GPIO_SUPPORT_SWAP_DEB_PINBIT                   (0)
#define GPIO_SUPPORT_LS_SYNC                           (1)
#define GPIO_SUPPORT_INT_BOTHEDGE                      (0)

typedef struct
{
    __IO uint32_t GPIO_DR;                       /*!< 0X00 */
    __IO uint32_t GPIO_DDR;                      /*!< 0X04 */
    __IO uint32_t GPIO_SRC;                      /*!< 0X08 */
    __IO uint32_t GPIO_INT_EN;                   /*!< 0X30 */
    __IO uint32_t GPIO_INT_MASK;                 /*!< 0X34 */
    __IO uint32_t GPIO_INT_LV;                   /*!< 0X38 */
    __IO uint32_t GPIO_INT_POL;                  /*!< 0X3C */
    __IO uint32_t GPIO_INT_STS;                  /*!< 0X40 */
    __IO uint32_t GPIO_INT_RAW_STS;              /*!< 0X44 */
    __IO uint32_t GPIO_IN_DEB_EN;                /*!< 0X48 */
    __IO uint32_t GPIO_INT_CLR;                  /*!< 0X4C */
    __IO uint32_t GPIO_EXT;                      /*!< 0X50 */
    __IO uint32_t GPIO_LS_SYNC;                  /*!< 0X60 */
    __IO uint32_t GPIO_ID_CODE;                  /*!< 0X64 */
    __IO uint32_t GPIO_INT_BOTHEDGE;             /*!< 0X68 */
    __IO uint32_t GPIO_OUT_MODE;                 /*!< 0X100 */
    __IO uint32_t GPIO_PAD_STATE;                /*!< 0X104 */
    __IO uint32_t GPIO_EXT_DEB_CLK_CTL[16];      /*!< 0x108 ~ 0x144 */
    __IO uint32_t GPIO_EXT_DEB_FUNC_CTL;         /*!< 0X148 */
    __IO uint32_t GPIO_EXT_DEB_POL_CTL;          /*!< 0X14C */
    __IO uint32_t GPIO_VERSION;                  /*!< 0X150 */
} GPIO_TypeDef;


/*============================================================================*
 *                         GPIO Declaration
 *============================================================================*/
/** \defgroup 87X2G_GPIO      GPIO
  * \brief
  * \{
  */

/** \defgroup GPIO_Declaration GPIO Declaration
  * \brief
  * \{
  */

#define GPIOA              ((GPIO_TypeDef          *) GPIO0_BASE)
#define GPIOB              ((GPIO_TypeDef          *) GPIO1_BASE)

/** End of GPIO_Declaration
  * \}
  */

/* 0x00
   31:0    R/W    GPIO_SWPORTA_DR         32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t GPIO_SWPORTA_DR: 32;
    } b;
} GPIO_DR_TypeDef;



/* 0x04
   31:0    R/W    GPIO_SWPORTA_DDR        32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t GPIO_SWPORTA_DDR: 32;
    } b;
} GPIO_DDR_TypeDef;



/* 0x08
   31:0    R/W    GPIO_SWPORTA_CTL        32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t GPIO_SWPORTA_CTL: 32;
    } b;
} GPIO_SRC_TypeDef;

/* 0x30
   31:0    R/W    GPIO_INTEN              32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t GPIO_INTEN: 32;
    } b;
} GPIO_INT_EN_TypeDef;



/* 0x34
   31:0    R/W    GPIO_INTMASK            32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t GPIO_INTMASK: 32;
    } b;
} GPIO_INT_MASK_TypeDef;



/* 0x38
   31:0    R/W    GPIO_INTTYPE_LEVEL      32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t GPIO_INTTYPE_LEVEL: 32;
    } b;
} GPIO_INT_LV_TypeDef;



/* 0x3C
   31:0    R/W    RESERVED                32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t RESERVED_0: 32;
    } b;
} GPIO_INT_POL_TypeDef;

/* 0x40
   31:0    R      GPIO_INTSTATUS          32'h0
*/
typedef union
{
    uint32_t d32;
    struct
    {
        const uint32_t GPIO_INTSTATUS: 32;
        } b;
    } GPIO_INT_STS_TypeDef;



    /* 0x44
       31:0    R      GPIO_RAW_INTSTATUS      32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                const uint32_t GPIO_RAW_INTSTATUS: 32;
            } b;
        } GPIO_INT_RAW_STS_TypeDef;



    /* 0x48
       31:0    R/W    RESERVED                32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                const uint32_t RESERVED_0: 32;
            } b;
        } GPIO_IN_DEB_EN_TypeDef;


    /* 0x4C
       31:0    W1C    GPIO_PORTA_EOI          32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t GPIO_PORTA_EOI: 32;
            } b;
        } GPIO_INT_CLR_TypeDef;



    /* 0x50
       31:0    R/W    RESERVED                32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                const uint32_t RESERVED_0: 32;
            } b;
        } GPIO_EXT_TypeDef;

    /* 0x60
       0       R/W    GPIO_LS_SYNC            1'h0
       31:1    R/W    RESERVED                31'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t GPIO_LS_SYNC: 1;
                uint32_t RESERVED_0: 31;
            } b;
        } GPIO_LS_SYNC_TypeDef;



    /* 0x64
       31:0    R/W    RESERVED                32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                const uint32_t RESERVED_0: 32;
            } b;
        } GPIO_ID_CODE_TypeDef;


    /* 0x68
       31:0    R/W    RESERVED                32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t RESERVED_0: 32;
            } b;
        } GPIO_INT_BOTHEDGE_TypeDef;

    /* 0x100
       31:0    R/W    GPIO_OUTPUT_MODE        32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t GPIO_OUTPUT_MODE: 32;
            } b;
        } GPIO_OUT_MODE_TypeDef;



    /* 0x104
       31:0    R      GPIO_PAD_STATE          32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                const uint32_t GPIO_PAD_STATE: 32;
            } b;
        } GPIO_PAD_STATE_TypeDef;


    /* 0x108 ~ 0x144
       7:0     R/W    GPIO_G0_CNT_LIMIT      8'h1
       8       R/W    DUMMY                  1'h0
       11:9    R/W    GPIO_G0_DEB_CLK_DIV    3'h0
       15:12   R/W    DUMMY                  4'h0
       23:16   R/W    GPIO_G1_CNT_LIMIT      8'h1
       24      R/W    DUMMY                  1'h0
       27:25   R/W    GPIO_G1_DEB_CLK_DIV    3'h0
       31:28   R/W    DUMMY                  4'h0
    */

    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t GPIO_G_L_CNT_LIMIT: 8;
                uint32_t reserved_3: 1;
                uint32_t GPIO_G_L_DEB_CLK_DIV: 3;
                uint32_t reserved_2: 4;
                uint32_t GPIO_G_H_CNT_LIMIT: 8;
                uint32_t reserved_1: 1;
                uint32_t GPIO_G_H_DEB_CLK_DIV: 3;
                uint32_t reserved_0: 4;
            } b;
        } GPIO_EXT_DEB_CLK_CTL_TypeDef;




    /* 0x148
       31:0    R/W    GPIO_X_DEB_FUNC_EN      32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t GPIO_X_DEB_FUNC_EN: 32;
            } b;
        } GPIO_EXT_DEB_FUNC_CTL_TypeDef;



    /* 0x14C
       31:0    R/W    GPIO_POLARITY           32'h0
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                uint32_t GPIO_POLARITY: 32;
            } b;
        } GPIO_EXT_DEB_POL_CTL_TypeDef;



    /* 0x150
       31:0    R      GPIO_VERSION            32'h20221219
    */
    typedef union
        {
            uint32_t d32;
            struct
            {
                const uint32_t GPIO_VERSION: 32;
            } b;
        } GPIO_VERSION_TypeDef;


    /* ================================================================================ */
    /* ================                  TYPE/API  WRAPPER             ================ */
    /* ================================================================================ */

    /**
     * \brief    GPIO IRQn Define
     *
     * \ingroup  GPIO_Exported_Constants
     */
#ifdef GPIOA
#define GPIOA0_IRQn         SUB_IRQ_DEF(GPIO_A0_IRQn,      0)
#define GPIOA1_IRQn         SUB_IRQ_DEF(GPIO_A1_IRQn,      1)
#define GPIOA2_IRQn         SUB_IRQ_DEF(GPIO_A2_7_IRQn,    2)
#define GPIOA3_IRQn         SUB_IRQ_DEF(GPIO_A2_7_IRQn,    3)
#define GPIOA4_IRQn         SUB_IRQ_DEF(GPIO_A2_7_IRQn,    4)
#define GPIOA5_IRQn         SUB_IRQ_DEF(GPIO_A2_7_IRQn,    5)
#define GPIOA6_IRQn         SUB_IRQ_DEF(GPIO_A2_7_IRQn,    6)
#define GPIOA7_IRQn         SUB_IRQ_DEF(GPIO_A2_7_IRQn,    7)
#define GPIOA8_IRQn         SUB_IRQ_DEF(GPIO_A8_15_IRQn,   8)
#define GPIOA9_IRQn         SUB_IRQ_DEF(GPIO_A8_15_IRQn,   9)
#define GPIOA10_IRQn        SUB_IRQ_DEF(GPIO_A8_15_IRQn,   10)
#define GPIOA11_IRQn        SUB_IRQ_DEF(GPIO_A8_15_IRQn,   11)
#define GPIOA12_IRQn        SUB_IRQ_DEF(GPIO_A8_15_IRQn,   12)
#define GPIOA13_IRQn        SUB_IRQ_DEF(GPIO_A8_15_IRQn,   13)
#define GPIOA14_IRQn        SUB_IRQ_DEF(GPIO_A8_15_IRQn,   14)
#define GPIOA15_IRQn        SUB_IRQ_DEF(GPIO_A8_15_IRQn,   15)
#define GPIOA16_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  16)
#define GPIOA17_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  17)
#define GPIOA18_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  18)
#define GPIOA19_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  19)
#define GPIOA20_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  20)
#define GPIOA21_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  21)
#define GPIOA22_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  22)
#define GPIOA23_IRQn        SUB_IRQ_DEF(GPIO_A16_23_IRQn,  23)
#define GPIOA24_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  24)
#define GPIOA25_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  25)
#define GPIOA26_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  26)
#define GPIOA27_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  27)
#define GPIOA28_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  28)
#define GPIOA29_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  29)
#define GPIOA30_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  30)
#define GPIOA31_IRQn        SUB_IRQ_DEF(GPIO_A24_31_IRQn,  31)
#endif
#ifdef GPIOB
#define GPIOB0_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    32)
#define GPIOB1_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    33)
#define GPIOB2_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    34)
#define GPIOB3_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    35)
#define GPIOB4_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    36)
#define GPIOB5_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    37)
#define GPIOB6_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    38)
#define GPIOB7_IRQn         SUB_IRQ_DEF(GPIO_B0_7_IRQn,    39)
#define GPIOB8_IRQn         SUB_IRQ_DEF(GPIO_B8_15_IRQn,   40)
#define GPIOB9_IRQn         SUB_IRQ_DEF(GPIO_B8_15_IRQn,   41)
#define GPIOB10_IRQn        SUB_IRQ_DEF(GPIO_B8_15_IRQn,   42)
#define GPIOB11_IRQn        SUB_IRQ_DEF(GPIO_B8_15_IRQn,   43)
#define GPIOB12_IRQn        SUB_IRQ_DEF(GPIO_B8_15_IRQn,   44)
#define GPIOB13_IRQn        SUB_IRQ_DEF(GPIO_B8_15_IRQn,   45)
#define GPIOB14_IRQn        SUB_IRQ_DEF(GPIO_B8_15_IRQn,   46)
#define GPIOB15_IRQn        SUB_IRQ_DEF(GPIO_B8_15_IRQn,   47)
#define GPIOB16_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  48)
#define GPIOB17_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  49)
#define GPIOB18_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  50)
#define GPIOB19_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  51)
#define GPIOB20_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  52)
#define GPIOB21_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  53)
#define GPIOB22_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  54)
#define GPIOB23_IRQn        SUB_IRQ_DEF(GPIO_B16_23_IRQn,  55)
#define GPIOB24_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  56)
#define GPIOB25_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  57)
#define GPIOB26_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  58)
#define GPIOB27_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  59)
#define GPIOB28_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  60)
#define GPIOB29_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  61)
#define GPIOB30_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  62)
#define GPIOB31_IRQn        SUB_IRQ_DEF(GPIO_B24_31_IRQn,  63)
#endif


    /**
     * \brief    GPIO Macro Wrapper
     *
     * \ingroup  GPIO_Exported_Constants
     */
#define GPIOIT_LevelType        GPIOITTrigger_TypeDef
#define GPIOIT_PolarityType     GPIOITPolarity_TypeDef
#define GPIOIT_DebounceType     GPIODebounce_TypeDef
#define GPIO_PinBit             GPIO_Pin
#define GPIO_Mode               GPIO_Dir
#define GPIO_Mode_IN            GPIO_DIR_IN
#define GPIO_Mode_OUT           GPIO_DIR_OUT
#define GPIOMode_TypeDef        GPIODir_TypeDef
#define GPIO_INT_Trigger_LEVEL  GPIO_INT_TRIGGER_LEVEL
#define GPIO_INT_Trigger_EDGE   GPIO_INT_TRIGGER_EDGE
#define GPIO_DebounceTime       GPIO_DebounceCntLimit

    /**
     * \brief    GPIO API Wrapper
     *
     * \ingroup  GPIO_Exported_Constants
     */
#define GPIOx_DeInit                GPIO_DeInit
//#define GPIOx_Init                  GPIO_Init
#define GPIOx_INTConfig             GPIO_INTConfig
#define GPIOx_ClearINTPendingBit    GPIO_ClearINTPendingBit
#define GPIOx_MaskINTConfig         GPIO_MaskINTConfig
#define GPIOx_ReadInputDataBit      GPIO_ReadInputDataBit
#define GPIOx_ReadInputData         GPIO_ReadInputData
#define GPIOx_ReadOutputDataBit     GPIO_ReadOutputDataBit
#define GPIOx_ReadOutputData        GPIO_ReadOutputData
#define GPIOx_SetBits               GPIO_SetBits
#define GPIOx_ResetBits             GPIO_ResetBits
#define GPIOx_WriteBit              GPIO_WriteBit
#define GPIOx_Write                 GPIO_Write
#define GPIOx_GetINTStatus          GPIO_GetINTStatus
#define GPIOx_ModeSet               GPIO_SetDirection
#define GPIOx_ChangePolarity        GPIO_SetPolarity
#define GPIOx_IntPolaritySet        GPIO_SetPolarity
#define GPIO_GetPin                 GPIO_GetPinBit

#ifdef  __cplusplus
}
#endif /* __cplusplus */
#endif /* RTL_GPIO_DEF_H */
