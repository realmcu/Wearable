/*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     communicate_parse_notify.c
* @brief
* @details
* @author
* @date
* @version  v0.1
*********************************************************************************************************
*/

#include "board.h"
#include "communicate_parse.h"
#include "communicate_parse_notify.h"
#include "communicate_protocol.h"
#include "communicate_task.h"
#include "string.h"
#include "trace.h"


/**
* @brief   resolve notify command received from remote APP
* @param   key: L2 key
* @param   pValue: received value pointer
* @param   length: value length
* @retval  error code
*/
void resolve_Notify_command(uint8_t key, uint8_t *pValue, uint16_t length, uint8_t messaye_type)
{
    APP_PRINT_INFO1("resolve_Notify_command KEY is %d", key);
    APP_PRINT_INFO1("resolve_Notify_command pValue is %s", TRACE_STRING(pValue));
    APP_PRINT_INFO1("resolve_Notify_command pValue is %b", TRACE_BINARY(length, pValue));
    switch (key)
    {
    case KEY_INCOMMING_CALL:

    case KEY_INCOMMING_CALL_ACCEPT:

    case KEY_INCOMMING_CALL_REFUSE:
        {
            break;
        }
    case KEY_INCOMMING_MESSAGE:
        {
            break;
        }
    default:
        break;
    }
}
