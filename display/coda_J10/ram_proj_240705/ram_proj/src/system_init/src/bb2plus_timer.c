#include "rtl876x.h"//new include

#include "hwtest_config.h"
#include "bblite_timer.h"


void HW_Timer_Init(void)
{
    /* RCC config */
    SOC_FUNC_EN_REG |= SYSBLK_GTIMER_EN_Msk;
    CLK_CTRL_REG |= (SYSBLK_SLPCK_TIMER_EN_Msk | SYSBLK_ACTCK_TIMER_EN_Msk);

    /* Clock source 40 M */
    *((volatile uint32_t *)0x40000348UL) |= BIT4 | BIT7;
//      *((volatile uint32_t*)0x40000360UL) |= BIT11|BIT12|BIT13;
    CLK_SOURCE_REG_35C &= (0x07 << 6);
    /* No PWM, User define, Interrupt Unmask*/
    TIM_CTLREG_REG |= BIT1;
    TIM_CTLREG_REG &= ~(BIT0 | BIT2 | BIT3);

    /* Clear Interrupt status */
    TIM_EOI_REG;

    /* NVIC Disable */
    PERI_INT_REG &= ~VENDOR_TIMER_INT_EN_Msk;
}

void HW_Timer_DelayUs(uint32_t us)
{
    TIM_LOADCOUNT_REG = (us > 2) ? (us * 40 - 84) : 5;
    /* Enable Timer */
    TIM_CTLREG_REG |= BIT0;
    /* Polling Status */
    while (!TIM_INTSTATUS_REG);
    /* Disable Timer */
    TIM_CTLREG_REG &= ~BIT0;
    /* Clear Interrupt status */
    TIM_EOI_REG;
}

void HW_Timer_DelayMs(uint32_t ms)
{
    TIM_LOADCOUNT_REG = ms * 40000 - 84;
    /* Enable Timer */
    TIM_CTLREG_REG |= BIT0;
    /* Polling Status */
    while (!TIM_INTSTATUS_REG);
    /* Disable Timer */
    TIM_CTLREG_REG &= ~BIT0;
    /* Clear Interrupt status */
    TIM_EOI_REG;
}


