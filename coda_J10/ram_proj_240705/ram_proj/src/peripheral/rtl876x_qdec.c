/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_qdec.c
* @brief    This file provides all the QDEC firmware functions.
* @details
* @author   colin
* @date     2024-06-20
* @version  v1.0
*********************************************************************************************************
*/
#include "rtl876x_rcc.h"
#include "rtl876x_qdec.h"

/**
  * @brief  Deinitializes the Qdecoder peripheral registers to their default reset values(turn off Qdecoder clock).
  * @param  QDECx: selected Qdecoder peripheral.
  * @retval None
  */
void QDEC_DeInit(QDEC_TypeDef *QDECx)
{
    RCC_PeriphClockCmd(APBPeriph_QDEC, APBPeriph_QDEC_CLOCK, DISABLE);
}

/**
  * @brief Initializes the Qdecoder peripheral according to the specified
  *   parameters in the QDEC_InitStruct
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_InitStruct: pointer to a QDEC_InitStruct structure that
  *     contains the configuration information for the specified Qdecoder peripheral
  * @retval None
  */
void QDEC_Init(QDEC_TypeDef *QDECx, QDEC_InitTypeDef *QDEC_InitStruct)
{
    assert_param(IS_QDEC_PERIPH(QDECx));

    QDEC_DIV_TypeDef qdec_0x00 = {.d32 = QDECx->QDEC_DIV};
#if (QDEC_CLOCK_INIT_USE_KHZ == 1)
    uint16_t scanClockDIV;
    uint16_t scanClock;
    uint16_t debounceClockDIV;
    if (QDEC_InitStruct->ScanClockKHZ >= QDEC_SOURCE_CLOCK)
    {
        scanClock = QDEC_SOURCE_CLOCK;
        scanClockDIV = 0;
    }
    else
    {
        scanClockDIV = QDEC_SOURCE_CLOCK / QDEC_InitStruct->ScanClockKHZ - 1;
        if (scanClockDIV > 0xFFF)
        {
            scanClockDIV = 0xFFF;
            scanClock = QDEC_SOURCE_CLOCK / (scanClockDIV + 1);
        }
        else
        {
            scanClock = QDEC_InitStruct->ScanClockKHZ;
        }
    }
    qdec_0x00.b.scan_div = scanClockDIV;

    if (QDEC_InitStruct->DebonceClockKHZ > scanClock)
    {
        debounceClockDIV = 0;
    }
    else
    {
        debounceClockDIV = scanClock / QDEC_InitStruct->DebonceClockKHZ - 1;
        if (debounceClockDIV > 0xF)
        {
            debounceClockDIV = 0xF;
        }
    }
    qdec_0x00.b.deb_div = debounceClockDIV;
#else
    qdec_0x00.b.scan_div = QDEC_InitStruct->scanClockDiv;
    qdec_0x00.b.deb_div = QDEC_InitStruct->debounceClockDiv;
#endif
    QDECx->QDEC_DIV = qdec_0x00.d32;
    /* QDEC configure */
    if (QDEC_InitStruct->axisConfigX == ENABLE)
    {
        QDEC_CR_X_TypeDef qdec_0x04 = {.d32 = QDECx->QDEC_CR_X};
        qdec_0x04.b.x_axis_en = 0x0;
        qdec_0x04.b.x_fsm_en = 0x1;
        /* Set Manual Initial Phase*/
        if (QDEC_InitStruct->manualLoadInitPhase == ENABLE)
        {
            qdec_0x04.b.manual_set_initial_phase = 0x1;
            qdec_0x04.b.x_initial_phase = QDEC_InitStruct->initPhaseX;
            QDECx->QDEC_CR_X = qdec_0x04.d32;

            qdec_0x04.b.manual_set_initial_phase = 0x0;
            QDECx->QDEC_CR_X = qdec_0x04.d32;
        }
        else
        {
            qdec_0x04.b.manual_set_initial_phase = 0x0;
            QDECx->QDEC_CR_X = qdec_0x04.d32;
        }
        qdec_0x04.b.x_debounce_cnt = QDEC_InitStruct->debounceTimeX;
        qdec_0x04.b.x_cnt_scale = QDEC_InitStruct->counterScaleX;
        qdec_0x04.b.x_debounce_en = QDEC_InitStruct->debounceEnableX;
        qdec_0x04.b.x_cnt_pause = 0;
        QDECx->QDEC_CR_X = qdec_0x04.d32;
        QDECx->QDEC_INT_CLR |= QDEC_CLR_ACC_CT_X;
    }
    if (QDEC_InitStruct->axisConfigY == ENABLE)
    {
        QDEC_CR_Y_TypeDef qdec_0x0C = {.d32 = QDECx->QDEC_CR_Y};
        qdec_0x0C.b.y_axis_en = 0x0;
        qdec_0x0C.b.y_fsm_en = 0x1;
        /* Set Manual Initial Phase*/
        if (QDEC_InitStruct->manualLoadInitPhase == ENABLE)
        {
            qdec_0x0C.b.manual_set_initial_phase = 0x1;
            qdec_0x0C.b.y_initial_phase = QDEC_InitStruct->initPhaseY;
            QDECx->QDEC_CR_Y = qdec_0x0C.d32;

            qdec_0x0C.b.manual_set_initial_phase = 0x0;
            QDECx->QDEC_CR_Y = qdec_0x0C.d32;
        }
        else
        {
            qdec_0x0C.b.manual_set_initial_phase = 0x0;
            QDECx->QDEC_CR_Y = qdec_0x0C.d32;
        }
        qdec_0x0C.b.y_debounce_cnt = QDEC_InitStruct->debounceTimeY;
        qdec_0x0C.b.y_cnt_scale = QDEC_InitStruct->counterScaleY;
        qdec_0x0C.b.y_debounce_en = QDEC_InitStruct->debounceEnableY;
        qdec_0x0C.b.y_cnt_pause = 0;
        QDECx->QDEC_CR_Y = qdec_0x0C.d32;
        QDECx->QDEC_INT_CLR |= QDEC_CLR_ACC_CT_Y;
    }
    if (QDEC_InitStruct->axisConfigZ == ENABLE)
    {
        QDEC_CR_Z_TypeDef qdec_0x14 = {.d32 = QDECx->QDEC_CR_Z};
        qdec_0x14.b.z_axis_en = 0x0;
        qdec_0x14.b.z_fsm_en = 0x1;
        /* Set Manual Initial Phase*/
        if (QDEC_InitStruct->manualLoadInitPhase == ENABLE)
        {
            qdec_0x14.b.manual_set_initial_phase = 0x1;
            qdec_0x14.b.z_initial_phase = QDEC_InitStruct->initPhaseZ;
            QDECx->QDEC_CR_Z = qdec_0x14.d32;

            qdec_0x14.b.manual_set_initial_phase = 0x0;
            QDECx->QDEC_CR_Z = qdec_0x14.d32;
        }
        else
        {
            qdec_0x14.b.manual_set_initial_phase = 0x0;
            QDECx->QDEC_CR_Z = qdec_0x14.d32;
        }
        qdec_0x14.b.z_debounce_cnt = QDEC_InitStruct->debounceTimeZ;
        qdec_0x14.b.z_cnt_scale = QDEC_InitStruct->counterScaleZ;
        qdec_0x14.b.z_debounce_en = QDEC_InitStruct->debounceEnableZ;
        qdec_0x14.b.z_cnt_pause = 0;
        QDECx->QDEC_CR_Z = qdec_0x14.d32;
        QDECx->QDEC_INT_CLR |= QDEC_CLR_ACC_CT_Z;
    }
    return;
}

/**
  * @brief  Fills each QDEC_InitStruct member with its default value.
  * @param  QDEC_InitStruct: pointer to an QDEC_InitStruct structure which will be initialized.
  * @retval None
  */
void QDEC_StructInit(QDEC_InitTypeDef *QDEC_InitStruct)
{
#if (QDEC_CLOCK_INIT_USE_KHZ == 1)
    QDEC_InitStruct->ScanClockKHZ         = 10; /*!< 10KHz */
    QDEC_InitStruct->DebonceClockKHZ     = 2;  /*!< 2KHz */
#else
    QDEC_InitStruct->scanClockDiv      = 0x7CF;/*!< 20M/(scanClockDiv+1) = 10KHz */
    QDEC_InitStruct->debounceClockDiv  = 0x4; /*!< 10KHz/(debounceClockDiv +1) = 2K */
#endif
    QDEC_InitStruct->axisConfigX       = DISABLE;
    QDEC_InitStruct->debounceTimeX     = 2 * 5; /*!< 5ms */
    QDEC_InitStruct->counterScaleX     = CounterScale_1_Phase;
    QDEC_InitStruct->debounceEnableX   = DISABLE;
    QDEC_InitStruct->initPhaseX        = phaseMode0;

    QDEC_InitStruct->axisConfigY       = DISABLE;
    QDEC_InitStruct->debounceTimeY     = 2 * 5; /*!< 5ms */
    QDEC_InitStruct->counterScaleY     = CounterScale_1_Phase;
    QDEC_InitStruct->debounceEnableY   = DISABLE;
    QDEC_InitStruct->initPhaseY        = phaseMode0;

    QDEC_InitStruct->axisConfigZ       = DISABLE;
    QDEC_InitStruct->debounceTimeZ     = 2 * 5; /*!< 5ms */
    QDEC_InitStruct->counterScaleZ     = CounterScale_1_Phase;
    QDEC_InitStruct->debounceEnableZ   = DISABLE;
    QDEC_InitStruct->initPhaseZ        = phaseMode0;

    return;
}


/**
  * @brief  Enables or disables the specified Qdecoder interrupts.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_IT: specifies the QDECODER interrupts sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg QDEC_X_INT_NEW_DATA:
  *     @arg QDEC_X_INT_ILLEAGE:
  *     @arg QDEC_Y_INT_NEW_DATA:
  *     @arg QDEC_Y_INT_ILLEAGE:
  *     @arg QDEC_Z_INT_NEW_DATA:
  *     @arg QDEC_Z_INT_ILLEAGE:
  * @param  NewState: new state of the specified QDECODER interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QDEC_INTConfig(QDEC_TypeDef *QDECx, uint32_t QDEC_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_QDEC_INT_CONFIG(QDEC_IT));

    QDEC_CR_X_TypeDef qdec_0x04 = {.d32 = QDECx->QDEC_CR_X};
    QDEC_CR_Y_TypeDef qdec_0x0C = {.d32 = QDECx->QDEC_CR_Y};
    QDEC_CR_Z_TypeDef qdec_0x14 = {.d32 = QDECx->QDEC_CR_Z};

    /* Enable the selected QDECODER interrupts */
    if (QDEC_IT & QDEC_X_INT_NEW_DATA)
    {
        qdec_0x04.b.x_cnt_int_en = NewState;
    }
    if (QDEC_IT & QDEC_Y_INT_NEW_DATA)
    {
        qdec_0x0C.b.y_cnt_int_en = NewState;
    }
    if (QDEC_IT & QDEC_Z_INT_NEW_DATA)
    {
        qdec_0x14.b.z_cnt_int_en = NewState;
    }
    if (QDEC_IT & QDEC_X_INT_ILLEAGE)
    {
        qdec_0x04.b.x_illegal_int_en = NewState;
    }
    if (QDEC_IT & QDEC_Y_INT_ILLEAGE)
    {
        qdec_0x0C.b.y_illegal_int_en = NewState;
    }
    if (QDEC_IT & QDEC_Z_INT_ILLEAGE)
    {
        qdec_0x14.b.z_illegal_int_en = NewState;
    }
    QDECx->QDEC_CR_X = qdec_0x04.d32;
    QDECx->QDEC_CR_Y = qdec_0x0C.d32;
    QDECx->QDEC_CR_Z = qdec_0x14.d32;

    return;
}

/**
  * @brief  Checks whether the specified Qdecoder flag is set or not.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg QDEC_FLAG_NEW_CT_STATUS_X:
  *     @arg QDEC_FLAG_NEW_CT_STATUS_Y:
  *     @arg QDEC_FLAG_NEW_CT_STATUS_Z:
  *     @arg QDEC_FLAG_OVERFLOW_X:
  *     @arg QDEC_FLAG_OVERFLOW_Y:
  *     @arg QDEC_FLAG_OVERFLOW_Z:
  *     @arg QDEC_FLAG_UNDERFLOW_X:
  *     @arg QDEC_FLAG_UNDERFLOW_Y:
  *     @arg QDEC_FLAG_UNDERFLOW_Z:
  *     @arg QDEC_FLAG_ILLEGAL_STATUS_X:
  *     @arg QDEC_FLAG_ILLEGAL_STATUS_Y:
  *     @arg QDEC_FLAG_ILLEGAL_STATUS_Z:
  * @retval The new state of QDEC_FLAG (SET or RESET).
  */
FlagStatus QDEC_GetFlagState(QDEC_TypeDef *QDECx, uint32_t QDEC_FLAG)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_INT_STATUS(QDEC_FLAG));

    FlagStatus bitstatus = RESET;

    if (((QDEC_FLAG & 0x7) | (QDEC_FLAG & (0x7 << 12))))
    {
        if (QDECx->QDEC_INT_SR & QDEC_FLAG)
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_OVERFLOW_X)
    {
        if (QDECx->QDEC_SR_X & (BIT18))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_OVERFLOW_Y)
    {
        if (QDECx->QDEC_SR_Y & (BIT18))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_OVERFLOW_Z)
    {
        if (QDECx->QDEC_SR_Z & (BIT18))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_UNDERFLOW_X)
    {
        if (QDECx->QDEC_SR_X & (BIT19))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_UNDERFLOW_Y)
    {
        if (QDECx->QDEC_SR_Y & (BIT19))
        {
            bitstatus = SET;
        }
    }
    else if (QDEC_FLAG == QDEC_FLAG_UNDERFLOW_Z)
    {
        if (QDECx->QDEC_SR_Z & (BIT19))
        {
            bitstatus = SET;
        }
    }

    return bitstatus;
}

/**
  * @brief  Enables or disables mask the specified Qdecoder axis interrupts.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *   This parameter can be one or logical OR of the following values:
  *     @arg QDEC_X_CT_INT_MASK: The qdecoder X axis.
  *     @arg QDEC_X_ILLEAGE_INT_MASK: The qdecoder Y axis.
  *     @arg QDEC_Y_CT_INT_MASK: The qdecoder Z axis.
  *     @arg QDEC_Y_ILLEAGE_INT_MASK: The qdecoder X axis.
  *     @arg QDEC_Z_CNT_INT_MASK: The qdecoder Y axis.
  *     @arg QDEC_Z_ILLEAGE_INT_MASK: The qdecoder Z axis.
  * @param  NewState: new state of the specified Qdecoder interrupts mask.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void QDEC_INTMask(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_INT_MASK_CONFIG(QDEC_AXIS));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* mask the selected QDEC interrupts */
        QDECx->QDEC_INT_MASK |= (QDEC_AXIS);
    }
    else
    {
        /* unmask the selected QDEC interrupts */
        QDECx->QDEC_INT_MASK &= (~(QDEC_AXIS));
    }

    return;
}

/**
  * @brief  Get Qdecoder Axis(x/y/z) direction.
  * @param  QDECx: selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *   This parameter can be one of the following values:
  *     @arg QDEC_AXIS_X: The qdecoder X axis.
  *     @arg QDEC_AXIS_Y: The qdecoder Y axis.
  *     @arg QDEC_AXIS_Z: The qdecoder Z axis.
  * @param  newState
  *   This parameter can be one of the following values:
  *     @arg ENABLE: Pause.
  *     @arg DISABLE: Resume.
  * @retval The count of the axis.
  */
void QDEC_Cmd(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS,
              FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_AXIS_DIR(QDEC_AXIS));

    if (NewState == ENABLE)
    {
        *((volatile uint32_t *)(&QDECx->QDEC_CR_X) + QDEC_AXIS / 2) |= QDEC_0X04_AXIS_EN;
    }
    else
    {
        *((volatile uint32_t *)(&QDECx->QDEC_CR_X) + QDEC_AXIS / 2) &= ~(QDEC_0X04_AXIS_EN);
    }
}

/**
  * @brief  Clear Qdecoder interrupt pending bit.
  * @param  QDECx: Selected Qdecoder peripheral.
  * @param  QDEC_FLAG: Specifies the flag to clear.
  * @return None.
  */
void QDEC_ClearINTPendingBit(QDEC_TypeDef *QDECx, uint32_t QDEC_CLR_INT)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_CLR_INT_STATUS(QDEC_CLR_INT));

    QDECx->QDEC_INT_CLR |= QDEC_CLR_INT;

    return;
}

/**
  * @brief  Get Qdecoder Axis(x/y/z) direction.
  * @param  QDECx: Selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *         This parameter can be one of the following values:
  *         @arg QDEC_AXIS_X: The qdecoder X axis.
  *         @arg QDEC_AXIS_Y: The qdecoder Y axis.
  *         @arg QDEC_AXIS_Z: The qdecoder Z axis.
  *
  * @return The direction of the axis.
  */
uint16_t QDEC_GetAxisDirection(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_AXIS_DIR(QDEC_AXIS));

    return ((*((volatile uint32_t *)(&QDECx->QDEC_SR_X) + QDEC_AXIS / 2) &
             QDEC_0X08_CNT_DIR) == QDEC_0X08_CNT_DIR);
}

/**
  * @brief  Get Qdecoder Axis(x/y/z) count.
  * @param  QDECx: Selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *         This parameter can be one of the following values:
  *         @arg QDEC_AXIS_X: The qdecoder X axis.
  *         @arg QDEC_AXIS_Y: The qdecoder Y axis.
  *         @arg QDEC_AXIS_Z: The qdecoder Z axis.
  *
  * @return The count of the axis.
  */
uint16_t QDEC_GetAxisCount(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_AXIS_DIR(QDEC_AXIS));

    return ((uint16_t)(*((volatile uint32_t *)(&QDECx->QDEC_SR_X) + QDEC_AXIS / 2)));
}

/**
  * @brief  Pause or resume Qdecoder Axis(x/y/z).
  * @param  QDECx: Selected Qdecoder peripheral.
  * @param  QDEC_AXIS: specifies the Qdecoder axis.
  *         This parameter can be one of the following values:
  *         @arg QDEC_AXIS_X: The qdecoder X axis.
  *         @arg QDEC_AXIS_Y: The qdecoder Y axis.
  *         @arg QDEC_AXIS_Z: The qdecoder Z axis.
  * @param  NewState: This parameter can be one of the following values:
  *         @arg ENABLE: Pause.
  *         @arg DISABLE: Resume.
  * @return None.
  */
void QDEC_CounterPauseCmd(QDEC_TypeDef *QDECx, uint32_t QDEC_AXIS,
                          FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_QDEC_PERIPH(QDECx));
    assert_param(IS_QDEC_AXIS_DIR(QDEC_AXIS));

    if (NewState == ENABLE)
    {
        *((volatile uint32_t *)(&QDECx->QDEC_CR_X) + QDEC_AXIS / 2) |= QDEC_0X00_CNT_PAUSE;
    }
    else
    {
        *((volatile uint32_t *)(&QDECx->QDEC_CR_X) + QDEC_AXIS / 2) &= ~QDEC_0X00_CNT_PAUSE;
    }
}