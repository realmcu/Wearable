#ifndef         _MFB_API_
#define         _MFB_API_

#include <stdbool.h>

/** @addtogroup MFB MFB
  * @brief MFB function module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/


/** @defgroup MFB_Exported_Types MFB Exported Types
  * @{
  */

/**
  * @brief  MFB callback definition
  */
typedef void (*P_MFB_LEVEL_CHANGE_CBACK)(void);

/** End of group MFB_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup MFB_Exported_Functions MFB Exported Functions
  * @{
  */

/**
    * @brief  mfb_get_level
    * @param  none
    * @return mfb level 1 high 0 low
    */
bool mfb_get_level(void);

/**
    * @brief  Enable MFB interrupt
    * @param  none
    * @return none
    */
void mfb_irq_enable(void);

/**
    * @brief  Disable MFB interrupt
    * @param  none
    * @return none
    */
void mfb_irq_disable(void);

/**
    * @brief  Initialize the MFB for power on key
    * @param  cback: callback function which called in mfb interrupt handler
    * @return none
    */
void mfb_init(P_MFB_LEVEL_CHANGE_CBACK cback);

/** @} */ /* End of group MFB_Exported_Functions */
/** @} */ /* End of group MFB */

#endif
