#include "app_ble_ota.h"
#include "trace.h"
#include "ble_gap_msg.h"
#include "ota_service.h"
#include "gap_conn_le.h"
extern bool dfu_switch_to_ota_mode_pending;

static T_SERVER_ID ota_srv_id;


static T_APP_RESULT app_ota_callback(T_SERVER_ID service_id, void *p_data)
{
    if (service_id == ota_srv_id)
    {
        T_OTA_CALLBACK_DATA *p_ota_cb_data = (T_OTA_CALLBACK_DATA *)p_data;
        switch (p_ota_cb_data->msg_type)
        {
        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
            {
#ifdef RTL87x2G
                if (OTA_WRITE_CHAR_VAL == p_ota_cb_data->msg_data.write.opcode &&
                    OTA_VALUE_ENTER == p_ota_cb_data->msg_data.write.value)
                {
                    /*battery level is above 60 percent*/
                    APP_PRINT_INFO0("Preparing switch into OTA mode");
                    /*prepare to enter OTA mode, before switch action, we should disconnect first.*/
                    dfu_switch_to_ota_mode_pending = true;
                    le_disconnect(p_ota_cb_data->conn_id);
                }
#elif RTL8762D
                if (OTA_WRITE_CHAR_VAL == p_ota_cb_data->msg_data.write.opcode &&
                    OTA_VALUE_ENTER == p_ota_cb_data->msg_data.write.u.value)
                {
                    /*battery level is above 60 percent*/
                    APP_PRINT_INFO0("Preparing switch into OTA mode\n");
                    /*prepare to enter OTA mode, before switch action, we should disconnect first.*/
                    dfu_switch_to_ota_mode_pending = true;
                    le_disconnect(0);
                }
#endif
            }
            break;
        default:
            break;
        }
    }
    return APP_RESULT_SUCCESS;
}

static void app_ota_msg(T_IO_MSG *p_gap_msg)
{
    APP_PRINT_INFO1("line = %d \n", __LINE__);
    APP_PRINT_INFO1("app_handle_gap_msg: subtype %d \n", p_gap_msg->subtype);
}

void app_ble_ota_init(void)
{
    ota_srv_id  = ota_add_service(app_ota_callback);
    le_msg_handler_cback_register(app_ota_msg);
}
