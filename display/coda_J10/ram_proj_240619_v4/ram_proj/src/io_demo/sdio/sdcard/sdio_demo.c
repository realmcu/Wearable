/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     sdio_demo.c
* @brief    This file provides SDcard demo code.
* @details
* @author   justin kang
* @date     2022-03-24
* @version  v1.0
*********************************************************************************************************
*/

/* Includes -----------------------------------------------------------------*/
#include <string.h>
#include "trace.h"
#include "sd.h"
#if defined(IC_TYPE_BB2)
///#include "os_mem.h"
#endif

/** @defgroup  SDIO_DEMO_SDIO  SDIO DEMO
    * @brief
    * @{
    */


/* Declaration --------------------------------------------------------------*/

/* Defines ------------------------------------------------------------------*/
#define SINGLE_BLOCK_SIZE       ((uint32_t)512)
#define BLOCK_NUM               ((uint32_t)1)
#define OPER_SD_CARD_ADDR       ((uint32_t)0x8000)
#define BLOCK_DATA_SIZE         (SINGLE_BLOCK_SIZE * BLOCK_NUM)

const T_SD_CONFIG    sd_card_cfg =
{
    .sd_if_type = SD_IF_SD_CARD,
    .sdh_group = GROUP_0,
    .sdh_bus_width = SD_BUS_WIDTH_4B,
    .sd_bus_clk_sel = SD_BUS_CLK_20M
};

/* Globals ------------------------------------------------------------------*/
#if defined(IC_TYPE_BB2)
uint8_t buf[BLOCK_DATA_SIZE];//= NULL;
#elif defined(IC_TYPE_BBPRO3)
__align(32) uint8_t buf[SINGLE_BLOCK_SIZE * BLOCK_NUM];
#endif

void sd_test(void)
{
    //IO_PRINT_INFO0("sd_test ");

    uint32_t sd_status = 0;

#if defined(IC_TYPE_BB2)
    /// buf = os_mem_alloc(OS_MEM_TYPE_BUFFER, BLOCK_DATA_SIZE);
#endif

    sd_config_init((T_SD_CONFIG *)&sd_card_cfg);
    sd_board_init();
    sd_card_init();

    memset(buf, 0, BLOCK_DATA_SIZE);
    sd_status = sd_read(OPER_SD_CARD_ADDR, (uint32_t)buf, SINGLE_BLOCK_SIZE, BLOCK_NUM);

    if (sd_status != 0)
    {
        return ;
    }
    sd_print_binary_data(buf, BLOCK_DATA_SIZE);

    for (uint32_t i = 0; i < BLOCK_DATA_SIZE; i++)
    {
        buf[i] = i & 0xff;
    }

    sd_print_binary_data(buf, BLOCK_DATA_SIZE);
    sd_status = sd_write(OPER_SD_CARD_ADDR, (uint32_t)buf, SINGLE_BLOCK_SIZE, BLOCK_NUM);

    if (sd_status != 0)
    {
        return ;
    }

    memset(buf, 0, BLOCK_DATA_SIZE);

    sd_status = sd_read(OPER_SD_CARD_ADDR, (uint32_t)buf, SINGLE_BLOCK_SIZE, BLOCK_NUM);

    if (sd_status != 0)
    {
        return ;
    }
    sd_print_binary_data(buf, BLOCK_DATA_SIZE);

}

/** @} */ /* End of group SDIO_DEMO_SDIO */

/******************* (C) COPYRIGHT 2021 Realtek Semiconductor Corporation *****END OF FILE****/

