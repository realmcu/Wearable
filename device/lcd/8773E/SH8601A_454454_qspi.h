#ifndef _LCD_SH8601A_454_QSPI_H_
#define _LCD_SH8601A_454_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

#define SH8601A_MAX_PARA_COUNT                          (300)

#if defined RTL8763EP
#define TE_VALID                            0
#else
#define TE_VALID                            1
#endif
#define SH8601A_LCD_WIDTH                   454
#define SH8601A_LCD_HEIGHT                  454
#define SH8601A_DRV_PIXEL_BITS              32
#define INPUT_PIXEL_BYTES                   4
#if INPUT_PIXEL_BYTES == 3
#error "LCDC DMA doesn't allow 3 bytes input"
#endif
#define OUTPUT_PIXEL_BYTES                  3



typedef struct _SH8601A_CMD_DESC
{
    uint8_t instruction;
    uint8_t index;
    uint16_t delay;
    uint16_t wordcount;
    uint8_t  payload[SH8601A_MAX_PARA_COUNT];
} SH8601A_CMD_DESC;


void rtk_lcd_hal_set_window(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h);
void rtk_lcd_hal_init(void);
void rtk_lcd_hal_update_framebuffer(uint8_t *buf, uint32_t len);
void rtk_lcd_hal_clear_screen(uint32_t ARGB_color);

void rtk_lcd_hal_start_transfer(uint8_t *buf, uint32_t len);
void rtk_lcd_hal_transfer_done(void);
uint32_t rtk_lcd_hal_get_width(void);
uint32_t rtk_lcd_hal_get_height(void);
uint32_t rtk_lcd_hal_get_pixel_bits(void);
uint32_t rtk_lcd_hal_power_off(void);
uint32_t rtk_lcd_hal_power_on(void);
#ifdef __cplusplus
}
#endif

#endif /* _LCD_SH8601A_454_QSPI_H_ */
