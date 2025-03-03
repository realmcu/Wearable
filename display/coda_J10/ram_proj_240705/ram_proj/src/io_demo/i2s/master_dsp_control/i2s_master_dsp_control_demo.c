/**
*********************************************************************************************************
*               Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     i2s_master_dsp_control_demo.c
* @brief    This file provides demo code of I2S master in DSP control mode.
* @details
* @author   mh_chang
* @date     2022-10-25
* @version  v1.0
*********************************************************************************************************
*/

#include "rtl876x_i2s.h"
#include "rtl876x_pinmux.h"

#define I2S_LRCK_PIN                    P0_0
#define I2S_BCLK_PIN                    P0_1
#define I2S_SDO_PIN                     P0_2
#define I2S_SDI_PIN                     P0_3

#define I2S_NUM                         I2S3
#define I2S_LRCK_PINMUX                 LRC_SPORT3
#define I2S_BCLK_PINMUX                 BCLK_SPORT3
#define I2S_SDO_PINMUX                  DACDAT_SPORT3
#define I2S_SDI_PINMUX                  ADCDAT_SPORT3

/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  No parameter.
  * @return Void
  */
void board_i2s_init(void)
{
    /* set PAD_SW_MODE & PAD_PULL_DOWN when I2S disable to prevent PAD floating */
    Pad_Config(I2S_BCLK_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(I2S_LRCK_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(I2S_SDO_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(I2S_SDI_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);

    Pinmux_Config(I2S_BCLK_PIN, I2S_BCLK_PINMUX);
    Pinmux_Config(I2S_LRCK_PIN, I2S_LRCK_PINMUX);
    Pinmux_Config(I2S_SDO_PIN, I2S_SDO_PINMUX);
    Pinmux_Config(I2S_SDI_PIN, I2S_SDI_PINMUX);
}

/**
  * @brief  Initialize I2S peripheral.
  * @param  No parameter.
  * @return Void
  */
void driver_i2s_init(void)
{
    I2S_InitTypeDef I2S_InitStruct;

    I2S_StructInit(&I2S_InitStruct);
    I2S_InitStruct.I2S_ClockSource      = I2S_CLK_XTAL;
    /* BCLK = 40MHz * (I2S_BClockNi / I2S_BClockMi), LRCK = BCLK / (I2S_BClockDiv + 1) */
    I2S_InitStruct.I2S_BClockMi         = 0x271;
    I2S_InitStruct.I2S_BClockNi         = 0x30;     /* BCLK = 3.072MHz */
    I2S_InitStruct.I2S_BClockDiv        = 0x3F;     /* LRCK = 48KHz */
    I2S_InitStruct.I2S_DeviceMode       = I2S_DeviceMode_Master;
    I2S_InitStruct.I2S_TxChannelType    = I2S_Channel_Stereo;
    I2S_InitStruct.I2S_RxChannelType    = I2S_Channel_Stereo;
    I2S_InitStruct.I2S_TxDataWidth      = I2S_Width_24Bits;
    I2S_InitStruct.I2S_RxDataWidth      = I2S_Width_24Bits;
    I2S_InitStruct.I2S_TxChannelWidth   = I2S_Width_32Bits;
    I2S_InitStruct.I2S_RxChannelWidth   = I2S_Width_32Bits;
    I2S_InitStruct.I2S_TxDataFormat     = I2S_Mode;
    I2S_InitStruct.I2S_RxDataFormat     = I2S_Mode;
    I2S_InitStruct.I2S_DMACmd           = I2S_DMA_ENABLE;
    I2S_InitStruct.I2S_ControlMode      = I2S_ControlMode_DSP;
    I2S_Init(I2S_NUM, &I2S_InitStruct);

    /* there's only need to config I2S parameter, and it should be enabled by DSP */
}

/**
  * @brief  Demo code of I2S master in DSP control mode.
  * @param  No parameter.
  * @return Void
*/
void i2s_demo(void)
{
    board_i2s_init();
    driver_i2s_init();
}
/******************* (C) COPYRIGHT 2022 Realtek Semiconductor Corporation *****END OF FILE****/

