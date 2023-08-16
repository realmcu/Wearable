/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */
#include <stdlib.h>
#include <string.h>
#include "os_mem.h"
#include "os_timer.h"
#include "os_sync.h"
#include "console.h"
#include "gap.h"
#include "gap_br.h"
#include "trace.h"
#include "bt_types.h"
#include "app_timer.h"
#include "transmit_service.h"
#include "bt_iap.h"
#include "app_transfer.h"
#include "app_cmd.h"
#include "app_cfg.h"
#include "bt_spp.h"
#include "app_sdp.h"
#include "bt_rfc.h"
#include "board.h"
#include "dp_br_info.h"
#include "dp_ble_info.h"
#include "gap_msg.h"


#if F_APP_SPP_CAPTURE_DSP_DATA_2
#include "app_data_capture.h"
#endif

#define UART_TX_QUEUE_NO                    400
#define DT_QUEUE_NO                         16

#define DT_STATUS_IDLE                      0
#define DT_STATUS_ACTIVE                    1

typedef enum
{
    APP_TIMER_UART_ACK,
    APP_TIMER_UART_WAKE_UP,
    APP_TIMER_UART_TX_WAKE_UP,
    APP_TIMER_DATA_TRANSFER,
    APP_TRANSFER_TIMER_CHECK_CONSOLE_COMP,
    APP_TRANSFER_TIMER_CHECK_DATA_TRANSFER_SPP_BT,
} T_APP_TRANSFER_TIMER;

typedef struct
{
    uint8_t     *pkt_ptr;
    uint16_t    pkt_len;
    uint8_t     active;
    uint8_t     link_idx;
} T_DT_QUEUE;

typedef struct
{
    uint8_t     dt_queue_w_idx;
    uint8_t     dt_queue_r_idx;
    uint8_t     dt_resend_count;
    uint8_t     dt_status;
} T_DT_QUEUE_CTRL;

typedef struct
{
    uint8_t     *packet_ptr;
    uint16_t    packet_len;
    uint8_t     active;
    uint8_t     extra_param;
} T_UART_TX_QUEUE;

static uint8_t app_transfer_timer_id = 0;


static uint8_t timer_idx_uart_ack = 0;
static uint8_t timer_idx_uart_wake_up = 0;
static uint8_t timer_idx_uart_tx_wake_up = 0;
static uint8_t timer_idx_data_transfer = 0;
#if(F_APP_WATCH_CUSTOMER_CONSOLE_TX_BUF_EMPTY == 0)
static uint8_t timer_handle_console_send_comp = 0;
#endif
static uint8_t timer_handle_data_transfer_spp_bt = 0;

static T_DT_QUEUE_CTRL     dt_queue_ctrl;
static T_DT_QUEUE          *dt_queue = NULL;
static T_UART_TX_QUEUE     *uart_tx_queue = NULL;
static uint16_t uart_tx_rIndex;                 /**<uart transfer packet read index*/
static uint16_t uart_tx_wIndex;                 /**<uart transfer packet write index*/
static uint8_t uart_resend_count;              /**<uart resend count*/
uint8_t uart_tx_status = DT_STATUS_IDLE;
uint32_t uart_console_write_comp = 1;

// for CMD_LEGACY_DATA_TRANSFER and CMD_LE_DATA_TRANSFER
static uint8_t *uart_rx_dt_pkt_ptr = NULL;
static uint16_t uart_rx_dt_pkt_len;

static bool app_transfer_uart_empty_flag = false;

bool app_transfer_get_uart_empty_flag()
{
    return app_transfer_uart_empty_flag;
}

void app_transfer_set_uart_empty_flag(bool flag)
{
    app_transfer_uart_empty_flag = flag;
}

void app_transfer_queue_recv_ack_check(uint16_t event_id, uint8_t cmd_path)
{
    uint16_t    tx_queue_id = ((dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr[4]) |
                               (dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr[5] << 8));

    bool move_to_next = (event_id == tx_queue_id) ? true : false;

    app_pop_data_transfer_queue(cmd_path, move_to_next);
}

void app_transfer_queue_reset(uint8_t cmd_path)
{
    if (cmd_path == CMD_PATH_SPP || cmd_path == CMD_PATH_LE || cmd_path == CMD_PATH_IAP)
    {
        app_stop_timer(&timer_idx_data_transfer);

        for (uint8_t idx = 0 ; idx < DT_QUEUE_NO; idx++)
        {
            if (dt_queue[idx].active)
            {
                dt_queue[idx].active = 0;

                if (dt_queue[idx].pkt_ptr != NULL)
                {
                    free(dt_queue[idx].pkt_ptr);
                    dt_queue[idx].pkt_ptr = NULL;
                }
            }
        }

        dt_queue_ctrl.dt_queue_w_idx = 0;
        dt_queue_ctrl.dt_queue_r_idx = 0;
        dt_queue_ctrl.dt_resend_count = 0;
        dt_queue_ctrl.dt_status = DT_STATUS_IDLE;
    }
}

void app_pop_data_transfer_queue(uint8_t cmd_path, bool next_flag)
{
    T_UART_TX_QUEUE *p_queue;
    uint8_t     app_idx;
    uint8_t     *pkt_ptr;
    uint16_t    pkt_len;
    uint16_t    event_id;

    APP_PRINT_TRACE2("app_pop_data_transfer_queue: cmd_path %d, next_flag %d", cmd_path, next_flag);

    if (CMD_PATH_UART == cmd_path)
    {
        APP_PRINT_INFO0("app_pop_data_transfer_queue CMD_PATH_UART");
    }
    else
    {
        if (next_flag == true)
        {
            app_stop_timer(&timer_idx_data_transfer);
            DBG_DIRECT("SPP CPATURE DATA V2 dt_queue active 0x%x",
                       dt_queue[dt_queue_ctrl.dt_queue_r_idx].active);
            if (dt_queue[dt_queue_ctrl.dt_queue_r_idx].active)
            {
                dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                if (cmd_path == CMD_PATH_SPP  && (false) &&//app_cfg_const.enable_dsp_capture_data_by_spp
                    (dt_queue_ctrl.dt_resend_count < 20))
                {
                    uint8_t ptr[3] = {0};
                    ptr[0] = CMD_LEGACY_DATA_TRANSFER;
                    ptr[1] = CMD_LEGACY_DATA_TRANSFER >> 8;
                    ptr[2] = CMD_SET_STATUS_SPP;
                    app_cmd_set_event_ack(CMD_PATH_UART, dt_queue[dt_queue_ctrl.dt_queue_r_idx].link_idx, ptr);
                }

                if (dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr != NULL)
                {
                    free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                }

                dt_queue_ctrl.dt_queue_r_idx++;
                if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                {
                    dt_queue_ctrl.dt_queue_r_idx = 0;
                }
            }
            else
            {
                return;
            }
            dt_queue_ctrl.dt_resend_count = 0;
            dt_queue_ctrl.dt_status = DT_STATUS_IDLE;
        }

        app_idx = dt_queue[dt_queue_ctrl.dt_queue_r_idx].link_idx;
        if (dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr == NULL)
        {
            APP_PRINT_INFO3("SPP CAPTURE DATA V2 %s %d pkt_ptr 0x%x", TRACE_STRING(__FUNCTION__), __LINE__,
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
            return;
        }
        pkt_ptr = dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr;
        pkt_len = dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_len;
        event_id = ((dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr[4]) |
                    (dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr[5] << 8));

        APP_PRINT_INFO7("app_pop_data_transfer_queue: dt_status %d, active %d, connected_profile 0x%x, rfc_credit %d, pkt_len:%d, event_id:0x%x, r idx:%d",
                        dt_queue_ctrl.dt_status,
                        dt_queue[dt_queue_ctrl.dt_queue_r_idx].active,
                        br_db.br_link[app_idx].connected_profile,
                        br_db.br_link[app_idx].rfc_credit,
                        pkt_len, event_id, dt_queue_ctrl.dt_queue_r_idx);

        if (dt_queue_ctrl.dt_status == DT_STATUS_IDLE)
        {
            if (dt_queue[dt_queue_ctrl.dt_queue_r_idx].active == CMD_PATH_IAP)
            {
#if F_APP_IAP_RTK_SUPPORT && F_APP_IAP_SUPPORT
                APP_IAP_HDL app_iap_hdl = NULL;
                APP_PRINT_INFO2("app_pop_data_transfer_queue: iap credit %d, rtk iap connected %d",
                                app_iap_get_credit(app_iap_hdl), app_iap_rtk_connected(app_db.br_link[app_idx].bd_addr));

                if (app_db.br_link[app_idx].connected_profile & IAP_PROFILE_MASK)
                {
                    if (app_iap_rtk_connected(app_db.br_link[app_idx].bd_addr))
                    {
                        if (app_iap_rtk_send(app_db.br_link[app_idx].bd_addr,
                                             pkt_ptr, pkt_len) == true)
                        {
                            if (app_cfg_const.enable_embedded_cmd)
                            {
                                if (event_id == EVENT_ACK)
                                {
                                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                                    free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                                    dt_queue_ctrl.dt_queue_r_idx++;
                                    if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                                    {
                                        dt_queue_ctrl.dt_queue_r_idx = 0;
                                    }

                                    app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                    app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                                    1);
                                }
                                else
                                {
                                    dt_queue_ctrl.dt_status = DT_STATUS_ACTIVE;
                                    app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                    app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x01, false,
                                                    2000);
                                }
                            }
                        }
                        else
                        {
                            app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                            app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                            100);
                        }
                    }
                    else
                    {
                        if (app_iap_is_authened(app_iap_hdl))
                        {
                            app_iap_rtk_launch(app_db.br_link[app_idx].bd_addr, BT_IAP_APP_LAUNCH_WITH_USER_ALERT);
                        }
                        else
                        {
                            free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                            dt_queue_ctrl.dt_queue_r_idx++;
                            if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                            {
                                dt_queue_ctrl.dt_queue_r_idx = 0;
                            }
                            app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                            app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                            1);
                        }
                    }
                }
#endif
            }
            else if (dt_queue[dt_queue_ctrl.dt_queue_r_idx].active == CMD_PATH_SPP)
            {
                if (br_db.br_link[app_idx].connected_profile & SPP_PROFILE_MASK)
                {
                    if (br_db.br_link[app_idx].rfc_credit)
                    {
                        uint8_t local_server_chann =  RFC_SPP_CHANN_NUM;

                        if (br_db.br_link[app_idx].rtk_vendor_spp_active)
                        {
                            local_server_chann = RFC_RTK_VENDOR_CHANN_NUM;
                        }

#if (F_APP_DONGLE_FEATURE_SUPPORT == 1)
                        if (app_db.remote_is_8753bau)
                        {
                            local_server_chann = RFC_SPP_DONGLE_CHANN_NUM;
                        }
#endif
                        APP_PRINT_INFO4("app_pop_data_transfer_queue: local_server_chann %d dt_w %d dt_r %d tx_id %d",
                                        local_server_chann,
                                        dt_queue_ctrl.dt_queue_w_idx,
                                        dt_queue_ctrl.dt_queue_r_idx,
                                        dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr[1]);

                        if (bt_spp_data_send(br_db.br_link[app_idx].bd_addr, local_server_chann,
                                             pkt_ptr, pkt_len, false) == true)
                        {
#if (F_APP_DONGLE_FEATURE_SUPPORT == 1)
                            if (app_db.remote_is_8753bau)
                            {
                                app_db.br_link[app_idx].rfc_credit --;
                            }
#endif

                            if (true)//app_cfg_const.enable_embedded_cmd
                            {
                                if ((event_id == EVENT_ACK)
#if (F_APP_DONGLE_FEATURE_SUPPORT == 1)
                                    || (app_db.remote_is_8753bau)
#endif
                                   )
                                {
                                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                                    free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                                    dt_queue_ctrl.dt_queue_r_idx++;
                                    if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                                    {
                                        dt_queue_ctrl.dt_queue_r_idx = 0;
                                    }

                                    if (true)//app_cfg_const.enable_dsp_capture_data_by_spp
                                    {
                                        app_pop_data_transfer_queue(CMD_PATH_SPP, true);
                                    }
                                    else
                                    {
#if (F_APP_DONGLE_FEATURE_SUPPORT == 1)
                                        if (app_db.remote_is_8753bau)
                                        {
                                            //do nothing.
                                        }
                                        else
#endif
                                        {
                                            app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                            app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                                            1);
                                        }
                                    }
                                }
                                else
                                {
                                    if (true)//app_cfg_const.enable_dsp_capture_data_by_spp
                                    {
#if 0
                                        APP_PRINT_ERROR2("dsp_capture_send_data_decode, id = %x, pkt_len = %x",
                                                         pkt_ptr[13] << 8 | pkt_ptr[12],
                                                         pkt_len);
#endif
                                        app_pop_data_transfer_queue(CMD_PATH_SPP, true);
                                    }
                                    else
                                    {
                                        dt_queue_ctrl.dt_status = DT_STATUS_ACTIVE;
                                        app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                        app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x01, false,
                                                        2000);
                                    }
                                }
                            }
                            else
                            {
                                APP_PRINT_INFO0("send nrf success");
                                br_db.br_link[app_idx].rfc_credit--;
                                app_pop_data_transfer_queue(CMD_PATH_SPP, true);
                            }
                        }
                        else
                        {
                            APP_PRINT_TRACE1("send spp fail, app_idx =%d", app_idx);
                            if (true)//app_cfg_const.enable_dsp_capture_data_by_spp
                            {
#if F_APP_SAIYAN_MODE
                                if (data_capture_saiyan.saiyan_enable) // saiyan mode on
                                {
                                    app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                    app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                                    10);
                                }
                                else
#endif
                                {
                                    app_pop_data_transfer_queue(CMD_PATH_SPP, true);
                                }
                            }
                            else
                            {
                                app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                app_transfer_timer_id, APP_TRANSFER_TIMER_CHECK_DATA_TRANSFER_SPP_BT, 0x00, false,
                                                100);
                            }
                        }
                    }
                    else
                    {
#if F_APP_SPP_CAPTURE_DSP_DATA_2
                        if (app_data_capture_executing_check() == false)
#endif
                        {
                            free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                            dt_queue_ctrl.dt_queue_r_idx++;
                            if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                            {
                                dt_queue_ctrl.dt_queue_r_idx = 0;
                            }
                            dt_queue_ctrl.dt_resend_count = 0;
                            APP_PRINT_INFO1("rfc_credit 0 drop, app_idx =%d", app_idx);
                            app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                            app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                            1);
                        }
                    }
                }
            }
            else if (dt_queue[dt_queue_ctrl.dt_queue_r_idx].active == CMD_PATH_LE)
            {
                if (le_db.le_link[app_idx].state == GAP_CONN_STATE_CONNECTED)
                {
                    if (true)//le_db.le_link[app_idx].transmit_srv_tx_enable_fg == TX_ENABLE_READY
                    {
                        if (transmit_srv_tx_data(le_db.le_link[app_idx].conn_id, pkt_len, pkt_ptr) == true)
                        {
                            dt_queue_ctrl.dt_status = DT_STATUS_ACTIVE;
                            if (true)//app_cfg_const.enable_embedded_cmd
                            {
                                if (event_id != EVENT_ACK)
                                {
                                    app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                                    app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x01, false,
                                                    2000);
                                }
                            }
                        }
                        else
                        {
#if F_APP_WATCH_CUSTOMER_SPP_CAPTURE_DATA_V2_SUPPORT == 1
                            free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                            dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                            dt_queue_ctrl.dt_queue_r_idx++;
                            if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                            {
                                dt_queue_ctrl.dt_queue_r_idx = 0;
                            }
                            dt_queue_ctrl.dt_resend_count = 0;
                            //set timer to pop queue
                            app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                            app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                            1);
#else
                            app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                            app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                            100);
#endif
                        }
                    }
                    else
                    {
#if F_APP_WATCH_CUSTOMER_SPP_CAPTURE_DATA_V2_SUPPORT == 1
                        free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                        dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                        dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                        dt_queue_ctrl.dt_queue_r_idx++;
                        if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                        {
                            dt_queue_ctrl.dt_queue_r_idx = 0;
                        }
                        dt_queue_ctrl.dt_resend_count = 0;
                        //set timer to pop queue
                        app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                        app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                        1);
#else
                        app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                        app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                        1000);
#endif
                    }
                }
                else
                {
                    free(dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr);
                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].pkt_ptr = NULL;
                    dt_queue[dt_queue_ctrl.dt_queue_r_idx].active = 0;
                    dt_queue_ctrl.dt_queue_r_idx++;
                    if (dt_queue_ctrl.dt_queue_r_idx == DT_QUEUE_NO)
                    {
                        dt_queue_ctrl.dt_queue_r_idx = 0;
                    }
                    dt_queue_ctrl.dt_resend_count = 0;
                    //set timer to pop queue
                    app_start_timer(&timer_idx_data_transfer, "data_transfer",
                                    app_transfer_timer_id, APP_TIMER_DATA_TRANSFER, 0x00, false,
                                    1);
                }
            }
        }
    }
}

bool app_push_data_transfer_queue(uint8_t cmd_path, uint8_t *data, uint16_t data_len,
                                  uint8_t extra_param)
{
    APP_PRINT_TRACE4("app_push_data_transfer_queue: cmd_path %d, data_len %d w_idx %d r_idx %d",
                     cmd_path,
                     data_len,
                     dt_queue_ctrl.dt_queue_w_idx,
                     dt_queue_ctrl.dt_queue_r_idx);

    if (CMD_PATH_UART == cmd_path)
    {
        if (uart_tx_queue[uart_tx_wIndex].active == 0)
        {
            uart_tx_queue[uart_tx_wIndex].active = 1;
            uart_tx_queue[uart_tx_wIndex].packet_ptr = data;
            uart_tx_queue[uart_tx_wIndex].packet_len = data_len;
            uart_tx_queue[uart_tx_wIndex].extra_param = extra_param;
            uart_tx_wIndex++;
            uint16_t event_id = (data[5] << 8) | data[4];
            if ((event_id != EVENT_XM_BT_SDP_RECORD_DATA_IND) && (event_id != EVENT_LE_SCAN_INFO))
            {
                APP_PRINT_INFO5("event id 0x%x seq 0x%x write index %d, read index = %d, queue number = %d",
                                event_id, data[1],
                                uart_tx_wIndex, uart_tx_rIndex, UART_TX_QUEUE_NO);
            }
            if (uart_tx_wIndex >= UART_TX_QUEUE_NO)
            {
                uart_tx_wIndex = 0;
            }

            app_pop_data_transfer_queue(cmd_path, false);
            return true;
        }
        else
        {
            APP_PRINT_ERROR0("app_push_data_transfer_queue: uart_tx_queue full");
            return false;
        }
    }
    else
    {
        if (dt_queue[dt_queue_ctrl.dt_queue_w_idx].active == 0)
        {
            dt_queue[dt_queue_ctrl.dt_queue_w_idx].active = cmd_path;
            dt_queue[dt_queue_ctrl.dt_queue_w_idx].link_idx = extra_param;
            dt_queue[dt_queue_ctrl.dt_queue_w_idx].pkt_ptr = data;
            dt_queue[dt_queue_ctrl.dt_queue_w_idx].pkt_len = data_len;
            dt_queue_ctrl.dt_queue_w_idx++;
            if (dt_queue_ctrl.dt_queue_w_idx == DT_QUEUE_NO)
            {
                dt_queue_ctrl.dt_queue_w_idx = 0;
            }


            uint8_t idx = extra_param;
            uint8_t *bd_addr = br_db.br_link[idx].bd_addr;
            APP_PRINT_INFO2("app_push_data_transfer_queue: idx %d, bd_addr %s", idx, TRACE_BDADDR(bd_addr));

            app_pop_data_transfer_queue(cmd_path, false);
            return true;
        }
        else
        {
            APP_PRINT_ERROR1("app_push_data_transfer_queue: dt_queue[%d] full", dt_queue_ctrl.dt_queue_w_idx);
            return false;
        }
    }
}

bool app_transfer_check_active(uint8_t cmd_path)
{
    if (CMD_PATH_UART == cmd_path)
    {
        if (uart_tx_queue[uart_tx_wIndex].active == 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if (dt_queue[dt_queue_ctrl.dt_queue_w_idx].active == 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

static void app_transfer_timeout_cb(uint8_t timer_evt, uint16_t param)
{
    APP_PRINT_TRACE2("app_transfer_timeout_cb: timer_id %d, timer_chann %d", timer_evt, param);

    switch (timer_evt)
    {
#if(F_APP_WATCH_CUSTOMER_CONSOLE_TX_BUF_EMPTY == 0)
    case APP_TRANSFER_TIMER_CHECK_CONSOLE_COMP:
        {
            app_stop_timer(&timer_handle_console_send_comp);
            uart_tx_status = DT_STATUS_IDLE;
            app_pop_data_transfer_queue(CMD_PATH_UART, false);
        }
        break;
#endif
    case APP_TIMER_UART_ACK:
        {
            APP_PRINT_INFO0("APP_TIMER_UART_ACK");
        }
        break;

    case APP_TIMER_UART_WAKE_UP:
        APP_PRINT_INFO0("APP_TIMER_UART_WAKE_UP");
        break;

    case APP_TIMER_UART_TX_WAKE_UP:
        if (!app_transfer_uart_empty_flag)
        {
            APP_PRINT_INFO1("app_transfer_uart_empty_flag: %d", app_transfer_uart_empty_flag);
            app_start_timer(&timer_idx_uart_tx_wake_up, "uart_tx_wake_up",
                            app_transfer_timer_id, APP_TIMER_UART_TX_WAKE_UP, 0, false,
                            30);//stop wakeup signal after timeout
            break;
        }
        app_stop_timer(&timer_idx_uart_tx_wake_up);
#if (F_APP_WATCH_CUSTOMER_UART_FLOW_SUPPORT == 1)
        app_dlps_enable(APP_DLPS_ENTER_CHECK_UART_TX);
#if F_APP_WATCH_CUSTOMER_LOG_WAKE_UP_SPPORT
        tx_wakeup_low(TX_WAKE_UP_TRIGGER_BY_UART_TX);
#else
        Pad_Config(app_cfg_const.tx_wake_up_pinmux, \
                   PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_DOWN, PAD_OUT_ENABLE, PAD_OUT_LOW);
#endif
#else
        app_pop_data_transfer_queue(CMD_PATH_UART, false);
#endif
        break;

    case APP_TIMER_DATA_TRANSFER:
        app_stop_timer(&timer_idx_data_transfer);
        dt_queue_ctrl.dt_status = DT_STATUS_IDLE;
        if (param == 0x01) //ack timeout
        {
            dt_queue_ctrl.dt_resend_count++;
            //dt_queue_ctrl.dt_status = DT_STATUS_IDLE;
            if (dt_queue_ctrl.dt_resend_count >= 3)//app_cfg_const.dt_resend_num
            {
                app_pop_data_transfer_queue(dt_queue[dt_queue_ctrl.dt_queue_r_idx].active, true);
            }
            else
            {
                app_pop_data_transfer_queue(dt_queue[dt_queue_ctrl.dt_queue_r_idx].active, false);
            }
        }
        else
        {
            app_pop_data_transfer_queue(dt_queue[dt_queue_ctrl.dt_queue_r_idx].active, false);
        }
        break;

    case APP_TRANSFER_TIMER_CHECK_DATA_TRANSFER_SPP_BT:
        app_stop_timer(&timer_handle_data_transfer_spp_bt);
        dt_queue_ctrl.dt_status = DT_STATUS_IDLE;
        APP_PRINT_INFO1("app_transfer_timeout_cb: dt_queue_ctrl.dt_resend_count %d",
                        dt_queue_ctrl.dt_resend_count);
        dt_queue_ctrl.dt_resend_count++;
        if (dt_queue_ctrl.dt_resend_count >= 20)
        {
            app_pop_data_transfer_queue(dt_queue[dt_queue_ctrl.dt_queue_r_idx].active, true);
        }
        else
        {
            uint8_t ptr[3] = {0};
            ptr[0] = CMD_LEGACY_DATA_TRANSFER;
            ptr[1] = CMD_LEGACY_DATA_TRANSFER >> 8;
            ptr[2] = CMD_SET_STATUS_SPP_ERROR;
            app_cmd_set_event_ack(CMD_PATH_UART, dt_queue[dt_queue_ctrl.dt_queue_r_idx].link_idx, ptr);
            app_pop_data_transfer_queue(dt_queue[dt_queue_ctrl.dt_queue_r_idx].active, false);
        }
        break;
    default:
        break;
    }
}

#if(F_APP_WATCH_CUSTOMER_CONSOLE_TX_BUF_EMPTY == 1)
void app_transfer_pop_uart_queue_after_tx_buf_empty(void)
{
    APP_PRINT_TRACE0("app_transfer_pop_uart_queue_after_tx_buf_empty");
    app_transfer_uart_empty_flag = true;
    if (uart_console_write_comp == 0)
    {
        uart_tx_status = DT_STATUS_IDLE;
        app_pop_data_transfer_queue(CMD_PATH_UART, false);
    }
}
#endif

void app_transfer_init(void)
{
    //br_db.external_mcu_mtu = APP_WATCH_CUSTOMER_BUFFER_SIZE;
    if ((dt_queue == NULL) && (uart_tx_queue == NULL))
    {
        dt_queue = malloc(DT_QUEUE_NO * sizeof(T_DT_QUEUE));
        uart_tx_queue = malloc(UART_TX_QUEUE_NO * sizeof(T_UART_TX_QUEUE));
    }
    if (dt_queue != NULL && uart_tx_queue != NULL)
    {
        memset(dt_queue, 0, DT_QUEUE_NO * sizeof(T_DT_QUEUE));
        memset(uart_tx_queue, 0, UART_TX_QUEUE_NO * sizeof(T_UART_TX_QUEUE));
    }
    else
    {
        APP_PRINT_ERROR0("uart queue error!!!");
    }
    app_timer_reg_cb(app_transfer_timeout_cb, &app_transfer_timer_id);
}

static void app_transfer_bt_data(uint8_t *cmd_ptr, uint8_t cmd_path, uint8_t app_idx,
                                 uint8_t *ack_pkt)
{
    uint16_t cmd_id = (uint16_t)(cmd_ptr[0] | (cmd_ptr[1] << 8));
    uint16_t total_len;
    uint16_t pkt_len;
    uint8_t  idx;
    uint8_t  pkt_type;
    uint8_t  *pkt_ptr;

    idx        = cmd_ptr[2];
    pkt_type   = cmd_ptr[3];
    total_len  = (cmd_ptr[4] | (cmd_ptr[5] << 8));
    pkt_len    = (cmd_ptr[6] | (cmd_ptr[7] << 8));
    pkt_ptr    = &cmd_ptr[8];

    if (((cmd_id == CMD_LEGACY_DATA_TRANSFER) &&
         ((br_db.br_link[idx].connected_profile & SPP_PROFILE_MASK) ||
          (br_db.br_link[idx].connected_profile & IAP_PROFILE_MASK))) ||
        ((cmd_id == CMD_LE_DATA_TRANSFER) && (le_db.le_link[idx].state == GAP_CONN_STATE_CONNECTED)))
    {
        if (cmd_path == CMD_PATH_UART)
        {
            if (pkt_len)
            {
                if ((pkt_type == PKT_TYPE_SINGLE) || (pkt_type == PKT_TYPE_START))
                {
                    if ((cmd_id == CMD_LEGACY_DATA_TRANSFER) &&
                        (((br_db.br_link[idx].connected_profile & SPP_PROFILE_MASK) &&
                          (!app_transfer_check_active(CMD_PATH_SPP))) ||
                         (((br_db.br_link[idx].connected_profile & IAP_PROFILE_MASK) &&
                           (!app_transfer_check_active(CMD_PATH_IAP))))))
                    {
                        if (br_db.br_link[idx].uart_rx_dt_pkt_ptr)
                        {
                            free(br_db.br_link[idx].uart_rx_dt_pkt_ptr);
                        }

                        br_db.br_link[idx].uart_rx_dt_pkt_ptr = malloc(total_len);
                        memcpy(br_db.br_link[idx].uart_rx_dt_pkt_ptr, pkt_ptr, pkt_len);
                        br_db.br_link[idx].uart_rx_dt_pkt_len = pkt_len;
                    }
                    else if ((cmd_id == CMD_LE_DATA_TRANSFER) && (!app_transfer_check_active(CMD_PATH_LE)))
                    {
                        if (uart_rx_dt_pkt_ptr)
                        {
                            free(uart_rx_dt_pkt_ptr);
                        }

                        uart_rx_dt_pkt_ptr = malloc(total_len);
                        memcpy(uart_rx_dt_pkt_ptr, pkt_ptr, pkt_len);
                        uart_rx_dt_pkt_len = pkt_len;
                    }
                    else
                    {
                        ack_pkt[2] = CMD_SET_STATUS_DISALLOW;
                    }
                }
                else
                {
                    if ((cmd_id == CMD_LEGACY_DATA_TRANSFER) && (br_db.br_link[idx].uart_rx_dt_pkt_ptr))
                    {
                        uint8_t *temp_ptr;

                        temp_ptr = br_db.br_link[idx].uart_rx_dt_pkt_ptr +
                                   br_db.br_link[idx].uart_rx_dt_pkt_len;
                        memcpy(temp_ptr, pkt_ptr, pkt_len);
                        br_db.br_link[idx].uart_rx_dt_pkt_len += pkt_len;
                    }
                    else if ((cmd_id == CMD_LE_DATA_TRANSFER) && uart_rx_dt_pkt_ptr)
                    {
                        uint8_t *temp_ptr;

                        temp_ptr = uart_rx_dt_pkt_ptr + uart_rx_dt_pkt_len;
                        memcpy(temp_ptr, pkt_ptr, pkt_len);
                        uart_rx_dt_pkt_len += pkt_len;
                    }
                    else//maybe start packet been lost
                    {
                        ack_pkt[2] = CMD_SET_STATUS_PARAMETER_ERROR;
                    }
                }

                if ((pkt_type == PKT_TYPE_SINGLE) || (pkt_type == PKT_TYPE_END))
                {
                    if (ack_pkt[2] == CMD_SET_STATUS_COMPLETE)
                    {
                        if (cmd_id == CMD_LEGACY_DATA_TRANSFER)
                        {
                            if (br_db.br_link[idx].connected_profile & SPP_PROFILE_MASK)
                            {
                                app_report_raw_data(CMD_PATH_SPP, app_idx, br_db.br_link[idx].uart_rx_dt_pkt_ptr,
                                                    br_db.br_link[idx].uart_rx_dt_pkt_len);

                                free(br_db.br_link[idx].uart_rx_dt_pkt_ptr);
                                br_db.br_link[idx].uart_rx_dt_pkt_ptr = NULL;

                                if (app_transfer_check_active(CMD_PATH_SPP))
                                {
                                    ack_pkt[2] = CMD_SET_STATUS_BUSY;
                                    br_db.br_link[idx].resume_fg = 0x01;
                                }
                            }
                            else if (br_db.br_link[idx].connected_profile & IAP_PROFILE_MASK)
                            {
                                app_report_raw_data(CMD_PATH_IAP, app_idx, br_db.br_link[idx].uart_rx_dt_pkt_ptr,
                                                    br_db.br_link[idx].uart_rx_dt_pkt_len);

                                free(br_db.br_link[idx].uart_rx_dt_pkt_ptr);
                                br_db.br_link[idx].uart_rx_dt_pkt_ptr = NULL;

                                if (app_transfer_check_active(CMD_PATH_IAP))
                                {
                                    ack_pkt[2] = CMD_SET_STATUS_BUSY;
                                    br_db.br_link[idx].resume_fg = 0x01;
                                }
                            }
                        }
                        else if (cmd_id == CMD_LE_DATA_TRANSFER)
                        {
                            app_report_raw_data(CMD_PATH_LE, app_idx, uart_rx_dt_pkt_ptr, uart_rx_dt_pkt_len);

                            free(uart_rx_dt_pkt_ptr);
                            uart_rx_dt_pkt_ptr = NULL;

                            if (app_transfer_check_active(CMD_PATH_LE))
                            {
                                ack_pkt[2] = CMD_SET_STATUS_BUSY;
                            }
                        }
                    }
                }
            }
            else
            {
                ack_pkt[2] = CMD_SET_STATUS_PARAMETER_ERROR;
            }
        }
    }
    else
    {
        ack_pkt[2] = CMD_SET_STATUS_DISALLOW;
    }

    app_report_event(cmd_path, EVENT_ACK, app_idx, ack_pkt, 3);
}

void app_transfer_cmd_handle(uint8_t *cmd_ptr, uint16_t cmd_len, uint8_t cmd_path, uint8_t app_idx,
                             uint8_t *ack_pkt)
{
    uint16_t cmd_id = (uint16_t)(cmd_ptr[0] | (cmd_ptr[1] << 8));

    switch (cmd_id)
    {
    case CMD_LEGACY_DATA_TRANSFER:
    case CMD_LE_DATA_TRANSFER:
        {
            app_transfer_bt_data(&cmd_ptr[0], cmd_path, app_idx, &ack_pkt[0]);
        }
        break;

    default:
        {
            ack_pkt[2] = CMD_SET_STATUS_UNKNOW_CMD;
            app_report_event(cmd_path, EVENT_ACK, app_idx, ack_pkt, 3);
        }
        break;
    }
}

