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
#include "jpeg_heap.h"

static int FillSdramBurst(BufInfo *pBufInfo, uint32_t targetAddr, PhysicalAddress bsBufStartAddr,
                          PhysicalAddress bsBufEndAddr, uint32_t size, int checkeos, int *streameos, int endian);
static int SyncSdramBurst(BufInfo *pBufInfo, uint32_t targetAddr, PhysicalAddress bsBufStartAddr,
                          PhysicalAddress bsBufEndAddr, uint32_t size,  int checkeos, int *streameos, int endian);
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


JpgRet SyncJpgBsBufHelper(JpgDecHandle handle, BufInfo *pBufInfo, PhysicalAddress paBsBufStart,
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


    fillSize = SyncSdramBurst(pBufInfo, paWrPtr, paBsBufStart, paBsBufEnd, fillSize, checkeos,
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

int SyncSdramBurst(BufInfo *pBufInfo, uint32_t targetAddr, PhysicalAddress bsBufStartAddr,
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
        // JpuWriteMem(targetAddr, pBuf, room, endian);
        // JpuWriteMem(bsBufStartAddr, pBuf + room, (pBufInfo->count - room), endian);
    }
    else
    {
        // JpuWriteMem(targetAddr, pBuf, pBufInfo->count, endian);
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




void ProcessEncodedBitstreamBurst(FILE *fp, int targetAddr, PhysicalAddress bsBufStartAddr,
                                  PhysicalAddress bsBufEndAddr, int size, int endian)
{
    uint8_t *val = 0;
    int room = 0;

    val = (uint8_t *)jpg_malloc(size);
    if ((targetAddr + size) > (int)bsBufEndAddr)
    {
        room = bsBufEndAddr - targetAddr;
        JpuReadMem(targetAddr, val, room,  endian);
        JpuReadMem(bsBufStartAddr, val + room, (size - room), endian);
    }
    else
    {
        JpuReadMem(targetAddr, val, size, endian);
    }

    if (fp)
    {
        fwrite(val, sizeof(uint8_t), size, fp);
        fflush(fp);
    }

    jpg_free(val);
}

/**
* Bitstream Read for encoders
*/
JpgRet ReadJpgBsBufHelper(JpgEncHandle handle,
                          FILE *bsFp,
                          PhysicalAddress paBsBufStart,
                          PhysicalAddress paBsBufEnd,
                          int encHeaderSize,
                          int endian)
{
    JpgRet ret = JPG_RET_SUCCESS;
    int loadSize = 0;
    int stuffSize;
    PhysicalAddress paRdPtr, paWrPtr;
    int size = 0;


    ret = JPU_EncGetBitstreamBuffer(handle, &paRdPtr, &paWrPtr, &size);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_EncGetBitstreamBuffer failed Error code is 0x%x \n", ret);
        goto LOAD_BS_ERROR;
    }

    if (size > 0)
    {
        stuffSize  = 0;
        if (encHeaderSize && (size + encHeaderSize) % 8)
        {
            stuffSize = (size + encHeaderSize) - ((size + encHeaderSize) / 8) * 8;
            stuffSize = 8 - stuffSize;
        }

        loadSize = size;

        if (loadSize > 0)
        {
            ProcessEncodedBitstreamBurst(bsFp, paRdPtr, paBsBufStart, paBsBufEnd, loadSize, endian);

            ret = JPU_EncUpdateBitstreamBuffer(handle, loadSize);
            if (ret != JPG_RET_SUCCESS)
            {
                DBG_DIRECT("VPU_EncUpdateBitstreamBuffer failed Error code is 0x%x \n", ret);
                goto LOAD_BS_ERROR;
            }
        }

    }

LOAD_BS_ERROR:

    return ret;
}


/******************************************************************************
    EncOpenParam Initialization
******************************************************************************/

static int getTblElement(FILE *fp, char *str)
{
    static int  LineNum = 1;

    while (1)
    {
        if (fgets(str, 256, fp) == NULL)
        {
            return 0;
        }
        if ((str[0] != ';') && (str[0] != '/'))
        {
            break;
        }
        LineNum++;
    }
    return 1;
}

static int parseHuffmanTable(FILE *fp, EncMjpgParam *param)
{
    char sLine[256] = {0,};
    unsigned int h[8] = {0,};
    BYTE *huffBit;
    BYTE *huffVal;
    int i, j;

    huffBit = param->huffBits[DC_TABLE_INDEX0];
    huffVal = param->huffVal[DC_TABLE_INDEX0];

    for (i = 0; i < 2; i++) // Luma DC BitLength
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);

        for (j = 0; j < 8; j++)
        {
            *huffBit++ = (BYTE)h[j];
        }
    }

    for (i = 0; i < (16 / 8); i++) // Luma DC HuffValue
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);

        for (j = 0; j < 8; j++)
        {
            *huffVal++ = (BYTE)h[j];
        }
    }

    huffBit = param->huffBits[AC_TABLE_INDEX0];
    huffVal = param->huffVal[AC_TABLE_INDEX0];

    for (i = 0; i < (16 / 8); i++) // Luma AC BitLength
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *huffBit++ = (BYTE)h[j];
        }
    }

    for (i = 0; i < (162 / 8); i++) // Luma DC HuffValue
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *huffVal++ = (BYTE)h[j];
        }
    }

    if (getTblElement(fp, sLine) == 0)
    {
        return 0;
    }

    memset(h, 0x00, 8 * sizeof(unsigned int));
    sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
    for (j = 0; j < 2; j++)
    {
        *huffVal++ = (BYTE)h[j];
    }


    huffBit = param->huffBits[DC_TABLE_INDEX1];
    huffVal = param->huffVal[DC_TABLE_INDEX1];

    for (i = 0; i < (16 / 8); i++) // Chroma DC BitLength
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *huffBit++ = (BYTE)h[j];
        }
    }

    for (i = 0; i < (16 / 8); i++) // Chroma DC HuffValue
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *huffVal++ = (BYTE)h[j];
        }
    }

    huffBit = param->huffBits[AC_TABLE_INDEX1];
    huffVal = param->huffVal[AC_TABLE_INDEX1];

    for (i = 0; i < (16 / 8); i++) // Chroma DC BitLength
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *huffBit++ = (BYTE)h[j];
        }
    }

    for (i = 0; i < (162 / 8); i++) // Luma DC HuffValue
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *huffVal++ = (BYTE)h[j];
        }
    }

    if (getTblElement(fp, sLine) == 0)
    {
        return 0;
    }

    memset(h, 0x00, 8 * sizeof(unsigned int));
    sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
    for (j = 0; j < 2; j++)
    {
        *huffVal++ = (BYTE)h[j];
    }

    return 1;
}

static int parseQMatrix(FILE *fp, EncMjpgParam *param)
{
    char sLine[256] = {0,};
    int i, j;
    unsigned int h[8] = {0,};
    BYTE temp_dc[64], temp_ac[64];
    BYTE *qTable_dc, *qTable_ac, *qTable;

    qTable = temp_dc;
    for (i = 0; i < (64 / 8); i++)
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *qTable++ = (BYTE)h[j];
        }
    }

    qTable = temp_ac;
    for (i = 0; i < (64 / 8); i++)
    {
        if (getTblElement(fp, sLine) == 0)
        {
            return 0;
        }

        memset(h, 0x00, 8 * sizeof(unsigned int));
        sscanf(sLine, "%x %x %x %x %x %x %x %x", &h[0], &h[1], &h[2], &h[3], &h[4], &h[5], &h[6], &h[7]);
        for (j = 0; j < 8; j++)
        {
            *qTable++ = (BYTE)h[j];
        }
    }

    qTable_dc = param->qMatTab[DC_TABLE_INDEX0];
    qTable_ac = param->qMatTab[AC_TABLE_INDEX0];

    for (i = 0; i < 64; i++)
    {
        qTable_dc[InvScanTable[i]] = temp_dc[i];
        qTable_ac[InvScanTable[i]] = temp_ac[i];
    }

    memcpy(param->qMatTab[DC_TABLE_INDEX1], param->qMatTab[DC_TABLE_INDEX0], 64);
    memcpy(param->qMatTab[AC_TABLE_INDEX1], param->qMatTab[AC_TABLE_INDEX0], 64);

    return 1;
}



int jpgGetHuffTable(char *huffFileName, EncMjpgParam *param)
{
    char huffFilePath[256];
    FILE *huffFp = NULL;
    if (huffFileName[0] != 0)
    {
        strcpy(huffFilePath, huffFileName);
        huffFp = fopen(huffFilePath, "rt");
        if (!huffFp)
        {
            // JLOG( ERR, "Can't open Huffman Table file %s \n", huffFilePath);
            return 0;
        }
        parseHuffmanTable(huffFp, param);
        fclose(huffFp);
    }
    else
    {
        // Rearrange and insert pre-defined Huffman table to deticated variable.
        memcpy(param->huffBits[DC_TABLE_INDEX0], lumaDcBits, 16);   // Luma DC BitLength
        memcpy(param->huffVal[DC_TABLE_INDEX0], lumaDcValue, 16);   // Luma DC HuffValue

        memcpy(param->huffBits[AC_TABLE_INDEX0], lumaAcBits, 16);   // Luma DC BitLength
        memcpy(param->huffVal[AC_TABLE_INDEX0], lumaAcValue, 162);  // Luma DC HuffValue

        memcpy(param->huffBits[DC_TABLE_INDEX1], chromaDcBits, 16); // Chroma DC BitLength
        memcpy(param->huffVal[DC_TABLE_INDEX1], chromaDcValue, 16); // Chroma DC HuffValue

        memcpy(param->huffBits[AC_TABLE_INDEX1], chromaAcBits, 16); // Chroma AC BitLength
        memcpy(param->huffVal[AC_TABLE_INDEX1], chromaAcValue, 162); // Chorma AC HuffValue
    }

    return 1;
}


int jpgGetQMatrix(char *qMatFileName, EncMjpgParam *param)
{
    char qMatFilePath[256];
    FILE *qMatFp = NULL;


    if (qMatFileName[0] != 0)
    {
        strcpy(qMatFilePath, qMatFileName);
        qMatFp = fopen(qMatFilePath, "rt");
        if (!qMatFp)
        {
            DBG_DIRECT("Can't open Q Matrix file %s \n", qMatFilePath);
            return 0;
        }
        parseQMatrix(qMatFp, param);
        fclose(qMatFp);
    }
    else
    {
        // Rearrange and insert pre-defined Q-matrix to deticated variable.
        memcpy(param->qMatTab[DC_TABLE_INDEX0], lumaQ2, 64);
        memcpy(param->qMatTab[AC_TABLE_INDEX0], chromaBQ2, 64);

        memcpy(param->qMatTab[DC_TABLE_INDEX1], param->qMatTab[DC_TABLE_INDEX0], 64);
        memcpy(param->qMatTab[AC_TABLE_INDEX1], param->qMatTab[AC_TABLE_INDEX0], 64);
    }

    return 1;
}


int getJpgEncOpenParamDefault(JpgEncOpenParam *pEncOP, EncConfigParam *pEncConfig)
{
    int ret;
    EncMjpgParam mjpgParam;

    memset(&mjpgParam, 0x00, sizeof(EncMjpgParam));


    {
        pEncOP->picWidth = pEncConfig->picWidth;
        pEncOP->picHeight = pEncConfig->picHeight;
    }

    pEncOP->restartInterval = 0;
    pEncOP->chromaInterleave = pEncConfig->chromaInterleave;
    pEncOP->packedFormat = pEncConfig->packedFormat;
    mjpgParam.sourceFormat = pEncConfig->mjpgChromaFormat;
    pEncOP->sourceFormat = mjpgParam.sourceFormat;
    if (pEncConfig->huffFileName[0] ==
        '0')                // In case of manual encoding option, pEncConfig->huffFileName is '0'.
    {
        memset(pEncConfig->huffFileName, 0, MAX_FILE_PATH);
    }
    ret = jpgGetHuffTable(pEncConfig->huffFileName, &mjpgParam);
    if (ret == 0) { return ret; }
    if (pEncConfig->qMatFileName[0] ==
        '0')                // In case of manual encoding option, pEncConfig->qMatFileName is '0'.
    {
        memset(pEncConfig->qMatFileName, 0, MAX_FILE_PATH);
    }
    ret = jpgGetQMatrix(pEncConfig->qMatFileName, &mjpgParam);
    if (ret == 0) { return ret; }


    memcpy(pEncOP->huffVal, mjpgParam.huffVal, 4 * 162);
    memcpy(pEncOP->huffBits, mjpgParam.huffBits, 4 * 256);
    memcpy(pEncOP->qMatTab, mjpgParam.qMatTab, 4 * 64);


    return 1;
}

int JpgEncSetupTables(JpgEncOpenParam *pop, int quality)
{
    int scale_factor;
    int i;
    long temp;
    const int force_baseline = 1;



    /* These are the sample quantization tables given in JPEG spec section K.1.
      * The spec says that the values given produce "good" quality, and
      * when divided by 2, "very good" quality.
      */
    static const unsigned int std_luminance_quant_tbl[64] =
    {
        16,  11,  10,  16,  24,  40,  51,  61,
        12,  12,  14,  19,  26,  58,  60,  55,
        14,  13,  16,  24,  40,  57,  69,  56,
        14,  17,  22,  29,  51,  87,  80,  62,
        18,  22,  37,  56,  68, 109, 103,  77,
        24,  35,  55,  64,  81, 104, 113,  92,
        49,  64,  78,  87, 103, 121, 120, 101,
        72,  92,  95,  98, 112, 100, 103,  99
    };
    static const unsigned int std_chrominance_quant_tbl[64] =
    {
        17,  18,  24,  47,  99,  99,  99,  99,
        18,  21,  26,  66,  99,  99,  99,  99,
        24,  26,  56,  99,  99,  99,  99,  99,
        47,  66,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99,
        99,  99,  99,  99,  99,  99,  99,  99
    };

    if (quality <= 0) { quality = 1; }
    if (quality > 100) { quality = 100; }

    /* The basic table is used as-is (scaling 100) for a quality of 50.
    * Qualities 50..100 are converted to scaling percentage 200 - 2*Q;
    * note that at Q=100 the scaling is 0, which will cause jpeg_add_quant_table
    * to make all the table entries 1 (hence, minimum quantization loss).
    * Qualities 1..50 are converted to scaling percentage 5000/Q.
    */
    if (quality < 50)
    {
        scale_factor = 5000 / quality;
    }
    else
    {
        scale_factor = 200 - quality * 2;
    }


    for (i = 0; i < 64; i++)
    {
        temp = ((long) std_luminance_quant_tbl[i] * scale_factor + 50L) / 100L;
        /* limit the values to the valid range */
        if (temp <= 0L) { temp = 1L; }
        if (temp > 32767L) { temp = 32767L; } /* max quantizer needed for 12 bits */
        if (force_baseline && temp > 255L)
        {
            temp = 255L;    /* limit to baseline range if requested */
        }

        pop->qMatTab[DC_TABLE_INDEX0][i] = (BYTE)temp;
    }

    for (i = 0; i < 64; i++)
    {
        temp = ((long) std_chrominance_quant_tbl[i] * scale_factor + 50L) / 100L;
        /* limit the values to the valid range */
        if (temp <= 0L) { temp = 1L; }
        if (temp > 32767L) { temp = 32767L; } /* max quantizer needed for 12 bits */
        if (force_baseline && temp > 255L)
        {
            temp = 255L;    /* limit to baseline range if requested */
        }

        pop->qMatTab[AC_TABLE_INDEX0][i] = (BYTE)temp;
    }

    //setting of qmatrix table information
//#define USE_CNM_DEFAULT_QMAT_TABLE
#ifdef USE_CNM_DEFAULT_QMAT_TABLE
    memset(&pop->qMatTab[DC_TABLE_INDEX0], 0x00, 64);
    memcpy(&pop->qMatTab[DC_TABLE_INDEX0], lumaQ2, 64);

    memset(&pop->qMatTab[AC_TABLE_INDEX0], 0x00, 64);
    memcpy(&pop->qMatTab[AC_TABLE_INDEX0], chromaBQ2, 64);
#endif

    memcpy(&pop->qMatTab[DC_TABLE_INDEX1], &pop->qMatTab[DC_TABLE_INDEX0], 64);
    memcpy(&pop->qMatTab[AC_TABLE_INDEX1], &pop->qMatTab[AC_TABLE_INDEX0], 64);

    //setting of huffman table information
    memset(&pop->huffBits[DC_TABLE_INDEX0], 0x00, 256);
    memcpy(&pop->huffBits[DC_TABLE_INDEX0], lumaDcBits, 16);        // Luma DC BitLength
    memset(&pop->huffVal[DC_TABLE_INDEX0], 0x00, 162);
    memcpy(&pop->huffVal[DC_TABLE_INDEX0], lumaDcValue, 16);        // Luma DC HuffValue

    memset(&pop->huffBits[AC_TABLE_INDEX0], 0x00, 256);
    memcpy(&pop->huffBits[AC_TABLE_INDEX0], lumaAcBits, 16);        // Luma AC BitLength
    memset(&pop->huffVal[AC_TABLE_INDEX0], 0x00, 162);
    memcpy(&pop->huffVal[AC_TABLE_INDEX0], lumaAcValue, 162);       // Luma AC HuffValue

    memset(&pop->huffBits[DC_TABLE_INDEX1], 0x00, 256);
    memcpy(&pop->huffBits[DC_TABLE_INDEX1], chromaDcBits, 16);      // Chroma DC BitLength
    memset(&pop->huffVal[DC_TABLE_INDEX1], 0x00, 162);
    memcpy(&pop->huffVal[DC_TABLE_INDEX1], chromaDcValue, 16);      // Chroma DC HuffValue

    memset(&pop->huffBits[AC_TABLE_INDEX1], 0x00, 256);
    memcpy(&pop->huffBits[AC_TABLE_INDEX1], chromaAcBits, 16);      // Chroma AC BitLength
    memset(&pop->huffVal[AC_TABLE_INDEX1], 0x00, 162);
    memcpy(&pop->huffVal[AC_TABLE_INDEX1], chromaAcValue, 162); // Chorma AC HuffValue

    return 1;
}


int LoadYuvImageBurstFormat(uint8_t *src, int picWidth, int picHeight,
                            int addrY, int addrCb, int addrCr, int stride, int interLeave, int format, int endian, int packed)
{
    int y, nY, nCb, nCr;
    int addr;
    int size;
    int lumaSize, chromaSize, chromaStride, chromaWidth;
    uint8_t *puc;

    switch (format)
    {
    case FORMAT_420:
        nY = picHeight;
        nCb = nCr = picHeight / 2;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_224:
        nY = picHeight;
        nCb = nCr = picHeight / 2;
        chromaSize = picWidth * picHeight / 2;
        chromaStride = stride;
        chromaWidth = picWidth;
        break;
    case FORMAT_422:
        nY = picHeight;
        nCb = nCr = picHeight;
        chromaSize = picWidth * picHeight / 2;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    case FORMAT_444:
        nY = picHeight;
        nCb = nCr = picHeight;
        chromaSize = picWidth * picHeight;
        chromaStride = stride;
        chromaWidth = picWidth;
        break;
    case FORMAT_400:
        nY = picHeight;
        nCb = nCr = 0;
        chromaSize = picWidth * picHeight / 4;
        chromaStride = stride / 2;
        chromaWidth = picWidth / 2;
        break;
    }


    puc = src;
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

    if (picWidth == stride) // for fast write
    {
        JpuWriteMem(addr, (uint8_t *)(puc), lumaSize, endian);

        if (format == FORMAT_400)
        {
            return size;
        }

        if (packed)
        {
            return size;
        }


        if (interLeave)
        {
            uint8_t t0, t1, t2, t3, t4, t5, t6, t7;
            int i, height, width;
            int stride;
            uint8_t *pTemp;
            uint8_t *dstAddrCb;
            uint8_t *dstAddrCr;

            addr   = addrCb;
            stride = chromaStride * 2;
            height = nCb;
            width  = chromaWidth * 2;

            dstAddrCb = (uint8_t *)(puc + picWidth * nY);
            dstAddrCr = (uint8_t *)(dstAddrCb + chromaSize);

            pTemp = jpg_malloc(width);
            if (!pTemp)
            {
                return 0;
            }

            for (y = 0; y < height; ++y)
            {
                for (i = 0; i < width; i += 8)
                {
                    t0 = *dstAddrCb++ ;
                    t2 = *dstAddrCb++ ;
                    t4 = *dstAddrCb++ ;
                    t6 = *dstAddrCb++ ;
                    t1 = *dstAddrCr++ ;
                    t3 = *dstAddrCr++ ;
                    t5 = *dstAddrCr++ ;
                    t7 = *dstAddrCr++ ;
                    if (interLeave == CBCR_INTERLEAVE)
                    {
                        pTemp[i]     = t0;
                        pTemp[i + 1] = t1;
                        pTemp[i + 2] = t2;
                        pTemp[i + 3] = t3;
                        pTemp[i + 4] = t4;
                        pTemp[i + 5] = t5;
                        pTemp[i + 6] = t6;
                        pTemp[i + 7] = t7;
                    }
                    else // CRCB_INTERLEAVE
                    {
                        pTemp[i]     = t1;
                        pTemp[i + 1] = t0;
                        pTemp[i + 2] = t3;
                        pTemp[i + 3] = t2;
                        pTemp[i + 4] = t5;
                        pTemp[i + 5] = t4;
                        pTemp[i + 6] = t7;
                        pTemp[i + 7] = t6;
                    }
                }
                JpuWriteMem(addr + stride * y, (unsigned char *)pTemp, width, endian);
            }

            jpg_free(pTemp);
        }
        else
        {
            puc = src + lumaSize;
            addr = addrCb;
            JpuWriteMem(addr, (uint8_t *)puc, chromaSize, endian);

            puc = src + lumaSize + chromaSize;
            addr = addrCr;
            JpuWriteMem(addr, (uint8_t *)puc, chromaSize, endian);
        }

    }
    else
    {
        for (y = 0; y < nY; ++y)
        {
            JpuWriteMem(addr + stride * y, (uint8_t *)(puc + y * picWidth), picWidth, endian);
        }


        if (format == FORMAT_400)
        {
            return size;
        }

        if (packed)
        {
            return size;
        }

        if (interLeave == 1)
        {
            uint8_t t0, t1, t2, t3, t4, t5, t6, t7;
            int i, width, height, stride;
            uint8_t *pTemp;
            uint8_t *dstAddrCb;
            uint8_t *dstAddrCr;

            addr = addrCb;
            stride = chromaStride * 2;
            height = nCb / 2;
            width  = chromaWidth * 2;

            dstAddrCb = (uint8_t *)(puc + picWidth * nY);
            dstAddrCr = (uint8_t *)(dstAddrCb + chromaSize);

            pTemp = jpg_malloc((width + 7) & ~7);
            if (!pTemp)
            {
                return 0;
            }

            // it may be not occur that pic_width in not 8byte alined.
            for (y = 0; y < height; ++y)
            {
                for (i = 0; i < width; i += 8)
                {
                    t0 = *dstAddrCb++ ;
                    t2 = *dstAddrCb++ ;
                    t4 = *dstAddrCb++ ;
                    t6 = *dstAddrCb++ ;
                    t1 = *dstAddrCr++ ;
                    t3 = *dstAddrCr++ ;
                    t5 = *dstAddrCr++ ;
                    t7 = *dstAddrCr++ ;

                    if (interLeave == CBCR_INTERLEAVE)
                    {
                        pTemp[i]     = t0;
                        pTemp[i + 1] = t1;
                        pTemp[i + 2] = t2;
                        pTemp[i + 3] = t3;
                        pTemp[i + 4] = t4;
                        pTemp[i + 5] = t5;
                        pTemp[i + 6] = t6;
                        pTemp[i + 7] = t7;
                    }
                    else // CRCB_INTERLEAVE
                    {
                        pTemp[i]     = t1;
                        pTemp[i + 1] = t0;
                        pTemp[i + 2] = t3;
                        pTemp[i + 3] = t2;
                        pTemp[i + 4] = t5;
                        pTemp[i + 5] = t3;
                        pTemp[i + 6] = t7;
                        pTemp[i + 7] = t6;
                    }

                    JpuWriteMem(addr + stride * y, (unsigned char *)pTemp, stride, endian);
                }
            }

        }
        else
        {
            puc = src + lumaSize;
            addr = addrCb;
            for (y = 0; y < nCb; ++y)
            {
                JpuWriteMem(addr + chromaStride * y, (uint8_t *)(puc + y * chromaWidth), chromaWidth, endian);
            }

            puc = src + lumaSize + chromaSize;
            addr = addrCr;
            for (y = 0; y < nCr; ++y)
            {
                JpuWriteMem(addr + chromaStride * y, (uint8_t *)(puc + y * chromaWidth), chromaWidth, endian);
            }
        }

    }

    return size;
}
int LoadYuvPartialImageHelperFormat(FILE *yuvFp,
                                    uint8_t *pYuv,
                                    PhysicalAddress addrY,
                                    PhysicalAddress addrCb,
                                    PhysicalAddress addrCr,
                                    int picWidth,
                                    int picHeight,
                                    int picHeightPartial,
                                    int stride,
                                    int interleave,
                                    int format,
                                    int endian,
                                    int partPosIdx,
                                    int frameIdx,
                                    int packed)
{

    int LumaPicSize;
    int ChromaPicSize;
    int LumaPartialSize;
    int ChromaPartialSize;
    int pos;
    int divX, divY;
    int frameSize;

    divX = format == FORMAT_420 || format == FORMAT_422 ? 2 : 1;
    divY = format == FORMAT_420 || format == FORMAT_224 ? 2 : 1;

    LumaPicSize   = picWidth * picHeight;
    ChromaPicSize = LumaPicSize / divX / divY;

    LumaPartialSize   = picWidth * picHeightPartial;
    ChromaPartialSize = LumaPartialSize / divX / divY;

    if (format == FORMAT_400)
    {
        frameSize     = LumaPicSize;
    }
    else
    {
        frameSize     = LumaPicSize + ChromaPicSize * 2;
    }
    // Load source one picture image to encode to SDRAM frame buffer.

    if (packed)
    {
        if (packed == PACKED_FORMAT_444)
        {
            LumaPicSize   = picWidth * 3 * picHeight;
            LumaPartialSize = picWidth * 3 * picHeightPartial;
        }
        else
        {
            LumaPicSize   = picWidth * 2 * picHeight;
            LumaPartialSize = picWidth * 2 * picHeightPartial;
        }
        frameSize = LumaPicSize;
        ChromaPicSize = 0;
        ChromaPartialSize = 0;
    }

    // Y
    fseek(yuvFp, (frameIdx * frameSize), SEEK_SET);
    pos = LumaPartialSize * partPosIdx;
    fseek(yuvFp, pos, SEEK_CUR);

    if (!fread(pYuv, 1, LumaPartialSize, yuvFp))
    {
        if (!feof(yuvFp))
        {
            DBG_DIRECT("Yuv Data fread failed file handle is 0x%x \n", yuvFp);
        }
        return 0;
    }

    if (format != FORMAT_400 && packed == 0)
    {
        // Cb
        fseek(yuvFp, (frameIdx * frameSize), SEEK_SET);
        pos = LumaPicSize + ChromaPartialSize * partPosIdx;
        fseek(yuvFp, pos, SEEK_CUR);

        if (!fread(pYuv + LumaPartialSize, 1, ChromaPartialSize, yuvFp))
        {
            if (!feof(yuvFp))
            {
                DBG_DIRECT("Yuv Data fread failed file handle is 0x%x \n", yuvFp);
            }
            return 0;
        }

        // Cr
        fseek(yuvFp, (frameIdx * frameSize), SEEK_SET);
        pos = LumaPicSize + ChromaPicSize + ChromaPartialSize * partPosIdx;
        fseek(yuvFp, pos, SEEK_CUR);

        if (!fread(pYuv + LumaPartialSize + ChromaPartialSize, 1, ChromaPartialSize, yuvFp))
        {
            if (!feof(yuvFp))
            {
                DBG_DIRECT("Yuv Data fread failed file handle is 0x%x \n", yuvFp);
            }
            return 0;
        }
    }

    LoadYuvImageBurstFormat(pYuv,
                            picWidth,
                            picHeightPartial,
                            addrY,
                            addrCb,
                            addrCr,
                            stride,
                            interleave,
                            format,
                            endian,
                            packed);

    return 1;
}

/******************************************************************************
    DPB Image Data Control
******************************************************************************/

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
                             int packed)
{
    int frameSize;

    switch (format)
    {
    case FORMAT_420:
        frameSize = picWidth * picHeight * 3 / 2;
        break;
    case FORMAT_224:
        frameSize = picWidth * picHeight * 4 / 2;
        break;
    case FORMAT_422:
        frameSize = picWidth * picHeight * 4 / 2;
        break;
    case FORMAT_444:
        frameSize = picWidth * picHeight * 6 / 2;
        break;
    case FORMAT_400:
        frameSize = picWidth * picHeight;
        break;
    }

    if (packed == PACKED_FORMAT_NONE)
    {
        frameSize = frameSize;
    }
    else if (packed == PACKED_FORMAT_444)
    {
        frameSize = picWidth * 3 * picHeight ;
    }
    else// PACKED_FORMAT_422_XXXX
    {
        frameSize = picWidth * 2 * picHeight ;
    }
    // Load source one picture image to encode to SDRAM frame buffer.
    // memcpy(pYuv, pic_yuv, frameSize);
    // if (!fread(pYuv, 1, frameSize, yuvFp))
    // {
    //  if( !feof( yuvFp ) )
    //      DBG_DIRECT( "Yuv Data fread failed file handle is 0x%x \n", yuvFp );
    //  return 0;
    // }


    LoadYuvImageBurstFormat(pYuv,
                            picWidth,
                            picHeight,
                            addrY,
                            addrCb,
                            addrCr,
                            stride,
                            interleave,
                            format,
                            endian
                            , packed);

    return 1;
}