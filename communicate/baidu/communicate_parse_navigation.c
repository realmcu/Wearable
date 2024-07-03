/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     communicate_parse.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "communicate_parse.h"
#include "communicate_task.h"
#include "communicate_parse_navigation.h"
#include "string.h"
#include "trace.h"


extern void navigation_information_cb(uint32_t len, void *value);
extern void navigation_type_fresh(uint8_t navi_type);

static P_FUNC_NAVI_CB navi_info_cb = NULL;
static P_FUNC_NAVI_TYPE_CB navi_type_cb = NULL;

void Navi_register_cb(P_FUNC_NAVI_CB cb)
{
    navi_info_cb = cb;
}

void Navi_type_register_cb(P_FUNC_NAVI_TYPE_CB cb)
{
    navi_type_cb = cb;
}

/**
* @brief   resolve health data command received from remote APP
* @param   key: L2 key
* @param   pValue: received value pointer
* @param   length: value length
* @retval  error code
*/
void resolve_Navigation_command(uint8_t key, const uint8_t *pValue, uint16_t length)
{
    switch (key)
    {
    case KEY_NAVIGTOR_DATA:
        {
            APP_PRINT_INFO1("KEY_NAVIGTOR_DATA,length is %d", length);
            APP_PRINT_INFO1("navigator data info is  %s", TRACE_STRING(pValue + 3));
            APP_PRINT_INFO1("navigator data info is  %b", TRACE_BINARY(length, pValue));

        }
        break;
    case KEY_NAVIGTOR_STATE:
        {
            APP_PRINT_INFO1("KEY_NAVIGTOR_STATE,length is %d", length);
            APP_PRINT_INFO1("navigator state info is  %s", TRACE_STRING(pValue + 3));
            APP_PRINT_INFO1("navigator state info is  %b", TRACE_BINARY(length, pValue));

        }
        break;
    default:
        break;
    }
}
