/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      sdcard.h
* @brief     This file provides all the sdcard firmware functions.
* @details
* @author   elliot chen
* @date      2021-01-28
* @version  v2.0
* *********************************************************************************************************
*/

#ifndef __SD_CARD_H
#define __SD_CARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sd_def.h"
#include "rtl876x.h"

/* Defines  ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* SDIO Commands  Index */
#define SDIO_GO_IDLE_STATE                       ((uint32_t)0)
#define SDIO_SEND_OP_COND                        ((uint32_t)1) /* SD Card doesn't support it */
#define SDIO_ALL_SEND_CID                        ((uint32_t)2)
#define SDIO_SET_REL_ADDR                        ((uint32_t)3) /* SDIO_SEND_REL_ADDR for SD Card */
#define SDIO_SET_DSR                             ((uint32_t)4)
#define SDIO_SLEEP_AWAKE                         ((uint32_t)5)
#define SDIO_SWITCH                              ((uint32_t)6)
#define SDIO_SEL_DESEL_CARD                      ((uint32_t)7)
#define SDIO_SEND_IF_COND                        ((uint32_t)8)
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
/* To set the address of the first write block to be erased. (For SD card only) */
#define SDIO_SD_ERASE_GRP_START                  ((uint32_t)32)
/* To set the address of the last write block of the continuous range to be erased. (For SD card only) */
#define SDIO_SD_ERASE_GRP_END                    ((uint32_t)33)
/* To set the address of the first write block to be erased.(For MMC card only spec 3.31) */
#define SDIO_ERASE_GRP_START                     ((uint32_t)35)
/* To set the address of the last write block of the continuous range to be erased. (For MMC card only spec 3.31) */
#define SDIO_ERASE_GRP_END                       ((uint32_t)36)
#define SDIO_ERASE                               ((uint32_t)38)
#define SDIO_FAST_IO                             ((uint32_t)39) /* SD Card doesn't support it */
#define SDIO_GO_IRQ_STATE                        ((uint32_t)40) /* SD Card doesn't support it */
#define SDIO_CMD_SEND_OP_COND                    ((uint32_t)41)
#define SDIO_LOCK_UNLOCK                         ((uint32_t)42)
#define SDIO_APP_CMD                             ((uint32_t)55)
#define SDIO_GEN_CMD                             ((uint32_t)56)
#define SDIO_NO_CMD                              ((uint32_t)64)

/* Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these
   commands. */
#define SDIO_APP_SD_SET_BUSWIDTH                 ((uint32_t)6)  /* For SD Card only */
#define SDIO_SD_APP_STAUS                        ((uint32_t)13) /* For SD Card only */
#define SDIO_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint32_t)22) /* For SD Card only */
#define SDIO_SD_APP_OP_COND                      ((uint32_t)41) /* For SD Card only */
#define SDIO_SD_APP_SET_CLR_CARD_DETECT          ((uint32_t)42) /* For SD Card only */
#define SDIO_SD_APP_SEND_SCR                     ((uint32_t)51) /* For SD Card only */
#define SDIO_SDIO_RW_DIRECT                      ((uint32_t)52) /* For SD I/O Card only */
#define SDIO_SDIO_RW_EXTENDED                    ((uint32_t)53) /* For SD I/O Card only */

/* Following commands are SD Card Specific security commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SD_APP_GET_MKB                      ((uint32_t)43) /* For SD Card only */
#define SDIO_SD_APP_GET_MID                      ((uint32_t)44) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RN1                  ((uint32_t)45) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RN2                  ((uint32_t)46) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RES2                 ((uint32_t)47) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RES1                 ((uint32_t)48) /* For SD Card only */
#define SDIO_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint32_t)18) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint32_t)25) /* For SD Card only */
#define SDIO_SD_APP_SECURE_ERASE                 ((uint32_t)38) /* For SD Card only */
#define SDIO_SD_APP_CHANGE_SECURE_AREA           ((uint32_t)49) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MKB             ((uint32_t)48) /* For SD Card only */

/* Exported constants --------------------------------------------------------*/
#define SD_TYPE_SDHC_OR_SDXC          1
#define SD_TYPE_SDSC                  0

/* OCR Register */
typedef enum
{
    SD_VDD_27_28           = BIT15,
    SD_VDD_28_29           = BIT16,
    SD_VDD_29_30           = BIT17,
    SD_VDD_30_31           = BIT18,
    SD_VDD_31_32           = BIT19,
    SD_VDD_32_33           = BIT20,
    SD_VDD_33_34           = BIT21,
    SD_VDD_34_35           = BIT22,
    SD_VDD_35_36           = BIT23,
    SD_CARD_CAPA_STATUS    = BIT30,
    //CARD_PWR_UP_STATUS    = BIT31
} T_SD_OCR_VOLTAGE_PROFILE;

typedef enum
{
    SDSC_ONLY           = 0,
    SDHC_SUPPORT        = 1
} HOST_CAPACITY_SUPPORT;

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
} T_SD_DEV_CSD;

typedef struct      /*Card Identification Data*/
{
    uint8_t  ManufacturerID;       /* ManufacturerID */
    uint16_t OEM_AppliID;          /* OEM/Application ID */
    uint32_t ProdName1;            /* Product Name part1 */
    uint8_t  ProdName2;            /* Product Name part2 */
    //uint8_t  ProdName[5];          /* Product Name */
    uint8_t  ProdRev;              /* Product Revision */
    uint32_t ProdSN;               /* Product Serial Number */
    uint8_t  Reserved1;            /* Reserved1 */
    uint16_t ManufactDate;         /* Manufacturing Date */
    uint8_t  CID_CRC;              /* CID CRC */
    uint8_t  Reserved2;            /* always 1 */
} T_SD_DEV_CID;

typedef union
{
    uint32_t d32;
    struct
    {
        uint32_t AcessMode      : 4;
        uint32_t CommandSystem  : 4;
        uint32_t DriveStrength  : 4;
        uint32_t PowerLimit     : 4;
        uint32_t RsvdForGroup5  : 4;
        uint32_t RsvdForGroup6  : 4;
        uint32_t Rsvd           : 7;
        uint32_t Mode           : 1;
    } b;
} T_CMD6_ARG;

typedef struct
{
    uint8_t initialized;
    T_SD_DEV_CSD dev_csd;
    // T_SD_DEV_CID dev_cid;
    uint64_t  capacity;            /* Card Capacity */
    uint32_t  block_size;          /* Card Block Size */
    uint16_t  rca;
    uint8_t   card_type;           /* Store relative card address information */
    /* Variables which uesd for sd card information analysis */
    uint32_t CSD_Tab[4];
    uint32_t CID_Tab[4];
} T_SD_DEV_INFO;

/* Exported functions ------------------------------------------------------- */
T_SD_STATUS SD_InitCard(void *p_dev);
T_SD_STATUS SD_ResetCard(void);
T_SD_STATUS SD_SelectCmd(FunctionalState NewState);
T_SD_STATUS SD_SetHighSpeedMode(FunctionalState NewState, const uint32_t RawCSDTab[4]);
T_SD_STATUS SD_SetBusWide(uint32_t bus_width);
T_SD_STATUS SD_SleepOrAwakeCmd(FunctionalState NewState);
T_SD_STATUS SD_GoInactiveState(void);
T_SD_STATUS SD_GetCardInfo(void *p_info);
T_SD_STATUS SD_GetCardStatus(uint32_t *pStatus);
T_SD_STATUS SD_WaitCardFinishProgram(void);
T_SD_STATUS SD_SetBlockLen(uint32_t block_len);
T_SD_STATUS SD_ReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS SD_ReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                              uint16_t block_count);
T_SD_STATUS SD_WriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS SD_WriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                               uint16_t block_count);
T_SD_STATUS SD_Earse(uint32_t start_addr, uint32_t end_addr);
T_SD_STATUS SD_StopTransfer(void);
T_SD_STATUS SD_WaitCmdDatComplete(void);
T_SD_STATUS SD_AsyncReadSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS SD_AsyncReadMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                   uint16_t block_count);
T_SD_STATUS SD_AsyncWriteSingleBlock(uint32_t sector, uint32_t buf, uint16_t block_size);
T_SD_STATUS SD_AsyncWriteMultiBlock(uint32_t sector, uint32_t buf, uint16_t block_size,
                                    uint16_t block_count);
void SD_RegisterFops(T_SDIF_PROT_PROP **pp_sd_if_fops);

#ifdef __cplusplus
}
#endif

#endif /*__SD_CARD_H*/
/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/
