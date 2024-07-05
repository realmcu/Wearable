#include "hwtest_config.h"
#include "SystemTick.h"

#include "rtl876x.h"//new include


void SYSTEMTICK_StructInit(SYSTEMTICK_InitTypeDef *SYSTEMTICK_Init)
{
    SYSTEMTICK_Init->SYSTICK_CLK_SOURCE = SYSTEMTICK_CORE_CLK;
    SYSTEMTICK_Init->SYSTICK_CLK_FREQ   = CPU_CLOCK_FREQ;
    SYSTEMTICK_Init->SYSTICK_Int        = SYSTEMTICK_INT_DISABLE;
    SYSTEMTICK_Init->SYSTICK_Counter    = 0x1000;
}

void Initial_SystemTick(SYSTEMTICK_InitTypeDef *SYSTEMTICK_Init)
{
    uint32_t SYSTEM_TICK_CTL;

    //3 Set SYSTEM TICK CTL register 0xE000E010 32Bits; Bit Control
    SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
    //Clear Register Bit
    SYSTEM_TICK_CTL &= (BIT_INVC_SYSTICK_CLK_SOURCE &    //enable 40MHz divider
                        BIT_INVC_SYSTICK_CLK_TICKINT &
                        BIT_INVC_SYSTICK_CLK_ENABLE); //enable 5MHz clock for ADC
    //Set egister Bit
    SYSTEM_TICK_CTL |= (SYSTICK_CLK_SOURCE(SYSTEMTICK_Init->SYSTICK_CLK_SOURCE) |
                        SYSTICK_CLK_TICKINT(SYSTEMTICK_Init->SYSTICK_Int));
    HAL_WRITE32(REG_SYSTEM_TICK_CTL, 0, SYSTEM_TICK_CTL);
}

void SYSTEMTICK_ReEnable_Cnt(void)
{
    uint32_t SYSTEM_TICK_CTL = 0;
    //3 Set SYSTEM TICK CTL register 0xE000E010 32Bits; Bit Control
    SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
    //Clear Register Bit
    SYSTEM_TICK_CTL &= (BIT_INVC_SYSTICK_CLK_ENABLE); //disable SystemTick
    //Set egister Bit
    SYSTEM_TICK_CTL |= (SYSTICK_CLK_ENABLE(SYSTEMTICK_ENABLE));
    HAL_WRITE32(REG_SYSTEM_TICK_CTL, 0, SYSTEM_TICK_CTL);
}

void SYSTEMTICK_Set2DelayMsMode(void)
{
    SYSTEMTICK_InitTypeDef SYSTEMTICK_Init;
    SYSTEMTICK_Init.SYSTICK_CLK_SOURCE = SYSTEMTICK_CORE_CLK;
    SYSTEMTICK_Init.SYSTICK_CLK_FREQ   = CPU_CLOCK_FREQ;
    SYSTEMTICK_Init.SYSTICK_Int        = SYSTEMTICK_INT_DISABLE;
    //SYSTEMTICK_Init.SYSTICK_Counter    = (CPU_CLOCK_FREQ/1000);
    Initial_SystemTick(&SYSTEMTICK_Init);
}

void SYSTEMTICK_DelayMs(uint32_t DelayMsTime)
{
    uint32_t SYSTEM_TICK_CTL    = 0;
    uint32_t SYSTEM_TICK_RELOAD = 0;
//    uint32_t SYSTEM_TICK_CNT    = 0;
    uint32_t Repeat_times       = DelayMsTime;

    //3 Set SYSTEM TICK CTL register 0xE000E010 32Bits; Bit Control
    SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
    //Clear Register Bit
    SYSTEM_TICK_CTL &= (BIT_INVC_SYSTICK_CLK_ENABLE); //disable SystemTick
    //Set egister Bit
    SYSTEM_TICK_CTL |= (SYSTICK_CLK_ENABLE(SYSTEMTICK_DISABLE));
    HAL_WRITE32(REG_SYSTEM_TICK_CTL, 0, SYSTEM_TICK_CTL);

    //3 Set SYSTEM TICK Reolad register 0xE000E014 32Bits; Bit Control
    //Set counter to 1ms tick
    SYSTEM_TICK_RELOAD = SYSTICK_CNT_1MS_TICK;
    HAL_WRITE32(REG_SYSTEM_TICK_RELOAD, 0, SYSTEM_TICK_RELOAD);

    //Reenable System Tick
    //3 Set SYSTEM TICK CTL register 0xE000E010 32Bits; Bit Control
    SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
    //Clear Register Bit
    SYSTEM_TICK_CTL &= (BIT_INVC_SYSTICK_CLK_SOURCE &
                        BIT_INVC_SYSTICK_CLK_TICKINT &
                        BIT_INVC_SYSTICK_CLK_ENABLE);
    //Set egister Bit
    SYSTEM_TICK_CTL |= (SYSTICK_CLK_SOURCE(SYSTEMTICK_CORE_CLK) |
                        SYSTICK_CLK_TICKINT(SYSTEMTICK_INT_DISABLE) |
                        SYSTICK_CLK_ENABLE(SYSTEMTICK_ENABLE));
    HAL_WRITE32(REG_SYSTEM_TICK_CTL, 0, SYSTEM_TICK_CTL);

    while (1)
    {
        SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
        if ((SYSTEM_TICK_CTL & SYSTICK_CNT_FLAG) != 0)
        {
            Repeat_times--;
        }

        if (Repeat_times == 0)
        {
            break;
        }
    }
}

void SYSTEMTICK_DelayUs(uint32_t DelayUsTime)
{
    uint32_t SYSTEM_TICK_CTL    = 0;
    uint32_t SYSTEM_TICK_RELOAD = 0;
//    uint32_t SYSTEM_TICK_CNT    = 0;
    uint32_t Repeat_times       = DelayUsTime;

    //3 Set SYSTEM TICK CTL register 0xE000E010 32Bits; Bit Control
    SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
    //Clear Register Bit
    SYSTEM_TICK_CTL &= (BIT_INVC_SYSTICK_CLK_ENABLE); //disable SystemTick
    //Set egister Bit
    SYSTEM_TICK_CTL |= (SYSTICK_CLK_ENABLE(SYSTEMTICK_DISABLE));
    HAL_WRITE32(REG_SYSTEM_TICK_CTL, 0, SYSTEM_TICK_CTL);

    //3 Set SYSTEM TICK Reolad register 0xE000E014 32Bits; Bit Control
    //Set counter to 1us tick
    SYSTEM_TICK_RELOAD = SYSTICK_CNT_1US_TICK;
    HAL_WRITE32(REG_SYSTEM_TICK_RELOAD, 0, SYSTEM_TICK_RELOAD);

    //Reenable System Tick
    //3 Set SYSTEM TICK CTL register 0xE000E010 32Bits; Bit Control
    SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
    //Clear Register Bit
    SYSTEM_TICK_CTL &= (BIT_INVC_SYSTICK_CLK_SOURCE &
                        BIT_INVC_SYSTICK_CLK_TICKINT &
                        BIT_INVC_SYSTICK_CLK_ENABLE);
    //Set egister Bit
    SYSTEM_TICK_CTL |= (SYSTICK_CLK_SOURCE(SYSTEMTICK_CORE_CLK) |
                        SYSTICK_CLK_TICKINT(SYSTEMTICK_INT_DISABLE) |
                        SYSTICK_CLK_ENABLE(SYSTEMTICK_ENABLE));
    HAL_WRITE32(REG_SYSTEM_TICK_CTL, 0, SYSTEM_TICK_CTL);

    while (1)
    {
        SYSTEM_TICK_CTL = HAL_READ32(REG_SYSTEM_TICK_CTL, 0);
        if ((SYSTEM_TICK_CTL & SYSTICK_CNT_FLAG) != 0)
        {
            Repeat_times--;
        }

        if (Repeat_times == 0)
        {
            break;
        }
    }
}
