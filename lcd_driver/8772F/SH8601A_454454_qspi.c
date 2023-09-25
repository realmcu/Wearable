#include "board.h"
#include "sh8601a_454454_qspi.h"
#include "rtl_lcdc_dbic.h"
#include "utils.h"
#include "trace.h"
#include "os_sched.h"
#include "rtl_pinmux.h"
#include "drv_gpio.h"

#define LCDC_DMA_CHANNEL_NUM              0
#define LCDC_DMA_CHANNEL_INDEX            LCDC_DMA_Channel0

#define BIT_CMD_CH(x)           (((x) & 0x00000003) << 20)
#define BIT_DATA_CH(x)          (((x) & 0x00000003) << 18)
#define BIT_ADDR_CH(x)          (((x) & 0x00000003) << 16)
#define BIT_TMOD(x)             (((x) & 0x00000003) << 8)

#define BIT_TXSIM               (0x00000001 << 9)
#define BIT_SEQ_EN              (0x00000001 << 3)





static void spic3_spi_write(uint8_t *buf, uint32_t len)
{
    DBIC->CTRLR0 |= BIT31;
    DBIC->CTRLR0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));//SET CHANNEL NUM
    DBIC->CTRLR0 &= ~(BIT_TMOD(3)); //tx mode

    DBIC_TX_NDF(len);
    for (uint32_t i = 0; i < len; i++)
    {
        DBIC->DR[0].byte = buf[i];
    }
    DBIC_Cmd(ENABLE);
    while (DBIC->SR & BIT0);// wait bus busy
    //DBIC_Cmd(DISABLE);//disable DBIC
    DBIC->CTRLR0 &= ~BIT31;
}


/* SH8601A QSPI Instruction Code */
#define SH8601A_QSPI_INST_CMD_WRITE                     (0x02)
#define SH8601A_QSPI_INST_CMD_READ                      (0x03)
#define SH8601A_QSPI_INST_1WIRE_PIXEL_WRITE             (0x02)
#define SH8601A_QSPI_INST_4WIRE_PIXEL_WRITE_TYPE1       (0x32)
#define SH8601A_QSPI_INST_4WIRE_PIXEL_WRITE_TYPE2       (0x12)
#define SH8601A_QSPI_SEQ_FINISH_CODE                    (0x00)

const SH8601A_CMD_DESC SH8601A_PRE_OTP_POWERON_SEQ_CMD[] =
{
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1, 2, {0x5A, 0x5A}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1, 2, {0x5A, 0x5A}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xE4, 1, 1, {0x01}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x90, 1, 6, {0x33, 0x00, 0xC6, 0x01, 0xC6, 0x01}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x91, 1, 20, {0x65, 0x00, 0x00, 0xE2, 0x00, 0x00, 0x00, 0xE2, 0x00, 0xE2, 0x00, 0xE2, 0x00, 0xE2, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x11}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x92, 1, 20, {0x61, 0xE3, 0x00, 0xC5, 0x01, 0x00, 0x00, 0xE2, 0x00, 0xE3, 0x00, 0xE2, 0x00, 0xE2, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x22}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x93, 1, 20, {0x69, 0x00, 0x00, 0xE2, 0x00, 0xE3, 0x00, 0xC5, 0x01, 0xE2, 0x00, 0xE3, 0x00, 0xE2, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x33}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x94, 1, 20, {0x6D, 0xE3, 0x00, 0xC5, 0x01, 0xE3, 0x00, 0xC5, 0x01, 0xE3, 0x00, 0xE3, 0x00, 0xE2, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x33}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x9D, 1, 168, {0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x06, 0x0C, 0x12, 0x18, 0x1E, 0x24, 0x2A, 0x30, 0x36, 0x3C, 0x42, 0x48, 0x4E, 0x54, 0x5A, 0x60, 0x09, 0x0D, 0x12, 0x16, 0x1B, 0x24, 0x2D, 0x36, 0x3F, 0x5A, 0x63, 0x6C, 0x75, 0x7E, 0x87, 0x90, 0x05, 0x0A, 0x0F, 0x14, 0x19, 0x1E, 0x23, 0x28, 0x2D, 0x32, 0x37, 0x3C, 0x41, 0x46, 0x4B, 0x50, 0x0F, 0x1E, 0x2D, 0x3C, 0x4B, 0x5A, 0x69, 0x78, 0x87, 0x96, 0xA5, 0xB4, 0xC3, 0xD2, 0xE1, 0xF5, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x60, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x9E, 1, 12, {0x3B, 0x00, 0x71, 0x00, 0xA3, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1, 51, {0x00, 0xC6, 0x01, 0xC6, 0x01, 0x05, 0x00, 0x05, 0x00, 0xA7, 0x00, 0xA7, 0x00, 0x05, 0x00, 0x05, 0x00, 0xA7, 0x00, 0xA7, 0x00, 0x00, 0x52, 0x00, 0x64, 0x00, 0x8A, 0x00, 0xB0, 0x00, 0x52, 0x00, 0x64, 0x00, 0x8A, 0x00, 0xB0, 0x00, 0x00, 0x10, 0x00, 0x00, 0xDF, 0x01, 0x00, 0x00, 0xDF, 0x01, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB2, 1, 68, {0x19, 0x14, 0x19, 0x14, 0x01, 0xEE, 0x02, 0x30, 0x02, 0xE4, 0x02, 0x3F, 0x02, 0x06, 0x76, 0x78, 0xE8, 0x04, 0x06, 0x00, 0x00, 0x31, 0x16, 0x15, 0x3D, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5, 0x10, 0xFF, 0xFF, 0xE0, 0xAA, 0xAA, 0xFF, 0xFF, 0x24, 0x14, 0x04, 0x14, 0x13, 0x14, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB3, 1, 44, {0x00, 0x0D, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x1F, 0x02, 0x00, 0x0B, 0x00, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x12, 0x05, 0x00, 0x06, 0x13, 0x04, 0x00, 0x08, 0x15, 0x09, 0x00, 0x07}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB4, 1, 65, {0x09, 0x02, 0x00, 0x00, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x40, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x03, 0x47, 0x8B, 0x30, 0x74, 0xB8, 0x12, 0x56, 0x9A, 0x21, 0x65, 0xA9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x56, 0x9A, 0x21, 0x65, 0xA9, 0x03, 0x47, 0x8B, 0x30, 0x74, 0xB8, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB5, 1, 58, {0x4C, 0x09, 0x09, 0x09, 0x49, 0x40, 0x00, 0x01, 0x21, 0x00, 0x00, 0x00, 0x21, 0x00, 0x14, 0x03, 0x21, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x03, 0x02, 0x52, 0x00, 0x21, 0x00, 0x21, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB6, 1, 26, {0x00, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x40, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB7, 1, 51, {0x0C, 0x00, 0x01, 0x21, 0x00, 0x00, 0x00, 0x21, 0x00, 0x14, 0x03, 0x21, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x03, 0x02, 0x52, 0x00, 0x21, 0x00, 0x21, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x21, 0x00, 0x14, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB8, 1, 95, {0x00, 0x67, 0x31, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x22, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x22, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xBB, 1, 19, {0x01, 0x02, 0x07, 0x01, 0x46, 0x46, 0x46, 0xD9, 0x00, 0xAA, 0x00, 0x7D, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xBD, 1, 22, {0x01, 0x00, 0x00, 0x64, 0x00, 0x62, 0x00, 0x04, 0x01, 0x15, 0x00, 0x00, 0x00, 0x00, 0x1D, 0x05, 0x00, 0x10, 0x16, 0x16, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xBE, 1, 106, {0x4B, 0x00, 0x69, 0x00, 0x87, 0x00, 0xA5, 0x00, 0xC3, 0x00, 0xE1, 0x00, 0xFF, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0x64, 0x00, 0xAA, 0x00, 0x19, 0x00, 0x32, 0x00, 0x4B, 0x00, 0x4B, 0x00, 0x4B, 0x00, 0x4B, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x01, 0x01, 0x15, 0x00, 0x00, 0x00, 0x00, 0x8A, 0x00, 0x26, 0x01, 0xD0, 0x01, 0xD0, 0x01, 0xD0, 0x01, 0xD0, 0x01, 0xD0, 0x01, 0xD0, 0x01, 0x1C, 0x00, 0x64, 0x00, 0x00, 0x00, 0x89, 0x00, 0xD0, 0x01, 0xD0, 0x01, 0x4B, 0x00, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xBF, 1, 35, {0x03, 0xF0, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC2, 1, 136, {0x00, 0x00, 0x84, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDD, 0x00, 0x97, 0x00, 0x83, 0x00, 0xEE, 0x00, 0xA8, 0x00, 0x94, 0x00, 0x16, 0x01, 0xCF, 0x00, 0xBC, 0x00, 0x3B, 0x01, 0xF4, 0x00, 0xE1, 0x00, 0x61, 0x01, 0x12, 0x01, 0x08, 0x01, 0x94, 0x01, 0x46, 0x01, 0x41, 0x01, 0xB7, 0x01, 0x6D, 0x01, 0x6B, 0x01, 0xDE, 0x01, 0x93, 0x01, 0x9C, 0x01, 0xF7, 0x01, 0xAF, 0x01, 0xBB, 0x01, 0x27, 0x02, 0xE1, 0x01, 0xFC, 0x01, 0x52, 0x02, 0x0F, 0x02, 0x34, 0x02, 0x77, 0x02, 0x36, 0x02, 0x65, 0x02, 0xBE, 0x02, 0x7C, 0x02, 0xB9, 0x02, 0xFA, 0x02, 0xBA, 0x02, 0x08, 0x03, 0x31, 0x03, 0xF4, 0x02, 0x4F, 0x03, 0x63, 0x03, 0x29, 0x03, 0x8C, 0x03, 0x9E, 0x03, 0x5F, 0x03, 0xD1, 0x03, 0xFD, 0x03, 0xBC, 0x03, 0x4A, 0x04, 0x6E, 0x04, 0x28, 0x04, 0xC4, 0x04, 0xD1, 0x04, 0x91, 0x04, 0x49, 0x05, 0x3D, 0x05, 0xF1, 0x04, 0xC1, 0x05}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC3, 1, 136, {0x00, 0x00, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDD, 0x00, 0x97, 0x00, 0x83, 0x00, 0xEE, 0x00, 0xA8, 0x00, 0x94, 0x00, 0x16, 0x01, 0xCF, 0x00, 0xBC, 0x00, 0x3B, 0x01, 0xF4, 0x00, 0xE1, 0x00, 0x61, 0x01, 0x12, 0x01, 0x08, 0x01, 0x94, 0x01, 0x46, 0x01, 0x41, 0x01, 0xB7, 0x01, 0x6D, 0x01, 0x6B, 0x01, 0xDE, 0x01, 0x93, 0x01, 0x9C, 0x01, 0xF7, 0x01, 0xAF, 0x01, 0xBB, 0x01, 0x27, 0x02, 0xE1, 0x01, 0xFC, 0x01, 0x52, 0x02, 0x0F, 0x02, 0x34, 0x02, 0x77, 0x02, 0x36, 0x02, 0x65, 0x02, 0xBE, 0x02, 0x7C, 0x02, 0xB9, 0x02, 0xFA, 0x02, 0xBA, 0x02, 0x08, 0x03, 0x31, 0x03, 0xF4, 0x02, 0x4F, 0x03, 0x63, 0x03, 0x29, 0x03, 0x8C, 0x03, 0x9E, 0x03, 0x5F, 0x03, 0xD1, 0x03, 0xFD, 0x03, 0xBC, 0x03, 0x4A, 0x04, 0x6E, 0x04, 0x28, 0x04, 0xC4, 0x04, 0xD1, 0x04, 0x91, 0x04, 0x49, 0x05, 0x3D, 0x05, 0xF1, 0x04, 0xC1, 0x05}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC5, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x37, 0x00, 0x32, 0x00, 0x24, 0x01, 0xBD, 0x00, 0xAC, 0x00, 0x3D, 0x01, 0xD6, 0x00, 0xC5, 0x00, 0x50, 0x01, 0xE9, 0x00, 0xD8, 0x00, 0x66, 0x01, 0xFF, 0x00, 0xEE, 0x00, 0x95, 0x01, 0x2E, 0x01, 0x1E, 0x01, 0xB8, 0x01, 0x50, 0x01, 0x44, 0x01, 0xDB, 0x01, 0x73, 0x01, 0x68, 0x01, 0xF5, 0x01, 0x8E, 0x01, 0x84, 0x01, 0x21, 0x02, 0xBE, 0x01, 0xBA, 0x01, 0x4B, 0x02, 0xEB, 0x01, 0xEE, 0x01, 0x6B, 0x02, 0x0E, 0x02, 0x18, 0x02, 0xA9, 0x02, 0x51, 0x02, 0x6C, 0x02, 0xE2, 0x02, 0x8B, 0x02, 0xB2, 0x02, 0x0B, 0x03, 0xB7, 0x02, 0xE7, 0x02, 0x34, 0x03, 0xE5, 0x02, 0x1F, 0x03, 0x57, 0x03, 0x0A, 0x03, 0x4B, 0x03, 0xA3, 0x03, 0x52, 0x03, 0xA9, 0x03, 0xEB, 0x03, 0x9A, 0x03, 0x00, 0x04, 0x2F, 0x04, 0xDC, 0x03, 0x5A, 0x04, 0x64, 0x04, 0x14, 0x04, 0x9E, 0x04}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC6, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x4D, 0x00, 0x46, 0x00, 0x28, 0x01, 0xC1, 0x00, 0xB0, 0x00, 0x42, 0x01, 0xDB, 0x00, 0xCA, 0x00, 0x5C, 0x01, 0xF5, 0x00, 0xE4, 0x00, 0x77, 0x01, 0x10, 0x01, 0xFF, 0x00, 0xA9, 0x01, 0x42, 0x01, 0x35, 0x01, 0xCE, 0x01, 0x67, 0x01, 0x5B, 0x01, 0xF1, 0x01, 0x8A, 0x01, 0x80, 0x01, 0x0C, 0x02, 0xA7, 0x01, 0xA0, 0x01, 0x3D, 0x02, 0xDD, 0x01, 0xDD, 0x01, 0x66, 0x02, 0x08, 0x02, 0x11, 0x02, 0x8C, 0x02, 0x33, 0x02, 0x45, 0x02, 0xD0, 0x02, 0x79, 0x02, 0x9D, 0x02, 0x04, 0x03, 0xB0, 0x02, 0xDE, 0x02, 0x33, 0x03, 0xE5, 0x02, 0x1F, 0x03, 0x5D, 0x03, 0x11, 0x03, 0x53, 0x03, 0x89, 0x03, 0x3C, 0x03, 0x8A, 0x03, 0xDE, 0x03, 0x8D, 0x03, 0xF1, 0x03, 0x2F, 0x04, 0xDC, 0x03, 0x59, 0x04, 0x6F, 0x04, 0x20, 0x04, 0xAC, 0x04, 0xB3, 0x04, 0x64, 0x04, 0xFF, 0x04}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC7, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x00, 0x63, 0x00, 0x5A, 0x00, 0x2C, 0x01, 0xC5, 0x00, 0xB4, 0x00, 0x48, 0x01, 0xE1, 0x00, 0xD0, 0x00, 0x65, 0x01, 0xFE, 0x00, 0xED, 0x00, 0x87, 0x01, 0x20, 0x01, 0x0F, 0x01, 0xB5, 0x01, 0x4E, 0x01, 0x41, 0x01, 0xE5, 0x01, 0x7D, 0x01, 0x72, 0x01, 0x01, 0x02, 0x9B, 0x01, 0x93, 0x01, 0x1D, 0x02, 0xBA, 0x01, 0xB6, 0x01, 0x55, 0x02, 0xF5, 0x01, 0xFB, 0x01, 0x7C, 0x02, 0x21, 0x02, 0x2F, 0x02, 0xA4, 0x02, 0x4C, 0x02, 0x65, 0x02, 0xF0, 0x02, 0x99, 0x02, 0xC1, 0x02, 0x20, 0x03, 0xCE, 0x02, 0x04, 0x03, 0x51, 0x03, 0x04, 0x03, 0x44, 0x03, 0x83, 0x03, 0x36, 0x03, 0x82, 0x03, 0xB5, 0x03, 0x62, 0x03, 0xBF, 0x03, 0x0F, 0x04, 0xBD, 0x03, 0x2F, 0x04, 0x5C, 0x04, 0x0C, 0x04, 0x94, 0x04, 0xAB, 0x04, 0x5C, 0x04, 0xF5, 0x04, 0xFA, 0x04, 0xA8, 0x04, 0x50, 0x05}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC8, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBC, 0x00, 0x79, 0x00, 0x6E, 0x00, 0x2F, 0x01, 0xC8, 0x00, 0xB7, 0x00, 0x4D, 0x01, 0xE6, 0x00, 0xD5, 0x00, 0x6D, 0x01, 0x06, 0x01, 0xF5, 0x00, 0x8F, 0x01, 0x28, 0x01, 0x18, 0x01, 0xC1, 0x01, 0x5A, 0x01, 0x4E, 0x01, 0xEF, 0x01, 0x87, 0x01, 0x7D, 0x01, 0x0F, 0x02, 0xAB, 0x01, 0xA4, 0x01, 0x2F, 0x02, 0xCD, 0x01, 0xCB, 0x01, 0x63, 0x02, 0x04, 0x02, 0x0D, 0x02, 0x91, 0x02, 0x38, 0x02, 0x4A, 0x02, 0xBC, 0x02, 0x65, 0x02, 0x86, 0x02, 0x01, 0x03, 0xAC, 0x02, 0xD9, 0x02, 0x39, 0x03, 0xEA, 0x02, 0x25, 0x03, 0x6E, 0x03, 0x23, 0x03, 0x69, 0x03, 0xA4, 0x03, 0x53, 0x03, 0xAA, 0x03, 0xD7, 0x03, 0x85, 0x03, 0xE8, 0x03, 0x37, 0x04, 0xE5, 0x03, 0x64, 0x04, 0x8A, 0x04, 0x3C, 0x04, 0xCF, 0x04, 0xE0, 0x04, 0x8F, 0x04, 0x32, 0x05, 0x30, 0x05, 0xDF, 0x04, 0x92, 0x05}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC9, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x8F, 0x00, 0x82, 0x00, 0x33, 0x01, 0xCC, 0x00, 0xBB, 0x00, 0x52, 0x01, 0xEB, 0x00, 0xDA, 0x00, 0x76, 0x01, 0x0F, 0x01, 0xFE, 0x00, 0x98, 0x01, 0x31, 0x01, 0x22, 0x01, 0xCD, 0x01, 0x65, 0x01, 0x5A, 0x01, 0xF8, 0x01, 0x92, 0x01, 0x89, 0x01, 0x1B, 0x02, 0xB8, 0x01, 0xB2, 0x01, 0x3C, 0x02, 0xDB, 0x01, 0xDC, 0x01, 0x70, 0x02, 0x14, 0x02, 0x1F, 0x02, 0xA1, 0x02, 0x49, 0x02, 0x60, 0x02, 0xCF, 0x02, 0x78, 0x02, 0x9C, 0x02, 0x11, 0x03, 0xBE, 0x02, 0xF0, 0x02, 0x4D, 0x03, 0xFF, 0x02, 0x3E, 0x03, 0x87, 0x03, 0x3A, 0x03, 0x87, 0x03, 0xC1, 0x03, 0x6E, 0x03, 0xCD, 0x03, 0xF7, 0x03, 0xA6, 0x03, 0x10, 0x04, 0x56, 0x04, 0x05, 0x04, 0x8B, 0x04, 0xB2, 0x04, 0x63, 0x04, 0xFD, 0x04, 0x10, 0x05, 0xBE, 0x04, 0x6A, 0x05, 0x64, 0x05, 0x14, 0x05, 0xD2, 0x05}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xCA, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA5, 0x00, 0x96, 0x00, 0x36, 0x01, 0xCF, 0x00, 0xBE, 0x00, 0x57, 0x01, 0xF0, 0x00, 0xDF, 0x00, 0x7E, 0x01, 0x17, 0x01, 0x06, 0x01, 0xA0, 0x01, 0x39, 0x01, 0x2B, 0x01, 0xD9, 0x01, 0x71, 0x01, 0x66, 0x01, 0x02, 0x02, 0x9C, 0x01, 0x94, 0x01, 0x26, 0x02, 0xC4, 0x01, 0xC1, 0x01, 0x49, 0x02, 0xE8, 0x01, 0xEB, 0x01, 0x7E, 0x02, 0x23, 0x02, 0x32, 0x02, 0xB1, 0x02, 0x59, 0x02, 0x76, 0x02, 0xDF, 0x02, 0x88, 0x02, 0xAF, 0x02, 0x22, 0x03, 0xD1, 0x02, 0x07, 0x03, 0x60, 0x03, 0x14, 0x03, 0x57, 0x03, 0x9F, 0x03, 0x4E, 0x03, 0xA4, 0x03, 0xDA, 0x03, 0x88, 0x03, 0xEC, 0x03, 0x13, 0x04, 0xC1, 0x03, 0x34, 0x04, 0x74, 0x04, 0x25, 0x04, 0xB3, 0x04, 0xD8, 0x04, 0x87, 0x04, 0x29, 0x05, 0x37, 0x05, 0xE7, 0x04, 0x9B, 0x05, 0x95, 0x05, 0x41, 0x05, 0x0B, 0x06}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xCB, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x01, 0xBB, 0x00, 0xAA, 0x00, 0x3A, 0x01, 0xD3, 0x00, 0xC2, 0x00, 0x5C, 0x01, 0xF5, 0x00, 0xE4, 0x00, 0x87, 0x01, 0x20, 0x01, 0x0F, 0x01, 0xA9, 0x01, 0x42, 0x01, 0x35, 0x01, 0xE5, 0x01, 0x7D, 0x01, 0x72, 0x01, 0x0C, 0x02, 0xA7, 0x01, 0xA0, 0x01, 0x32, 0x02, 0xD1, 0x01, 0xCF, 0x01, 0x55, 0x02, 0xF5, 0x01, 0xFB, 0x01, 0x8C, 0x02, 0x33, 0x02, 0x44, 0x02, 0xC1, 0x02, 0x6A, 0x02, 0x8C, 0x02, 0xF0, 0x02, 0x99, 0x02, 0xC2, 0x02, 0x33, 0x03, 0xE4, 0x02, 0x1E, 0x03, 0x74, 0x03, 0x29, 0x03, 0x70, 0x03, 0xB6, 0x03, 0x63, 0x03, 0xC0, 0x03, 0xF3, 0x03, 0xA2, 0x03, 0x0A, 0x04, 0x2E, 0x04, 0xDB, 0x03, 0x58, 0x04, 0x93, 0x04, 0x45, 0x04, 0xDA, 0x04, 0xFE, 0x04, 0xAC, 0x04, 0x54, 0x05, 0x5F, 0x05, 0x0F, 0x05, 0xCC, 0x05, 0xC5, 0x05, 0x6E, 0x05, 0x43, 0x06}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xCC, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x72, 0x00, 0x49, 0x00, 0x43, 0x00, 0x27, 0x01, 0xC0, 0x00, 0xAF, 0x00, 0x42, 0x01, 0xDB, 0x00, 0xCA, 0x00, 0x5A, 0x01, 0xF3, 0x00, 0xE2, 0x00, 0x74, 0x01, 0x0D, 0x01, 0xFC, 0x00, 0xA6, 0x01, 0x3F, 0x01, 0x32, 0x01, 0xCA, 0x01, 0x63, 0x01, 0x57, 0x01, 0xEE, 0x01, 0x87, 0x01, 0x7D, 0x01, 0x08, 0x02, 0xA3, 0x01, 0x9C, 0x01, 0x39, 0x02, 0xD9, 0x01, 0xD8, 0x01, 0x62, 0x02, 0x03, 0x02, 0x0C, 0x02, 0x87, 0x02, 0x2D, 0x02, 0x3D, 0x02, 0xCB, 0x02, 0x74, 0x02, 0x97, 0x02, 0x00, 0x03, 0xAB, 0x02, 0xD8, 0x02, 0x2D, 0x03, 0xDD, 0x02, 0x16, 0x03, 0x56, 0x03, 0x09, 0x03, 0x4A, 0x03, 0x82, 0x03, 0x35, 0x03, 0x81, 0x03, 0xD5, 0x03, 0x83, 0x03, 0xE6, 0x03, 0x24, 0x04, 0xD1, 0x03, 0x4B, 0x04, 0x65, 0x04, 0x14, 0x04, 0x9E, 0x04, 0xA7, 0x04, 0x58, 0x04, 0xF1, 0x04}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xCD, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC1, 0x00, 0x7D, 0x00, 0x71, 0x00, 0x30, 0x01, 0xC9, 0x00, 0xB8, 0x00, 0x4E, 0x01, 0xE7, 0x00, 0xD6, 0x00, 0x6F, 0x01, 0x08, 0x01, 0xF7, 0x00, 0x91, 0x01, 0x2A, 0x01, 0x1A, 0x01, 0xC3, 0x01, 0x5C, 0x01, 0x50, 0x01, 0xF0, 0x01, 0x89, 0x01, 0x7F, 0x01, 0x11, 0x02, 0xAD, 0x01, 0xA7, 0x01, 0x32, 0x02, 0xD1, 0x01, 0xCE, 0x01, 0x65, 0x02, 0x07, 0x02, 0x10, 0x02, 0x93, 0x02, 0x3B, 0x02, 0x4E, 0x02, 0xC0, 0x02, 0x69, 0x02, 0x8B, 0x02, 0x03, 0x03, 0xAF, 0x02, 0xDD, 0x02, 0x3C, 0x03, 0xEE, 0x02, 0x2A, 0x03, 0x73, 0x03, 0x28, 0x03, 0x6F, 0x03, 0xA9, 0x03, 0x58, 0x03, 0xB0, 0x03, 0xDD, 0x03, 0x8B, 0x03, 0xEF, 0x03, 0x3C, 0x04, 0xEA, 0x03, 0x6A, 0x04, 0x92, 0x04, 0x44, 0x04, 0xD9, 0x04, 0xE9, 0x04, 0x98, 0x04, 0x3C, 0x05, 0x39, 0x05, 0xE8, 0x04, 0x9D, 0x05}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xCE, 1, 136, {0x00, 0x00, 0x6A, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x01, 0xBB, 0x00, 0xAA, 0x00, 0x3A, 0x01, 0xD3, 0x00, 0xC2, 0x00, 0x5C, 0x01, 0xF5, 0x00, 0xE4, 0x00, 0x87, 0x01, 0x20, 0x01, 0x0F, 0x01, 0xA9, 0x01, 0x42, 0x01, 0x35, 0x01, 0xE5, 0x01, 0x7D, 0x01, 0x72, 0x01, 0x0C, 0x02, 0xA7, 0x01, 0xA0, 0x01, 0x32, 0x02, 0xD1, 0x01, 0xCF, 0x01, 0x55, 0x02, 0xF5, 0x01, 0xFB, 0x01, 0x8C, 0x02, 0x33, 0x02, 0x44, 0x02, 0xC1, 0x02, 0x6A, 0x02, 0x8C, 0x02, 0xF0, 0x02, 0x99, 0x02, 0xC2, 0x02, 0x33, 0x03, 0xE4, 0x02, 0x1E, 0x03, 0x74, 0x03, 0x29, 0x03, 0x70, 0x03, 0xB6, 0x03, 0x63, 0x03, 0xC0, 0x03, 0xF3, 0x03, 0xA2, 0x03, 0x0A, 0x04, 0x2E, 0x04, 0xDB, 0x03, 0x58, 0x04, 0x93, 0x04, 0x45, 0x04, 0xDA, 0x04, 0xFE, 0x04, 0xAC, 0x04, 0x54, 0x05, 0x5F, 0x05, 0x0F, 0x05, 0xCC, 0x05, 0xC5, 0x05, 0x6E, 0x05, 0x43, 0x06}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD3, 1, 11, {0x11, 0xC6, 0x01, 0xC6, 0x01, 0x08, 0x08, 0x14, 0x14, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD4, 1, 54, {0x02, 0x00, 0x2c, 0x00, 0x19, 0x00, 0x06, 0x00, 0x6c, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x19, 0x00, 0x2c, 0x00, 0x02, 0x00, 0x3b, 0x00, 0x6c, 0x00, 0x06, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x03, 0x00, 0x1e, 0x00, 0x7e, 0x00, 0x1e, 0x00, 0x08, 0x00, 0x29, 0x00, 0x08, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD5, 1, 54, {0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x06, 0x00, 0x6c, 0x00, 0x3b, 0x00, 0x02, 0x00, 0x2c, 0x00, 0x19, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x6c, 0x00, 0x06, 0x00, 0x19, 0x00, 0x2c, 0x00, 0x02, 0x00, 0x08, 0x00, 0x29, 0x00, 0x08, 0x00, 0x1e, 0x00, 0x7e, 0x00, 0x1e, 0x00, 0x03, 0x00, 0x07, 0x00, 0x03, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD6, 1, 54, {0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x06, 0x00, 0x6c, 0x00, 0x3b, 0x00, 0x02, 0x00, 0x2c, 0x00, 0x19, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x6c, 0x00, 0x06, 0x00, 0x19, 0x00, 0x2c, 0x00, 0x02, 0x00, 0x08, 0x00, 0x29, 0x00, 0x08, 0x00, 0x1e, 0x00, 0x7e, 0x00, 0x1e, 0x00, 0x03, 0x00, 0x07, 0x00, 0x03, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD7, 1, 54, {0x02, 0x00, 0x2c, 0x00, 0x19, 0x00, 0x06, 0x00, 0x6c, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x19, 0x00, 0x2c, 0x00, 0x02, 0x00, 0x3b, 0x00, 0x6c, 0x00, 0x06, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x07, 0x00, 0x03, 0x00, 0x1e, 0x00, 0x7e, 0x00, 0x1e, 0x00, 0x08, 0x00, 0x29, 0x00, 0x08, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xDF, 1, 19, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xEE, 1, 44, {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF0, 1, 24, {0x10, 0x79, 0x77, 0x22, 0x4A, 0x25, 0x1C, 0x19, 0x00, 0x10, 0x0F, 0x11, 0x38, 0xAA, 0x20, 0x20, 0x2A, 0x22, 0x2A, 0xCA, 0x88, 0x10, 0x10, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF1, 1, 25, {0x10, 0x09, 0x03, 0x00, 0x4A, 0x25, 0x1C, 0x19, 0x00, 0x10, 0x0F, 0x00, 0x30, 0xAA, 0x20, 0x20, 0x2F, 0x23, 0x2F, 0x22, 0x2E, 0xCF, 0x88, 0x10, 0x10}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF1, 1, 25, {0x10, 0x09, 0x03, 0x00, 0x4A, 0x25, 0x1C, 0x19, 0x00, 0x10, 0x0F, 0x11, 0x30, 0xAA, 0x10, 0x10, 0x26, 0x20, 0x25, 0x12, 0x16, 0xC5, 0x88, 0x10, 0x01}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF2, 1, 34, {0xFF, 0x53, 0x00, 0x11, 0x19, 0x0A, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F, 0x00, 0x01, 0x01, 0xFF, 0x53, 0x10, 0x11, 0x19, 0x0A, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F, 0x01, 0x01, 0x01}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF3, 1, 36, {0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x02, 0x03, 0x02, 0x02, 0x03, 0x01, 0x00, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF4, 1, 21, {0x99, 0x99, 0x99, 0x88, 0x88, 0x77, 0x66, 0x55, 0x55, 0x55, 0x55, 0x55, 0x44, 0x94, 0x01, 0x01, 0x04, 0x01, 0x01, 0x01, 0x01}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF5, 1, 21, {0x66, 0x66, 0x56, 0x55, 0x45, 0x44, 0x33, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x62, 0x02, 0x02, 0x02, 0x00, 0x01, 0x01, 0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xF8, 1, 16, {0x03, 0x22, 0x22, 0x77, 0x37, 0x00, 0x10, 0x10, 0x26, 0x20, 0x25, 0x12, 0x14, 0x05, 0x66, 0x66}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x11, 10, 0, {0}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x2A, 1, 4, {0x00, 0x00, 0x01, 0xC5}},
//    {SH8601A_QSPI_INST_CMD_WRITE, 0x2B, 1, 4, {0x00, 0x00, 0x01, 0xC5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x44, 1, 2, {0x01, 0xC2}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x35, 1, 1, {0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x51, 1, 1, {0x0}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x4A, 1, 1, {0xFF}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x63, 1, 1, {0xFF}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x53, 1, 1, {0x28}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC4, 25, 1, {0x84}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x29, 1, 0, {0}},

    //{SH8601A_QSPI_INST_CMD_WRITE, 0xBA, 1, 1, {0x10}},      // bist Mode   Built-in Self-Test
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1,  1,  {0xC0}},

    {SH8601A_QSPI_SEQ_FINISH_CODE,  0,  0,  0,  {0}},

};

const SH8601A_CMD_DESC SH8601A_POST_OTP_POWERON_SEQ_CMD[] =
{
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1,  2,  {0x5A, 0x5A}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1,  2,  {0x5A, 0x5A}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x11, 10, 0,  {0}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x2A, 1,  4,  {0x00, 0x00, 0x01, 0xC5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x2B, 1,  4,  {0x00, 0x00, 0x01, 0xC5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x44, 1,  2,  {0x01, 0xC2}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x35, 1,  1,  {0x00}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB0, 1,  1,  {0x16}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1,  9,  {0x01, 0x05, 0x00, 0xA2, 0x00, 0xA7, 0x00, 0xA7, 0x01}},   // 0x01=45Hz, 0x00=60Hz

    {SH8601A_QSPI_INST_CMD_WRITE, 0x53, 1,  1,  {0x28}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC4, 25, 1,  {0x84}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x29, 1,  0,  {0}},
    //{SH8601A_QSPI_INST_CMD_WRITE, 0xBA, 1,  1,  {0x81}},         // bist: 0x81,      exit bist: 0x80
    {SH8601A_QSPI_INST_CMD_WRITE, 0xB1, 1,  1,  {0xC0}},

    {SH8601A_QSPI_INST_CMD_WRITE, 0xC0, 1,  2,  {0xA5, 0xA5}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xC1, 1,  2,  {0xA5, 0xA5}},

    {SH8601A_QSPI_SEQ_FINISH_CODE,   0, 0,  0,  {0}},
};

const SH8601A_CMD_DESC SH8601A_OTP_WRITE_POWERON[] =
{
    {SH8601A_QSPI_INST_CMD_WRITE, 0x29, 1, 1, {0}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x11, 5, 0, {0}},
    {SH8601A_QSPI_SEQ_FINISH_CODE,  0,  0,  0,  {0}},
};

const SH8601A_CMD_DESC SH8601A_POWEROFF_SEQ_CMD[] =
{
    {SH8601A_QSPI_INST_CMD_WRITE, 0x28, 25, 0,  {0}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0x10, 50, 0,  {0}},
    {SH8601A_QSPI_SEQ_FINISH_CODE,  0,  0,  0,  {0}},
};

const SH8601A_CMD_DESC SH8601A_OTP_WRITE[] =
{
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD0, 1000,   2,  {0x01}},
    {SH8601A_QSPI_INST_CMD_WRITE, 0xD0, 10, 2,  {0x00}},
    {SH8601A_QSPI_SEQ_FINISH_CODE,  0,  0,  0,  {0}},
};

static void SH8601A_Reg_Write(const SH8601A_CMD_DESC *cmd)
{
    uint16_t idx = 0;

    while (cmd[idx].instruction != SH8601A_QSPI_SEQ_FINISH_CODE)
    {
        uint8_t sdat[cmd[idx].wordcount + 4];

        sdat[0] = cmd[idx].instruction;

        sdat[1] = 0;
        sdat[2] = cmd[idx].index;        // Set in the middle 8 bits ADDR[15:8] of the 24 bits ADDR[23:0]
        sdat[3] = 0;

        for (uint16_t i = 0; i < cmd[idx].wordcount; i++)
        {
            sdat[i + 4] = cmd[idx].payload[i];
        }

        spic3_spi_write(sdat, sizeof(sdat));
        if (cmd[idx].delay != 0)
        {
#ifdef __RTTHREAD__
            rt_thread_mdelay(cmd[idx].delay);
#else
            platform_delay_ms(cmd[idx].delay);
#endif
        }

        idx++;
    }
}

void SH8601A_Init_Pre_OTP(void)
{
    //pull low RESX
    //power on VBAT: VBAT = 3.7V
    //power on VDDI: VDDI = 1.8V
    //pull high VCI_EN: enable VCI = 3.3V
    //delay 10ms
    //pull high RESX: IC reset
    //delay 10ms
    SH8601A_Reg_Write(SH8601A_PRE_OTP_POWERON_SEQ_CMD);
}

void SH8601A_Init_Post_OTP(void)
{
    //pull low RESX
    //power on VBAT: VBAT = 3.7V
    //power on VDDI: VDDI = 1.8V
    //pull high VCI_EN: enable VCI = 3.3V
    //delay 10ms
    //pull high RESX: IC reset
    //delay 10ms
    SH8601A_Reg_Write(SH8601A_POST_OTP_POWERON_SEQ_CMD);
}
void SH8601A_qspi_power_on(void)
{
    //pull low RESX
    //power on VBAT: VBAT = 3.7V
    //power on VDDI: VDDI = 1.8V
    //pull high VCI_EN: enable VCI = 3.3V
    //delay 10ms
    //pull high RESX: IC reset
    //delay 10ms
    SH8601A_Reg_Write(SH8601A_OTP_WRITE_POWERON);
}
void SH8601A_qspi_power_off(void)
{
    SH8601A_Reg_Write(SH8601A_POWEROFF_SEQ_CMD);
    //delay 100ms
    //pull low RESX
    //delay 10ms
    //pull low VCI_EN: disable VCI
    //power off VDDI
    //power off VBAT
}

void SH8601A_OTP_Write(void)
{
    /*********************************************
    * Register read:
    * Index: 0xCF (OTP_STATUS)
    * Para: 1 Byte read
    * Check BANK_CHECK_MCS[1:0]:
    * - 00h: No writen, 3 times writable
    * - 01h: 1 time written, 2 times writable
    * - 02h: 2 times written, 1 time writable
    * - 03h: 3 time written, no longer be written
    **********************************************/

    //power on VOTP: VOTP = 6V external supply
    //delay 20ms
    SH8601A_Reg_Write(SH8601A_OTP_WRITE);
    //power off VOTP
    //delay 20ms

    /*********************************************
    * OTP status verification:
    * Index: 0xCF (OTP_STATUS)
    * Para: 3 Bytes read
    * Check PRG_ERR_1:
    * - 0: OK
    * - 1: FAIL
           OTP rewrite(power off -> power on -> rewrite)
    * Check PRG_ERR_0:
    * - 0: OK
    * - 1: FAIL
           Not rewrite OTP
    **********************************************/

    /* Go to power off sequence */
}

static void rtl_SH8601A_qspi_cmd_param4(uint8_t cmd,
                                        uint8_t *data) //total 8 byte, first byte is 0x02
{
    uint8_t sdat[] = {0x02, 0x00, cmd, 0x00, data[0], data[1], data[2], data[3]};
    spic3_spi_write(sdat, sizeof(sdat));
}



static void rtl_SH8601A_qspi_enter_data_output_mode(uint32_t
                                                    len_byte) //total 4 byte, first byte is 0x32
{
    LCDC_AXIMUXMode(LCDC_FW_MODE);
    DBIC_SwitchMode(DBIC_USER_MODE);
    DBIC_SwitchDirect(DBIC_TMODE_TX);
    DBIC_CmdLength(1);
    DBIC_AddrLength(3);
    DBIC_TX_NDF(len_byte);

    DBIC->CTRLR0 &= ~(BIT_CMD_CH(3) | BIT_ADDR_CH(3) | BIT_DATA_CH(3));//SET CHANNEL NUM
    DBIC->CTRLR0 |= (BIT_CMD_CH(0) | BIT_ADDR_CH(0) | BIT_DATA_CH(2));

    /* must push cmd and address to handler before SPIC enable */
    LCDC_SPICCmd(0x32);
    LCDC_SPICAddr(0x002c00);


    DBIC->DMACR = 2;

    /* change this value can not influence the result. the wave is split into two periods. the first is 32 bytes. */
    DBIC->DMATDLR = 4; /* no any influence. */

    LCDC_AXIMUXMode(LCDC_HW_MODE);
}


void rtk_lcd_hal_set_window(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h)
{
    uint8_t data[4];
    uint16_t xEnd = xStart + w - 1;
    uint16_t yEnd = yStart + h - 1;
    data[0] = xStart >> 8;
    data[1] = xStart & 0xff;
    data[2] = xEnd >> 8;
    data[3] = xEnd & 0xff;
    rtl_SH8601A_qspi_cmd_param4(0x2A, data);


    data[0] = yStart >> 8;
    data[1] = yStart & 0xff;
    data[2] = yEnd >> 8;
    data[3] = yEnd & 0xff;
    rtl_SH8601A_qspi_cmd_param4(0x2B, data);

    uint32_t len_byte = (xEnd - xStart + 1) * (yEnd - yStart + 1) * OUTPUT_PIXEL_BYTES;
    rtl_SH8601A_qspi_enter_data_output_mode(len_byte);
}

void rtk_lcd_hal_update_framebuffer(uint8_t *buf, uint32_t len)
{
#if (DMA_LINKLIST == 0)
    GDMA_InitTypeDef LCDC_DMA_InitStruct = {0};
    LCDC_DMA_StructInit(&LCDC_DMA_InitStruct);
    LCDC_DMA_InitStruct.GDMA_ChannelNum          = LCDC_DMA_CHANNEL_NUM;
    LCDC_DMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    LCDC_DMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    LCDC_DMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    LCDC_DMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    LCDC_DMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_8;
    LCDC_DMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_8;
    LCDC_DMA_InitStruct.GDMA_SourceAddr          = (uint32_t)buf;
    LCDC_DMA_InitStruct.GDMA_Multi_Block_En     = 0;
    LCDC_DMA_Init(LCDC_DMA_CHANNEL_INDEX, &LCDC_DMA_InitStruct);
#else
    GDMA_InitTypeDef LCDC_DMA_InitStruct = {0};
    LCDC_DMA_StructInit(&LCDC_DMA_InitStruct);
    LCDC_DMA_InitStruct.GDMA_ChannelNum          = LCDC_DMA_CHANNEL_NUM;
    LCDC_DMA_InitStruct.GDMA_SourceInc           = DMA_SourceInc_Inc;
    LCDC_DMA_InitStruct.GDMA_DestinationInc      = DMA_DestinationInc_Fix;
    LCDC_DMA_InitStruct.GDMA_SourceDataSize      = GDMA_DataSize_Word;
    LCDC_DMA_InitStruct.GDMA_DestinationDataSize = GDMA_DataSize_Word;
    LCDC_DMA_InitStruct.GDMA_SourceMsize         = GDMA_Msize_8;
    LCDC_DMA_InitStruct.GDMA_DestinationMsize    = GDMA_Msize_8;
    LCDC_DMA_InitStruct.GDMA_SourceAddr          = 0;

    LCDC_DMA_InitStruct.GDMA_Multi_Block_Mode   =
        LLI_TRANSFER;//LLI_TRANSFER or LLI_WITH_CONTIGUOUS_SAR
    LCDC_DMA_InitStruct.GDMA_Multi_Block_En     = 1;
    LCDC_DMA_InitStruct.GDMA_Multi_Block_Struct  = LCDC_DMA_LINKLIST_REG_BASE + 0x50;
    LCDC_DMA_Init(LCDC_DMA_CHANNEL_INDEX, &LCDC_DMA_InitStruct);

    LCDC_SET_GROUP1_BLOCKSIZE(SH8601A_LCD_WIDTH * INPUT_PIXEL_BYTES);
    LCDC_SET_GROUP2_BLOCKSIZE(SH8601A_LCD_WIDTH * INPUT_PIXEL_BYTES);

    /*16 pixel aligned for GPU*/
    //uint32_t gpu_width = SH8601A_LCD_WIDTH;
    uint32_t gpu_width = ((SH8601A_LCD_WIDTH + 15) >> 4) << 4;
    /*16 pixel aligned for GPU*/
    LCDC_DMALLI_InitTypeDef LCDC_DMA_LLI_Init = {0};
    LCDC_DMA_LLI_Init.g1_source_addr = (uint32_t)buf;
    LCDC_DMA_LLI_Init.g1_sar_offset = gpu_width * INPUT_PIXEL_BYTES * 2;

    LCDC_DMA_LLI_Init.g2_source_addr = (uint32_t)(buf + gpu_width * INPUT_PIXEL_BYTES);
    LCDC_DMA_LLI_Init.g2_sar_offset = gpu_width * INPUT_PIXEL_BYTES * 2;
    LCDC_DMA_LinkList_Init(&LCDC_DMA_LLI_Init,
                           &LCDC_DMA_InitStruct);//LLI_TRANSFER or LLI_WITH_CONTIGUOUS_SAR
#endif

    LCDC_ClearDmaFifo();
    LCDC_ClearTxPixelCnt();

    LCDC_SwitchMode(LCDC_AUTO_MODE);
    LCDC_SwitchDirect(LCDC_TX_MODE);

    LCDC_SetTxPixelLen(len);

    LCDC_Cmd(ENABLE);
#if DMA_LINKLIST
    LCDC_DMA_MultiBlockCmd(ENABLE);
#endif
    LCDC_DMAChannelCmd(LCDC_DMA_CHANNEL_NUM, ENABLE);
    LCDC_DmaCmd(ENABLE);
#if (TE_VALID == 1)
    TEAR_CTR_t handler_reg_0x10 = {.d32 = LCDC_HANDLER->TEAR_CTR};
    handler_reg_0x10.b.bypass_t2w_delay = 0;
    handler_reg_0x10.b.t2w_delay = 0xfff;
    LCDC_HANDLER->TEAR_CTR = handler_reg_0x10.d32;
    LCDC_TeCmd(ENABLE);
#else
    LCDC_AutoWriteCmd(ENABLE);
#endif
    while ((LCDC_HANDLER->DMA_FIFO_CTRL & LCDC_DMA_ENABLE) != RESET)//wait dma finish
    {
        os_delay(1);
    }
    while (((LCDC_HANDLER->DMA_FIFO_OFFSET & LCDC_DMA_TX_FIFO_OFFSET) != RESET) &&
           (LCDC_HANDLER->TX_CNT == LCDC_HANDLER->TX_LEN));//wait lcd tx cnt finish
#if (TE_VALID == 1)
    LCDC_TeCmd(DISABLE);                            // disable Tear trigger auto_write_start
#endif
    LCDC_Cmd(DISABLE);
    LCDC_ClearDmaFifo();
    LCDC_ClearTxPixelCnt();
    LCDC_AXIMUXMode(LCDC_FW_MODE);
}

uint32_t rtk_lcd_hal_power_on(void)
{

    return 0;
}

uint32_t rtk_lcd_hal_power_off(void)
{

    return 0;
}

uint32_t rtk_lcd_hal_dlps_restore(void)
{
    return 0;
}

static void sh8601a_pad_config(void)
{
    Pad_Config(P16_6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P16_7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_3, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_4, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_5, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(P17_7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    Pad_Dedicated_Config(P16_6, ENABLE);
    Pad_Dedicated_Config(P16_7, ENABLE);
    Pad_Dedicated_Config(P17_0, ENABLE);
    Pad_Dedicated_Config(P17_1, ENABLE);
    Pad_Dedicated_Config(P17_2, ENABLE);
    Pad_Dedicated_Config(P17_3, ENABLE);
    Pad_Dedicated_Config(P17_4, ENABLE);
    Pad_Dedicated_Config(P17_5, ENABLE);
    Pad_Dedicated_Config(P17_6, ENABLE);
    Pad_Dedicated_Config(P17_7, ENABLE);

    drv_pin_mode(P13_7, PIN_MODE_OUTPUT);
    drv_pin_write(P13_7, 0); //im0
    drv_pin_mode(P13_6, PIN_MODE_OUTPUT);
    drv_pin_write(P13_6, 1); //im1

}

static void driver_ic_init(void)
{
    sh8601a_pad_config();

    LCDC_LCD_SET_RST(false);
    platform_delay_ms(100);
    LCDC_LCD_SET_RST(true);
    platform_delay_ms(50);
    LCDC_LCD_SET_RST(false);
    platform_delay_ms(50);

    LCDC_AXIMUXMode(LCDC_FW_MODE);
    DBIC_SwitchMode(DBIC_USER_MODE);
    DBIC_SwitchDirect(DBIC_TMODE_TX);

    SH8601A_Init_Post_OTP();
    //SH8601A_qspi_power_on();
}

void rtk_lcd_hal_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_DISP, APBPeriph_DISP_CLOCK_CLOCK, ENABLE);
    RCC_PeriphClockCmd(APBPeriph_MIPI_HOST, APBPeriph_MIPI_HOST_CLOCK, ENABLE);
    //sh8601a_pad_config();
    LCDC_InitTypeDef lcdc_init = {0};
    lcdc_init.LCDC_Interface = LCDC_IF_DBIC;
    lcdc_init.LCDC_PixelInputFarmat = LCDC_INPUT_ARGB8888;
    lcdc_init.LCDC_PixelOutpuFarmat = LCDC_OUTPUT_RGB888;
    lcdc_init.LCDC_PixelBitSwap = LCDC_SWAP_BYPASS; //lcdc_handler_cfg->LCDC_TeEn = LCDC_TE_DISABLE;
#if TE_VALID
    lcdc_init.LCDC_TeEn = DISABLE;
    lcdc_init.LCDC_TePolarity = LCDC_TE_EDGE_FALLING;
    lcdc_init.LCDC_TeInputMux = LCDC_TE_LCD_INPUT;
#endif
    lcdc_init.LCDC_DmaThreshold =
        8;    //only support threshold = 8 for DMA MSIZE = 8; the other threshold setting will be support later
    LCDC_Init(&lcdc_init);

//    LCDC_clk_src_sel(CKO1_PLL1_VCORE4, FROM_CLK_DISPLAY_SRC_MUX0, FRO_CLK_DISPLAY_SRC_MUX1,
//                     LCDC_DIV_DISABLE, LCDC_DIV_1_DIV);

    LCDC_DBICCfgTypeDef dbic_init = {0};
    dbic_init.DBIC_SPEED_SEL         = 2;

    dbic_init.DBIC_TxThr             = 0;//0 or 4
    dbic_init.DBIC_RxThr             = 0;
    dbic_init.SCPOL                  = DBIC_SCPOL_LOW;
    dbic_init.SCPH                   = DBIC_SCPH_1Edge;
    DBIC_Init(&dbic_init);

    LCDC_SwitchMode(LCDC_MANUAL_MODE);
    LCDC_SwitchDirect(LCDC_TX_MODE);
    LCDC_Cmd(ENABLE);

    driver_ic_init();
}

uint32_t rtk_lcd_hal_get_width(void)
{
    return SH8601A_LCD_WIDTH;
}
uint32_t rtk_lcd_hal_get_height(void)
{
    return SH8601A_LCD_HEIGHT;
}

uint32_t rtk_lcd_hal_get_pixel_bits(void)
{
    return SH8601A_LCD_BITS_PIXEL;
}

void rtk_lcd_hal_start_transfer(uint8_t *buf, uint32_t len)
{
    //todo
}
void rtk_lcd_hal_transfer_done(void)
{
    //todo
}
