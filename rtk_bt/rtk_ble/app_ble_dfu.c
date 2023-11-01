#include "app_ble_dfu.h"
#include "trace.h"
#include "ble_gap_msg.h"
#include "dfu_service.h"
#include "ble_dfu_transport.h"

static T_SERVER_ID dfu_srv_id;


static T_APP_RESULT app_dfu_callback(T_SERVER_ID service_id, void *p_data)
{
    T_DFU_CALLBACK_DATA *p_dfu_cb_data = (T_DFU_CALLBACK_DATA *)p_data;
    switch (p_dfu_cb_data->msg_type)
    {
    case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION:
        {
            if (p_dfu_cb_data->msg_data.notification_indification_index == DFU_CP_NOTIFY_ENABLE)
            {
                APP_PRINT_INFO0("dfu notification enable");
            }
            else if (p_dfu_cb_data->msg_data.notification_indification_index ==
                     DFU_CP_NOTIFY_DISABLE)
            {
                APP_PRINT_INFO0("dfu notification disable");
            }
        }
        break;
    case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE:
        {
            uint8_t dfu_write_opcode = p_dfu_cb_data->msg_data.write.opcode;
            if (dfu_write_opcode == DFU_WRITE_ATTR_EXIT)
            {
                if (p_dfu_cb_data->msg_data.write.write_attrib_index == INDEX_DFU_CHAR_DFU_CONTROL_POINT_INDEX)
                {
                    uint8_t control_point_opcode = *p_dfu_cb_data->msg_data.write.p_value;
                    switch (control_point_opcode)
                    {
                    case DFU_OPCODE_VALID_FW:
                        {
//                            T_IO_MSG dfu_valid_fw_msg;
//                            dfu_valid_fw_msg.type = IO_MSG_TYPE_DFU_VALID_FW;
//                            dfu_valid_fw_msg.u.param = p_data->conn_id;
//                            if (app_send_msg_to_apptask(&dfu_valid_fw_msg) == false)
//                            {
//                                DBG_DIRECT("DFU send Valid FW msg fail!");
//                            }
                        }
                        break;
                    case DFU_OPCODE_ACTIVE_IMAGE_RESET:
                        {
#if (ENABLE_AUTO_BANK_SWITCH == 1)
//                            if (is_ota_support_bank_switch())
//                            {
//                                uint32_t ota_addr;
//                                ota_addr = get_header_addr_by_img_id(IMG_OTA);
//                                DFU_PRINT_INFO1("DFU_OPCODE_ACTIVE_IMAGE_RESET: Bank switch erase ota_addr=0x%x", ota_addr);
//                                unlock_flash_bp_all();
//                                flash_nor_erase_locked(ota_addr, FLASH_NOR_ERASE_SECTOR);
//                                lock_flash_bp();
//                            }
#endif
//                            le_disconnect(0);
//                            app_dfu_active_rst_pending = true;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
            else if (dfu_write_opcode == DFU_WRITE_FAIL)
            {
                DBG_DIRECT("DFU FAIL!");
            }
            else if (dfu_write_opcode == DFU_WRITE_ATTR_ENTER)
            {
                // application can add check conditions before start dfu procefure
                //if check fail, return some error code except APP_RESULT_SUCCESS to exit dfu procedure
                //app_result = APP_RESULT_REJECT;
                //APP_PRINT_INFO0("exit dfu procedure");
            }
            else
            {
            }
        }
        break;
    default:
        break;
    }
    return APP_RESULT_SUCCESS;
}

static void app_dfu_msg(T_IO_MSG *p_gap_msg)
{
    APP_PRINT_INFO1("line = %d \n", __LINE__);
    APP_PRINT_INFO1("app_handle_gap_msg: subtype %d \n", p_gap_msg->subtype);
}

void app_ble_dfu_init(void)
{
    dfu_srv_id  = dfu_add_service(app_dfu_callback);
    le_msg_handler_cback_register(app_dfu_msg);
}
