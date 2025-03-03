//------------------------------------------------------------------------------
// File: Mixer.c
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpuapi.h"
#include "regdefine.h"
//#include "jpulog.h"
#include "jpuhelper_rt.h"
#include "mixer.h"



typedef struct
{
    FRAME_BUF frameBuf[MAX_FRAME];
    jpu_buffer_t vb_base;
    int instIndex;
    int last_num;
    int last_addr;
} fb_context;

static fb_context s_fb[MAX_NUM_INSTANCE];


int RegisterFrameBuffer(int instIdx, int format, int width, int height, int frameBufNum, int pack,
                        void *addr)
{
    int  divX, divY;
    int  i;
    int  lum_size, chr_size;
    fb_context *fb;

    fb = &s_fb[instIdx];

    divX = format == FORMAT_420 || format == FORMAT_422 ? 2 : 1;
    divY = format == FORMAT_420 || format == FORMAT_224 ? 2 : 1;

    switch (format)
    {
    case FORMAT_420:
        height = (height + 1) / 2 * 2;
        width = (width + 1) / 2 * 2;
        break;
    case FORMAT_224:
        height = (height + 1) / 2 * 2;
        break;
    case FORMAT_422:
        width = (width + 1) / 2 * 2;
        break;
    case FORMAT_444:
        height = (height + 1) / 2 * 2;
        width = (width + 1) / 2 * 2;
        break;
    case FORMAT_400:
        height = (height + 1) / 2 * 2;
        width = (width + 1) / 2 * 2;
        break;
    }

    lum_size   = width * height;

    if (pack)
    {
        chr_size   = 0;
    }
    else
    {
        chr_size   = lum_size / divX / divY;
    }


    if (format == FORMAT_400)
    {
        chr_size = 0;
    }

    fb->vb_base.size = lum_size + chr_size * 2;
    fb->vb_base.size *= frameBufNum;
    extern int jdi_register_dma_memory(jpu_buffer_t *vb, void *addr);
    if (jdi_register_dma_memory(&fb->vb_base, addr) < 0)
    {
        DBG_DIRECT("Fail to allocate frame buffer size=%d\n", fb->vb_base.size);
        return 0;
    }

    fb->last_addr = fb->vb_base.phys_addr;


    for (i = fb->last_num; i < fb->last_num + frameBufNum; i++)
    {
        fb->frameBuf[i].Format = format;
        fb->frameBuf[i].Index  = i;

        fb->frameBuf[i].vbY.phys_addr = fb->last_addr;
        fb->frameBuf[i].vbY.size = lum_size;

        fb->last_addr += fb->frameBuf[i].vbY.size;
        fb->last_addr = ((fb->last_addr + 7) & ~7);

        if (chr_size)
        {
            fb->frameBuf[i].vbCb.phys_addr = fb->last_addr;
            fb->frameBuf[i].vbCb.size = chr_size;

            fb->last_addr += fb->frameBuf[i].vbCb.size;
            fb->last_addr = ((fb->last_addr + 7) & ~7);

            fb->frameBuf[i].vbCr.phys_addr = fb->last_addr;
            fb->frameBuf[i].vbCr.size = chr_size;

            fb->last_addr += fb->frameBuf[i].vbCr.size;
            fb->last_addr = ((fb->last_addr + 7) & ~7);
        }

        fb->frameBuf[i].strideY = width;
        fb->frameBuf[i].strideC = width / divX;
    }
    fb->last_num += frameBufNum;

    return 1;
}
int AllocateFrameBuffer(int instIdx, int format, int width, int height, int frameBufNum, int pack)
{
    int  divX, divY;
    int  i;
    int  lum_size, chr_size;
    fb_context *fb;

    fb = &s_fb[instIdx];

    divX = format == FORMAT_420 || format == FORMAT_422 ? 2 : 1;
    divY = format == FORMAT_420 || format == FORMAT_224 ? 2 : 1;

    switch (format)
    {
    case FORMAT_420:
        height = (height + 1) / 2 * 2;
        width = (width + 1) / 2 * 2;
        break;
    case FORMAT_224:
        height = (height + 1) / 2 * 2;
        break;
    case FORMAT_422:
        width = (width + 1) / 2 * 2;
        break;
    case FORMAT_444:
        height = (height + 1) / 2 * 2;
        width = (width + 1) / 2 * 2;
        break;
    case FORMAT_400:
        height = (height + 1) / 2 * 2;
        width = (width + 1) / 2 * 2;
        break;
    }

    lum_size   = width * height;

    if (pack)
    {
        chr_size   = 0;
    }
    else
    {
        chr_size   = lum_size / divX / divY;
    }


    if (format == FORMAT_400)
    {
        chr_size = 0;
    }

    fb->vb_base.size = lum_size + chr_size * 2;
    fb->vb_base.size *= frameBufNum;
    if (jdi_allocate_dma_memory(&fb->vb_base) < 0)
    {
        DBG_DIRECT("Fail to allocate frame buffer size=%d\n", fb->vb_base.size);
        return 0;
    }

    fb->last_addr = fb->vb_base.phys_addr;


    for (i = fb->last_num; i < fb->last_num + frameBufNum; i++)
    {
        fb->frameBuf[i].Format = format;
        fb->frameBuf[i].Index  = i;

        fb->frameBuf[i].vbY.phys_addr = fb->last_addr;
        fb->frameBuf[i].vbY.size = lum_size;

        fb->last_addr += fb->frameBuf[i].vbY.size;
        fb->last_addr = ((fb->last_addr + 7) & ~7);

        if (chr_size)
        {
            fb->frameBuf[i].vbCb.phys_addr = fb->last_addr;
            fb->frameBuf[i].vbCb.size = chr_size;

            fb->last_addr += fb->frameBuf[i].vbCb.size;
            fb->last_addr = ((fb->last_addr + 7) & ~7);

            fb->frameBuf[i].vbCr.phys_addr = fb->last_addr;
            fb->frameBuf[i].vbCr.size = chr_size;

            fb->last_addr += fb->frameBuf[i].vbCr.size;
            fb->last_addr = ((fb->last_addr + 7) & ~7);
        }

        fb->frameBuf[i].strideY = width;
        fb->frameBuf[i].strideC = width / divX;
    }

#if 0
    for (i = fb->last_num; i < fb->last_num + frameBufNum; i++)
    {
        JLOG(TRACE, " >_Create DPB [ %i] -- Linear\n", i);
        JLOG(TRACE, " >_ Luminance Frame 0x%08x\n", fb->frameBuf[i].vbY.phys_addr);
        JLOG(TRACE, " >_ Cb        Frame 0x%08x\n", fb->frameBuf[i].vbCb.phys_addr);
        JLOG(TRACE, " >_ Cr        Frame 0x%08x\n", fb->frameBuf[i].vbCr.phys_addr);
        JLOG(TRACE, " >_ fb->last_addr     0x%08x\n", fb->last_addr);
        JLOG(TRACE, " >_ StrideY         (%d)\n", fb->frameBuf[i].strideY);
        JLOG(TRACE, " >_ StrideC         (%d)\n", fb->frameBuf[i].strideC);
    }
#endif

    fb->last_num += frameBufNum;

    return 1;
}



int GetFrameBufBase(int instIdx)
{
    fb_context *fb;
    fb = &s_fb[instIdx];

    return fb->vb_base.phys_addr;
}

int GetFrameBufAllocSize(int instIdx)
{
    fb_context *fb;
    fb = &s_fb[instIdx];

    return (fb->last_addr - fb->vb_base.phys_addr);
}


FRAME_BUF *GetFrameBuffer(int instIdx, int index)
{
    fb_context *fb;
    fb = &s_fb[instIdx];
    return &fb->frameBuf[index];
}

int getFramBuffyuv(PhysicalAddress addrY, PhysicalAddress addrU, PhysicalAddress addrV)
{
    int i;
    fb_context *fb;

    fb = &s_fb[0];

    for (i = 0; i < MAX_FRAME; i++)
    {
        if (fb->frameBuf[i].vbY.phys_addr == addrY)
        {
            return 0;
        }
    }
    return 1;
}
FRAME_BUF *FindFrameBuffer(int instIdx, PhysicalAddress addrY)
{
    int i;
    fb_context *fb;

    fb = &s_fb[instIdx];

    for (i = 0; i < MAX_FRAME; i++)
    {
        if (fb->frameBuf[i].vbY.phys_addr == addrY)
        {
            return &fb->frameBuf[i];
        }
    }

    return NULL;
}
void ClearFrameBuffer(int instIdx, int index)
{

    return;
}

void FreeFrameBuffer(int instIdx)
{
    fb_context *fb;

    fb = &s_fb[instIdx];

    fb->last_num = 0;
    fb->last_addr = -1;

    jdi_free_dma_memory(&fb->vb_base);
    fb->vb_base.base = 0;
    fb->vb_base.size = 0;
    memset(fb->frameBuf, 0, sizeof(fb->frameBuf));
}

void CleanFrameBuffer(int instIdx)
{
    fb_context *fb;

    fb = &s_fb[instIdx];

    fb->last_num = 0;
    fb->last_addr = -1;

    extern void jdi_clean_dma_memory(jpu_buffer_t *vb);
    jdi_clean_dma_memory(&fb->vb_base);
    fb->vb_base.base = 0;
    fb->vb_base.size = 0;
    memset(fb->frameBuf, 0, sizeof(fb->frameBuf));
}

#ifdef CNM_FPGA_PLATFORM

//------------------------------------------------------------------------------
// MIXER REGISTER ADDRESS
//------------------------------------------------------------------------------
#define MIX_BASE                0x1000000
#define DISP_MIX                0x2000000

#define MIX_INT                 (MIX_BASE + 0x044)

#define MIX_STRIDE_Y            (MIX_BASE + 0x144)
#define MIX_STRIDE_CB           (MIX_BASE + 0x148)
#define MIX_STRIDE_CR           (MIX_BASE + 0x14c)

#define MIX_ADDR_Y              (MIX_BASE + 0x138)
#define MIX_ADDR_CB             (MIX_BASE + 0x13C)
#define MIX_ADDR_CR             (MIX_BASE + 0x140)

#define MIX_RUN                 (MIX_BASE + 0x120)

#define DISP_TOTAL_SAMPLE       (DISP_MIX + 0x00C)
#define DISP_ACTIVE_SAMPLE      (DISP_MIX + 0x010)
#define DISP_HSYNC_START_END    (DISP_MIX + 0x014)
#define DISP_VSYNC_TOP_START    (DISP_MIX + 0x018)
#define DISP_VSYNC_TOP_END      (DISP_MIX + 0x01C)
#define DISP_VSYNC_BOT_START    (DISP_MIX + 0x020)
#define DISP_VSYNC_BOT_END      (DISP_MIX + 0x024)
#define DISP_ACTIVE_REGION_TOP  (DISP_MIX + 0x02C)
#define DISP_ACTIVE_REGION_BOT  (DISP_MIX + 0x030)


#define MIX_MIX_INTRPT          (MIX_BASE + 0x0000)
#define MIX_SYNC_STATE          (MIX_BASE + 0x0004)
#define MIX_SYNC_CTRL           (MIX_BASE + 0x0008)
#define MIX_TOTAL_SAMPLE        (MIX_BASE + 0x000c)
#define MIX_ACTIVE_SAMPLE       (MIX_BASE + 0x0010)
#define MIX_HSYNC_START_END     (MIX_BASE + 0x0014)
#define MIX_VSYNC_TOP_START     (MIX_BASE + 0x0018)
#define MIX_VSYNC_TOP_END       (MIX_BASE + 0x001c)
#define MIX_VSYNC_BOT_START     (MIX_BASE + 0x0020)
#define MIX_VSYNC_BOT_END       (MIX_BASE + 0x0024)
#define MIX_ACT_REGION_SAMPLE   (MIX_BASE + 0x0028)
#define MIX_ACT_REGION_TOP      (MIX_BASE + 0x002c)
#define MIX_ACT_REGION_BOT      (MIX_BASE + 0x0030)
#define MIX_TOP_START           (MIX_BASE + 0x0034)
#define MIX_BOT_START           (MIX_BASE + 0x0038)
#define MIX_LINE_INC            (MIX_BASE + 0x003c)
#define MIX_LATCH_PARAM_CTRL    (MIX_BASE + 0x0040)
#define MIX_INTERRUPT           (MIX_BASE + 0x0044)

#define MIX_LAYER_CTRL          (MIX_BASE + 0x0100)
#define MIX_LAYER_ORDER         (MIX_BASE + 0x0104)
#define MIX_BIG_ENDIAN          (MIX_BASE + 0x0108)
#define MIX_L0_BG_COLOR         (MIX_BASE + 0x0110)
#define MIX_L1_CTRL             (MIX_BASE + 0x0120)
#define MIX_L1_LSIZE            (MIX_BASE + 0x0124)
#define MIX_L1_SSIZE            (MIX_BASE + 0x0128)
#define MIX_L1_LPOS             (MIX_BASE + 0x012c)
#define MIX_L1_SPOS             (MIX_BASE + 0x0130)
#define MIX_L1_BG_COLOR         (MIX_BASE + 0x0134)
#define MIX_L1_Y_SADDR          (MIX_BASE + 0x0138)
#define MIX_L1_CB_SADDR         (MIX_BASE + 0x013c)
#define MIX_L1_CR_SADDR         (MIX_BASE + 0x0140)
#define MIX_L1_Y_STRIDE         (MIX_BASE + 0x0144)
#define MIX_L1_CB_STRIDE        (MIX_BASE + 0x0148)
#define MIX_L1_CR_STRIDE        (MIX_BASE + 0x014c)


int  SetMixerDecOutFrame(FRAME_BUF *pFrame, int width, int height)
{
    int  staX, staY;
    int div;
    staX = (MAX_DISPLAY_WIDTH - width) / 2;
    if (height > MAX_DISPLAY_HEIGHT)
    {
        staY = 0;
    }
    else
    {
        staY = (MAX_DISPLAY_HEIGHT - height) / 2;
    }
    if (staX % 16)
    {
        staX = (staX + 15) / 16 * 16;
    }

    JpuWriteReg(MIX_L0_BG_COLOR, (0 << 16) | (0x80 << 8) | 0x80);

    JpuWriteReg(MIX_L1_LSIZE, (height << 12) | width);
    JpuWriteReg(MIX_L1_SSIZE, (height << 12) | width);
    JpuWriteReg(MIX_L1_LPOS, (staY << 12) | staX);

    div = (pFrame->Format == FORMAT_420 || pFrame->Format == FORMAT_422 ||
           pFrame->Format == FORMAT_400) ? 2 : 1;

    JpuWriteReg(MIX_STRIDE_Y,  width);
    JpuWriteReg(MIX_STRIDE_CB, width / div);
    JpuWriteReg(MIX_STRIDE_CR, width / div);


    JpuWriteReg(MIX_ADDR_Y,  pFrame->vbY.phys_addr);
    JpuWriteReg(MIX_ADDR_CB, pFrame->vbCb.phys_addr);
    JpuWriteReg(MIX_ADDR_CR, pFrame->vbCr.phys_addr);

    JpuWriteReg(DISP_HSYNC_START_END, ((0x7d7 - 40) << 12) | (0x82f - 40)); // horizontal center
    JpuWriteReg(DISP_ACTIVE_REGION_TOP, ((0x014 - 2) << 12) | (0x230 - 2));
    JpuWriteReg(DISP_ACTIVE_REGION_BOT, ((0x247 - 2) << 12) | (0x463 - 2));

    JpuWriteReg(MIX_LAYER_CTRL, 0x3);                      // backgroup on
    JpuWriteReg(MIX_RUN, 0x92);                            // on, vdec, from sdram

    return 1;
}


int  SetMixerDecOutLayer(int instIdx, int index, int width, int height)
{
    FRAME_BUF *pFrame;
    int  staX, staY;
    int div;

    pFrame = GetFrameBuffer(instIdx, index);

    staX = (MAX_DISPLAY_WIDTH - width) / 2;
    if (height > MAX_DISPLAY_HEIGHT)
    {
        staY = 0;
    }
    else
    {
        staY = (MAX_DISPLAY_HEIGHT - height) / 2;
    }
    if (staX % 16)
    {
        staX = (staX + 15) / 16 * 16;
    }

    JpuWriteReg(MIX_L0_BG_COLOR, (0 << 16) | (0x80 << 8) | 0x80);

    JpuWriteReg(MIX_L1_LSIZE, (height << 12) | width);
    JpuWriteReg(MIX_L1_SSIZE, (height << 12) | width);
    JpuWriteReg(MIX_L1_LPOS, (staY << 12) | staX);

    div = (pFrame->Format == FORMAT_420 || pFrame->Format == FORMAT_422 ||
           pFrame->Format == FORMAT_400) ? 2 : 1;

    JpuWriteReg(MIX_STRIDE_Y,  width);
    JpuWriteReg(MIX_STRIDE_CB, width / div);
    JpuWriteReg(MIX_STRIDE_CR, width / div);



    JpuWriteReg(MIX_ADDR_Y,  pFrame->vbY.phys_addr);
    JpuWriteReg(MIX_ADDR_CB, pFrame->vbCb.phys_addr);
    JpuWriteReg(MIX_ADDR_CR, pFrame->vbCr.phys_addr);

    JpuWriteReg(DISP_HSYNC_START_END, ((0x7d7 - 40) << 12) | (0x82f - 40)); // horizontal center
    JpuWriteReg(DISP_ACTIVE_REGION_TOP, ((0x014 - 2) << 12) | (0x230 - 2));
    JpuWriteReg(DISP_ACTIVE_REGION_BOT, ((0x247 - 2) << 12) | (0x463 - 2));

    JpuWriteReg(MIX_LAYER_CTRL, 0x3);                      // backgroup on
    JpuWriteReg(MIX_RUN, 0x92);                            // on, vdec, from sdram

    return 1;
}

void WaitMixerInt(void)
{
    int  data;

    return;

    if (JpuReadReg(MIX_INT) == 1)
    {
        JpuWriteReg(MIX_INT, 0);
    }

    while (1)
    {
        data = JpuReadReg(MIX_INT);
        if (data & 1)
        {
            break;
        }
    }
    JpuWriteReg(MIX_INT, 0);
}

#endif




int sw_mixer_open(int instIdx, int width, int height)
{
#if defined(PLATFORM_WIN32)
    sw_mixer_context_t *mixer = &s_mixer[instIdx];

    HDC hdc;

    if (mixer->s_dib_section)
    {
        return 0;
    }

    memset(&mixer->bmi2, 0x00, sizeof(mixer->bmi2));
    mixer->bmi2.bmih.biSize = sizeof(BITMAPINFOHEADER);
    mixer->bmi2.bmih.biWidth = width;
    mixer->bmi2.bmih.biHeight = -(height);
    mixer->bmi2.bmih.biPlanes = 1;
    mixer->bmi2.bmih.biBitCount = 32;
    mixer->bmi2.bmih.biCompression = BI_RGB;
    if (mixer->bmi2.bmih.biBitCount == 16)
    {
        mixer->bmi2.bmih.biCompression = BI_BITFIELDS;
        *(DWORD *)(&mixer->bmi2.rgb[0]) = 0xF800;
        *(DWORD *)(&mixer->bmi2.rgb[1]) = 0x07e0;
        *(DWORD *)(&mixer->bmi2.rgb[2]) = 0x001F;
    }

    mixer->s_dib_section = CreateDIBSection(
                               NULL,
                               (PBITMAPINFO)&mixer->bmi2,
                               DIB_RGB_COLORS,
                               &mixer->s_dib_buffer,
                               NULL,
                               0
                           );


    hdc = GetDC(mixer->s_hWnd);
    mixer->s_hdc_memory = CreateCompatibleDC(hdc);
    if (!mixer->s_hdc_memory)
    {
        return 0;
    }
    ReleaseDC(mixer->s_hWnd, hdc);

    mixer->s_old_hobject = SelectObject(mixer->s_hdc_memory, mixer->s_dib_section);
    if (!mixer->s_old_hobject)
    {
        return 0;
    }


#ifdef DRAW_IN_WINDOW
    {
        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style   = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra  = 0;
        wcex.cbWndExtra  = 0;
        wcex.hInstance  = GetModuleHandle(NULL);
        wcex.hIcon   = LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor  = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = "CNMMIXER";
        wcex.hIconSm  = LoadIcon(NULL, IDI_APPLICATION);

        RegisterClassEx(&wcex);

        mixer->s_hWnd = CreateWindow("CNMMIXER", "C&M YuvViewer", WS_OVERLAPPEDWINDOW,
                                     CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
        MoveWindow(mixer->s_hWnd, instIdx * width, 0, width + 16, height + 38, TRUE);
        mixer->wndShow = 0;
    }
#endif
#endif //#if defined(PLATFORM_WIN32) 

    return 1;

}





int sw_mixer_draw(int instIdx, int x, int y, int width, int height, int planar_format,
                  int pack_format, int interleave, unsigned char *pbImage)
{
#ifdef PLATFORM_LINUX
    sw_mixer_context_t *mixer = &s_mixer[instIdx];
    unsigned char *src_ptr = pbImage;
    unsigned char *dest_ptr = (unsigned char *)mixer->s_scr_ptr;
    unsigned char *src_cpy_ptr;
    unsigned char *dest_cpy_ptr ;
    int i;

    if (mixer->s_fd < 0)
    {
        return 0;
    }

    jpu_yuv2rgb(width, height, (yuv2rgb_color_format)convert_jpuapi_format_to_yuv2rgb_color_format(
                    planar_format, pack_format, interleave),
                src_ptr,  mixer->s_rgb_ptr, 1);

    src_cpy_ptr = mixer->s_rgb_ptr;
    dest_cpy_ptr = dest_ptr;

    for (i = 0; i < height; ++i)
    {
        memcpy(dest_cpy_ptr, src_cpy_ptr, (width * 2));
        src_cpy_ptr += (width * 2);
        dest_cpy_ptr += (mixer->s_fb_stride);
    }
#endif  //#ifdef PLATFORM_LINUX

#if defined(PLATFORM_WIN32)
    sw_mixer_context_t *mixer = &s_mixer[instIdx];
    HDC hdc_screen;
    RECT rc;
    int dispWidth;
    int dispHeight;



    GetClientRect(mixer->s_hWnd, &rc);
    dispWidth = rc.right - rc.left;
    dispHeight = rc.bottom - rc.top;

    if (!mixer->wndShow)
    {
        ShowWindow(mixer->s_hWnd, SW_SHOW);
        //UpdateWindow(mixer->s_hWnd);
        //SetForegroundWindow(mixer->s_hWnd);
        mixer->wndShow = 1;
    }
    else
    {
        MSG msg = {0};
        while (1)
        {
            if (PeekMessage(&msg, mixer->s_hWnd, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                break;
            }
        }
    }

#ifdef DRAW_IN_WINDOW
    hdc_screen = GetDC(mixer->s_hWnd);
#else
    hdc_screen = GetDC(NULL);
#endif

    jpu_yuv2rgb(width, height, (yuv2rgb_color_format)convert_jpuapi_format_to_yuv2rgb_color_format(
                    planar_format, pack_format, interleave),
                pbImage, (unsigned char *)mixer->s_dib_buffer, 1);

    if (hdc_screen)
    {
        if (width != dispWidth || height != dispHeight)
        {
            StretchBlt(hdc_screen, 0, 0, dispWidth, dispHeight, mixer->s_hdc_memory, x, y, width, height,
                       SRCCOPY);
        }
        else
        {
            BitBlt(hdc_screen, x, y, width, height, mixer->s_hdc_memory, 0, 0, SRCCOPY);
        }
        ReleaseDC(mixer->s_hWnd, hdc_screen);
        hdc_screen = NULL;
    }
#endif  //#if defined(PLATFORM_WIN32) 
    return 1;

}

void sw_mixer_close(int instIdx)
{
#ifdef PLATFORM_LINUX
    sw_mixer_context_t *mixer = &s_mixer[instIdx];
    if (mixer->s_scr_ptr)
    {
        munmap(mixer->s_scr_ptr, mixer->s_product);
        mixer->s_scr_ptr = NULL;
    }
    if (mixer->s_rgb_ptr)
    {
        free(mixer->s_rgb_ptr);
        mixer->s_rgb_ptr = NULL;
    }

    if (mixer->s_fd)
    {
        close(mixer->s_fd);
        mixer->s_fd = 0;
    }
#endif

#if defined(PLATFORM_WIN32)
    sw_mixer_context_t *mixer = &s_mixer[instIdx];
    if (mixer->s_old_hobject)
    {
        SelectObject(mixer->s_hdc_memory, mixer->s_old_hobject);
        mixer->s_old_hobject = NULL;
    }
    if (mixer->s_hdc_memory)
    {
        DeleteDC(mixer->s_hdc_memory);
        mixer->s_hdc_memory = NULL;
    }
    if (mixer->s_dib_section)
    {
        DeleteObject(mixer->s_dib_section);
        mixer->s_dib_section = NULL;
    }

    if (mixer->s_hWnd)
    {
        DestroyWindow(mixer->s_hWnd);
        mixer->s_hWnd = NULL;
    }
#endif  //#if defined(PLATFORM_WIN32) 

}

#if defined(PLATFORM_WIN32)
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_CREATE:
        return (0);

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        EndPaint(hwnd, &ps);
        return (0);

    case WM_CLOSE:
        return (0);

    case WM_DESTROY:
        PostQuitMessage(0);
        return (0);
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

#endif //defined(PLATFORM_WIN32) 


