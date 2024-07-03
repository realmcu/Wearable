/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl_can.h
* \brief    The header file of the peripheral CAN driver.
* \details  This file provides all CAN firmware functions.
* \author   colin
* \date     2024-06-26
* \version  v1.0
* *******************************************************************************************************
*/
/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _RTL_CAN_H_
#define _RTL_CAN_H_
#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "rtl876x.h"
#include "rtl_can_def.h"

/*============================================================================*
 *                          Private Macros
 *============================================================================*/
#define CAN_MESSAGE_BUFFER_MAX_CNT      16
#define CAN_MESSAGE_BUFFER_MAX_INDEX    (CAN_MESSAGE_BUFFER_MAX_CNT - 1)
#define CAN_MESSAGE_BUFFER_DEFAULT_LEN  20
#define CAN_MESSAGE_FIFO_START_ID       12
#define CAN_DEFAULT_ERR_WARN_TH         96
#define CAN_STAND_DATA_MAX_LEN          8
#define CAN_FD_DATA_MAX_LEN             64
#define CAN_STD_FRAME_ID_POS            18
#define CAN_EXT_FRAME_ID_POS            0
#define CAN_RAM_ACC_DATA_POS            11
#define CAN_STAND_FRAME_ID_MAX_VALUE    0x7FFUL
#define CAN_EXTEND_FRAME_ID_MAX_VALUE   0x3FFFFUL
#define CAN_FRAME_ID_MASK_MAX_VALUE     0x1FFFFFFFUL
/*============================================================================*
 *                         Register Defines
 *============================================================================*/
/** \defgroup CAN         CAN
  * \brief
  * \{
  */
/*============================================================================*
 *                         Constants
 *============================================================================*/
/** \defgroup CAN_Exported_Constants CAN Exported Constants
  * \brief
  * \{
  */
/**
 * \defgroup    CAN_DLC_BYTE CAN DLC BYTE
 * \{
 * \ingroup     CAN_Exported_Constants
 */
#define CAN_DLC_BYTES_0                         (0x0UL)
#define CAN_DLC_BYTES_1                         (0x1UL)
#define CAN_DLC_BYTES_2                         (0x2UL)
#define CAN_DLC_BYTES_3                         (0x3UL)
#define CAN_DLC_BYTES_4                         (0x4UL)
#define CAN_DLC_BYTES_5                         (0x5UL)
#define CAN_DLC_BYTES_6                         (0x6UL)
#define CAN_DLC_BYTES_7                         (0x7UL)
#define CAN_DLC_BYTES_8                         (0x8UL)
#define CAN_DLC_BYTES_12                        (0x9UL)
#define CAN_DLC_BYTES_16                        (0xAUL)
#define CAN_DLC_BYTES_20                        (0xBUL)
#define CAN_DLC_BYTES_24                        (0xCUL)
#define CAN_DLC_BYTES_32                        (0xDUL)
#define CAN_DLC_BYTES_48                        (0xEUL)
#define CAN_DLC_BYTES_64                        (0xFUL)
/** End of CAN_DLC_BYTE
  * \}
  */
/**
 * \defgroup    CAN_Interrupt_Definition CAN Interrupt Definition
 * \{
 * \ingroup     CAN_Exported_Constants
 */
#define CAN_RAM_MOVE_DONE_INT                       BIT5
#define CAN_BUS_OFF_INT                             BIT4
#define CAN_WAKE_UP_INT                             BIT3
#define CAN_ERROR_INT                               BIT2
#define CAN_RX_INT                                  BIT1
#define CAN_TX_INT                                  BIT0
#define IS_CAN_INT_CONFIG(CONFIG)     (((CONFIG) == CAN_RAM_MOVE_DONE_INT)  || \
                                       ((CONFIG) == CAN_BUS_OFF_INT)        || \
                                       ((CONFIG) == CAN_WAKE_UP_INT)        || \
                                       ((CONFIG) == CAN_ERROR_INT)          || \
                                       ((CONFIG) == CAN_RX_INT)             || \
                                       ((CONFIG) == CAN_TX_INT))
/** End of CAN_Interrupt_Definition
  * \}
  */
/**
 * \defgroup    CAN_Interrupt_Flag CAN Interrupt Flag
 * \{
 * \ingroup     CAN_Exported_Constants
 */
#define CAN_RAM_MOVE_DONE_INT_FLAG                  BIT5
#define CAN_BUS_OFF_INT_FLAG                        BIT4
#define CAN_WAKE_UP_INT_FLAG                        BIT3
#define CAN_ERROR_INT_FLAG                          BIT2
#define CAN_RX_INT_FLAG                             BIT1
#define CAN_TX_INT_FLAG                             BIT0
#define IS_CAN_INT_FLAG(FLAG)         (((CONFIG) == CAN_RAM_MOVE_DONE_INT_FLAG) || \
                                       ((CONFIG) == CAN_BUS_OFF_INT_FLAG)       || \
                                       ((CONFIG) == CAN_WAKE_UP_INT_FLAG)       || \
                                       ((CONFIG) == CAN_ERROR_INT_FLAG)         || \
                                       ((CONFIG) == CAN_RX_INT_FLAG)            || \
                                       ((CONFIG) == CAN_TX_INT_FLAG))
/** End of CAN_Interrupt_Flag
  * \}
  */
/**
 * \defgroup    CAN_ERROR_Mask CAN ERROR Mask
 * \{
 * \ingroup     CAN_Exported_Constants
 */
#define CAN_ERROR_RX                                BIT9
#define CAN_ERROR_TX                                BIT8
#define CAN_ERROR_ACK                               BIT5
#define CAN_ERROR_STUFF                             BIT4
#define CAN_ERROR_CRC                               BIT3
#define CAN_ERROR_FORM                              BIT2
#define CAN_ERROR_BIT1                              BIT1
#define CAN_ERROR_BIT0                              BIT0
#define IS_CAN_ERROR_STATUS(STATUS)   (((STATUS) == CAN_ERROR_RX)   || \
                                       ((STATUS) == CAN_ERROR_TX)   || \
                                       ((STATUS) == CAN_ERROR_ACK)  || \
                                       ((STATUS) == CAN_ERROR_CRC)  || \
                                       ((STATUS) == CAN_ERROR_FORM) || \
                                       ((STATUS) == CAN_ERROR_BIT1) || \
                                       ((STATUS) == CAN_ERROR_BIT0))
/** End of CAN_ERROR_Mask
  * \}
  */
/** End of CAN_Exported_Constants
  * \}
  */
/*============================================================================*
 *                         Types
 *============================================================================*/
/** \defgroup CAN_Exported_Types CAN Exported Types
  * \brief
  * \{
  */
/**
 * \brief       CAN init structure definition.
 *
 * \ingroup     CAN_Exported_Types
 */
typedef struct
{
    uint8_t CAN_FdEn;
    uint8_t CAN_AutoReTxEn;
    uint8_t CAN_RxFifoEn;
    uint8_t CAN_RxDmaEn;
    uint8_t CAN_FdSspAutoEn;
    uint8_t CAN_TriSampleEn;
    uint8_t CAN_FdCrcMode;
    uint8_t CAN_TestModeSel;
    uint16_t CAN_ErrorWarnThd;
    CAN_0x0C_TYPE_TypeDef CAN_BitTiming;
    CAN_0x10_TYPE_TypeDef CAN_FdBitTiming;
    CAN_0x14_TYPE_TypeDef CAN_FdSspCal;
    CAN_0x40_TYPE_TypeDef CAN_TimeStamp;
} CAN_InitTypeDef;

typedef enum
{
    CAN_FD_ISO_CRC,
    CAN_FD_NON_ISO_CRC,
} CANFdCrcModeSel_TypeDef;

typedef enum
{
    CAN_BUS_STATE_OFF,
    CAN_BUS_STATE_ON,
} CANBusStateSel_TypeDef;

typedef enum
{
    CAN_RAM_STATE_IDLE,
    CAN_RAM_STATE_EXCHANGING,
} CANRamStateSel_TypeDef;

typedef enum
{
    CAN_TEST_MODE_SILENCE,
#if CAN_SUPPORT_EXT_LOOPBACK
    CAN_TEST_MODE_EXT_LOOPBACK,
#endif
    CAN_TEST_MODE_INT_LOOPBACK,
    CAN_TEST_MODE_NONE,
} CANTestModeSel_TypeDef;

typedef enum
{
    CAN_RTR_DATA_FRAME = 0,
    CAN_RTR_REMOTE_FRAME = 1,
} CANRtrSel_TypeDef;

typedef enum
{
    CAN_IDE_STANDARD_FORMAT = 0,
    CAN_IDE_EXTEND_FORMAT = 1,
} CANIdeSel_TypeDef;

typedef enum
{
    CAN_EDL_STARDARD_FRAME = 0,
    CAN_EDL_FD_FRAME = 1,
} CANEdlSel_TypeDef;

typedef enum
{
    CAN_BRS_NO_SWITCH_BIT_TIMING = 0,
    CAN_BRS_SWITCH_BIT_TIMING = 1,
} CANBrsSel_TypeDef;

typedef enum
{
    CAN_INVALID_DATA_FRAME, /* invalide data frame */
    CAN_STD_DATA_FRAME,     /* standard data frame */
    CAN_EXT_DATA_FRAME,     /* extend data frame */
    CAN_STD_REMOTE_FRAME,   /* standard remote frame */
    CAN_EXT_REMOTE_FRAME,   /* extend remote frame */
    CAN_FD_STD_DATA_FRAME,  /* FD standard data frame */
    CAN_FD_EXT_DATA_FRAME,  /* FD extend data frame */
} CANDataFrameSel_TypeDef;

typedef enum
{
    CAN_NO_ERR = 0,         /* no error */
    CAN_MSG_ID_ERR = 1,     /* can message id error */
    CAN_ID_ERR = 2,         /* can frame id error */
    CAN_DATA_LEN_ERR = 3,   /* can frame data length error */
    CAN_TYPE_ERR = 4,       /* can frame type error */
    CAN_RAM_STATE_ERR = 5,  /* can frame ram status error */
    CAN_TIMEOUT_ERR = 6,    /* can timeout error */
} CANError_TypeDef;

typedef struct
{
    uint8_t msg_buf_id;
    uint8_t frame_brs_bit;
    uint8_t auto_reply_bit;
    CANDataFrameSel_TypeDef frame_type;
    uint16_t standard_frame_id;
    uint32_t extend_frame_id;
} CANTxFrame_TypeDef;

typedef struct
{
    uint8_t msg_buf_id;
    uint8_t rx_dma_en;
    uint8_t frame_rtr_mask; /* can frame RTR mask, 1 means don't care, 0 means the bit should match */
    uint8_t frame_ide_mask; /* can frame IDE mask, 1 means don't care, 0 means the bit should match */
    uint32_t frame_id_mask; /* can frame ID mask, 1 means the ID bit in CAN_RAM_ARB don't care, 0 means the bit should match. */
    uint8_t frame_rtr_bit;  /* can frame RTR bit, determine DATA or REMOTE frame */
    uint8_t frame_ide_bit;  /* can frame IDE bit, determine standard or extend format */
    uint8_t auto_reply_bit;
    uint16_t standard_frame_id;
    uint32_t extend_frame_id;
} CANRxFrame_TypeDef;

typedef struct
{
    uint8_t rtr_mask;
    uint8_t ide_mask;
    uint8_t id_mask;
    uint8_t esi_bit;
    uint8_t auto_reply_bit;
    uint8_t rxtx_bit;
    uint8_t rx_lost_bit;
    uint8_t data_length;
    uint8_t rx_dma_en;
    CANBrsSel_TypeDef brs_bit;
    CANEdlSel_TypeDef edl_bit;
    CANRtrSel_TypeDef rtr_bit;
    CANIdeSel_TypeDef ide_bit;
    uint16_t rx_timestamp;
    uint16_t standard_frame_id;
    uint32_t extend_frame_id;
} CANMsgBufInfo_TypeDef;

typedef struct
{
    uint8_t fifo_msg_lvl;
    uint8_t fifo_msg_overflow;
    uint8_t fifo_msg_empty;
    uint8_t fifo_msg_full;
} CANFifoStatus_TypeDef;

typedef struct
{
    CAN_0x340_TYPE_TypeDef can_ram_arb;
    CAN_0x348_TYPE_TypeDef can_ram_cs;
    uint8_t rx_dma_data[CAN_FD_DATA_MAX_LEN];
} CANRxDmaData_TypeDef;
/** End of CAN_Exported_Types
  * \}
  */
/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup CAN_Exported_Functions CAN Exported Functions
  * \brief
  * \{
  */
/**
 * \brief   Deinitializes the CAN peripheral registers to their default values.
 * \param[in] CANx: selected CAN peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void can_driver_init(void)
 * {
 *     CAN_DeInit(CAN0);
 * }
 * \endcode
 */
void CAN_DeInit(CAN_TypeDef *CANx);
/**
 * \brief   Initializes the CAN peripheral according to the specified
 *          parameters in the CAN_InitStruct
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] CAN_InitStruct: Pointer to a CAN_InitTypeDef structure that
 *            contains the configuration information for the specified CAN peripheral
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void can_driver_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CAN0, APBPeriph_CAN0_CLOCK, ENABLE);
 *
 *     CAN_InitTypeDef init_struct;
 *
 *     CAN_StructInit(&init_struct);
 *     init_struct.CAN_AutoReTxEn = DISABLE;
 *     init_struct.CAN_BitTiming.can_brp = 7;
 *     init_struct.CAN_BitTiming.can_sjw = 3;
 *     init_struct.CAN_BitTiming.can_tseg1 = 4;
 *     init_struct.CAN_BitTiming.can_tseg2 = 3;
 *
 *     init_struct.CAN_FdEn = ENABLE;
 *     init_struct.CAN_FdBitTiming.can_fd_brp = 4;
 *     init_struct.CAN_FdBitTiming.can_fd_sjw = 3;
 *     init_struct.CAN_FdBitTiming.can_fd_tseg1 = 4;
 *     init_struct.CAN_FdBitTiming.can_fd_tseg2 = 4;
 *     init_struct.CAN_FdSspCal.can_fd_ssp_dco = 0;
 *     init_struct.CAN_FdSspCal.can_fd_ssp_min = 0;
 *     init_struct.CAN_FdSspCal.can_fd_ssp = 0;
 *
 *     CAN_Init(CAN0, &init_struct);
 *     CAN_Cmd(CAN0, ENABLE);
 * }
 * \endcode
 */
void CAN_Init(CAN_TypeDef *CANx, CAN_InitTypeDef *CAN_InitStruct);
/**
 * \brief   Fills each CAN_InitStruct member with its default value.
 * \param[in] CAN_InitStruct: Pointer to an CAN_InitTypeDef structure which will be initialized.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void can_driver_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CAN, APBPeriph_CAN_CLOCK, ENABLE);
 *
 *     CAN_InitTypeDef init_struct;
 *
 *     CAN_StructInit(&init_struct);
 *     init_struct.CAN_AutoReTxEn = DISABLE;
 *     init_struct.CAN_BitTiming.can_brp = 7;
 *     init_struct.CAN_BitTiming.can_sjw = 3;
 *     init_struct.CAN_BitTiming.can_tseg1 = 4;
 *     init_struct.CAN_BitTiming.can_tseg2 = 3;
 *
 *     init_struct.CAN_FdEn = ENABLE;
 *     init_struct.CAN_FdBitTiming.can_fd_brp = 4;
 *     init_struct.CAN_FdBitTiming.can_fd_sjw = 3;
 *     init_struct.CAN_FdBitTiming.can_fd_tseg1 = 4;
 *     init_struct.CAN_FdBitTiming.can_fd_tseg2 = 4;
 *     init_struct.CAN_FdSspCal.can_fd_ssp_dco = 0;
 *     init_struct.CAN_FdSspCal.can_fd_ssp_min = 0;
 *     init_struct.CAN_FdSspCal.can_fd_ssp = 0;
 *
 *     CAN_Init(&init_struct);
 *     CAN_Cmd(ENABLE);
 * }
 * \endcode
 */
void CAN_StructInit(CAN_InitTypeDef *CAN_InitStruct);
/**
 * \brief   Enable or disable the selected CAN mode.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] NewState: New state of the operation mode.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void can_driver_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CAN0, APBPeriph_CAN0_CLOCK, ENABLE);
 *
 *     CAN_InitTypeDef init_struct;
 *
 *     CAN_StructInit(&init_struct);
 *     init_struct.CAN_AutoReTxEn = DISABLE;
 *     init_struct.CAN_BitTiming.can_brp = 7;
 *     init_struct.CAN_BitTiming.can_sjw = 3;
 *     init_struct.CAN_BitTiming.can_tseg1 = 4;
 *     init_struct.CAN_BitTiming.can_tseg2 = 3;
 *
 *     init_struct.CAN_FdEn = ENABLE;
 *     init_struct.CAN_FdBitTiming.can_fd_brp = 4;
 *     init_struct.CAN_FdBitTiming.can_fd_sjw = 3;
 *     init_struct.CAN_FdBitTiming.can_fd_tseg1 = 4;
 *     init_struct.CAN_FdBitTiming.can_fd_tseg2 = 4;
 *     init_struct.CAN_FdSspCal.can_fd_ssp_dco = 0;
 *     init_struct.CAN_FdSspCal.can_fd_ssp_min = 0;
 *     init_struct.CAN_FdSspCal.can_fd_ssp = 0;
 *
 *     CAN_Init(CAN0, &init_struct);
 *     CAN_Cmd(CAN0, ENABLE);
 * }
 * \endcode
 */
void CAN_Cmd(CAN_TypeDef *CANx, FunctionalState NewState);
/**
 * \brief   Enable or disable the specified CAN interrupt source.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] CAN_INT: Specifies the CAN interrupt source to be enable or disable.
 *      This parameter can be the following values:
 *      \arg CAN_RAM_MOVE_DONE_INT: data move from register to CAN IP internal RAM finished interupt.
 *      \arg CAN_BUS_OFF_INT: bus off interrupt.
 *      \arg CAN_WAKE_UP_INT: wakeup interrupt.
 *      \arg CAN_ERROR_INT: error interrupt.
 *      \arg CAN_RX_INT: RX interrupt.
 *      \arg CAN_TX_INT: TX interrupt.
 * \param[in]  NewState: New state of the specified CAN interrupt.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void can_driver_init(void)
 * {
 *     CAN_INTConfig(CAN0, (CAN_RAM_MOVE_DONE_INT | CAN_BUS_OFF_INT | CAN_WAKE_UP_INT |
 *                      CAN_ERROR_INT | CAN_RX_INT | CAN_TX_INT), ENABLE);
 * }
 * \endcode
 */
void CAN_INTConfig(CAN_TypeDef *CANx, uint32_t CAN_INT, FunctionalState newState);
#if (CAN_SUPPORT_INT_MSK_STS == 1)
/**
 * \brief   Mask or unmask the specified CAN interrupt source.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] CAN_INT: Specifies the CAN interrupt source to be enable or disable.
 *      This parameter can be the following values:
 *      \arg CAN_RAM_MOVE_DONE_INT: data move from register to CAN IP internal RAM finished interupt.
 *      \arg CAN_BUS_OFF_INT: bus off interrupt.
 *      \arg CAN_WAKE_UP_INT: wakeup interrupt.
 *      \arg CAN_ERROR_INT: error interrupt.
 *      \arg CAN_RX_INT: RX interrupt.
 *      \arg CAN_TX_INT: TX interrupt.
 * \param[in]  NewState: New state of the specified CAN interrupt.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void can_driver_init(void)
 * {
 *     CAN_MaskINTConfig(CAN0, (CAN_RAM_MOVE_DONE_INT | CAN_BUS_OFF_INT | CAN_WAKE_UP_INT |
 *                      CAN_ERROR_INT | CAN_RX_INT | CAN_TX_INT), ENABLE);
 * }
 * \endcode
 */

void CAN_MaskINTConfig(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG, FunctionalState NewState);
#endif
/**
 * \brief   Get the specified CAN interrupt raw status.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] CAN_INT: the specified CAN interrupt.
 *     This parameter can be one of the following values:
 *     \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
 *     \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
 *     \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
 *     \arg CAN_ERROR_INT_FLAG: error interrupt flag.
 *     \arg CAN_RX_INT_FLAG: RX interrupt flag.
 *     \arg CAN_TX_INT_FLAG: Clear TX interrupt flag.
 * \return The new state of CAN_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void CAN_Handler(void)
 * {
 *     if (SET == CAN_GetINTRawStatus(CAN0, CAN_RAM_MOVE_DONE_INT_FLAG))
 *  {
 *      DBG_DIRECT("[CAN] CAN_Handler CAN_RAM_MOVE_DONE_INT_FLAG");
 *      CAN_ClearINTPendingBit(CAN0, CAN_RAM_MOVE_DONE_INT_FLAG);
 *  }
 * }
 * \endcode
 */
ITStatus CAN_GetINTRawStatus(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG);
/**
 * \brief   Get the specified CAN interrupt status.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] CAN_INT: the specified CAN interrupt.
 *     This parameter can be one of the following values:
 *     \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
 *     \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
 *     \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
 *     \arg CAN_ERROR_INT_FLAG: error interrupt flag.
 *     \arg CAN_RX_INT_FLAG: RX interrupt flag.
 *     \arg CAN_TX_INT_FLAG: Clear TX interrupt flag.
 * \return The new state of CAN_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void CAN_Handler(void)
 * {
 *     if (SET == CAN_GetINTStatus(CAN0, CAN_RAM_MOVE_DONE_INT_FLAG))
 *  {
 *      DBG_DIRECT("[CAN] CAN_Handler CAN_RAM_MOVE_DONE_INT_FLAG");
 *      CAN_ClearINTPendingBit(CAN0, CAN_RAM_MOVE_DONE_INT_FLAG);
 *  }
 * }
 * \endcode
 */
ITStatus CAN_GetINTStatus(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG);
/**
 * \brief  Clear the CAN interrupt pending bit.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] CAN_INT_FLAG: Specifies the interrupt pending bit to clear.
 *      This parameter can be any combination of the following values:
 *      \arg CAN_RAM_MOVE_DONE_INT_FLAG: ram move done interrupt flag.
 *      \arg CAN_BUS_OFF_INT_FLAG: bus off interrupt flag.
 *      \arg CAN_WAKE_UP_INT_FLAG: wakeup interrupt flag.
 *      \arg CAN_ERROR_INT_FLAG: error interrupt flag.
 *      \arg CAN_RX_INT_FLAG: RX interrupt flag.
 *      \arg CAN_TX_INT_FLAG: Clear TX interrupt flag.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void CAN_Handler(void)
 * {
 *     if (SET == CAN_GetINTStatus(CAN0, CAN_RAM_MOVE_DONE_INT_FLAG))
 *     {
 *      DBG_DIRECT("[CAN] CAN_Handler CAN_RAM_MOVE_DONE_INT_FLAG");
 *      CAN_ClearINTPendingBit(CAN0, CAN_RAM_MOVE_DONE_INT_FLAG);
 *     }
 * }
 * \endcode
 */
void CAN_ClearINTPendingBit(CAN_TypeDef *CANx, uint32_t CAN_INT_FLAG);
/**
 * \brief  Gets the specified CAN error status.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  CAN_ERR_STAT: the specified CAN error.
 *     This parameter can be one of the following values:
 *     \arg CAN_ERROR_RX: can rx error flag
 *     \arg CAN_ERROR_TX: can tx error flag
 *     \arg CAN_ERROR_ACK: latest error is ack error
 *     \arg CAN_ERROR_STUFF: latest error is stuff error
 *     \arg CAN_ERROR_CRC: latest error is crc error
 *     \arg CAN_ERROR_FORM: latest error is form error
 *     \arg CAN_ERROR_BIT1: latest error is bit1 error, tx=1 but rx=0
 *     \arg CAN_ERROR_BIT0: latest error is bit0 error, tx=0 but rx=1
 * \return The new state of CAN_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void CAN_Handler(void)
 * {
 *     if (SET == CAN_GetErrorStatus(CAN0, CAN_ERROR_TX))
 *     {
 *          DBG_DIRECT("[CAN] CAN_ERROR_TX");
 *          CAN_CLearErrorStatus(CAN0, CAN_ERROR_TX);
 *     }
 * }
 * \endcode
 */
FlagStatus CAN_GetErrorStatus(CAN_TypeDef *CANx, uint32_t CAN_ERR_STAT);
/**
 * \brief  Clears the specified CAN error status.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  CAN_ERR_STAT: the specified CAN error.
 *     This parameter can be one of the following values:
 *     \arg CAN_ERROR_RX: can rx error flag
 *     \arg CAN_ERROR_TX: can tx error flag
 *     \arg CAN_ERROR_ACK: latest error is ack error
 *     \arg CAN_ERROR_STUFF: latest error is stuff error
 *     \arg CAN_ERROR_CRC: latest error is crc error
 *     \arg CAN_ERROR_FORM: latest error is form error
 *     \arg CAN_ERROR_BIT1: latest error is bit1 error, tx=1 but rx=0
 *     \arg CAN_ERROR_BIT0: latest error is bit0 error, tx=0 but rx=1
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void CAN_Handler(void)
 * {
 *     if (SET == CAN_GetErrorStatus(CAN0, CAN_ERROR_TX))
 *     {
 *          DBG_DIRECT("[CAN] CAN_ERROR_TX");
 *          CAN_CLearErrorStatus(CAN0, CAN_ERROR_TX);
 *     }
 * }
 * \endcode
 */
void CAN_CLearErrorStatus(CAN_TypeDef *CANx, uint32_t CAN_ERR_STAT);
/**
 * \brief  Sets the CAN message buffer Tx mode.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  p_tx_frame_params: the CAN frame parameter.
 * \param[in]  p_frame_data: the specified CAN data.
 * \param[in]  data_len: the length of CAN data to be sent.
 * \return the state of set buffer.
 */
CANError_TypeDef CAN_SetMsgBufTxMode(CAN_TypeDef *CANx, CANTxFrame_TypeDef *p_tx_frame_params,
                                     uint8_t *p_frame_data,
                                     uint8_t data_len);
/**
 * \brief  Sets the CAN message buffer Rx mode.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  p_rx_frame_params: the CAN frame parameter.
 * \return the state of set buffer.
 */
CANError_TypeDef CAN_SetMsgBufRxMode(CAN_TypeDef *CANx, CANRxFrame_TypeDef *p_rx_frame_params);
/**
 * \brief  Gets message buffer infomation.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  msg_buf_id: message buffer id.
 * \param[in]  p_mb_info.
 * \return the state of set buffer.
 */
CANError_TypeDef CAN_GetMsgBufInfo(CAN_TypeDef *CANx, uint8_t msg_buf_id,
                                   CANMsgBufInfo_TypeDef *p_mb_info);
/**
 * \brief  Gets RAM data.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  data_len: the length of RAM data.
 * \param[in]  p_data.
 * \return the state of set buffer.
 */
CANError_TypeDef CAN_GetRamData(CAN_TypeDef *CANx, uint8_t data_len, uint8_t *p_data);
/**
 * \brief  Gets RAM data.
 * \param[in]  rtr_bit.
 * \param[in]  ide_bit.
 * \param[in]  edl_bit.
 * \return the frame of CAN data.
 */
CANDataFrameSel_TypeDef CAN_CheckFrameType(uint8_t rtr_bit, uint8_t ide_bit, uint8_t edl_bit);
/**
 * \brief  Config message buffer tx interrupt.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \param[in]  newState: the tx state of message buffer.
 * \return None.
 */
void CAN_MBTxINTConfig(CAN_TypeDef *CANx, uint8_t message_buffer_index, FunctionalState newState);
/**
 * \brief  Config message buffer rx interrupt.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \param[in]  newState: the rx state of message buffer.
 * \return None.
 */
void CAN_MBRxINTConfig(CAN_TypeDef *CANx, uint8_t message_buffer_index, FunctionalState newState);
/**
 * \brief  Gets CAN FIFO status.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  CAN_FifoStatus: the status of CAN FIFO.
 * \return None.
 */
void CAN_GetFifoStatus(CAN_TypeDef *CANx, CANFifoStatus_TypeDef *CAN_FifoStatus);
/**
 * \brief  Sets tx message trigger by timestamp timer.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  newState: the state of tx trigger.
 * \param[in]  trigger_timestamp_begin: end of trigger time.
 * \param[in]  close_offset: start of trigger time.
 * \return None.
 */
void CAN_TxTriggerConfig(CAN_TypeDef *CANx, FunctionalState newState,
                         uint16_t trigger_timestamp_begin,
                         uint16_t close_offset);
/**
 * \brief  Get can bus state.
 * \param[in] CANx: selected CAN peripheral.
 * \return The state of bus(CAN_BUS_STATE_ON or CAN_BUS_STATE_OFF).
 */
uint32_t CAN_GetBusState(CAN_TypeDef *CANx);
/**
 * \brief  Get message buffer ram state.
 * \param[in] CANx: selected CAN peripheral.
 * \return The state of message buffer ram(CAN_RAM_STATE_EXCHANGING or CAN_RAM_STATE_IDLE).
 */
uint32_t CAN_GetRamState(CAN_TypeDef *CANx);
/**
 * \brief  Get message buffer tx done flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer tx done.
 */
FlagStatus CAN_GetMBnTxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Clear message buffer tx done flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return none.
 */
void CAN_ClearMBnTxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Get message buffer tx error flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer tx error.
 */
FlagStatus CAN_GetMBnTxErrorFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Clear message buffer tx error flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return none.
 */
void CAN_ClearMBnTxErrorFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Get message buffer tx finish flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer tx finish.
 */
FlagStatus CAN_GetMBnStatusTxFinishFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Get message buffer tx request flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer tx request.
 */
FlagStatus CAN_GetMBnStatusTxReqFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Get message buffer rx done flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer rx done.
 */
FlagStatus CAN_GetMBnRxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Clear message buffer rx done flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return none.
 */
void CAN_ClearMBnRxDoneFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Get message buffer rx valid flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer rx valid.
 */
FlagStatus CAN_GetMBnStatusRxValidFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Get message buffer rx ready flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer rx ready.
 */
FlagStatus CAN_GetMBnStatusRxReadyFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Config can time stamp.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  newState: New state of the time stamp.
 * \return none.
 */
void CAN_TimeStampConfig(CAN_TypeDef *CANx, FunctionalState newState);
/**
 * \brief  Get time stamp count.
 * \param[in] CANx: selected CAN peripheral.
 * \return Time stamp count.
 */
uint16_t CAN_GetTimeStampCount(CAN_TypeDef *CANx);
/**
 * \brief  Get rx dma block size(word).
 * \param[in] CANx: selected CAN peripheral.
 * \return Rx dma block size.
 */
uint32_t CAN_GetRxDmaMsize(CAN_TypeDef *CANx);
/**
 * \brief  Get message buffer rx dma enable flag.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \return The flag of message buffer rx dma enable.
 */
FlagStatus CAN_GetMBnRxDmaEnFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index);
/**
 * \brief  Set message buffer rx dma enable.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  message_buffer_index: CAN message buffer index.
 * \param[in]  newState: New state of the message buffer rx dma enable.
 * \return none.
 */
void CAN_SetMBnRxDmaEnFlag(CAN_TypeDef *CANx, uint8_t message_buffer_index,
                           FunctionalState newState);

/**
 * \brief  Set message buffer rx dma enable.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in]  div: CAN clock div.
 * \return none.
 */
void CAN_ClkDivConfig(CAN_TypeDef *CANx, CANClkDIV_TypeDef div);

#if (CAN_SUPPORT_SLEEP_MODE == 1)
/**
 * \brief  Check CAN sleep state.
 * \param[in] CANx: selected CAN peripheral.
 * \return The flag of CAN sleep state.
 */
FlagStatus CAN_CheckSleepStatus(CAN_TypeDef *CANx);

/**
 * \brief  Enable or disable CAN lower power clock.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] newState: Enable or disable CAN lower power clock.
 * \param[in] div: Low power clock div.
 * \return none.
 */
void CAN_LowPowerClkCmd(CAN_TypeDef *CANx, FunctionalState newState, CANLowPowerClkDIV_TypeDef div);

/**
 * \brief  Config wake up pin fit function.
 * \param[in] CANx: selected CAN peripheral.
 * \param[in] newState: Enable or disable CAN wake up pin fit function.
 * \param[in] flt_length: Config wake up pin fit length.
 *                        minimu value is 1. The unit is low power clock period.
 * \return none.
 */
void CAN_SetWakeUpPinFltFunction(CAN_TypeDef *CANx, FunctionalState newState, uint8_t flt_length);
/**
 * \brief  Request CAN to sleep mode.
 * \param[in] CANx: selected CAN peripheral.
 * \return none.
 */
void CAN_RequestToSleepMode(CAN_TypeDef *CANx);

/**
 * \brief  Manual wake up CAN.
 * \param[in] CANx: selected CAN peripheral.
 * \return The flag of message buffer rx dma enable.
 */
void CAN_ManualWakeup(CAN_TypeDef *CANx);
#endif

/** End of CAN_Exported_Functions
  * \}
  */
/** End of CAN
  * \}
  */
#ifdef __cplusplus
}
#endif
#endif /* _RTL_CAN_H_ */
/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/
