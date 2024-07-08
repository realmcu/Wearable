
// File: jpuhelper.h
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef _JPU_HELPER_H_
#define _JPU_HELPER_H_

#include <stdio.h>
#include "jpuapi.h"


#if defined (__cplusplus)
extern "C" {
#endif


JpgRet WriteJpgBsBufHelper(JpgDecHandle handle,
                           BufInfo *pBufInfo,
                           PhysicalAddress paBsBufStart,
                           PhysicalAddress paBsBufEnd,
                           int defaultsize,
                           int checkeos,
                           int *pstreameos,
                           int endian);

int WriteBsBufFromBufHelper(JpgDecHandle handle,
                            jpu_buffer_t *pJbStream,
                            BYTE *pChunk,
                            int chunkSize,
                            int endian);


int LoadYuvImageHelperFormat(uint8_t *pic_yuv,
                             uint8_t *pYuv,
                             PhysicalAddress addrY,
                             PhysicalAddress addrCb,
                             PhysicalAddress addrCr,
                             int picWidth,
                             int picHeight,
                             int stride,
                             int interleave,
                             int format,
                             int endian,
                             int packed);



int SaveYuvImageHelperFormat(void *yuvFp,
                             uint8_t *pYuv,
                             PhysicalAddress addrY,
                             PhysicalAddress addrCb,
                             PhysicalAddress addrCr,
                             int picWidth,
                             int picHeight,
                             int stride,
                             int interLeave,
                             int format,
                             int endian,
                             int packed);

int SaveYuvPartialImageHelperFormat(void *yuvFp,
                                    uint8_t *pYuv,
                                    PhysicalAddress addrY,
                                    PhysicalAddress addrCb,
                                    PhysicalAddress addrCr,
                                    int picWidth,
                                    int picHeight,
                                    int picHeightPartial,
                                    int stride,
                                    int interLeave,
                                    int format,
                                    int endian,
                                    int partPosIdx,
                                    int frameIdx,
                                    int packed);

int GetFrameBufSize(int framebufFormat, int picWidth, int picHeight);
void GetMcuUnitSize(int format, int *mcuWidth, int *mcuHeight);

typedef enum { YUV444, YUV422, YUV420, NV12, NV21, YUV400, YUYV, YVYU, UYVY, VYUY, YYY, RGB_PLANAR, RGB32, RGB24, RGB16 } yuv2rgb_color_format;
void jpu_yuv2rgb(int width, int height, yuv2rgb_color_format format, unsigned char *src,
                 unsigned char *rgba, int chroma_reverse);
yuv2rgb_color_format convert_jpuapi_format_to_yuv2rgb_color_format(int planar_format,
                                                                   int pack_format, int interleave);
#if defined (__cplusplus)
}
#endif

#endif //#ifndef _JPU_HELPER_H_
