/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      mmc.h
* @brief     This file provides all the mmc firmware functions.
* @details
* @author    elliot chen
* @date      2021-04-15
* @version   v2.0
* *********************************************************************************************************
*/

#ifndef __MMC_H
#define __MMC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_def.h"
#include "rtl876x.h"

/* Defines  ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* MMC Commands  Index */
#define SDIO_GO_IDLE_STATE                       ((uint32_t)0)
#define SDIO_GO_PRE_IDLE_STATE                   ((uint32_t)0)
#define SDIO_BOOT_INITIATION                     ((uint32_t)0)
#define SDIO_SEND_OP_COND                        ((uint32_t)1) /* SD Card doesn't support it */
#define SDIO_ALL_SEND_CID                        ((uint32_t)2)
#define SDIO_SET_REL_ADDR                        ((uint32_t)3) /* SDIO_SEND_REL_ADDR for SD Card */
#define SDIO_SET_DSR                             ((uint32_t)4)
#define SDIO_SLEEP_AWAKE                         ((uint32_t)5)
#define SDIO_SWITCH                              ((uint32_t)6)
#define SDIO_SEL_DESEL_CARD                      ((uint32_t)7)
#define SDIO_SEND_EXT_CSD                        ((uint32_t)8)
#define SDIO_SEND_CSD                            ((uint32_t)9)
#define SDIO_SEND_CID                            ((uint32_t)10)
#define SDIO_READ_DAT_UNTIL_STOP                 ((uint32_t)11) /* SD Card doesn't support it */
#define SDIO_STOP_TRANSMISSION                   ((uint32_t)12)
#define SDIO_SEND_STATUS                         ((uint32_t)13)
#define SDIO_BUSTEST_R                           ((uint32_t)14)
#define SDIO_GO_INACTIVE_STATE                   ((uint32_t)15)
#define SDIO_SET_BLOCKLEN                        ((uint32_t)16)
#define SDIO_READ_SINGLE_BLOCK                   ((uint32_t)17)
#define SDIO_READ_MULT_BLOCK                     ((uint32_t)18)
#define SDIO_BUSTEST_W                           ((uint32_t)19)
#define SDIO_WRITE_DAT_UNTIL_STOP                ((uint32_t)20) /* SD Card doesn't support it */
#define SDIO_SET_BLOCK_COUNT                     ((uint32_t)23) /* SD Card doesn't support it */
#define SDIO_WRITE_SINGLE_BLOCK                  ((uint32_t)24)
#define SDIO_WRITE_MULT_BLOCK                    ((uint32_t)25)
#define SDIO_PROG_CID                            ((uint32_t)26) /* reserved for manufacturers */
#define SDIO_PROG_CSD                            ((uint32_t)27)
#define SDIO_SET_WRITE_PROT                      ((uint32_t)28)
#define SDIO_CLR_WRITE_PROT                      ((uint32_t)29)
#define SDIO_SEND_WRITE_PROT                     ((uint32_t)30)
#define SDIO_SEND_WRITE_PROT_TYPE                ((uint32_t)31)
/* To set the address of the first write block to be erased.(For MMC card only spec 3.31) */
#define SDIO_ERASE_GRP_START                     ((uint32_t)35)
/* To set the address of the last write block of the continuous range to be erased. (For MMC card only spec 3.31) */
#define SDIO_ERASE_GRP_END                       ((uint32_t)36)
#define SDIO_ERASE                               ((uint32_t)38)
#define SDIO_FAST_IO                             ((uint32_t)39) /* SD Card doesn't support it */
#define SDIO_GO_IRQ_STATE                        ((uint32_t)40) /* SD Card doesn't support it */
#define SDIO_LOCK_UNLOCK                         ((uint32_t)42)
#define SDIO_APP_CMD                             ((uint32_t)55)
#define SDIO_GEN_CMD                             ((uint32_t)56)


/* Exported constants --------------------------------------------------------*/

#define MMC_TYPE_HC_OR_XC             1
#define MMC_TYPE_SC                   0

/* OCR Register */
typedef enum
{
    MMC_VDD_27_28           = BIT15,
    MMC_VDD_28_29           = BIT16,
    MMC_VDD_29_30           = BIT17,
    MMC_VDD_30_31           = BIT18,
    MMC_VDD_31_32           = BIT19,
    MMC_VDD_32_33           = BIT20,
    MMC_VDD_33_34           = BIT21,
    MMC_VDD_34_35           = BIT22,
    MMC_VDD_35_36           = BIT23,
    MMC_CARD_CAPA_STATUS    = BIT30,
    //MMC_CARD_PWR_UP_STATUS    = BIT31
} T_MMC_OCR_VOLTAGE_PROFILE;

typedef struct       /* Card Specific Data */
{
    uint8_t  CSDStruct;            /* CSD structure */
    uint8_t  SysSpecVersion;       /* System specification version */
    uint8_t  Reserved1;            /* Reserved */
    uint8_t  TAAC;                 /* Data read access-time 1 */
    uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
    uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
    uint16_t CardComdClasses;      /* Card command classes */
    uint8_t  RdBlockLen;           /* Max. read data block length */
    uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
    uint8_t  WrBlockMisalign;      /* Write block misalignment */
    uint8_t  RdBlockMisalign;      /* Read block misalignment */
    uint8_t  DSRImpl;              /* DSR implemented */
    uint8_t  Reserved2;            /* Reserved */
    uint32_t DeviceSize;           /* Device Size */
    uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
    uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
    uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
    uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
    uint8_t  DeviceSizeMul;        /* Device size multiplier */
    uint8_t  EraseGrSize;          /* Erase group size */
    uint8_t  EraseGrMul;           /* Erase group size multiplier */
    uint8_t  WrProtectGrSize;      /* Write protect group size */
    uint8_t  WrProtectGrEnable;    /* Write protect group enable */
    uint8_t  ManDeflECC;           /* Manufacturer default ECC */
    uint8_t  WrSpeedFact;          /* Write speed factor */
    uint8_t  MaxWrBlockLen;        /* Max. write data block length */
    uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
    uint8_t  Reserved3;            /* Reserded */
    uint8_t  ContentProtectAppli;  /* Content protection application */
    uint8_t  FileFormatGrouop;     /* File format group */
    uint8_t  CopyFlag;             /* Copy flag (OTP) */
    uint8_t  PermWrProtect;        /* Permanent write protection */
    uint8_t  TempWrProtect;        /* Temporary write protection */
    uint8_t  FileFormat;           /* File Format */
    uint8_t  ECC;                  /* ECC code */
    uint8_t  CSD_CRC;              /* CSD CRC */
    uint8_t  Reserved4;            /* always 1*/
} T_MMC_DEV_CSD;

typedef union
{
    uint8_t d[512];
    struct
    {
        uint8_t rsvd0[134];
        uint8_t sec_bad_blk_mgmnt;

        uint8_t rsvd1[1];
        uint32_t enh_start_addr;
        uint8_t enh_size_mult[3];
        uint8_t gp_size_mult[12];
        uint8_t partitions_setting_completed;
        uint8_t partitions_attribute;
        uint8_t max_enh_size_mult[3];
        uint8_t partitioning_support;
        uint8_t hpi_mgmt;
        uint8_t rst_n_function;
        uint8_t bkops_en;
        uint8_t bkops_start;

        uint8_t rsvd2[1];
        uint8_t wr_rel_param;
        uint8_t wr_rel_set;
        uint8_t rpmb_size_mult;
        uint8_t fw_config;

        uint8_t rsvd3[1];
        uint8_t user_wp;

        uint8_t rsvd4[1];
        uint8_t boot_wp;

        uint8_t rsvd5[1];
        uint8_t erase_group_def;

        uint8_t rsvd6[1];
        uint8_t boot_bus_width;
        uint8_t boot_cfg_prot;
        uint8_t partition_config;

        uint8_t rsvd7[1];
        uint8_t erased_mem_cont;

        uint8_t rsvd8[1];
        uint8_t bus_width;

        uint8_t rsvd9[1];
        uint8_t hs_timing;

        uint8_t rsvd10[1];
        uint8_t power_class;

        uint8_t rsvd11[1];
        uint8_t cmd_set_rev;

        uint8_t rsvd12[1];
        uint8_t cmd_set;
        uint8_t ext_csd_rev;

        uint8_t rsvd13[1];
        uint8_t csd_structure;

        uint8_t rsvd14[1];
        uint8_t card_type;

        uint8_t rsvd15[1];
        uint8_t out_out_interrupt_time;
        uint8_t partition_switch_time;
        uint8_t pwr_cl_52_195;
        uint8_t pwr_cl_26_195;
        uint8_t pwr_cl_52_360;
        uint8_t pwr_cl_26_360;

        uint8_t rsvd16[1];
        uint8_t min_perf_r_4_26;
        uint8_t min_perf_w_4_26;
        uint8_t min_perf_r_8_26_4_52;
        uint8_t min_perf_w_8_26_4_52;
        uint8_t min_perf_r_8_52;
        uint8_t min_perf_w_8_52;

        uint8_t rsvd17[1];
        uint32_t sec_count;

        uint8_t rsvd18[1];
        uint8_t s_a_timeout;

        uint8_t rsvd19[1];
        uint8_t s_c_vccq;
        uint8_t s_c_vcc;
        uint8_t hc_wp_grp_size;
        uint8_t rel_wr_sec_c;
        uint8_t erase_timeout_mult;
        uint8_t hc_erase_grp_size;
        uint8_t acc_size;
        uint8_t boot_size_multi;

        uint8_t rsvd20[1];
        uint8_t boot_info;
        uint8_t sec_trim_mult;
        uint8_t sec_earse_mult;
        uint8_t sec_feature_support;
        uint8_t trim_mult;

        uint8_t rsvd21[1];
        uint8_t min_perf_ddr_r_8_52;
        uint8_t min_perf_ddr_w_8_52;

        uint8_t rsvd22[2];
        uint8_t pwr_cl_ddr_52_195;
        uint8_t pwr_cl_ddr_52_360;

        uint8_t rsvd23[1];
        uint8_t ini_timeout_ap;
        uint32_t correctly_prg_sectors_num;
        uint8_t bkops_status;

        uint8_t rsvd24[255];
        uint8_t bkops_support;
        uint8_t hpi_features;
        uint8_t s_cmd_set;

        uint8_t rsvd25[7];
    } __attribute__((packed)) b;
} T_MMC_EXT_CSD;

typedef union      /*Card Identification Data*/
{
    uint8_t d[16];
    struct
    {
        uint8_t         : 1;
        uint8_t  crc    : 7;
        uint8_t  mdt;
        uint32_t psn;
        uint8_t  prv;
        uint8_t  pnm[6];
        uint8_t  oid;
        uint8_t  cbx    : 2;
        uint8_t         : 6;
        uint8_t  mid;
    } __attribute__((packed)) b;
} T_MMC_DEV_CID;

typedef struct
{
    uint8_t initialized;
    T_MMC_DEV_CSD dev_csd;
    // T_MMC_DEV_CID dev_cid;
    uint64_t capacity;            /* Card Capacity */
    uint32_t block_size;          /* Card Block Size */
    uint32_t rca;                 /* Store relative card address information */
    uint8_t  card_type;
    /* Variables which uesd for sd card information analysis */
    uint32_t CSD_Tab[4];
    uint32_t CID_Tab[4];
} T_MMC_DEV_INFO;

/* Exported functions ------------------------------------------------------- */
T_SD_STATUS MMC_InitCard(void *p_dev);
T_SD_STATUS MMC_ResetCard(void);
T_SD_STATUS MMC_SelectCmd(FunctionalState NewState);
T_SD_STATUS MMC_SetBusWide(uint32_t bus_width);
T_SD_STATUS MMC_SleepOrAwakeCmd(FunctionalState NewState);
T_SD_STATUS MMC_GoInactiveState(void);
T_SD_STATUS MMC_GetCardInfo(void *p_info);
T_SD_STATUS MMC_GetCardStatus(uint32_t *pStatus);
T_SD_STATUS MMC_WaitCardFinishProgram(void);
T_SD_STATUS MMC_SetBlockLen(uint32_t block_len);
T_SD_STATUS MMC_ReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS MMC_ReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                               uint16_t block_count);
T_SD_STATUS MMC_WriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS MMC_WriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                uint16_t block_count);
T_SD_STATUS MMC_Earse(uint32_t start_addr, uint32_t end_addr);
T_SD_STATUS MMC_StopTransfer(void);
T_SD_STATUS MMC_WaitCmdDatComplete(void);
T_SD_STATUS MMC_AsyncReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS MMC_AsyncReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                    uint16_t block_count);
T_SD_STATUS MMC_AsyncWriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS MMC_AsyncWriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                     uint16_t block_count);
void MMC_RegisterFops(T_SDIF_PROT_PROP **pp_sd_if_fops);
void MMC_Deinit(void);
#ifdef __cplusplus
}
#endif

#endif /*__MMC_H*/
/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
