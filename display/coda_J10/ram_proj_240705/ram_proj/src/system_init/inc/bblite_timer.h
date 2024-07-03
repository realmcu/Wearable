#ifndef _RTL8762C_TIMER_H_
#define _RTL8762C_TIMER_H_


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------RCC Reg---------------------------------*/
#define  SOC_FUNC_EN_REG                                    *((volatile uint32_t*)0x40000210UL)
#define  CLK_CTRL_REG                                       *((volatile uint32_t*)0x40000230UL)

#define CLK_SOURCE_REG_348                                  *((volatile uint32_t *)0x40000348UL)
#define CLK_SOURCE_REG_35C                                  *((volatile uint32_t *)0x4000035CUL)
#define CLK_SOURCE_REG_360                                  *((volatile uint32_t *)0x40000360UL)

/*-----------------------------------TIM Reg---------------------------------*/
#define  TIM_Index                                          (7)


#if 1

#define  TIM_LOADCOUNT_REG                                  *((volatile uint32_t*)(TIM_Base + TIM_Index*(0x14)))
#define  TIM_CURRENTVAL_REG                                 *((volatile uint32_t*)(TIM_Base + 0x4 + TIM_Index*(0x14)))
#define  TIM_CTLREG_REG                                     *((volatile uint32_t*)(TIM_Base + 0x8 + TIM_Index*(0x14)))
#define  TIM_EOI_REG                                        *((volatile uint32_t*)(TIM_Base + 0xC + TIM_Index*(0x14)))
#define  TIM_INTSTATUS_REG                                  *((volatile uint32_t*)(TIM_Base + 0x10 + TIM_Index*(0x14)))

#else

#define  TIM_LOADCOUNT_REG                                  *((volatile uint32_t*)(0x4000208CUL))
#define  TIM_CURRENTVAL_REG                                 *((volatile uint32_t*)(0x40002090UL))
#define  TIM_CTLREG_REG                                     *((volatile uint32_t*)(0x40002094UL))
#define  TIM_EOI_REG                                        *((volatile uint32_t*)(0x40002098UL))
#define  TIM_INTSTATUS_REG                                  *((volatile uint32_t*)(0x4000209CUL))

#endif

/*-----------------------------------Vendor Reg---------------------------------*/
#define  PERI_INT_REG                                       *((volatile uint32_t *)0x4000600CUL)




/* SOC_FUNC_EN_REG */
/* Bit 16 : BIT_SOC_GTIMER_EN. 1: enable GTIMER IP and GTIMER register */
#define SYSBLK_GTIMER_EN_Pos (16UL) /*!< Position of  */
#define SYSBLK_GTIMER_EN_Msk (0x1UL << SYSBLK_GTIMER_EN_Pos) /*!< Bit mask of  */

/* CLK_CTRL_REG */
/* Bit 15 : BIT_SOC_SLPCK_TIMER_EN */
#define SYSBLK_SLPCK_TIMER_EN_Pos (15UL) /*!< Position of  */
#define SYSBLK_SLPCK_TIMER_EN_Msk (0x1UL << SYSBLK_SLPCK_TIMER_EN_Pos) /*!< Bit mask of  */
/* Bit 14 : BIT_SOC_ACTCK_TIMER_EN */
#define SYSBLK_ACTCK_TIMER_EN_Pos (14UL) /*!< Position of  */
#define SYSBLK_ACTCK_TIMER_EN_Msk (0x1UL << SYSBLK_ACTCK_TIMER_EN_Pos) /*!< Bit mask of  */

/* Vendor Reg */
/* Bit 9: DW Timer interrupt Enable */
#define VENDOR_TIMER_INT_EN_Pos (9UL) /*!< Position of  */
#define VENDOR_TIMER_INT_EN_Msk (0x1UL << VENDOR_TIMER_INT_EN_Pos) /*!< Bit mask of  */




void HW_Timer_Init(void);
void HW_Timer_DelayUs(uint32_t us);
void HW_Timer_DelayMs(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _RTL8763B_PERIPHERAL_H_ */

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor *****END OF FILE****/



