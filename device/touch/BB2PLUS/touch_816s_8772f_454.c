/*
 * Copyright (c) 2006-2020, Watch Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2020-08-04     howie  first version
 */


#include "touch_816s_8772f_454.h"
#include "string.h"
#include "trace.h"
#include "board.h"


void (*touch_wakeup_indicate)(void) = NULL;


bool rtk_touch_hal_read_all(uint16_t *x, uint16_t *y, bool *pressing)
{

    return true;
}


void rtk_touch_hal_set_indicate(void (*indicate)(void *))
{

}

void rtk_touch_hal_int_config(bool enable)
{

}



void drv_touch_dlps_init(void)
{

}

void drv_touch_set_wakeup_indicate(void (*wakeup_ind))
{

}

void rtk_touch_hal_init(void)
{

}

