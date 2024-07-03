/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file       rtl876x_sdio.c
* @brief     This file provides all the SDIO firmware functions.
* @details
* @author   elliot chen
* @date      2017-01-03
* @version  v1.0
*********************************************************************************************************
*/

/* Includes -----------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_sdio.h"
#include "indirect_access.h"

/* Internal defines -----------------------------------------------------------*/
#define SDIO_REG_TEST_MODE          *((volatile uint32_t *)0x400002B4UL)
#define SDIO_REG_PULL_MODE          *((volatile uint32_t *)0x400002ACUL)
#define REG0X_SDH_MODE              0x732

/**
  * @brief  Select the SDIO output pin group.
  * @param  SDIO_PinGroupType:
    This parameter can be one of the following values:
  *  @arg SDIO_PinGroup_0
  *  @arg SDIO_PinGroup_1
  * @retval None
  */
void SDIO_PinGroupConfig(uint32_t SDIO_PinGroupType)
{
    /* Check the parameters */
    assert_param(IS_SDIO_PINGROUP_IDX(SDIO_PinGroupType));

#if defined(CONFIG_SOC_SERIES_RTL8773E) || defined(IC_TYPE_BB2U)

    /*0x732[0:0]SDH source selection 0: P_GPIO5 1: P_GPIO6, [6:1] SDH input port enable in HS mode 0: disable 1: enable*/
    if (SDIO_PinGroupType == SDIO_PinGroup_0)
    {
        btaon_fast_update_safe(REG0X_SDH_MODE, 0x7f, 0x7e);
    }
    else
    {
        btaon_fast_update_safe(REG0X_SDH_MODE, 0x7f, 0x7f);
    }

#else
    if (SDIO_PinGroupType == SDIO_PinGroup_0)
    {
        SDIO_REG_PULL_MODE |= 0x0F << 20;
    }

    /* Dedicated SDIO pin option */
    SDIO_REG_TEST_MODE &= ~(BIT(1));
    SDIO_REG_TEST_MODE |= SDIO_PinGroupType << 1;
#endif
}

/**
  * @brief  Select the SDIO data bus pin.
  * @param  SDIO_BusWide:
    This parameter can be one of the following values:
  *  @arg SDIO_BusWide_1b
  *  @arg SDIO_BusWide_4b
  * @param  NewState: new state of the SDIO data pin.
    This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_DataPinConfig(uint32_t SDIO_BusWide, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_BUS_WIDE(SDIO_BusWide));

    if (NewState != DISABLE)
    {
        if (SDIO_BusWide == SDIO_BusWide_1b)
        {
            SDIO_REG_TEST_MODE |= 0b00000111 << 4;
        }
        else
        {
            SDIO_REG_TEST_MODE |= 0x00000FF0;
        }
    }
    else
    {
        SDIO_REG_TEST_MODE &= ~(0x00000FF0);
    }
}

/**
  * @brief  Enables or disables the SDIO output pin.
  * @param  NewState: new state of the SDIO output pin.
    This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_PinOutputCmd(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        /* Enable SDIO pin output */
        SDIO_REG_TEST_MODE |= BIT(0);
    }
    else
    {
        /* Disable SDIO pin output */
        SDIO_REG_TEST_MODE &= ~(BIT(0));
    }
}

/**
 * @brief  Software reset host controller.
 * @clock  none.
 * @return none.
 */
void SDIO_SoftwareReset(void)
{
    /* Software reset for all */
    SDIO->SW_RESET |= SDIO_SF_RESET_MASK;
}

/**
 * @brief  Get Software reset status.
 * @clock  none.
 * @return Software reset status: SET: reset in progress, RESET: can work.
 */
FlagStatus SDIO_GetSoftwareResetStatus(void)
{
    FlagStatus bitstatus = RESET;

    if ((SDIO->SW_RESET & SDIO_SF_RESET_MASK) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * @brief  Enables or disables the SDIO internal Clock.
  * @param  NewState: new state of the SDIO internal Clock.
    This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_InternalClockCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable internal clock */
        SDIO->CLK_CTRL |= SDIO_INTERNAL_CLOCK_MASK;
    }
    else
    {
        /* Disable internal clock */
        SDIO->CLK_CTRL &= SDIO_INTERNAL_CLOCK_CLR;
    }
}

/**
 * @brief  Get internal clock stable or not.
 * @clock  none.
 * @return Software reset status: SET: ready, RESET: not ready.
 */
FlagStatus SDIO_GetInternalClockStatus(void)
{
    FlagStatus bitstatus = RESET;

    if ((SDIO->CLK_CTRL & SDIO_INTERNAL_CLK_STABLE_MASK) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * @brief Initializes the SDIO peripheral according to the specified
  *   parameters in the SDIO_InitStruct.
  * @param  SDIO_InitStruct: pointer to a SDIO_InitTypeDef structure that
  *   contains the configuration information for the specified SDIO peripheral.
  * @retval None
  */
void SDIO_Init(SDIO_InitTypeDef *SDIO_InitStruct)
{
    /* Enable SDIO host card detect circuit */
    //*((volatile uint32_t *)0x4005d000) |= BIT(10);

    /* Select 32-bit ADMA2 */
    if (SDIO->CAPABILITIES_L & BIT(19))
    {
        SDIO->HOST_CTRL &= ~(0x03 << 3);
        SDIO->HOST_CTRL |= 0x02 << 3;
    }

    /*Configure SDCLK frequency */
    SDIO->CLK_CTRL &= ~(BIT(2));
    //SDIO->CLK_CTRL &= 0xff;
    SDIO->CLK_CTRL &= ~(0xFF00);
    SDIO->CLK_CTRL |= SDIO_InitStruct->SDIO_ClockDiv;
    SDIO->CLK_CTRL |= BIT(2);

    /* Configure SDIO bus width */
    SDIO->HOST_CTRL &=  ~(BIT(1));
    SDIO->HOST_CTRL |=  SDIO_InitStruct->SDIO_BusWide;

    /* Configure DAT line timeouts */
    SDIO->TIMEOUT_CTRL = SDIO_InitStruct->SDIO_TimeOut;
}

/**
  * @brief  Fills each SDIO_InitStruct member with its default value.
  * @param  SDIO_InitStruct: pointer to an SDIO_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SDIO_StructInit(SDIO_InitTypeDef *SDIO_InitStruct)
{
    /* time out = TMCLK * 2^(SDIO_TimeOut+13). 0<= SDIO_TimeOut <= 0x0E. TMCLK: the base clock. */
    SDIO_InitStruct->SDIO_TimeOut = 0x0E;
    SDIO_InitStruct->SDIO_BusWide = SDIO_BusWide_1b;
    SDIO_InitStruct->SDIO_ClockDiv = SDIO_CLOCK_DIV_256;
}

/**
  * @brief  Select bus delay through the SDIO peripheral.
  * @param  bus delay time(unit ns).
  * @retval none.
  */
void SDIO_BusDelaySel(uint32_t SDIO_BusWide, uint8_t SDIO_BusDelay)
{
    SDIO->BUS_DLY_SEL &= ~(0xFFFFFF);

    SDIO->BUS_DLY_SEL |= (SDIO_BusDelay) | (SDIO_BusDelay << 4) | (SDIO_BusDelay << 8);

    if (SDIO_BusWide == SDIO_BusWide_4b)
    {
        SDIO->BUS_DLY_SEL |= (SDIO_BusDelay << 12) | (SDIO_BusDelay << 16) | (SDIO_BusDelay << 20);
    }
}

/**
  * @brief  Sets the SD bus power status of the controller.
  * @param  SDIO_PowerState: new state of the Power state.
  *   This parameter can be one of the following values:
  *   - SDIO_PowerState_OFF
  *   - SDIO_PowerState_ON
  * @retval None
  */
void SDIO_SetBusPower(uint32_t SDIO_PowerState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_POWER_STATE(SDIO_PowerState));

    /* SD bus power off */
    SDIO->PWR_CTRL &= ~(BIT(0));
    SDIO->PWR_CTRL = SDIO_VOLT_3_3V;
    /* SD bus power on */
    SDIO->PWR_CTRL |= SDIO_PowerState;
}

/**
  * @brief  Initializes the SDIO data path according to the specified
*      parameters in the SDIO_DataInitStruct.
  * @param  SDIO_DataInitStruct : pointer to a SDIO_DataInitTypeDef
  *   structure that contains the configuration information for the SDIO command.
  * @retval None
  */
void SDIO_DataConfig(SDIO_DataInitTypeDef *SDIO_DataInitStruct)
{
    /* Check the parameters */
    assert_param(IS_SDIO_BLOCK_COUNT(SDIO_DataInitStruct->SDIO_BlockCount));
    assert_param(IS_SDIO_BLOCK_SIZE(SDIO_DataInitStruct->SDIO_BlockSize));
    assert_param(IS_SDIO_TRANSFER_DIR(SDIO_DataInitStruct->SDIO_TransferDir));

    if (SDIO_DataInitStruct->SDIO_DMAEn == ENABLE)
    {
        /* Set start address of the descriptor table */
        SDIO->ADMA_SYS_ADDR_L = SDIO_DataInitStruct->SDIO_Address;
    }

    /* Set block size */
    SDIO->BLK_SIZE = SDIO_DataInitStruct->SDIO_BlockSize;
    /* Set block count */
    SDIO->BLK_CNT = SDIO_DataInitStruct->SDIO_BlockCount;

    if (SDIO_DataInitStruct->SDIO_BlockCount == 1)
    {
        /* Single block */
        SDIO->TF_MODE = SDIO_DataInitStruct->SDIO_TransferDir | \
                        SDIO_DataInitStruct->SDIO_DMAEn;
    }
    else
    {
        /* Multiple block */
        SDIO->TF_MODE = SDIO_MULTI_BLOCK_EN | \
                        SDIO_DataInitStruct->SDIO_TransferDir | \
                        SDIO_AUTO_CMD12_EN |  SDIO_BLOCK_COUNT_EN | \
                        SDIO_DataInitStruct->SDIO_DMAEn;
    }
}

/**
  * @brief  Fills each SDIO_DataInitStruct member with its default value.
  * @param  SDIO_DataInitStruct: pointer to an SDIO_DataInitTypeDef structure which
  *         will be initialized.
  * @retval None
*/
void SDIO_DataStructInit(SDIO_DataInitTypeDef *SDIO_DataInitStruct)
{
    /* SDIO_DataInitStruct members default value */
    SDIO_DataInitStruct->SDIO_Address       = 0;
    SDIO_DataInitStruct->SDIO_BlockCount    = 0x00;
    SDIO_DataInitStruct->SDIO_BlockSize     = 0x00;
    SDIO_DataInitStruct->SDIO_TransferDir   = SDIO_TransferDir_WRITE;
    SDIO_DataInitStruct->SDIO_DMAEn         = ENABLE;
}

/**
 * @brief  Configure SD clock.
 * @param SDIO_ClockDiv: value of SDIO clock divider.
 * @return none.
 */
void SDIO_SetClock(uint32_t clock_div)
{
    /* Check the parameters */
    assert_param(IS_SDIO_CLOCK_DIV(clock_div));

    /* Disable SDIO clock */
    SDIO->CLK_CTRL &= ~(BIT(2));
    /* Configure SDIO clock divider */
    SDIO->CLK_CTRL &= ~(0xFF00);
    SDIO->CLK_CTRL |= clock_div;
    /* Enable SDIO clock */
    SDIO->CLK_CTRL |= BIT(2);
}

/**
 * @brief  Configure SDIO clock.
 * @param SDIO_BusWide: value of SDIO bus wide.
 *   This parameter can be the following values:
 *   @arg SDIO_BusWide_1b: 1-bit mode.
 *   @arg SDIO_BusWide_4b: 4-bit mode.
 * @return none.
 */
void SDIO_SetBusWide(uint32_t SDIO_BusWide)
{
    /* Check the parameters */
    assert_param(IS_SDIO_BUS_WIDE(SDIO_BusWide));

    SDIO->HOST_CTRL &= ~(BIT(1));
    SDIO->HOST_CTRL |= SDIO_BusWide;
}

/**
 * @brief  Set start address of the descriptor table.
 * @address: start address of the descriptor table.
 * @return none.
 */
void SDIO_SetSystemAddress(uint32_t address)
{
    SDIO->ADMA_SYS_ADDR_L = address;
}

/**
 * @brief  Set block size.
 * @BlockSize:  block size.
 * @return none.
 */
void SDIO_SetBlockSize(uint32_t BlockSize)
{
    SDIO->BLK_SIZE = BlockSize;
}

/**
 * @brief  Set block count.
 * @BlockCount:  block count.
 * @return none.
 */
void SDIO_SetBlockCount(uint32_t BlockCount)
{
    SDIO->BLK_CNT = BlockCount;
}

/**
  * @brief  Initializes the SDIO Command according to the specified
  *         parameters in the SDIO_CmdInitStruct and send the command.
  * @param  SDIO_CmdInitStruct : pointer to a SDIO_CmdInitTypeDef
  *         structure that contains the configuration information for the SDIO command.
  * @retval None
  */
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
    /* Check the parameters */
    assert_param(IS_SDIO_CMD_INDEX(SDIO_CmdInitStruct->SDIO_CmdIndex));

    /* Set the SDIO Argument value */
    SDIO->ARG = SDIO_CmdInitStruct->SDIO_Argument;
    /* SDIO CMD Configuration */
    SDIO->CMD = (SDIO_CmdInitStruct->SDIO_CmdIndex << 8)     | \
                (SDIO_CmdInitStruct->SDIO_CmdType << 6)      | \
                (SDIO_CmdInitStruct->SDIO_DataPresent << 5)  | \
                (SDIO_CmdInitStruct->SDIO_CmdIdxCheck << 4)  | \
                (SDIO_CmdInitStruct->SDIO_CmdCrcCheck << 3)  | \
                (SDIO_CmdInitStruct->SDIO_ResponseType);
}

/**
  * @brief  Fills each SDIO_CmdInitStruct member with its default value.
  * @param  SDIO_CmdInitStruct: pointer to an SDIO_CmdInitTypeDef structure which
  *         will be initialized.
  * @retval None
*/
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct)
{
    /* SDIO_CmdInitStruct members default value */
    SDIO_CmdInitStruct->SDIO_Argument       = 0;
    SDIO_CmdInitStruct->SDIO_CmdIndex       = 0;
    SDIO_CmdInitStruct->SDIO_CmdType        = NORMAL;
    SDIO_CmdInitStruct->SDIO_DataPresent    = NO_DATA;
    SDIO_CmdInitStruct->SDIO_CmdIdxCheck    = 0;
    SDIO_CmdInitStruct->SDIO_CmdCrcCheck    = 0;
    SDIO_CmdInitStruct->SDIO_ResponseType   = SDIO_Response_No;
}

/**
  * @brief  Read data through the SDIO peripheral.
  * @param  None.
  * @retval  The value of the received data.
  */
uint32_t SDIO_ReadData(void)
{
    return SDIO->BUF_DATA_PORT;
}

/**
  * @brief  Write data through the SDIO peripheral.
  * @param  Data: Data to be transmitted.
  * @retval  none.
  */
void SDIO_WriteData(uint32_t Data)
{
    SDIO->BUF_DATA_PORT = Data;
}

/**
  * @brief  Enables or disables the specified SDIO interrupt status.
  * @param  SDIO_INTStatus: specifies the IR interrupt status to be enabled or disabled.
  *   This parameter can be the following values:
  *      @arg SDIO_INT_CARD: card interrupt status.
  *      @arg SDIO_INT_DMA: DMA interrupt status.
  *      @arg SDIO_INT_TF_CMPL: transfer complete interrupt status.
  *      @arg SDIO_INT_CMD_CMPL: command complete interrupt status.
  * @param  NewState: new state of the specified SDIO interrupt status.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_INTStatusConfig(uint32_t SDIO_INTStatus, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_INT(SDIO_INTStatus));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* Enable the selected SDIO interrupt status */
        SDIO->NORMAL_INTR_SR_EN |= SDIO_INTStatus;
    }
    else
    {
        /* Disable the selected SDIO interrupt status */
        SDIO->NORMAL_INTR_SR_EN &= ~SDIO_INTStatus;
    }
}

/**
  * @brief  Enables or disables the specified SDIO interrupts.
  * @param  SDIO_INT: specifies the IR interrupts sources to be enabled or disabled.
  *   This parameter can be the following values:
  *      @arg SDIO_INT_ERROR: Error interrupt signal caused by any type error in error interrupt status register
  *    which can trigger interrupt to NVIC.
  *      @arg SDIO_INT_CARD: card interrupt signal which can trigger interrupt to NVIC.
  *      @arg SDIO_INT_DMA: DMA interrupt signal which can trigger interrupt to NVIC.
  *      @arg SDIO_INT_TF_CMPL: transfer interrupt interrupt signal which can trigger interrupt to NVIC.
  *      @arg SDIO_INT_CMD_CMPL: command complete signal interrupt which can trigger interrupt to NVIC.
  * @param  NewState: new state of the specified SDIO interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_INTConfig(uint32_t SDIO_INT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_INT(SDIO_INT));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* Enable the selected SDIO interrupt */
        SDIO->NORMAL_INTR_SIG_EN |= SDIO_INT;
    }
    else
    {
        /* Disable the selected SDIO interrupt */
        SDIO->NORMAL_INTR_SIG_EN &= ~SDIO_INT;
    }
}

/**
  * @brief  Enables or disables the specified SDIO error interrupt status.
  * @param  SDIO_INTStatus: specifies the SDIO error interrupts status to be enabled or disabled.
  *   This parameter can be the following values:
  *    @arg  SDIO_INT_VENDOR_SPECIFIC_ERR:
  *    @arg  SDIO_INT_ADMA_ERR: Set when the host controller detects errors during ADMA based data transfer.
  *    @arg  SDIO_INT_AUTO_CMD12_ERR: Set when detecting that one of this bits D00-D04 in Auto CMD12 Error Status register
  *    has changed from 0 to 1.
  *      @arg SDIO_INT_CURRENT_LIMIT_ERR:
  *      @arg SDIO_INT_DAT_END_BIT_ERR: Set when detecting 0 at the end bit position of read data which uses the DAT line or at
  *    the end bit position of the CRC Status.
  *      @arg SDIO_INT_DAT_CRC_ERR:
  *      @arg SDIO_INT_DAT_TIMEOUT_ERR:
  *      @arg SDIO_INT_CMD_INDEX_ERR:
  *      @arg SDIO_INT_CMD_END_BIT_ERR:
  *      @arg SDIO_INT_CMD_CRC_ERR:
  *      @arg SDIO_INT_CMD_TIMEOUT_ERR:
  * @param  NewState: new state of the specified SDIO error interrupt status.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_ErrrorINTStatusConfig(uint32_t SDIO_INTStatus, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_ERR_INT(SDIO_INTStatus));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* Enable the selected SDIO error interrupt status */
        SDIO->ERR_INTR_SR_EN |= SDIO_INTStatus;
    }
    else
    {
        /* Disable the selected SDIO error interrupt status */
        SDIO->ERR_INTR_SR_EN &= ~SDIO_INTStatus;
    }
}

/**
  * @brief  Enables or disables the specified SDIO error interrupts.
  * @param  SDIO_INT: specifies the SDIO error interrupts sources to be enabled or disabled.
  *   This parameter can be the following values:
  *    @arg  SDIO_INT_VENDOR_SPECIFIC_ERR:
  *    @arg  SDIO_INT_ADMA_ERR:
  *    @arg  SDIO_INT_AUTO_CMD12_ERR:
  *      @arg SDIO_INT_CURRENT_LIMIT_ERR:
  *      @arg SDIO_INT_DAT_END_BIT_ERR:
  *      @arg SDIO_INT_DAT_CRC_ERR:
  *      @arg SDIO_INT_DAT_TIMEOUT_ERR:
  *      @arg SDIO_INT_CMD_INDEX_ERR:
  *      @arg SDIO_INT_CMD_END_BIT_ERR:
  *      @arg SDIO_INT_CMD_CRC_ERR:
  *      @arg SDIO_INT_CMD_TIMEOUT_ERR:
  * @param  NewState: new state of the specified SDIO error interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_ErrrorINTConfig(uint32_t SDIO_INT, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_ERR_INT(SDIO_INT));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* Enable the selected SDIO interrupt */
        SDIO->ERR_INTR_SIG_EN |= SDIO_INT;
    }
    else
    {
        /* Disable the selected SDIO error interrupt */
        SDIO->ERR_INTR_SIG_EN &= ~SDIO_INT;
    }
}

/**
  * @brief  Enables or disables the specified SDIO wake up event.
  * @param  SDIO_WakeUp: specifies the SDIO wake up sources to be enabled or disabled.
  *   This parameter can be the following values:
  *      @arg SDIO_WAKE_UP_SDCARD_REMOVAL:
  *      @arg SDIO_WAKE_UP_SDCARD_INSERT:
  *      @arg SDIO_WAKE_UP_SDCARD_INTR:
  * @param  NewState: new state of the specified SDIO wake up event.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SDIO_WakeUpConfig(uint32_t SDIO_WakeUp, FunctionalState newState)
{
    /* Check the parameters */
    assert_param(IS_SDIO_WAKE_UP(SDIO_WakeUp));
    assert_param(IS_FUNCTIONAL_STATE(newState));

    if (newState == ENABLE)
    {
        /* Enable the selected SDIO wake up event */
        SDIO->WAKEUP_CTRL |= SDIO_WakeUp;
    }
    else
    {
        /* Disable the selected SDIO wake up event */
        SDIO->WAKEUP_CTRL &= ~SDIO_WakeUp;
    }
}

/**
 * @brief  Checks whether the specified SDIO interrupt is set or not.
 * @param  SDIO_FLAG: specifies the interrupt to check.
 *   This parameter can be one of the following values:
 *       @arg SDIO_INT_ERROR: Set when any of bits in the Error interrupt status register are set.
 *       @arg SDIO_INT_CARD: Set when generate card interrupt.
 *       @arg SDIO_INT_DMA: In case of ADMA, by setting int field in the descriptor table, host controller generates this interrupt.
 *       @arg SDIO_BLOCK_GAP_EVENT: Set when stop at Block Gap Request is set.
 *       @arg SDIO_INT_TF_CMPL: Set when a read/write transfer and a command with busy is complete.
 *     @arg SDIO_INT_CMD_CMPL: Set when get the end bit of command response(Except Auto CMD12).
 * @retval The new state of ITStatus (SET or RESET).
 */
ITStatus SDIO_GetINTStatus(uint32_t SDIO_INT)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SDIO_INT(SDIO_INT));

    if ((SDIO->NORMAL_INTR_SR & SDIO_INT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
 * @brief  Checks whether the specified SDIO error interrupt is set or not.
 * @param  SDIO_INT: specifies the error interrupt to check.
 *   This parameter can be one of the following values:
 *     @arg SDIO_INT_VENDOR_SPECIFIC_ERR:
 *     @arg SDIO_INT_ADMA_ERR:
 *     @arg SDIO_INT_AUTO_CMD12_ERR:
 *   @arg SDIO_INT_CURRENT_LIMIT_ERR:
 *   @arg SDIO_INT_DAT_END_BIT_ERR:
 *   @arg SDIO_INT_DAT_CRC_ERR:
 *   @arg SDIO_INT_DAT_TIMEOUT_ERR:
 *   @arg SDIO_INT_CMD_INDEX_ERR:
 *   @arg SDIO_INT_CMD_END_BIT_ERR:
 *   @arg SDIO_INT_CMD_CRC_ERR:
 *   @arg SDIO_INT_CMD_TIMEOUT_ERR:
 * @retval The new state of ITStatus (SET or RESET).
 */
ITStatus SDIO_GetErrorINTStatus(uint32_t SDIO_INT)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SDIO_ERR_INT(SDIO_INT));

    if ((SDIO->ERR_INTR_SR & SDIO_INT) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
 * @brief  Checks whether the specified SDIO flag is set or not.
 * @param  SDIO_FLAG: specifies the flag to check.
 *   This parameter can be one of the following values:
 *       @arg : SDIO_FLAG_BUF_READ_EN:This status is used for non-DMA read transfer.
 *      -SET: Have data in buffer when block data is ready in the buffer.
 *      -RESET:  No Data.
 *     Supplement: A change of this bit from 0 to 1 occurs when block data is ready
 *      in the buffer and generates the Buffer Read Interrupt.
 *       @arg : SDIO_FLAG_WRITE_BUF_EN:This status is used for non-DMA write transfer.
 *      -SET: can write in non-DMA write transfers.
 *      -RESET:  can not write in non-DMA write transfers.
 *     Supplement: A change of this bit from 0 to 1 occurs when top of block data can be written to the buffer
 *      and generates the Buffer Write Ready Interrupt.
 *       @arg : SDIO_FLAG_READ_TF_ACTIVE:
 *      -SET: Transferring data in a read transfer.
 *      -RESET:  No valid data in a read transfer.
 *       @arg : SDIO_FLAG_WRITE_TF_ACTIVE:
 *      -SET: Transferring data in a write transfer.
 *      -RESET:  No valid data in a write transfer.
 *       @arg : SDIO_FLAG_DAT_LINE_ACTIVE:
 *      -SET: DAT line active
 *      -RESET:  DAT line inactive
 *       @arg : SDIO_FLAG_CMD_DAT_INHIBIT:
 *      -SET: Can not issue command which uses the DAT line
 *      -RESET: Can issue command which uses the DAT line
 *     @arg : SDIO_FLAG_CMD_INHIBIT
 *      -SET: Can not issue command
 *      -RESET: Can issue command using only CMD line
 * @retval The new state of SDIO_FLAG (SET or RESET).
 */
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_SDIO_FLAG(SDIO_FLAG));

    if ((SDIO->PRESENT_STATE & SDIO_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * @brief  Clears the SDIO interrupt pending bits.
  * @param  SDIO_INT: specifies the interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg SDIO_INT_ERROR:
  *     @arg SDIO_INT_CARD:
  *     @arg SDIO_INT_DMA:
  *     @arg SDIO_INT_TF_CMPL:
  *     @arg SDIO_INT_CMD_CMPL:Clear command complete interrupt.
  * @retval None
  */
void SDIO_ClearINTPendingBit(uint32_t SDIO_INT)
{
    /* Check the parameters */
    assert_param(IS_SDIO_INT(SDIO_INT));

    /* Clear the selected SDIO interrupt */
    SDIO->NORMAL_INTR_SR = SDIO_INT;
}

/**
  * @brief  Clears the SDIO error interrupt pending bits.
  * @param SDIO_INT: specifies the error interrupt pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg SDIO_INT_VENDOR_SPECIFIC_ERR:
  *     @arg SDIO_INT_ADMA_ERR:
  *     @arg SDIO_INT_AUTO_CMD12_ERR:
  *   @arg SDIO_INT_CURRENT_LIMIT_ERR:
  *   @arg SDIO_INT_DAT_END_BIT_ERR:
  *   @arg SDIO_INT_DAT_CRC_ERR:
  *   @arg SDIO_INT_DAT_TIMEOUT_ERR:
  *   @arg SDIO_INT_CMD_INDEX_ERR:
  *   @arg SDIO_INT_CMD_END_BIT_ERR:
  *   @arg SDIO_INT_CMD_CRC_ERR:
  *   @arg SDIO_INT_CMD_TIMEOUT_ERR:
  * @retval None
  */
void SDIO_ClearErrorINTPendingBit(uint32_t SDIO_INT)
{
    /* Check the parameters */
    assert_param(IS_SDIO_ERR_INT(SDIO_INT));

    /* Clear the selected SDIO interrupt */
    SDIO->ERR_INTR_SR = SDIO_INT;
}

/**
 * @brief  Returns response received from the card for the last command.
 * @param  SDIO_RESP: Specifies the SDIO response register.
 *   This parameter can be one of the following values:
 *       @arg SDIO_RSP0: Response Register 0~1
 *       @arg SDIO_RSP2: Response Register 2~3
 *       @arg SDIO_RSP4: Response Register 4~5
 *       @arg SDIO_RSP6: Response Register 6~7
 * @retval value of SDIO_RESP.
 */
uint32_t SDIO_GetResponse(uint32_t SDIO_RSP)
{
    /* Check the parameters */
    assert_param(IS_SDIO_RESP(SDIO_RSP));

    return *((volatile uint32_t *)((uint32_t)(&(SDIO->RSP0)) + SDIO_RSP));
}

void RCC_SDIOClockConfig(uint16_t ClockSource, uint16_t ClockDiv)
{
    if ((ClockSource == SDIO_CLOCK_SOURCE_PLL1) || (ClockSource == SDIO_CLOCK_SOURCE_PLL2))
    {
        SYSBLKCTRL->u_214.BITS_214.sdio0_clk_sel0 = ClockSource;
        SYSBLKCTRL->u_214.BITS_214.sdio0_clk_sel1 = 1;
    }
    else
    {
        SYSBLKCTRL->u_214.BITS_214.sdio0_clk_sel1 = 0;
    }

    SYSBLKCTRL->u_214.BITS_214.sdio0_clk_src_en = 1;
    SYSBLKCTRL->u_214.BITS_214.sdio0_clk_div_en = 1;
    SYSBLKCTRL->u_214.BITS_214.sdio0_clk_div = ClockDiv;
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor Corporation *****END OF FILE****/

