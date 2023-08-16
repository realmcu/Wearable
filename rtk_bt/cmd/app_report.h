/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _APP_REPORT_H_
#define _APP_REPORT_H_

#include <stdint.h>
#if F_APP_APT_SUPPORT
#include "app_audio_passthrough.h"
#endif

#include "app_eq.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup APP_REPORT App Report
  * @brief App Report
  * @{
  */
//for EVENT_VOLUME_SYNC
#define SYNC_TYPE_HF_GAIN_LEVEL             0x00
#define SYNC_TYPE_AVRCP_UP_DN               0x01
#define SYNC_TYPE_AVRCP_ABSOLUTE_VOL        0x02

//for EVENT_CALLER_ID
#define CALLER_ID_TYPE_NUMBER               0x00
#define CALLER_ID_TYPE_NAME                 0x01

typedef enum
{
    CMD_PATH_NONE = 0x00,
    CMD_PATH_UART = 0x01,
    CMD_PATH_LE   = 0x02,
    CMD_PATH_SPP  = 0x03,
    CMD_PATH_IAP  = 0x04,

    CMD_PATH_RWS_ASYNC = 0x10,
    CMD_PATH_RWS_SYNC  = 0x11,
} T_CMD_PATH;

typedef enum
{
    EVENT_ACK                               = 0x0000,
    EVENT_REPLY_PAIRED_RECORD               = 0x0001,
    EVENT_LEGACY_DATA_TRANSFER              = 0x0002,
    EVENT_CONNECT_STATUS                    = 0x0003,
    EVENT_DISCONNECT_STATUS                 = 0x0004,
    EVENT_CALLER_ID                         = 0x0005,
    EVENT_IAP_STATUS                        = 0x0006,
    EVENT_DEVICE_STATE                      = 0x0007,
    EVENT_REPLY_LINK_INFO                   = 0x0008,
    EVENT_REPLY_REMOTE_NAME                 = 0x0009,
    EVENT_CALL_STATUS                       = 0x000A,
    EVENT_PLAYER_STATUS                     = 0x000B,
    EVENT_FEATURE_REPORT                    = 0x000C,
    EVENT_REPORT_SPP_INFO                   = 0x000D,
    EVENT_TTS                               = 0x000E,
    EVENT_VENDOR_AT_RESULT                  = 0x0010,
    EVENT_INFO_RSP                          = 0x0011,
    EVENT_RESUME_DATA_TRANSFER              = 0x0012,
    EVENT_VOLUME_SYNC                       = 0x0013,
    EVENT_LINK_STATUS                       = 0x0014,
    EVENT_CODEC_SETTING                     = 0x0015,
    EVENT_INITIATE_CONN_DONE                = 0x0016,
    EVENT_LANGUAGE_REPORT                   = 0x0017,
    EVENT_REPORT_CFG_TYPE                   = 0x0018,
    EVENT_REPORT_STATUS                     = 0x0019,  // used to report part of bud info
    EVENT_GET_BD_ADDR                       = 0x001A,
    EVENT_REPORT_DLPS_STATUS                = 0x001C,

    EVENT_AUDIO_VOL_CHANGE                  = 0x0020,
    EVENT_TRACK_CHANGED                     = 0x0021,
    EVENT_REPORT_BUD_INFO                   = 0x0022,  // used to report complete bud info

    /*#if F_APP_DEVICE_CMD_SUPPORT
        EVENT_BT_READY                          = 0x0023,
        EVENT_REPORT_PIN_CODE                   = 0x0024,
        EVENT_INQUIRY_STATE                     = 0x0025,
        EVENT_SERVICES_SEARCH_STATE             = 0x0026,
        EVENT_PAIRING_REQUEST                   = 0x0027,
        EVENT_REPORT_SSP_NUMERIC_VALUE          = 0x0028,
        EVENT_LEGACY_RSSI                       = 0x0029,
        EVENT_REPORT_CONNECTED_DEV_ID           = 0x002A,
        EVENT_INQUIRY_RESULT                    = 0x002B,
        EVENT_REPORT_REMOTE_DEV_ATTR_INFO       = 0x002C,
    #endif*/

    EVENT_PROFILE_CONNECT_FAIL_STATUS       = 0x002F,

    EVENT_LE_CONNECTED                      = 0x0100,
    EVENT_LE_DISCONNECTED                   = 0x0101,
    EVENT_LE_PAIR_STATUS                    = 0x0102,
    EVENT_LE_DATA_TRANSFER                  = 0x0103,
    EVENT_LE_PUBLIC_ADDR                    = 0x0104,
    EVENT_LE_SCAN_INFO                      = 0x0105,
    EVENT_LE_SET_DATA_LEN                   = 0x0106,
    EVENT_LE_PHY_UPD                        = 0x0107,
    EVENT_LE_REMOTE_FEATURES                = 0x0108,
    EVENT_LE_BONDED_DEV                     = 0x0109,
    EVENT_LE_IGNORE_SLAVE_LATENCY           = 0x010A,

    EVENT_ANCS_REGISTER_COMPLETE            = 0x0110,
    EVENT_ANCS_NOTIFICATION                 = 0x0111,
    EVENT_ANCS_ACTION_RESULT                = 0x0112,

    EVENT_AMS_REGISTER_COMPLETE             = 0x0120,
    EVENT_AMS_ENTITY_UPD_NOTIFY             = 0x0121,
    EVENT_AMS_ENTITY_UPD_NOTIFY_EN          = 0x0122,
    EVENT_AMS_REMOTE_CMD_NOTIFY             = 0x0123,
    EVENT_AMS_REMOTE_CMD_NOTIFY_EN          = 0x0124,
    EVENT_AMS_ENTITY_UPD_RESULT             = 0x0125,

    EVENT_AUDIO_EQ_REPLY                    = 0x0200,
    EVENT_AUDIO_EQ_PARAM_REPORT             = 0x0201,
    EVENT_AUDIO_EQ_INDEX_REPORT             = 0x0202,

    EVENT_DSP_READ_PARAM                    = 0x0203,
    EVENT_DSP_REPORT_AUDIO_EQ               = 0x0204,
#if F_APP_APT_SUPPORT
    EVENT_APT_VOLUME_OUT_LEVEL              = 0x0205,
#endif
    EVENT_APT_EQ_INDEX_INFO                 = 0x0206,

    EVENT_AUDIO_DSP_CTRL_INFO               = 0x0207,
    // for equalize page
    EVENT_AUDIO_EQ_REPLY_PARAM              = 0x0208,
    EVENT_TONE_VOLUME_STATUS                = 0x0209,
    EVENT_TONE_VOLUME_LEVEL_SET             = 0x020A,
    EVENT_DSP_TOOL_FUNCTION_ADJUSTMENT      = 0x0213,

#if F_APP_SPP_CAPTURE_DSP_DATA_2
    EVENT_DSP_CAPTURE_V2_START_STOP_RESULT      = 0x0220,
    EVENT_DSP_CAPTURE_V2_DATA                   = 0x0221,
    EVT_DSP_CAPTURE_SCENARIO_ENTER_EXIT_RESULT  = 0x0222,
#endif

#if F_APP_HFP_CMD_SUPPORT
    EVENT_SCO_STATE                         = 0x0300,
    EVENT_CALL_WAITING                      = 0x0301,
    EVENT_HFP_SIGNAL                        = 0x0302,
    EVENT_HFP_ROAM                          = 0x0303,
    EVENT_HFP_SERVICE                       = 0x0304,
    EVENT_NETWORK_OPERATOR                  = 0x0305,
    EVENT_SUBSCRIBER_NUMBER                 = 0x0306,
    EVENT_CURRENT_CALLS                     = 0x0307,
    EVENT_DTMF                              = 0x0308,
    EVENT_REPORT_HFP_BATTERY                = 0x0309,
    EVENT_REPORT_AG_BRSF                    = 0x030A,
#endif
    EVENT_RING_IND                          = 0x030B,

    EVENT_REPORT_NUM_OF_CONNECTION          = 0x0315,
    EVENT_REG_ACCESS                        = 0x0316,
    EVENT_RF_XTAL_K                         = 0x032A,
    EVENT_RF_XTAL_K_GET_RESULT              = 0x032B,

    //0x0400 ~ 0x04FF reserved for profile
    /*#if F_APP_HFP_CMD_SUPPORT
        EVENT_CALL_WAITING                      = 0x0401,
        EVENT_HFP_SIGNAL                        = 0x0402,
        EVENT_HFP_ROAM                          = 0x0403,
        EVENT_HFP_SERVICE                       = 0x0404,
        EVENT_NETWORK_OPERATOR                  = 0x0405,
        EVENT_SUBSCRIBER_NUMBER                 = 0x0406,
        EVENT_CURRENT_CALLS                     = 0x0407,
        EVENT_DTMF                              = 0x0408,
        EVENT_REPORT_HFP_BATTERY                = 0x0409,
        EVENT_REPORT_AG_BRSF                    = 0x040A,
    #endif*/

#if F_APP_AVRCP_CMD_SUPPORT
    EVENT_AVRCP_REPORT_LIST_SETTING_ATTR    = 0x0400,
    EVENT_AVRCP_REPORT_LIST_SETTING_VALUE   = 0x0401,
    EVENT_AVRCP_REPORT_CURRENT_VALUE        = 0x0402,
    EVENT_AVRCP_REPORT_SETTING_CHANGED      = 0x0403,
    EVENT_AVRCP_REPORT_ELEMENT_ATTR         = 0x0404,
    EVENT_AVRCP_REPORT_PLAYER_STATUS        = 0x0405,
#endif

#if F_APP_PBAP_CMD_SUPPORT
    EVENT_PBAP_REPORT_DATA                  = 0x0500,
    EVENT_PBAP_REPORT_SESSION_STATUS        = 0x0501,
    EVENT_PBAP_DOWNLOAD_START               = 0x0502,
    EVENT_PBAP_DOWNLOAD_CMPL                = 0x0503,
    EVENT_PBAP_REPORT_SIZE                  = 0x0504,
    EVENT_PBAP_CONNECT_FAIl                 = 0x0505,
#endif

    EVENT_OTA_DEV_INFO                      = 0x0600,
    EVENT_OTA_ACTIVE_BANK_VER               = 0x0601,
    EVENT_OTA_START                         = 0x0602,
    EVENT_OTA_PACKET                        = 0x0603,
    EVENT_OTA_VALID                         = 0x0604,
    EVENT_OTA_BUFFER_CHECK_ENABLE           = 0x0605,
    EVENT_OTA_BUFFER_CHECK                  = 0x0606,
    EVENT_OTA_IMG_INFO                      = 0x0607,
    EVENT_OTA_SECTION_SIZE                  = 0x0608,
    EVENT_OTA_DEV_EXTRA_INFO                = 0x0609,
    EVENT_OTA_PROTOCOL_TYPE                 = 0x060A,
    EVENT_OTA_ACTIVE_ACK                    = 0x060B,
    EVENT_OTA_GET_RELEASE_VER               = 0x060C,
    EVENT_OTA_INACTIVE_BANK_VER             = 0x060D,
    EVENT_OTA_COPY_IMG                      = 0x060E,
    EVENT_OTA_CHECK_SHA256                  = 0x060F,
    EVENT_OTA_ROLESWAP                      = 0x0610,

    EVENT_PLAYBACK_QUERY_INFO               = 0x0680,
    EVENT_PLAYBACK_GET_LIST_DATA            = 0x0681,
    EVENT_PLAYBACK_TRANS_START              = 0x0682,
    EVENT_PLAYBACK_REPORT_BUFFER_CHECK      = 0x0683,
    EVENT_PLAYBACK_VALID_SONG               = 0x0684,
    EVENT_PLAYBACK_TRIGGER_ROLE_SWAP        = 0x0685,
    EVENT_PLAYBACK_TRANS_CANCEL             = 0x0686,
    EVENT_PLAYBACK_EXIT_TRANS               = 0x0687,
    EVENT_PLAYBACK_PERMANENT_DELETE_SONG    = 0x0688,
    EVENT_PLAYBACK_PLAYLIST_ADD_SONG        = 0x0689,
    EVENT_PLAYBACK_PLAYLIST_DELETE_SONG     = 0x068A,
    EVENT_PLAYBACK_PERMANENT_DELETE_ALL_SONG = 0x068B,

    EVENT_REPORT_SUPPORTED_MMI_LIST         = 0x0700,
    EVENT_REPORT_SUPPORTED_CLICK_TYPE       = 0x0701,
    EVENT_REPORT_SUPPORTED_CALL_STATE       = 0x0702,
    EVENT_REPORT_KEY_MMI_MAP                = 0x0703,

    EVENT_REPORT_USER_BIN_VER               = 0x0705,
    EVENT_REPORT_PAD_VOLTAGE                = 0x0706,
    EVENT_PX318J_CALIBRATION_REPORT         = 0x0707,
    EVENT_REPORT_RWS_KEY_MMI_MAP            = 0x0708,
    EVENT_IQS773_STREAM_REPORT              = 0x0709,

    EVENT_VENDOR_SEPC                       = 0x8800, //It has been reserved for vendor customer A, please dont't use this value.

    EVENT_LED_TEST                          = 0x0900,
    EVENT_KEY_TEST                          = 0x0901,
    EVENT_CLEAR_MP_DATA                     = 0x0902,
    EVENT_LOCAL_ADDR                        = 0x0903,
    EVENT_BT_BOND_INFO_CLEAR                = 0x0904,
    EVENT_DEVICE_ROLE                       = 0x0905,
    EVENT_CRYSTAL_TRIM                      = 0x0906,
    EVENT_LINK_KEY                          = 0x0907,
    EVENT_COUNTRY_CODE                      = 0x0908,
    EVENT_FW_VERSION                        = 0x0909,
    EVENT_GET_FLASH_BKP_DATA                = 0x090A,
    EVENT_ADC_VALUE_1                       = 0x090B,
    EVENT_ADC_VALUE_2                       = 0x090C,
    EVENT_GET_UNSIZE_RAM                    = 0x090D,
    EVENT_REPORT_FLASH_DATA                 = 0x090E,
    EVENT_MIC_SWITCH                        = 0x090F,
    EVENT_FACTORY_RESET_DONE                = 0x0910,
    EVENT_REPORT_PACKAGE_ID                 = 0x0911,
#if F_APP_DEVICE_CMD_SUPPORT
    EVENT_BT_READY                          = 0x0920,
    EVENT_REPORT_PIN_CODE                   = 0x0921,
    EVENT_GET_LOCAL_DEV_STATE               = 0x0922,
    EVENT_INQUIRY_STATE                     = 0x0923,
    EVENT_SERVICES_SEARCH_STATE             = 0x0924,
    EVENT_PAIRING_REQUEST                   = 0x0925,
    EVENT_REPORT_SSP_NUMERIC_VALUE          = 0x0926,
    EVENT_LEGACY_RSSI                       = 0x0927,
    EVENT_REPORT_CONNECTED_DEV_ID           = 0x0928,
    EVENT_INQUIRY_RESULT                    = 0x0929,
    EVENT_STABLE_STATE                      = 0x092A,
    EVENT_REPORT_REMOTE_DEV_ATTR_INFO       = 0x092B,
#endif

    //for customize
    EVENT_JSA_CAL_RESULT                    = 0x0A02,
    EVENT_SITRON_CUSTOMIZED_FEATURE_REPORT  = 0x0A06,
    EVENT_MIC_MP_VERIFY_BY_HFP              = 0x0A07,
    EVENT_GET_DSP_CONFIG_GAIN               = 0x0A08,
    EVENT_CUSTOMIZED_TOZO_FEATURE           = 0x0A09,
    EVENT_RSV                               = 0x0A0A,
    EVENT_IO_PIN_PULL_HIGH                  = 0x0A0B,
    EVENT_ENTER_BAT_OFF_MODE                = 0x0A0C,

    //for HCI command
    EVENT_ANC_VENDOR_REPORT                 = 0x0B00,
    EVENT_WDG_RESET                         = 0x0B01,
    EVENT_DUAL_MIC_MP_VERIFY                = 0x0B02,

    EVENT_SOUND_PRESS_CALIBRATION           = 0x0B10,
    EVENT_CAP_TOUCH_CTL                     = 0x0B11,
    EVENT_REPORT_SPACE_SENSOR_DATA          = 0x0B12,

    // ANC
    EVENT_ANC_TEST_MODE                     = 0x0C00,
    EVENT_ANC_WRITE_GAIN,
    EVENT_ANC_READ_GAIN,
    EVENT_ANC_BURN_GAIN,
    EVENT_ANC_COMPARE,
    EVENT_ANC_GEN_TONE,
    EVENT_ANC_CONFIG_DATA_LOG,
    EVENT_ANC_READ_DATA_LOG,
    EVENT_ANC_READ_MIC_CONFIG,
    EVENT_ANC_READ_SPEAKER_CHANNEL,
    EVENT_ANC_READ_REGISTER,                //0x0C0A
    EVENT_ANC_WRITE_REGISTER,
    EVENT_ANC_LLAPT_WRITE_GAIN              = 0x0C0C,
    EVENT_ANC_LLAPT_READ_GAIN               = 0x0C0D,
    EVENT_ANC_LLAPT_BURN_GAIN               = 0x0C0E,
    EVENT_ANC_LLAPT_FEATURE_CONTROL         = 0x0C0F,

    EVENT_ANC_QUERY                         = 0x0C20,
    EVENT_ANC_ENABLE_DISABLE                = 0x0C21,
    EVENT_LLAPT_QUERY                       = 0x0C22,
    EVENT_LLAPT_ENABLE_DISABLE              = 0x0C23,

    EVENT_RAMP_GET_INFO                     = 0x0C26,
    EVENT_RAMP_BURN_PARA_START              = 0x0C27,
    EVENT_RAMP_BURN_PARA_CONTINUE           = 0x0C28,
    EVENT_RAMP_BURN_GRP_INFO                = 0x0C29,
    EVENT_RAMP_MULTI_DEVICE_APPLY           = 0x0C2A,

    EVENT_LISTENING_MODE_CYCLE_SET          = 0x0C2B,
    EVENT_LISTENING_MODE_CYCLE_GET          = 0x0C2C,
    EVENT_CUSTOMIZE_SENSOR_ACK              = 0x0C2D,

    EVENT_APT_VOLUME_INFO                   = 0x0C2E,
    EVENT_APT_VOLUME_SET                    = 0x0C2F,
    EVENT_APT_VOLUME_STATUS                 = 0x0C30,
    EVENT_LLAPT_BRIGHTNESS_INFO             = 0x0C31,
    EVENT_LLAPT_BRIGHTNESS_SET              = 0x0C32,
    EVENT_LLAPT_BRIGHTNESS_STATUS           = 0x0C33,
    EVENT_LLAPT_SCENARIO_CHOOSE_INFO        = 0x0C36,
    EVENT_LLAPT_SCENARIO_CHOOSE_TRY         = 0x0C37,
    EVENT_LLAPT_SCENARIO_CHOOSE_RESULT      = 0x0C38,
    EVENT_APT_POWER_ON_DELAY_TIME           = 0x0C39,
    EVENT_LISTENING_STATE_SET               = 0x0C3B,
    EVENT_LISTENING_STATE_STATUS            = 0x0C3C,
    // ADSP PARA
    EVENT_ANC_GET_ADSP_INFO                 = 0x0C40,
    EVENT_ANC_SEND_ADSP_PARA_START          = 0x0C41,
    EVENT_ANC_SEND_ADSP_PARA_CONTINUE       = 0x0C42,
    EVENT_ANC_ENABLE_DISABLE_ADAPTIVE_ANC   = 0x0C43,

    EVENT_ANC_SCENARIO_CHOOSE_INFO          = 0x0C44,
    EVENT_ANC_SCENARIO_CHOOSE_TRY           = 0x0C45,
    EVENT_ANC_SCENARIO_CHOOSE_RESULT        = 0x0C46,

    // OTA Tooling
    EVENT_OTA_TOOLING_PARKING               = 0x0D00,

    EVENT_LOW_LATENCY_STATUS = 0x0E00,      //0x0E00
    EVENT_EAR_DETECTION_STATUS = 0x0E01,    //0x0E01
    EVENT_LOW_LATENCY_LEVEL_SET             = 0x0E02,

    EVENT_MP_TEST_RESULT                    = 0x0F00,
    EVENT_FORCE_ENGAGE                      = 0x0F01,
    EVENT_AGING_TEST_CONTROL                = 0x0F02,

#if (F_APP_HEARABLE_SUPPORT == 1)
    EVENT_HA_VER_INFO                       = 0x2000,
    EVENT_HA_EFFECT_INDEX_INFO              = 0x2001,
    EVENT_HA_ON_OFF_INFO                    = 0x2002,
    EVENT_HA_TOOL_EXTEND_INFO               = 0x2003,
    EVENT_HA_PROGRAM_ID_INFO                = 0x2004,
    EVENT_HA_PROGRAM_NUM_INFO               = 0x2005,
    EVENT_HA_PROGRAM_NAME_INFO              = 0x2006,
#endif

#if F_APP_DURIAN_SUPPORT
    EVENT_AVP_BUD_LOCAL                     = 0x2100,
    EVENT_AVP_ANC_SET                       = 0x2101,
#endif

#if F_APP_SS_SUPPORT
    EVENT_SS_RESP                           = 0x2200,
#endif

    EVENT_GCSS_REG                  = 0x3000,
    EVENT_GCSS_DATA_TRANSFER        = 0x3001,
    EVENT_GCSS_CON_NOTIFY           = 0x3002,
    EVENT_GCSS_INFO                 = 0x3003,
    EVENT_GCSS_READ_REQ             = 0x3004,
    EVENT_GCSS_IND_ACK              = 0x3005,
    EVENT_GCSS_SERV_CHG             = 0x3006,
    EVENT_GCSS_NOTI_CREDIT_UPD      = 0x3007,

    EVENT_GCSC_DISCOVER_STATE                = 0x3050,
    EVENT_GCSC_DISCOVER_RES                  = 0x3051,
    EVENT_GCSC_READ_RES                      = 0x3052,
    EVENT_GCSC_WRITE_RES                     = 0x3053,
    EVENT_GCSC_NOTI_IND                      = 0x3054,
    EVENT_GCSC_DISC                          = 0x3055,
    EVENT_GCSC_DISCOVER_ALL_END              = 0x3056,

    EVENT_XM_SPP_DATA_TRANSFER              = 0x8002,
    EVENT_XM_SPP_CONNECT_STATUS             = 0x8003,
    EVENT_XM_SPP_DISCONNECT_STATUS          = 0x8004,
    EVENT_XM_BR_ATT_CONNECTED               = 0x8005,
    EVENT_XM_BR_ATT_DISCONNECTED            = 0x8006,
    EVENT_XM_GATT_DATA_TRANSFER             = 0x8007,
    EVENT_XM_FACTORY_RESET_STATUS           = 0x8008,
    EVENT_XM_GATT_CREDIT_UPDATE             = 0x8009,
#if (F_APP_WATCH_CUSTOMER_CONN_MANAGER_SUPPORT == 1)
    EVENT_XM_ALL_CONNECT_INFO               = 0x8010,
#endif
    EVENT_XM_BT_SDP_RECORD_IND              = 0x8011,
    EVENT_XM_BT_SDP_RECORD_CMPL             = 0x8012,
    EVENT_XM_BT_SDP_RECORD_DISCONN          = 0x8013,
    EVENT_XM_BT_SDP_RECORD_DATA_IND         = 0x8014,
    EVENT_XM_BT_SDP_RFC_MSG_CREDIT_INFO     = 0x8015,
    EVENT_XM_BT_SDP_RFC_SEND_ERROR          = 0x8016,
    EVENT_XM_LE_ADV_STATE                   = 0x8100,
    EVENT_XM_LE_ATT_MTU                     = 0x8101,
    EVENT_XM_LE_ADDR                        = 0x8102,
    EVENT_XM_LE_CON_STATE                   = 0x8103,
    EVENT_XM_LE_CON_PARAM                   = 0x8104,
    EVENT_XM_GATT_CON_NOTIFY                = 0x8105,
    EVENT_XM_LE_USER_CONFIRMATION_REQ       = 0x8106,
    EVENT_XM_LE_LINK_KEY                    = 0x8107,
    EVENT_XM_LE_ADV_CREATED                 = 0x8108,
    EVENT_XM_GET_PHY                        = 0x8109,
    EVENT_XM_MUSIC_REQ_FRAME                = 0x8500,
    EVENT_XM_TTS_REQ_FRAME                  = 0x8501,
    EVENT_XM_RECORDING_DATA                 = 0x8502,
    EVENT_XM_REOCRD_SP_STOP                 = 0x8503,
    EVENT_XM_RECORD_PLAY_REQ_FRAME          = 0x8504,
    EVENT_XM_AUDIO_PLAY_TIME                = 0x8505,
    EVENT_XM_AUDIO_TEST_PA_ID               = 0x8600,
    EVENT_XM_BT_AVRCP_STATUS                = 0x8700,
    EVENT_XM_AVRCP_ELEM_ATTRS_GET           = 0x8701,
    EVENT_XM_BT_AVRCP_GET_ABS_SUPPORTED     = 0x8702,
    EVENT_XM_BT_ACL_SNIFF_STATUS            = 0x8703,
    EVENT_AVRCP_ABSOLUTE_VOLUME_SET_RSP     = 0x8704,
    EVENT_XM_USER_CONFIRMATION_REQ          = 0x8800,
    EVENT_XM_CRASH_DUMP                     = 0x8F03,
    EVENT_TOTAL
} T_EVENT_ID;

//for EVENT_CALLER_ID
#define CALLER_ID_TYPE_NUMBER           0x00
#define CALLER_ID_TYPE_NAME             0x01

/**
    * @brief  report app event by spp/le/uart
    * @param  cmd_path br/le/uart
    * @param  event_id event id
    * @param  app_index BR/LE link index
    * @param  data event data
    * @param  len  event len
    * @return void
    */
void app_report_event(uint8_t cmd_path, uint16_t event_id, uint8_t app_index, uint8_t *data,
                      uint16_t len);

/**
    * @brief  broadcast event to all path and all devices: UART, br link and le link.
    * @param  event_id @ref T_EVENT_ID
    * @param  buf content in the event packet
    * @param  len buf length.
    * @return void
    */
void app_report_event_broadcast(uint16_t event_id, uint8_t *buf, uint16_t len);

/**
    * @brief  report raw data by spp/le/uart
    * @param  cmd_path br/le/uart
    * @param  app_index BR/LE link index
    * @param  data event data
    * @param  len  event len
    * @return void
    */
void app_report_raw_data(uint8_t cmd_path, uint8_t app_index, uint8_t *data,
                         uint16_t len);

/**
    * @brief  report rws state to phone
    * @param  none
    * @return void
    */
void app_report_rws_state(void);

/**
    * @brief  report rws bud side to phone
    * @param  none
    * @return void
    */
void app_report_rws_bud_side(void);

#if F_APP_APT_SUPPORT
/**
    * @brief  report apt eq index to phone
    * @param  apt_eq_data_update_event
    * @return void
    */
void app_report_apt_eq_idx(T_APT_EQ_DATA_UPDATE_EVENT apt_eq_data_update_event);
#endif

/**
    * @brief  report rws bud info to phone
    * @param  none
    * @return void
    */
void app_report_rws_bud_info(void);
void app_report_get_bud_info(uint8_t *data);

/** End of APP_REPORT
* @}
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _APP_REPORT_H_ */
