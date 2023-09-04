/**
*********************************************************************************************************
*               Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_PPEV2.c
* @brief    This file provides all the the PPE 2.0 firmware functions.
* @details
* @author   feihu wang
* @date     2023-07-05
* @version  v0.1
*********************************************************************************************************
*/

#include "rtl_PPEV2.h"
#include "string.h"


#define USE_INTERNAL_MAT     0
#define ABS(x)               (((x) < 0)    ? -(x) :  (x))
#define EPS                  2.2204460492503131e-14

static void ppe_get_identity(ppe_matrix_t *matrix);
#if USE_INTERNAL_MAT
static void ppe_translate(float x, float y, ppe_matrix_t *matrix);
static void ppe_scale(float scale_x, float scale_y, ppe_matrix_t *matrix);
static void ppe_rotate(float degrees, ppe_matrix_t *matrix);
static void ppe_perspective(float px, float py, ppe_matrix_t *matrix);
static int  ppe_get_transform_matrix(ppe_point4_t src, ppe_point4_t dst, ppe_matrix_t *mat);
static void matrix_inverse(ppe_matrix_t *matrix);
static void multiply(ppe_matrix_t *matrix, ppe_matrix_t *mult);
#endif
static bool inv_matrix2complement(ppe_matrix_t *matrix, uint32_t *comp);

void PPEV2_CLK_ENABLE(FunctionalState NewState)
{

    if (NewState != ENABLE)
    {
        /*clock disable*/
        uint32_t reg_value = *(uint32_t *)(0x40000234UL);
        reg_value &= ~(BIT26 | BIT27);
        *(uint32_t *)(0x40000234UL) = reg_value;

        /*function disable*/
        reg_value = *(uint32_t *)(0x40000210UL);
        reg_value &= ~(BIT24);
        *(uint32_t *)(0x40000210UL) = reg_value;
    }
    else
    {
        /*clock enable*/
        uint32_t reg_value = *(uint32_t *)(0x40000234UL);
        reg_value |= (BIT26 | BIT27);
        *(uint32_t *)(0x40000234UL) = reg_value;

        /*function enable*/
        reg_value = *(uint32_t *)(0x40000210UL);
        reg_value |= (BIT24);
        *(uint32_t *)(0x40000210UL) = reg_value;
    }
}

/*enable PPEV2 FSM*/
void PPEV2_Cmd(FunctionalState NewState)
{
    PPEV2_REG_GLB_STATUS_t ppev2_reg_glb_status_0x00 = {.d32 = PPEV2->REG_GLB_STATUS};

    if (NewState == DISABLE)
    {
        ppev2_reg_glb_status_0x00.b.run_state = 0x0;
    }
    else
    {
        ppev2_reg_glb_status_0x00.b.run_state = 0x1;
    }

    PPEV2->REG_GLB_STATUS = ppev2_reg_glb_status_0x00.d32;
}

void PPEV2_Init(PPEV2_Init_Typedef *PPEV2_Init_Struct)
{
    PPEV2_Cmd(DISABLE);

    PPEV2_REG_LD_CFG_t ppev2_reg_ld_cfg_0x08  = {.d32 = PPEV2->REG_LD_CFG};
    ppev2_reg_ld_cfg_0x08.b.auto_clr          = PPEV2_Init_Struct->SetValid_AutoClear;
    PPEV2->REG_LD_CFG                         = ppev2_reg_ld_cfg_0x08.d32;

    PPEV2_REG_LLP_t ppev2_reg_llp_0x10        = {.d32 = PPEV2->REG_LLP};
    ppev2_reg_llp_0x10.b.llp                  = PPEV2_Init_Struct->LLP >> 2;
    PPEV2->REG_LLP                            = ppev2_reg_llp_0x10.d32;

    PPEV2_REG_SECURE_t  ppev2_reg_secure_0x18 = {.d32 = PPEV2->REG_SECURE};
    ppev2_reg_secure_0x18.b.secure            = PPEV2_Init_Struct->Secure_En;
    PPEV2->REG_SECURE                         = ppev2_reg_secure_0x18.d32;
}

void PPEV2_ResultLayer_Init(PPEV2_ResultLayer_Init_Typedef *PPEV2_ResultLyaer_Init_Struct)
{
    PPEV2_REG_LYR0_ADDR_t ppev2_reg_lyr0_addr_0x60 = {.d32 = PPEV2_ResultLayer->REG_LYR0_ADDR};
    ppev2_reg_lyr0_addr_0x60.b.addr                = PPEV2_ResultLyaer_Init_Struct->Layer_Address;
    PPEV2_ResultLayer->REG_LYR0_ADDR               = ppev2_reg_lyr0_addr_0x60.d32;

    PPEV2_REG_CANVAS_SIZE_t ppev2_reg_canvas_size_0x68 = {.d32 = PPEV2_ResultLayer->REG_CANVAS_SIZE};
    ppev2_reg_canvas_size_0x68.b.canvas_width          = PPEV2_ResultLyaer_Init_Struct->Canvas_Width;
    ppev2_reg_canvas_size_0x68.b.canvas_height         = PPEV2_ResultLyaer_Init_Struct->Canvas_Height;
    PPEV2_ResultLayer->REG_CANVAS_SIZE                 = ppev2_reg_canvas_size_0x68.d32;

    PPEV2_REG_LYR0_PIC_CFG_t  ppev2_reg_lyr0_pic_cfg_0x6C = {.d32 = PPEV2_ResultLayer->REG_LYR0_PIC_CFG};
    ppev2_reg_lyr0_pic_cfg_0x6C.b.format                  = PPEV2_ResultLyaer_Init_Struct->Color_Format;
    ppev2_reg_lyr0_pic_cfg_0x6C.b.line_length             = PPEV2_ResultLyaer_Init_Struct->Line_Length;
    PPEV2_ResultLayer->REG_LYR0_PIC_CFG                   = ppev2_reg_lyr0_pic_cfg_0x6C.d32;

    PPEV2_REG_BACKGROUND_t ppev2_reg_background_0x70 = {.d32 = PPEV2_ResultLayer->REG_BACKGROUND};
    ppev2_reg_background_0x70.b.background           = PPEV2_ResultLyaer_Init_Struct->BackGround;
    PPEV2_ResultLayer->REG_BACKGROUND                = ppev2_reg_background_0x70.d32;

    PPEV2_REG_LYR0_BUS_CFG_t ppev2_reg_lyr0_bus_cfg_0x74 = {.d32 = PPEV2_ResultLayer->REG_LYR0_BUS_CFG};
    ppev2_reg_lyr0_bus_cfg_0x74.b.incr                   = PPEV2_ResultLyaer_Init_Struct->LayerBus_Inc;
    PPEV2_ResultLayer->REG_LYR0_BUS_CFG                  = ppev2_reg_lyr0_bus_cfg_0x74.d32;

    PPEV2_REG_LYR0_HS_CFG_t ppev2_reg_lyr0_hs_cfg_0x78 = {.d32 = PPEV2_ResultLayer->REG_LYR0_HS_CFG};
    ppev2_reg_lyr0_hs_cfg_0x78.b.hs_en                 =
        PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_En;
    ppev2_reg_lyr0_hs_cfg_0x78.b.hw_index              =
        PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_Index;
    ppev2_reg_lyr0_hs_cfg_0x78.b.polar                 =
        PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_Polarity;
    ppev2_reg_lyr0_hs_cfg_0x78.b.msize_log             =
        PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_MsizeLog;
    PPEV2_ResultLayer->REG_LYR0_HS_CFG                 = ppev2_reg_lyr0_hs_cfg_0x78.d32;

    PPEV2_REG_LD_CFG_t ppev2_reg_ld_cfg_0x08  = {.d32 = PPEV2->REG_LD_CFG};
    ppev2_reg_ld_cfg_0x08.b.reload_en        &= ~BIT0;
    ppev2_reg_ld_cfg_0x08.b.reload_en        |= PPEV2_ResultLyaer_Init_Struct->MultiFrame_Reload_En;
    PPEV2->REG_LD_CFG                         =    ppev2_reg_ld_cfg_0x08.d32;

    PPEV2_REG_LL_CFG_t ppev2_reg_ll_cfg_0x0c = {.d32 = PPEV2->REG_LL_CFG};
    ppev2_reg_ll_cfg_0x0c.b.ll_en           &= ~BIT0;
    ppev2_reg_ll_cfg_0x0c.b.ll_en           |= PPEV2_ResultLyaer_Init_Struct->MultiFrame_LLP_En;
    PPEV2->REG_LL_CFG                        = ppev2_reg_ll_cfg_0x0c.d32;
}

void PPEV2_InputLayer_Init(PPEV2_INPUT_LAYER_INDEX intput_layer_index, \
                           PPEV2_InputLayer_Init_Typedef *PPEV2_InputLayer_Init_Struct)
{
    PPEV2_Input_Layer_Typedef *input_layer;
    switch (intput_layer_index)
    {
    case PPEV2_INPUT_1:
        {
            input_layer = PPEV2_InputLayer1;
            break;
        }
    case PPEV2_INPUT_2:
        {
            input_layer = PPEV2_InputLayer2;
            break;
        }
    case PPEV2_INPUT_3:
        {
            input_layer = PPEV2_InputLayer3;
            break;
        }
    case PPEV2_INPUT_4:
        {
            input_layer = PPEV2_InputLayer4;
            break;
        }
    }

    PPEV2_REG_LYRx_ADDR_t ppev2_reg_lyrx_addr_t = {.d32 = input_layer->REG_LYRx_ADDR};
    ppev2_reg_lyrx_addr_t.b.addr                = PPEV2_InputLayer_Init_Struct->Layer_Address;
    input_layer->REG_LYRx_ADDR                  = ppev2_reg_lyrx_addr_t.d32;

    PPEV2_REG_LYRx_PIC_SIZE_t ppev2_reg_lyrx_pic_size_t = {.d32 = input_layer->REG_LYRx_PIC_SIZE};
    ppev2_reg_lyrx_pic_size_t.b.pic_width               = PPEV2_InputLayer_Init_Struct->Pic_Width;
    ppev2_reg_lyrx_pic_size_t.b.pic_height              = PPEV2_InputLayer_Init_Struct->Pic_Height;
    input_layer->REG_LYRx_PIC_SIZE                      = ppev2_reg_lyrx_pic_size_t.d32;

    PPEV2_REG_LYRx_PIC_CFG_t ppev2_reg_lyrx_pic_cfg_t     = {.d32 = input_layer->REG_LYRx_PIC_CFG};
    ppev2_reg_lyrx_pic_cfg_t.b.line_length                = PPEV2_InputLayer_Init_Struct->Line_Length;
    ppev2_reg_lyrx_pic_cfg_t.b.color_key_mode             =
        PPEV2_InputLayer_Init_Struct->Color_Key_Mode;
    ppev2_reg_lyrx_pic_cfg_t.b.pic_src                    = PPEV2_InputLayer_Init_Struct->Pixel_Source;
    ppev2_reg_lyrx_pic_cfg_t.b.format                     =
        PPEV2_InputLayer_Init_Struct->Pixel_Color_Format;
    ppev2_reg_lyrx_pic_cfg_t.b.input_lyr_read_matrix_size =
        PPEV2_InputLayer_Init_Struct->Read_Matrix_Size;
    input_layer->REG_LYRx_PIC_CFG                         = ppev2_reg_lyrx_pic_cfg_t.d32;

    PPEV2_REG_LYRx_FIXED_COLOR_t ppev2_reg_lyrx_fixed_color_t = {.d32 = input_layer->REG_LYRx_FIXED_COLOR};
    ppev2_reg_lyrx_fixed_color_t.b.const_pixel                =
        PPEV2_InputLayer_Init_Struct->Const_Pixel;
    input_layer->REG_LYRx_FIXED_COLOR                         = ppev2_reg_lyrx_fixed_color_t.d32;

    PPEV2_REG_LYRx_BUS_CFG_t ppev2_reg_lyrx_bus_cfg_t = {.d32 = input_layer->REG_LYRx_BUS_CFG};
    ppev2_reg_lyrx_bus_cfg_t.b.incr                   = PPEV2_InputLayer_Init_Struct->LayerBus_Inc;
    input_layer->REG_LYRx_BUS_CFG                     = ppev2_reg_lyrx_bus_cfg_t.d32;

    PPEV2_REG_LYRx_HS_CFG_t ppev2_reg_lyrx_hs_cfg_t = {.d32 = input_layer->REG_LYRx_HS_CFG};
    ppev2_reg_lyrx_hs_cfg_t.b.hs_en                 =
        PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_En;
    ppev2_reg_lyrx_hs_cfg_t.b.hw_index              =
        PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_Index;
    ppev2_reg_lyrx_hs_cfg_t.b.msize_log             =
        PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_MsizeLog;
    ppev2_reg_lyrx_hs_cfg_t.b.polar                 =
        PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_Polarity;
    input_layer->REG_LYRx_HS_CFG                    = ppev2_reg_lyrx_hs_cfg_t.d32;

    PPEV2_REG_LYRx_WIN_MIN_t ppev2_reg_lyrx_win_min_t = {.d32 = input_layer->REG_LYRx_WIN_MIN};
    ppev2_reg_lyrx_win_min_t.b.win_x_min              = PPEV2_InputLayer_Init_Struct->Layer_Window_Xmin;
    ppev2_reg_lyrx_win_min_t.b.win_y_min              = PPEV2_InputLayer_Init_Struct->Layer_Window_Ymin;
    input_layer->REG_LYRx_WIN_MIN                     = ppev2_reg_lyrx_win_min_t.d32;

    PPEV2_REG_LYRx_WIN_MAX_t ppev2_reg_lyrx_win_max_t = {.d32 = input_layer->REG_LYRx_WIN_MAX};
    ppev2_reg_lyrx_win_max_t.b.win_x_max              = PPEV2_InputLayer_Init_Struct->Layer_Window_Xmax;
    ppev2_reg_lyrx_win_max_t.b.win_y_max              = PPEV2_InputLayer_Init_Struct->Layer_Window_Ymax;
    input_layer->REG_LYRx_WIN_MAX                     = ppev2_reg_lyrx_win_max_t.d32;

    PPEV2_REG_LYRx_KEY_MIN_t ppev2_reg_lyrx_key_min_t = {.d32 = input_layer->REG_LYRx_KEY_MIN};
    ppev2_reg_lyrx_key_min_t.b.color_key_min          = PPEV2_InputLayer_Init_Struct->Color_Key_Min;
    input_layer->REG_LYRx_KEY_MIN                     = ppev2_reg_lyrx_key_min_t.d32;

    PPEV2_REG_LYRx_KEY_MAX_t ppev2_reg_lyrx_key_max_t = {.d32 = input_layer->REG_LYRx_KEY_MAX};
    ppev2_reg_lyrx_key_max_t.b.color_key_max          = PPEV2_InputLayer_Init_Struct->Color_Key_Max;
    input_layer->REG_LYRx_KEY_MAX                     = ppev2_reg_lyrx_key_max_t.d32;

    input_layer->REG_LYRx_TRANS_MATRIX_E11            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E11;
    input_layer->REG_LYRx_TRANS_MATRIX_E12            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E12;
    input_layer->REG_LYRx_TRANS_MATRIX_E13            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E13;
    input_layer->REG_LYRx_TRANS_MATRIX_E21            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E21;
    input_layer->REG_LYRx_TRANS_MATRIX_E22            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E22;
    input_layer->REG_LYRx_TRANS_MATRIX_E23            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E23;
    input_layer->REG_LYRx_TRANS_MATRIX_E31            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E31;
    input_layer->REG_LYRx_TRANS_MATRIX_E32            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E32;
    input_layer->REG_LYRx_TRANS_MATRIX_E33            =
        PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E33;

    PPEV2_REG_LD_CFG_t ppev2_reg_ld_cfg_0x08 = {.d32 = PPEV2->REG_LD_CFG};
    ppev2_reg_ld_cfg_0x08.b.reload_en       &= ~BIT(intput_layer_index);
    ppev2_reg_ld_cfg_0x08.b.reload_en       |= (PPEV2_InputLayer_Init_Struct->MultiFrame_Reload_En <<
                                                intput_layer_index);
    PPEV2->REG_LD_CFG                        = ppev2_reg_ld_cfg_0x08.d32;

    PPEV2_REG_LL_CFG_t ppev2_reg_ll_cfg_0x0c = {.d32 = PPEV2->REG_LL_CFG};
    ppev2_reg_ll_cfg_0x0c.b.ll_en           &= ~BIT(intput_layer_index);
    ppev2_reg_ll_cfg_0x0c.b.ll_en           |= (PPEV2_InputLayer_Init_Struct->MultiFrame_LLP_En <<
                                                intput_layer_index);
    PPEV2->REG_LL_CFG                        = ppev2_reg_ll_cfg_0x0c.d32;
}




void PPEV2_StructInit(PPEV2_Init_Typedef *PPEV2_Init_Struct)
{
    /*read register*/
    PPEV2_REG_LD_CFG_t             ppev2_reg_ld_cfg_0x08             = {.d32 = PPEV2->REG_LD_CFG};
    PPEV2_REG_LLP_t                 ppev2_reg_llp_0x10                     = {.d32 = PPEV2->REG_LLP};
    PPEV2_REG_SECURE_t             ppev2_reg_secure_0x18             = {.d32 = PPEV2->REG_SECURE};

    /*initialization*/
    PPEV2_Init_Struct->SetValid_AutoClear = ppev2_reg_ld_cfg_0x08.b.auto_clr;

    PPEV2_Init_Struct->LLP                                 = (ppev2_reg_llp_0x10.b.llp) << 2;

    PPEV2_Init_Struct->Secure_En                    = ppev2_reg_secure_0x18.b.secure;

}
void PPEV2_ResultLayer_StructInit(PPEV2_ResultLayer_Init_Typedef *PPEV2_ResultLyaer_Init_Struct)
{
    PPEV2_REG_LYR0_ADDR_t       ppev2_reg_lyr0_addr_0x60            = {.d32 = PPEV2_ResultLayer->REG_LYR0_ADDR};
    PPEV2_REG_CANVAS_SIZE_t     ppev2_reg_canvas_size_0x68          = {.d32 = PPEV2_ResultLayer->REG_CANVAS_SIZE};
    PPEV2_REG_LYR0_PIC_CFG_t    ppev2_reg_lyr0_pic_cfg_0x6C         = {.d32 = PPEV2_ResultLayer->REG_LYR0_PIC_CFG};
    PPEV2_REG_BACKGROUND_t      ppev2_reg_background_0x70           = {.d32 = PPEV2_ResultLayer->REG_BACKGROUND};
    PPEV2_REG_LYR0_BUS_CFG_t    ppev2_reg_lyr0_bus_cfg_0x74         = {.d32 = PPEV2_ResultLayer->REG_LYR0_BUS_CFG};
    PPEV2_REG_LYR0_HS_CFG_t     ppev2_reg_lyr0_hs_cfg_0x78          = {.d32 = PPEV2_ResultLayer->REG_LYR0_HS_CFG};

    PPEV2_REG_LD_CFG_t          ppev2_reg_ld_cfg_0x08               = {.d32 = PPEV2->REG_LD_CFG};
    PPEV2_REG_LL_CFG_t          ppev2_reg_ll_cfg_0x0c               = {.d32 = PPEV2->REG_LL_CFG};

    PPEV2_ResultLyaer_Init_Struct->Layer_Address                    = ppev2_reg_lyr0_addr_0x60.b.addr;
    PPEV2_ResultLyaer_Init_Struct->Canvas_Width                     =
        ppev2_reg_canvas_size_0x68.b.canvas_width;
    PPEV2_ResultLyaer_Init_Struct->Canvas_Height                    =
        ppev2_reg_canvas_size_0x68.b.canvas_height;
    PPEV2_ResultLyaer_Init_Struct->Color_Format                     =
        ppev2_reg_lyr0_pic_cfg_0x6C.b.format;
    PPEV2_ResultLyaer_Init_Struct->Line_Length                      =
        ppev2_reg_lyr0_pic_cfg_0x6C.b.line_length;
    PPEV2_ResultLyaer_Init_Struct->BackGround                       =
        ppev2_reg_background_0x70.b.background;
    PPEV2_ResultLyaer_Init_Struct->LayerBus_Inc                     =
        ppev2_reg_lyr0_bus_cfg_0x74.b.incr;
    PPEV2_ResultLyaer_Init_Struct->MultiFrame_Reload_En             = ppev2_reg_ld_cfg_0x08.b.reload_en;
    PPEV2_ResultLyaer_Init_Struct->MultiFrame_LLP_En                = ppev2_reg_ll_cfg_0x0c.b.ll_en;
    PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_En            =
        ppev2_reg_lyr0_hs_cfg_0x78.b.hs_en;
    PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_Index         =
        ppev2_reg_lyr0_hs_cfg_0x78.b.hw_index;
    PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_Polarity      =
        ppev2_reg_lyr0_hs_cfg_0x78.b.polar;
    PPEV2_ResultLyaer_Init_Struct->Layer_HW_Handshake_MsizeLog      =
        ppev2_reg_lyr0_hs_cfg_0x78.b.msize_log;
}

void PPEV2_InputLayer_StructInit(PPEV2_INPUT_LAYER_INDEX intput_layer_index, \
                                 PPEV2_InputLayer_Init_Typedef *PPEV2_InputLayer_Init_Struct)
{
    PPEV2_Input_Layer_Typedef *input_layer;
    switch (intput_layer_index)
    {
    case PPEV2_INPUT_1:
        {
            input_layer = PPEV2_InputLayer1;
            break;
        }
    case PPEV2_INPUT_2:
        {
            input_layer = PPEV2_InputLayer2;
            break;
        }
    case PPEV2_INPUT_3:
        {
            input_layer = PPEV2_InputLayer3;
            break;
        }
    case PPEV2_INPUT_4:
        {
            input_layer = PPEV2_InputLayer4;
            break;
        }
    }

    PPEV2_REG_LYRx_ADDR_t                ppev2_reg_lyrx_addr_t                =    {.d32 = input_layer->REG_LYRx_ADDR};
    PPEV2_REG_LYRx_PIC_SIZE_t            ppev2_reg_lyrx_pic_size_t            =    {.d32 = input_layer->REG_LYRx_PIC_SIZE};
    PPEV2_REG_LYRx_PIC_CFG_t             ppev2_reg_lyrx_pic_cfg_t             =    {.d32 = input_layer->REG_LYRx_PIC_CFG};
    PPEV2_REG_LYRx_FIXED_COLOR_t         ppev2_reg_lyrx_fixed_color_t         =    {.d32 = input_layer->REG_LYRx_FIXED_COLOR};
    PPEV2_REG_LYRx_BUS_CFG_t             ppev2_reg_lyrx_bus_cfg_t             =    {.d32 = input_layer->REG_LYRx_BUS_CFG};
    PPEV2_REG_LYRx_HS_CFG_t              ppev2_reg_lyrx_hs_cfg_t              =    {.d32 = input_layer->REG_LYRx_HS_CFG};
    PPEV2_REG_LYRx_WIN_MIN_t             ppev2_reg_lyrx_win_min_t             =    {.d32 = input_layer->REG_LYRx_WIN_MIN};
    PPEV2_REG_LYRx_WIN_MAX_t             ppev2_reg_lyrx_win_max_t             =    {.d32 = input_layer->REG_LYRx_WIN_MAX};
    PPEV2_REG_LYRx_KEY_MIN_t             ppev2_reg_lyrx_key_min_t             =    {.d32 = input_layer->REG_LYRx_KEY_MIN};
    PPEV2_REG_LYRx_KEY_MAX_t             ppev2_reg_lyrx_key_max_t             =    {.d32 = input_layer->REG_LYRx_KEY_MAX};
    PPEV2_REG_LYRx_TRANS_MATRIX_E11_t    ppev2_reg_lyrx_trans_matrix_e11_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E11};
    PPEV2_REG_LYRx_TRANS_MATRIX_E12_t    ppev2_reg_lyrx_trans_matrix_e12_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E12};
    PPEV2_REG_LYRx_TRANS_MATRIX_E13_t    ppev2_reg_lyrx_trans_matrix_e13_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E13};
    PPEV2_REG_LYRx_TRANS_MATRIX_E21_t    ppev2_reg_lyrx_trans_matrix_e21_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E21};
    PPEV2_REG_LYRx_TRANS_MATRIX_E22_t    ppev2_reg_lyrx_trans_matrix_e22_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E22};
    PPEV2_REG_LYRx_TRANS_MATRIX_E23_t    ppev2_reg_lyrx_trans_matrix_e23_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E23};
    PPEV2_REG_LYRx_TRANS_MATRIX_E31_t    ppev2_reg_lyrx_trans_matrix_e31_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E31};
    PPEV2_REG_LYRx_TRANS_MATRIX_E32_t    ppev2_reg_lyrx_trans_matrix_e32_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E32};
    PPEV2_REG_LYRx_TRANS_MATRIX_E33_t    ppev2_reg_lyrx_trans_matrix_e33_t    =    {.d32 = input_layer->REG_LYRx_TRANS_MATRIX_E33};

    PPEV2_REG_LD_CFG_t ppev2_reg_ld_cfg_0x08 = {.d32 = PPEV2->REG_LD_CFG};
    PPEV2_REG_LL_CFG_t ppev2_reg_ll_cfg_0x0c = {.d32 = PPEV2->REG_LL_CFG};

    /*initialization*/
    PPEV2_InputLayer_Init_Struct->Layer_Address         =    ppev2_reg_lyrx_addr_t.b.addr;

    PPEV2_InputLayer_Init_Struct->Pic_Width             =    ppev2_reg_lyrx_pic_size_t.b.pic_width;
    PPEV2_InputLayer_Init_Struct->Pic_Height            =    ppev2_reg_lyrx_pic_size_t.b.pic_height;

    PPEV2_InputLayer_Init_Struct->Line_Length           =    ppev2_reg_lyrx_pic_cfg_t.b.line_length;
    PPEV2_InputLayer_Init_Struct->Color_Key_Mode        =    ppev2_reg_lyrx_pic_cfg_t.b.color_key_mode;
    PPEV2_InputLayer_Init_Struct->Pixel_Source          =    ppev2_reg_lyrx_pic_cfg_t.b.pic_src;
    PPEV2_InputLayer_Init_Struct->Pixel_Color_Format    =    ppev2_reg_lyrx_pic_cfg_t.b.format;
    PPEV2_InputLayer_Init_Struct->Read_Matrix_Size      =
        ppev2_reg_lyrx_pic_cfg_t.b.input_lyr_read_matrix_size;

    PPEV2_InputLayer_Init_Struct->Const_Pixel           =    ppev2_reg_lyrx_fixed_color_t.b.const_pixel;

    PPEV2_InputLayer_Init_Struct->LayerBus_Inc          =    ppev2_reg_lyrx_bus_cfg_t.b.incr;

    PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_En =    ppev2_reg_lyrx_hs_cfg_t.b.hs_en;
    PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_Index      =    ppev2_reg_lyrx_hs_cfg_t.b.hw_index;
    PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_MsizeLog   =
        ppev2_reg_lyrx_hs_cfg_t.b.msize_log;
    PPEV2_InputLayer_Init_Struct->Layer_HW_Handshake_Polarity   =    ppev2_reg_lyrx_hs_cfg_t.b.polar;

    PPEV2_InputLayer_Init_Struct->Layer_Window_Xmin             =
        ppev2_reg_lyrx_win_min_t.b.win_x_min;
    PPEV2_InputLayer_Init_Struct->Layer_Window_Ymin             =
        ppev2_reg_lyrx_win_min_t.b.win_y_min;

    PPEV2_InputLayer_Init_Struct->Layer_Window_Xmax             =
        ppev2_reg_lyrx_win_max_t.b.win_x_max;
    PPEV2_InputLayer_Init_Struct->Layer_Window_Ymax             =
        ppev2_reg_lyrx_win_max_t.b.win_y_max;

    PPEV2_InputLayer_Init_Struct->Color_Key_Min =    ppev2_reg_lyrx_key_min_t.b.color_key_min;
    PPEV2_InputLayer_Init_Struct->Color_Key_Max =    ppev2_reg_lyrx_key_max_t.b.color_key_max;




    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E11 = ppev2_reg_lyrx_trans_matrix_e11_t.b.matrix_e11;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E12 = ppev2_reg_lyrx_trans_matrix_e12_t.b.matrix_e12;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E13 = ppev2_reg_lyrx_trans_matrix_e13_t.b.matrix_e13;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E21 = ppev2_reg_lyrx_trans_matrix_e21_t.b.matrix_e21;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E22 = ppev2_reg_lyrx_trans_matrix_e22_t.b.matrix_e22;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E23 = ppev2_reg_lyrx_trans_matrix_e23_t.b.matrix_e23;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E31 = ppev2_reg_lyrx_trans_matrix_e31_t.b.matrix_e31;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E32 = ppev2_reg_lyrx_trans_matrix_e32_t.b.matrix_e32;
    PPEV2_InputLayer_Init_Struct->Transfer_Matrix_E33 = ppev2_reg_lyrx_trans_matrix_e33_t.b.matrix_e33;

    PPEV2_InputLayer_Init_Struct->MultiFrame_Reload_En = (ppev2_reg_ld_cfg_0x08.b.reload_en & BIT(
                                                              intput_layer_index)) >> intput_layer_index;
    PPEV2_InputLayer_Init_Struct->MultiFrame_LLP_En    = (ppev2_reg_ll_cfg_0x0c.b.ll_en & BIT(
                                                              intput_layer_index)) >> intput_layer_index;
}

void PPEV2_InputLayer_enable(PPEV2_INPUT_LAYER_INDEX intput_layer_index, FunctionalState NewState)
{
    PPEV2_REG_LYR_ENABLE_t     ppev2_reg_lyr_enable_0x04     = {.d32 = PPEV2->REG_LYR_ENABLE};
    uint32_t input_index = (uint32_t)intput_layer_index;

    if (NewState != DISABLE)
    {
        ppev2_reg_lyr_enable_0x04.b.input_lyr_en |= BIT(input_index - PPEV2_INPUT_1);
    }
    else
    {
        ppev2_reg_lyr_enable_0x04.b.input_lyr_en &= ~BIT(input_index - PPEV2_INPUT_1);
    }

    PPEV2->REG_LYR_ENABLE =  ppev2_reg_lyr_enable_0x04.d32;
}

FunctionalState PPEV2_Get_Interrupt_Status(PPEV2_INTERRUPT PPEV2_INT)
{
    if (PPEV2->REG_INTR_STATUS & BIT(PPEV2_INT))
    {
        return ENABLE;
    }
    else
    {
        return DISABLE;
    }
}

FunctionalState PPEV2_Get_Raw_Interrupt_Status(PPEV2_INTERRUPT PPEV2_INT)
{
    if (PPEV2->REG_INTR_RAW & BIT(PPEV2_INT))
    {
        return ENABLE;
    }
    else
    {
        return DISABLE;
    }
}
void PPEV2_Clear_Interrupt(PPEV2_INTERRUPT PPEV2_INT)
{
    PPEV2->REG_INTR_CLR |= BIT(PPEV2_INT);
}

void PPEV2_Mask_Interrupt(PPEV2_INTERRUPT PPEV2_INT, FunctionalState NewState)
{
    if (NewState != ENABLE)
    {
        PPEV2->REG_INTR_MASK |= BIT(PPEV2_INT);
    }
    else
    {
        PPEV2->REG_INTR_MASK &= ~BIT(PPEV2_INT);
    }
}

void PPEV2_Mask_All_Interrupt(FunctionalState NewState)
{
    if (NewState != ENABLE)
    {
        PPEV2->REG_INTR_MASK |= (BIT(PPEV2_ALL_OVER_INT) \
                                 | BIT(PPEV2_FRAME_OVER_INT) \
                                 | BIT(PPEV2_LOAD_OVER_INT) \
                                 | BIT(PPEV2_LINE_OVER_INT) \
                                 | BIT(PPEV2_SUSPEND_IACTIVE_INT) \
                                 | BIT(PPEV2_SECURE_ERROR_INT) \
                                 | BIT(PPEV2_BUS_ERROR_INT) \
                                 | BIT(PPEV2_DIV0_ERR_INT) \
                                );
    }
    else
    {
        PPEV2->REG_INTR_MASK &= ~(BIT(PPEV2_ALL_OVER_INT) \
                                  | BIT(PPEV2_FRAME_OVER_INT) \
                                  | BIT(PPEV2_LOAD_OVER_INT) \
                                  | BIT(PPEV2_LINE_OVER_INT) \
                                  | BIT(PPEV2_SUSPEND_IACTIVE_INT) \
                                  | BIT(PPEV2_SECURE_ERROR_INT) \
                                  | BIT(PPEV2_BUS_ERROR_INT) \
                                  | BIT(PPEV2_DIV0_ERR_INT) \
                                 );
    }
}

uint8_t PPEV2_Get_Pixel_Size(PPEV2_PIXEL_FORMAT format)
{
    if (format <= PPEV2_RGBX8888)
    {
        return 4;
    }
    else if (format <= PPEV2_RGBX4444)
    {
        return 2;
    }
    else if (format <= PPEV2_RGBX2222)
    {
        return 1;
    }
    else if (format <= PPEV2_RGBX5658)
    {
        return 3;
    }
    else if (format <= PPEV2_RGBX5551)
    {
        return 2;
    }
    else if (format <= PPEV2_RGB888)
    {
        return 3;
    }
    else if (format <= PPEV2_RGB565)
    {
        return 2;
    }
    else if (format <= PPEV2_X8)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#include "trace.h"
PPEV2_err PPEV2_Blend(ppe_buffer_t *dst, ppe_buffer_t *src)
{
    if (dst->address == NULL)
    {
        return PPEV2_ERR_NULL_TARGET;
    }
    ppe_matrix_t matrix;
    ppe_get_identity(&matrix);
    memcpy(&matrix, &src->inv_matrix, sizeof(ppe_matrix_t));

    uint32_t color = (src->opacity << 24);

    uint32_t comp[9];
    if (!inv_matrix2complement(&matrix, comp))
    {
        return PPEV2_ERR_INVALID_MATRIX;
    }

    PPEV2_CLK_ENABLE(ENABLE);
    PPEV2_REG_GLB_STATUS_t     ppev2_reg_blg_status_0x00;
    PPEV2_REG_LYR_ENABLE_t     ppev2_reg_lyr_enable_0x04;
    PPEV2_REG_LD_CFG_t     ppev2_reg_ld_cfg_0x08;
    PPEV2_REG_LL_CFG_t     ppev2_reg_ll_cfg_0x0c;


    /*PPEV2 Global initialization*/
    PPEV2_Init_Typedef PPEV2_Global;
    PPEV2_StructInit(&PPEV2_Global);
    PPEV2_Global.SetValid_AutoClear = ENABLE;
    PPEV2_Global.LLP = 0x0;
    PPEV2_Global.Secure_En = PPEV2_NON_SECURE_MODE;
    PPEV2_Init(&PPEV2_Global);

    /*input layer 1 initilization*/
    PPEV2_InputLayer_enable(PPEV2_INPUT_2, ENABLE);           // logic enable of layer
    PPEV2_InputLayer_Init_Typedef PPEV2_input_layer2_init;
    PPEV2_InputLayer_StructInit(PPEV2_INPUT_2, &PPEV2_input_layer2_init);
    PPEV2_input_layer2_init.Layer_Address                   = (uint32_t)src->address;
    PPEV2_input_layer2_init.Pic_Height                      = (uint32_t)src->height;
    PPEV2_input_layer2_init.Pic_Width                       = (uint32_t)src->width;
    PPEV2_input_layer2_init.Line_Length                     = PPEV2_input_layer2_init.Pic_Width *
                                                              PPEV2_Get_Pixel_Size(src->format);
    PPEV2_input_layer2_init.Pixel_Source                    = PPEV2_LAYER_SRC_FROM_DMA;
    PPEV2_input_layer2_init.Pixel_Color_Format              = src->format;
    PPEV2_input_layer2_init.Const_Pixel                     = color;
    PPEV2_input_layer2_init.Color_Key_Mode                  = src->color_key_enable;
    if (src->color_key_enable >= PPEV2_COLOR_KEY_INSIDE)
    {
        PPEV2_input_layer2_init.Color_Key_Min               = src->color_key_min;
        PPEV2_input_layer2_init.Color_Key_Max               = src->color_key_max;
    }
    else
    {
        PPEV2_input_layer2_init.Color_Key_Min               = 0x000000;
        PPEV2_input_layer2_init.Color_Key_Max               = 0xFFFFFF;
    }

    PPEV2_input_layer2_init.MultiFrame_Reload_En            = DISABLE;
    PPEV2_input_layer2_init.MultiFrame_LLP_En               = DISABLE;
    PPEV2_input_layer2_init.LayerBus_Inc                    = PPEV2_AWBURST_INC;
    PPEV2_input_layer2_init.Layer_HW_Handshake_En           = PPEV2_HW_HS_DISABLE;
    PPEV2_input_layer2_init.Layer_HW_Handshake_Index        = PPEV2_HS_RTZIP_Tx;
    PPEV2_input_layer2_init.Layer_HW_Handshake_Polarity     = PPEV2_HW_HS_ACTIVE_HIGH;
    PPEV2_input_layer2_init.Layer_HW_Handshake_MsizeLog     = PPEV2_MSIZE_1;
    PPEV2_input_layer2_init.Layer_Window_Xmin               = src->win_x_min;
    PPEV2_input_layer2_init.Layer_Window_Xmax               = src->win_x_max;
    PPEV2_input_layer2_init.Layer_Window_Ymin               = src->win_y_min;
    PPEV2_input_layer2_init.Layer_Window_Ymax               = src->win_y_max;

    uint32_t ct = 0;
    PPEV2_input_layer2_init.Transfer_Matrix_E11             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E12             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E13             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E21             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E22             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E23             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E31             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E32             = comp[ct++];
    PPEV2_input_layer2_init.Transfer_Matrix_E33             = comp[ct++];
    DBG_DIRECT("0x%x | 0x%x | 0x%x ", comp[0], comp[1], comp[2]);
    DBG_DIRECT("0x%x | 0x%x | 0x%x ", comp[3], comp[4], comp[5]);
    DBG_DIRECT("0x%x | 0x%x | 0x%x ", comp[6], comp[7], comp[8]);
    PPEV2_InputLayer_Init(PPEV2_INPUT_2, &PPEV2_input_layer2_init);


    /*input layer 1 initilization*/
    PPEV2_InputLayer_enable(PPEV2_INPUT_1, ENABLE);           // logic enable of layer
    PPEV2_InputLayer_Init_Typedef PPEV2_input_layer1_init;
    PPEV2_InputLayer_StructInit(PPEV2_INPUT_1, &PPEV2_input_layer1_init);
    PPEV2_input_layer1_init.Layer_Address                   = (uint32_t)dst->address;
    PPEV2_input_layer1_init.Pic_Height                      = (uint32_t)dst->height;
    PPEV2_input_layer1_init.Pic_Width                       = (uint32_t)dst->width;
    PPEV2_input_layer1_init.Line_Length                     = PPEV2_input_layer1_init.Pic_Width *
                                                              PPEV2_Get_Pixel_Size(src->format);
    PPEV2_input_layer1_init.Pixel_Source                    = PPEV2_LAYER_SRC_FROM_DMA;
    PPEV2_input_layer1_init.Pixel_Color_Format              = dst->format;
    PPEV2_input_layer1_init.Const_Pixel                     = 0xFFFFFFFF;
    PPEV2_input_layer1_init.MultiFrame_Reload_En            = DISABLE;
    PPEV2_input_layer1_init.MultiFrame_LLP_En               = DISABLE;
    PPEV2_input_layer1_init.LayerBus_Inc                    = PPEV2_AWBURST_INC;
    PPEV2_input_layer1_init.Layer_HW_Handshake_En           = PPEV2_HW_HS_DISABLE;
    PPEV2_input_layer1_init.Layer_HW_Handshake_Index        = PPEV2_HS_RTZIP_Tx;
    PPEV2_input_layer1_init.Layer_HW_Handshake_Polarity     = PPEV2_HW_HS_ACTIVE_HIGH;
    PPEV2_input_layer1_init.Layer_HW_Handshake_MsizeLog     = PPEV2_MSIZE_1;
    PPEV2_input_layer1_init.Layer_Window_Xmin               = 0;
    PPEV2_input_layer1_init.Layer_Window_Xmax               = dst->width;
    PPEV2_input_layer1_init.Layer_Window_Ymin               = 0;
    PPEV2_input_layer1_init.Layer_Window_Ymax               = dst->height;

    PPEV2_input_layer1_init.Transfer_Matrix_E11             = 0x10000;
    PPEV2_input_layer1_init.Transfer_Matrix_E12             = 0;
    PPEV2_input_layer1_init.Transfer_Matrix_E13             = 0;
    PPEV2_input_layer1_init.Transfer_Matrix_E21             = 0;
    PPEV2_input_layer1_init.Transfer_Matrix_E22             = 0x10000;
    PPEV2_input_layer1_init.Transfer_Matrix_E23             = 0;
    PPEV2_input_layer1_init.Transfer_Matrix_E31             = 0;
    PPEV2_input_layer1_init.Transfer_Matrix_E32             = 0;
    PPEV2_input_layer1_init.Transfer_Matrix_E33             = 0x10000;


    PPEV2_InputLayer_Init(PPEV2_INPUT_1, &PPEV2_input_layer1_init);

    PPEV2_ResultLayer_Init_Typedef PPEV2_ResultLayer0_Init;
    PPEV2_ResultLayer_StructInit(&PPEV2_ResultLayer0_Init);
    PPEV2_ResultLayer0_Init.Layer_Address                   = (uint32_t)dst->address;
    PPEV2_ResultLayer0_Init.Canvas_Height                   = dst->height;
    PPEV2_ResultLayer0_Init.Canvas_Width                    = dst->width;
    PPEV2_ResultLayer0_Init.Line_Length                     = PPEV2_ResultLayer0_Init.Canvas_Width *
                                                              PPEV2_Get_Pixel_Size(dst->format);
    PPEV2_ResultLayer0_Init.Color_Format                    = dst->format;
    PPEV2_ResultLayer0_Init.BackGround                      = dst->const_color;
    PPEV2_ResultLayer0_Init.LayerBus_Inc                    = PPEV2_AWBURST_INC;
    PPEV2_ResultLayer0_Init.MultiFrame_Reload_En            = DISABLE;
    PPEV2_ResultLayer0_Init.MultiFrame_LLP_En               = DISABLE;
    PPEV2_ResultLayer_Init(&PPEV2_ResultLayer0_Init);

    PPEV2_Cmd(ENABLE);
    while (((PPEV2_REG_GLB_STATUS_t)PPEV2->REG_GLB_STATUS).b.run_state);
    return PPEV2_SUCCESS;
}

void ppe_get_identity(ppe_matrix_t *matrix)
{
    /* Set identify matrix. */
    matrix->m[0][0] = 1.0f;
    matrix->m[0][1] = 0.0f;
    matrix->m[0][2] = 0.0f;
    matrix->m[1][0] = 0.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[1][2] = 0.0f;
    matrix->m[2][0] = 0.0f;
    matrix->m[2][1] = 0.0f;
    matrix->m[2][2] = 1.0f;
}

#if USE_INTERNAL_MAT
static const int16_t sin_table[] =
{
    0,     572,   1144,  1715,  2286,  2856,  3425,  3993,  4560,  5126,  5690,  6252,  6813,  7371,  7927,  8481,
    9032,  9580,  10126, 10668, 11207, 11743, 12275, 12803, 13328, 13848, 14364, 14876, 15383, 15886, 16383, 16876,
    17364, 17846, 18323, 18794, 19260, 19720, 20173, 20621, 21062, 21497, 21925, 22347, 22762, 23170, 23571, 23964,
    24351, 24730, 25101, 25465, 25821, 26169, 26509, 26841, 27165, 27481, 27788, 28087, 28377, 28659, 28932, 29196,
    29451, 29697, 29934, 30162, 30381, 30591, 30791, 30982, 31163, 31335, 31498, 31650, 31794, 31927, 32051, 32165,
    32269, 32364, 32448, 32523, 32587, 32642, 32687, 32722, 32747, 32762, 32767
};

static int16_t ppe_fix_sin(int16_t angle)
{
    int16_t ret = 0;
    angle       = angle % 360;

    if (angle < 0) { angle = 360 + angle; }

    if (angle < 90)
    {
        ret = sin_table[angle];
    }
    else if (angle >= 90 && angle < 180)
    {
        angle = 180 - angle;
        ret   = sin_table[angle];
    }
    else if (angle >= 180 && angle < 270)
    {
        angle = angle - 180;
        ret   = -sin_table[angle];
    }
    else     /*angle >=270*/
    {
        angle = 360 - angle;
        ret   = -sin_table[angle];
    }

    return ret;
}

static inline int16_t ppe_fix_cos(int16_t angle)
{
    return ppe_fix_sin(angle + 90);
}


static void multiply(ppe_matrix_t *matrix, ppe_matrix_t *mult)
{
    ppe_matrix_t temp;
    int row, column;

    /* Process all rows. */
    for (row = 0; row < 3; row++)
    {
        /* Process all columns. */
        for (column = 0; column < 3; column++)
        {
            /* Compute matrix entry. */
            temp.m[row][column] = (matrix->m[row][0] * mult->m[0][column])
                                  + (matrix->m[row][1] * mult->m[1][column])
                                  + (matrix->m[row][2] * mult->m[2][column]);
        }
    }

    /* Copy temporary matrix into result. */
    memcpy(matrix, &temp, sizeof(temp));
}

void ppe_translate(float x, float y, ppe_matrix_t *matrix)
{
    /* Set translation matrix. */
    ppe_matrix_t t = { { {1.0f, 0.0f, x},
            {0.0f, 1.0f, y},
            {0.0f, 0.0f, 1.0f}
        }
    };

    /* Multiply with current matrix. */
    multiply(matrix, &t);
}

void ppe_scale(float scale_x, float scale_y, ppe_matrix_t *matrix)
{
    /* Set scale matrix. */
    ppe_matrix_t s = { { {scale_x, 0.0f, 0.0f},
            {0.0f, scale_y, 0.0f},
            {0.0f, 0.0f, 1.0f}
        }
    };

    /* Multiply with current matrix. */
    multiply(matrix, &s);
}

void ppe_rotate(float degrees, ppe_matrix_t *matrix)
{
#ifndef M_PI
#define M_PI 3.1415926535898f
#endif
    /* Convert degrees into radians. */
    int16_t angle = (int)degrees;

    /* Compuet cosine and sine values. */
    float cos_angle = ppe_fix_cos(angle) / 32767.0f;
    float sin_angle = ppe_fix_sin(angle) / 32767.0f;

    /* Set rotation matrix. */
    ppe_matrix_t r = { { {cos_angle, -sin_angle, 0.0f},
            {sin_angle, cos_angle, 0.0f},
            {0.0f, 0.0f, 1.0f}
        }
    };

    /* Multiply with current matrix. */
    multiply(matrix, &r);
}

void ppe_perspective(float px, float py, ppe_matrix_t *matrix)
{
    /* set prespective matrix */
    ppe_matrix_t p = { { {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {px, py, 1.0f}
        }
    };
    /* Multiply with current matrix. */
    multiply(matrix, &p);
}

static int swap(float *a, float *b)
{
    float temp;
    if (a == NULL || b == NULL)
    {
        return -1;
    }
    temp = *a;
    *a = *b;
    *b = temp;
    return 0;
}

int ppe_get_transform_matrix(ppe_point4_t src, ppe_point4_t dst,
                             ppe_matrix_t *mat)
{
    float a[8][8], b[9], A[64];
    int i, j, k, m = 8, n = 1;
    int astep = 8, bstep = 1;
    float d;

    if (src == NULL || dst == NULL || mat == NULL)
    {
        return  -1;
    }

    for (i = 0; i < 4; ++i)
    {
        a[i][0] = a[i + 4][3] = src[i].x;
        a[i][1] = a[i + 4][4] = src[i].y;
        a[i][2] = a[i + 4][5] = 1;
        a[i][3] = a[i][4] = a[i][5] =
                                a[i + 4][0] = a[i + 4][1] = a[i + 4][2] = 0;
        a[i][6] = -src[i].x * dst[i].x;
        a[i][7] = -src[i].y * dst[i].x;
        a[i + 4][6] = -src[i].x * dst[i].y;
        a[i + 4][7] = -src[i].y * dst[i].y;
        b[i] = dst[i].x;
        b[i + 4] = dst[i].y;
    }
    for (i = 0; i < 8; ++i)
    {
        for (j = 0; j < 8; ++j)
        {
            A[8 * i + j] = a[i][j];
        }
    }

    for (i = 0; i < m; i++)
    {
        k = i;
        for (j = i + 1; j < m; j++)
            if (ABS(A[j * astep + i]) > ABS(A[k * astep + i]))
            {
                k = j;
            }
        if (ABS(A[k * astep + i]) < EPS)
        {
            return -1;
        }
        if (k != i)
        {
            for (j = i; j < m; j++)
            {
                swap(&A[i * astep + j], &A[k * astep + j]);
            }
            for (j = 0; j < n; j++)
            {
                swap(&b[i * bstep + j], &b[k * bstep + j]);
            }
        }
        d = -1 / A[i * astep + i];
        for (j = i + 1; j < m; j++)
        {
            float alpha = A[j * astep + i] * d;
            for (k = i + 1; k < m; k++)
            {
                A[j * astep + k] += alpha * A[i * astep + k];
            }
            for (k = 0; k < n; k++)
            {
                b[j * bstep + k] += alpha * b[i * bstep + k];
            }
        }
    }

    for (i = m - 1; i >= 0; i--)
        for (j = 0; j < n; j++)
        {
            float s = b[i * bstep + j];
            for (k = i + 1; k < m; k++)
            {
                s -= A[i * astep + k] * b[k * bstep + j];
            }
            b[i * bstep + j] = s / A[i * astep + i];
        }

    b[8] = 1;

    for (i = 0; i < 3; ++i)
    {
        for (j = 0; j < 3; ++j)
        {
            mat->m[i][j] = b[i * 3 + j];
        }
    }
}

void matrix_inverse(ppe_matrix_t *matrix)
{
    ppe_matrix_t temp;

    float detal = matrix->m[0][0] * matrix->m[1][1] * matrix->m[2][2] + \
                  matrix->m[0][1] * matrix->m[1][2] * matrix->m[2][0] + \
                  matrix->m[0][2] * matrix->m[1][0] * matrix->m[2][1] - \
                  matrix->m[0][0] * matrix->m[1][2] * matrix->m[2][1] - \
                  matrix->m[0][1] * matrix->m[1][0] * matrix->m[2][2] - \
                  matrix->m[0][2] * matrix->m[1][1] * matrix->m[2][0];

#if 1
    temp.m[0][0] = (matrix->m[1][1] * matrix->m[2][2] - matrix->m[1][2] * matrix->m[2][1]) / detal;
    temp.m[1][0] = (matrix->m[1][2] * matrix->m[2][0] - matrix->m[1][0] * matrix->m[2][2]) / detal;
    temp.m[2][0] = (matrix->m[1][0] * matrix->m[2][1] - matrix->m[1][1] * matrix->m[2][0]) / detal;

    temp.m[0][1] = (matrix->m[2][1] * matrix->m[0][2] - matrix->m[2][2] * matrix->m[0][1]) / detal;
    temp.m[1][1] = (matrix->m[2][2] * matrix->m[0][0] - matrix->m[2][0] * matrix->m[0][2]) / detal;
    temp.m[2][1] = (matrix->m[2][1] * matrix->m[0][0] - matrix->m[2][0] * matrix->m[0][1]) / detal;

    temp.m[0][2] = (matrix->m[0][1] * matrix->m[1][2] - matrix->m[0][2] * matrix->m[1][1]) / detal;
    temp.m[1][2] = (matrix->m[0][2] * matrix->m[1][0] - matrix->m[0][0] * matrix->m[1][2]) / detal;
    temp.m[2][2] = (matrix->m[0][0] * matrix->m[1][1] - matrix->m[0][1] * matrix->m[1][0]) / detal;
#else
    temp.m[0][0] = (matrix->m[1][1] * matrix->m[2][2] - matrix->m[1][2] * matrix->m[2][1]) / detal;
    temp.m[0][1] = (matrix->m[1][2] * matrix->m[2][0] - matrix->m[1][0] * matrix->m[2][2]) / detal;
    temp.m[0][2] = (matrix->m[1][0] * matrix->m[2][1] - matrix->m[1][1] * matrix->m[2][0]) / detal;

    temp.m[1][0] = (matrix->m[2][1] * matrix->m[0][2] - matrix->m[2][2] * matrix->m[0][1]) / detal;
    temp.m[1][1] = (matrix->m[2][2] * matrix->m[0][0] - matrix->m[2][0] * matrix->m[0][2]) / detal;
    temp.m[1][2] = (matrix->m[2][1] * matrix->m[0][0] - matrix->m[2][0] * matrix->m[0][1]) / detal;

    temp.m[2][0] = (matrix->m[0][1] * matrix->m[1][2] - matrix->m[0][2] * matrix->m[1][1]) / detal;
    temp.m[2][1] = (matrix->m[0][2] * matrix->m[1][0] - matrix->m[0][0] * matrix->m[1][2]) / detal;
    temp.m[2][2] = (matrix->m[0][0] * matrix->m[1][1] - matrix->m[0][1] * matrix->m[1][0]) / detal;
#endif

    /* Copy temporary matrix into result. */
    memcpy(matrix, &temp, sizeof(temp));
}
#endif

bool inv_matrix2complement(ppe_matrix_t *matrix, uint32_t *comp)
{
    float *tf = (float *)matrix->m;
    for (int i = 0; i < 9; i++)
    {
        int32_t tr = tf[i] * 65536;
        if (tf[i] < 0)
        {
            tr = -tr;
            comp[i] = *(uint32_t *)&tr;
            if (comp[i]  > 0x7FFFFFFF)
            {
                return false;
            }
            else

            {
                comp[i] = ((0x7FFFFFFF - comp[i]) | 0x80000000) + 0x1 ;
            }
        }
        else
        {
            comp[i] = *(uint32_t *)&tr;
        }
    }
    return true;
}

