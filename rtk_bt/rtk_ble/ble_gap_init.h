/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _BLE_GAP_INIT_APP__
#define _BLE_GAP_INIT_APP__

#ifdef __cplusplus
extern "C" {
#endif

void app_le_gap_init(void);
void update_ibeacon_adv_data(void);
void update_normal_adv_data(void);

/** End of PERIPH_APP
* @}
*/


#ifdef __cplusplus
}
#endif

#endif

