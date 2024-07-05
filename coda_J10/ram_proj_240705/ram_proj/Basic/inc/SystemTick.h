#ifndef _SYSTEM_TCIK_H_
#define _SYSTEM_TCIK_H_

#include <stdint.h>
#include "REG_BASE.h"

//#define FREQ_20MHZ 20000000
//#define FREQ_40MHZ 40000000


#define CPU_CLOCK_FREQ CPU_CLOCK  //40MHz

//3 SYSTEM TICK CTL register
#define REG_SYSTEM_TICK_CTL                     0xE000E010                  //32bits
//3 SYSTEM TICK RELOAD Register
#define REG_SYSTEM_TICK_RELOAD              0xE000E014                  //32bits
//3 SYSTEM TICK Counter Register
#define REG_SYSTEM_TICK_CNT                     0xE000E018                  //32bits
//3 SYSTEM TICK calibration Register
#define REG_SYSTEM_TICK_CAL                     0xE000E01C                  //32bits



//3 SYSTEM TICK CTL register 0xE000E010     32Bits; Bit Control
#define SYSTICK_CNT_FLAG BIT16
#define SYSTICK_CNT_MASK MASK_24BITS
#define SYSTICK_MAX_CNT_TIME (SYSTICK_CNT_MASK/CPU_CLOCK_FREQ)
#define SYSTICK_CNT_1MS_TICK (CPU_CLOCK_FREQ/1000)
#define SYSTICK_CNT_1US_TICK (CPU_CLOCK_FREQ/1000000)

#define BIT_SHIFT_SYSTICK_CLK_SOURCE 2
#define BIT_MASK_SYSTICK_CLK_SOURCE 0x1
#define SYSTICK_CLK_SOURCE(x) (((x) & BIT_MASK_SYSTICK_CLK_SOURCE) << BIT_SHIFT_SYSTICK_CLK_SOURCE)
#define BIT_INVC_SYSTICK_CLK_SOURCE (~(BIT_MASK_SYSTICK_CLK_SOURCE << BIT_SHIFT_SYSTICK_CLK_SOURCE))

#define BIT_SHIFT_SYSTICK_CLK_TICKINT 1
#define BIT_MASK_SYSTICK_CLK_TICKINT 0x1
#define SYSTICK_CLK_TICKINT(x) (((x) & BIT_MASK_SYSTICK_CLK_TICKINT) << BIT_SHIFT_SYSTICK_CLK_TICKINT)
#define BIT_INVC_SYSTICK_CLK_TICKINT (~(BIT_MASK_SYSTICK_CLK_TICKINT << BIT_SHIFT_SYSTICK_CLK_TICKINT))

#define BIT_SHIFT_SYSTICK_CLK_ENABLE 0
#define BIT_MASK_SYSTICK_CLK_ENABLE 0x1
#define SYSTICK_CLK_ENABLE(x) (((x) & BIT_MASK_SYSTICK_CLK_ENABLE) << BIT_SHIFT_SYSTICK_CLK_ENABLE)
#define BIT_INVC_SYSTICK_CLK_ENABLE (~(BIT_MASK_SYSTICK_CLK_ENABLE << BIT_SHIFT_SYSTICK_CLK_ENABLE))






typedef enum
{
    SYSTEMTICK_EXT_CLK = 0x0, /**< Use core clock */
    SYSTEMTICK_CORE_CLK  = 0x1, /**< Use external clock  */
} SYSTEMTICK_CLK_SourceType;


typedef enum
{
    SYSTEMTICK_DISABLE = 0x0, /**< Disable System Tick */
    SYSTEMTICK_ENABLE = 0x1, /**< Enable System Tick  */
} SYSTEMTICK_CLK_EnableType;

typedef enum
{
    SYSTEMTICK_INT_DISABLE = 0x0, /**< Disable System Tick Interrupt*/
    SYSTEMTICK_INT_ENABLE = 0x1, /**< Enable System Tick Interrupt*/
} SYSTEMTICK_IntCtrl;

typedef struct
{
    uint32_t
    SYSTICK_Counter;         /*!< Specifies SystemTick Counter load*/

    SYSTEMTICK_CLK_SourceType         SYSTICK_CLK_SOURCE;      /*!< Specifies SystemTick Clock Source*/
    uint32_t
    SYSTICK_CLK_FREQ;        /**< System Tick Clock Frequence */

    SYSTEMTICK_IntCtrl
    SYSTICK_Int;                     /**< Interrupt Enable*/
} SYSTEMTICK_InitTypeDef;



void SYSTEMTICK_DelayUs(uint32_t DelayUsTime);
void SYSTEMTICK_DelayMs(uint32_t DelayMsTime);




#endif
