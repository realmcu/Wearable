#ifndef _LCD_ST7789_240_8080_H_
#define _LCD_ST7789_240_8080_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

#define LCD_WIDTH                           240
#define LCD_HEIGHT                          240
#define PAGE_SWITCH_TIMER_INTERVAL          40
#define FEATURE_PSRAM                       0
#define LCD_DIRECTION_SELECTION_90_READINGS 1

#define RGB16BIT_565                        16
#define RGB24BIT_888                        24

#define FONT_BACK_64M                       1

void rtk_lcd_hal_init(void);
void rtk_lcd_hal_set_window(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h);
void rtk_lcd_hal_rect_fill(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h,
                           uint32_t color);
void rtk_lcd_hal_start_transfer(uint8_t *buf, uint32_t len);
void rtk_lcd_hal_transfer_done(void);

uint32_t rtk_lcd_hal_get_width(void);
uint32_t rtk_lcd_hal_get_height(void);

#ifdef __cplusplus
}
#endif

#endif /* _LCD_ST7789_240_8080_H_ */
