/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      datebse.h
* @brief
* @details
* @author
* @date
* @version
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/


/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "dp_wearable.h"
#include "dp_sports_refs.h"
#include "dp_notification.h"
#include <stdlib.h>
#include <flashdb.h>

/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      datebse.h
* @brief
* @details
* @author
* @date
* @version
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/


/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdlib.h>
#include "dp_wearable.h"
#include "dp_sports_refs.h"
#include <flashdb.h>
#include "tuya_ble_log.h"


void dp_database_init(void)
{
    dp_tuya_cfg_flashDB_init();
    dp_tuya_panelinfo_flashDB_init();
    dp_sports_refs_flashDB_init();
    sport_min_data_flashDB_init();

    notification_data_manage_init();
}

