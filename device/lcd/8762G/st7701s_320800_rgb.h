#ifndef _LCD_ST7701S_360640_RGB_H_
#define _LCD_ST7701S_360640_RGB_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"


#define ST7701S_320800_LCD_WIDTH                   320
#define ST7701S_320800_LCD_HEIGHT                  800

#define ST7701S_DRV_PIXEL_BITS                     16



void st7701s_init(void);
void lcd_st7701s_init_framebuffer(uint8_t *buf, uint32_t len);
void lcd_st7701s_update_framebuffer(uint8_t *buf, uint32_t len);

void rtk_lcd_hal_init(void);
void rtk_lcd_hal_update_framebuffer(uint8_t *p_buf, uint32_t size);
void rtk_lcd_hal_set_window(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h);
void rtk_lcd_hal_start_transfer(uint8_t *buf, uint32_t len);
void rtk_lcd_hal_transfer_done(void);
uint32_t rtk_lcd_hal_get_width(void);
uint32_t rtk_lcd_hal_get_height(void);
uint32_t rtk_lcd_hal_get_pixel_bits(void);
uint32_t rtk_lcd_hal_power_off(void);
uint32_t rtk_lcd_hal_power_on(void);
uint32_t rtk_lcd_hal_dlps_restore(void);


#ifdef __cplusplus
}
#endif

#endif /* _LCD_ST7701S_360640_RGB_H_ */
