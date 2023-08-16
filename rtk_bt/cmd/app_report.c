/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */
#include <stdlib.h>
#include <string.h>
#include "trace.h"
#include "os_mem.h"
#include "console.h"
#include "app_util.h"
#include "app_cmd.h"
#include "app_br_link_util.h"
#include "app_le_link_util.h"
#include "app_transfer.h"
#include "app_report.h"
#include "app_transfer.h"
#include "app_cfg.h"
#include "gap_msg.h"
#include "app_eq.h"
#include "dp_br_info.h"
#include "dp_ble_info.h"


static uint8_t uart_tx_seqn = 0;

static void app_report_uart_event(uint16_t event_id, uint8_t *data, uint16_t len)
{
    APP_PRINT_INFO0("app_report_uart_event");
}

static void app_report_le_event(T_APP_LE_LINK *p_link, uint16_t event_id, uint8_t *data,
                                uint16_t len)
{
    if (p_link->state == GAP_CONN_STATE_CONNECTED)
    {
        uint8_t *buf;

        buf = malloc(len + 6);
        if (buf == NULL)
        {
            return;
        }

        buf[0] = CMD_SYNC_BYTE;
        p_link->tx_event_seqn++;
        if (p_link->tx_event_seqn == 0)
        {
            p_link->tx_event_seqn = 1;
        }
        buf[1] = p_link->tx_event_seqn;
        buf[2] = (uint8_t)(len + 2);
        buf[3] = (uint8_t)((len + 2) >> 8);
        buf[4] = (uint8_t)event_id;
        buf[5] = (uint8_t)(event_id >> 8);

        if (len)
        {
            memcpy(&buf[6], data, len);
        }

        if (app_push_data_transfer_queue(CMD_PATH_LE, buf, len + 6, p_link->id) == false)
        {
            free(buf);
        }
    }
}

static void app_report_spp_iap_event(T_APP_BR_LINK *p_link, uint16_t event_id, uint8_t *data,
                                     uint16_t len, uint8_t cmd_path)
{
    uint32_t check_prof = 0;

    if (cmd_path == CMD_PATH_SPP)
    {
        check_prof = SPP_PROFILE_MASK;
    }
    else if (cmd_path == CMD_PATH_IAP)
    {
        check_prof = IAP_PROFILE_MASK;
    }

    if (p_link->connected_profile & check_prof)
    {
        uint8_t *buf;

        buf = malloc(len + 6);
        if (buf == NULL)
        {
            return;
        }

        buf[0] = CMD_SYNC_BYTE;
        p_link->tx_event_seqn++;
        if (p_link->tx_event_seqn == 0)
        {
            p_link->tx_event_seqn = 1;
        }
        buf[1] = p_link->tx_event_seqn;
        buf[2] = (uint8_t)(len + 2);
        buf[3] = (uint8_t)((len + 2) >> 8);
        buf[4] = (uint8_t)event_id;
        buf[5] = (uint8_t)(event_id >> 8);

        if (len)
        {
            memcpy(&buf[6], data, len);
        }

        if (app_push_data_transfer_queue(cmd_path, buf, len + 6, p_link->id) == false)
        {
            free(buf);
        }
    }
}

static void app_report_spp_iap_raw_data(T_APP_BR_LINK *p_link, uint8_t *data, uint16_t len,
                                        uint8_t cmd_path)
{
    uint32_t check_prof = 0;

    if (cmd_path == CMD_PATH_SPP)
    {
        check_prof = SPP_PROFILE_MASK;
    }
    else if (cmd_path == CMD_PATH_IAP)
    {
        check_prof = IAP_PROFILE_MASK;
    }

    if (p_link->connected_profile & check_prof)
    {
        uint8_t *buf;

        buf = malloc(len);
        if (buf == NULL)
        {
            return;
        }

        memcpy(buf, data, len);

        if (app_push_data_transfer_queue(cmd_path, buf, len, p_link->id) == false)
        {
            free(buf);
        }
    }
}

static void app_report_le_raw_data(T_APP_LE_LINK *p_link, uint8_t *data, uint16_t len)
{
    uint8_t *buf;

    buf = malloc(len);
    if (buf == NULL)
    {
        return;
    }

    memcpy(buf, data, len);

    if (app_push_data_transfer_queue(CMD_PATH_LE, buf, len, p_link->id) == false)
    {
        free(buf);
    }
}

void app_report_event(uint8_t cmd_path, uint16_t event_id, uint8_t app_index, uint8_t *data,
                      uint16_t len)
{
    APP_PRINT_TRACE4("app_report_event: cmd_path %d, event_id 0x%04x, app_index %d, data_len %d",
                     cmd_path, event_id, app_index, len);

    switch (cmd_path)
    {
    case CMD_PATH_UART:
        app_report_uart_event(event_id, data, len);
        break;

//    case CMD_PATH_LE:
//        if (app_index < MAX_BLE_LINK_NUM)
//        {
//            app_report_le_event(&app_db.le_link[app_index], event_id, data, len);
//        }
//        break;

    case CMD_PATH_SPP:
    case CMD_PATH_IAP:
        if (app_index < MAX_BR_LINK_NUM)
        {
            app_report_spp_iap_event(&br_db.br_link[app_index], event_id, data, len, cmd_path);
        }
        break;

    default:
        break;
    }
}

void app_report_event_broadcast(uint16_t event_id, uint8_t *buf, uint16_t len)
{
    T_APP_BR_LINK *br_link;
    T_APP_LE_LINK *le_link;
    uint8_t        i;

    app_report_event(CMD_PATH_UART, event_id, 0, buf, len);

    for (i = 0; i < MAX_BR_LINK_NUM; i ++)
    {
        br_link = &br_db.br_link[i];

        if (br_link->cmd_set_enable == true)
        {
            if (br_link->connected_profile & SPP_PROFILE_MASK)
            {
                app_report_event(CMD_PATH_SPP, event_id, i, buf, len);
            }

            if (br_link->connected_profile & IAP_PROFILE_MASK)
            {
                app_report_event(CMD_PATH_IAP, event_id, i, buf, len);
            }
        }
    }

    for (i = 0; i < MAX_BLE_LINK_NUM; i++)
    {
        le_link = &le_db.le_link[i];

        if (le_link->state == GAP_CONN_STATE_CONNECTED)
        {
            if (le_link->cmd_set_enable == true)
            {
                app_report_event(CMD_PATH_LE, event_id, i, buf, len);
            }
        }
    }
}

void app_report_raw_data(uint8_t cmd_path, uint8_t app_index, uint8_t *data,
                         uint16_t len)
{
    APP_PRINT_TRACE3("app_report_raw_data: cmd_path %d, app_index %d, data_len %d", cmd_path, app_index,
                     len);

    switch (cmd_path)
    {
    case CMD_PATH_SPP:
    case CMD_PATH_IAP:
        if ((app_index < MAX_BR_LINK_NUM) && data)
        {
            app_report_spp_iap_raw_data(&br_db.br_link[app_index], data, len, cmd_path);
        }
        break;

    case CMD_PATH_LE:
        if ((app_index < MAX_BLE_LINK_NUM) && data)
        {
            app_report_le_raw_data(&le_db.le_link[app_index], data, len);
        }
        break;

    default:
        break;
    }
}

void app_report_rws_state(void)
{
    APP_PRINT_INFO2("SPP CAPTURE DATA V2 %s %d", TRACE_STRING(__FUNCTION__), __LINE__);
}

void app_report_rws_bud_side(void)
{
    APP_PRINT_INFO2("SPP CAPTURE DATA V2 %s %d", TRACE_STRING(__FUNCTION__), __LINE__);
}

#if F_APP_APT_SUPPORT
void app_report_apt_eq_idx(T_APT_EQ_DATA_UPDATE_EVENT apt_eq_data_update_event)
{
    if (!app_db.eq_ctrl_by_src)
    {
        return;
    }

    uint8_t buf[2];

    buf[0]  = app_cfg_nv.apt_eq_idx;
    buf[1]  = apt_eq_data_update_event;

    app_report_event_broadcast(EVENT_APT_EQ_INDEX_INFO, buf, 2);
}
#endif

void app_report_get_bud_info(uint8_t *data)
{
    APP_PRINT_INFO2("SPP CAPTURE DATA V2 %s %d", TRACE_STRING(__FUNCTION__), __LINE__);
}

void app_report_rws_bud_info(void)
{
    uint8_t buf[6];

    app_report_get_bud_info(&buf[0]);
    app_report_event_broadcast(EVENT_REPORT_BUD_INFO, buf, sizeof(buf));
}
