/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* \file     rtl_can.c
* \brief    This file provides all the CAN firmware functions.
* \details
* \author   colin
* \date     2024-06-26
* \version  v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x_rcc.h"
#include "rtl_can.h"

/*============================================================================*
 *                        Private Functions
 *============================================================================*/
#if (CAN_SUPPORT_SLEEP_MODE == 1)
extern void CAN_LowPowerClkDIV(CAN_TypeDef *CANx, CANLowPowerClkDIV_TypeDef div);
#endif

/*============================================================================*
 *                           Public Functions
 *============================================================================*/
/**
  * \brief  Deinitializes the CAN peripheral registers to their default values.
  * \param  None
  * \return None
  */
void CAN_DeInit(CAN_TypeDef *CANx)
{
    if (CANx ==  CAN0)
    {
        RCC_PeriphClockCmd(APBPeriph_CAN0, APBPeriph_CAN0_CLOCK, DISABLE);
    }
    else if (CANx ==  CAN1)
    {
        RCC_PeriphClockCmd(APBPeriph_CAN1, APBPeriph_CAN1_CLOCK, DISABLE);
    }
}

/**
  * \brief  Initializes the CAN peripheral according to the specified
  *         parameters in the CAN_InitStruct
  * \param  CAN_InitStruct: pointer to a CAN_InitTypeDef structure that
  *         contains the configuration information for the specified CAN peripheral
  * \return None
  */
void CAN_Init(CAN_TypeDef *CANx, CAN_InitTypeDef *CAN_InitStruct)
{
    CAN_0x00_TYPE_TypeDef can_reg_0x00 = {.d32 = CANx->CAN_CTL};
    CAN_0x0C_TYPE_TypeDef can_reg_0x0C = {.d32 = CANx->CAN_BIT_TIMING};
    CAN_0x10_TYPE_TypeDef can_reg_0x10 = {.d32 = CANx->CAN_FD_BIT_TIMING};
    CAN_0x14_TYPE_TypeDef can_reg_0x14 = {.d32 = CANx->CAN_FD_SSP_CAL};
    CAN_0x2C_TYPE_TypeDef can_reg_0x2C = {.d32 = CANx->CAN_ERR_CNT_CTL};
    CAN_0x40_TYPE_TypeDef can_reg_0x40 = {.d32 = CANx->CAN_TIME_STAMP};
    CAN_0x54_TYPE_TypeDef can_reg_0x54 = {.d32 = CANx->CAN_TEST};
    CAN_MB_BA_END_TypeDef can_reg_0x2f0 = {.d32 = CANx->CAN_MB_BA_END};
    CAN_0x200_0x23C_TYPE_TypeDef can_reg_0x200_0x23c = {.d32 = 0};

    /* common configuration */
    can_reg_0x00.b.can_tri_sample = CAN_InitStruct->CAN_TriSampleEn;
    can_reg_0x00.b.auto_re_tx_en = CAN_InitStruct->CAN_AutoReTxEn;
    can_reg_0x00.b.rx_fifo_en = CAN_InitStruct->CAN_RxFifoEn;
    can_reg_0x00.b.can_rxdma_en = CAN_InitStruct->CAN_RxDmaEn;

    can_reg_0x40.b.can_time_stamp_en = CAN_InitStruct->CAN_TimeStamp.b.can_time_stamp_en;
    can_reg_0x40.b.can_time_stamp_div = CAN_InitStruct->CAN_TimeStamp.b.can_time_stamp_div;

    can_reg_0x2C.b.can_error_warn_th = CAN_InitStruct->CAN_ErrorWarnThd;

    /* CAN 2.0 A/B configuration */
    can_reg_0x0C.d32 = CAN_InitStruct->CAN_BitTiming.d32;

    /* CAN FD configuration */
    can_reg_0x00.b.can_fd_en = CAN_InitStruct->CAN_FdEn;
    can_reg_0x10.d32 = CAN_InitStruct->CAN_FdBitTiming.d32;
    can_reg_0x14.d32 = CAN_InitStruct->CAN_FdSspCal.d32;
    can_reg_0x14.b.can_fd_ssp_dco =
        (CAN_InitStruct->CAN_FdBitTiming.b.can_fd_tseg1 + 2) *
        (CAN_InitStruct->CAN_FdBitTiming.b.can_fd_brp + 1);
    CAN_DFS_BIT_FIELD_FD_SSP_AUTO = CAN_InitStruct->CAN_FdSspAutoEn;
    if (CAN_InitStruct->CAN_FdEn)
    {
        CAN_DFS_BIT_FIELD_FD_SSP_DCO_2 = (CAN_InitStruct->CAN_FdBitTiming.b.can_fd_tseg2 + 1) *
                                         (CAN_InitStruct->CAN_FdBitTiming.b.can_fd_brp + 1);
    }
    /* CAN test mode configuration */
    if (CAN_TEST_MODE_NONE == CAN_InitStruct->CAN_TestModeSel)
    {
        can_reg_0x00.b.test_mode_en = DISABLE;
    }
    else
    {
        can_reg_0x00.b.test_mode_en = ENABLE;
#if (CAN_SUPPORT_EXT_LOOPBACK == 1)
        can_reg_0x54.b.can_test_cfg = CAN_InitStruct->CAN_TestModeSel;
#else
        can_reg_0x54.b.can_silence_en = CAN_InitStruct->CAN_TestModeSel == CAN_TEST_MODE_SILENCE;
        can_reg_0x54.b.can_lpb_en = CAN_InitStruct->CAN_TestModeSel == CAN_TEST_MODE_INT_LOOPBACK;
#endif
        if ((CAN_InitStruct->CAN_TestModeSel == CAN_TEST_MODE_INT_LOOPBACK
#if (CAN_SUPPORT_EXT_LOOPBACK == 1)
             || CAN_InitStruct->CAN_TestModeSel == CAN_TEST_MODE_EXT_LOOPBACK
#endif
            )
            && CAN_InitStruct->CAN_FdEn)
        {
            CAN_DFS_BIT_FIELD_FD_SSP_AUTO = 0;
            can_reg_0x14.b.can_fd_ssp_min = 0;
            can_reg_0x14.b.can_fd_ssp = 0;
        }
    }

    /* adjust CAN message buffer base address */
    for (uint8_t index = 0; index < CAN_MESSAGE_BUFFER_MAX_CNT; index++)
    {
        can_reg_0x200_0x23c.d32 = CANx->CAN_MB0_15_CTRL[index];
        can_reg_0x200_0x23c.b.can_msg_ba = CAN_MESSAGE_BUFFER_DEFAULT_LEN * index;
        CANx->CAN_MB0_15_CTRL[index] = can_reg_0x200_0x23c.d32;
    }

    can_reg_0x2f0.b.can_msg_ba_end = CAN_MESSAGE_BUFFER_MAX_CNT * CAN_MESSAGE_BUFFER_DEFAULT_LEN;

    CANx->CAN_MB_BA_END = can_reg_0x2f0.d32;
    CANx->CAN_CTL = can_reg_0x00.d32;
    CANx->CAN_BIT_TIMING = can_reg_0x0C.d32;
    CANx->CAN_FD_BIT_TIMING = can_reg_0x10.d32;
    CANx->CAN_FD_SSP_CAL = can_reg_0x14.d32;
    CANx->CAN_ERR_CNT_CTL = can_reg_0x2C.d32;
    CANx->CAN_TIME_STAMP = can_reg_0x40.d32;
    CANx->CAN_TEST = can_reg_0x54.d32;
}

/**
  * \brief  Fills each CAN_InitStruct member with its default value.
  * \param  CAN_InitStruct: pointer to an CAN_InitTypeDef structure which will be initialized.
  * \return None
  */
void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct)
{
    /* common configuration */
    CAN_InitStruct->CAN_TriSampleEn                  = DISABLE;
    CAN_InitStruct->CAN_AutoReTxEn                   = ENABLE;
    CAN_InitStruct->CAN_RxFifoEn                     = DISABLE;
    CAN_InitStruct->CAN_RxDmaEn                      = DISABLE;
    CAN_InitStruct->CAN_TimeStamp.b.can_time_stamp_en  = DISABLE;
    CAN_InitStruct->CAN_TimeStamp.b.can_time_stamp_div = 0;
    CAN_InitStruct->CAN_ErrorWarnThd                 = CAN_DEFAULT_ERR_WARN_TH;

    /* CAN 2.0 A/B configuration */
    /* baud rate period = ((tseg_1 +1) + (tseg_2+1) + 1 ) * (brp + 1) * clk_can */
    CAN_InitStruct->CAN_BitTiming.b.can_brp            = 7;
    CAN_InitStruct->CAN_BitTiming.b.can_sjw            = 3;
    CAN_InitStruct->CAN_BitTiming.b.can_tseg2          = 3;
    CAN_InitStruct->CAN_BitTiming.b.can_tseg1          = 4;

    /* CAN FD configuration */
    CAN_InitStruct->CAN_FdEn                         = ENABLE;
    CAN_InitStruct->CAN_FdCrcMode                    = CAN_FD_ISO_CRC;
    CAN_InitStruct->CAN_FdSspAutoEn                  = ENABLE;
    CAN_InitStruct->CAN_FdBitTiming.b.can_fd_brp       = 1;
    CAN_InitStruct->CAN_FdBitTiming.b.can_fd_sjw       = 3;
    CAN_InitStruct->CAN_FdBitTiming.b.can_fd_tseg2     = 3;
    CAN_InitStruct->CAN_FdBitTiming.b.can_fd_tseg1     = 4;
    CAN_InitStruct->CAN_FdSspCal.b.can_fd_ssp_dco      = 0;
    CAN_InitStruct->CAN_FdSspCal.b.can_fd_ssp_min      = 0;
    CAN_InitStruct->CAN_FdSspCal.b.can_fd_ssp          = 0;

    /* CAN test mode configuration */
    CAN_InitStruct->CAN_TestModeSel             = CAN_TEST_MODE_NONE;
}

/**
  * \brief  Enable or disable the CAN function.
  * \param  NewState: new state of the operation mode.
  *         This parameter can be: ENABLE or DISABLE.
  * \return None
  */
void CAN_Cmd(CAN_TypeDef *CANx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    CAN_0x00_TYPE_TypeDef can_reg_0x00 = {.d32 = CANx->CAN_CTL};

    if (NewState != DISABLE)
    {
        /* enable can bus and set bus_on_req */
        can_reg_0x00.b.can_en = ENABLE;
        can_reg_0x00.b.bus_on_req = SET;
    }
    else
    {
        /* disable can bus */
        can_reg_0x00.b.can_en = DISABLE;
    }
    CANx->CAN_CTL = can_reg_0x00.d32;
}

/**
  * \brief  Enables or disables the specified CAN interrupts.
  * \param  CAN_INT: specifies the CAN interrupts sources to be enabled or disabled.
  *         This parameter can be the following values:
  *         \arg CAN_RAM_MOVE_DONE_INT: data move from register to CAN IP internal RAM finished interupt.
  *         \arg CAN_BUS_OFF_INT: bus off interrupt.
  *         \arg CAN_WAKE_UP_INT: wakeup interrupt.
  *         \arg CAN_ERROR_INT: error interrupt.
  *         \arg CAN_RX_INT: RX interrupt.
  *         \arg CAN_TX_INT: TX interrupt.
  * \param  NewState: new state of the specified CAN interrupts.
  *         This parameter can be: ENABLE or DISABLE.
  * \return None
  */
void CAN_INTConfig(CAN_TypeDef *CANx, uint32_t CAN_INT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_CAN_INT_CONFIG(CAN_INT));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    CAN_0x18_TYPE_TypeDef can_reg_0x18 = {.d32 = CANx->CAN_INT_EN};

    if (newState == ENABLE)
    {
        can_reg_0x18.d32 |= CAN_INT;
    }
    else
    {
        can_reg_0x18.d32 &= (~CAN_INT);
    }

    CANx->CAN_INT_EN = can_reg_0x18.d32;
}

void CAN_MBTxINTConfig(CAN_TypeDef *CANx, uint8_t message_buffer_index, FunctionalState newState)
{
    CAN_0x20_TYPE_TypeDef can_reg_0x20 = {.d32 = CANx->CAN_MB_TXINT_EN};

    if (newState == ENABLE)
    {
        can_reg_0x20.b.can_mb_txint_en |= (1UL << message_buffer_index);
    }
    else
    {
        can_reg_0x20.b.can_mb_txint_en &= (1UL << message_buffer_index);
    }

    CANx->CAN_MB_TXINT_EN = can_reg_0x20.d32;
}

void CAN_MBRxINTConfig(CAN_TypeDef *CANx, uint8_t message_buffer_index, FunctionalState newState)
{
    CAN_0x1C_TYPE_TypeDef can_reg_0x1c = {.d32 = CANx->CAN_MB_RXINT_EN};

    if (newState == ENABLE)
    {
        can_reg_0x1c.b.can_mb_rxint_en |= (1UL << message_buffer_index);
    }
    else
    {
        can_reg_0x1c.b.can_mb_rxint_en &= (1UL << message_buffer_index);
    }
    CANx->CAN_MB_RXINT_EN = can_reg_0x1c.d32;
}

#if (CAN_SUPPORT_INT_MSK_STS == 1)
/**
  * \brief  Mask or unmask the specified CAN interrupt.
  * \param  CAN_INT_FLAG: the specified CAN interrupts.
  *         This parameter can be one of the following values:
  *         \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
  *         \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
  *         \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
  *         \arg CAN_ERROR_INT_FLAG: error interrupt flag.
  *         \arg CAN_RX_INT_FLAG: RX interrupt flag.
  *         \arg CAN_TX_INT_FLAG: Clear TX interrupt flag.
  * \return None.
  */

void CAN_MaskINTConfig(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG, FunctionalState NewState)
{
    assert_param(IS_CAN_INT_FLAG(CAN_INT_FLAG));

    CAN_0x18_TYPE_TypeDef can_reg_0x18 = {.d32 = CANx->CAN_INT_EN};

    if (NewState != DISABLE)
    {
        can_reg_0x18.d32 |= (CAN_INT_FLAG << 16);
    }
    else
    {
        can_reg_0x18.d32 &= ~(CAN_INT_FLAG << 16);
    }
    CANx->CAN_INT_EN = can_reg_0x18.d32;
}
#endif

/**
  * \brief  Get the specified CAN interrupt raw status.
  * \param  CAN_INT_FLAG: the specified CAN interrupts.
  *         This parameter can be one of the following values:
  *         \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
  *         \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
  *         \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
  *         \arg CAN_ERROR_INT_FLAG: error interrupt flag.
  *         \arg CAN_RX_INT_FLAG: RX interrupt flag.
  *         \arg CAN_TX_INT_FLAG: Clear TX interrupt flag.
  * \return The new state of CAN_INT (SET or RESET).
  */
ITStatus CAN_GetINTRawStatus(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG)
{
    ITStatus bit_status = RESET;
    /* Check the parameters */
    assert_param(IS_CAN_INT_FLAG(CAN_INT_FLAG));

    if ((CANx->CAN_INT_FLAG & CAN_INT_FLAG) != (uint32_t)RESET)
    {
        bit_status = SET;
    }
    /* Return the CAN_INT status */
    return  bit_status;
}
/**
  * \brief  Get the specified CAN interrupt status.
  * \param  CAN_INT_FLAG: the specified CAN interrupts.
  *         This parameter can be one of the following values:
  *         \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
  *         \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
  *         \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
  *         \arg CAN_ERROR_INT_FLAG: error interrupt flag.
  *         \arg CAN_RX_INT_FLAG: RX interrupt flag.
  *         \arg CAN_TX_INT_FLAG: Clear TX interrupt flag.
  * \return The new state of CAN_INT (SET or RESET).
  */
ITStatus CAN_GetINTStatus(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG)
{
    ITStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_CAN_INT_FLAG(CAN_INT_FLAG));
#if CAN_SUPPORT_INT_MSK_STS
    if ((CANx->CAN_INT_FLAG & (CAN_INT_FLAG << 16)) != (uint32_t)RESET)
    {
        bit_status = SET;
    }
#else
    bit_status = CAN_GetINTRawStatus(CAN_INT_FLAG);
#endif
    /* Return the CAN_INT status */
    return  bit_status;
}

/**
  * \brief  Clears the CAN interrupt pending bits.
  * \param  CAN_INT_FLAG: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *         \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
  *         \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
  *         \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
  *         \arg CAN_ERROR_INT_FLAG: error interrupt flag.
  *         \arg CAN_RX_INT_FLAG: RX interrupt flag.
  *         \arg CAN_TX_INT: Clear TX interrupt flag.
  * \return None
  */
void CAN_ClearINTPendingBit(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG)
{
    /* Check the parameters */
    assert_param(IS_CAN_INT_FLAG(CAN_INT_FLAG));

    /* write 1 to clear */
    CANx->CAN_INT_FLAG = CAN_INT_FLAG;
}

FlagStatus CAN_GetErrorStatus(CAN_TypeDef *CANx, uint32_t CAN_ERR_STAT)
{
    FlagStatus bit_status = RESET;

    /* Check the parameters */
    assert_param(IS_CAN_ERROR_STATUS(CAN_ERR_STAT));

    if ((CANx->CAN_ERR_STATUS & CAN_ERR_STAT) != (uint32_t)RESET)
    {
        bit_status = SET;
    }

    /* Return the FlagStatus */
    return  bit_status;
}

void CAN_CLearErrorStatus(CAN_TypeDef *CANx, uint32_t CAN_ERR_STAT)
{
    /* Check the parameters */
    assert_param(IS_CAN_ERROR_STATUS(CAN_ERR_STAT));

    /* write 1 to clear */
    CANx->CAN_ERR_STATUS = CAN_ERR_STAT;
}

CANError_TypeDef CAN_SetMsgBufTxMode(CAN_TypeDef *CANx, CANTxFrame_TypeDef *p_tx_frame_params,
                                     uint8_t *p_frame_data,
                                     uint8_t data_len)
{
    CAN_0x340_TYPE_TypeDef can_reg_0x340 = {.d32 = 0};
    CAN_0x348_TYPE_TypeDef can_reg_0x348 = {.d32 = 0};
    CAN_0x34C_TYPE_TypeDef can_reg_0x34C = {.d32 = 0};
    CAN_0x00_TYPE_TypeDef can_reg_0x00 = {.d32 = CANx->CAN_CTL};
    CAN_0x300_0x33C_TYPE_TypeDef can_reg_0x300_0x33c = {.d32 = 0};

    /* Check the parameters */
    if (can_reg_0x00.b.rx_fifo_en == ENABLE)
    {
        if (p_tx_frame_params->msg_buf_id >= CAN_MESSAGE_FIFO_START_ID)
        {
            return CAN_MSG_ID_ERR;
        }
    }
    else
    {
        if (p_tx_frame_params->msg_buf_id >= CAN_MESSAGE_BUFFER_MAX_CNT)
        {
            return CAN_MSG_ID_ERR;
        }
    }

    if ((p_tx_frame_params->standard_frame_id > CAN_STAND_FRAME_ID_MAX_VALUE)
        || (p_tx_frame_params->extend_frame_id > CAN_EXTEND_FRAME_ID_MAX_VALUE))
    {
        return CAN_ID_ERR;
    }

    if ((p_tx_frame_params->frame_type == CAN_FD_STD_DATA_FRAME)
        || (p_tx_frame_params->frame_type == CAN_FD_EXT_DATA_FRAME))
    {
        if ((data_len <= CAN_STAND_DATA_MAX_LEN)
            || (data_len == 12) || (data_len == 16) || (data_len == 20)
            || (data_len == 24) || (data_len == 32) || (data_len == 48)
            || (data_len == 64))
        {
            /* data length is valid */
        }
        else
        {
            return CAN_DATA_LEN_ERR;
        }
    }
    else
    {
        if (data_len > CAN_STAND_DATA_MAX_LEN)
        {
            return CAN_DATA_LEN_ERR;
        }
    }

    /* fill frame configuration */
    switch (p_tx_frame_params->frame_type)
    {
    case CAN_STD_DATA_FRAME:
        {
            can_reg_0x340.b.can_ram_rtr = CAN_RTR_DATA_FRAME;
            can_reg_0x340.b.can_ram_ide = CAN_IDE_STANDARD_FORMAT;
            can_reg_0x348.b.can_ram_edl = CAN_EDL_STARDARD_FRAME;
        }
        break;

    case CAN_EXT_DATA_FRAME:
        {
            can_reg_0x340.b.can_ram_rtr = CAN_RTR_DATA_FRAME;
            can_reg_0x340.b.can_ram_ide = CAN_IDE_EXTEND_FORMAT;
            can_reg_0x348.b.can_ram_edl = CAN_EDL_STARDARD_FRAME;
        }
        break;

    case CAN_STD_REMOTE_FRAME:
        {
            can_reg_0x340.b.can_ram_rtr = CAN_RTR_REMOTE_FRAME;
            can_reg_0x340.b.can_ram_ide = CAN_IDE_STANDARD_FORMAT;
            can_reg_0x348.b.can_ram_edl = CAN_EDL_STARDARD_FRAME;
        }
        break;

    case CAN_EXT_REMOTE_FRAME:
        {
            can_reg_0x340.b.can_ram_rtr = CAN_RTR_REMOTE_FRAME;
            can_reg_0x340.b.can_ram_ide = CAN_IDE_EXTEND_FORMAT;
            can_reg_0x348.b.can_ram_edl = CAN_EDL_STARDARD_FRAME;
        }
        break;

    case CAN_FD_STD_DATA_FRAME:
        {
            can_reg_0x340.b.can_ram_rtr = CAN_RTR_DATA_FRAME;
            can_reg_0x340.b.can_ram_ide = CAN_IDE_STANDARD_FORMAT;
            can_reg_0x348.b.can_ram_edl = CAN_EDL_FD_FRAME;
        }
        break;

    case CAN_FD_EXT_DATA_FRAME:
        {
            can_reg_0x340.b.can_ram_rtr = CAN_RTR_DATA_FRAME;
            can_reg_0x340.b.can_ram_ide = CAN_IDE_EXTEND_FORMAT;
            can_reg_0x348.b.can_ram_edl = CAN_EDL_FD_FRAME;
        }
        break;

    default:
        return CAN_TYPE_ERR;
    }

    /* fill frame id */
    can_reg_0x340.b.can_ram_id = ((p_tx_frame_params->standard_frame_id & CAN_STAND_FRAME_ID_MAX_VALUE)
                                  << CAN_STD_FRAME_ID_POS
                                  | (p_tx_frame_params->extend_frame_id & CAN_EXTEND_FRAME_ID_MAX_VALUE)) ;
    /* set frame TX */
    can_reg_0x348.b.can_ram_rxtx = SET;  /* 0 - Rx, 1 - Tx */

    /* set autoreply bit */
    can_reg_0x348.b.can_ram_autoreply = p_tx_frame_params->auto_reply_bit;

    /* set brs bit */
    can_reg_0x348.b.can_ram_brs = p_tx_frame_params->frame_brs_bit;

    /* fill frame data */
    if (data_len <= CAN_STAND_DATA_MAX_LEN)
    {
        can_reg_0x348.b.can_ram_dlc = data_len;
    }
    else if (data_len == 12)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_12;
    }
    else if (data_len == 16)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_16;
    }
    else if (data_len == 20)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_20;
    }
    else if (data_len == 24)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_24;
    }
    else if (data_len == 32)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_32;
    }
    else if (data_len == 48)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_48;
    }
    else if (data_len == 64)
    {
        can_reg_0x348.b.can_ram_dlc = CAN_DLC_BYTES_64;
    }

    uint8_t blkcount  = data_len / 4;
    uint8_t remainder = data_len % 4;

    uint32_t *p = (uint32_t *)p_frame_data;

    for (uint8_t index = 0; index < blkcount; index++)
    {
        CANx->CAN_RAM_DATA15_0[CAN_MESSAGE_BUFFER_MAX_INDEX - index] = p[index];
    }

    if (remainder != 0)
    {
        for (uint8_t index = 0; index < remainder; index++)
        {
            can_reg_0x300_0x33c.d8[index] =  p_frame_data[blkcount * 4 + index];
        }
        CANx->CAN_RAM_DATA15_0[CAN_MESSAGE_BUFFER_MAX_INDEX - blkcount] = can_reg_0x300_0x33c.d32;
    }

    /* set CAN message buffer access control */
    can_reg_0x34C.b.can_ram_acc_num = p_tx_frame_params->msg_buf_id;
    can_reg_0x34C.b.can_ram_acc_mask = SET;
    can_reg_0x34C.b.can_ram_acc_cs = SET;
    can_reg_0x34C.b.can_ram_acc_arb = SET;
    can_reg_0x34C.b.can_ram_acc_data = 0xFFFF;  /* set data exchange field */
    can_reg_0x34C.b.can_ram_buffer_en = SET;
    can_reg_0x34C.b.can_ram_dir = SET;  /* 0 - read, 1 - write */
    can_reg_0x34C.b.can_ram_start = SET;

    CANx->CAN_RAM_ARB = can_reg_0x340.d32;
    CANx->CAN_RAM_CS = can_reg_0x348.d32;
    CANx->CAN_RAM_CMD = can_reg_0x34C.d32;

    return CAN_NO_ERR;
}

CANError_TypeDef CAN_SetMsgBufRxMode(CAN_TypeDef *CANx, CANRxFrame_TypeDef *p_rx_frame_params)
{
    CAN_0x340_TYPE_TypeDef can_reg_0x340 = {.d32 = 0};
    CAN_0x344_TYPE_TypeDef can_reg_0x344 = {.d32 = 0};
    CAN_0x348_TYPE_TypeDef can_reg_0x348 = {.d32 = 0};
    CAN_0x34C_TYPE_TypeDef can_reg_0x34C = {.d32 = 0};
    CAN_0x00_TYPE_TypeDef can_reg_0x00 = {.d32 = CANx->CAN_CTL};
    CAN_0x200_0x23C_TYPE_TypeDef can_reg_0x200_0x23c = {.d32 = 0};

    /* Check the parameters */
    if (can_reg_0x00.b.rx_fifo_en == ENABLE)
    {
        if (p_rx_frame_params->msg_buf_id > CAN_MESSAGE_FIFO_START_ID)
        {
            return CAN_MSG_ID_ERR;
        }
    }
    else
    {
        if (p_rx_frame_params->msg_buf_id >= CAN_MESSAGE_BUFFER_MAX_CNT)
        {
            return CAN_MSG_ID_ERR;
        }
    }

    if ((p_rx_frame_params->frame_id_mask == RESET)
        && ((p_rx_frame_params->standard_frame_id > CAN_STAND_FRAME_ID_MAX_VALUE)
            || (p_rx_frame_params->extend_frame_id > CAN_EXTEND_FRAME_ID_MAX_VALUE)))
    {
        return CAN_ID_ERR;
    }

    /* fill arb and mask field configuration */
    can_reg_0x344.b.can_ram_rtr_mask = p_rx_frame_params->frame_rtr_mask;
    can_reg_0x340.b.can_ram_rtr = p_rx_frame_params->frame_rtr_bit;

    can_reg_0x344.b.can_ram_ide_mask = p_rx_frame_params->frame_ide_mask;
    can_reg_0x340.b.can_ram_ide = p_rx_frame_params->frame_ide_bit;

    can_reg_0x344.b.can_ram_id_mask = p_rx_frame_params->frame_id_mask & CAN_FRAME_ID_MASK_MAX_VALUE;
    can_reg_0x340.b.can_ram_id = ((p_rx_frame_params->standard_frame_id & CAN_STAND_FRAME_ID_MAX_VALUE)
                                  << CAN_STD_FRAME_ID_POS
                                  | (p_rx_frame_params->extend_frame_id & CAN_EXTEND_FRAME_ID_MAX_VALUE)) ;

    /* set frame TX */
    can_reg_0x348.b.can_ram_rxtx = RESET;  /* 0 - Rx, 1 - Tx */

    /* set autoreply bit */
    can_reg_0x348.b.can_ram_autoreply = p_rx_frame_params->auto_reply_bit;

    /* set CAN message buffer access control */
    can_reg_0x34C.b.can_ram_acc_num = p_rx_frame_params->msg_buf_id;
    can_reg_0x34C.b.can_ram_acc_mask = SET;
    can_reg_0x34C.b.can_ram_acc_cs = SET;
    can_reg_0x34C.b.can_ram_acc_arb = SET;
    can_reg_0x34C.b.can_ram_buffer_en = SET;
    can_reg_0x34C.b.can_ram_dir = SET;  /* 0 - read, 1 - write */
    can_reg_0x34C.b.can_ram_start = SET;

    if (ENABLE == p_rx_frame_params->rx_dma_en)
    {
        can_reg_0x200_0x23c.d32 = CANx->CAN_MB0_15_CTRL[p_rx_frame_params->msg_buf_id];
        can_reg_0x200_0x23c.b.can_msg_rxdma_en = ENABLE;
        CANx->CAN_MB0_15_CTRL[p_rx_frame_params->msg_buf_id] = can_reg_0x200_0x23c.d32;
    }

    CANx->CAN_RAM_ARB = can_reg_0x340.d32;
    CANx->CAN_RAM_MASK = can_reg_0x344.d32;
    CANx->CAN_RAM_CS = can_reg_0x348.d32;
    CANx->CAN_RAM_CMD = can_reg_0x34C.d32;

    return CAN_NO_ERR;
}

static const uint8_t CAN_DlcBytesTable[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64,
};

CANError_TypeDef CAN_GetMsgBufInfo(CAN_TypeDef *CANx, uint8_t msg_buf_id,
                                   CANMsgBufInfo_TypeDef *p_mb_info)
{
    uint32_t timeout;

    CAN_0x340_TYPE_TypeDef can_reg_0x340 = {.d32 = 0};
    CAN_0x344_TYPE_TypeDef can_reg_0x344 = {.d32 = 0};
    CAN_0x348_TYPE_TypeDef can_reg_0x348 = {.d32 = 0};
    CAN_0x34C_TYPE_TypeDef can_reg_0x34C = {.d32 = 0};
    CAN_0x200_0x23C_TYPE_TypeDef can_reg_0x200_0x23c = {.d32 = 0};

    /* Check the parameters */
    if (msg_buf_id >= CAN_MESSAGE_BUFFER_MAX_CNT)
    {
        return CAN_MSG_ID_ERR;
    }

    /* set CAN message buffer access control */
    can_reg_0x34C.b.can_ram_acc_num = msg_buf_id;
    can_reg_0x34C.b.can_ram_acc_mask = SET;
    can_reg_0x34C.b.can_ram_acc_cs = SET;
    can_reg_0x34C.b.can_ram_acc_arb = SET;
    can_reg_0x34C.b.can_ram_acc_data = 0xFFFF;  /* set data exchange field */
    can_reg_0x34C.b.can_ram_dir = RESET;  /* 0 - read, 1 - write */
    can_reg_0x34C.b.can_ram_start = SET;

    CANx->CAN_RAM_CMD = can_reg_0x34C.d32;

    timeout = 100;
    while ((CAN_GetRamState(CANx) != CAN_RAM_STATE_IDLE) && --timeout);
    if (timeout == 0)
    {
        return CAN_TIMEOUT_ERR;
    }

    can_reg_0x340.d32 = CANx->CAN_RAM_ARB;
    can_reg_0x344.d32 = CANx->CAN_RAM_MASK;
    can_reg_0x348.d32 = CANx->CAN_RAM_CS;

    p_mb_info->rtr_bit = (CANRtrSel_TypeDef)(can_reg_0x340.b.can_ram_rtr);
    p_mb_info->ide_bit = (CANIdeSel_TypeDef)(can_reg_0x340.b.can_ram_ide);
    p_mb_info->standard_frame_id = (can_reg_0x340.b.can_ram_id >> CAN_STD_FRAME_ID_POS) &
                                   CAN_STAND_FRAME_ID_MAX_VALUE;
    p_mb_info->extend_frame_id  = can_reg_0x340.b.can_ram_id & CAN_EXTEND_FRAME_ID_MAX_VALUE;

    p_mb_info->rtr_mask = can_reg_0x344.b.can_ram_rtr_mask;
    p_mb_info->ide_mask = can_reg_0x344.b.can_ram_ide_mask;
    p_mb_info->id_mask = can_reg_0x344.b.can_ram_id_mask;

    p_mb_info->rx_timestamp = can_reg_0x348.b.can_ram_timestamp;
    p_mb_info->esi_bit = can_reg_0x348.b.can_ram_esi;
    p_mb_info->brs_bit = (CANBrsSel_TypeDef)(can_reg_0x348.b.can_ram_brs);
    p_mb_info->edl_bit = (CANEdlSel_TypeDef)(can_reg_0x348.b.can_ram_edl);
    p_mb_info->auto_reply_bit = can_reg_0x348.b.can_ram_autoreply;
    p_mb_info->rxtx_bit = can_reg_0x348.b.can_ram_rxtx;
    p_mb_info->rx_lost_bit = can_reg_0x348.b.can_ram_lost;

    /* convert dlc to data length */
    p_mb_info->data_length = CAN_DlcBytesTable[can_reg_0x348.b.can_ram_dlc];
    /* get message buffer rx dma enable flag */
    can_reg_0x200_0x23c.d32 = CANx->CAN_MB0_15_CTRL[msg_buf_id];
    p_mb_info->rx_dma_en = can_reg_0x200_0x23c.b.can_msg_rxdma_en;

    return CAN_NO_ERR;
}

CANError_TypeDef CAN_GetRamData(CAN_TypeDef *CANx, uint8_t data_len, uint8_t *p_data)
{
    /* Check the parameters */
    if ((data_len == 0) || (data_len > CAN_FD_DATA_MAX_LEN))
    {
        return CAN_DATA_LEN_ERR;
    }

    uint8_t blkcount  = data_len / 4;
    uint8_t remainder = data_len % 4;
    CAN_0x300_0x33C_TYPE_TypeDef can_reg_0x300_0x33c = {.d32 = 0};
    uint32_t *p = (uint32_t *)p_data;

    for (uint8_t index = 0; index < blkcount; index++)
    {
        p[index] = CANx->CAN_RAM_DATA15_0[CAN_MESSAGE_BUFFER_MAX_INDEX - index];
    }

    if (remainder != 0)
    {
        can_reg_0x300_0x33c.d32 = CANx->CAN_RAM_DATA15_0[CAN_MESSAGE_BUFFER_MAX_INDEX - blkcount];
        for (uint8_t index = 0; index < remainder; index++)
        {
            p_data[blkcount * 4 + index] = can_reg_0x300_0x33c.d8[index];
        }
    }

    return CAN_NO_ERR;
}

CANDataFrameSel_TypeDef CAN_CheckFrameType(uint8_t rtr_bit, uint8_t ide_bit, uint8_t edl_bit)
{
    CANDataFrameSel_TypeDef can_frame_type = CAN_INVALID_DATA_FRAME;

    if ((rtr_bit == CAN_RTR_DATA_FRAME)
        && (ide_bit == CAN_IDE_STANDARD_FORMAT)
        && (edl_bit == CAN_EDL_STARDARD_FRAME))
    {
        can_frame_type = CAN_STD_DATA_FRAME;
    }
    else if ((rtr_bit == CAN_RTR_DATA_FRAME)
             && (ide_bit == CAN_IDE_EXTEND_FORMAT)
             && (edl_bit == CAN_EDL_STARDARD_FRAME))
    {
        can_frame_type = CAN_EXT_DATA_FRAME;
    }
    else if ((rtr_bit == CAN_RTR_REMOTE_FRAME)
             && (ide_bit == CAN_IDE_STANDARD_FORMAT)
             && (edl_bit == CAN_EDL_STARDARD_FRAME))
    {
        can_frame_type = CAN_STD_REMOTE_FRAME;
    }
    else if ((rtr_bit == CAN_RTR_REMOTE_FRAME)
             && (ide_bit == CAN_IDE_EXTEND_FORMAT)
             && (edl_bit == CAN_EDL_STARDARD_FRAME))
    {
        can_frame_type = CAN_EXT_REMOTE_FRAME;
    }
    else if ((rtr_bit == CAN_RTR_DATA_FRAME)
             && (ide_bit == CAN_IDE_STANDARD_FORMAT)
             && (edl_bit == CAN_EDL_FD_FRAME))
    {
        can_frame_type = CAN_FD_STD_DATA_FRAME;
    }
    else if ((rtr_bit == CAN_RTR_DATA_FRAME)
             && (ide_bit == CAN_IDE_EXTEND_FORMAT)
             && (edl_bit == CAN_EDL_FD_FRAME))
    {
        can_frame_type = CAN_FD_EXT_DATA_FRAME;
    }
    else
    {
        can_frame_type = CAN_INVALID_DATA_FRAME;
    }

    return can_frame_type;
}

void CAN_GetFifoStatus(CAN_TypeDef *CANx, CANFifoStatus_TypeDef *CAN_FifoStatus)
{
    CAN_0x08_TYPE_TypeDef can_reg_0x08 = {.d32 = CANx->CAN_FIFO_STS};

    CAN_FifoStatus->fifo_msg_lvl = can_reg_0x08.b.fifo_msg_lvl;
    CAN_FifoStatus->fifo_msg_empty = can_reg_0x08.b.fifo_msg_empty;
    CAN_FifoStatus->fifo_msg_full = can_reg_0x08.b.fifo_msg_full;
    CAN_FifoStatus->fifo_msg_overflow = can_reg_0x08.b.fifo_msg_overflow;
}

void CAN_TxTriggerConfig(CAN_TypeDef *CANx, FunctionalState newState,
                         uint16_t trigger_timestamp_begin,
                         uint16_t close_offset)
{
    CAN_0x44_TYPE_TypeDef can_reg_0x44 = {.d32 = 0};

    if (newState == ENABLE)
    {
        can_reg_0x44.b.tx_trigger_en = ENABLE;
        can_reg_0x44.b.tx_trigger_begin = trigger_timestamp_begin;
        can_reg_0x44.b.tx_trigger_close_offset = close_offset;
    }
    else
    {
        can_reg_0x44.b.tx_trigger_en = DISABLE;
    }

    CANx->CAN_MB_TRIGGER = can_reg_0x44.d32;
}

uint32_t CAN_GetBusState(CAN_TypeDef *CANx)
{
    CAN_0x04_TYPE_TypeDef can_reg_0x04 = {.d32 = CANx->CAN_STS};
    return ((can_reg_0x04.b.bus_on_state) ? CAN_BUS_STATE_ON : CAN_BUS_STATE_OFF);
}

uint32_t CAN_GetRamState(CAN_TypeDef *CANx)
{
    CAN_0x34C_TYPE_TypeDef can_reg_0x34c = {.d32 = CANx->CAN_RAM_CMD};
    return ((can_reg_0x34c.b.can_ram_start) ? CAN_RAM_STATE_EXCHANGING : CAN_RAM_STATE_IDLE);
}

FlagStatus CAN_GetMBnTxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x38_TYPE_TypeDef can_reg_0x38 = {.d32 = CANx->CAN_TX_DONE};
    return (can_reg_0x38.b.can_tx_done & (1UL << message_buffer_index)) ? SET : RESET;
}

void CAN_ClearMBnTxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CANx->CAN_TX_DONE = (1UL << message_buffer_index);
}

FlagStatus CAN_GetMBnTxErrorFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x34_TYPE_TypeDef can_reg_0x34 = {.d32 = CANx->CAN_TX_ERROR_FLAG};
    return (can_reg_0x34.b.can_tx_error_flag & (1UL << message_buffer_index)) ? SET : RESET;
}

void CAN_ClearMBnTxErrorFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CANx->CAN_TX_ERROR_FLAG = (1UL << message_buffer_index);
}

FlagStatus CAN_GetMBnStatusTxFinishFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    return (FlagStatus)(((CAN_0x100_0x13C_TYPE_TypeDef)(
                             CANx->CAN_MB0_15_STS[message_buffer_index])).b.can_msg_tx_done);
}

FlagStatus CAN_GetMBnStatusTxReqFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x100_0x13C_TYPE_TypeDef can_reg_0x100_0x13c = {.d32 = CANx->CAN_MB0_15_STS[message_buffer_index]};
    return (FlagStatus)(can_reg_0x100_0x13c.b.can_msg_tx_req);
}

FlagStatus CAN_GetMBnRxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x3C_TYPE_TypeDef can_reg_0x3c = {.d32 = CANx->CAN_RX_DONE};
    return (can_reg_0x3c.b.can_rx_done & (1UL << message_buffer_index)) ? SET : RESET;
}

void CAN_ClearMBnRxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CANx->CAN_RX_DONE = (1UL << message_buffer_index);
}

FlagStatus CAN_GetMBnStatusRxValidFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x100_0x13C_TYPE_TypeDef can_reg_0x100_0x13c = {.d32 = CANx->CAN_MB0_15_STS[message_buffer_index]};
    return (FlagStatus)(can_reg_0x100_0x13c.b.can_msg_rx_vld);
}

FlagStatus CAN_GetMBnStatusRxReadyFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x100_0x13C_TYPE_TypeDef can_reg_0x100_0x13c = {.d32 = CANx->CAN_MB0_15_STS[message_buffer_index]};
    return (FlagStatus)(can_reg_0x100_0x13c.b.can_msg_rx_rdy);
}

void CAN_TimeStampConfig(CAN_TypeDef *CANx, FunctionalState newState)
{
    CAN_0x40_TYPE_TypeDef can_reg_0x40 = {.d32 = CANx->CAN_TIME_STAMP};
    can_reg_0x40.b.can_time_stamp_en = newState;
    CANx->CAN_TIME_STAMP = can_reg_0x40.d32;
}

uint16_t CAN_GetTimeStampCount(CAN_TypeDef *CANx)
{
    CAN_0x40_TYPE_TypeDef can_reg_0x40 = {.d32 = CANx->CAN_TIME_STAMP};
    return (can_reg_0x40.b.can_time_stamp);
}

uint32_t CAN_GetRxDmaMsize(CAN_TypeDef *CANx)
{
    CAN_0x48_TYPE_TypeDef can_reg_0x48 = {.d32 = CANx->CAN_RXDMA_MSIZE};
    return (can_reg_0x48.b.can_rxdma_msize);
}

FlagStatus CAN_GetMBnRxDmaEnFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index)
{
    CAN_0x200_0x23C_TYPE_TypeDef can_reg_0x200_0x23c = {.d32 = CANx->CAN_MB0_15_CTRL[message_buffer_index]};
    return (FlagStatus)(can_reg_0x200_0x23c.b.can_msg_rxdma_en);
}

void CAN_SetMBnRxDmaEnFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index,
                           FunctionalState newState)
{
    CAN_0x200_0x23C_TYPE_TypeDef can_reg_0x200_0x23c = {.d32 = CANx->CAN_MB0_15_CTRL[message_buffer_index]};
    can_reg_0x200_0x23c.b.can_msg_rxdma_en = newState;
    CANx->CAN_MB0_15_CTRL[message_buffer_index] = can_reg_0x200_0x23c.d32;
}

#if (CAN_SUPPORT_SLEEP_MODE == 1)
FlagStatus CAN_CheckSleepStatus(CAN_TypeDef *CANx)
{
    CAN_0x50_TYPE_TypeDef can_reg_0x50 = {.d32 = CANx->CAN_SLEEP_MODE};
    return (can_reg_0x50.b.can_sleep_state);
}

void CAN_LowPowerClkCmd(CAN_TypeDef *CANx, FunctionalState newState, CANLowPowerClkDIV_TypeDef div)
{
    assert_param(IS_CAN_LOWER_CLK_DIV(div));

    CAN_0x50_TYPE_TypeDef can_reg_0x50 = {.d32 = CANx->CAN_SLEEP_MODE};

    if (newState == ENABLE)
    {
        CAN_LowPowerClkDIV(CANx, div);
        can_reg_0x50.b.can_lp_ck_en = 1;
        CANx->CAN_SLEEP_MODE = can_reg_0x50.d32;
    }
    else
    {
        can_reg_0x50.b.can_lp_ck_en = 0;
        CANx->CAN_SLEEP_MODE = can_reg_0x50.d32;
        CAN_LowPowerClkDIV(CANx, div);
    }
}

void CAN_SetWakeUpPinFltFunction(CAN_TypeDef *CANx, FunctionalState newState, uint8_t flt_length)
{
    CAN_0x50_TYPE_TypeDef can_reg_0x50 = {.d32 = CANx->CAN_SLEEP_MODE};

    if (newState == ENABLE)
    {
        if (flt_length < 1)
        {
            flt_length = 1;
        }
        can_reg_0x50.b.can_wakepin_flt_en = 1;
        can_reg_0x50.b.can_wakepin_flt_length = flt_length;
    }
    else
    {
        can_reg_0x50.b.can_wakepin_flt_en = 0;
    }

    CANx->CAN_SLEEP_MODE = can_reg_0x50.d32;
}

void CAN_RequestToSleepMode(CAN_TypeDef *CANx)
{
    CAN_0x50_TYPE_TypeDef can_reg_0x50 = {.d32 = CANx->CAN_SLEEP_MODE};
    can_reg_0x50.b.can_sleep_req = 1;
    CANx->CAN_SLEEP_MODE = can_reg_0x50.d32;
}

void CAN_ManualWakeup(CAN_TypeDef *CANx)
{
    CAN_0x50_TYPE_TypeDef can_reg_0x50 = {.d32 = CANx->CAN_SLEEP_MODE};
    can_reg_0x50.b.can_manual_wakeup = 1;
    CANx->CAN_SLEEP_MODE = can_reg_0x50.d32;
}
#endif
/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/
