/**
 * @file ble_components_init.c
 * @author howie_wang (howie_wang@realtek.com.cn)
 * @brief
 * @version 0.1
 * @date 2024-01-18
 *
 * @copyright Copyright (c) 2024
 *
 */


#include "ble_components_init.h"


static int rti_start(void)
{
    return 0;
}
BLE_APP_INIT_EXPORT(rti_start, "0");

static int rti_board_start(void)
{
    return 0;
}
BLE_APP_INIT_EXPORT(rti_board_start, "0.end");

static int rti_board_end(void)
{
    return 0;
}
BLE_APP_INIT_EXPORT(rti_board_end, "1.end");

static int rti_end(void)
{
    return 0;
}
BLE_APP_INIT_EXPORT(rti_end, "6.end");

#include "trace.h"
void ble_components_init(void)
{
    volatile const ble_init_fn_t *fn_ptr;

    // cppcheck-suppress comparePointers
    for (fn_ptr = &__ble_init_rti_board_end; fn_ptr < &__ble_init_rti_end; fn_ptr ++)
    {
        DBG_DIRECT("fn_ptr %x", fn_ptr);
        (*fn_ptr)();
    }
}

