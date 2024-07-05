#ifndef __RTL8762C_ADDRESS_MAPPING_H
#define __RTL8762C_ADDRESS_MAPPING_H
#include "REG_BASE.h"

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------Register Base---------------------------------*/
#define SYSON_ADDRESS_BASE                                                                  (0x40000000UL)

#define TIM_Base                                            (0x40025000UL)

#define RTK_UART1_INTERNAL_REG_BASE                                     (0x40011000UL)

#define RTK_UART0_INTERNAL_REG_BASE                                         (0x40012000UL)






/*-----------------------------------AON Register OFFset---------------------------------*/
#define REG_AON_ENTRY                                   0x48
/* Bit difinition */


//WSTROBE_BIT [0]       RW  FW set "1" to "0" to strobe write data
#define AON_WSTROBE_BIT_SHIFT 0
#define AON_WSTROBE_BIT_MASK REG_1BITS_MASK
#define AON_WSTROBE(x) (((x) & AON_WSTROBE_BIT_MASK) << AON_WSTROBE_BIT_SHIFT)
#define AON_WSTROBE_BIT_CLR (~(AON_WSTROBE_BIT_MASK << AON_WSTROBE_BIT_SHIFT))

//FW_ONREG_WDATA [8:1]
#define AON_WDATA_BIT_SHIFT 1
#define AON_WDATA_BIT_MASK REG_8BITS_MASK
#define AON_WDATA(x) (((x) & AON_WDATA_BIT_MASK) << AON_WDATA_BIT_SHIFT)
#define AON_WDATA_BIT_CLR   (~(AON_WDATA_BIT_MASK << AON_WDATA_BIT_SHIFT))

//FW_ONREG_ADDR [24:17]
#define AON_ADDR_BIT_SHIFT 17
#define AON_ADDR_BIT_MASK REG_8BITS_MASK
#define AON_ADDR(x) (((x) & AON_ADDR_BIT_MASK) << AON_ADDR_BIT_SHIFT)
#define AON_ADDR_BIT_CLR    (~(AON_ADDR_BIT_MASK << AON_ADDR_BIT_SHIFT))


//FW_ONREG_ADDR [16:9]
#define AON_RDATA_BIT_SHIFT 9
#define AON_RDATA_BIT_MASK REG_8BITS_MASK
#define AON_RDATA(x) (((x) & AON_RDATA_BIT_MASK) << AON_RDATA_BIT_SHIFT)
#define AON_RDATA_BIT_CLR   (~(AON_RDATA_BIT_MASK << AON_RDATA_BIT_SHIFT))


/*-----------------------------------RCC Reg---------------------------------*/
#define  SOC_FUNC_EN_REG                                    *((volatile uint32_t*)0x40000210UL)
#define  PERI_FUNC0_EN_REG_218                              *((volatile uint32_t*)0x40000218UL)

#define  CLK_CTRL_REG                                       *((volatile uint32_t*)0x40000230UL)
#define  PERI_CLK_CTRL0_REG_234                             *((volatile uint32_t*)0x40000234UL)

#define  CLK_SOURCE_REG_348                                  *((volatile uint32_t *)0x40000348UL)
#define  CLK_SOURCE_REG_35C                                  *((volatile uint32_t *)0x4000035CUL)
#define  CLK_SOURCE_REG_360                                  *((volatile uint32_t *)0x40000360UL)




/*-----------------------------------Vendor Reg---------------------------------*/
#define  PERI_INT_REG                                       *((volatile uint32_t *)0x4000600CUL)




/*-----------------------------------RCC Reg---------------------------------*/
/* Bit difinition */
/* SOC_FUNC_EN_REG */
/* Bit 16 : BIT_SOC_GTIMER_EN. 1: enable GTIMER IP and GTIMER register */
#define SYSBLK_GTIMER_EN_Pos (16UL) /*!< Position of  */
#define SYSBLK_GTIMER_EN_Msk (0x1UL << SYSBLK_GTIMER_EN_Pos) /*!< Bit mask of  */
/* Bit 12 : BIT_SOC_LOG_UART_EN. 1: enable log UART; 0: disable log UART */
#define SYSBLK_LOGUART_EN_Pos (12UL) /*!< Position of . */
#define SYSBLK_LOGUART_EN_Msk (0x1UL << SYSBLK_LOGUART_EN_Pos) /*!< Bit mask of  */

/* Register: REG_SOC_PERI_FUNC0_EN */
/* Description: REG_SOC_PERI_FUNC0_EN. */
/* Bit 1 : BIT_PERI_UART1_EN. 1; enable UART1 interface ( DATA Uart1) */
//#define SYSBLK_UART2_EN_Pos (1UL) /*!< Position of . */
//#define SYSBLK_UART2_EN_Msk (0x1UL << SYSBLK_UART1_EN_Pos) /*!< Bit mask of  */
/* Bit 0 : BIT_PERI_UART0_EN. 1; enable UART0 interface ( DATA Uart) */
#define SYSBLK_UART0_EN_Pos (0UL) /*!< Position of . */
#define SYSBLK_UART0_EN_Msk (0x1UL << SYSBLK_UART0_EN_Pos) /*!< Bit mask of  */

/* CLK_CTRL_REG */
/* Bit 15 : BIT_SOC_SLPCK_TIMER_EN */
#define SYSBLK_SLPCK_TIMER_EN_Pos (15UL) /*!< Position of  */
#define SYSBLK_SLPCK_TIMER_EN_Msk (0x1UL << SYSBLK_SLPCK_TIMER_EN_Pos) /*!< Bit mask of  */
/* Bit 14 : BIT_SOC_ACTCK_TIMER_EN */
#define SYSBLK_ACTCK_TIMER_EN_Pos (14UL) /*!< Position of  */
#define SYSBLK_ACTCK_TIMER_EN_Msk (0x1UL << SYSBLK_ACTCK_TIMER_EN_Pos) /*!< Bit mask of  */
/* Bit 13 : BIT_SOC_SLPCK_LOG_UART_EN */
#define SYSBLK_SLPCK_LOGUART_EN_Pos (13UL) /*!< Position of  */
#define SYSBLK_SLPCK_LOGUART_EN_Msk (0x1UL << SYSBLK_SLPCK_LOGUART_EN_Pos) /*!< Bit mask of  */
/* Bit 12 : BIT_SOC_ACTCK_LOG_UART_EN */
#define SYSBLK_ACTCK_LOGUART_EN_Pos (12UL) /*!< Position of  */
#define SYSBLK_ACTCK_LOGUART_EN_Msk (0x1UL << SYSBLK_ACTCK_LOGUART_EN_Pos) /*!< Bit mask of  */
/* Bit 11 : BIT_SOC_SLPCK_UART1_DATA_EN */
#define SYSBLK_SLPCK_UART1DATA_EN_Pos (11UL) /*!< Position of  */
#define SYSBLK_SLPCK_UART1DATA_EN_Msk (0x1UL << SYSBLK_SLPCK_UART1DATA_EN_Pos) /*!< Bit mask of  */
/* Bit 10 : BIT_SOC_ACTCK_UART1_DATA_EN */
#define SYSBLK_ACTCK_UART1DATA_EN_Pos (10UL) /*!< Position of  */
#define SYSBLK_ACTCK_UART1DATA_EN_Msk (0x1UL << SYSBLK_ACTCK_UART1DATA_EN_Pos) /*!< Bit mask of  */

/* Register: REG_PESOC_PERI_CLK_CTRL0 */
/* Description: REG_PESOC_PERI_CLK_CTRL0. */
/* Bit 1 : BIT_SOC_SLPCK_UART0_DATA_EN */
#define SYSBLK_SLPCK_UART0DATA_EN_Pos (1UL) /*!< Position of  */
#define SYSBLK_SLPCK_UART0DATA_EN_Msk (0x1UL << SYSBLK_SLPCK_UART0DATA_EN_Pos) /*!< Bit mask of  */
/* Bit 0 : BIT_SOC_ACTCK_UART0_DATA_EN */
#define SYSBLK_ACTCK_UART0DATA_EN_Pos (0UL) /*!< Position of  */
#define SYSBLK_ACTCK_UART0DATA_EN_Msk (0x1UL << SYSBLK_ACTCK_UART0DATA_EN_Pos) /*!< Bit mask of  */

/* Vendor Reg */
/* Bit 9: DW Timer interrupt Enable */
#define VENDOR_TIMER_INT_EN_Pos (9UL) /*!< Position of  */
#define VENDOR_TIMER_INT_EN_Msk (0x1UL << VENDOR_TIMER_INT_EN_Pos) /*!< Bit mask of  */




#ifdef __cplusplus
}
#endif

#endif /* _RTL8763B_AON_H_ */

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/



