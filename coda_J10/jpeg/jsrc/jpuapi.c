//--=========================================================================--
//  This file is a part of VPU Reference API project
//-----------------------------------------------------------------------------
//
//       This confidential and proprietary software may be used only
//     as authorized by a licensing agreement from Chips&Media Inc.
//     In the event of publication, the following notice is applicable:
//
//            (C) COPYRIGHT 2006 - 2011  CHIPS&MEDIA INC.
//                      ALL RIGHTS RESERVED
//
//       The entire notice above must be reproduced on all authorized
//       copies.
//
//--=========================================================================--

#include "jpuapifunc_rt.h"
#include "jpuapi.h"

int JPU_IsBusy()
{
    uint32_t val;
    val = JpuReadReg(MJPEG_PIC_STATUS_REG);

    if ((val & (1 << INT_JPU_DONE)) ||
        (val & (1 << INT_JPU_ERROR)))
    {
        return 0;
    }

    return 1;
}
void JPU_ClrStatus(uint32_t val)
{
    if (val != 0)
    {
        JpuWriteReg(MJPEG_PIC_STATUS_REG, val);
    }
}

uint32_t JPU_GetStatus()
{
    return JpuReadReg(MJPEG_PIC_STATUS_REG);
}


uint32_t JPU_IsInit()
{
    jpu_instance_pool_t *pjip;

    pjip = (jpu_instance_pool_t *)jdi_get_instance_pool();

    if (!pjip)
    {
        return 0;
    }

    return 1;
}

uint32_t  JPU_WaitInterrupt(int timeout)
{
    uint32_t reason = 0;

    reason = jdi_wait_interrupt(timeout);

    JpgSetClockGate(1);

    if (reason != (uint32_t) - 1)
    {
        reason = JpuReadReg(MJPEG_PIC_STATUS_REG);
    }


    JpgSetClockGate(0);

    return reason;
}

int JPU_GetOpenInstanceNum()
{
    jpu_instance_pool_t *pjip;

    pjip = (jpu_instance_pool_t *)jdi_get_instance_pool();
    if (!pjip)
    {
        return -1;
    }

    return pjip->jpu_instance_num;
}

JpgRet JPU_Init()
{
    jpu_instance_pool_t *pjip;

    if (jdi_init() < 0)
    {
        return JPG_RET_FAILURE;
    }


    pjip = (jpu_instance_pool_t *)jdi_get_instance_pool();
    if (!pjip)
    {
        return JPG_RET_FAILURE;
    }


    InitJpgInstancePool();
    JPU_SWReset();
    return JPG_RET_SUCCESS;
}
void JPU_DeInit()
{
    jdi_release();
}

JpgRet JPU_GetVersionInfo(uint32_t *versionInfo)
{

    if (JPU_IsInit() == 0)
    {
        return JPG_RET_NOT_INITIALIZED;
    }
    JpgEnterLock();
    *versionInfo = JpuReadReg(MJPEG_VERSION_INFO_REG);
    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecOpen(JpgDecHandle *pHandle, JpgDecOpenParam *pop)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;


    ret = CheckJpgDecOpenParam(pop);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    JpgEnterLock();

    ret = GetJpgInstance(&pJpgInst);
    if (ret == JPG_RET_FAILURE)
    {
        *pHandle = 0;
        JpgLeaveLock();
        return JPG_RET_FAILURE;
    }

    *pHandle = pJpgInst;

    pDecInfo = &pJpgInst->JpgInfo.decInfo;
    memset(pDecInfo, 0x00, sizeof(JpgDecInfo));

    pDecInfo->streamWrPtr = pop->bitstreamBuffer;
    pDecInfo->streamRdPtr = pop->bitstreamBuffer;

    pDecInfo->streamBufStartAddr = pop->bitstreamBuffer;
    pDecInfo->streamBufSize = pop->bitstreamBufferSize;
    pDecInfo->streamBufEndAddr = pop->bitstreamBuffer + pop->bitstreamBufferSize;
    JpuWriteReg(MJPEG_BBC_BAS_ADDR_REG, pDecInfo->streamBufStartAddr);
    JpuWriteReg(MJPEG_BBC_END_ADDR_REG, pDecInfo->streamBufEndAddr);
    JpuWriteReg(MJPEG_BBC_RD_PTR_REG, pDecInfo->streamRdPtr);
    JpuWriteReg(MJPEG_BBC_WR_PTR_REG, pDecInfo->streamWrPtr);
    JpuWriteReg(MJPEG_BBC_STRM_CTRL_REG, 0);

    pDecInfo->pBitStream = pop->pBitStream;

    pDecInfo->streamEndian = pop->streamEndian;
    pDecInfo->frameEndian = pop->frameEndian;
    pDecInfo->chromaInterleave = pop->chromaInterleave;
    pDecInfo->packedFormat = pop->packedFormat;
    pDecInfo->roiEnable = pop->roiEnable;
    pDecInfo->roiWidth = pop->roiWidth;
    pDecInfo->roiHeight = pop->roiHeight;
    pDecInfo->roiOffsetX = pop->roiOffsetX;
    pDecInfo->roiOffsetY = pop->roiOffsetY;

    // pDecInfo->hwVersion = JpuReadReg(MJPEG_VERSION_INFO_REG);

    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecClose(JpgDecHandle handle)
{
    JpgInst *pJpgInst;
    JpgRet ret;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    pJpgInst = handle;

    JpgEnterLock();
    FreeJpgInstance(pJpgInst);
    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecGetInitialInfo(JpgDecHandle handle, JpgDecInitialInfo *info)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;
    int32_t val;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    if (info == 0)
    {
        return JPG_RET_INVALID_PARAM;
    }
    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;

    val = JpegDecodeHeader(pDecInfo);
    if (val <= 0)
    {
        if (val == -3)
        {
            return JPG_RET_NOT_SUPPORTED_FORMAT;
        }
        else
        {
            return JPG_RET_FAILURE;
        }
    }

    info->picWidth = pDecInfo->picWidth;
    info->picHeight = pDecInfo->picHeight;
    info->minFrameBufferCount = 1;
    info->sourceFormat = pDecInfo->format;
    info->ecsPtr = pDecInfo->ecsPtr;

    pDecInfo->initialInfoObtained = 1;
    pDecInfo->minFrameBufferNum = 1;

#ifdef MJPEG_ERROR_CONCEAL
    pDecInfo->curRstIdx = 0;
    pDecInfo->nextRstIdx = 0;
#endif
    DBG_DIRECT("JPU_DecGetInitialInfo w:%d h:%d format:%d\n", pDecInfo->picWidth, pDecInfo->picHeight,
               pDecInfo->format);
    if ((pDecInfo->packedFormat == PACKED_FORMAT_444) && (pDecInfo->format != FORMAT_444))
    {
        return JPG_RET_INVALID_PARAM;
    }

    if (pDecInfo->roiEnable)
    {

        pDecInfo->roiMcuWidth = pDecInfo->roiWidth / pDecInfo->mcuWidth;
        pDecInfo->roiMcuHeight = pDecInfo->roiHeight / pDecInfo->mcuHeight;
        pDecInfo->roiMcuOffsetX = pDecInfo->roiOffsetX / pDecInfo->mcuWidth;
        pDecInfo->roiMcuOffsetY = pDecInfo->roiOffsetY / pDecInfo->mcuHeight;

        if ((pDecInfo->roiOffsetX > pDecInfo->alignedWidth)
            || (pDecInfo->roiOffsetY > pDecInfo->alignedHeight)
            || (pDecInfo->roiOffsetX + pDecInfo->roiWidth > pDecInfo->alignedWidth)
            || (pDecInfo->roiOffsetY + pDecInfo->roiHeight > pDecInfo->alignedHeight))
        {
            return JPG_RET_INVALID_PARAM;
        }

        if (((pDecInfo->roiOffsetX + pDecInfo->roiWidth) < pDecInfo->mcuWidth)
            || ((pDecInfo->roiOffsetY + pDecInfo->roiHeight) < pDecInfo->mcuHeight))
        {
            return JPG_RET_INVALID_PARAM;
        }

        info->roiFrameWidth = pDecInfo->roiMcuWidth * pDecInfo->mcuWidth;
        info->roiFrameHeight = pDecInfo->roiMcuHeight * pDecInfo->mcuHeight;
        info->roiFrameOffsetX = pDecInfo->roiMcuOffsetX * pDecInfo->mcuWidth;
        info->roiFrameOffsetY = pDecInfo->roiMcuOffsetY * pDecInfo->mcuHeight;
        info->roiMCUSize = pDecInfo->mcuWidth;
    }
    info->colorComponents = pDecInfo->compNum;

    return JPG_RET_SUCCESS;
}


JpgRet JPU_DecRegisterFrameBuffer(JpgDecHandle handle, FrameBuffer *bufArray, int num, int stride)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;


    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;


    if (!pDecInfo->initialInfoObtained)
    {
        return JPG_RET_WRONG_CALL_SEQUENCE;
    }

    if (bufArray == 0)
    {
        return JPG_RET_INVALID_FRAME_BUFFER;
    }

    if (num < pDecInfo->minFrameBufferNum)
    {
        return JPG_RET_INSUFFICIENT_FRAME_BUFFERS;
    }
    if (pDecInfo->usePartial && pDecInfo->bufNum == 0)
    {
        return JPG_RET_INSUFFICIENT_FRAME_BUFFERS;
    }
    if (pDecInfo->usePartial && num < pDecInfo->bufNum)
    {
        return JPG_RET_INSUFFICIENT_FRAME_BUFFERS;
    }

    if (!pDecInfo->roiEnable)
    {
        if (stride < pDecInfo->dispWidth >> 3 || stride % 8 != 0)
        {
            return JPG_RET_INVALID_STRIDE;
        }
    }

    pDecInfo->frameBufPool = bufArray;
    pDecInfo->numFrameBuffers = num;
    pDecInfo->stride = stride;

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecGetBitstreamBuffer(JpgDecHandle handle,
                                 PhysicalAddress *prdPrt,
                                 PhysicalAddress *pwrPtr,
                                 int *size)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;
    PhysicalAddress rdPtr;
    PhysicalAddress wrPtr;
    int room;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    if (prdPrt == 0 || pwrPtr == 0 || size == 0)
    {
        return JPG_RET_INVALID_PARAM;
    }

    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;

    if (GetJpgPendingInst() == pJpgInst)
    {
        rdPtr = JpuReadReg(MJPEG_BBC_RD_PTR_REG);
    }
    else
    {
        rdPtr = pDecInfo->streamRdPtr;
    }

    wrPtr = pDecInfo->streamWrPtr;

    if (wrPtr == pDecInfo->streamBufStartAddr)
    {
        if (pDecInfo->frameOffset == 0)
        {
            room = (pDecInfo->streamBufEndAddr - pDecInfo->streamBufStartAddr);
        }
        else
        {
            room = (pDecInfo->frameOffset);
        }
    }
    else
    {
        room = (pDecInfo->streamBufEndAddr - wrPtr);
    }

    room = ((room >> 9) << 9); // multiple of 512

    *prdPrt = rdPtr;
    *pwrPtr = wrPtr;

    *size = room;


    return JPG_RET_SUCCESS;
}


JpgRet JPU_DecUpdateBitstreamBuffer(JpgDecHandle handle, int size)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    PhysicalAddress wrPtr;
    PhysicalAddress rdPtr;
    JpgRet ret;
    int val = 0;


    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;
    wrPtr = pDecInfo->streamWrPtr;


    if (size == 0)
    {
        val = (wrPtr - pDecInfo->streamBufStartAddr) / 256;
        if ((wrPtr - pDecInfo->streamBufStartAddr) % 256)
        {
            val = val + 1;
        }
        if (GetJpgPendingInst() == pJpgInst)
        {
            JpuWriteReg(MJPEG_BBC_STRM_CTRL_REG, (1 << 31 | val));
        }
        pDecInfo->streamEndflag = 1;
        return JPG_RET_SUCCESS;
    }



    JpgSetClockGate(1);

    wrPtr = pDecInfo->streamWrPtr;
    wrPtr += size;

    if (wrPtr == pDecInfo->streamBufEndAddr)
    {
        wrPtr = pDecInfo->streamBufStartAddr;
    }

    pDecInfo->streamWrPtr = wrPtr;

    if (GetJpgPendingInst() == pJpgInst)
    {
        rdPtr = JpuReadReg(MJPEG_BBC_RD_PTR_REG);

        if (rdPtr == pDecInfo->streamBufEndAddr)
        {
            JpuWriteReg(MJPEG_BBC_CUR_POS_REG, 0);
            JpuWriteReg(MJPEG_GBU_TT_CNT_REG, 0);
            JpuWriteReg(MJPEG_GBU_TT_CNT_REG + 4, 0);
        }

        JpuWriteReg(MJPEG_BBC_WR_PTR_REG, wrPtr);
        if (wrPtr == pDecInfo->streamBufStartAddr)
        {
            JpuWriteReg(MJPEG_BBC_END_ADDR_REG, pDecInfo->streamBufEndAddr);
        }
        else
        {
            JpuWriteReg(MJPEG_BBC_END_ADDR_REG, wrPtr);
        }
    }
    else
    {
        rdPtr = pDecInfo->streamRdPtr;
    }

    pDecInfo->streamRdPtr = rdPtr;

    JpgSetClockGate(0);
    return JPG_RET_SUCCESS;

}
JpgRet JPU_SWReset()
{
    uint32_t val;
    PhysicalAddress streamBufStartAddr;
    PhysicalAddress streamBufEndAddr;
    PhysicalAddress streamWrPtr;
    PhysicalAddress streamRdPtr;

    JpgEnterLock();

    streamBufStartAddr = JpuReadReg(MJPEG_BBC_BAS_ADDR_REG);
    streamBufEndAddr = JpuReadReg(MJPEG_BBC_END_ADDR_REG);
    streamRdPtr = JpuReadReg(MJPEG_BBC_RD_PTR_REG);
    streamWrPtr = JpuReadReg(MJPEG_BBC_WR_PTR_REG);

    JpuWriteReg(MJPEG_PIC_START_REG, (1 << JPG_START_INIT));

    do
    {
        val = JpuReadReg(MJPEG_PIC_START_REG);
    }
    while ((val & (1 << JPG_START_INIT)) == (1 << JPG_START_INIT));


    JpuWriteReg(MJPEG_BBC_BAS_ADDR_REG, streamBufStartAddr);
    JpuWriteReg(MJPEG_BBC_END_ADDR_REG, streamBufEndAddr);
    JpuWriteReg(MJPEG_BBC_RD_PTR_REG, streamRdPtr);
    JpuWriteReg(MJPEG_BBC_WR_PTR_REG, streamWrPtr);

    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}
JpgRet JPU_HWReset()
{
    if (jdi_hw_reset() < 0)
    {
        return JPG_RET_FAILURE;
    }

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecIssueStop(JpgDecHandle handle)
{
    JpgInst *pJpgInst;
    JpgRet ret;


    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    pJpgInst = handle;

    if (pJpgInst != GetJpgPendingInst())
    {
        return JPG_RET_WRONG_CALL_SEQUENCE;
    }

    JpgSetClockGate(1);
    JpuWriteReg(MJPEG_PIC_START_REG, 1 << JPG_START_STOP);
    JpgSetClockGate(0);
    return JPG_RET_SUCCESS;

}
JpgRet JPU_DecCompleteStop(JpgDecHandle handle)
{
    JpgInst *pJpgInst;
    JpgRet ret;
    uint32_t val;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }

    pJpgInst = handle;

    if (pJpgInst != GetJpgPendingInst())
    {
        return JPG_RET_WRONG_CALL_SEQUENCE;
    }

    JpgSetClockGate(1);
    val = JpuReadReg(MJPEG_PIC_STATUS_REG);

    if (val & (1 << INT_JPU_BIT_BUF_STOP))
    {
        SetJpgPendingInst(0);
        JpgSetClockGate(0);
    }
    else
    {
        JpgSetClockGate(0);
        return JPG_RET_WRONG_CALL_SEQUENCE;
    }

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecSetRdPtr(JpgDecHandle handle, PhysicalAddress addr, int updateWrPtr)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }


    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;


    JpgEnterLock();

    if (GetJpgPendingInst())
    {
        JpgLeaveLock();
        return JPG_RET_FRAME_NOT_COMPLETE;
    }
    pDecInfo->streamRdPtr = addr;
    if (updateWrPtr)
    {
        pDecInfo->streamWrPtr = addr;
    }

    pDecInfo->frameOffset = addr - pDecInfo->streamBufStartAddr;
    pDecInfo->consumeByte = 0;

    JpuWriteReg(MJPEG_BBC_RD_PTR_REG, pDecInfo->streamRdPtr);

    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecSetRdPtrEx(JpgDecHandle handle, PhysicalAddress addr, int updateWrPtr)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }


    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;


    JpgEnterLock();

    if (GetJpgPendingInst())
    {
        JpgLeaveLock();
        return JPG_RET_FRAME_NOT_COMPLETE;
    }
    pDecInfo->streamRdPtr = addr;
    pDecInfo->streamBufStartAddr = addr;
    if (updateWrPtr)
    {
        pDecInfo->streamWrPtr = addr;
    }

    pDecInfo->frameOffset = 0;
    pDecInfo->consumeByte = 0;

    JpuWriteReg(MJPEG_BBC_RD_PTR_REG, pDecInfo->streamRdPtr);

    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecStartOneFrame(JpgDecHandle handle, JpgDecParam *param)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    uint32_t rotMir;
    JpgRet ret;
    int32_t val;
    PhysicalAddress rdPtr, wrPtr;

    int i;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }


    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;

    if (pDecInfo->frameBufPool == 0)   // This means frame buffers have not been registered.
    {
        return JPG_RET_WRONG_CALL_SEQUENCE;
    }

    rotMir = 0;
    if (pDecInfo->rotationEnable)
    {
        rotMir |= 0x10; // Enable rotator
        switch (pDecInfo->rotationAngle)
        {
        case 0:
            rotMir |= 0x0;
            break;

        case 90:
            rotMir |= 0x1;
            break;

        case 180:
            rotMir |= 0x2;
            break;

        case 270:
            rotMir |= 0x3;
            break;
        }
    }

    if (pDecInfo->mirrorEnable)
    {
        rotMir |= 0x10; // Enable rotator
        switch (pDecInfo->mirrorDirection)
        {
        case MIRDIR_NONE :
            rotMir |= 0x0;
            break;

        case MIRDIR_VER :
            rotMir |= 0x4;
            break;

        case MIRDIR_HOR :
            rotMir |= 0x8;
            break;

        case MIRDIR_HOR_VER :
            rotMir |= 0xc;
            break;

        }
    }
    JpgEnterLock();

    if (GetJpgPendingInst())
    {
        JpgLeaveLock();
        return JPG_RET_FRAME_NOT_COMPLETE;
    }

    if (pDecInfo->frameOffset < 0)
    {
        SetJpgPendingInst(pJpgInst);
        return JPG_RET_EOS;
    }

    //check for stream empty case
    if (pDecInfo->streamEndflag == 0)
    {
        rdPtr = pDecInfo->streamRdPtr;
        wrPtr = pDecInfo->streamWrPtr;
        if (wrPtr == pDecInfo->streamBufStartAddr)
        {
            wrPtr = pDecInfo->streamBufEndAddr;
        }
        if (rdPtr > wrPtr)   // wrap-around case
        {
            if (((pDecInfo->streamBufEndAddr - rdPtr) + (wrPtr - pDecInfo->streamBufStartAddr)) < 1024)
            {
                return JPG_RET_BIT_EMPTY;
            }
        }
        else
        {
            if (wrPtr - rdPtr < 1024)
            {
                return JPG_RET_BIT_EMPTY;
            }
        }
    }

    val = JpegDecodeHeader(pDecInfo);
    if (val == 0)
    {
        JpgLeaveLock();
        return JPG_RET_FAILURE;
    }
    if (val == -3)
    {
        JpgLeaveLock();
        return JPG_RET_NOT_SUPPORTED_FORMAT;
    }

    if (val == -2)      // wrap around case
    {
        pDecInfo->frameOffset = 0;
        pDecInfo->ecsPtr = 0;

        val = JpegDecodeHeader(pDecInfo);
        if (val == 0)
        {
            JpgLeaveLock();
            return JPG_RET_FAILURE;
        }

        if (val == -3)
        {
            JpgLeaveLock();
            return JPG_RET_NOT_SUPPORTED_FORMAT;
        }
    }

    if (val == -1)      //stream empty case
    {
        if (pDecInfo->streamEndflag == 1)
        {
            SetJpgPendingInst(pJpgInst);
            pDecInfo->frameOffset = -1;
            return JPG_RET_EOS;
        }
        JpgLeaveLock();
        return JPG_RET_BIT_EMPTY;
    }

    if (pDecInfo->streamRdPtr == pDecInfo->streamBufEndAddr)
    {
        JpuWriteReg(MJPEG_BBC_CUR_POS_REG, 0);
        JpuWriteReg(MJPEG_GBU_TT_CNT_REG, 0);
        JpuWriteReg(MJPEG_GBU_TT_CNT_REG + 4, 0);
    }

    JpuWriteReg(MJPEG_BBC_WR_PTR_REG, pDecInfo->streamWrPtr);
    if (pDecInfo->streamWrPtr == pDecInfo->streamBufStartAddr)
    {
        JpuWriteReg(MJPEG_BBC_END_ADDR_REG, pDecInfo->streamBufEndAddr);
    }
    else
    {
        JpuWriteReg(MJPEG_BBC_END_ADDR_REG, pDecInfo->streamWrPtr);
    }

    JpuWriteReg(MJPEG_BBC_BAS_ADDR_REG, pDecInfo->streamBufStartAddr);

    if (pDecInfo->streamEndflag == 1)
    {
        val = JpuReadReg(MJPEG_BBC_STRM_CTRL_REG);
        val = (pDecInfo->streamWrPtr - pDecInfo->streamBufStartAddr) / 256;
        if ((pDecInfo->streamWrPtr - pDecInfo->streamBufStartAddr) % 256)
        {
            val = val + 1;
        }

        JpuWriteReg(MJPEG_BBC_STRM_CTRL_REG, (1 << 31 | val));
    }
    else
    {
        JpuWriteReg(MJPEG_BBC_STRM_CTRL_REG, 0);
    }

    JpuWriteReg(MJPEG_GBU_TT_CNT_REG, 0);
    JpuWriteReg(MJPEG_GBU_TT_CNT_REG + 4, 0);
    JpuWriteReg(MJPEG_PIC_ERRMB_REG, 0);
    JpuWriteReg(MJPEG_PIC_CTRL_REG,
                pDecInfo->huffAcIdx << 10 | pDecInfo->huffDcIdx << 7 | pDecInfo->userHuffTab << 6 |
                (JPU_DEC_CHECK_WRITE_RESPONSE_BVALID_SIGNAL << 2) | pDecInfo->usePartial);

    JpuWriteReg(MJPEG_PIC_SIZE_REG, (pDecInfo->alignedWidth << 16) | pDecInfo->alignedHeight);
    JpuWriteReg(MJPEG_ROT_INFO_REG, 0);
    JpuWriteReg(MJPEG_OP_INFO_REG,
                pDecInfo->lineNum << 16 | pDecInfo->bufNum << 3 | pDecInfo->busReqNum);
    JpuWriteReg(MJPEG_MCU_INFO_REG,
                pDecInfo->mcuBlockNum << 16 | pDecInfo->compNum << 12 | pDecInfo->compInfo[0] << 8 |
                pDecInfo->compInfo[1] << 4 | pDecInfo->compInfo[2]);

    if (pDecInfo->packedFormat == PACKED_FORMAT_NONE)
    {
        JpuWriteReg(MJPEG_DPB_CONFIG_REG,
                    (pDecInfo->frameEndian << 6) | (0 << 5) | (0 << 4) | ((pDecInfo->chromaInterleave == 0) ? 0 :
                                                                          (pDecInfo->chromaInterleave == 1) ? 2 : 3));
    }
    else if (pDecInfo->packedFormat == PACKED_FORMAT_444)
    {
        JpuWriteReg(MJPEG_DPB_CONFIG_REG,
                    (pDecInfo->frameEndian << 6) | (1 << 5) | (0 << 4) | (0 << 2) | ((pDecInfo->chromaInterleave == 0) ?
                                                                                     0 : (pDecInfo->chromaInterleave == 1) ? 2 : 3));
    }
    else
    {
        JpuWriteReg(MJPEG_DPB_CONFIG_REG,
                    (pDecInfo->frameEndian << 6) | (0 << 5) | (1 << 4) | ((pDecInfo->packedFormat - 1) << 2) | ((
                                pDecInfo->chromaInterleave == 0) ? 0 : (pDecInfo->chromaInterleave == 1) ? 2 : 3));
    }

    JpuWriteReg(MJPEG_RST_INTVAL_REG, pDecInfo->rstIntval);

    if (param)
    {
        if (param->scaleDownRatioWidth > 0)
        {
            pDecInfo->iHorScaleMode = param->scaleDownRatioWidth;
        }
        if (param->scaleDownRatioHeight > 0)
        {
            pDecInfo->iVerScaleMode = param->scaleDownRatioHeight;
        }
    }
    if (pDecInfo->iHorScaleMode | pDecInfo->iVerScaleMode)
    {
        val = ((pDecInfo->iHorScaleMode & 0x3) << 2) | ((pDecInfo->iVerScaleMode & 0x3)) | 0x10 ;
    }
    else
    {
        val = 0;
    }
    JpuWriteReg(MJPEG_SCL_INFO_REG, val);

    if (pDecInfo->userHuffTab)
    {
        if (!JpgDecHuffTabSetUp(pDecInfo))
        {
            JpgLeaveLock();
            return JPG_RET_INVALID_PARAM;
        }
    }

    if (!JpgDecQMatTabSetUp(pDecInfo))
    {
        JpgLeaveLock();
        return JPG_RET_INVALID_PARAM;
    }

    JpgDecGramSetup(pDecInfo);

    JpuWriteReg(MJPEG_RST_INDEX_REG, 0);    // RST index at the beginning.
    JpuWriteReg(MJPEG_RST_COUNT_REG, 0);

    JpuWriteReg(MJPEG_DPCM_DIFF_Y_REG, 0);
    JpuWriteReg(MJPEG_DPCM_DIFF_CB_REG, 0);
    JpuWriteReg(MJPEG_DPCM_DIFF_CR_REG, 0);

    JpuWriteReg(MJPEG_GBU_FF_RPTR_REG, pDecInfo->bitPtr);
    JpuWriteReg(MJPEG_GBU_CTRL_REG, 3);

    JpuWriteReg(MJPEG_ROT_INFO_REG, rotMir);

    if (rotMir & 1)
    {
        pDecInfo->format = (pDecInfo->format == FORMAT_422) ? FORMAT_224 : (pDecInfo->format == FORMAT_224)
                           ? FORMAT_422 : pDecInfo->format;
    }

    if (rotMir & 0x10)
    {
        JpuWriteReg(MJPEG_DPB_BASE00_REG, pDecInfo->rotatorOutput.bufY);
        JpuWriteReg(MJPEG_DPB_BASE01_REG, pDecInfo->rotatorOutput.bufCb);
        JpuWriteReg(MJPEG_DPB_BASE02_REG, pDecInfo->rotatorOutput.bufCr);
    }
    else if (pDecInfo->usePartial)
    {
        val = (pDecInfo->frameIdx % (pDecInfo->numFrameBuffers / pDecInfo->bufNum));
        for (i = 0; i < pDecInfo->bufNum; i++)
        {
            JpuWriteReg(MJPEG_DPB_BASE00_REG + (i * 12),
                        pDecInfo->frameBufPool[(val * pDecInfo->bufNum) + i].bufY);
            JpuWriteReg(MJPEG_DPB_BASE01_REG + (i * 12),
                        pDecInfo->frameBufPool[(val * pDecInfo->bufNum) + i].bufCb);
            JpuWriteReg(MJPEG_DPB_BASE02_REG + (i * 12),
                        pDecInfo->frameBufPool[(val * pDecInfo->bufNum) + i].bufCr);
        }
    }
    else
    {
        val = (pDecInfo->frameIdx % pDecInfo->numFrameBuffers);
        JpuWriteReg(MJPEG_DPB_BASE00_REG, pDecInfo->frameBufPool[val].bufY);
        JpuWriteReg(MJPEG_DPB_BASE01_REG, pDecInfo->frameBufPool[val].bufCb);
        JpuWriteReg(MJPEG_DPB_BASE02_REG, pDecInfo->frameBufPool[val].bufCr);
    }

    if (pDecInfo->rotationEnable)
    {
        JpuWriteReg(MJPEG_DPB_YSTRIDE_REG, pDecInfo->rotatorStride);
        val = (pDecInfo->format == FORMAT_420 || pDecInfo->format == FORMAT_422 ||
               pDecInfo->format == FORMAT_400) ? 2 : 1;
        if (pDecInfo->chromaInterleave)
        {
            JpuWriteReg(MJPEG_DPB_CSTRIDE_REG, (pDecInfo->rotatorStride / (int)val) * 2);
        }
        else
        {
            JpuWriteReg(MJPEG_DPB_CSTRIDE_REG, pDecInfo->rotatorStride / (int)val);
        }
    }
    else
    {
        JpuWriteReg(MJPEG_DPB_YSTRIDE_REG, pDecInfo->stride);
        val = (pDecInfo->format == FORMAT_420 || pDecInfo->format == FORMAT_422 ||
               pDecInfo->format == FORMAT_400) ? 2 : 1;
        if (pDecInfo->chromaInterleave)
        {
            JpuWriteReg(MJPEG_DPB_CSTRIDE_REG, (pDecInfo->stride / (int)val) * 2);
        }
        else
        {
            JpuWriteReg(MJPEG_DPB_CSTRIDE_REG, pDecInfo->stride / (int)val);
        }
    }
    if (pDecInfo->roiEnable)
    {
        JpuWriteReg(MJPEG_CLP_INFO_REG, 1);
        JpuWriteReg(MJPEG_CLP_BASE_REG, pDecInfo->roiOffsetX << 16 | pDecInfo->roiOffsetY); // pixel unit
        JpuWriteReg(MJPEG_CLP_SIZE_REG,
                    (pDecInfo->roiMcuWidth * pDecInfo->mcuWidth) << 16 | (pDecInfo->roiMcuHeight *
                                                                          pDecInfo->mcuHeight)); // pixel Unit
    }
    else
    {
        JpuWriteReg(MJPEG_CLP_INFO_REG, 0);
    }


    if (pJpgInst->loggingEnable)
    {
        jdi_log(JDI_LOG_CMD_PICRUN, 1);
    }

    JpuWriteReg(MJPEG_PIC_START_REG, (1 << JPG_START_PIC));


    SetJpgPendingInst(pJpgInst);
    return JPG_RET_SUCCESS;
}

JpgRet JPU_DecGetOutputInfo(JpgDecHandle handle, JpgDecOutputInfo *info)
{
    JpgInst *pJpgInst;
    JpgDecInfo     *pDecInfo;
    JpgRet      ret;
    uint32_t      val = 0;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        SetJpgPendingInst(0);
        JpgLeaveLock();
        return ret;
    }

    if (info == 0)
    {
        SetJpgPendingInst(0);
        JpgLeaveLock();
        return JPG_RET_INVALID_PARAM;
    }

    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;


    val = JpuReadReg(MJPEG_PIC_STATUS_REG);

    if (pJpgInst != GetJpgPendingInst())
    {
        SetJpgPendingInst(0);
        JpgLeaveLock();
        return JPG_RET_WRONG_CALL_SEQUENCE;
    }

    if (pDecInfo->frameOffset < 0)
    {
        info->numOfErrMBs = 0;
        info->decodingSuccess = 1;
        info->indexFrameDisplay = -1;
        SetJpgPendingInst(0);
        JpgLeaveLock();
        return JPG_RET_SUCCESS;
    }

    if (pDecInfo->roiEnable)
    {
        info->decPicWidth = pDecInfo->roiMcuWidth * pDecInfo->mcuWidth;
        info->decPicHeight = pDecInfo->roiMcuHeight * pDecInfo->mcuHeight;
    }
    else
    {
        info->decPicWidth = pDecInfo->alignedWidth;
        info->decPicHeight = pDecInfo->alignedHeight;
    }

    info->decPicWidth  >>= pDecInfo->iHorScaleMode;
    info->decPicHeight >>= pDecInfo->iVerScaleMode;

    info->indexFrameDisplay = (pDecInfo->frameIdx % pDecInfo->numFrameBuffers);
#ifdef MJPEG_ERROR_CONCEAL
    info->consumedByte = (JpuReadReg(MJPEG_GBU_TT_CNT_REG)) / 8;
#else
    info->consumedByte = (JpuReadReg(MJPEG_GBU_TT_CNT_REG)) / 8;
#endif
    pDecInfo->streamRdPtr = JpuReadReg(MJPEG_BBC_RD_PTR_REG);
    pDecInfo->consumeByte = info->consumedByte - 16 - pDecInfo->ecsPtr;
    info->bytePosFrameStart = pDecInfo->frameOffset;
    info->ecsPtr = pDecInfo->ecsPtr;


    pDecInfo->ecsPtr = 0;
    pDecInfo->frameIdx++;

    val = JpuReadReg(MJPEG_PIC_STATUS_REG);
    if (val & (1 << INT_JPU_DONE))
    {
        info->decodingSuccess = 1;
        info->numOfErrMBs = 0;

#ifdef MJPEG_ERROR_CONCEAL
        pDecInfo->errInfo.bError = 0;
        pDecInfo->nextRstMarkerPtr = 0;
#endif
    }
    else if (val & (1 << INT_JPU_ERROR))
    {

        info->numOfErrMBs = JpuReadReg(MJPEG_PIC_ERRMB_REG);
        info->decodingSuccess = 0;

#ifdef MJPEG_ERROR_CONCEAL
        pDecInfo->errInfo.bError = 1;
        pDecInfo->errInfo.rstMarker = ((info->numOfErrMBs >> 24) & 0xf);
        pDecInfo->errInfo.errPosY = info->numOfErrMBs & 0xFFF;
        pDecInfo->errInfo.errPosX = (info->numOfErrMBs >> 12) & 0xFFF;

        // set search point to find next rstMarker from origin of frame buffer by host
        pDecInfo->curRstMarkerPtr = info->ecsPtr + ((info->consumedByte / 256) * 256);
#endif
    }

    info->frameCycle = JpuReadReg(MJPEG_CYCLE_INFO_REG);
    if (val != 0)
    {
        JpuWriteReg(MJPEG_PIC_STATUS_REG, val);
    }

    if (pJpgInst->loggingEnable)
    {
        jdi_log(JDI_LOG_CMD_PICRUN, 0);
    }

    SetJpgPendingInst(0);
    JpgLeaveLock();

    return JPG_RET_SUCCESS;
}



JpgRet JPU_DecGiveCommand(
    JpgDecHandle handle,
    JpgCommand cmd,
    void *param)
{
    JpgInst *pJpgInst;
    JpgDecInfo *pDecInfo;
    JpgRet ret;

    ret = CheckJpgInstValidity(handle);
    if (ret != JPG_RET_SUCCESS)
    {
        return ret;
    }


    pJpgInst = handle;
    pDecInfo = &pJpgInst->JpgInfo.decInfo;
    switch (cmd)
    {
    case ENABLE_JPG_ROTATION :
        {
            if (pDecInfo->roiEnable)
            {
                return JPG_RET_INVALID_PARAM;
            }

            if (pDecInfo->rotatorStride == 0)
            {
                return JPG_RET_ROTATOR_STRIDE_NOT_SET;
            }
            pDecInfo->rotationEnable = 1;
            break;
        }

    case DISABLE_JPG_ROTATION :
        {
            pDecInfo->rotationEnable = 0;
            break;
        }

    case ENABLE_JPG_MIRRORING :
        {
            if (pDecInfo->rotatorStride == 0)
            {
                return JPG_RET_ROTATOR_STRIDE_NOT_SET;
            }
            pDecInfo->mirrorEnable = 1;
            break;
        }
    case DISABLE_JPG_MIRRORING :
        {
            pDecInfo->mirrorEnable = 0;
            break;
        }
    case SET_JPG_MIRROR_DIRECTION :
        {

            JpgMirrorDirection mirDir;

            if (param == 0)
            {
                return JPG_RET_INVALID_PARAM;
            }
            mirDir = *(JpgMirrorDirection *)param;
            if (!(MIRDIR_NONE <= mirDir && mirDir <= MIRDIR_HOR_VER))
            {
                return JPG_RET_INVALID_PARAM;
            }
            pDecInfo->mirrorDirection = mirDir;

            break;
        }
    case SET_JPG_ROTATION_ANGLE :
        {
            int angle;

            if (param == 0)
            {
                return JPG_RET_INVALID_PARAM;
            }

            angle = *(int *)param;
            if (angle != 0 && angle != 90 &&
                angle != 180 && angle != 270)
            {
                return JPG_RET_INVALID_PARAM;
            }

            pDecInfo->rotationAngle = angle;
            break;
        }

    case SET_JPG_ROTATOR_OUTPUT :
        {
            FrameBuffer *frame;

            if (param == 0)
            {
                return JPG_RET_INVALID_PARAM;
            }
            frame = (FrameBuffer *)param;
            pDecInfo->rotatorOutput = *frame;
            pDecInfo->rotatorOutputValid = 1;
            break;
        }

    case SET_JPG_ROTATOR_STRIDE :
        {
            int stride;

            if (param == 0)
            {
                return JPG_RET_INVALID_PARAM;
            }
            stride = *(int *)param;
            if (stride % 8 != 0 || stride == 0)
            {
                return JPG_RET_INVALID_STRIDE;
            }

            if (pDecInfo->rotationAngle == 90 || pDecInfo->rotationAngle == 270)
            {
                if (pDecInfo->alignedHeight > stride)
                {
                    return JPG_RET_INVALID_STRIDE;
                }
            }
            else
            {
                if (pDecInfo->alignedWidth > stride)
                {
                    return JPG_RET_INVALID_STRIDE;
                }
            }


            pDecInfo->rotatorStride = stride;
            break;
        }
    case SET_JPG_SCALE_HOR:
        {
            int scale;
            scale = *(int *)param;
            if (pDecInfo->alignedWidth < 128 || pDecInfo->alignedHeight < 128)
            {
                if (scale)
                {
                    return JPG_RET_INVALID_PARAM;
                }
            }

            pDecInfo->iHorScaleMode = scale;
            break;
        }
    case SET_JPG_SCALE_VER:
        {
            int scale;
            scale = *(int *)param;
            if (pDecInfo->alignedWidth < 128 || pDecInfo->alignedHeight < 128)
            {
                if (scale)
                {
                    return JPG_RET_INVALID_PARAM;
                }
            }
            pDecInfo->iVerScaleMode = scale;
            break;
        }
    case SET_JPG_USE_PARTIAL_MODE:
        {
            int enable;
            enable = *(int *)param;
            pDecInfo->usePartial = enable;

            break;
        }
    case SET_JPG_PARTIAL_FRAME_NUM:
        {
            int frame;

            if (pDecInfo->stride != 0)
            {
                return JPG_RET_WRONG_CALL_SEQUENCE;
            }

            frame = *(int *)param;
            pDecInfo->bufNum = frame;

            break;
        }
    case SET_JPG_PARTIAL_LINE_NUM:
        {
            int line;

            if (pDecInfo->stride != 0)
            {
                return JPG_RET_WRONG_CALL_SEQUENCE;
            }
            line = *(int *)param;
            pDecInfo->lineNum = line;

            break;
        }
    case ENABLE_LOGGING:
        {
            pJpgInst->loggingEnable = 1;
        }
        break;
    case DISABLE_LOGGING:
        {
            pJpgInst->loggingEnable = 0;
        }
        break;
    case SET_JPG_DISP_WIDTH :
        {
            int width;

            if (param == 0)
            {
                return JPG_RET_INVALID_PARAM;
            }

            width = *(int *)param;
            pDecInfo->dispWidth = width;
            break;
        }
    default:
        return JPG_RET_INVALID_COMMAND;
    }
    return JPG_RET_SUCCESS;
}
