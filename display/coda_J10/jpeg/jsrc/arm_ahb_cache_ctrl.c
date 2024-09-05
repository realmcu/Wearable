#include "arm_ahb_cache_ctrl.h"

#if 1
void arm_ahb_cache_setup(bool enable)
{

    if (enable == true)
    {
        // Enable the cache by writing 0x1 to the ENABLE field of the CTRL register.
        ARM_AHB_CACHE->CTRL |= CTRL_ENABLE_Msk;

        // Poll the MAINT_STATUS register and wait until the CACHE_ENABLED field is set to 0x1.
        while ((ARM_AHB_CACHE->MAINT_STATUS & MAINT_STATUS_CACHE_ENABLED_Msk) == 0);
    }
    else
    {
        // Enable the cache by writing 0x1 to the ENABLE field of the CTRL register.
        ARM_AHB_CACHE->CTRL &= ~CTRL_ENABLE_Msk;

        // Poll the MAINT_STATUS register and wait until the CACHE_ENABLED field is set to 0x0.
        while (ARM_AHB_CACHE->MAINT_STATUS & MAINT_STATUS_CACHE_ENABLED_Msk);
    }

}
#endif