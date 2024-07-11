#ifndef _LCD_ST7789_240_SPI_H_
#define _LCD_ST7789_240_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"


#define ST7789_LCD_WIDTH                  240
#define ST7789_LCD_HEIGHT                 320
#define ST7789_SEC_HEIGHT                 10

#define TOTAL_SECTION_COUNT              (ST7789_LCD_HEIGHT / ST7789_SEC_HEIGHT + ((ST7789_LCD_HEIGHT % ST7789_SEC_HEIGHT)?1:0))

#define ST7789_DRV_PIXEL_BITS             16

#define INPUT_PIXEL_BYTES                  2
#define OUTPUT_PIXEL_BYTES                 2


void rtk_lcd_hal_init(void);
void rtk_lcd_hal_set_window(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h);

void rtk_lcd_hal_update_framebuffer(uint8_t *buf, uint32_t len);
void rtk_lcd_hal_rect_fill(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h,
                           uint32_t color);
void rtk_lcd_hal_start_transfer(uint8_t *buf, uint32_t len);
void rtk_lcd_hal_transfer_done(void);


uint32_t rtk_lcd_hal_get_width(void);
uint32_t rtk_lcd_hal_get_height(void);
uint32_t rtk_lcd_hal_get_pixel_bits(void);


#ifdef __cplusplus
}
#endif

#endif /* _LCD_ST7789_240_SPI_H_ */
