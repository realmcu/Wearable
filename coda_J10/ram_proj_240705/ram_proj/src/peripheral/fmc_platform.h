/**
 *****************************************************************************************
 *     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file    fmc_platform.h
 * @brief   Flexible memory controller (SPIC & PSRAMC) platform dependent implementation header file
 * @author  Yao-Yu
 * @date    2020-10-20
 * @version v0.1
 * ***************************************************************************************
 */

#ifndef _FMC_PLATFORM_H
#define _FMC_PLATFORM_H

#include "rtl876x.h"

/****************************************************************************************
 * Flexible Memory Controller Compiling Flags
 ****************************************************************************************/
#define FMC_SUPPORT_CPU_CACHE                               1
#define FMC_SUPPORT_MAIN0_CACHE                             0

/****************************************************************************************
 * Flexible Memory Controller Address Map
 ****************************************************************************************/
// if there is no cache, then UNCACHEABLE_ADDR should equal to FMC_MAIN0_ADDR
#define FMC_MAIN0_ADDR                                      (0x02000000)
#define FMC_MAIN0_UNCACHEABLE_ADDR                          (0x02000000)
#define FMC_MAIN1_ADDR                                      (0x04000000)
#define FMC_MAIN1_UNCACHEABLE_ADDR                          (0x04000000)
#define FMC_MAIN2_ADDR                                      (0x10000000)
#define FMC_MAIN2_UNCACHEABLE_ADDR                          (0x10000000)
#define FMC_PSRAM_ADDR                                      (0x0A000000)
#define FMC_PSRAM_UNCACHEABLE_ADDR                          (0x0A000000)
#define FMC_MAIN0_SIZE                                      ( 32 * 1024 * 1024)
#define FMC_MAIN1_SIZE                                      ( 32 * 1024 * 1024)
#define FMC_MAIN2_SIZE                                      (256 * 1024 * 1024)
#define FMC_PSRAM_SIZE                                      ( 32 * 1024 * 1024)
#define FMC_MAIN0_NON_CACHE_ADDR(cache_addr)                ((cache_addr) & ~(FMC_MAIN0_ADDR) | (FMC_MAIN0_UNCACHEABLE_ADDR))
#define FMC_MAIN1_NON_CACHE_ADDR(cache_addr)                ((cache_addr) & ~(FMC_MAIN1_ADDR) | (FMC_MAIN1_UNCACHEABLE_ADDR))
#define FMC_MAIN2_NON_CACHE_ADDR(cache_addr)                ((cache_addr) & ~(FMC_MAIN2_ADDR) | (FMC_MAIN2_UNCACHEABLE_ADDR))
#define FMC_PSRAM_NON_CACHE_ADDR(cache_addr)                ((cache_addr) & ~(FMC_PSRAM_ADDR) | (FMC_PSRAM_UNCACHEABLE_ADDR))

/****************************************************************************************
 * Flexible Memory Controller Address Map Checking
 ****************************************************************************************/
#define FMC_IS_SPIC0_CACHEABLE_ADDR(addr)                   ((addr >= FMC_MAIN0_ADDR) && (addr < FMC_MAIN0_ADDR + FMC_MAIN0_SIZE))
#define FMC_IS_SPIC0_UNCACHEABLE_ADDR(addr)                 ((addr >= FMC_MAIN0_UNCACHEABLE_ADDR) && (addr < FMC_MAIN0_UNCACHEABLE_ADDR + FMC_MAIN0_SIZE))
#define FMC_IS_SPIC0_ADDR(addr)                             (FMC_IS_SPIC0_CACHEABLE_ADDR(addr) || FMC_IS_SPIC0_UNCACHEABLE_ADDR(addr))
#define FMC_IS_SPIC1_CACHEABLE_ADDR(addr)                   ((addr >= FMC_MAIN1_ADDR) && (addr < FMC_MAIN1_ADDR + FMC_MAIN1_SIZE))
#define FMC_IS_SPIC1_UNCACHEABLE_ADDR(addr)                 ((addr >= FMC_MAIN1_UNCACHEABLE_ADDR) && (addr < FMC_MAIN1_UNCACHEABLE_ADDR + FMC_MAIN1_SIZE))
#define FMC_IS_SPIC1_ADDR(addr)                             (FMC_IS_SPIC1_CACHEABLE_ADDR(addr) || FMC_IS_SPIC1_UNCACHEABLE_ADDR(addr))
#define FMC_IS_SPIC2_CACHEABLE_ADDR(addr)                   ((addr >= FMC_MAIN2_ADDR) && (addr < FMC_MAIN2_ADDR + FMC_MAIN2_SIZE))
#define FMC_IS_SPIC2_UNCACHEABLE_ADDR(addr)                 ((addr >= FMC_MAIN2_UNCACHEABLE_ADDR) && (addr < FMC_MAIN2_UNCACHEABLE_ADDR + FMC_MAIN2_SIZE))
#define FMC_IS_SPIC2_ADDR(addr)                             (FMC_IS_SPIC2_CACHEABLE_ADDR(addr) || FMC_IS_SPIC2_UNCACHEABLE_ADDR(addr))
#define FMC_IS_PSRAMC_CACHEABLE_ADDR(addr)                  ((addr >= FMC_PSRAM_ADDR) && (addr < FMC_PSRAM_ADDR + FMC_PSRAM_SIZE))
#define FMC_IS_PSRAMC_UNCACHEABLE_ADDR(addr)                ((addr >= FMC_PSRAM_UNCACHEABLE_ADDR) && (addr < FMC_PSRAM_UNCACHEABLE_ADDR + FMC_PSRAM_SIZE))
#define FMC_IS_PSRAMC_ADDR(addr)                            (FMC_IS_PSRAMC_CACHEABLE_ADDR(addr) || FMC_IS_PSRAMC_UNCACHEABLE_ADDR(addr))

/* ================================================================================ */
/* ================                      SPIC                      ================ */
/* ================================================================================ */
typedef struct
{
    __IO uint32_t CTRLR0;               /*!< Control reg 0,                         offset: 0x000 */
    __IO uint32_t RX_NDF;               /*!< User mode rx data data frame counter,  offset: 0x004 */
    __IO uint32_t SSIENR;               /*!< Enable reg,                            offset: 0x008 */
    __IO uint32_t MWCR;                 /*!< N/A,                                   offset: 0x00C */
    __IO uint32_t SER;                  /*!< Slave enable reg,                      offset: 0x010 */
    __IO uint32_t BAUDR;                /*!< Baudrate select reg,                   offset: 0x014 */
    __IO uint32_t TXFTLR;               /*!< Tx FIFO threshold level,               offset: 0x018 */
    __IO uint32_t RXFTLR;               /*!< Rx FIFO threshold level,               offset: 0x01C */
    __IO uint32_t TXFLR;                /*!< Tx FIFO level reg,                     offset: 0x020 */
    __IO uint32_t RXFLR;                /*!< Rx FIFO level reg,                     offset: 0x024 */
    __IO uint32_t SR;                   /*!< Status reg,                            offset: 0x028 */
    __IO uint32_t IMR;                  /*!< Interrupt mask reg,                    offset: 0x02C */
    __IO uint32_t ISR;                  /*!< Interrupt status reg,                  offset: 0x030 */
    __IO uint32_t RISR;                 /*!< Raw interrupt status reg,              offset: 0x034 */
    __IO uint32_t TXOICR;               /*!< Tx FIFO overflow interrupt clear reg,  offset: 0x038 */
    __IO uint32_t RXOICR;               /*!< Rx FIFO overflow interrupt clear reg,  offset: 0x03C */
    __IO uint32_t RXUICR;               /*!< Rx FIFO underflow interrupt clear reg, offset: 0x040 */
    __IO uint32_t MSTICR;               /*!< Master error interrupt clear reg,      offset: 0x044 */
    __IO uint32_t ICR;                  /*!< Interrupt clear reg,                   offset: 0x048 */
    __IO uint32_t DMACR;                /*!< DMA control reg,                       offset: 0x04C */
    __IO uint32_t DMATDLR;              /*!< DMA transimit data level reg,          offset: 0x050 */
    __IO uint32_t DMARDLR;              /*!< DMA revceive data level reg,           offset: 0x054 */
    __IO uint32_t IDR;                  /*!< Identiation reg,                       offset: 0x058 */
    __IO uint32_t SPIC_VERSION;         /*!< Version ID reg,                        offset: 0x05C */
    union
    {
        __IO uint8_t  BYTE;
        __IO uint16_t HALF;
        __IO uint32_t WORD;
    } DR[16];                           /*!< Data reg,                              offset: 0x060 */
    __IO uint32_t DM_DR[16];            /*!< Data mask data register,               offset: 0x0A0 */
    __IO uint32_t READ_FAST_SINGLE;     /*!< Fast read data cmd of flash,           offset: 0x0E0 */
    __IO uint32_t READ_DUAL_DATA;       /*!< Dual output read cmd of flash,         offset: 0x0E4 */
    __IO uint32_t READ_DUAL_ADDR_DATA;  /*!< Dual I/O read cmd of flash,            offset: 0x0E8 */
    __IO uint32_t READ_QUAD_DATA;       /*!< Quad output read cmd of flash,         offset: 0x0EC */
    __IO uint32_t READ_QUAD_ADDR_DATA;  /*!< Quad I/O read cmd of flash,            offset: 0x0F0 */
    __IO uint32_t WRITE_SINGLE;         /*!< Page program cmd of flash,             offset: 0x0F4 */
    __IO uint32_t WRITE_DUAL_DATA;      /*!< Dual data input program cmd of flash,  offset: 0x0F8 */
    __IO uint32_t WRITE_DUAL_ADDR_DATA; /*!< Dual addr & data program cmd of flash, offset: 0x0FC */
    __IO uint32_t WRITE_QUAD_DATA;      /*!< Quad data input program cmd of flash,  offset: 0x100 */
    __IO uint32_t WRITE_QUAD_ADDR_DATA; /*!< Quad addr & data program cmd of flash, offset: 0x104 */
    __IO uint32_t WRITE_ENABLE;         /*!< Write enabe cmd of flash,              offset: 0x108 */
    __IO uint32_t READ_STATUS;          /*!< Read status cmd of flash,              offset: 0x10C */
    __IO uint32_t CTRLR2;               /*!< Control reg 2,                         offset: 0x110 */
    __IO uint32_t FBAUDR;               /*!< Fast baudrate select,                  offset: 0x114 */
    __IO uint32_t USER_LENGTH;          /*!< Addr length reg,                       offset: 0x118 */
    __IO uint32_t AUTO_LENGTH;          /*!< Auto addr length reg,                  offset: 0x11C */
    __IO uint32_t VALID_CMD;            /*!< Valid cmd reg,                         offset: 0x120 */
    __IO uint32_t FLASH_SIZE_R;           /*!< Flash size reg,                        offset: 0x124 */
    __IO uint32_t FLUSH_FIFO;           /*!< Flush FIFO reg,                        offset: 0x128 */
    __IO uint32_t DUM_BYTE;             /*!< Dummy byte value,                      offset: 0x12C */
    __IO uint32_t TX_NDF;               /*!< Tx NDF,                                offset: 0x130 */
    __IO uint32_t DEVICE_INFO;          /*!< Device info,                           offset: 0x134 */
    __IO uint32_t TPR0;                 /*!< Timing parameters,                     offset: 0x138 */
    __IO uint32_t AUTO_LENGTH2;         /*!< Auto addr length reg 2,                offset: 0x13C */
} SPIC_TypeDef;

/* ================================================================================ */
/* ================                      PSRAM                     ================ */
/* ================================================================================ */
typedef struct
{
    __IO uint32_t CCR;           /*!< Configuration control register,          Address offset: 0x000 */
    __IO uint32_t DCR;           /*!< Device configuration control register,   Address offset: 0x004 */
    __IO uint32_t IOCR0;         /*!< I/O configuration control regsiter0,     Address offset: 0x008 */
    __IO uint32_t CSR;           /*!< Controller status register,              Address offset: 0x00c */
    __IO uint32_t DRR;           /*!< Device refresh/power-up register,        Address offset: 0x010 */
    __IO uint32_t RSVD0[4];      /*!< Reserved 0,                              Address offset: 0x014 */
    __IO uint32_t CMD_DPIN_NDGE; /*!< Device cmd/addr pin register (NEDGE),    Address offset: 0x024 */
    __IO uint32_t CMD_DPIN;      /*!< Device cmd/addr pin regsiter (PEDGE),    Address offset: 0x028 */
    __IO uint32_t CR_TDPIN;      /*!< Tie DPIN register (sw ctrl dfi_reset_n), Address offset: 0x02c */
    __IO uint32_t MR_INFO;       /*!< Mode latency information regster,        Address offset: 0x030 */
    __IO uint32_t MR0;           /*!< Device CR0 register,                     Address offset: 0x034 */
    __IO uint32_t MR1;           /*!< Device CR1 register,                     Address offset: 0x038 */
    __IO uint32_t RSVD1[9];      /*!< Reserved 1,                              Address offset: 0x03c */
    __IO uint32_t DPDRI;         /*!< DPIN data index register,                Address offset: 0x060 */
    __IO uint32_t DPDR;          /*!< DPIN data register,                      Address offset: 0x064 */
    __IO uint32_t RSVD2[35];     /*!< Reserved 2,                              Address offset: 0x068 */
    __IO uint32_t PCTL_SVN_ID;   /*!< PSRAM_LPC_CTRL version number,           Address offset: 0x0f4 */
    __IO uint32_t PCTL_IDR;      /*!< PSRAM_LPC_CTRL identification register,  Address offset: 0x0f8 */
    __IO uint32_t RSVD3[193];    /*!< Reserved 3,                              Address offset: 0x0fc */
    __IO uint32_t USER0_INDEX;   /*!< User extended index,                     Address offset: 0x400 */
    __IO uint32_t USER0_DATA;    /*!< User extended data,                      Address offset: 0x404 */
} PSRAMC_TypeDef;

#define SPIC0                           ((SPIC_TypeDef             *) SPIC0_BASE)
#define SPIC1                           ((SPIC_TypeDef             *) SPIC1_BASE)
#define SPIC2                           ((SPIC_TypeDef             *) SPIC2_BASE)

/****************************************************************************************
 * Flexible Memory Controller Calibration Definition
 ****************************************************************************************/
#define FMC_CAL_PATTERN                                     (0x5A5A12A5)

/****************************************************************************************
 * Flash power on delay time unit
 ****************************************************************************************/
#define FLASH_NOR_POWER_ON_DELAY_MS                         (3)

/****************************************************************************************
 * FMC Auto Write Settings
 ****************************************************************************************/
#define SPIC_AUTO_WR_ENABLE(idx, enable)                                            \
    (HAL_UPDATE32(0x40006024, 1 << (24 + (idx)), (enable) << (24 + (idx))))

#define PSRAMC_AUTO_WR_ENABLE(enable)                                               \
    (HAL_UPDATE32(0x40006024, (1 << 28), ((enable) << 28)))

/****************************************************************************************
 * SPIC Description
 ****************************************************************************************/
#define SPIC_NUM                                            (3)
#define SPIC_RX_FIFO_NUM                                    (32)
#define SPIC_TX_FIFO_NUM                                    (32)

/****************************************************************************************
 * SPIC Enumeration
 ****************************************************************************************/
typedef enum
{
    /* For SPIC0 Nor Flash */
    FLASH_NOR_MASTER_INTERNAL,      /* Master of internal nor flash (normal mode) */
    FLASH_NOR_MASTER_EXTERNAL,      /* Master of external nor flash (FT mode) */
    FLASH_NOR_SLAVE_EXTERNAL,       /* MCM nor flash as slave of of external master (MP mode) */

    /* For SPIC1 PSRAM */
    PSRAM_APM_QPI_TYPE,             /* APMemory QPI PSRAM (APS1604M-SQR) with SPIC as controller without PHY */
    PSRAM_APM_OPI_TYPE,             /* APMemory OPI PSRAM (APS6408L-OBx) with SPIC as controller with PHY */
    PSRAM_WINBOND_OPI_TYPE,         /* Winbond OPI PSRAM (W955D8MKY) with SPIC as controller with PHY */
} SPIC_INTERFACE_TYPE;

typedef enum
{
    FLASH_POW_SWR_AUDIO = 0,
    FLASH_POW_LDO_AUX2  = 1,
    FLASH_POW_HV33      = 3,
} FLASH_POW_TYPE;
/****************************************************************************************
 * SPIC Struct
 ****************************************************************************************/
typedef union
{
    uint16_t d16;
    struct
    {
        uint16_t SPIC_PAD_HS_MUX: 1;
        uint16_t SPIC_PAD_HS_OPI_MUX: 1; // only for SPIC1
        uint16_t SPIC_PAD_DUMMY2: 1;
        uint16_t SPIC_PAD_DUMMY3: 1;
        uint16_t SPIC_PAD_S: 1;
        uint16_t SPIC_PAD_SMT: 1;
        uint16_t SPIC_PAD_E3: 1;
        uint16_t SPIC_PAD_E2: 1;
        uint16_t SPIC_PAD_SHDN: 1;
        uint16_t SPIC_PAD_E: 1;
        uint16_t SPIC_PAD_DUMMY10: 1;
        uint16_t SPIC_PAD_DUMMY11: 1;
        uint16_t SPIC_PAD_O: 1;
        uint16_t SPIC_PAD_PUPDC: 1;
        uint16_t SPIC_PAD_PU: 1;
        uint16_t SPIC_PAD_PU_EN: 1;
    };
} SPIC_PAD_CONFIG_TYPE;

/****************************************************************************************
 * SPIC Functions and Variables
 ****************************************************************************************/
uint8_t spic_get_idx(SPIC_TypeDef *SPIC);
void spic_set_sio_pull_by_core(bool enable);
void spic0_flash_power_on(void);
bool spic_set_phy_ddr_en(SPIC_TypeDef *SPIC, bool enable);
bool spic_get_phy_ddr_en(SPIC_TypeDef *SPIC);

/* *INDENT-OFF* */
extern SPIC_TypeDef *spic_reg_base_map[SPIC_NUM];
extern void (*spic_init)(SPIC_TypeDef *SPIC);
extern void (*spic_set_interface)(SPIC_TypeDef *SPIC, SPIC_INTERFACE_TYPE type);
extern bool (*spic_interface_lpm_settings)(SPIC_TypeDef *SPIC, SPIC_INTERFACE_TYPE type, bool is_enter_lpm);

/****************************************************************************************
 * PSRAMC Functions and Variables
 ****************************************************************************************/
extern void (*psramc_init)(void);
extern void (*psramc_set_interface)(void);

#endif
