#include "st7701s_480640_rgb.h"
#include "rtl_lcdc_edpi.h"
#include "rtl_spi.h"
#include "rtl_pinmux.h"
#include "rtl_gpio.h"
#include "rtl_rcc.h"
#include "utils.h"
#include "mem_config.h"
#include "rtl_gdma.h"

#define RTL87x2G_TYPEB      1

/**************************************
NOTE:VCI=3.3V,IOVCC=1.8V,
Display resolution:640*360
params->vertical_sync_active=4
params->vertical_backporch=20
params->vertical_frontporch=20
params->horizontal_sync_active=40
params->horizontal_backporch=150
params->horizontal_frontporch=150
params->RGB.PLL_CLOCK=(33)
**************************************/

#define LCDC_DATA15         P3_3
#define LCDC_DATA14         P3_2
#define LCDC_DATA13         P9_2
#define LCDC_DATA12         P4_7
#define LCDC_DATA11         P4_6

#define LCDC_DATA10         P4_5
#define LCDC_DATA9          P4_4
#define LCDC_DATA8          P1_2
#define LCDC_DATA7          P4_3
#define LCDC_DATA6          P4_2
#define LCDC_DATA5          P4_1

#define LCDC_DATA4          P4_0
#define LCDC_DATA3          P0_7
#define LCDC_DATA2          P0_6
#define LCDC_DATA1          P0_5
#define LCDC_DATA0          P0_4

#define LCDC_RGB_WRCLK      P0_2
#define LCDC_HSYNC          P1_5
#define LCDC_VSYNC          P0_1
#define LCDC_CSN_DE         P0_0

#if RTL87x2G_TYPEB
#define GPD0                P1_1
//#define GPD1                P9_7
#define GPD2                P2_2
#define GPD3                P2_3
#define SPI0_SCK_PIN                               GPD0
#define SPI0_MOSI_PIN                              P1_0
#define SPI0_CS_PIN                                GPD3
#define LCDC_RESET          GPD2
#else
#if 0
#define SPI0_SCK_PIN                               P9_0
#define SPI0_MOSI_PIN                              P1_3
#define SPI0_MISO_PIN                              P1_6
#define SPI0_CS_PIN                                P9_1
#define LCDC_RESET                                 P3_6
#else
#define SPI0_SCK_PIN                               P1_1
#define SPI0_MOSI_PIN                              P1_0
//#define SPI0_MISO_PIN                            P1_6
#define SPI0_CS_PIN                                P2_3
#define LCDC_RESET                                 P2_2
#endif
#endif


#define LCDC_DMA_CHANNEL_NUM              0
#define LCDC_DMA_CHANNEL_INDEX            LCDC_DMA_Channel0

#define TRANSFER_DMA_CHANNEL_NUM              0
#define TRANSFER_DMA_CHANNEL_INDEX            GDMA_Channel0


static void write_command(uint32_t cmd)
{
    platform_delay_us(200);
    SPI_SendData(SPI0, cmd);
}
static void write_data(uint32_t data)
{
    uint32_t send = (data | 0x100);
    SPI_SendData(SPI0, send);
}

static void st7701s_reset_high(void)
{
    GPIO_WriteBit(GPIO_GetPort(LCDC_RESET), GPIO_GetPin(LCDC_RESET), 1);
}

static void st7701s_reset_low(void)
{
    GPIO_WriteBit(GPIO_GetPort(LCDC_RESET), GPIO_GetPin(LCDC_RESET), 0);
}

//--------------------initial code-----------------------------------------//
static void st7701s_gpio_init(void)
{
    Pad_Config(LCDC_RESET, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);

    Pinmux_Config(LCDC_RESET, DWGPIO);

    RCC_PeriphClockCmd(APBPeriph_GPIOA, APBPeriph_GPIOA_CLOCK, ENABLE);
    RCC_PeriphClockCmd(APBPeriph_GPIOB, APBPeriph_GPIOB_CLOCK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin    = GPIO_GetPin(LCDC_RESET);
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
    GPIO_Init(GPIO_GetPort(LCDC_RESET), &GPIO_InitStruct);
    st7701s_reset_high();
}

static void st7701s_spi_init(void)
{
    Pad_Config(SPI0_MOSI_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(SPI0_SCK_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(SPI0_CS_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_ENABLE, PAD_OUT_HIGH);

    Pinmux_Deinit(SPI0_MOSI_PIN);
    Pinmux_Deinit(SPI0_SCK_PIN);
    Pinmux_Deinit(SPI0_CS_PIN);

    Pinmux_Config(SPI0_CS_PIN, SPI0_CSN_0_MASTER);
    Pinmux_Config(SPI0_SCK_PIN, SPI0_CLK_MASTER);
    Pinmux_Config(SPI0_MOSI_PIN, SPI0_MO_MASTER);

    RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE);

    SPI_InitTypeDef spi_init_struct;
    SPI_StructInit(&spi_init_struct);
    spi_init_struct.SPI_CPOL = SPI_CPOL_Low;
    spi_init_struct.SPI_CPHA = SPI_CPHA_1Edge;
    spi_init_struct.SPI_Direction = SPI_Direction_TxOnly;
    spi_init_struct.SPI_DataSize = SPI_DataSize_9b;
    spi_init_struct.SPI_BaudRatePrescaler = 64;
    spi_init_struct.SPI_FrameFormat = SPI_Frame_Motorola;
    SPI_Init(SPI0, &spi_init_struct);

    SPI_Cmd(SPI0, ENABLE);

}

static void st7701s_dma_init(uint8_t *init_buffer)
{
    LCDC_DMA_InitTypeDef LCDC_DMA_InitStruct = {0};
    LCDC_DMA_StructInit(&LCDC_DMA_InitStruct);
    LCDC_DMA_InitStruct.LCDC_DMA_ChannelNum          = LCDC_DMA_CHANNEL_NUM;
    LCDC_DMA_InitStruct.LCDC_DMA_DIR                 = LCDC_DMA_DIR_PeripheralToMemory;
    LCDC_DMA_InitStruct.LCDC_DMA_SourceInc           = LCDC_DMA_SourceInc_Inc;
    LCDC_DMA_InitStruct.LCDC_DMA_DestinationInc      = LCDC_DMA_DestinationInc_Fix;
    LCDC_DMA_InitStruct.LCDC_DMA_SourceDataSize      = LCDC_DMA_DataSize_Word;
    LCDC_DMA_InitStruct.LCDC_DMA_DestinationDataSize = LCDC_DMA_DataSize_Word;
    LCDC_DMA_InitStruct.LCDC_DMA_SourceMsize         = LCDC_DMA_Msize_16;
    LCDC_DMA_InitStruct.LCDC_DMA_DestinationMsize    = LCDC_DMA_Msize_16;
    LCDC_DMA_InitStruct.LCDC_DMA_SourceAddr          = 0;

    LCDC_DMA_InitStruct.LCDC_DMA_Multi_Block_Mode   =
        LLI_TRANSFER;//LLI_TRANSFER or LLI_WITH_CONTIGUOUS_SAR
    LCDC_DMA_InitStruct.LCDC_DMA_Multi_Block_En     = 1;
    LCDC_DMA_InitStruct.LCDC_DMA_Multi_Block_Struct  = LCDC_DMA_LINKLIST_REG_BASE + 0x50;
    LCDC_DMA_Init(LCDC_DMA_CHANNEL_INDEX, &LCDC_DMA_InitStruct);

    LCDC_SET_GROUP1_BLOCKSIZE(ST7701S_480640_LCD_WIDTH * ST7701S_DRV_PIXEL_BITS / 8);
    LCDC_SET_GROUP2_BLOCKSIZE(ST7701S_480640_LCD_WIDTH * ST7701S_DRV_PIXEL_BITS / 8);

    LCDC_DMALLI_InitTypeDef LCDC_DMA_LLI_Init = {0};

    LCDC_DMA_LLI_Init.g1_source_addr = (uint32_t)init_buffer;
    LCDC_DMA_LLI_Init.g2_source_addr = (uint32_t)((uint32_t)init_buffer + ST7701S_480640_LCD_WIDTH *
                                                  ST7701S_DRV_PIXEL_BITS / 8);
    LCDC_DMA_LLI_Init.g1_sar_offset = ST7701S_480640_LCD_WIDTH * ST7701S_DRV_PIXEL_BITS / 8 * 2;
    LCDC_DMA_LLI_Init.g2_sar_offset = ST7701S_480640_LCD_WIDTH * ST7701S_DRV_PIXEL_BITS / 8 * 2;

    LCDC_DMA_Infinite_Buf_Update((uint8_t *)init_buffer,
                                 (uint8_t *)init_buffer + ST7701S_480640_LCD_WIDTH *
                                 ST7701S_DRV_PIXEL_BITS / 8);
    LCDC_DMA_LinkList_Init(&LCDC_DMA_LLI_Init,
                           &LCDC_DMA_InitStruct);//LLI_TRANSFER or LLI_WITH_CONTIGUOUS_SAR

    LCDC_ClearDmaFifo();
    LCDC_ClearTxPixelCnt();

    LCDC_SwitchMode(LCDC_AUTO_MODE);
    LCDC_SwitchDirect(LCDC_TX_MODE);

    LCDC_SetTxPixelLen(ST7701S_480640_LCD_WIDTH * ST7701S_480640_LCD_HEIGHT);

    LCDC_Cmd(ENABLE);

    LCDC_DMA_SetSourceAddress(LCDC_DMA_CHANNEL_INDEX, (uint32_t)init_buffer);

    LCDC_DMA_MultiBlockCmd(ENABLE);

    LCDC_DMAChannelCmd(LCDC_DMA_CHANNEL_NUM, ENABLE);

    LCDC_DmaCmd(ENABLE);
    LCDC_AutoWriteCmd(ENABLE);
}

static void lcd_pad_and_clk_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_DISP, APBPeriph_DISP_CLOCK, ENABLE);

    //from XTAL SOURCE = 40M
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.disp_ck_en = 1;
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.disp_func_en = 1;
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.r_disp_mux_clk_cg_en = 1;

    //From PLL1, SOURCE = 125M
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.r_disp_div_en = 1;
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.r_disp_clk_src_sel0 = 0; //pll1_peri(0) or pll2(1, pll2 = 160M)
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.r_disp_clk_src_sel1 = 1; //pll(1) or xtal(0)
    PERIBLKCTRL_PERI_CLK->u_324.BITS_324.r_disp_div_sel = 0; //div

    Pad_Config(LCDC_DATA0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA2, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA3, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA4, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA5, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA6, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA7, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA8, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA9, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA10, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA11, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA12, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA13, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA14, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_DATA15, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    Pad_Config(LCDC_RGB_WRCLK, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(LCDC_HSYNC, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_VSYNC, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);
    Pad_Config(LCDC_CSN_DE, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_HIGH);

    extern void Pad_Dedicated_Config(uint8_t Pin_Num, FunctionalState Status);
    Pad_Dedicated_Config(LCDC_DATA0, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA1, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA2, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA3, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA4, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA5, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA6, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA7, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA8, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA9, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA10, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA11, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA12, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA13, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA14, ENABLE);
    Pad_Dedicated_Config(LCDC_DATA15, ENABLE);

    Pad_Dedicated_Config(LCDC_RGB_WRCLK, ENABLE);
    Pad_Dedicated_Config(LCDC_HSYNC, ENABLE);
    Pad_Dedicated_Config(LCDC_VSYNC, ENABLE);
    Pad_Dedicated_Config(LCDC_CSN_DE, ENABLE);

    st7701s_gpio_init();
    platform_delay_ms(1);
    st7701s_spi_init();
}

#if 1
/* Private define ------------------------------------------------------------*/
#define OUTPUT_PIN          P9_7
#define GPIO_PIN            GPIO_GetPin(OUTPUT_PIN)
#define GPIO_PORT           GPIO_GetPort(OUTPUT_PIN)

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialization of pinmux settings and pad settings.
  * @param  None.
  * @retval None.
  */
void board_enable_init(void)
{
    Pad_Config(OUTPUT_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);

    Pinmux_Config(OUTPUT_PIN, DWGPIO);
}

/**
  * @brief  Initialize GPIO peripheral.
  * @param  None.
  * @retval None.
  */
void enable_lcd_init(void)
{
    board_enable_init();

    /* GPIO peripheral clock enable */
    RCC_PeriphClockCmd(APBPeriph_GPIOA, APBPeriph_GPIOA_CLOCK, ENABLE);

    /* Configure GPIO_PIN_OUTPUT as output mode */
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin    = GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode   = GPIO_DIR_OUT;
    GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
    GPIO_Init(GPIO_PORT, &GPIO_InitStruct);

    GPIO_WriteBit(GPIO_PORT, GPIO_PIN, (BitAction)(1));
}
#endif

void rtk_lcd_hal_init(void)
{
    lcd_pad_and_clk_init();

    LCDC_InitTypeDef lcdc_init = {0};
    lcdc_init.LCDC_Interface = LCDC_IF_DPI;
    lcdc_init.LCDC_PixelInputFarmat = LCDC_INPUT_RGB565;
    lcdc_init.LCDC_PixelOutpuFarmat = LCDC_OUTPUT_RGB565;
    lcdc_init.LCDC_PixelBitSwap = LCDC_SWAP_BYPASS; //lcdc_handler_cfg->LCDC_TeEn = LCDC_TE_DISABLE;
    lcdc_init.LCDC_GroupSel = 1;

    lcdc_init.LCDC_DmaThreshold =
        112;    //only support threshold = 8 for DMA MSIZE = 8; the other threshold setting will be support later
    lcdc_init.LCDC_InfiniteModeEn = 1;
    LCDC_Init(&lcdc_init);

    // 83.5 fps
//    uint32_t VSA = 8, VFP = 10, VBP = 10, VACT = ST7701S_480640_LCD_HEIGHT;  // 360
//    uint32_t HSA = 8, HFP = 10, HBP = 10,  HACT = ST7701S_480640_LCD_WIDTH;  // 640

    // 77.1 fps
    uint32_t VSA = 10, VFP = 20, VBP = 20, VACT = ST7701S_480640_LCD_HEIGHT;
    uint32_t HSA = 10, HFP = 20, HBP = 20,  HACT = ST7701S_480640_LCD_WIDTH;

    LCDC_eDPICfgTypeDef eDPICfg;//
    eDPICfg.eDPI_ClockDiv = 0x4;

    eDPICfg.eDPI_HoriSyncWidth = HSA;
    eDPICfg.eDPI_VeriSyncHeight = VSA;
    eDPICfg.eDPI_AccumulatedHBP = HSA + HBP;
    eDPICfg.eDPI_AccumulatedVBP = VSA + VBP;
    eDPICfg.eDPI_AccumulatedActiveW = HSA + HBP + HACT;
    eDPICfg.eDPI_AccumulatedActiveH = VSA + VBP + VACT;
    eDPICfg.eDPI_TotalWidth = HSA + HBP + HACT + HFP;
    eDPICfg.eDPI_TotalHeight = VSA + VBP + VACT + VFP;
    eDPICfg.eDPI_HoriSyncPolarity = 0;
    eDPICfg.eDPI_VeriSyncPolarity = 0;
    eDPICfg.eDPI_DataEnPolarity = 1;
    eDPICfg.eDPI_LineIntMask = 1;
    eDPICfg.eDPI_ColorMap = EDPI_PIXELFORMAT_RGB565_1;//for RGB888
    eDPICfg.eDPI_OperateMode = 0;//video mode
    eDPICfg.eDPI_LcdArc = 0;
    eDPICfg.eDPI_ShutdnPolarity = 0;
    eDPICfg.eDPI_ColorModePolarity = 0;
    eDPICfg.eDPI_ShutdnEn = 0;
    eDPICfg.eDPI_ColorModeEn = 0;
    eDPICfg.eDPI_UpdateCfgEn = 0;
    eDPICfg.eDPI_TearReq = 0;
    eDPICfg.eDPI_Halt = 0;
    eDPICfg.eDPI_CmdMaxLatency = 0;//todo
    eDPICfg.eDPI_LineBufferPixelThreshold = eDPICfg.eDPI_TotalWidth / 2;

    EDPI_Init(&eDPICfg);

    st7701s_reset_high();
    platform_delay_ms(10);
    st7701s_reset_low();
    platform_delay_ms(50);
    st7701s_reset_high();
    platform_delay_ms(120);
    //*******************************/
#include "st7701s_480640_rgb565.txt"
    uint8_t *pixel = (uint8_t *)SPIC1_ADDR;
    uint16_t *rgb = (uint16_t *)pixel;
    for (uint32_t i = 0; i < ST7701S_480640_LCD_WIDTH * ST7701S_480640_LCD_HEIGHT * 2; i = i + 2)
    {
        pixel[i] = 0x00;
        pixel[i + 1] = 0x00;
    }
    for (uint32_t y = 0; y < 320; y++)
    {
        for (uint32_t x = 60; x < 420; x++)
        {
            uint32_t offset = y * 420 + x;
            rgb[offset] = 0xF800;
        }
    }
    for (uint32_t y = 320; y < 640; y++)
    {
        for (uint32_t x = 60; x < 420; x++)
        {
            uint32_t offset = y * 420 + x;
            rgb[offset] = 0x001F;
        }
    }
//      memset((void*)pixel, 0xff, 640 * 360 *2);
//      memset((void*)pixel, 0x00, 320 * 360 *2);
//      memset((void*)pixel, 0xff, 50 * 360 *2);
    rtk_lcd_hal_update_framebuffer((uint8_t *)SPIC1_ADDR,
                                   ST7701S_480640_LCD_WIDTH * ST7701S_480640_LCD_HEIGHT);

    platform_delay_ms(300);
    enable_lcd_init();
    platform_delay_ms(300);
}

static bool flush_first = true;

void rtk_lcd_hal_update_framebuffer(uint8_t *buf, uint32_t len)
{
    if (flush_first == true)
    {
        st7701s_dma_init(buf);
        flush_first = false;
    }
    else
    {
        LCDC_DMA_Infinite_Buf_Update(buf,
                                     (uint32_t)buf + ST7701S_480640_LCD_WIDTH * ST7701S_DRV_PIXEL_BITS / 8);
    }
}

void rtk_lcd_hal_start_transfer(uint8_t *buf, uint32_t len)
{
    return;//todo by howie
}

void rtk_lcd_hal_transfer_done(void)
{
    return;//todo by howie
}

void rtk_lcd_hal_set_window(uint16_t xStart, uint16_t yStart, uint16_t w, uint16_t h)
{
    return;//todo by howie
}

uint32_t rtk_lcd_hal_get_width(void)
{
    return ST7701S_480640_LCD_HEIGHT;
}

uint32_t rtk_lcd_hal_get_height(void)
{
//    return ST7701S_480640_LCD_WIDTH;
    return 360;
}

uint32_t rtk_lcd_hal_get_pixel_bits(void)
{
    return ST7701S_DRV_PIXEL_BITS;
}
uint32_t rtk_lcd_hal_power_off(void)
{
    return 0;
}
uint32_t rtk_lcd_hal_power_on(void)
{
    return 0;
}
uint32_t rtk_lcd_hal_dlps_restore(void)
{
    return 0;
}
