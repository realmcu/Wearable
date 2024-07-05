/**
 *****************************************************************************************
 *     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file    flash_basic.h
 * @brief   Flash basic implementation header file
 * @author  Yao-Yu
 * @date    2020-07-08
 * @version v0.1
 * ***************************************************************************************
 */

#ifndef _FLASH_BASIC_H
#define _FLASH_BASIC_H

#include "spic_basic.h"

#define _PACKED_                __attribute__((packed))

/****************************************************************************************
 * Flash Vendor Definition
 ****************************************************************************************/
#define FLASH_VENDOR_MXIC                   0xC2
#define FLASH_VENDOR_GD                     0xC8
#define FLASH_VENDOR_MICRON                 0x20
#define FLASH_VENDOR_WINBOND                0xEF
#define FLASH_VENDOR_ATMEL                  0x1F
#define FLASH_VENDOR_SST                    0xBF
#define FLASH_VENDOR_SPANSION               0x01
#define FLASH_VENDOR_PARAGON                0xE0
#define FLASH_VENDOR_FUDAN                  0xA1
#define FLASH_VENDOR_ESMT                   0x1C
#define FLASH_VENDOR_PUYA                   0x85

/****************************************************************************************
 * Flash Information Definition
 ****************************************************************************************/
#define FMC_MAIN0                           (0x00800000)
#define FMC_MAIN1                           (0x02800000)
#define FMC_MAIN2                           (0x03000000)
#define FLASH0_SIZE_MAX                     (32 * 1024 * 1024)
#define FLASH1_SIZE_MAX                     ( 8 * 1024 * 1024)
#define FLASH2_SIZE_MAX                     ( 8 * 1024 * 1024)
#define FLASH_PAGE_SIZE                     (256)
#define FLASH_OFFSET_TO_NON_CACHE           (0)
#define FLASH_BP_FIELDS_OFFSET              (0x02)
#define FLASH_TABLE_MAGIC_PATTERN           (0x5a5a12a5)
#define BIT_FLASH_STATUS_WIP                (0x01)
#define BIT_FLASH_STATUS_WEL                (0x02)

/****************************************************************************************
 * Flash Macro
 ****************************************************************************************/
#define FLASH_QUERY_INFO_TABLE_ENTRY_NUM    (sizeof(flash_query_info_table) / sizeof(FLASH_QUERY_INFO_TABLE_STRUCT))

#define IS_SAME_FLASH_PAGE(addr0, addr1)    (((addr0) & 0xFFFFFF00) == ((addr1) & 0xFFFFFF00))

#define FLASH_SET_RD_CMD(idx, cmd_type, cmd_set, addr_ch, data_ch)      \
    do                                                                  \
    {                                                                   \
        flash_info[idx]->rd_cmd = flash_info[idx]->cmd_type.cmd_set;    \
        flash_info[idx]->rd_ch.addr = addr_ch;                          \
        flash_info[idx]->rd_ch.data = data_ch;                          \
    }                                                                   \
    while (0)

#define FLASH_SET_WR_CMD(idx, cmd_type, cmd_set, addr_ch, data_ch)      \
    do                                                                  \
    {                                                                   \
        flash_info[idx]->wr_cmd = flash_info[idx]->cmd_type.cmd_set;    \
        flash_info[idx]->wr_ch.addr = addr_ch;                          \
        flash_info[idx]->wr_ch.data = data_ch;                          \
    }                                                                   \
    while (0)

/****************************************************************************************
 * Flash Enumeration
 ****************************************************************************************/
typedef enum
{
    FLASH_IDX_SPIC0,                /* Flash controlled by SPIC0 */
    FLASH_IDX_SPIC1,                /* Flash controlled by SPIC1 */
    FLASH_IDX_SPIC2,                /* Flash controlled by SPIC2 */
    FLASH_IDX_MAX
} FLASH_IDX_TYPE;

typedef enum
{
    FLASH_EXIST_NONE        = 0,
    FLASH_EXIST_BASIC_CMD   = 1,
    FLASH_EXIST_ADV_CMD     = 2,
    FLASH_EXIST_QUERY_INFO  = 4,
    FLASH_EXIST_ALL         = 7,
} FLASH_EXIST_LV;

typedef enum
{
    FLASH_MASTER_INTERNAL,          /* Master of internal flash (normal mode) */
    FLASH_MASTER_EXTERNAL,          /* Master of external flash (FT mode) */
    FLASH_SLAVE_EXTERNAL            /* MCM flash as slave of of external master (MP mode) */
} FLASH_INTERFACE_TYPE;

typedef enum
{
    FLASH_WREN_REG,
    FLASH_WREN_DATA,
} FLASH_WREN_MODE;

typedef enum
{
    FLASH_1_BIT_MODE,
    FLASH_2_BIT_MODE,
    FLASH_4_BIT_MODE,
    FLASH_8_BIT_MODE,
} FLASH_BIT_MODE;

typedef enum
{
    FLASH_BP_LV_0       = 0x00000000,
    FLASH_BP_LV_1       = 0x00010000,
    FLASH_BP_LV_2       = 0x00020000,
    FLASH_BP_LV_3       = 0x00040000,
    FLASH_BP_LV_4       = 0x00080000,
    FLASH_BP_LV_5       = 0x00100000,
    FLASH_BP_LV_6       = 0x00200000,
    FLASH_BP_LV_7       = 0x00400000,
    FLASH_BP_LV_8       = 0x00800000,
    FLASH_BP_LV_9       = 0x01000000,
} FLASH_BP_LV;

typedef enum
{
    FLASH_XTAL_10MHZ,
    FLASH_XTAL_20MHZ,
    FLASH_PLL_30MHZ,
    FLASH_PLL_40MHZ,
    FLASH_PLL_60MHZ,
    FLASH_PLL_80MHZ,
    FLASH_CLK_MAX
} FLASH_CLK_FREQ;

typedef enum
{
    FLASH_ERASE_SECTOR  = 1,
    FLASH_ERASE_BLOCK   = 2,
    FLASH_ERASE_CHIP    = 4,
} FLASH_ERASE_MODE;

typedef enum
{
    FLASH_RET_UNKNOWN,
    FLASH_PARAM_INVALID,

    FLASH_RET_LOCK_FAILED,
    FLASH_RET_UNLOCK_FAILED,
    FLASH_RET_WAIT_BUSY_FAILED,

    FLASH_RET_INFO_NOT_RESET,

    FLASH_RET_NOT_EXIST_BASIC_CMD,
    FLASH_RET_NOT_EXIST_ADV_CMD,
    FLASH_RET_NOT_EXIST_QUERY_INFO,

    FLASH_RET_VENDOR_NOT_SUPPORT,
    FLASH_RET_DEV_NOT_SUPPORT,
    FLASH_RET_CMD_NOT_SUPPORT,

    FLASH_RET_SET_BIT_MODE_FAILED,
    FLASH_RET_BIT_MODE_NOT_SUPPORT,

    FLASH_RET_ADDR_OUT_OF_RANGE,

    FLASH_RET_CAL_FAILED,

    FLASH_RET_SUCCESS
} FLASH_RETURN_TYPE;

/****************************************************************************************
 * SPIC Structure
 ****************************************************************************************/
typedef struct _PACKED_ _FLASH_BASIC_CMD_STRUCT
{
    uint8_t cmd_rd_id;              /* Read flash ID cmd */
    uint8_t cmd_rd_sr;              /* Read status register */
    uint8_t cmd_wr_sr;              /* Write status register */
    uint8_t cmd_rd_data;            /* Single read cmd */
    uint8_t cmd_pp;                 /* Single page program cmd */
    uint8_t cmd_sector_e;           /* Erase cmd for flash sector (4kB) */
    uint8_t cmd_block_e;            /* Erase cmd for flash block (64KB) */
    uint8_t cmd_chip_e;             /* Erase cmd for whole flash */
    uint8_t cmd_wr_en;              /* Write enable cmd */
    uint8_t cmd_wr_en_vol;          /* Write enable for volatile status register cmd */
    uint8_t cmd_wr_di;              /* Write disable cmd */
    uint8_t cmd_dp;                 /* Enter deep power down mode cmd */
    uint8_t cmd_dp_release;         /* Release from deep power down mode cmd */
    uint8_t cmd_en_reset;           /* Enable software reset cmd */
    uint8_t cmd_reset;              /* Software reset cmd */
} FLASH_BASIC_CMD_STRUCT;

typedef struct _PACKED_ _FLASH_ADV_CMD_STRUCT
{
    uint8_t cmd_rd_dual_o;          /* Dual data read cmd */
    uint8_t cmd_rd_dual_io;         /* Dual data/addr read cmd */
    uint8_t cmd_rd_quad_o;          /* Quad data read cmd */
    uint8_t cmd_rd_quad_io;         /* Quad data/addr read cmd */
    uint8_t cmd_pp_quad_i;          /* Quad page program cmd (1-1-4) */
    uint8_t cmd_pp_quad_ii;         /* Quad page program cmd (1-4-4) */
    uint8_t cmd_rd_sr2;             /* Read status register 2 */
    uint8_t cmd_wr_sr2;             /* Write status register 2 */
    uint8_t cmd_rd_sr3;             /* Read status register 3 */
    uint8_t cmd_wr_sr3;             /* Write status register 3 */
    uint8_t cmd_rd_cr;              /* Read config cmd */
    uint8_t cmd_hpm;                /* High-performance or continuous read cmd */
    uint8_t cmd_suspend;            /* Suspend cmd */
    uint8_t cmd_resume;             /* Resume cmd */
} FLASH_ADV_CMD_STRUCT;

typedef struct _PACKED_ _FLASH_QUERY_INFO_STRUCT
{
    /* For following fields, except flash_size, equal to 0xFF means not support */
    uint32_t flash_size;            /* Flash size in byte */
    uint8_t qebo;                   /* QE bit offset in status register */
    uint8_t wsbo;                   /* Write suspend flag bit offset in status register */
    uint8_t esbo;                   /* Erase suspend flag bit offset in status register */
    uint8_t tbbo;                   /* Top-bottom bit offset in status register */
    uint8_t cmpbo;                  /* Complement bit offset in status register */
    uint8_t bp_all;                 /* Block protect all level in status register */
    uint8_t bp_mask;                /* Block protect configurable bit mask in status register */
} FLASH_QUERY_INFO_STRUCT;

typedef struct _PACKED_ _FLASH_ACCESS_CH
{
    SPIC_CFG_CH cmd;
    SPIC_CFG_CH addr;
    SPIC_CFG_CH data;
} FLASH_ACCESS_CH;

typedef struct _PACKED_ _FLASH_DELAY_INFO_STRUCT
{
    uint8_t path_len;               /* Path delay from pad to internal SPI controller */
    uint8_t dummy_len;              /* Delay cycles for data read, not include path delay */
} FLASH_DELAY_INFO_STRUCT;

typedef struct _PACKED_ _FLASH_INFO_STRUCT
{
    uint8_t manu_id;
    uint16_t device_id;

    uint8_t rd_cmd;                 /* Specify the read cmd for current bit mode */
    uint8_t wr_cmd;                 /* Specify the write cmd for current bit mode */

    FLASH_BASIC_CMD_STRUCT basic;
    FLASH_ADV_CMD_STRUCT adv;
    FLASH_QUERY_INFO_STRUCT *query;

    FLASH_ACCESS_CH rd_ch;
    FLASH_ACCESS_CH wr_ch;
    FLASH_DELAY_INFO_STRUCT delay;
} FLASH_INFO_STRUCT;

typedef union _PACKED_ _FLASH_ACCESS_INFO
{
    uint32_t d32[2];
    struct
    {
        uint32_t cmd: 8;
        uint32_t addr;
        uint32_t addr_len: 3;
        uint32_t cmd_ch: 2;
        uint32_t addr_ch: 2;
        uint32_t data_ch: 2;
        uint32_t dummy_len: 15;
    };
} FLASH_ACCESS_INFO;

typedef struct _PACKED_ _FLASH_QUERY_INFO_TABLE_STRUCT
{
    uint8_t manu_id;
    uint16_t device_id;
    FLASH_QUERY_INFO_STRUCT query;
} FLASH_QUERY_INFO_TABLE_STRUCT;


typedef struct _FLASH_SETTINGS_CFG
{
    uint32_t flash_task_priority: 1;
    uint32_t flash_task_stack_depth: 2;
    uint32_t flash_task_wait_tick : 3;
    uint32_t flash_task_linklist_length: 2;
    uint32_t flash_dma_ch: 4;
    uint32_t flash_clk: 3;
    uint32_t ftl_page_size;
    uint32_t spic_baud: 12;
    uint32_t data_addr_check_enable: 1;
    uint32_t power_mode: 1;
    uint32_t hardfault_record_cfg: 5;
    uint32_t bp_lv: 4;
    uint32_t cmp_en: 1;
    uint32_t hpm_en: 1;
    uint32_t wait_max_retry: 27;
    uint32_t erase_max_retry: 4;
    uint32_t dp_release_cyc_len: 3;
    uint32_t dp_release_delay_us: 8;
    uint32_t sw_reset_delay_us: 8;
    uint32_t wait_busy_delay_us: 4;
    uint32_t init_cmd_from_cfg: 1;
    uint32_t init_query_from_cfg: 1;
    uint32_t init_cmd_rdid: 8;
    uint32_t init_cmd_rd_data: 8;
    uint32_t init_cmd_dp_release: 8;
    uint32_t init_cmd_en_reset: 8;
    uint32_t init_cmd_reset: 8;
    FLASH_QUERY_INFO_TABLE_STRUCT query_info;
} __attribute__((packed)) FLASH_SETTINGS_CFG;
/****************************************************************************************
 * Flash Function Prototype
 ****************************************************************************************/
void flash_set_flash_exist(FLASH_IDX_TYPE idx, FLASH_EXIST_LV exist_lv);
FLASH_EXIST_LV flash_get_flash_exist(FLASH_IDX_TYPE idx);
void flash_reset_flash_exist(FLASH_IDX_TYPE idx);
void flash_set_interface(FLASH_INTERFACE_TYPE type);
FLASH_RETURN_TYPE flash_init(FLASH_IDX_TYPE idx, FLASH_INFO_STRUCT *info);
void flash_power_ctrl(bool is_on);
FLASH_RETURN_TYPE flash_dp_ctrl(FLASH_IDX_TYPE idx, bool enable);
FLASH_RETURN_TYPE flash_sw_reset(FLASH_IDX_TYPE idx);
uint32_t flash_get_rdid(FLASH_IDX_TYPE idx);
FLASH_RETURN_TYPE flash_update_cmd(FLASH_IDX_TYPE idx);
FLASH_RETURN_TYPE flash_set_1_bit_mode(FLASH_IDX_TYPE idx);
FLASH_RETURN_TYPE flash_set_2_bit_mode(FLASH_IDX_TYPE idx);
FLASH_RETURN_TYPE flash_set_4_bit_mode(FLASH_IDX_TYPE idx);
FLASH_RETURN_TYPE flash_set_qe_bit(FLASH_IDX_TYPE idx, bool enable);
FLASH_RETURN_TYPE flash_get_bit_mode(FLASH_IDX_TYPE idx, FLASH_BIT_MODE *mode);
uint32_t flash_get_flash_size(FLASH_IDX_TYPE idx);
FLASH_RETURN_TYPE flash_get_tb_bit(FLASH_IDX_TYPE idx, bool *from_bottom);
FLASH_RETURN_TYPE flash_set_tb_bit(FLASH_IDX_TYPE idx, bool from_bottom);
FLASH_RETURN_TYPE flash_get_bp_all_lv(FLASH_IDX_TYPE idx, uint8_t *bp_lv);
FLASH_IDX_TYPE flash_get_idx_and_offset(uint32_t *addr);

/****************************************************************************************
 * Flash Extern Variables
 ****************************************************************************************/
/* *INDENT-OFF* */
extern uint32_t FlashSrcClk;
extern FLASH_INFO_STRUCT default_flash_info;
extern const FLASH_QUERY_INFO_TABLE_STRUCT flash_query_info_table[];
extern FLASH_RETURN_TYPE(*flash_cmd_rx)(FLASH_IDX_TYPE idx, FLASH_ACCESS_INFO *info, uint8_t *buf, uint8_t len);
extern FLASH_RETURN_TYPE(*flash_cmd_tx)(FLASH_IDX_TYPE idx, FLASH_ACCESS_INFO *info, uint8_t *buf, uint8_t len);
extern FLASH_RETURN_TYPE(*flash_wait_busy)(FLASH_IDX_TYPE idx);
extern void (*flash_activate)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_update_mxic_cmd)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_update_gd_cmd)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_update_winbond_cmd)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_update_esmt_cmd)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_update_fudan_cmd)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_update_new_vendor_cmd)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_write_enable)(FLASH_IDX_TYPE idx, FLASH_WREN_MODE mode);
extern FLASH_RETURN_TYPE(*flash_set_status_regs)(FLASH_IDX_TYPE idx, uint8_t *sr1, uint8_t *sr2, uint8_t *sr3);
extern FLASH_RETURN_TYPE(*flash_get_status_regs)(FLASH_IDX_TYPE idx, uint8_t *sr1, uint8_t *sr2, uint8_t *sr3);
extern FLASH_RETURN_TYPE(*flash_load_query_info)(FLASH_IDX_TYPE idx);
extern FLASH_RETURN_TYPE(*flash_set_bit_mode)(FLASH_IDX_TYPE idx, FLASH_BIT_MODE mode);
extern FLASH_RETURN_TYPE(*flash_set_auto_mode)(FLASH_IDX_TYPE idx, FLASH_BIT_MODE mode);
extern FLASH_RETURN_TYPE(*flash_get_bp_lv)(FLASH_IDX_TYPE idx, uint8_t *bp_lv);
extern FLASH_RETURN_TYPE(*flash_set_bp_lv)(FLASH_IDX_TYPE idx, uint8_t bp_lv);
extern FLASH_RETURN_TYPE(*flash_read)(uint32_t addr, uint8_t *data, uint32_t byte_len);
extern FLASH_RETURN_TYPE(*flash_write)(uint32_t addr, uint8_t *data, uint32_t byte_len);
extern FLASH_RETURN_TYPE(*flash_erase)(uint32_t addr, FLASH_ERASE_MODE mode);
extern FLASH_RETURN_TYPE(*flash_unlock_bp_by_addr)(uint32_t addr, uint8_t *old_bp_lv);
extern FLASH_RETURN_TYPE(*flash_unlock_access)(uint32_t addr, uint8_t *data, uint32_t byte_len, FLASH_ERASE_MODE mode);
#define flash_unlock_write(addr, data, len) flash_unlock_access(addr, data, len, (FLASH_ERASE_MODE)0)
#define flash_unlock_erase(addr, mode) flash_unlock_access(addr, NULL, 0, mode)

#endif
