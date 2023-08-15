/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_dsi.h
* @brief    This file provides all the I2C firmware functions.
* @details
* @author   howie wang
* @date     2021-08-17
* @version  v0.1
*********************************************************************************************************
*/

#ifndef __RTL_LCDC_DSI_H
#define __RTL_LCDC_DSI_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "rtl876x.h"

#define DSI_MODE_VIDEO      0
#define DSI_MODE_CMD        1


#define DSI_LP_GSW0P_DISABLE     0
#define DSI_LP_GSW0P_ENABLE      1
#define DSI_LP_GSW1P_DISABLE     0
#define DSI_LP_GSW1P_ENABLE      1
#define DSI_LP_GSW2P_DISABLE     0
#define DSI_LP_GSW2P_ENABLE      1
#define DSI_LP_GSR0P_DISABLE     0
#define DSI_LP_GSR0P_ENABLE      1
#define DSI_LP_GSR1P_DISABLE     0
#define DSI_LP_GSR1P_ENABLE      1
#define DSI_LP_GSR2P_DISABLE     0
#define DSI_LP_GSR2P_ENABLE      1
#define DSI_LP_GLW_DISABLE       0
#define DSI_LP_GLW_ENABLE        1
#define DSI_LP_DSW0P_DISABLE     0
#define DSI_LP_DSW0P_ENABLE      1
#define DSI_LP_DSW1P_DISABLE     0
#define DSI_LP_DSW1P_ENABLE      1
#define DSI_LP_DSR0P_DISABLE     0
#define DSI_LP_DSR0P_ENABLE      1
#define DSI_LP_DLW_DISABLE       0
#define DSI_LP_DLW_ENABLE        1
#define DSI_LP_MRDP_DISABLE      0
#define DSI_LP_MRDP_ENABLE       1

#define DSI_TE_ACKNOWLEDGE_DISABLE 0
#define DSI_TE_ACKNOWLEDGE_ENABLE  1
#define DSI_ACKNOWLEDGE_DISABLE   0
#define DSI_ACKNOWLEDGE_ENABLE    1

typedef struct
{
    __IO uint32_t VERSION;
    __IO uint32_t PWR_UP;
    __IO uint32_t CLKMGR_CFG;
    __IO uint32_t DPI_VCID;
    __IO uint32_t DPI_COLOR_CODING ;
    __IO uint32_t DPI_CFG_POL ;
    __IO uint32_t DPI_LP_CMD_TIM ;
    uint32_t RESERVED0[4];/*!< Reserved, 0x1c - 0x28 */
    __IO uint32_t PCKHDL_CFG ;
    __IO uint32_t GEN_VCID ;
    __IO uint32_t MODE_CFG ;
    __IO uint32_t VID_MODE_CFG ;
    __IO uint32_t VID_PKT_SIZE ;
    __IO uint32_t VID_NUM_CHUNKS ;
    __IO uint32_t VID_NULL_SIZE ;
    __IO uint32_t VID_HSA_TIME ;
    __IO uint32_t VID_HBP_TIME ;
    __IO uint32_t VID_HLINE_TIME ;
    __IO uint32_t VID_VSA_LINES ;
    __IO uint32_t VID_VBP_LINES ;
    __IO uint32_t VID_VFP_LINES ;
    __IO uint32_t VID_VACTIVE_LINES ;
    __IO uint32_t EDPI_CMD_SIZE ;
    __IO uint32_t CMD_MODE_CFG ;
    __IO uint32_t GEN_HDR ;
    __IO uint32_t GEN_PLD_DATA ;
    __IO uint32_t CMD_PKT_STATUS ;
    __IO uint32_t TO_CNT_CFG ;
    __IO uint32_t HS_RD_TO_CNT ;
    __IO uint32_t LP_RD_TO_CNT ;
    __IO uint32_t HS_WR_TO_CNT ;
    __IO uint32_t LP_WR_TO_CNT ;
    __IO uint32_t BTA_TO_CNT ;
    uint32_t RESERVED1;/*!< Reserved, 0x90 */
    __IO uint32_t LPCLK_CTRL ;
    __IO uint32_t PHY_TMR_LPCLK_CFG ;
    __IO uint32_t PHY_TMR_CFG ;
    __IO uint32_t PHY_RSTZ ;
    __IO uint32_t PHY_IF_CFG ;
    __IO uint32_t PHY_ULPS_CTRL ;
    __IO uint32_t PHY_TX_TRIGGERS ;
    __IO uint32_t PHY_STATUS ;
    __IO uint32_t PHY_TST_CTRL0 ;
    __IO uint32_t PHY_TST_CTRL1 ;
    __IO uint32_t INT_ST0 ;
    __IO uint32_t INT_ST1 ;
    __IO uint32_t INT_MSK0 ;
    __IO uint32_t INT_MSK1 ;
    __IO uint32_t PHY_CAL ;
    uint32_t RESERVED2[2];/*!< Reserved, 0xC0,0XD4 */
    __IO uint32_t INT_FORCE0 ;
    __IO uint32_t INT_FORCE1 ;
    __IO uint32_t AUTO_ULPS_MODE ;
    __IO uint32_t AUTO_ULPS_ENTRY_DELAY ;
    __IO uint32_t AUTO_ULPS_WAKEUP_TIME ;
    uint32_t RESERVED3;/*!< Reserved, 0xec */
    __IO uint32_t DSC_PARAMETER ;
    __IO uint32_t PHY_TMR_RD_CFG ;
    __IO uint32_t AUTO_ULPS_MIN_TIME ;
    __IO uint32_t PHY_MODE ;
    __IO uint32_t VID_SHADOW_CTRL ;
    uint32_t RESERVED4[2];/*!< Reserved, 0x104,0x108 */
    __IO uint32_t DPI_VCID_ACT ;
    __IO uint32_t DPI_COLOR_CODING_ACT ;
    uint32_t RESERVED5;/*!< Reserved, 0x114 */
    __IO uint32_t DPI_LP_CMD_TIM_ACT ;
    __IO uint32_t EDPI_TE_HW_CFG ;
    uint32_t RESERVED6[6];/*!< Reserved, 0x120-0x134 */
    __IO uint32_t VID_MODE_CFG_ACT ;
    __IO uint32_t VID_PKT_SIZE_ACT ;
    __IO uint32_t VID_NUM_CHUNKS_ACT ;
    __IO uint32_t VID_NULL_SIZE_ACT ;
    __IO uint32_t VID_HSA_TIME_ACT ;
    __IO uint32_t VID_HBP_TIME_ACT ;
    __IO uint32_t VID_HLINE_TIME_ACT ;
    __IO uint32_t VID_VSA_LINES_ACT ;
    __IO uint32_t VID_VBP_LINES_ACT ;
    __IO uint32_t VID_VFP_LINES_ACT ;
    __IO uint32_t VID_VACTIVE_LINES_ACT ;
    uint32_t RESERVED7;/*!< Reserved, 0x164 */
    __IO uint32_t VID_PKT_STATUS ;
    uint32_t RESERVED8[74]; /*!< Reserved, 0x190 - 0x2B4 */
} DSI_TypeDef;

typedef struct
{
    uint32_t phy_mode;
    uint32_t ColorCoding;
    uint32_t NumberOfLanes;
    uint32_t StopWaitTime;
    uint32_t InterruptMask0;
    uint32_t InterruptMask1;
    uint32_t HS_TX_CNT;                 //for trigger timeout error measured in TO_CLK_DIVISION
    uint32_t LP_RX_CNT;                 //for trigger timeout error measured in TO_CLK_DIVISION
    uint32_t TOCkdiv;
    uint32_t TXEscapeCkdiv;
    uint32_t ModeCfg;                   //config for video mode or cmd mode

    uint32_t PeripherCharacteristics;
    uint32_t GenericPacketVCID;
    uint32_t GenericTearVCID;
    uint32_t GenericReadBackVCID;

    uint32_t AutoHS2LPEnable;
    uint32_t TxRequestCLKEnable;

    uint32_t ClockLaneHS2LPTime;
    uint32_t ClockLaneLP2HSTime;
    uint32_t DataLaneHS2LPTime;
    uint32_t DataLaneLP2HSTime;
    uint32_t DataLaneMaxReadTime;
    uint32_t VirtualChannelID;


    uint8_t phy_shutdownz;
    uint8_t phy_testclr;
    uint8_t phy_testen;
    uint8_t phy_enableclk;
    uint8_t phy_forcepll;

    uint8_t phy_rstz;
    uint8_t phy_testclk;

    uint8_t phy_testdout;
    uint8_t phy_testin;

    uint8_t phy_auto_ulps;
    uint8_t phy_pll_off_ulps;
    uint8_t phy_pre_pll_off_req;
    uint8_t phy_auto_ulps_delay;
    uint8_t phy_auto_ulps_time;

    uint32_t ColorModePolarity;
    uint32_t ShutDownPolarity;
    uint32_t HSPolarity;
    uint32_t VSPolarity;
    uint32_t DEPolarity;
    uint32_t CommandSize;

    uint32_t LPMaxReadPacket;
    uint32_t LPGenReadTime;
    uint32_t HSGenReadTime;
    uint32_t LPGenWriteTime;
    uint32_t HSGenWriteTime;
    uint32_t BTATime;
    uint32_t AcknowledgeRequest;

    uint32_t LPGenShortWriteNoP;
    uint32_t LPGenShortWriteOneP;
    uint32_t LPGenShortWriteTwoP;
    uint32_t LPGenShortReadNoP;
    uint32_t LPGenShortReadOneP;
    uint32_t LPGenShortReadTwoP;
    uint32_t LPGenLongWrite;
    uint32_t LPDcsShortWriteNoP;
    uint32_t LPDcsShortWriteOneP;
    uint32_t LPDcsShortReadNoP;
    uint32_t LPDcsLongWrite;

} DSI_InitTypeDef;


#define VIDEO_NON_BURST_WITH_SYNC_PULSES    0
#define VIDEO_NON_BURST_WITH_SYNC_EVENTS    1
#define VIDEO_BURST_MODE_2    2
#define VIDEO_BURST_MODE_3    3

#define VIDEO_VPG_MODE_V_AND_H    0
#define VIDEO_VPG_MODE_ONLY_V     1

#define VIDEO_VPG_ORIENTATION_V    0
#define VIDEO_VPG_ORIENTATION_H    1


typedef struct
{
    uint32_t Mode;//for brust or none brust
    uint32_t LPCommandEnable;
    uint32_t FrameBTAAcknowledgeEnable;
    uint32_t LPHorizontalFrontPorchEnable;
    uint32_t LPHorizontalBackPorchEnable;
    uint32_t LPVerticalActiveEnable;
    uint32_t LPVerticalFrontPorchEnable;
    uint32_t LPVerticalBackPorchEnable;
    uint32_t LPVerticalSyncActiveEnable;

    uint32_t invact_lpcmd_size;
    uint32_t outvact_lpcmd_size;

    uint32_t PacketSize;
    uint32_t NumberOfChunks;               /*!< Number of chunks                                                   */
    uint32_t NullPacketSize;               /*!< Null packet size                                                   */

    uint32_t HorizontalSyncActive;         /*!< Horizontal synchronism active duration (in lane byte clock cycles) */
    uint32_t HorizontalBackPorch;          /*!< Horizontal back-porch duration (in lane byte clock cycles)         */
    uint32_t HorizontalLine;               /*!< Horizontal line duration (in lane byte clock cycles)               */
    uint32_t VerticalSyncActive;           /*!< Vertical synchronism active duration                               */
    uint32_t VerticalBackPorch;            /*!< Vertical back-porch duration                                       */
    uint32_t VerticalFrontPorch;           /*!< Vertical front-porch duration                                      */
    uint32_t VerticalActive;               /*!< Vertical active duration                                           */

    uint32_t VPGOrientation;
    uint32_t VPGMode;
    uint32_t VPGEnable;

} DSI_VidCfgTypeDef;

typedef struct
{
    uint32_t TEAcknowledgeRequest;

    uint32_t scan_line_parameter;//for te
    uint32_t hw_set_scan_line;//for te
    uint32_t hw_tear_effect_gen;
    uint32_t hw_tear_effect_on;

} DSI_AdaptedCmdCfgTypeDef;


#define PHY_FORCEPLL_ENABLE     1
#define PHY_FORCEPLL_DISABLE    0
#define PHY_CLK_ENABLE     1
#define PHY_CLK_DISABLE    0
#define PHY_RST_ENABLE     1
#define PHY_RST_DISABLE    0
#define PHY_SHUTDOWN_ENABLE     1
#define PHY_SHUTDOWN_DISABLE    0
typedef struct
{
    uint32_t phy_forepll;
    uint32_t phy_enableclk;
    uint32_t phy_rstz;
    uint32_t phy_shutdownz;

//    uint32_t ClockLaneHS2LPTime;
//    uint32_t ClockLaneLP2HSTime;
//    uint32_t DataLaneHS2LPTime;
//    uint32_t DataLaneLP2HSTime;
//    uint32_t DataLaneMaxReadTime;
//    uint32_t StopWaitTime;
} DSI_PhyCfgTypeDef;




#define DSI                            ((DSI_TypeDef           *) MIPI_REG_BASE)

/** @defgroup DSI_SHORT_WRITE_PKT_Data_Type DSI SHORT WRITE PKT Data Type
  * @{
  */
#define DSI_DCS_SHORT_PKT_WRITE_P0  0x00000005U /*!< DCS short write, no parameters      */
#define DSI_DCS_SHORT_PKT_WRITE_P1  0x00000015U /*!< DCS short write, one parameter      */
#define DSI_GEN_SHORT_PKT_WRITE_P0  0x00000003U /*!< Generic short write, no parameters  */
#define DSI_GEN_SHORT_PKT_WRITE_P1  0x00000013U /*!< Generic short write, one parameter  */
#define DSI_GEN_SHORT_PKT_WRITE_P2  0x00000023U /*!< Generic short write, two parameters */
/**
  * @}
  */

/** @defgroup DSI_LONG_WRITE_PKT_Data_Type DSI LONG WRITE PKT Data Type
  * @{
  */
#define DSI_DCS_LONG_PKT_WRITE      0x00000039U /*!< DCS long write     */
#define DSI_GEN_LONG_PKT_WRITE      0x00000029U /*!< Generic long write */
/**
  * @}
  */

/** @defgroup DSI_SHORT_READ_PKT_Data_Type DSI SHORT READ PKT Data Type
  * @{
  */
#define DSI_DCS_SHORT_PKT_READ      0x00000006U /*!< DCS short read                     */
#define DSI_GEN_SHORT_PKT_READ_P0   0x00000004U /*!< Generic short read, no parameters  */
#define DSI_GEN_SHORT_PKT_READ_P1   0x00000014U /*!< Generic short read, one parameter  */
#define DSI_GEN_SHORT_PKT_READ_P2   0x00000024U /*!< Generic short read, two parameters */
/**
  * @}
  */

void DSI_Init(DSI_TypeDef *DSIx,  DSI_InitTypeDef *DSI_Init);
void DSI_ConfigVideoMode(DSI_TypeDef *DSIx,  DSI_VidCfgTypeDef *VidCfg);


/***************todo 20230301******************/
#define DSI_MODE_VIDEO      0
#define DSI_MODE_CMD        1


#define DSI_LP_GSW0P_DISABLE     0
#define DSI_LP_GSW0P_ENABLE      1
#define DSI_LP_GSW1P_DISABLE     0
#define DSI_LP_GSW1P_ENABLE      1
#define DSI_LP_GSW2P_DISABLE     0
#define DSI_LP_GSW2P_ENABLE      1
#define DSI_LP_GSR0P_DISABLE     0
#define DSI_LP_GSR0P_ENABLE      1
#define DSI_LP_GSR1P_DISABLE     0
#define DSI_LP_GSR1P_ENABLE      1
#define DSI_LP_GSR2P_DISABLE     0
#define DSI_LP_GSR2P_ENABLE      1
#define DSI_LP_GLW_DISABLE       0
#define DSI_LP_GLW_ENABLE        1
#define DSI_LP_DSW0P_DISABLE     0
#define DSI_LP_DSW0P_ENABLE      1
#define DSI_LP_DSW1P_DISABLE     0
#define DSI_LP_DSW1P_ENABLE      1
#define DSI_LP_DSR0P_DISABLE     0
#define DSI_LP_DSR0P_ENABLE      1
#define DSI_LP_DLW_DISABLE       0
#define DSI_LP_DLW_ENABLE        1
#define DSI_LP_MRDP_DISABLE      0
#define DSI_LP_MRDP_ENABLE       1

#define DSI_TE_ACKNOWLEDGE_DISABLE 0
#define DSI_TE_ACKNOWLEDGE_ENABLE  1
#define DSI_ACKNOWLEDGE_DISABLE   0
#define DSI_ACKNOWLEDGE_ENABLE    1


#define DW_DSI_RESET                    0
#define DW_DSI_PWON                     1

#define DW_DSI_DPHY                     0
#define DW_DSI_CPHY                     1

#define DW_DSI_LANE0                    0
#define DW_DSI_LANE01                   1
#define DW_DSI_LANE012              2
#define DW_DSI_LANE0123             3

#define DW_DSI_MASK                     0
#define DW_DSI_UNMASK                   1

#define TRANS_HS                            0
#define TRANS_LP                            1

/* DSI_PWR_UP[0]:shutdownz*/
#define DSI_SHUTDOWNZ_POS       (0)
#define DSI_SHUTDOWNZ_MSK       (0x1 << DSI_SHUTDOWNZ_POS)
#define DSI_SHUTDOWNZ_CLR       (~DSI_SHUTDOWNZ_MSK)


/* Register: CLKMGR_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x08. Address: . */

/*DSI_CLKMGR_CFG[15:8]:to_clk_division*/
#define DSI_TO_CLK_DIVISION_POS     (8)
#define DSI_TO_CLK_DIVISION_MSK (0XFF << DSI_TO_CLK_DIVISION_POS)
#define DSI_TO_CLK_DIVISION_CLR (~DSI_TO_CLK_DIVISION_MSK)

/*DSI_CLKMGR_CFG[7:0]:tx_esc_clk_division*/
#define DSI_TX_ESC_CLK_DIVISION_POS (0)
#define DSI_TX_ESC_CLK_DIVISION_MSK (0XFF << DSI_TX_ESC_CLK_DIVISION_POS)
#define DSI_TX_ESC_CLK_DIVISION_CLR (~DSI_TX_ESC_CLK_DIVISION_MSK)


/* Register: DPI_VCID -------------------------------------------------------*/
/* Description: Control register. Offset: 0xC. Address: . */

/*DSI_DPI_VCID[1:0]:dpi_vcid*/
#define DSI_DPI_VCID_POS    (0)
#define DSI_DPI_VCID_MSK    (0X3 << DSI_DPI_VCID_POS)
#define DSI_DPI_VCID_CLR    (~DSI_DPI_VCID_MSK)


/* Register: DPI_COLOR_CODING -------------------------------------------------------*/
/* Description: Control register. Offset: 0x10. Address: . */

/*DSI_DPI_COLOR_CODING[8]:loosely18_en*/
#define DSI_LOOSELY18_EN_POS    (8)
#define DSI_LOOSELY18_EN_MSK    (0x1 << DSI_LOOSELY18_EN_POS)
#define DSI_LOOSELY18_EN_CLR    (~DSI_LOOSELY18_EN_MSK)

/*DSI_DPI_COLOR_CODING[3:0]:dpi_color_coding*/
#define DSI_DPI_COLOR_CODING_POS (0)
#define DSI_DPI_COLOR_CODING_MSK (0xF << DSI_DPI_COLOR_CODING_POS )
#define DSI_DPI_COLOR_CODING_CLR (~ DSI_DPI_COLOR_CODING_MSK)


/* Register: DPI_CFG_POL -------------------------------------------------------*/
/* Description: Control register. Offset: 0x14. Address: . */

/*DSI_DPI_CFG_POL[4]:colorm_active_low*/
#define DSI_COLORM_ACTIVE_LOW_POS   (4)
#define DSI_COLORM_ACTIVE_LOW_MSK   (0x1 << DSI_COLORM_ACTIVE_LOW_POS)
#define DSI_COLORM_ACTIVE_LOW_CLR   (~ DSI_COLORM_ACTIVE_LOW_MSK)

/*DSI_DPI_CFG_POL[3]:shutd_active_low*/
#define DSI_SHUTD_ACTIVE_LOW_POS    (3)
#define DSI_SHUTD_ACTIVE_LOW_MSK    (0x1 << DSI_SHUTD_ACTIVE_LOW_POS)
#define DSI_SHUTD_ACTIVE_LOW_CLR    (~ DSI_SHUTD_ACTIVE_LOW_MSK)

/*DSI_DPI_CFG_POL[2]:hsync_active_low*/
#define DSI_HSYNC_ACTIVE_LOW_POS    (2)
#define DSI_HSYNC_ACTIVE_LOW_MSK    (0x1    <<   DSI_HSYNC_ACTIVE_LOW_POS)
#define DSI_HSYNC_ACTIVE_LOW_CLR    (~ DSI_HSYNC_ACTIVE_LOW_MSK)

/*DSI_DPI_CFG_POL[1]:vsync_active_low*/
#define DSI_VSYNC_ACTIVE_LOW_POS    (1)
#define DSI_VSYNC_ACTIVE_LOW_MSK    (   0x1 << DSI_VSYNC_ACTIVE_LOW_POS )
#define DSI_VSYNC_ACTIVE_LOW_CLR    (   ~DSI_VSYNC_ACTIVE_LOW_MSK   )

/*DSI_DPI_CFG_POL[0]:dataen_active_low*/
#define DSI_DATAEN_ACTIVE_LOW_POS   (0)
#define DSI_DATAEN_ACTIVE_LOW_MSK   (   0x1 << DSI_DATAEN_ACTIVE_LOW_POS    )
#define DSI_DATAEN_ACTIVE_LOW_CLR   (   ~DSI_DATAEN_ACTIVE_LOW_MSK  )

/* Register: DPI_LP_CMD_TIM -------------------------------------------------------*/
/* Description: Control register. Offset: 0x18. Address: . */

/*DPI_LP_CMD_TIM[23:16]:outvact_lpcmd_time*/
#define DSI_OUTVACT_LPCMD_TIME_POS  (16)
#define DSI_OUTVACT_LPCMD_TIME_MSK  (   0xFF    << DSI_OUTVACT_LPCMD_TIME_POS   )
#define DSI_OUTVACT_LPCMD_TIME_CLR  (   ~DSI_OUTVACT_LPCMD_TIME_MSK )


/*DPI_LP_CMD_TIM[7:0]:invact_lpcmd_time*/
#define DSI_INVACT_LPCMD_TIME_POS   (0)
#define DSI_INVACT_LPCMD_TIME_MSK   (   0xFF    << DSI_INVACT_LPCMD_TIME_POS    )
#define DSI_INVACT_LPCMD_TIME_CLR   (   ~DSI_INVACT_LPCMD_TIME_MSK  )



/* Register: DBI_VCID -------------------------------------------------------*/
/* Description: Control register. Offset: 0x1C. Address: . */

/*DBI_VCID[1:0]:dbi_vcid*/
#define DSI_DBI_VCID_POS    (0)
#define DSI_DBI_VCID_MSK    (   0x3 <<  DSI_DBI_VCID_POS    )
#define DSI_DBI_VCID_CLR    (   ~DSI_DBI_VCID_MSK   )



/* Register: DBI_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x20. Address: . */

/*DBI_CFG[17:16]:lut_size_conf*/
#define DSI_LUT_SIZE_CONF_POS   (16)
#define DSI_LUT_SIZE_CONF_MSK   (   0x3 <<  DSI_LUT_SIZE_CONF_POS   )
#define DSI_LUT_SIZE_CONF_CLR   (   ~DSI_LUT_SIZE_CONF_MSK  )

/*DBI_CFG[11:8]:out_dbi_conf*/
#define DSI_OUT_DBI_CONF_POS    (8)
#define DSI_OUT_DBI_CONF_MSK    (   0xF <<  DSI_OUT_DBI_CONF_POS    )
#define DSI_OUT_DBI_CONF_CLR    (   ~ DSI_OUT_DBI_CONF_MSK  )

/*DBI_CFG[3:0]:in_dbi_conf*/
#define DSI_IN_DBI_CONF_POS (0)
#define DSI_IN_DBI_CONF_MSK (   0xF << DSI_IN_DBI_CONF_POS  )
#define DSI_IN_DBI_CONF_CLR (   ~DSI_IN_DBI_CONF_MSK    )



/* Register: DBI_PARTITIONING_EN -------------------------------------------------------*/
/* Description: Control register. Offset: 0x24. Address: . */

/*DBI_PARTITIONING_EN[0]:partitioning_en*/
#define DSI_PARTITIONING_EN_POS (0)
#define DSI_PARTITIONING_EN_MSK (   0x1 <<  DSI_PARTITIONING_EN_POS )
#define DSI_PARTITIONING_EN_CLR (   ~DSI_PARTITIONING_EN_MSK    )

/* Register: DBI_CMDSIZE -------------------------------------------------------*/
/* Description: Control register. Offset: 0x28. Address: . */

/*DBI_CMDSIZE[31:16]:allowed_cmd_size*/
#define DSI_ALLOWED_CMD_SIZE_POS    (16)
#define DSI_ALLOWED_CMD_SIZE_MSK    (0xFFFF << DSI_ALLOWED_CMD_SIZE_POS)
#define DSI_ALLOWED_CMD_SIZE_CLR    (~DSI_ALLOWED_CMD_SIZE_MSK)

/*DBI_CMDSIZE[15:0]:wr_cmd_size*/
#define DSI_WR_CMD_SIZE_POS (0)
#define DSI_WR_CMD_SIZE_MSK (0xFFFF << DSI_WR_CMD_SIZE_POS)
#define DSI_WR_CMD_SIZE_CLR (~DSI_WR_CMD_SIZE_MSK)


/* Register: PCKHDL_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x2c. Address: . */

/*PCKHDL_CFG[5]:eotp_tx_lp_en*/
#define DSI_EOTP_TX_LP_EN_POS   (5)
#define DSI_EOTP_TX_LP_EN_MSK   (0x1 << DSI_EOTP_TX_LP_EN_POS)
#define DSI_EOTP_TX_LP_EN_CLR   (~DSI_EOTP_TX_LP_EN_MSK)

/*PCKHDL_CFG[4]:crc_rx_en*/
#define DSI_CRC_RX_EN_POS   (4)
#define DSI_CRC_RX_EN_MSK   (0x1 << DSI_CRC_RX_EN_POS)
#define DSI_CRC_RX_EN_CLR   (~DSI_CRC_RX_EN_MSK)

/*PCKHDL_CFG[3]:ecc_rx_en*/
#define DSI_ECC_RX_EN_POS   (3)
#define DSI_ECC_RX_EN_MSK   (0x1 << DSI_ECC_RX_EN_POS)
#define DSI_ECC_RX_EN_CLR   (~DSI_ECC_RX_EN_MSK)

/*PCKHDL_CFG[2]:bta_en*/
#define DSI_BTA_EN_POS  (2)
#define DSI_BTA_EN_MSK  (0x1 << DSI_BTA_EN_POS)
#define DSI_BTA_EN_CLR  (~DSI_BTA_EN_MSK)

/*PCKHDL_CFG[1]:eotp_rx_en*/
#define DSI_EOTP_RX_EN_POS  (1)
#define DSI_EOTP_RX_EN_MSK  (0x1 << DSI_EOTP_RX_EN_POS)
#define DSI_EOTP_RX_EN_CLR  (~DSI_EOTP_RX_EN_MSK)

/*PCKHDL_CFG[0]:eotp_tx_en*/
#define DSI_EOTP_TX_EN_POS  (0)
#define DSI_EOTP_TX_EN_MSK  (0x1 << DSI_EOTP_TX_EN_POS)
#define DSI_EOTP_TX_EN_CLR  (~DSI_EOTP_TX_EN_MSK)


/* Register: GEN_VCID -------------------------------------------------------*/
/* Description: Control register. Offset: 0x30. Address: . */

/*GEN_VCID[17:16]:gen_vcid_tx_auto*/
#define DSI_GEN_VCID_TX_AUTO_POS    (16)
#define DSI_GEN_VCID_TX_AUTO_MSK    (0x3 << DSI_GEN_VCID_TX_AUTO_POS)
#define DSI_GEN_VCID_TX_AUTO_CLR    (~DSI_GEN_VCID_TX_AUTO_MSK)

/*GEN_VCID[9:8]:gen_vcid_tear_auto*/
#define DSI_GEN_VCID_TEAR_AUTO_POS  (8)
#define DSI_GEN_VCID_TEAR_AUTO_MSK  (0x3 << DSI_GEN_VCID_TEAR_AUTO_POS)
#define DSI_GEN_VCID_TEAR_AUTO_CLR  (~DSI_GEN_VCID_TEAR_AUTO_MSK)

/*GEN_VCID[1:0]:gen_vcid_rx*/
#define DSI_GEN_VCID_RX_POS (0)
#define DSI_GEN_VCID_RX_MSK (0x3 << DSI_GEN_VCID_RX_POS)
#define DSI_GEN_VCID_RX_CLR (~DSI_GEN_VCID_RX_MSK)


/* Register: MODE_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x34. Address: . */

/*MODE_CFG[0]:cmd_video_mode*/
#define DSI_CMD_VIDEO_MODE_POS  (0)
#define DSI_CMD_VIDEO_MODE_MSK  (0x1 << DSI_CMD_VIDEO_MODE_POS)
#define DSI_CMD_VIDEO_MODE_CLR  (~DSI_CMD_VIDEO_MODE_MSK)


/* Register: VID_MODE_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x38. Address: . */

/*VID_MODE_CFG[24]:vpg_orientation*/
#define DSI_VPG_ORIENTATION_POS (24)
#define DSI_VPG_ORIENTATION_MSK (0x1 << DSI_VPG_ORIENTATION_POS)
#define DSI_VPG_ORIENTATION_CLR (~DSI_VPG_ORIENTATION_MSK)

/*VID_MODE_CFG[20]:vpg_mode*/
#define DSI_VPG_MODE_POS    (20)
#define DSI_VPG_MODE_MSK    (0x1 << DSI_VPG_MODE_POS)
#define DSI_VPG_MODE_CLR    (~DSI_VPG_MODE_MSK)

/*VID_MODE_CFG[16]:vpg_en*/
#define DSI_VPG_EN_POS  (16)
#define DSI_VPG_EN_MSK  (0x1 << DSI_VPG_EN_POS)
#define DSI_VPG_EN_CLR  (~DSI_VPG_EN_MSK)

/*VID_MODE_CFG[15]:lp_cmd_en*/
#define DSI_LP_CMD_EN_POS   (15)
#define DSI_LP_CMD_EN_MSK   (0x1 << DSI_LP_CMD_EN_POS)
#define DSI_LP_CMD_EN_CLR   (~DSI_LP_CMD_EN_MSK)

/*VID_MODE_CFG[14]:frame_bta_ack_en*/
#define DSI_FRAME_BTA_ACK_EN_POS    (14)
#define DSI_FRAME_BTA_ACK_EN_MSK    (0x1 << DSI_FRAME_BTA_ACK_EN_POS)
#define DSI_FRAME_BTA_ACK_EN_CLR    (~DSI_FRAME_BTA_ACK_EN_MSK)

/*VID_MODE_CFG[13]:lp_hfp_en*/
#define DSI_LP_HFP_EN_POS   (13)
#define DSI_LP_HFP_EN_MSK   (0x1 << DSI_LP_HFP_EN_POS)
#define DSI_LP_HFP_EN_CLR   (~DSI_LP_HFP_EN_MSK)

/*VID_MODE_CFG[12]:lp_hbp_en*/
#define DSI_LP_HBP_EN_POS   (12)
#define DSI_LP_HBP_EN_MSK   (0x1 << DSI_LP_HBP_EN_POS)
#define DSI_LP_HBP_EN_CLR   (~DSI_LP_HBP_EN_MSK)

/*VID_MODE_CFG[11]:lp_vact_en*/
#define DSI_LP_VACT_EN_POS  (11)
#define DSI_LP_VACT_EN_MSK  (0x1 << DSI_LP_VACT_EN_POS)
#define DSI_LP_VACT_EN_CLR  (~DSI_LP_VACT_EN_MSK)

/*VID_MODE_CFG[10]:lp_vfp_en*/
#define DSI_LP_VFP_EN_POS   (10)
#define DSI_LP_VFP_EN_MSK   (0x1 << DSI_LP_VFP_EN_POS)
#define DSI_LP_VFP_EN_CLR   (~DSI_LP_VFP_EN_MSK)

/*VID_MODE_CFG[9]:lp_vbp_en*/
#define DSI_LP_VBP_EN_POS   (9)
#define DSI_LP_VBP_EN_MSK   (0x1 << DSI_LP_VBP_EN_POS)
#define DSI_LP_VBP_EN_CLR   (~DSI_LP_VBP_EN_MSK)

/*VID_MODE_CFG[8]:lp_vsa_en*/
#define DSI_LP_VSA_EN_POS   (8)
#define DSI_LP_VSA_EN_MSK   (0x1 << DSI_LP_VSA_EN_POS)
#define DSI_LP_VSA_EN_CLR   (~DSI_LP_VSA_EN_MSK)

/*VID_MODE_CFG[0]:vid_mode_type*/
#define DSI_VID_MODE_TYPE_POS   (0)
#define DSI_VID_MODE_TYPE_MSK   (0x1 << DSI_VID_MODE_TYPE_POS)
#define DSI_VID_MODE_TYPE_CLR   (~DSI_VID_MODE_TYPE_MSK)


/* Register: VID_PKT_SIZE -------------------------------------------------------*/
/* Description: Control register. Offset: 0x3C. Address: . */

/*VID_PKT_SIZE[14:0]:vid_pkt_size*/
#define DSI_VID_PKT_SIZE_POS    (0)
#define DSI_VID_PKT_SIZE_MSK    (0x7FFF << DSI_VID_PKT_SIZE_POS)
#define DSI_VID_PKT_SIZE_CLR    (~DSI_VID_PKT_SIZE_MSK)


/* Register: VID_NUM_CHUNKS -------------------------------------------------------*/
/* Description: Control register. Offset: 0x40. Address: . */

/*VID_NUM_CHUNKS[12:0]:vid_num_chunks*/
#define DSI_VID_NUM_CHUNKS_POS  (0)
#define DSI_VID_NUM_CHUNKS_MSK  (0x1FFF << DSI_VID_NUM_CHUNKS_POS)
#define DSI_VID_NUM_CHUNKS_CLR  (~DSI_VID_NUM_CHUNKS_MSK)


/* Register: VID_NULL_SIZE -------------------------------------------------------*/
/* Description: Control register. Offset: 0x44. Address: . */

/*VID_NULL_SIZE[12:0]:vid_null_size*/
#define DSI_VID_NULL_SIZE_POS   (0)
#define DSI_VID_NULL_SIZE_MSK   (0x1FFF << DSI_VID_NULL_SIZE_POS)
#define DSI_VID_NULL_SIZE_CLR   (~DSI_VID_NULL_SIZE_MSK)


/* Register: VID_HSA_TIME -------------------------------------------------------*/
/* Description: Control register. Offset: 0x48. Address: . */

/*VID_HSA_TIME[11:0]:vid_hsa_time*/
#define DSI_VID_HSA_TIME_POS    (0)
#define DSI_VID_HSA_TIME_MSK    (0xFFF << DSI_VID_HSA_TIME_POS)
#define DSI_VID_HSA_TIME_CLR    (~DSI_VID_HSA_TIME_MSK)


/* Register: VID_HBP_TIME -------------------------------------------------------*/
/* Description: Control register. Offset: 0x4C. Address: . */

/*VID_HBP_TIME[11:0]:vid_hbp_time*/
#define DSI_VID_HBP_TIME_POS    (0)
#define DSI_VID_HBP_TIME_MSK    (0xFFF << DSI_VID_HBP_TIME_POS)
#define DSI_VID_HBP_TIME_CLR    (~DSI_VID_HBP_TIME_MSK)


/* Register: VID_HLINE_TIME -------------------------------------------------------*/
/* Description: Control register. Offset: 0x50. Address: . */

/*VID_HLINE_TIME[14:0]:vid_hline_time*/
#define DSI_VID_HLINE_TIME_POS  (0)
#define DSI_VID_HLINE_TIME_MSK  (0x7FFF << DSI_VID_HLINE_TIME_POS)
#define DSI_VID_HLINE_TIME_CLR  (~DSI_VID_HLINE_TIME_MSK)


/* Register: VID_VSA_LINES -------------------------------------------------------*/
/* Description: Control register. Offset: 0x54. Address: . */

/*VID_VSA_LINES[9:0]:vsa_lines*/
#define DSI_VSA_LINES_POS   (0)
#define DSI_VSA_LINES_MSK   (0x3FF << DSI_VSA_LINES_POS)
#define DSI_VSA_LINES_CLR   (~DSI_VSA_LINES_MSK)


/* Register: VID_VBP_LINES -------------------------------------------------------*/
/* Description: Control register. Offset: 0x58. Address: . */

/*VID_VBP_LINES[9:0]:vbp_lines*/
#define DSI_VBP_LINES_POS   (0)
#define DSI_VBP_LINES_MSK   (0x3FF << DSI_VBP_LINES_POS)
#define DSI_VBP_LINES_CLR   (~DSI_VBP_LINES_MSK)


/* Register: VID_VFP_LINES -------------------------------------------------------*/
/* Description: Control register. Offset: 0x5C. Address: . */

/*VID_VFP_LINES[9:0]:vfp_lines*/
#define DSI_VFP_LINES_POS   (0)
#define DSI_VFP_LINES_MSK   (0x3FF << DSI_VFP_LINES_POS)
#define DSI_VFP_LINES_CLR   (~DSI_VFP_LINES_MSK)


/* Register: VID_VACTIVE_LINES -------------------------------------------------------*/
/* Description: Control register. Offset: 0x60. Address: . */

/*VID_VACTIVE_LINES[13:0]:v_active_lines*/
#define DSI_V_ACTIVE_LINES_POS  (0)
#define DSI_V_ACTIVE_LINES_MSK  (0x3FFF << DSI_V_ACTIVE_LINES_POS)
#define DSI_V_ACTIVE_LINES_CLR  (~DSI_V_ACTIVE_LINES_MSK)


/* Register: EDPI_CMD_SIZE -------------------------------------------------------*/
/* Description: Control register. Offset: 0x64. Address: . */

/*EDPI_CMD_SIZE[15:0]:edpi_allowed_cmd_size*/
#define DSI_EDPI_ALLOWED_CMD_SIZE_POS   (0)
#define DSI_EDPI_ALLOWED_CMD_SIZE_MSK   (0xFFFF << DSI_EDPI_ALLOWED_CMD_SIZE_POS)
#define DSI_EDPI_ALLOWED_CMD_SIZE_CLR   (~DSI_EDPI_ALLOWED_CMD_SIZE_MSK)


/* Register: CMD_MODE_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x68. Address: . */

/*CMD_MODE_CFG[24]:max_rd_pkt_size*/
#define DSI_MAX_RD_PKT_SIZE_POS (24)
#define DSI_MAX_RD_PKT_SIZE_MSK (0x1 << DSI_MAX_RD_PKT_SIZE_POS)
#define DSI_MAX_RD_PKT_SIZE_CLR (~DSI_MAX_RD_PKT_SIZE_MSK)

/*CMD_MODE_CFG[19]:dcs_lw_tx*/
#define DSI_DCS_LW_TX_POS   (19)
#define DSI_DCS_LW_TX_MSK   (0x1 << DSI_DCS_LW_TX_POS)
#define DSI_DCS_LW_TX_CLR   (~DSI_DCS_LW_TX_MSK)

/*CMD_MODE_CFG[18]:dcs_sr_0p_tx*/
#define DSI_DCS_SR_0P_TX_POS    (18)
#define DSI_DCS_SR_0P_TX_MSK    (0x1 << DSI_DCS_SR_0P_TX_POS)
#define DSI_DCS_SR_0P_TX_CLR    (~DSI_DCS_SR_0P_TX_MSK)

/*CMD_MODE_CFG[17]:dcs_sw_1p_tx*/
#define DSI_DCS_SW_1P_TX_POS    (17)
#define DSI_DCS_SW_1P_TX_MSK    (0x1 << DSI_DCS_SW_1P_TX_POS)
#define DSI_DCS_SW_1P_TX_CLR    (~DSI_DCS_SW_1P_TX_MSK)

/*CMD_MODE_CFG[16]:dcs_sw_0p_tx*/
#define DSI_DCS_SW_0P_TX_POS    (16)
#define DSI_DCS_SW_0P_TX_MSK    (0x1 << DSI_DCS_SW_0P_TX_POS)
#define DSI_DCS_SW_0P_TX_CLR    (~DSI_DCS_SW_0P_TX_MSK)

/*CMD_MODE_CFG[14]:gen_lw_tx*/
#define DSI_GEN_LW_TX_POS   (14)
#define DSI_GEN_LW_TX_MSK   (0x1 << DSI_GEN_LW_TX_POS)
#define DSI_GEN_LW_TX_CLR   (~DSI_GEN_LW_TX_MSK)

/*CMD_MODE_CFG[13]:gen_sr_2p_tx*/
#define DSI_GEN_SR_2P_TX_POS    (13)
#define DSI_GEN_SR_2P_TX_MSK    (0x1 << DSI_GEN_SR_2P_TX_POS)
#define DSI_GEN_SR_2P_TX_CLR    (~DSI_GEN_SR_2P_TX_MSK)

/*CMD_MODE_CFG[12]:gen_sr_1p_tx*/
#define DSI_GEN_SR_1P_TX_POS    (12)
#define DSI_GEN_SR_1P_TX_MSK    (0x1 << DSI_GEN_SR_1P_TX_POS)
#define DSI_GEN_SR_1P_TX_CLR    (~DSI_GEN_SR_1P_TX_MSK)

/*CMD_MODE_CFG[11]:gen_sr_0p_tx*/

#define DSI_GEN_SR_0P_TX_POS    (11)
#define DSI_GEN_SR_0P_TX_MSK    (0x1 << DSI_GEN_SR_0P_TX_POS)
#define DSI_GEN_SR_0P_TX_CLR    (~DSI_GEN_SR_0P_TX_MSK)

/*CMD_MODE_CFG[10]:gen_sw_2p_tx*/
#define DSI_GEN_SW_2P_TX_POS    (10)
#define DSI_GEN_SW_2P_TX_MSK    (0x1 << DSI_GEN_SW_2P_TX_POS)
#define DSI_GEN_SW_2P_TX_CLR    (~DSI_GEN_SW_2P_TX_MSK)

/*CMD_MODE_CFG[9]:gen_sw_1p_tx*/
#define DSI_GEN_SW_1P_TX_POS    (9)
#define DSI_GEN_SW_1P_TX_MSK    (0x1 << DSI_GEN_SW_1P_TX_POS)
#define DSI_GEN_SW_1P_TX_CLR    (~DSI_GEN_SW_1P_TX_MSK)

/*CMD_MODE_CFG[8]:gen_sw_0p_tx*/
#define DSI_GEN_SW_0P_TX_POS    (8)
#define DSI_GEN_SW_0P_TX_MSK    (0x1 << DSI_GEN_SW_0P_TX_POS)
#define DSI_GEN_SW_0P_TX_CLR    (~DSI_GEN_SW_0P_TX_MSK)

/*CMD_MODE_CFG[1]:ack_rqst_en*/
#define DSI_ACK_RQST_EN_POS (1)
#define DSI_ACK_RQST_EN_MSK (0x1 << DSI_ACK_RQST_EN_POS)
#define DSI_ACK_RQST_EN_CLR (~DSI_ACK_RQST_EN_MSK)

/*CMD_MODE_CFG[0]:tear_fx_en*/
#define DSI_TEAR_FX_EN_POS  (0)
#define DSI_TEAR_FX_EN_MSK  (0x1 << DSI_TEAR_FX_EN_POS)
#define DSI_TEAR_FX_EN_CLR  (~DSI_TEAR_FX_EN_MSK)


/* Register: GEN_HDR -------------------------------------------------------*/
/* Description: Control register. Offset: 0x6C. Address: . */

/*GEN_HDR[23:16]:gen_wc_msbyte*/
#define DSI_GEN_WC_MSBYTE_POS   (16)
#define DSI_GEN_WC_MSBYTE_MSK   (0xFF << DSI_GEN_WC_MSBYTE_POS)
#define DSI_GEN_WC_MSBYTE_CLR   (~DSI_GEN_WC_MSBYTE_MSK)

/*GEN_HDR[15:8]:gen_wc_lsbyte*/
#define DSI_GEN_WC_LSBYTE_POS   (8)
#define DSI_GEN_WC_LSBYTE_MSK   (0xFF << DSI_GEN_WC_LSBYTE_POS)
#define DSI_GEN_WC_LSBYTE_CLR   (~DSI_GEN_WC_LSBYTE_MSK)

/*GEN_HDR[7:6]:gen_vc*/
#define DSI_GEN_VC_POS  (6)
#define DSI_GEN_VC_MSK  (0x3 << DSI_GEN_VC_POS)
#define DSI_GEN_VC_CLR  (~DSI_GEN_VC_MSK)

/*GEN_HDR[5:0]:gen_dt*/
#define DSI_GEN_DT_POS  (0)
#define DSI_GEN_DT_MSK  (0x3F << DSI_GEN_DT_POS)
#define DSI_GEN_DT_CLR  (~DSI_GEN_DT_MSK)


/* Register: GEN_PLD_DATA -------------------------------------------------------*/
/* Description: Control register. Offset: 0x70. Address: . */

/*GEN_PLD_DATA[31:24]:gen_pld_b4*/
#define DSI_GEN_PLD_B4_POS  (24)
#define DSI_GEN_PLD_B4_MSK  (0xFF << DSI_GEN_PLD_B4_POS)
#define DSI_GEN_PLD_B4_CLR  (~DSI_GEN_PLD_B4_MSK)

/*GEN_PLD_DATA[23:16]:gen_pld_b3*/
#define DSI_GEN_PLD_B3_POS  (16)
#define DSI_GEN_PLD_B3_MSK  (0xFF << DSI_GEN_PLD_B3_POS)
#define DSI_GEN_PLD_B3_CLR  (~DSI_GEN_PLD_B3_MSK)

/*GEN_PLD_DATA[15:8]:gen_pld_b2*/
#define DSI_GEN_PLD_B2_POS  (8)
#define DSI_GEN_PLD_B2_MSK  (0xFF << DSI_GEN_PLD_B2_POS)
#define DSI_GEN_PLD_B2_CLR  (~DSI_GEN_PLD_B2_MSK)

/*GEN_PLD_DATA[7:0]:gen_pld_b1*/
#define DSI_GEN_PLD_B1_POS  (0)
#define DSI_GEN_PLD_B1_MSK  (0xFF << DSI_GEN_PLD_B1_POS)
#define DSI_GEN_PLD_B1_CLR  (~DSI_GEN_PLD_B1_MSK)


/* Register: CMD_PKT_STATUS -------------------------------------------------------*/
/* Description: Control register. Offset: 0x74. Address: . */

/*CMD_PKT_STATUS[27]:dbi_buff_pld_full*/
#define DSI_DBI_BUFF_PLD_FULL_POS   (27)
#define DSI_DBI_BUFF_PLD_FULL_MSK   (0x1 << DSI_DBI_BUFF_PLD_FULL_POS)
#define DSI_DBI_BUFF_PLD_FULL_CLR   (~DSI_DBI_BUFF_PLD_FULL_MSK)

/*CMD_PKT_STATUS[26]:dbi_buff_pld_empty*/
#define DSI_DBI_BUFF_PLD_EMPTY_POS  (26)
#define DSI_DBI_BUFF_PLD_EMPTY_MSK  (0x1 << DSI_DBI_BUFF_PLD_EMPTY_POS)
#define DSI_DBI_BUFF_PLD_EMPTY_CLR  (~DSI_DBI_BUFF_PLD_EMPTY_MSK)

/*CMD_PKT_STATUS[25]:dbi_buff_cmd_full*/
#define DSI_DBI_BUFF_CMD_FULL_POS   (25)
#define DSI_DBI_BUFF_CMD_FULL_MSK   (0x1 << DSI_DBI_BUFF_CMD_FULL_POS)
#define DSI_DBI_BUFF_CMD_FULL_CLR   (~DSI_DBI_BUFF_CMD_FULL_MSK)

/*CMD_PKT_STATUS[24]:dbi_buff_cmd_empty*/
#define DSI_DBI_BUFF_CMD_EMPTY_POS  (24)
#define DSI_DBI_BUFF_CMD_EMPTY_MSK  (0x1 << DSI_DBI_BUFF_CMD_EMPTY_POS)
#define DSI_DBI_BUFF_CMD_EMPTY_CLR  (~DSI_DBI_BUFF_CMD_EMPTY_MSK)

/*CMD_PKT_STATUS[19]:gen_buff_pld_full*/
#define DSI_GEN_BUFF_PLD_FULL_POS   (19)
#define DSI_GEN_BUFF_PLD_FULL_MSK   (0x1 << DSI_GEN_BUFF_PLD_FULL_POS)
#define DSI_GEN_BUFF_PLD_FULL_CLR   (~DSI_GEN_BUFF_PLD_FULL_MSK)

/*CMD_PKT_STATUS[18]:gen_buff_pld_empty*/
#define DSI_GEN_BUFF_PLD_EMPTY_POS  (18)
#define DSI_GEN_BUFF_PLD_EMPTY_MSK  (0x1 << DSI_GEN_BUFF_PLD_EMPTY_POS)
#define DSI_GEN_BUFF_PLD_EMPTY_CLR  (~DSI_GEN_BUFF_PLD_EMPTY_MSK)

/*CMD_PKT_STATUS[17]:gen_buff_cmd_full*/
#define DSI_GEN_BUFF_CMD_FULL_POS   (17)
#define DSI_GEN_BUFF_CMD_FULL_MSK   (0x1 << DSI_GEN_BUFF_CMD_FULL_POS)
#define DSI_GEN_BUFF_CMD_FULL_CLR   (~DSI_GEN_BUFF_CMD_FULL_MSK)

/*CMD_PKT_STATUS[16]:gen_buff_cmd_empty*/
#define DSI_GEN_BUFF_CMD_EMPTY_POS  (16)
#define DSI_GEN_BUFF_CMD_EMPTY_MSK  (0x1 << DSI_GEN_BUFF_CMD_EMPTY_POS)
#define DSI_GEN_BUFF_CMD_EMPTY_CLR  (~DSI_GEN_BUFF_CMD_EMPTY_MSK)

/*CMD_PKT_STATUS[14]:dbi_rd_cmd_busy*/
#define DSI_DBI_RD_CMD_BUSY_POS (14)
#define DSI_DBI_RD_CMD_BUSY_MSK (0x1 << DSI_DBI_RD_CMD_BUSY_POS)
#define DSI_DBI_RD_CMD_BUSY_CLR (~DSI_DBI_RD_CMD_BUSY_MSK)

/*CMD_PKT_STATUS[13]:dbi_pld_r_full*/
#define DSI_DBI_PLD_R_FULL_POS  (13)
#define DSI_DBI_PLD_R_FULL_MSK  (0x1 << DSI_DBI_PLD_R_FULL_POS)
#define DSI_DBI_PLD_R_FULL_CLR  (~DSI_DBI_PLD_R_FULL_MSK)

/*CMD_PKT_STATUS[12]:dbi_pld_r_empty*/
#define DSI_DBI_PLD_R_EMPTY_POS (12)
#define DSI_DBI_PLD_R_EMPTY_MSK (0x1 << DSI_DBI_PLD_R_EMPTY_POS)
#define DSI_DBI_PLD_R_EMPTY_CLR (~DSI_DBI_PLD_R_EMPTY_MSK)

/*CMD_PKT_STATUS[11]:dbi_pld_w_full*/
#define DSI_DBI_PLD_W_FULL_POS  (11)
#define DSI_DBI_PLD_W_FULL_MSK  (0x1 << DSI_DBI_PLD_W_FULL_POS)
#define DSI_DBI_PLD_W_FULL_CLR  (~DSI_DBI_PLD_W_FULL_MSK)

/*CMD_PKT_STATUS[10]:dbi_pld_w_empty*/
#define DSI_DBI_PLD_W_EMPTY_POS (10)
#define DSI_DBI_PLD_W_EMPTY_MSK (0x1 << DSI_DBI_PLD_W_EMPTY_POS)
#define DSI_DBI_PLD_W_EMPTY_CLR (~DSI_DBI_PLD_W_EMPTY_MSK)

/*CMD_PKT_STATUS[9]:dbi_cmd_full*/
#define DSI_DBI_CMD_FULL_POS    (9)
#define DSI_DBI_CMD_FULL_MSK    (0x1 << DSI_DBI_CMD_FULL_POS)
#define DSI_DBI_CMD_FULL_CLR    (~DSI_DBI_CMD_FULL_MSK)

/*CMD_PKT_STATUS[8]:dbi_cmd_empy*/
#define DSI_DBI_CMD_EMPY_POS    (8)
#define DSI_DBI_CMD_EMPY_MSK    (0x1 << DSI_DBI_CMD_EMPY_POS)
#define DSI_DBI_CMD_EMPY_CLR    (~DSI_DBI_CMD_EMPY_MSK)

/*CMD_PKT_STATUS[6]:gen_rd_cmd_busy*/
#define DSI_GEN_RD_CMD_BUSY_POS (6)
#define DSI_GEN_RD_CMD_BUSY_MSK (0x1 << DSI_GEN_RD_CMD_BUSY_POS)
#define DSI_GEN_RD_CMD_BUSY_CLR (~DSI_GEN_RD_CMD_BUSY_MSK)

/*CMD_PKT_STATUS[5]:gen_pld_r_full*/
#define DSI_GEN_PLD_R_FULL_POS  (5)
#define DSI_GEN_PLD_R_FULL_MSK  (0x1 << DSI_GEN_PLD_R_FULL_POS)
#define DSI_GEN_PLD_R_FULL_CLR  (~DSI_GEN_PLD_R_FULL_MSK)

/*CMD_PKT_STATUS[4]:gen_pld_r_empty*/
#define DSI_GEN_PLD_R_EMPTY_POS (4)
#define DSI_GEN_PLD_R_EMPTY_MSK (0x1 << DSI_GEN_PLD_R_EMPTY_POS)
#define DSI_GEN_PLD_R_EMPTY_CLR (~DSI_GEN_PLD_R_EMPTY_MSK)

/*CMD_PKT_STATUS[3]:gen_pld_w_full*/
#define DSI_GEN_PLD_W_FULL_POS  (3)
#define DSI_GEN_PLD_W_FULL_MSK  (0x1 << DSI_GEN_PLD_W_FULL_POS)
#define DSI_GEN_PLD_W_FULL_CLR  (~DSI_GEN_PLD_W_FULL_MSK)

/*CMD_PKT_STATUS[2]:gen_pld_w_empty*/
#define DSI_GEN_PLD_W_EMPTY_POS (2)
#define DSI_GEN_PLD_W_EMPTY_MSK (0x1 << DSI_GEN_PLD_W_EMPTY_POS)
#define DSI_GEN_PLD_W_EMPTY_CLR (~DSI_GEN_PLD_W_EMPTY_MSK)

/*CMD_PKT_STATUS[1]:gen_cmd_full*/
#define DSI_GEN_CMD_FULL_POS    (1)
#define DSI_GEN_CMD_FULL_MSK    (0x1 << DSI_GEN_CMD_FULL_POS)
#define DSI_GEN_CMD_FULL_CLR    (~DSI_GEN_CMD_FULL_MSK)

/*CMD_PKT_STATUS[0]:gen_cmd_empty*/
#define DSI_GEN_CMD_EMPTY_POS   (0)
#define DSI_GEN_CMD_EMPTY_MSK   (0x1 << DSI_GEN_CMD_EMPTY_POS)
#define DSI_GEN_CMD_EMPTY_CLR   (~DSI_GEN_CMD_EMPTY_MSK)


/* Register: TO_CNT_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x78. Address: . */

/*TO_CNT_CFG[31:16]:hstx_to_cnt*/
#define DSI_HSTX_TO_CNT_POS (16)
#define DSI_HSTX_TO_CNT_MSK (0xFFFF << DSI_HSTX_TO_CNT_POS)
#define DSI_HSTX_TO_CNT_CLR (~DSI_HSTX_TO_CNT_MSK)

/*TO_CNT_CFG[15:0]:lprx_to_cnt*/
#define DSI_LPRX_TO_CNT_POS (0)
#define DSI_LPRX_TO_CNT_MSK (0xFFFF << DSI_LPRX_TO_CNT_POS)
#define DSI_LPRX_TO_CNT_CLR (~DSI_LPRX_TO_CNT_MSK)


/* Register: HS_RD_TO_CNT -------------------------------------------------------*/
/* Description: Control register. Offset: 0x7C. Address: . */

/*HS_RD_TO_CNT[15:0]:hs_rd_to_cnt*/
#define DSI_HS_RD_TO_CNT_POS    (0)

#define DSI_HS_RD_TO_CNT_MSK    (0xFFFF << DSI_HS_RD_TO_CNT_POS)
#define DSI_HS_RD_TO_CNT_CLR    (~DSI_HS_RD_TO_CNT_MSK)


/* Register: LP_RD_TO_CNT -------------------------------------------------------*/
/* Description: Control register. Offset: 0x80. Address: . */

/*LP_RD_TO_CNT[15:0]:lp_rd_to_cnt*/
#define DSI_LP_RD_TO_CNT_POS    (0)
#define DSI_LP_RD_TO_CNT_MSK    (0xFFFF << DSI_LP_RD_TO_CNT_POS)
#define DSI_LP_RD_TO_CNT_CLR    (~DSI_LP_RD_TO_CNT_MSK)


/* Register: HS_WR_TO_CNT -------------------------------------------------------*/
/* Description: Control register. Offset: 0x84. Address: . */

/*HS_WR_TO_CNT[24]:presp_to_mode*/
#define DSI_PRESP_TO_MODE_POS   (24)
#define DSI_PRESP_TO_MODE_MSK   (0x1 << DSI_PRESP_TO_MODE_POS)
#define DSI_PRESP_TO_MODE_CLR   (~DSI_PRESP_TO_MODE_MSK)

/*HS_WR_TO_CNT[15:0]:hs_wr_to_cnt*/
#define DSI_HS_WR_TO_CNT_POS    (0)
#define DSI_HS_WR_TO_CNT_MSK    (0xFFFF << DSI_HS_WR_TO_CNT_POS)
#define DSI_HS_WR_TO_CNT_CLR    (~DSI_HS_WR_TO_CNT_MSK)


/* Register: LP_WR_TO_CNT -------------------------------------------------------*/
/* Description: Control register. Offset: 0x88. Address: . */

/*LP_WR_TO_CNT[15:0]:lp_wr_to_cnt*/
#define DSI_LP_WR_TO_CNT_POS    (0)
#define DSI_LP_WR_TO_CNT_MSK    (0xFFFF << DSI_LP_WR_TO_CNT_POS)
#define DSI_LP_WR_TO_CNT_CLR    (~DSI_LP_WR_TO_CNT_MSK)


/* Register: BTA_TO_CNT -------------------------------------------------------*/
/* Description: Control register. Offset: 0x8C. Address: . */

/*BTA_TO_CNT[15:0]:bta_to_cnt*/
#define DSI_BTA_TO_CNT_POS  (0)
#define DSI_BTA_TO_CNT_MSK  (0xFFFF << DSI_BTA_TO_CNT_POS)
#define DSI_BTA_TO_CNT_CLR  (~DSI_BTA_TO_CNT_MSK)


/* Register: SDF_3D -------------------------------------------------------*/
/* Description: Control register. Offset: 0x90. Address: . */

/*SDF_3D[16]:send_3d_cfg*/
#define DSI_SEND_3D_CFG_POS (16)
#define DSI_SEND_3D_CFG_MSK (0x1 << DSI_SEND_3D_CFG_POS)
#define DSI_SEND_3D_CFG_CLR (~DSI_SEND_3D_CFG_MSK)

/*SDF_3D[5]:right_first*/
#define DSI_RIGHT_FIRST_POS (5)
#define DSI_RIGHT_FIRST_MSK (0x1 << DSI_RIGHT_FIRST_POS)
#define DSI_RIGHT_FIRST_CLR (~DSI_RIGHT_FIRST_MSK)

/*SDF_3D[4]:second_vsync*/
#define DSI_SECOND_VSYNC_POS    (4)
#define DSI_SECOND_VSYNC_MSK    (0x1 << DSI_SECOND_VSYNC_POS)
#define DSI_SECOND_VSYNC_CLR    (~DSI_SECOND_VSYNC_MSK)

/*SDF_3D[3:2]:format_3d*/
#define DSI_FORMAT_3D_POS   (2)
#define DSI_FORMAT_3D_MSK   (0x3 << DSI_FORMAT_3D_POS)
#define DSI_FORMAT_3D_CLR   (~DSI_FORMAT_3D_MSK)

/*SDF_3D[1:0]:mode_3d*/
#define DSI_MODE_3D_POS (0)
#define DSI_MODE_3D_MSK (0x3 << DSI_MODE_3D_POS)
#define DSI_MODE_3D_CLR (~DSI_MODE_3D_MSK)


/* Register: LPCLK_CTRL -------------------------------------------------------*/
/* Description: Control register. Offset: 0x94. Address: . */

/*LPCLK_CTRL[1]:auto_clklane_ctrl*/
#define DSI_AUTO_CLKLANE_CTRL_POS   (1)
#define DSI_AUTO_CLKLANE_CTRL_MSK   (0x1 << DSI_AUTO_CLKLANE_CTRL_POS)
#define DSI_AUTO_CLKLANE_CTRL_CLR   (~DSI_AUTO_CLKLANE_CTRL_MSK)

/*LPCLK_CTRL[0]:phy_txrequestclkhs*/
#define DSI_PHY_TXREQUESTCLKHS_POS  (0)
#define DSI_PHY_TXREQUESTCLKHS_MSK  (0x1 << DSI_PHY_TXREQUESTCLKHS_POS)
#define DSI_PHY_TXREQUESTCLKHS_CLR  (~DSI_PHY_TXREQUESTCLKHS_MSK)


/* Register: PHY_TMR_LPCLK_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x98. Address: . */

/*PHY_TMR_LPCLK_CFG[25:16]:phy_clkhs2lp_time*/
#define DSI_PHY_CLKHS2LP_TIME_POS   (16)
#define DSI_PHY_CLKHS2LP_TIME_MSK   (0x3FF << DSI_PHY_CLKHS2LP_TIME_POS)
#define DSI_PHY_CLKHS2LP_TIME_CLR   (~DSI_PHY_CLKHS2LP_TIME_MSK)

/*PHY_TMR_LPCLK_CFG[9:0]:phy_clklp2hs_time*/
#define DSI_PHY_CLKLP2HS_TIME_POS   (0)
#define DSI_PHY_CLKLP2HS_TIME_MSK   (0x3FF << DSI_PHY_CLKLP2HS_TIME_POS)
#define DSI_PHY_CLKLP2HS_TIME_CLR   (~DSI_PHY_CLKLP2HS_TIME_MSK)


/* Register: PHY_TMR_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x9C. Address: . */

/*PHY_TMR_CFG[25:16]:phy_hs2lp_time*/
#define DSI_PHY_HS2LP_TIME_POS  (16)
#define DSI_PHY_HS2LP_TIME_MSK  (0x3FF << DSI_PHY_HS2LP_TIME_POS)
#define DSI_PHY_HS2LP_TIME_CLR  (~DSI_PHY_HS2LP_TIME_MSK)

/*PHY_TMR_CFG[9:0]:phy_lp2hs_time*/
#define DSI_PHY_LP2HS_TIME_POS  (0)
#define DSI_PHY_LP2HS_TIME_MSK  (0x3FF << DSI_PHY_LP2HS_TIME_POS)
#define DSI_PHY_LP2HS_TIME_CLR  (~DSI_PHY_LP2HS_TIME_MSK)


/* Register: PHY_RSTZ -------------------------------------------------------*/
/* Description: Control register. Offset: 0xA0. Address: . */

/*PHY_RSTZ[3]:phy_forcepll*/
#define DSI_PHY_FORCEPLL_POS    (3)
#define DSI_PHY_FORCEPLL_MSK    (0x1 << DSI_PHY_FORCEPLL_POS)
#define DSI_PHY_FORCEPLL_CLR    (~DSI_PHY_FORCEPLL_MSK)

/*PHY_RSTZ[2]:phy_enableclk*/
#define DSI_PHY_ENABLECLK_POS   (2)
#define DSI_PHY_ENABLECLK_MSK   (0x1 << DSI_PHY_ENABLECLK_POS)
#define DSI_PHY_ENABLECLK_CLR   (~DSI_PHY_ENABLECLK_MSK)

/*PHY_RSTZ[1]:phy_rstz*/
#define DSI_PHY_RSTZ_POS    (1)
#define DSI_PHY_RSTZ_MSK    (0x1 << DSI_PHY_RSTZ_POS)
#define DSI_PHY_RSTZ_CLR    (~DSI_PHY_RSTZ_MSK)

/*PHY_RSTZ[0]:phy_shutdownz*/
#define DSI_PHY_SHUTDOWNZ_POS   (0)
#define DSI_PHY_SHUTDOWNZ_MSK   (0x1 << DSI_PHY_SHUTDOWNZ_POS)
#define DSI_PHY_SHUTDOWNZ_CLR   (~DSI_PHY_SHUTDOWNZ_MSK)


/* Register: PHY_IF_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0xA4. Address: . */

/*PHY_IF_CFG[15:8]:phy_stop_wait_time*/
#define DSI_PHY_STOP_WAIT_TIME_POS  (8)
#define DSI_PHY_STOP_WAIT_TIME_MSK  (0xFF << DSI_PHY_STOP_WAIT_TIME_POS)
#define DSI_PHY_STOP_WAIT_TIME_CLR  (~DSI_PHY_STOP_WAIT_TIME_MSK)

/*PHY_IF_CFG[1:0]:n_lanes*/
#define DSI_N_LANES_POS (0)
#define DSI_N_LANES_MSK (0x3 << DSI_N_LANES_POS)
#define DSI_N_LANES_CLR (~DSI_N_LANES_MSK)


/* Register: PHY_ULPS_CTRL -------------------------------------------------------*/
/* Description: Control register. Offset: 0xA8. Address: . */

/*PHY_ULPS_CTRL[3]:phy_txexitulpslan*/
#define DSI_PHY_TXEXITULPSLAN_POS   (3)
#define DSI_PHY_TXEXITULPSLAN_MSK   (0x1 << DSI_PHY_TXEXITULPSLAN_POS)
#define DSI_PHY_TXEXITULPSLAN_CLR   (~DSI_PHY_TXEXITULPSLAN_MSK)

/*PHY_ULPS_CTRL[2]:phy_txrequlpslan*/
#define DSI_PHY_TXREQULPSLAN_POS    (2)
#define DSI_PHY_TXREQULPSLAN_MSK    (0x1 << DSI_PHY_TXREQULPSLAN_POS)
#define DSI_PHY_TXREQULPSLAN_CLR    (~DSI_PHY_TXREQULPSLAN_MSK)

/*PHY_ULPS_CTRL[1]:phy_txexitulpsclk*/
#define DSI_PHY_TXEXITULPSCLK_POS   (1)
#define DSI_PHY_TXEXITULPSCLK_MSK   (0x1 << DSI_PHY_TXEXITULPSCLK_POS)
#define DSI_PHY_TXEXITULPSCLK_CLR   (~DSI_PHY_TXEXITULPSCLK_MSK)

/*PHY_ULPS_CTRL[0]:phy_txrequlpsclk*/
#define DSI_PHY_TXREQULPSCLK_POS    (0)
#define DSI_PHY_TXREQULPSCLK_MSK    (0x1 << DSI_PHY_TXREQULPSCLK_POS)
#define DSI_PHY_TXREQULPSCLK_CLR    (~DSI_PHY_TXREQULPSCLK_MSK)


/* Register: PHY_TX_TRIGGERS -------------------------------------------------------*/
/* Description: Control register. Offset: 0xAC. Address: . */

/*PHY_TX_TRIGGERS[3:0]:phy_tx_triggers*/
#define DSI_PHY_TX_TRIGGERS_POS (0)
#define DSI_PHY_TX_TRIGGERS_MSK (0xF << DSI_PHY_TX_TRIGGERS_POS)
#define DSI_PHY_TX_TRIGGERS_CLR (~DSI_PHY_TX_TRIGGERS_MSK)


/* Register: PHY_STATUS -------------------------------------------------------*/
/* Description: Control register. Offset: 0xB0. Address: . */

/*PHY_STATUS[12]:phy_ulpsactivenot3lane*/
#define DSI_PHY_ULPSACTIVENOT3LANE_POS  (12)
#define DSI_PHY_ULPSACTIVENOT3LANE_MSK  (0x1 << DSI_PHY_ULPSACTIVENOT3LANE_POS)
#define DSI_PHY_ULPSACTIVENOT3LANE_CLR  (~DSI_PHY_ULPSACTIVENOT3LANE_MSK)

/*PHY_STATUS[11]:phy_stopstate3lane*/
#define DSI_PHY_STOPSTATE3LANE_POS  (11)
#define DSI_PHY_STOPSTATE3LANE_MSK  (0x1 << DSI_PHY_STOPSTATE3LANE_POS)
#define DSI_PHY_STOPSTATE3LANE_CLR  (~DSI_PHY_STOPSTATE3LANE_MSK)

/*PHY_STATUS[10]:phy_ulpsactivenot2lane*/
#define DSI_PHY_ULPSACTIVENOT2LANE_POS  (10)
#define DSI_PHY_ULPSACTIVENOT2LANE_MSK  (0x1 << DSI_PHY_ULPSACTIVENOT2LANE_POS)
#define DSI_PHY_ULPSACTIVENOT2LANE_CLR  (~DSI_PHY_ULPSACTIVENOT2LANE_MSK)

/*PHY_STATUS[9]:phy_stopstate2lane*/
#define DSI_PHY_STOPSTATE2LANE_POS  (9)
#define DSI_PHY_STOPSTATE2LANE_MSK  (0x1 << DSI_PHY_STOPSTATE2LANE_POS)
#define DSI_PHY_STOPSTATE2LANE_CLR  (~DSI_PHY_STOPSTATE2LANE_MSK)

/*PHY_STATUS[8]:phy_ulpsactivenot1lane*/
#define DSI_PHY_ULPSACTIVENOT1LANE_POS  (8)
#define DSI_PHY_ULPSACTIVENOT1LANE_MSK  (0x1 << DSI_PHY_ULPSACTIVENOT1LANE_POS)
#define DSI_PHY_ULPSACTIVENOT1LANE_CLR  (~DSI_PHY_ULPSACTIVENOT1LANE_MSK)

/*PHY_STATUS[7]:phy_stopstate1lane*/
#define DSI_PHY_STOPSTATE1LANE_POS  (7)
#define DSI_PHY_STOPSTATE1LANE_MSK  (0x1 << DSI_PHY_STOPSTATE1LANE_POS)
#define DSI_PHY_STOPSTATE1LANE_CLR  (~DSI_PHY_STOPSTATE1LANE_MSK)

/*PHY_STATUS[6]:phy_rxulpsesc0lane*/
#define DSI_PHY_RXULPSESC0LANE_POS  (6)
#define DSI_PHY_RXULPSESC0LANE_MSK  (0x1 << DSI_PHY_RXULPSESC0LANE_POS)
#define DSI_PHY_RXULPSESC0LANE_CLR  (~DSI_PHY_RXULPSESC0LANE_MSK)

/*PHY_STATUS[5]:phy_ulpsactivenot0lane*/
#define DSI_PHY_ULPSACTIVENOT0LANE_POS  (5)
#define DSI_PHY_ULPSACTIVENOT0LANE_MSK  (0x1 << DSI_PHY_ULPSACTIVENOT0LANE_POS)
#define DSI_PHY_ULPSACTIVENOT0LANE_CLR  (~DSI_PHY_ULPSACTIVENOT0LANE_MSK)

/*PHY_STATUS[4]:phy_stopstate0lane*/
#define DSI_PHY_STOPSTATE0LANE_POS  (4)
#define DSI_PHY_STOPSTATE0LANE_MSK  (0x1 << DSI_PHY_STOPSTATE0LANE_POS)
#define DSI_PHY_STOPSTATE0LANE_CLR  (~DSI_PHY_STOPSTATE0LANE_MSK)

/*PHY_STATUS[3]:phy_ulpsactivenotclk*/
#define DSI_PHY_ULPSACTIVENOTCLK_POS    (3)
#define DSI_PHY_ULPSACTIVENOTCLK_MSK    (0x1 << DSI_PHY_ULPSACTIVENOTCLK_POS)
#define DSI_PHY_ULPSACTIVENOTCLK_CLR    (~DSI_PHY_ULPSACTIVENOTCLK_MSK)

/*PHY_STATUS[2]:phy_stopstateclklane*/
#define DSI_PHY_STOPSTATECLKLANE_POS    (2)
#define DSI_PHY_STOPSTATECLKLANE_MSK    (0x1 << DSI_PHY_STOPSTATECLKLANE_POS)
#define DSI_PHY_STOPSTATECLKLANE_CLR    (~DSI_PHY_STOPSTATECLKLANE_MSK)

/*PHY_STATUS[1]:phy_direction*/
#define DSI_PHY_DIRECTION_POS   (1)
#define DSI_PHY_DIRECTION_MSK   (0x1 << DSI_PHY_DIRECTION_POS)
#define DSI_PHY_DIRECTION_CLR   (~DSI_PHY_DIRECTION_MSK)

/*PHY_STATUS[0]:phy_lock*/
#define DSI_PHY_LOCK_POS    (0)
#define DSI_PHY_LOCK_MSK    (0x1 << DSI_PHY_LOCK_POS)
#define DSI_PHY_LOCK_CLR    (~DSI_PHY_LOCK_MSK)


/* Register: PHY_TST_CTRL0 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xB4. Address: . */

/*PHY_TST_CTRL0[1]:phy_testclk*/
#define DSI_PHY_TESTCLK_POS (1)
#define DSI_PHY_TESTCLK_MSK (0x1 << DSI_PHY_TESTCLK_POS)
#define DSI_PHY_TESTCLK_CLR (~DSI_PHY_TESTCLK_MSK)

/*PHY_TST_CTRL0[0]:phy_testclr*/
#define DSI_PHY_TESTCLR_POS (0)
#define DSI_PHY_TESTCLR_MSK (0x1 << DSI_PHY_TESTCLR_POS)
#define DSI_PHY_TESTCLR_CLR (~DSI_PHY_TESTCLR_MSK)


/* Register: PHY_TST_CTRL1 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xB8. Address: . */

/*PHY_TST_CTRL1[16]:phy_testen*/
#define DSI_PHY_TESTEN_POS  (16)
#define DSI_PHY_TESTEN_MSK  (0x1 << DSI_PHY_TESTEN_POS)
#define DSI_PHY_TESTEN_CLR  (~DSI_PHY_TESTEN_MSK)

/*PHY_TST_CTRL1[15:8]:pht_testdout*/
#define DSI_PHT_TESTDOUT_POS    (8)
#define DSI_PHT_TESTDOUT_MSK    (0xFF << DSI_PHT_TESTDOUT_POS)
#define DSI_PHT_TESTDOUT_CLR    (~DSI_PHT_TESTDOUT_MSK)

/*PHY_TST_CTRL1[7:0]:phy_testdin*/
#define DSI_PHY_TESTDIN_POS (0)
#define DSI_PHY_TESTDIN_MSK (0xFF << DSI_PHY_TESTDIN_POS)
#define DSI_PHY_TESTDIN_CLR (~DSI_PHY_TESTDIN_MSK)


/* Register: INT_ST0 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xBC. Address: . */

/*INT_ST0[20]:dphy_errors_4*/
#define DSI_DPHY_ERRORS_4_POS   (20)
#define DSI_DPHY_ERRORS_4_MSK   (0x1 << DSI_DPHY_ERRORS_4_POS)
#define DSI_DPHY_ERRORS_4_CLR   (~DSI_DPHY_ERRORS_4_MSK)

/*INT_ST0[19]:dphy_errors_3*/
#define DSI_DPHY_ERRORS_3_POS   (19)
#define DSI_DPHY_ERRORS_3_MSK   (0x1 << DSI_DPHY_ERRORS_3_POS)
#define DSI_DPHY_ERRORS_3_CLR   (~DSI_DPHY_ERRORS_3_MSK)

/*INT_ST0[18]:dphy_errors_2*/
#define DSI_DPHY_ERRORS_2_POS   (18)
#define DSI_DPHY_ERRORS_2_MSK   (0x1 << DSI_DPHY_ERRORS_2_POS)
#define DSI_DPHY_ERRORS_2_CLR   (~DSI_DPHY_ERRORS_2_MSK)

/*INT_ST0[17]:dphy_errors_1*/
#define DSI_DPHY_ERRORS_1_POS   (17)
#define DSI_DPHY_ERRORS_1_MSK   (0x1 << DSI_DPHY_ERRORS_1_POS)
#define DSI_DPHY_ERRORS_1_CLR   (~DSI_DPHY_ERRORS_1_MSK)

/*INT_ST0[16]:dphy_errors_0*/
#define DSI_DPHY_ERRORS_0_POS   (16)
#define DSI_DPHY_ERRORS_0_MSK   (0x1 << DSI_DPHY_ERRORS_0_POS)
#define DSI_DPHY_ERRORS_0_CLR   (~DSI_DPHY_ERRORS_0_MSK)

/*INT_ST0[15]:ack_with_err_15*/
#define DSI_ACK_WITH_ERR_15_POS (15)
#define DSI_ACK_WITH_ERR_15_MSK (0x1 << DSI_ACK_WITH_ERR_15_POS)
#define DSI_ACK_WITH_ERR_15_CLR (~DSI_ACK_WITH_ERR_15_MSK)

/*INT_ST0[14]:ack_with_err_14*/
#define DSI_ACK_WITH_ERR_14_POS (14)
#define DSI_ACK_WITH_ERR_14_MSK (0x1 << DSI_ACK_WITH_ERR_14_POS)
#define DSI_ACK_WITH_ERR_14_CLR (~DSI_ACK_WITH_ERR_14_MSK)

/*INT_ST0[13]:ack_with_err_13*/
#define DSI_ACK_WITH_ERR_13_POS (13)
#define DSI_ACK_WITH_ERR_13_MSK (0x1 << DSI_ACK_WITH_ERR_13_POS)
#define DSI_ACK_WITH_ERR_13_CLR (~DSI_ACK_WITH_ERR_13_MSK)

/*INT_ST0[12]:ack_with_err_12*/
#define DSI_ACK_WITH_ERR_12_POS (12)
#define DSI_ACK_WITH_ERR_12_MSK (0x1 << DSI_ACK_WITH_ERR_12_POS)
#define DSI_ACK_WITH_ERR_12_CLR (~DSI_ACK_WITH_ERR_12_MSK)

/*INT_ST0[11]:ack_with_err_11*/
#define DSI_ACK_WITH_ERR_11_POS (11)
#define DSI_ACK_WITH_ERR_11_MSK (0x1 << DSI_ACK_WITH_ERR_11_POS)
#define DSI_ACK_WITH_ERR_11_CLR (~DSI_ACK_WITH_ERR_11_MSK)

/*INT_ST0[10]:ack_with_err_10*/
#define DSI_ACK_WITH_ERR_10_POS (10)
#define DSI_ACK_WITH_ERR_10_MSK (0x1 << DSI_ACK_WITH_ERR_10_POS)
#define DSI_ACK_WITH_ERR_10_CLR (~DSI_ACK_WITH_ERR_10_MSK)

/*INT_ST0[9]:ack_with_err_9*/
#define DSI_ACK_WITH_ERR_9_POS  (9)
#define DSI_ACK_WITH_ERR_9_MSK  (0x1 << DSI_ACK_WITH_ERR_9_POS)
#define DSI_ACK_WITH_ERR_9_CLR  (~DSI_ACK_WITH_ERR_9_MSK)

/*INT_ST0[8]:ack_with_err_8*/
#define DSI_ACK_WITH_ERR_8_POS  (8)
#define DSI_ACK_WITH_ERR_8_MSK  (0x1 << DSI_ACK_WITH_ERR_8_POS)
#define DSI_ACK_WITH_ERR_8_CLR  (~DSI_ACK_WITH_ERR_8_MSK)

/*INT_ST0[7]:ack_with_err_7*/
#define DSI_ACK_WITH_ERR_7_POS  (7)
#define DSI_ACK_WITH_ERR_7_MSK  (0x1 << DSI_ACK_WITH_ERR_7_POS)
#define DSI_ACK_WITH_ERR_7_CLR  (~DSI_ACK_WITH_ERR_7_MSK)

/*INT_ST0[6]:ack_with_err_6*/
#define DSI_ACK_WITH_ERR_6_POS  (6)
#define DSI_ACK_WITH_ERR_6_MSK  (0x1 << DSI_ACK_WITH_ERR_6_POS)
#define DSI_ACK_WITH_ERR_6_CLR  (~DSI_ACK_WITH_ERR_6_MSK)

/*INT_ST0[5]:ack_with_err_5*/
#define DSI_ACK_WITH_ERR_5_POS  (5)
#define DSI_ACK_WITH_ERR_5_MSK  (0x1 << DSI_ACK_WITH_ERR_5_POS)
#define DSI_ACK_WITH_ERR_5_CLR  (~DSI_ACK_WITH_ERR_5_MSK)

/*INT_ST0[4]:ack_with_err_4*/
#define DSI_ACK_WITH_ERR_4_POS  (4)
#define DSI_ACK_WITH_ERR_4_MSK  (0x1 << DSI_ACK_WITH_ERR_4_POS)
#define DSI_ACK_WITH_ERR_4_CLR  (~DSI_ACK_WITH_ERR_4_MSK)

/*INT_ST0[3]:ack_with_err_3*/
#define DSI_ACK_WITH_ERR_3_POS  (3)
#define DSI_ACK_WITH_ERR_3_MSK  (0x1 << DSI_ACK_WITH_ERR_3_POS)
#define DSI_ACK_WITH_ERR_3_CLR  (~DSI_ACK_WITH_ERR_3_MSK)

/*INT_ST0[2]:ack_with_err_2*/
#define DSI_ACK_WITH_ERR_2_POS  (2)
#define DSI_ACK_WITH_ERR_2_MSK  (0x1 << DSI_ACK_WITH_ERR_2_POS)
#define DSI_ACK_WITH_ERR_2_CLR  (~DSI_ACK_WITH_ERR_2_MSK)

/*INT_ST0[1]:ack_with_err_1*/
#define DSI_ACK_WITH_ERR_1_POS  (1)
#define DSI_ACK_WITH_ERR_1_MSK  (0x1 << DSI_ACK_WITH_ERR_1_POS)
#define DSI_ACK_WITH_ERR_1_CLR  (~DSI_ACK_WITH_ERR_1_MSK)

/*INT_ST0[0]:ack_with_err_0*/
#define DSI_ACK_WITH_ERR_0_POS  (0)
#define DSI_ACK_WITH_ERR_0_MSK  (0x1 << DSI_ACK_WITH_ERR_0_POS)
#define DSI_ACK_WITH_ERR_0_CLR  (~DSI_ACK_WITH_ERR_0_MSK)



/* Register: INT_ST1 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xC0. Address: . */

/*INT_ST1[20]:tear_request_err*/
#define DSI_TEAR_REQUEST_ERR_POS    (20)
#define DSI_TEAR_REQUEST_ERR_MSK    (0x1 << DSI_TEAR_REQUEST_ERR_POS)
#define DSI_TEAR_REQUEST_ERR_CLR    (~DSI_TEAR_REQUEST_ERR_MSK)

/*INT_ST1[19]:dpi_buff_pld_under*/
#define DSI_DPI_BUFF_PLD_UNDER_POS  (19)
#define DSI_DPI_BUFF_PLD_UNDER_MSK  (0x1 << DSI_DPI_BUFF_PLD_UNDER_POS)
#define DSI_DPI_BUFF_PLD_UNDER_CLR  (~DSI_DPI_BUFF_PLD_UNDER_MSK)

/*INT_ST1[17]:dbi_ilegal_comm_err*/
#define DSI_DBI_ILEGAL_COMM_ERR_POS (17)
#define DSI_DBI_ILEGAL_COMM_ERR_MSK (0x1 << DSI_DBI_ILEGAL_COMM_ERR_POS)
#define DSI_DBI_ILEGAL_COMM_ERR_CLR (~DSI_DBI_ILEGAL_COMM_ERR_MSK)

/*INT_ST1[16]:dbi_pld_recv_err*/
#define DSI_DBI_PLD_RECV_ERR_POS    (16)
#define DSI_DBI_PLD_RECV_ERR_MSK    (0x1 << DSI_DBI_PLD_RECV_ERR_POS)
#define DSI_DBI_PLD_RECV_ERR_CLR    (~DSI_DBI_PLD_RECV_ERR_MSK)

/*INT_ST1[15]:dbi_pld_rd_err*/
#define DSI_DBI_PLD_RD_ERR_POS  (15)
#define DSI_DBI_PLD_RD_ERR_MSK  (0x1 << DSI_DBI_PLD_RD_ERR_POS)
#define DSI_DBI_PLD_RD_ERR_CLR  (~DSI_DBI_PLD_RD_ERR_MSK)

/*INT_ST1[14]:dbi_pld_wr_err*/
#define DSI_DBI_PLD_WR_ERR_POS  (14)
#define DSI_DBI_PLD_WR_ERR_MSK  (0x1 << DSI_DBI_PLD_WR_ERR_POS)
#define DSI_DBI_PLD_WR_ERR_CLR  (~DSI_DBI_PLD_WR_ERR_MSK)

/*INT_ST1[13]:dbi_cmd_wr_err*/
#define DSI_DBI_CMD_WR_ERR_POS  (13)
#define DSI_DBI_CMD_WR_ERR_MSK  (0x1 << DSI_DBI_CMD_WR_ERR_POS)
#define DSI_DBI_CMD_WR_ERR_CLR  (~DSI_DBI_CMD_WR_ERR_MSK)

/*INT_ST1[12]:gen_pld_recev_err*/
#define DSI_GEN_PLD_RECEV_ERR_POS   (12)
#define DSI_GEN_PLD_RECEV_ERR_MSK   (0x1 << DSI_GEN_PLD_RECEV_ERR_POS)
#define DSI_GEN_PLD_RECEV_ERR_CLR   (~DSI_GEN_PLD_RECEV_ERR_MSK)

/*INT_ST1[11]:gen_pld_rd_err*/
#define DSI_GEN_PLD_RD_ERR_POS  (11)
#define DSI_GEN_PLD_RD_ERR_MSK  (0x1 << DSI_GEN_PLD_RD_ERR_POS)
#define DSI_GEN_PLD_RD_ERR_CLR  (~DSI_GEN_PLD_RD_ERR_MSK)

/*INT_ST1[10]:gen_pld_send_err*/
#define DSI_GEN_PLD_SEND_ERR_POS    (10)
#define DSI_GEN_PLD_SEND_ERR_MSK    (0x1 << DSI_GEN_PLD_SEND_ERR_POS)
#define DSI_GEN_PLD_SEND_ERR_CLR    (~DSI_GEN_PLD_SEND_ERR_MSK)

/*INT_ST1[9]:gen_pld_wr_err*/
#define DSI_GEN_PLD_WR_ERR_POS  (9)
#define DSI_GEN_PLD_WR_ERR_MSK  (0x1 << DSI_GEN_PLD_WR_ERR_POS)
#define DSI_GEN_PLD_WR_ERR_CLR  (~DSI_GEN_PLD_WR_ERR_MSK)

/*INT_ST1[8]:gen_cmd_wr_err*/
#define DSI_GEN_CMD_WR_ERR_POS  (8)
#define DSI_GEN_CMD_WR_ERR_MSK  (0x1 << DSI_GEN_CMD_WR_ERR_POS)
#define DSI_GEN_CMD_WR_ERR_CLR  (~DSI_GEN_CMD_WR_ERR_MSK)

/*INT_ST1[7]:dpi_pld_wr_err*/
#define DSI_DPI_PLD_WR_ERR_POS  (7)
#define DSI_DPI_PLD_WR_ERR_MSK  (0x1 << DSI_DPI_PLD_WR_ERR_POS)
#define DSI_DPI_PLD_WR_ERR_CLR  (~DSI_DPI_PLD_WR_ERR_MSK)

/*INT_ST1[6]:eopt_err*/
#define DSI_EOPT_ERR_POS    (6)
#define DSI_EOPT_ERR_MSK    (0x1 << DSI_EOPT_ERR_POS)
#define DSI_EOPT_ERR_CLR    (~DSI_EOPT_ERR_MSK)

/*INT_ST1[5]:pkt_size_err*/
#define DSI_PKT_SIZE_ERR_POS    (5)
#define DSI_PKT_SIZE_ERR_MSK    (0x1 << DSI_PKT_SIZE_ERR_POS)
#define DSI_PKT_SIZE_ERR_CLR    (~DSI_PKT_SIZE_ERR_MSK)

/*INT_ST1[4]:crc_err*/
#define DSI_CRC_ERR_POS (4)
#define DSI_CRC_ERR_MSK (0x1 << DSI_CRC_ERR_POS)
#define DSI_CRC_ERR_CLR (~DSI_CRC_ERR_MSK)

/*INT_ST1[3]:ecc_milti_err*/
#define DSI_ECC_MILTI_ERR_POS   (3)
#define DSI_ECC_MILTI_ERR_MSK   (0x1 << DSI_ECC_MILTI_ERR_POS)
#define DSI_ECC_MILTI_ERR_CLR   (~DSI_ECC_MILTI_ERR_MSK)

/*INT_ST1[2]:ecc_single_err*/
#define DSI_ECC_SINGLE_ERR_POS  (2)
#define DSI_ECC_SINGLE_ERR_MSK  (0x1 << DSI_ECC_SINGLE_ERR_POS)
#define DSI_ECC_SINGLE_ERR_CLR  (~DSI_ECC_SINGLE_ERR_MSK)

/*INT_ST1[1]:to_lp_rx*/
#define DSI_TO_LP_RX_POS    (1)
#define DSI_TO_LP_RX_MSK    (0x1 << DSI_TO_LP_RX_POS)
#define DSI_TO_LP_RX_CLR    (~DSI_TO_LP_RX_MSK)

/*INT_ST1[0]:to_hs_tx*/
#define DSI_TO_HS_TX_POS    (0)
#define DSI_TO_HS_TX_MSK    (0x1 << DSI_TO_HS_TX_POS)
#define DSI_TO_HS_TX_CLR    (~DSI_TO_HS_TX_MSK)



/* Register: INT_MSK0 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xC4. Address: . */

/*INT_MSK0[20]:mask_dphy_errors_4*/
#define DSI_MASK_DPHY_ERRORS_4_POS  (20)
#define DSI_MASK_DPHY_ERRORS_4_MSK  (0x1 << DSI_MASK_DPHY_ERRORS_4_POS)
#define DSI_MASK_DPHY_ERRORS_4_CLR  (~DSI_MASK_DPHY_ERRORS_4_MSK)

/*INT_MSK0[19]:mask_dphy_errors_3*/
#define DSI_MASK_DPHY_ERRORS_3_POS  (19)
#define DSI_MASK_DPHY_ERRORS_3_MSK  (0x1 << DSI_MASK_DPHY_ERRORS_3_POS)
#define DSI_MASK_DPHY_ERRORS_3_CLR  (~DSI_MASK_DPHY_ERRORS_3_MSK)

/*INT_MSK0[18]:mask_dphy_errors_2*/
#define DSI_MASK_DPHY_ERRORS_2_POS  (18)
#define DSI_MASK_DPHY_ERRORS_2_MSK  (0x1 << DSI_MASK_DPHY_ERRORS_2_POS)
#define DSI_MASK_DPHY_ERRORS_2_MSKINT_CLR   (~DSI_MASK_DPHY_ERRORS_2_MSK)

/*INT_MSK0[17]:mask_dphy_errors_1*/
#define DSI_MASK_DPHY_ERRORS_1_POS  (17)
#define DSI_MASK_DPHY_ERRORS_1_MSK  (0x1 << DSI_MASK_DPHY_ERRORS_1_POS)
#define DSI_MASK_DPHY_ERRORS_1_CLR  (~DSI_MASK_DPHY_ERRORS_1_MSK)

/*INT_MSK0[16]:mask_dphy_errors_0*/
#define DSI_MASK_DPHY_ERRORS_0_POS  (16)
#define DSI_MASK_DPHY_ERRORS_0_MSK  (0x1 << DSI_MASK_DPHY_ERRORS_0_POS)
#define DSI_MASK_DPHY_ERRORS_0_CLR  (~DSI_MASK_DPHY_ERRORS_0_MSK)

/*INT_MSK0[15]:mask_ack_with_err_15*/
#define DSI_MASK_ACK_WITH_ERR_15_POS    (15)
#define DSI_MASK_ACK_WITH_ERR_15_MSK    (0x1 << DSI_MASK_ACK_WITH_ERR_15_POS)
#define DSI_MASK_ACK_WITH_ERR_15_CLR    (~DSI_MASK_ACK_WITH_ERR_15_MSK)

/*INT_MSK0[14]:mask_ack_with_err_14*/
#define DSI_MASK_ACK_WITH_ERR_14_POS    (14)
#define DSI_MASK_ACK_WITH_ERR_14_MSK    (0x1 << DSI_MASK_ACK_WITH_ERR_14_POS)
#define DSI_MASK_ACK_WITH_ERR_14_CLR    (~DSI_MASK_ACK_WITH_ERR_14_MSK)

/*INT_MSK0[13]:mask_ack_with_err_13*/
#define DSI_MASK_ACK_WITH_ERR_13_POS    (13)
#define DSI_MASK_ACK_WITH_ERR_13_MSK    (0x1 << DSI_MASK_ACK_WITH_ERR_13_POS)
#define DSI_MASK_ACK_WITH_ERR_13_CLR    (~DSI_MASK_ACK_WITH_ERR_13_MSK)

/*INT_MSK0[12]:mask_ack_with_err_12*/
#define DSI_MASK_ACK_WITH_ERR_12_POS    (12)
#define DSI_MASK_ACK_WITH_ERR_12_MSK    (0x1 << DSI_MASK_ACK_WITH_ERR_12_POS)
#define DSI_MASK_ACK_WITH_ERR_12_CLR    (~DSI_MASK_ACK_WITH_ERR_12_MSK)

/*INT_MSK0[11]:mask_ack_with_err_11*/
#define DSI_MASK_ACK_WITH_ERR_11_POS    (11)
#define DSI_MASK_ACK_WITH_ERR_11_MSK    (0x1 << DSI_MASK_ACK_WITH_ERR_11_POS)
#define DSI_MASK_ACK_WITH_ERR_11_CLR    (~DSI_MASK_ACK_WITH_ERR_11_MSK)

/*INT_MSK0[10]:mask_ack_with_err_10*/
#define DSI_MASK_ACK_WITH_ERR_10_POS    (10)
#define DSI_MASK_ACK_WITH_ERR_10_MSK    (0x1 << DSI_MASK_ACK_WITH_ERR_10_POS)
#define DSI_MASK_ACK_WITH_ERR_10_CLR    (~DSI_MASK_ACK_WITH_ERR_10_MSK)

/*INT_MSK0[9]:mask_ack_with_err_9*/
#define DSI_MASK_ACK_WITH_ERR_9_POS (9)
#define DSI_MASK_ACK_WITH_ERR_9_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_9_POS)
#define DSI_MASK_ACK_WITH_ERR_9_CLR (~DSI_MASK_ACK_WITH_ERR_9_MSK)

/*INT_MSK0[8]:mask_ack_with_err_8*/
#define DSI_MASK_ACK_WITH_ERR_8_POS (8)
#define DSI_MASK_ACK_WITH_ERR_8_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_8_POS)
#define DSI_MASK_ACK_WITH_ERR_8_CLR (~DSI_MASK_ACK_WITH_ERR_8_MSK)

/*INT_MSK0[7]:mask_ack_with_err_7*/
#define DSI_MASK_ACK_WITH_ERR_7_POS (7)
#define DSI_MASK_ACK_WITH_ERR_7_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_7_POS)
#define DSI_MASK_ACK_WITH_ERR_7_CLR (~DSI_MASK_ACK_WITH_ERR_7_MSK)

/*INT_MSK0[6]:mask_ack_with_err_6*/
#define DSI_MASK_ACK_WITH_ERR_6_POS (6)
#define DSI_MASK_ACK_WITH_ERR_6_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_6_POS)
#define DSI_MASK_ACK_WITH_ERR_6_CLR (~DSI_MASK_ACK_WITH_ERR_6_MSK)

/*INT_MSK0[5]:mask_ack_with_err_5*/
#define DSI_MASK_ACK_WITH_ERR_5_POS (5)
#define DSI_MASK_ACK_WITH_ERR_5_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_5_POS)
#define DSI_MASK_ACK_WITH_ERR_5_CLR (~DSI_MASK_ACK_WITH_ERR_5_MSK)

/*INT_MSK0[4]:mask_ack_with_err_4*/
#define DSI_MASK_ACK_WITH_ERR_4_POS (4)
#define DSI_MASK_ACK_WITH_ERR_4_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_4_POS)
#define DSI_MASK_ACK_WITH_ERR_4_CLR (~DSI_MASK_ACK_WITH_ERR_4_MSK)

/*INT_MSK0[3]:mask_ack_with_err_3*/
#define DSI_MASK_ACK_WITH_ERR_3_POS (3)
#define DSI_MASK_ACK_WITH_ERR_3_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_3_POS)
#define DSI_MASK_ACK_WITH_ERR_3_CLR (~DSI_MASK_ACK_WITH_ERR_3_MSK)

/*INT_MSK0[2]:mask_ack_with_err_2*/
#define DSI_MASK_ACK_WITH_ERR_2_POS (2)
#define DSI_MASK_ACK_WITH_ERR_2_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_2_POS)
#define DSI_MASK_ACK_WITH_ERR_2_CLR (~DSI_MASK_ACK_WITH_ERR_2_MSK)

/*INT_MSK0[1]:mask_ack_with_err_1*/
#define DSI_MASK_ACK_WITH_ERR_1_POS (1)
#define DSI_MASK_ACK_WITH_ERR_1_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_1_POS)
#define DSI_MASK_ACK_WITH_ERR_1_CLR (~DSI_MASK_ACK_WITH_ERR_1_MSK)

/*INT_MSK0[0]:mask_ack_with_err_0*/
#define DSI_MASK_ACK_WITH_ERR_0_POS (0)
#define DSI_MASK_ACK_WITH_ERR_0_MSK (0x1 << DSI_MASK_ACK_WITH_ERR_0_POS)
#define DSI_MASK_ACK_WITH_ERR_0_CLR (~DSI_MASK_ACK_WITH_ERR_0_MSK)


/* Register: INT_MSK1 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xC8. Address: . */

/*INT_MSK1[20]:mask_tear_request_err*/
#define DSI_MASK_TEAR_REQUEST_ERR_POS   (20)
#define DSI_MASK_TEAR_REQUEST_ERR_MSK   (0x1 << DSI_MASK_TEAR_REQUEST_ERR_POS)
#define DSI_MASK_TEAR_REQUEST_ERR_CLR   (~DSI_MASK_TEAR_REQUEST_ERR_MSK)

/*INT_MSK1[19]:mask_dpi_buff_pld_under*/
#define DSI_MASK_DPI_BUFF_PLD_UNDER_POS (19)
#define DSI_MASK_DPI_BUFF_PLD_UNDER_MSK (0x1 << DSI_MASK_DPI_BUFF_PLD_UNDER_POS)
#define DSI_MASK_DPI_BUFF_PLD_UNDER_CLR (~DSI_MASK_DPI_BUFF_PLD_UNDER_MSK)

/*INT_MSK1[17]:mask_dbi_ilegal_comm_err*/
#define DSI_MASK_DBI_ILEGAL_COMM_ERR_POS    (17)
#define DSI_MASK_DBI_ILEGAL_COMM_ERR_MSK    (0x1 << DSI_MASK_DBI_ILEGAL_COMM_ERR_POS)
#define DSI_MASK_DBI_ILEGAL_COMM_ERR_CLR    (~DSI_MASK_DBI_ILEGAL_COMM_ERR_MSK)

/*INT_MSK1[16]:mask_dbi_pld_recv_err*/
#define DSI_MASK_DBI_PLD_RECV_ERR_POS   (16)
#define DSI_MASK_DBI_PLD_RECV_ERR_MSK   (0x1 << DSI_MASK_DBI_PLD_RECV_ERR_POS)
#define DSI_MASK_DBI_PLD_RECV_ERR_CLR   (~DSI_MASK_DBI_PLD_RECV_ERR_MSK)

/*INT_MSK1[15]:mask_dbi_pld_rd_err*/
#define DSI_MASK_DBI_PLD_RD_ERR_POS (15)
#define DSI_MASK_DBI_PLD_RD_ERR_MSK (0x1 << DSI_MASK_DBI_PLD_RD_ERR_POS)
#define DSI_MASK_DBI_PLD_RD_ERR_CLR (~DSI_MASK_DBI_PLD_RD_ERR_MSK)

/*INT_MSK1[14]:mask_dbi_pld_wr_err*/
#define DSI_MASK_DBI_PLD_WR_ERR_POS (14)
#define DSI_MASK_DBI_PLD_WR_ERR_MSK (0x1 << DSI_MASK_DBI_PLD_WR_ERR_POS)
#define DSI_MASK_DBI_PLD_WR_ERR_CLR (~DSI_MASK_DBI_PLD_WR_ERR_MSK)

/*INT_MSK1[13]:mask_dbi_cmd_wr_err*/
#define DSI_MASK_DBI_CMD_WR_ERR_POS (13)
#define DSI_MASK_DBI_CMD_WR_ERR_MSK (0x1 << DSI_MASK_DBI_CMD_WR_ERR_POS)
#define DSI_MASK_DBI_CMD_WR_ERR_CLR (~DSI_MASK_DBI_CMD_WR_ERR_MSK)

/*INT_MSK1[12]:mask_gen_pld_recev_err*/
#define DSI_MASK_GEN_PLD_RECEV_ERR_POS  (12)
#define DSI_MASK_GEN_PLD_RECEV_ERR_MSK  (0x1 << DSI_MASK_GEN_PLD_RECEV_ERR_POS)
#define DSI_MASK_GEN_PLD_RECEV_ERR_CLR  (~DSI_MASK_GEN_PLD_RECEV_ERR_MSK)

/*INT_MSK1[11]:mask_gen_pld_rd_err*/
#define DSI_MASK_GEN_PLD_RD_ERR_POS (11)
#define DSI_MASK_GEN_PLD_RD_ERR_MSK (0x1 << DSI_MASK_GEN_PLD_RD_ERR_POS)
#define DSI_MASK_GEN_PLD_RD_ERR_CLR (~DSI_MASK_GEN_PLD_RD_ERR_MSK)

/*INT_MSK1[10]:mask_gen_pld_send_err*/
#define DSI_MASK_GEN_PLD_SEND_ERR_POS   (10)
#define DSI_MASK_GEN_PLD_SEND_ERR_MSK   (0x1 << DSI_MASK_GEN_PLD_SEND_ERR_POS)
#define DSI_MASK_GEN_PLD_SEND_ERR_CLR   (~DSI_MASK_GEN_PLD_SEND_ERR_MSK)

/*INT_MSK1[9]:mask_gen_pld_wr_err*/
#define DSI_MASK_GEN_PLD_WR_ERR_POS (9)
#define DSI_MASK_GEN_PLD_WR_ERR_MSK (0x1 << DSI_MASK_GEN_PLD_WR_ERR_POS)
#define DSI_MASK_GEN_PLD_WR_ERR_CLR (~DSI_MASK_GEN_PLD_WR_ERR_MSK)

/*INT_MSK1[8]:mask_gen_cmd_wr_err*/
#define DSI_MASK_GEN_CMD_WR_ERR_POS (8)
#define DSI_MASK_GEN_CMD_WR_ERR_MSK (0x1 << DSI_MASK_GEN_CMD_WR_ERR_POS)
#define DSI_MASK_GEN_CMD_WR_ERR_CLR (~DSI_MASK_GEN_CMD_WR_ERR_MSK)

/*INT_MSK1[7]:mask_dpi_pld_wr_err*/
#define DSI_MASK_DPI_PLD_WR_ERR_POS (7)
#define DSI_MASK_DPI_PLD_WR_ERR_MSK (0x1 << DSI_MASK_DPI_PLD_WR_ERR_POS)
#define DSI_MASK_DPI_PLD_WR_ERR_CLR (~DSI_MASK_DPI_PLD_WR_ERR_MSK)

/*INT_MSK1[6]:mask_eopt_err*/
#define DSI_MASK_EOPT_ERR_POS   (6)
#define DSI_MASK_EOPT_ERR_MSK   (0x1 << DSI_MASK_EOPT_ERR_POS)
#define DSI_MASK_EOPT_ERR_CLR   (~DSI_MASK_EOPT_ERR_MSK)

/*INT_MSK1[5]:mask_pkt_size_err*/
#define DSI_MASK_PKT_SIZE_ERR_POS   (5)
#define DSI_MASK_PKT_SIZE_ERR_MSK   (0x1 << DSI_MASK_PKT_SIZE_ERR_POS)
#define DSI_MASK_PKT_SIZE_ERR_CLR   (~DSI_MASK_PKT_SIZE_ERR_MSK)

/*INT_MSK1[4]:mask_crc_err*/
#define DSI_MASK_CRC_ERR_POS    (4)
#define DSI_MASK_CRC_ERR_MSK    (0x1 << DSI_MASK_CRC_ERR_POS)
#define DSI_MASK_CRC_ERR_CLR    (~DSI_MASK_CRC_ERR_MSK)

/*INT_MSK1[3]:mask_ecc_milti_err*/
#define DSI_MASK_ECC_MILTI_ERR_POS  (3)
#define DSI_MASK_ECC_MILTI_ERR_MSK  (0x1 << DSI_MASK_ECC_MILTI_ERR_POS)
#define DSI_MASK_ECC_MILTI_ERR_CLR  (~DSI_MASK_ECC_MILTI_ERR_MSK)

/*INT_MSK1[2]:mask_ecc_single_err*/
#define DSI_MASK_ECC_SINGLE_ERR_POS (2)
#define DSI_MASK_ECC_SINGLE_ERR_MSK (0x1 << DSI_MASK_ECC_SINGLE_ERR_POS)
#define DSI_MASK_ECC_SINGLE_ERR_CLR (~DSI_MASK_ECC_SINGLE_ERR_MSK)

/*INT_MSK1[1]:mask_to_lp_rx*/
#define DSI_MASK_TO_LP_RX_POS   (1)
#define DSI_MASK_TO_LP_RX_MSK   (0x1 << DSI_MASK_TO_LP_RX_POS)
#define DSI_MASK_TO_LP_RX_CLR   (~DSI_MASK_TO_LP_RX_MSK)

/*INT_MSK1[0]:mask_to_hs_tx*/
#define DSI_MASK_TO_HS_TX_POS   (0)
#define DSI_MASK_TO_HS_TX_MSK   (0x1 << DSI_MASK_TO_HS_TX_POS)
#define DSI_MASK_TO_HS_TX_CLR   (~DSI_MASK_TO_HS_TX_MSK)



/* Register: PHY_CAL -------------------------------------------------------*/
/* Description: Control register. Offset: 0xCC. Address: . */

/*PHY_CAL[0]:txskewcalhs*/
#define DSI_TXSKEWCALHS_POS (0)
#define DSI_TXSKEWCALHS_MSK (0x1 << DSI_TXSKEWCALHS_POS)
#define DSI_TXSKEWCALHS_CLR (~DSI_TXSKEWCALHS_MSK)


/* Register: INT_FORCE0 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xD8. Address: . */

/*INT_FORCE0[20]:force_dphy_errors_4*/
#define DSI_FORCE_DPHY_ERRORS_4_POS (20)
#define DSI_FORCE_DPHY_ERRORS_4_MSK (0x1 << DSI_FORCE_DPHY_ERRORS_4_POS)
#define DSI_FORCE_DPHY_ERRORS_4_CLR (~DSI_FORCE_DPHY_ERRORS_4_MSK)

/*INT_FORCE0[19]:force_dphy_errors_3*/
#define DSI_FORCE_DPHY_ERRORS_3_POS (19)
#define DSI_FORCE_DPHY_ERRORS_3_MSK (0x1 << DSI_FORCE_DPHY_ERRORS_3_POS)
#define DSI_FORCE_DPHY_ERRORS_3_CLR (~DSI_FORCE_DPHY_ERRORS_3_MSK)

/*INT_FORCE0[18]:force_dphy_errors_2*/
#define DSI_FORCE_DPHY_ERRORS_2_POS (18)
#define DSI_FORCE_DPHY_ERRORS_2_MSK (0x1 << DSI_FORCE_DPHY_ERRORS_2_POS)
#define DSI_FORCE_DPHY_ERRORS_2_CLR (~DSI_FORCE_DPHY_ERRORS_2_MSK)

/*INT_FORCE0[17]:force_dphy_errors_1*/
#define DSI_FORCE_DPHY_ERRORS_1_POS (17)
#define DSI_FORCE_DPHY_ERRORS_1_MSK (0x1 << DSI_FORCE_DPHY_ERRORS_1_POS)
#define DSI_FORCE_DPHY_ERRORS_1_CLR (~DSI_FORCE_DPHY_ERRORS_1_MSK)

/*INT_FORCE0[16]:force_dphy_errors_0*/
#define DSI_FORCE_DPHY_ERRORS_0_POS (16)
#define DSI_FORCE_DPHY_ERRORS_0_MSK (0x1 << DSI_FORCE_DPHY_ERRORS_0_POS)
#define DSI_FORCE_DPHY_ERRORS_0_CLR (~DSI_FORCE_DPHY_ERRORS_0_MSK)

/*INT_FORCE0[15]:force_ack_with_err_15*/
#define DSI_FORCE_ACK_WITH_ERR_15_POS   (15)
#define DSI_FORCE_ACK_WITH_ERR_15_MSK   (0x1 << DSI_FORCE_ACK_WITH_ERR_15_POS)
#define DSI_FORCE_ACK_WITH_ERR_15_CLR   (~DSI_FORCE_ACK_WITH_ERR_15_MSK)

/*INT_FORCE0[14]:force_ack_with_err_14*/
#define DSI_FORCE_ACK_WITH_ERR_14_POS   (14)
#define DSI_FORCE_ACK_WITH_ERR_14_MSK   (0x1 << DSI_FORCE_ACK_WITH_ERR_14_POS)
#define DSI_FORCE_ACK_WITH_ERR_14_CLR   (~DSI_FORCE_ACK_WITH_ERR_14_MSK)

/*INT_FORCE0[13]:force_ack_with_err_13*/
#define DSI_FORCE_ACK_WITH_ERR_13_POS   (13)
#define DSI_FORCE_ACK_WITH_ERR_13_MSK   (0x1 << DSI_FORCE_ACK_WITH_ERR_13_POS)
#define DSI_FORCE_ACK_WITH_ERR_13_CLR   (~DSI_FORCE_ACK_WITH_ERR_13_MSK)

/*INT_FORCE0[12]:force_ack_with_err_12*/
#define DSI_FORCE_ACK_WITH_ERR_12_POS   (12)
#define DSI_FORCE_ACK_WITH_ERR_12_MSK   (0x1 << DSI_FORCE_ACK_WITH_ERR_12_POS)
#define DSI_FORCE_ACK_WITH_ERR_12_CLR   (~DSI_FORCE_ACK_WITH_ERR_12_MSK)

/*INT_FORCE0[11]:force_ack_with_err_11*/
#define DSI_FORCE_ACK_WITH_ERR_11_POS   (11)
#define DSI_FORCE_ACK_WITH_ERR_11_MSK   (0x1 << DSI_FORCE_ACK_WITH_ERR_11_POS)
#define DSI_FORCE_ACK_WITH_ERR_11_CLR   (~DSI_FORCE_ACK_WITH_ERR_11_MSK)

/*INT_FORCE0[10]:force_ack_with_err_10*/
#define DSI_FORCE_ACK_WITH_ERR_10_POS   (10)
#define DSI_FORCE_ACK_WITH_ERR_10_MSK   (0x1 << DSI_FORCE_ACK_WITH_ERR_10_POS)
#define DSI_FORCE_ACK_WITH_ERR_10_CLR   (~DSI_FORCE_ACK_WITH_ERR_10_MSK)

/*INT_FORCE0[9]:force_ack_with_err_9*/
#define DSI_FORCE_ACK_WITH_ERR_9_POS    (9)
#define DSI_FORCE_ACK_WITH_ERR_9_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_9_POS)
#define DSI_FORCE_ACK_WITH_ERR_9_CLR    (~DSI_FORCE_ACK_WITH_ERR_9_MSK)

/*INT_FORCE0[8]:force_ack_with_err_8*/
#define DSI_FORCE_ACK_WITH_ERR_8_POS    (8)
#define DSI_FORCE_ACK_WITH_ERR_8_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_8_POS)
#define DSI_FORCE_ACK_WITH_ERR_8_CLR    (~DSI_FORCE_ACK_WITH_ERR_8_MSK)

/*INT_FORCE0[7]:force_ack_with_err_7*/
#define DSI_FORCE_ACK_WITH_ERR_7_POS    (7)
#define DSI_FORCE_ACK_WITH_ERR_7_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_7_POS)
#define DSI_FORCE_ACK_WITH_ERR_7_CLR    (~DSI_FORCE_ACK_WITH_ERR_7_MSK)

/*INT_FORCE0[6]:force_ack_with_err_6*/
#define DSI_FORCE_ACK_WITH_ERR_6_POS    (6)
#define DSI_FORCE_ACK_WITH_ERR_6_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_6_POS)
#define DSI_FORCE_ACK_WITH_ERR_6_CLR    (~DSI_FORCE_ACK_WITH_ERR_6_MSK)

/*INT_FORCE0[5]:force_ack_with_err_5*/
#define DSI_FORCE_ACK_WITH_ERR_5_POS    (5)
#define DSI_FORCE_ACK_WITH_ERR_5_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_5_POS)
#define DSI_FORCE_ACK_WITH_ERR_5_CLR    (~DSI_FORCE_ACK_WITH_ERR_5_MSK)

/*INT_FORCE0[4]:force_ack_with_err_4*/
#define DSI_FORCE_ACK_WITH_ERR_4_POS    (4)
#define DSI_FORCE_ACK_WITH_ERR_4_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_4_POS)
#define DSI_FORCE_ACK_WITH_ERR_4_CLR    (~DSI_FORCE_ACK_WITH_ERR_4_MSK)

/*INT_FORCE0[3]:force_ack_with_err_3*/
#define DSI_FORCE_ACK_WITH_ERR_3_POS    (3)
#define DSI_FORCE_ACK_WITH_ERR_3_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_3_POS)
#define DSI_FORCE_ACK_WITH_ERR_3_CLR    (~DSI_FORCE_ACK_WITH_ERR_3_MSK)

/*INT_FORCE0[2]:force_ack_with_err_2*/
#define DSI_FORCE_ACK_WITH_ERR_2_POS    (2)
#define DSI_FORCE_ACK_WITH_ERR_2_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_2_POS)
#define DSI_FORCE_ACK_WITH_ERR_2_CLR    (~DSI_FORCE_ACK_WITH_ERR_2_MSK)

/*INT_FORCE0[1]:force_ack_with_err_1*/
#define DSI_FORCE_ACK_WITH_ERR_1_POS    (1)
#define DSI_FORCE_ACK_WITH_ERR_1_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_1_POS)
#define DSI_FORCE_ACK_WITH_ERR_1_CLR    (~DSI_FORCE_ACK_WITH_ERR_1_MSK)

/*INT_FORCE0[0]:force_ack_with_err_0*/
#define DSI_FORCE_ACK_WITH_ERR_0_POS    (0)
#define DSI_FORCE_ACK_WITH_ERR_0_MSK    (0x1 << DSI_FORCE_ACK_WITH_ERR_0_POS)

#define DSI_FORCE_ACK_WITH_ERR_0_CLR    (~DSI_FORCE_ACK_WITH_ERR_0_MSK)



/* Register: INT_FORCE1 -------------------------------------------------------*/
/* Description: Control register. Offset: 0xDC. Address: . */

/*INT_FORCE1[20]:force_tear_request_err*/
#define DSI_FORCE_TEAR_REQUEST_ERR_POS  (20)
#define DSI_FORCE_TEAR_REQUEST_ERR_MSK  (0x1 << DSI_FORCE_TEAR_REQUEST_ERR_POS)
#define DSI_FORCE_TEAR_REQUEST_ERR_CLR  (~DSI_FORCE_TEAR_REQUEST_ERR_MSK)

/*INT_FORCE1[19]:force_dpi_buff_pld_under*/
#define DSI_FORCE_DPI_BUFF_PLD_UNDER_POS    (19)
#define DSI_FORCE_DPI_BUFF_PLD_UNDER_MSK    (0x1 << DSI_FORCE_DPI_BUFF_PLD_UNDER_POS)
#define DSI_FORCE_DPI_BUFF_PLD_UNDER_CLR    (~DSI_FORCE_DPI_BUFF_PLD_UNDER_MSK)

/*INT_FORCE1[17]:force_dbi_ilegal_comm_err*/
#define DSI_FORCE_DBI_ILEGAL_COMM_ERR_POS   (17)
#define DSI_FORCE_DBI_ILEGAL_COMM_ERR_MSK   (0x1 << DSI_FORCE_DBI_ILEGAL_COMM_ERR_POS)
#define DSI_FORCE_DBI_ILEGAL_COMM_ERR_CLR   (~DSI_FORCE_DBI_ILEGAL_COMM_ERR_MSK)

/*INT_FORCE1[16]:force_dbi_pld_recv_err*/
#define DSI_FORCE_DBI_PLD_RECV_ERR_POS  (16)
#define DSI_FORCE_DBI_PLD_RECV_ERR_MSK  (0x1 << DSI_FORCE_DBI_PLD_RECV_ERR_POS)
#define DSI_FORCE_DBI_PLD_RECV_ERR_CLR  (~DSI_FORCE_DBI_PLD_RECV_ERR_MSK)

/*INT_FORCE1[15]:force_dbi_pld_rd_err*/
#define DSI_FORCE_DBI_PLD_RD_ERR_POS    (15)
#define DSI_FORCE_DBI_PLD_RD_ERR_MSK    (0x1 << DSI_FORCE_DBI_PLD_RD_ERR_POS)
#define DSI_FORCE_DBI_PLD_RD_ERR_CLR    (~DSI_FORCE_DBI_PLD_RD_ERR_MSK)

/*INT_FORCE1[14]:force_dbi_pld_wr_err*/
#define DSI_FORCE_DBI_PLD_WR_ERR_POS    (14)
#define DSI_FORCE_DBI_PLD_WR_ERR_MSK    (0x1 << DSI_FORCE_DBI_PLD_WR_ERR_POS)
#define DSI_FORCE_DBI_PLD_WR_ERR_CLR    (~DSI_FORCE_DBI_PLD_WR_ERR_MSK)

/*INT_FORCE1[13]:force_dbi_cmd_wr_err*/
#define DSI_FORCE_DBI_CMD_WR_ERR_POS    (13)
#define DSI_FORCE_DBI_CMD_WR_ERR_MSK    (0x1 << DSI_FORCE_DBI_CMD_WR_ERR_POS)
#define DSI_FORCE_DBI_CMD_WR_ERR_CLR    (~DSI_FORCE_DBI_CMD_WR_ERR_MSK)

/*INT_FORCE1[12]:force_gen_pld_recev_err*/
#define DSI_FORCE_GEN_PLD_RECEV_ERR_POS (12)
#define DSI_FORCE_GEN_PLD_RECEV_ERR_MSK (0x1 << DSI_FORCE_GEN_PLD_RECEV_ERR_POS)
#define DSI_FORCE_GEN_PLD_RECEV_ERR_CLR (~DSI_FORCE_GEN_PLD_RECEV_ERR_MSK)

/*INT_FORCE1[11]:force_gen_pld_rd_err*/
#define DSI_FORCE_GEN_PLD_RD_ERR_POS    (11)
#define DSI_FORCE_GEN_PLD_RD_ERR_MSK    (0x1 << DSI_FORCE_GEN_PLD_RD_ERR_POS)
#define DSI_FORCE_GEN_PLD_RD_ERR_CLR    (~DSI_FORCE_GEN_PLD_RD_ERR_MSK)

/*INT_FORCE1[10]:force_gen_pld_send_err*/
#define DSI_FORCE_GEN_PLD_SEND_ERR_POS  (10)
#define DSI_FORCE_GEN_PLD_SEND_ERR_MSK  (0x1 << DSI_FORCE_GEN_PLD_SEND_ERR_POS)
#define DSI_FORCE_GEN_PLD_SEND_ERR_CLR  (~DSI_FORCE_GEN_PLD_SEND_ERR_MSK)

/*INT_FORCE1[9]:force_gen_pld_wr_err*/
#define DSI_FORCE_GEN_PLD_WR_ERR_POS    (9)
#define DSI_FORCE_GEN_PLD_WR_ERR_MSK    (0x1 << DSI_FORCE_GEN_PLD_WR_ERR_POS)
#define DSI_FORCE_GEN_PLD_WR_ERR_CLR    (~DSI_FORCE_GEN_PLD_WR_ERR_MSK)

/*INT_FORCE1[8]:force_gen_cmd_wr_err*/
#define DSI_FORCE_GEN_CMD_WR_ERR_POS    (8)
#define DSI_FORCE_GEN_CMD_WR_ERR_MSK    (0x1 << DSI_FORCE_GEN_CMD_WR_ERR_POS)
#define DSI_FORCE_GEN_CMD_WR_ERR_CLR    (~DSI_FORCE_GEN_CMD_WR_ERR_MSK)

/*INT_FORCE1[7]:force_dpi_pld_wr_err*/
#define DSI_FORCE_DPI_PLD_WR_ERR_POS    (7)
#define DSI_FORCE_DPI_PLD_WR_ERR_MSK    (0x1 << DSI_FORCE_DPI_PLD_WR_ERR_POS)
#define DSI_FORCE_DPI_PLD_WR_ERR_CLR    (~DSI_FORCE_DPI_PLD_WR_ERR_MSK)

/*INT_FORCE1[6]:force_eopt_err*/
#define DSI_FORCE_EOPT_ERR_POS  (6)
#define DSI_FORCE_EOPT_ERR_MSK  (0x1 << DSI_FORCE_EOPT_ERR_POS)
#define DSI_FORCE_EOPT_ERR_CLR  (~DSI_FORCE_EOPT_ERR_MSK)

/*INT_FORCE1[5]:force_pkt_size_err*/
#define DSI_FORCE_PKT_SIZE_ERR_POS  (5)
#define DSI_FORCE_PKT_SIZE_ERR_MSK  (0x1 << DSI_FORCE_PKT_SIZE_ERR_POS)
#define DSI_FORCE_PKT_SIZE_ERR_CLR  (~DSI_FORCE_PKT_SIZE_ERR_MSK)

/*INT_FORCE1[4]:force_crc_err*/
#define DSI_FORCE_CRC_ERR_POS   (4)
#define DSI_FORCE_CRC_ERR_MSK   (0x1 << DSI_FORCE_CRC_ERR_POS)
#define DSI_FORCE_CRC_ERR_CLR   (~DSI_FORCE_CRC_ERR_MSK)

/*INT_FORCE1[3]:force_ecc_milti_err*/
#define DSI_FORCE_ECC_MILTI_ERR_POS (3)
#define DSI_FORCE_ECC_MILTI_ERR_MSK (0x1 << DSI_FORCE_ECC_MILTI_ERR_POS)
#define DSI_FORCE_ECC_MILTI_ERR_CLR (~DSI_FORCE_ECC_MILTI_ERR_MSK)

/*INT_FORCE1[2]:force_ecc_single_err*/
#define DSI_FORCE_ECC_SINGLE_ERR_POS    (2)
#define DSI_FORCE_ECC_SINGLE_ERR_MSK    (0x1 << DSI_FORCE_ECC_SINGLE_ERR_POS)
#define DSI_FORCE_ECC_SINGLE_ERR_CLR    (~DSI_FORCE_ECC_SINGLE_ERR_MSK)

/*INT_FORCE1[1]:force_to_lp_rx*/
#define DSI_FORCE_TO_LP_RX_POS  (1)
#define DSI_FORCE_TO_LP_RX_MSK  (0x1 << DSI_FORCE_TO_LP_RX_POS)
#define DSI_FORCE_TO_LP_RX_CLR  (~DSI_FORCE_TO_LP_RX_MSK)

/*INT_FORCE1[0]:force_to_hs_tx*/
#define DSI_FORCE_TO_HS_TX_POS  (0)
#define DSI_FORCE_TO_HS_TX_MSK  (0x1 << DSI_FORCE_TO_HS_TX_POS)
#define DSI_FORCE_TO_HS_TX_CLR  (~DSI_FORCE_TO_HS_TX_MSK)



/* Register: AUTO_ULPS_MODE -------------------------------------------------------*/
/* Description: Control register. Offset: 0xe0. Address: . */

/*AUTO_ULPS_MODE[17]:pre_pll_off_req*/
#define DSI_PRE_PLL_OFF_REQ_POS (17)
#define DSI_PRE_PLL_OFF_REQ_MSK (0x1 << DSI_PRE_PLL_OFF_REQ_POS)
#define DSI_PRE_PLL_OFF_REQ_CLR (~DSI_PRE_PLL_OFF_REQ_MSK)

/*AUTO_ULPS_MODE[16]:pll_off_ulps*/
#define DSI_PLL_OFF_ULPS_POS    (16)
#define DSI_PLL_OFF_ULPS_MSK    (0x1 << DSI_PLL_OFF_ULPS_POS)
#define DSI_PLL_OFF_ULPS_CLR    (~DSI_PLL_OFF_ULPS_MSK)

/*AUTO_ULPS_MODE[0]:auto_ulps*/
#define DSI_AUTO_ULPS_POS   (0)
#define DSI_AUTO_ULPS_MSK   (0x1 << DSI_AUTO_ULPS_POS)
#define DSI_AUTO_ULPS_CLR   (~DSI_AUTO_ULPS_MSK)


/* Register: AUTO_ULPS_ENTRY_DELAY -------------------------------------------------------*/
/* Description: Control register. Offset: 0xe4. Address: . */

/*AUTO_ULPS_ENTRY_DELAY[31:0]:ulps_entry_delay*/
#define DSI_ULPS_ENTRY_DELAY_POS    (0)
#define DSI_ULPS_ENTRY_DELAY_MSK    (0xFFFFFFFF << DSI_ULPS_ENTRY_DELAY_POS)
#define DSI_ULPS_ENTRY_DELAY_CLR    (~DSI_ULPS_ENTRY_DELAY_MSK)


/* Register: AUTO_ULPS_WAKEUP_TIME -------------------------------------------------------*/
/* Description: Control register. Offset: 0xe8. Address: . */

/*AUTO_ULPS_WAKEUP_TIME[31:16]:twakeup_cnt*/
#define DSI_TWAKEUP_CNT_POS (16)
#define DSI_TWAKEUP_CNT_MSK (0xFFFF << DSI_TWAKEUP_CNT_POS)
#define DSI_TWAKEUP_CNT_CLR (~DSI_TWAKEUP_CNT_MSK)

/*AUTO_ULPS_WAKEUP_TIME[15:0]:twakeup_clk_div*/
#define DSI_TWAKEUP_CLK_DIV_POS (0)
#define DSI_TWAKEUP_CLK_DIV_MSK (0xFFFF << DSI_TWAKEUP_CLK_DIV_POS)
#define DSI_TWAKEUP_CLK_DIV_CLR (~DSI_TWAKEUP_CLK_DIV_MSK)


/* Register: DSC_PARAMETER -------------------------------------------------------*/
/* Description: Control register. Offset: 0xf0. Address: . */

/*DSC_PARAMETER[17:16]:pps_sel*/
#define DSI_PPS_SEL_POS (16)
#define DSI_PPS_SEL_MSK (0x3 << DSI_PPS_SEL_POS)
#define DSI_PPS_SEL_CLR (~DSI_PPS_SEL_MSK)

/*DSC_PARAMETER[9:8]:compress_algo*/
#define DSI_COMPRESS_ALGO_POS   (8)
#define DSI_COMPRESS_ALGO_MSK   (0x3 << DSI_COMPRESS_ALGO_POS)
#define DSI_COMPRESS_ALGO_CLR   (~DSI_COMPRESS_ALGO_MSK)

/*DSC_PARAMETER[0]:compression_mode*/
#define DSI_COMPRESSION_MODE_POS    (0)
#define DSI_COMPRESSION_MODE_MSK    (0x1 << DSI_COMPRESSION_MODE_POS)
#define DSI_COMPRESSION_MODE_CLR    (~DSI_COMPRESSION_MODE_MSK)


/* Register: PHY_TMR_RD_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0xf4. Address: . */

/*PHY_TMR_RD_CFG[14:0]:max_rd_time*/
#define DSI_MAX_RD_TIME_POS (0)
#define DSI_MAX_RD_TIME_MSK (0x7FFF << DSI_MAX_RD_TIME_POS)
#define DSI_MAX_RD_TIME_CLR (~DSI_MAX_RD_TIME_MSK)


/* Register: AUTO_ULPS_MIN_TIME -------------------------------------------------------*/
/* Description: Control register. Offset: 0xf8. Address: . */

/*AUTO_ULPS_MIN_TIME[11:0]:ulps_min_time*/
#define DSI_ULPS_MIN_TIME_POS   (0)
#define DSI_ULPS_MIN_TIME_MSK   (0xFFF << DSI_ULPS_MIN_TIME_POS)
#define DSI_ULPS_MIN_TIME_CLR   (~DSI_ULPS_MIN_TIME_MSK)


/* Register: PHY_MODE -------------------------------------------------------*/
/* Description: Control register. Offset: 0xfc. Address: . */

/*PHY_MODE[0]:phy_mode*/
#define DSI_PHY_MODE_POS    (0)
#define DSI_PHY_MODE_MSK    (0x1 << DSI_PHY_MODE_POS)
#define DSI_PHY_MODE_CLR    (~DSI_PHY_MODE_MSK)


/* Register: VID_SHADOW_CTRL -------------------------------------------------------*/
/* Description: Control register. Offset: 0x100. Address: . */

/*VID_SHADOW_CTRL[16]:vid_shadow_pin_req*/
#define DSI_VID_SHADOW_PIN_REQ_POS  (16)
#define DSI_VID_SHADOW_PIN_REQ_MSK  (0x1 << DSI_VID_SHADOW_PIN_REQ_POS)
#define DSI_VID_SHADOW_PIN_REQ_CLR  (~DSI_VID_SHADOW_PIN_REQ_MSK)

/*VID_SHADOW_CTRL[8]:vid_shadow_req*/
#define DSI_VID_SHADOW_REQ_POS  (8)
#define DSI_VID_SHADOW_REQ_MSK  (0x1 << DSI_VID_SHADOW_REQ_POS)
#define DSI_VID_SHADOW_REQ_CLR  (~DSI_VID_SHADOW_REQ_MSK)

/*VID_SHADOW_CTRL[0]:vid_shadow_en*/
#define DSI_VID_SHADOW_EN_POS   (0)
#define DSI_VID_SHADOW_EN_MSK   (0x1 << DSI_VID_SHADOW_EN_POS)
#define DSI_VID_SHADOW_EN_CLR   (~DSI_VID_SHADOW_EN_MSK)

/* Register: EDPI_TE_HW_CFG -------------------------------------------------------*/
/* Description: Control register. Offset: 0x11C. Address: . */

/*EDPI_TE_HW_CFG[16]:scan_line_parameter*/
#define  SCAN_LINE_PARAMETER_POS    (16)
#define  SCAN_LINE_PARAMETER_MSK    (0x1 << SCAN_LINE_PARAMETER_POS)
#define  SCAN_LINE_PARAMETER_CLR    (   ~SCAN_LINE_PARAMETER_MSK    )

/*EDPI_TE_HW_CFG[4]:hw_set_scan_line*/
#define  HW_SET_SCAN_LINE_POS   (4)
#define  HW_SET_SCAN_LINE_MSK   (   0x1 <<HW_SET_SCAN_LINE_POS )
#define  HW_SET_SCAN_LINE_CLR   (   ~HW_SET_SCAN_LINE_MSK   )

/*EDPI_TE_HW_CFG[1]:hw_tear_effect_gen*/
#define  HW_TEAR_EFFECT_GEN_POS (1)
#define  HW_TEAR_EFFECT_GEN_MSK (0x1 << HW_TEAR_EFFECT_GEN_POS  )
#define  HW_TEAR_EFFECT_GEN_CLR (   ~HW_TEAR_EFFECT_GEN_MSK )

/*EDPI_TE_HW_CFG[0]:hw_tear_effect_on*/
#define  HW_TEAR_EFFECT_ON_POS  (0)
#define  HW_TEAR_EFFECT_ON_MSK  (   0x1 << HW_TEAR_EFFECT_ON_POS)
#define  HW_TEAR_EFFECT_ON_CLR  (   ~HW_TEAR_EFFECT_ON_MSK  )


/* Register: VID_PKT_STATUS -------------------------------------------------------*/
/* Description: Control register. Offset: 0x168. Address: . */

/*VID_PKT_STATUS[23]:edpi_buff_pld_full*/
#define  EDPI_BUFF_PLD_FULL_POS (23)
#define  EDPI_BUFF_PLD_FULL_MSK (0x1 << EDPI_BUFF_PLD_FULL_POS  )
#define  EDPI_BUFF_PLD_FULL_CLR (   ~EDPI_BUFF_PLD_FULL_MSK )

/*VID_PKT_STATUS[22]:edpi_buff_pld_empty*/
#define  EDPI_BUFF_PLD_EMPTY_POS    (22)
#define  EDPI_BUFF_PLD_EMPTY_MSK    (   0x1 << EDPI_BUFF_PLD_EMPTY_POS)
#define  EDPI_BUFF_PLD_EMPTY_CLR    (   ~EDPI_BUFF_PLD_EMPTY_MSK    )

/*VID_PKT_STATUS[21]:edpi_buff_cmd_full*/
#define  EDPI_BUFF_CMD_FULL_POS (21)
#define  EDPI_BUFF_CMD_FULL_MSK (   0x1 << EDPI_BUFF_CMD_FULL_POS)
#define  EDPI_BUFF_CMD_FULL_CLR (   ~EDPI_BUFF_CMD_FULL_MSK )

/*VID_PKT_STATUS[20]:edpi_buff_cmd_empty*/
#define  EDPI_BUFF_CMD_EMPTY_POS    (20)
#define  EDPI_BUFF_CMD_EMPTY_MSK    (   0x1 << EDPI_BUFF_CMD_EMPTY_POS)
#define  EDPI_BUFF_CMD_EMPTY_CLR    (   ~EDPI_BUFF_CMD_EMPTY_MSK    )

/*VID_PKT_STATUS[17]:dpi_buff_pld_full*/
#define  DPI_BUFF_PLD_FULL_POS  (17)
#define  DPI_BUFF_PLD_FULL_MSK  (   0x1 << DPI_BUFF_PLD_FULL_POS)
#define  DPI_BUFF_PLD_FULL_CLR  (   ~DPI_BUFF_PLD_FULL_MSK  )

/*VID_PKT_STATUS[16]:dpi_buff_pld_empty*/
#define  DPI_BUFF_PLD_EMPTY_POS (16)
#define  DPI_BUFF_PLD_EMPTY_MSK (   0x1 << DPI_BUFF_PLD_EMPTY_POS)
#define  DPI_BUFF_PLD_EMPTY_CLR (   ~DPI_BUFF_PLD_EMPTY_MSK )

/*VID_PKT_STATUS[7]:edpi_pld_w_full*/
#define  EDPI_PLD_W_FULL_POS    (7)
#define  EDPI_PLD_W_FULL_MSK    (   0x1 << EDPI_PLD_W_FULL_POS)
#define  EDPI_PLD_W_FULL_CLR    (   ~EDPI_PLD_W_FULL_MSK    )

/*VID_PKT_STATUS[6]:edpi_pld_w_empty*/
#define  EDPI_PLD_W_EMPTY_POS   (6)
#define  EDPI_PLD_W_EMPTY_MSK   (   0x1 << EDPI_PLD_W_EMPTY_POS)
#define  EDPI_PLD_W_EMPTY_CLR   (   ~EDPI_PLD_W_EMPTY_MSK   )

/*VID_PKT_STATUS[5]:edpi_cmd_w_full*/
#define  EDPI_CMD_W_FULL_POS    (5)
#define  EDPI_CMD_W_FULL_MSK    (0x1 << EDPI_CMD_W_FULL_POS )
#define  EDPI_CMD_W_FULL_CLR    (   ~EDPI_CMD_W_FULL_MSK    )

/*VID_PKT_STATUS[4]:edpi_cmd_w_empty*/
#define  EDPI_CMD_W_EMPTY_POS   (4)
#define  EDPI_CMD_W_EMPTY_MSK   (   0x1 << EDPI_CMD_W_EMPTY_POS)
#define  EDPI_CMD_W_EMPTY_CLR   (   ~EDPI_CMD_W_EMPTY_MSK   )

/*VID_PKT_STATUS[3]:dpi_pld_w_full*/
#define  DPI_PLD_W_FULL_POS (3)
#define  DPI_PLD_W_FULL_MSK (0x1 << DPI_PLD_W_FULL_POS)
#define  DPI_PLD_W_FULL_CLR (   ~DPI_PLD_W_FULL_MSK )

/*VID_PKT_STATUS[2]:dpi_pld_w_empty*/
#define  DPI_PLD_W_EMPTY_POS    (2)
#define  DPI_PLD_W_EMPTY_MSK    (   0x1 << DPI_PLD_W_EMPTY_POS)
#define  DPI_PLD_W_EMPTY_CLR    (   ~DPI_PLD_W_EMPTY_MSK    )

/*VID_PKT_STATUS[1]:dpi_cmd_w_full*/
#define  DPI_CMD_W_FULL_POS (1)
#define  DPI_CMD_W_FULL_MSK (   0x1 << DPI_CMD_W_FULL_POS)
#define  DPI_CMD_W_FULL_CLR (   ~DPI_CMD_W_FULL_MSK )

/*VID_PKT_STATUS[0]:dpi_cmd_w_empty*/
#define  DPI_CMD_W_EMPTY_POS    (0)
#define  DPI_CMD_W_EMPTY_MSK    (0x1 << DPI_CMD_W_EMPTY_POS)
#define  DPI_CMD_W_EMPTY_CLR    (   ~DPI_CMD_W_EMPTY_MSK    )

#define TEST_BITCLK_PERIOD_NS               3       // 2.5ns actually,400MHz
#define TEST_LANEBYTECLK_PERIOD_NS  20      // 50MHz
#define TEST_LANEBYTECLK_DIV                3       // source lanebyte_clk,can not be set to 0 or 1
#define TEST_ESCAPECLK_PERIOD_NS        TEST_LANEBYTECLK_DIV*TEST_LANEBYTECLK_PERIOD_NS

#define  TX_LPX_TIME                        (TEST_ESCAPECLK_PERIOD_NS / TEST_LANEBYTECLK_PERIOD_NS)     // escape clock period, min 50ns
#define  HS_EXIT_TIME                       (100 / TEST_LANEBYTECLK_PERIOD_NS)                                              // min 100ns
#define  CLK_PRE_TIME                       (TEST_LANEBYTECLK_PERIOD_NS)                                                            // min 8 UI(bit clock period)
#define  CLK_ZERO_TIME                  (300 / TEST_LANEBYTECLK_PERIOD_NS)                                              // min 300ns(clk_prepare+clk_zero)
#define  CLK_PRPR_TIME                  (70 / TEST_LANEBYTECLK_PERIOD_NS)                                                   // 38-95ns
#define  CLK_POST_TIME                  (60 / TEST_LANEBYTECLK_PERIOD_NS + 52 / 8)                              // min 60ns+52UI

#define  TX_TAIL_TIME                       (60 / TEST_LANEBYTECLK_PERIOD_NS + 4 / 8)                                   // min 60ns + 4UI
#define  HS_ZERO_TIME                       (100)
#define  HS_PRPR_TIME                       (40 / TEST_LANEBYTECLK_PERIOD_NS + 4 / 8)                                   // min 40ns + 4UI

#define  CLOCK_MARGIN                       (0)
#define  DATA_MARGIN                        (5)


#ifdef __cplusplus
}
#endif

#endif /*__RTL_LCDC_DSI_H*/


/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

