#ifndef _ARM_AHB_CACHE_CTRL_H_
#define _ARM_AHB_CACHE_CTRL_H_

#include <stdbool.h>
#include <stdint.h>
#include "rtl876x.h"

/********************************************************************************
 * Macro
********************************************************************************/
#define CTRL_ENABLE_Pos                (0UL) /*!< Position of . */
#define CTRL_ENABLE_Msk                (0x1UL)
#define ALLOW_NSEC_MAINT_LINES_Pos     (17UL) /*!< Position of . */
#define ALLOW_NSEC_MAINT_LINES_Msk     (0x1UL << ALLOW_NSEC_MAINT_LINES_Pos)

#define TRIG_CLEAN_Pos                 (0UL) /*!< Position of . */
#define TRIG_CLEAN_Msk                 (0x1UL)
#define TRIG_INVALIDATE_Pos            (1UL) /*!< Position of . */
#define TRIG_INVALIDATE_Msk            (0x1UL << TRIG_INVALIDATE_Pos)
#define SECURITY_Pos                   (2UL) /*!< Position of . */
#define SECURITY_Msk                   (0x1UL << SECURITY_Pos)
#define ADDR_Pos                       (5UL) /*!< Position of . */
#define ADDR_Msk                       (0x7FFFFFFUL << ADDR_Pos)

#define MAINT_DONE_Pos                 (2UL) /*!< Position of . */
#define MAINT_DONE_Msk                 (0x1UL << MAINT_DONE_Pos)

#define MAINT_STATUS_CACHE_ENABLED_Pos (0UL) /*!< Position of . */
#define MAINT_STATUS_CACHE_ENABLED_Msk (0x1UL)

/********************************************************************************
 * Regiter type
********************************************************************************/
/* Refer to: https://developer.arm.com/documentation/101807/0000/Programmers-model/Register-summary?lang=en */

typedef struct
{
    __I uint32_t HWPARAMS;          // 0x000
    __I uint32_t RSVD[3];           // 0x004~0x00c
    __IO uint32_t CTRL;             // 0x010
    __I uint32_t NSEC_ACCESS;       // 0x014
    __I uint32_t RSVD1[2];          // 0x018~0x01c
    __O uint32_t MAINT_CTRL_ALL;    // 0x020
    __O uint32_t MAINT_CTRL_LINES;  // 0x024
    __I uint32_t MAINT_STATUS;      // 0x028
    __I uint32_t RSVD2[53];         // 0x02c~0x0fc
    __I uint32_t SECIRQSTAT;        // 0x100
    __O uint32_t SECIRQSCLR;        // 0x104
    __IO uint32_t SECIRQEN;         // 0x108
    __I uint32_t SECIRQINFO1;       // 0x10c
    __I uint32_t SECIRQINFO2;       // 0x110
    __I uint32_t RSVD3[11];         // 0x114~0x13c
    __I uint32_t NSECIRQSTAT;       // 0x140
    __O uint32_t NSECIRQSCLR;       // 0x144
    __IO uint32_t NSECIRQEN;        // 0x148
    __I uint32_t NSECIRQINFO1;      // 0x14c
    __I uint32_t NSECIRQINFO2;      // 0x150
    __I uint32_t RSVD4[107];        // 0x154~0x2fc
    __I uint32_t SECHIT;            // 0x300
    __I uint32_t SECMISS;           // 0x304
    __IO uint32_t SECSTATCTRL;      // 0x308
    __I uint32_t RSVD5;             // 0x30c
    __I uint32_t NSECHIT;           // 0x310
    __I uint32_t NSECMISS;          // 0x314
    __IO uint32_t NSECSTATCTRL;     // 0x318
    __I uint32_t RSVD6[185];        // 0x31c~0x5fc
    __I uint32_t PMSVR0;            // 0x600
    __I uint32_t PMSVR1;            // 0x604
    __I uint32_t PMSVR2;            // 0x608
    __I uint32_t PMSVR3;            // 0x60c
    __I uint32_t RSVD7[28];         // 0x610~0x67c
    __I uint32_t PMSSSR;            // 0x680
    __I uint32_t RSVD8[27];         // 0x684~0x6ec
    __O uint32_t PMSSCR;            // 0x6f0
    __IO uint32_t PMSSRR;           // 0x6f4
    __I uint32_t RSVD9[566];        // 0x6f8~0xfcc
    __I uint32_t PIDR4;             // 0xfd0
    __I uint32_t PIDR5;             // 0xfd4
    __I uint32_t PIDR6;             // 0xfd8
    __I uint32_t PIDR7;             // 0xfdc
    __I uint32_t PIDR0;             // 0xfe0
    __I uint32_t PIDR1;             // 0xfe4
    __I uint32_t PIDR2;             // 0xfe8
    __I uint32_t PIDR3;             // 0xfec
    __I uint32_t CIDR0;             // 0xff0
    __I uint32_t CIDR1;             // 0xff4
    __I uint32_t CIDR2;             // 0xff8
    __I uint32_t CIDR3;             // 0xffc

} ARM_AHB_CACHE_Typedef;

#define ARM_AHB_CACHE  ((ARM_AHB_CACHE_Typedef *) ARM_AHB_CACHE_BASE)

/********************************************************************************
 * function
********************************************************************************/
void arm_ahb_cache_setup(bool enable);
void arm_ahb_cache_invalidate_by_addr(uint32_t *addr, int32_t dsize);

#endif /* _ARM_AHB_CACHE_CTRL_H_ */