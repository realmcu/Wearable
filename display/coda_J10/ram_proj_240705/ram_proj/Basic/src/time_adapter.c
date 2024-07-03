#include "time_adapter.h"
#include "hwtest_config.h"
#include "bblite_timer.h"


#if (DELAY_USE_SELECT == DELAY_USE_SYSTICK)
#include "SystemTick.h"

#elif (DELAY_USE_SELECT == DELAY_USE_OS_DELAY)
#include "FreeRTOS.h"
#include "task.h"

#else
//    #error "Please configure time function required low level support."

#endif

void init_timer(void)
{
#if (DELAY_USE_SELECT == DELAY_USE_SYSTICK)
    /* no need */

#elif (DELAY_USE_SELECT == DELAY_USE_HWTIMER)
    //====================HW Timer implement==============================
    HW_Timer_Init();

#elif (DELAY_USE_SELECT == DELAY_USE_OS_DELAY)
    //====================HW Timer implement==============================
    /* no need */

#else
#error "Please configure time function required low level support."

#endif
}

void delay_us(uint32_t us)
{
#if (DELAY_USE_SELECT == DELAY_USE_SYSTICK)
    //====================System Tick implement===========================
    SYSTEMTICK_DelayUs(us);

#elif (DELAY_USE_SELECT == DELAY_USE_HWTIMER)
    //====================HW Timer implement==============================
    HW_Timer_DelayUs(us);

#elif (DELAY_USE_SELECT == DELAY_USE_OS_DELAY)
    //====================FreeRTOS implement==============================
    /* FreeRTOS doesn't support delay US */
    //while (1);
    for (int i = 0; i < 2 * us; i++)
    {
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
    }

#else
#error "Please configure time function required low level support."

#endif
}

void delay_ms(uint32_t ms)
{
#if (DELAY_USE_SELECT == DELAY_USE_SYSTICK)
    //====================System Tick implement===========================
    SYSTEMTICK_DelayMs(ms);

#elif (DELAY_USE_SELECT == DELAY_USE_HWTIMER)
    //====================HW Timer implement==============================
    HW_Timer_DelayMs(ms);

#elif (DELAY_USE_SELECT == DELAY_USE_OS_DELAY)
    //====================FreeRTOS implement==============================
    vTaskDelay(ms);

#else
#error "Please configure time function required low level support."

#endif
}
