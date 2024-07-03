//------------------------------------------------------------------------------
// File: vpureport.c
//
// Copyright (c) 2006, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "regdefine.h"
#include "jpuapi.h"
#include "jpuapifunc_rt.h"
#include "jputable.h"


#include "trace.h"
#include "jpuhelper_rt.h"

static int FillSdramBurst(BufInfo *pBufInfo, uint32_t targetAddr, PhysicalAddress bsBufStartAddr,
                          PhysicalAddress bsBufEndAddr, uint32_t size, int checkeos, int *streameos, int endian);
static int StoreYuvImageBurstFormat(uint8_t *dst, int picWidth, int picHeight, int addrY,
                                    int addrCb,
                                    int addrCr,  int stride,  int interLeave, int format, int endian, int packed);


// Figure A.6 - Zig-zag sequence of quantized DCT coefficients
const int InvScanTable[64] =
{
    0,  1,  5,  6, 14, 15, 27, 28,
    2,  4,  7, 13, 16, 26, 29, 42,
    3,  8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

const int ScanTable[64] =
{
    0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};







JpgRet WriteJpgBsBufHelper(JpgDecHandle handle, BufInfo *pBufInfo, PhysicalAddress paBsBufStart,
                           PhysicalAddress paBsBufEnd, int defaultsize, int checkeos, int *pstreameos, int endian)
{
    JpgRet ret = JPG_RET_SUCCESS;
    int size = 0;
    int fillSize = 0;
    PhysicalAddress paRdPtr, paWrPtr;

    ret = JPU_DecGetBitstreamBuffer(handle, &paRdPtr, &paWrPtr, &size);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("VPU_DecGetBitstreamBuffer failed Error code is 0x%x \n", ret);
        goto FILL_BS_ERROR;
    }

    if (size <= 0)
    {
        return JPG_RET_SUCCESS;
    }

    if (defaultsize)
    {
        if (size < defaultsize)
        {
            fillSize = size;
        }
        else
        {
            fillSize = defaultsize;
        }
    }
    else
    {
        fillSize = size;
    }


    fillSize = FillSdramBurst(pBufInfo, paWrPtr, paBsBufStart, paBsBufEnd, fillSize, checkeos,
                              pstreameos, endian);

    if (*pstreameos == 0)
    {
        ret = JPU_DecUpdateBitstreamBuffer(handle, fillSize);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("VPU_DecUpdateBitstreamBuffer failed Error code is 0x%x \n", ret);
            goto FILL_BS_ERROR;
        }

        if ((pBufInfo->size - pBufInfo->point) <= 0)
        {
            ret = JPU_DecUpdateBitstreamBuffer(handle, STREAM_END_SIZE) ;
            if (ret != JPG_RET_SUCCESS)
            {
                DBG_DIRECT("VPU_DecUpdateBitstreamBuffer failed Error code is 0x%x \n", ret);
                goto FILL_BS_ERROR;
            }

            pBufInfo->fillendbs = 1;
        }
    }
    else
    {
        if (!pBufInfo->fillendbs)
        {
            ret = JPU_DecUpdateBitstreamBuffer(handle, STREAM_END_SIZE) ;
            if (ret != JPG_RET_SUCCESS)
            {
                DBG_DIRECT("VPU_DecUpdateBitstreamBuffer failed Error code is 0x%x \n", ret);
                goto FILL_BS_ERROR;
            }
            pBufInfo->fillendbs = 1;
        }
    }

FILL_BS_ERROR:

    return ret;
}

/******************************************************************************
    DPB Image Data Control
******************************************************************************/


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
                             int packed)
{
    int frameSize;
    frameSize = StoreYuvImageBurstFormat(pYuv, picWidth, picHeight,
                                         addrY,
                                         addrCb,
                                         addrCr,
                                         stride,
                                         interLeave,
                                         format,
                                         endian,
                                         packed);

    return 1;
}

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
                                    int packed)
{
    int LumaPicSize;
    int ChromaPicSize;
    int frameSize;

    int LumaPartialSize;
    int ChromaPartialSize;

    int pos;



    switch (format)
    {
    case FORMAT_420:
        LumaPicSize   = picWidth * ((picHeight + 1) / 2 * 2);
        ChromaPicSize = ((picWidth + 1) / 2) * ((picHeight + 1) / 2);
        frameSize     = LumaPicSize + ChromaPicSize * 2;

        LumaPartialSize   = picWidth * ((picHeightPartial + 1) / 2 * 2);
        ChromaPartialSize = ((picWidth + 1) / 2) * ((picHeightPartial + 1) / 2);
        if (interLeave)
        {
            ChromaPartialSize = (((picWidth + 1) / 2) * 2) * ((picHeightPartial + 1) / 2);
        }
        break;
    case FORMAT_224:
        LumaPicSize   = picWidth * ((picHeight + 1) / 2 * 2);
        ChromaPicSize = picWidth * ((picHeight + 1) / 2);
        frameSize     = LumaPicSize + ChromaPicSize * 2;

        LumaPartialSize   = picWidth * ((picHeightPartial + 1) / 2 * 2);
        ChromaPartialSize = picWidth * ((picHeightPartial + 1) / 2);
        if (interLeave)
        {
            ChromaPartialSize = picWidth * 2 * ((picHeightPartial + 1) / 2);
        }
        break;
    case FORMAT_422:
        LumaPicSize   = picWidth * picHeight;
        ChromaPicSize = ((picWidth + 1) / 2) * picHeight;
        frameSize     = LumaPicSize + ChromaPicSize * 2;

        LumaPartialSize   = picWidth * picHeightPartial;
        ChromaPartialSize = ((picWidth + 1) / 2) * picHeightPartial;
        if (interLeave)
        {
            ChromaPartialSize = (((picWidth + 1) / 2) * 2) * picHeightPartial;
        }
        break;
    case FORMAT_444:
        LumaPicSize   = picWidth * picHeight;
        ChromaPicSize = picWidth * picHeight;
        frameSize     = LumaPicSize + ChromaPicSize * 2;

        LumaPartialSize   = picWidth * picHeightPartial;
        ChromaPartialSize = picWidth * picHeightPartial;
        if (interLeave)
        {
            ChromaPartialSize = picWidth * 2 * picHeightPartial;
        }
        break;
    case FORMAT_400:
        LumaPicSize   = picWidth * picHeight;
        ChromaPicSize = 0;
        frameSize     = LumaPicSize + ChromaPicSize * 2;

        LumaPartialSize   = picWidth * picHeightPartial;
        ChromaPartialSize = 0;
        break;
    }

    if (packed)
    {
        if (packed == PACKED_FORMAT_444)
        {
            picWidth *= 3;
        }
        else
        {
            picWidth *= 2;
        }

        LumaPicSize   = picWidth * picHeight;
        ChromaPicSize = 0;
        frameSize = LumaPicSize;
        LumaPartialSize = picWidth * picHeightPartial;
        ChromaPartialSize = 0;
    }


    StoreYuvImageBurstFormat(pYuv, picWidth, picHeightPartial,
                             addrY,
                             addrCb,
                             addrCr,
                             stride,
                             interLeave,
                             format,
                             endian,
                             packed);


#if 0
    if (yuvFp)
    {
        // Y
        fseek(yuvFp, (frameIdx * frameSize), SEEK_SET);
        pos = LumaPartialSize * partPosIdx;
        fseek(yuvFp, pos, SEEK_CUR);
        if (!fwrite(pYuv, sizeof(uint8_t), LumaPartialSize, yuvFp))
        {
            DBG_DIRECT("Frame Data fwrite failed file handle is 0x%x \n", yuvFp);
            return 0;
        }

        if (packed)
        {
            fflush(yuvFp);
            return 1;
        }

        if (format != FORMAT_400)
        {
            // Cb
            fseek(yuvFp, (frameIdx * frameSize), SEEK_SET);
            pos = LumaPicSize + ChromaPartialSize * partPosIdx;
            fseek(yuvFp, pos, SEEK_CUR);
            if (!fwrite(pYuv + LumaPartialSize, sizeof(uint8_t), ChromaPartialSize, yuvFp))
            {
                DBG_DIRECT("Frame Data fwrite failed file handle is 0x%x \n", yuvFp);
                return 0;
            }

            if (interLeave)
            {
                fflush(yuvFp);
                return 1;
            }

            //Cr
            fseek(yuvFp, (frameIdx * frameSize), SEEK_SET);
            pos = LumaPicSize + ChromaPicSize + ChromaPartialSize * partPosIdx;
            fseek(yuvFp, pos, SEEK_CUR);
            if (!fwrite(pYuv + LumaPartialSize + ChromaPartialSize, sizeof(uint8_t), ChromaPartialSize, yuvFp))
            {
                DBG_DIRECT("Frame Data fwrite failed file handle is 0x%x \n", yuvFp);
                return 0;
            }
        }
        fflush(yuvFp);
    }
#endif
    return 1;
}

/******************************************************************************
    JPEG specific Helper
******************************************************************************/

int GetFrameBufSize(int framebufFormat, int picWidth, int picHeight)
{
    int framebufSize = 0;
    int framebufWidth, framebufHeight;

    if (framebufFormat == FORMAT_420 || framebufFormat == FORMAT_422)
    {
        framebufWidth = ((picWidth + 15) / 16) * 16;
    }
    else
    {
        framebufWidth = ((picWidth + 7) / 8) * 8;
    }

    if (framebufFormat == FORMAT_420 || framebufFormat == FORMAT_224)
    {
        framebufHeight = ((picHeight + 15) / 16) * 16;
    }
    else
    {
        framebufHeight = ((picHeight + 7) / 8) * 8;
    }


    switch (framebufFormat)
    {
    case FORMAT_420:
        framebufSize = framebufWidth * ((framebufHeight + 1) / 2 * 2) + ((framebufWidth + 1) / 2) * ((
                           framebufHeight + 1) / 2) * 2;
        break;
    case FORMAT_224:
        framebufSize = framebufWidth * ((framebufHeight + 1) / 2 * 2) + framebufWidth * ((
                           framebufHeight + 1) / 2) * 2;
        break;
    case FORMAT_422:
        framebufSize = framebufWidth * framebufHeight + ((framebufWidth + 1) / 2) * framebufHeight * 2;
        break;
    case FORMAT_444:
        framebufSize = framebufWidth * framebufHeight * 3;
        break;
    case FORMAT_400:
        framebufSize = framebufWidth * framebufHeight;
        break;
    }

    framebufSize = ((framebufSize + 7) & ~7);

    return framebufSize;
}



int FillSdramBurst(BufInfo *pBufInfo, uint32_t targetAddr, PhysicalAddress bsBufStartAddr,
                   PhysicalAddress bsBufEndAddr, uint32_t size,  int checkeos, int *streameos, int endian)
{
    uint8_t *pBuf;
    int room;

    pBufInfo->count = 0;

    if (checkeos == 1 && (pBufInfo->point >= pBufInfo->size))
    {
        *streameos = 1;
        return 0;
    }

    if ((pBufInfo->size - pBufInfo->point) < (int)size)
    {
        pBufInfo->count = (pBufInfo->size - pBufInfo->point);
    }
    else
    {
        pBufInfo->count = size;
    }

    pBuf = pBufInfo->buf + pBufInfo->point;
    if ((targetAddr + pBufInfo->count) > bsBufEndAddr)
    {
        room = bsBufEndAddr - targetAddr;
        JpuWriteMem(targetAddr, pBuf, room, endian);
        JpuWriteMem(bsBufStartAddr, pBuf + room, (pBufInfo->count - room), endian);
    }
    else
    {
        JpuWriteMem(targetAddr, pBuf, pBufInfo->count, endian);
    }

    pBufInfo->point += pBufInfo->count;
    return pBufInfo->count;
}


int StoreYuvImageBurstFormat(uint8_t *dst, int picWidth, int picHeight,
                             int addrY, int addrCb, int addrCr, int stride, int interLeave, int format, int endian, int packed)
{
    int size;
    int y, nY, nCb, nCr;
    int addr;
    int lumaSize, chromaSize, chromaStride, chromaWidth, chromaHeight;

    uint8_t *puc;

    switch (format)
    {
    case FORMAT_420:
        nY = (picHeight + 1) / 2 * 2;
        nCb = nCr = (picHeight + 1) / 2;
        chromaSize = ((picWidth + 1) / 2) * ((picHeight + 1) / 2);
        chromaStride = stride / 2;
        chromaWidth = (picWidth + 1) / 2;
        chromaHeight = nY;
        break;
    case FORMAT_224:
        nY = (picHeight + 1) / 2 * 2;
        nCb = nCr = (picHeight + 1) / 2;
        chromaSize = (picWidth) * ((picHeight + 1) / 2);
        chromaStride = stride;
        chromaWidth = picWidth;
        chromaHeight = nY;
        break;
    case FORMAT_422:
        nY = picHeight;
        nCb = nCr = picHeight;
        chromaSize = ((picWidth + 1) / 2) * picHeight ;
        chromaStride = stride / 2;
        chromaWidth = (picWidth + 1) / 2;
        chromaHeight = nY * 2;
        break;
    case FORMAT_444:
        nY = picHeight;
        nCb = nCr = picHeight;
        chromaSize = picWidth * picHeight;
        chromaStride = stride;
        chromaWidth = picWidth;
        chromaHeight = nY * 2;
        break;
    case FORMAT_400:
        nY = picHeight;
        nCb = nCr = 0;
        chromaSize = 0;
        chromaStride = 0;
        chromaWidth = 0;
        chromaHeight = 0;
        break;
    }

    puc = dst;
    addr = addrY;

    if (packed)
    {
        if (packed == PACKED_FORMAT_444)
        {
            picWidth *= 3;
        }
        else
        {
            picWidth *= 2;
        }

        chromaSize = 0;
    }

    lumaSize = picWidth * nY;

    size = lumaSize + chromaSize * 2;
    DBG_DIRECT("picWidth %d picHeight %d nY %d lumaSize %d chromaSize*2 %d", picWidth, picHeight, nY,
               lumaSize, chromaSize * 2);

    if (picWidth == stride)
    {
        DBG_DIRECT("addr 0x%x", addr);
        JpuReadMem(addr, (uint8_t *)(puc), lumaSize, endian);

        if (packed)
        {
            return size;
        }

        if (interLeave)
        {
            puc = dst + lumaSize;
            addr = addrCb;
            JpuReadMem(addr, (uint8_t *)(puc), chromaSize * 2, endian);
        }
        else
        {
            puc = dst + lumaSize;
            addr = addrCb;
            JpuReadMem(addr, (uint8_t *)(puc), chromaSize, endian);

            puc = dst + lumaSize + chromaSize;
            addr = addrCr;
            JpuReadMem(addr, (uint8_t *)(puc), chromaSize, endian);
        }
    }
    else
    {
        for (y = 0; y < nY; ++y)
        {
            JpuReadMem(addr + stride * y, (uint8_t *)(puc + y * picWidth), picWidth,  endian);
        }

        if (packed)
        {
            return size;
        }

        if (interLeave)
        {
            puc = dst + lumaSize;
            addr = addrCb;
            for (y = 0; y < (chromaHeight / 2); ++y)
            {
                JpuReadMem(addr + (chromaStride * 2)*y, (uint8_t *)(puc + y * (chromaWidth * 2)), (chromaWidth * 2),
                           endian);
            }
        }
        else
        {
            puc = dst + lumaSize;
            addr = addrCb;
            for (y = 0; y < nCb; ++y)
            {
                JpuReadMem(addr + chromaStride * y, (uint8_t *)(puc + y * chromaWidth), chromaWidth,  endian);
            }

            puc = dst + lumaSize + chromaSize;
            addr = addrCr;
            for (y = 0; y < nCr; ++y)
            {
                JpuReadMem(addr + chromaStride * y, (uint8_t *)(puc + y * chromaWidth), chromaWidth,  endian);
            }
        }

    }


    return size;
}

