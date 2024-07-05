#include "time_adapter.h"
#include "trace.h"
#include "rtl876x.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_aon.h"
#include "rtl876x_rcc.h"
#include "string.h"



#include "jpurun.h"
#include "regdefine.h"
//#include "jdi.h"
#include "jpuconfig.h"
#include "jpuapi.h"
#include "mixer.h"
#include "jpuhelper_rt.h"
#include "jpuapifunc_rt.h"
#include "jpeg_heap.h"

#define JPEG_DECODER_EN
// #define JPEG_ENCODER_EN


#ifdef JPEG_DECODER_EN
// #include "pic_hex.txt"
// #include "pic_hex_122144.txt"
// #include "pic_hex_1632.txt"
#include "pic_hex_184_96.txt"
#else
static uint8_t *pic;
#endif

#ifdef JPEG_ENCODER_EN
#include "pic_yuv_122144.txt"
#else
static uint8_t *pic_yuv;
#endif



//PCC config functin
uint32_t RCC_MJPEG_Config_Enable(void)
{
    DBG_DIRECT("MJPEG RCC ENABLE");
    RCC_PeriphClockCmd(APBPeriph_JPEG, APBPeriph_JPEG_CLOCK, ENABLE);
    //Function enable
    // DBG_DIRECT("BIT28 reg_0x210 = 0x%x", *((uint32_t*) 0x40000210));
    //clock enable
    // DBG_DIRECT("BIT16_17 reg_0x23c = 0x%x", *((uint32_t*) 0x4000023C));
    return 0;
}
uint32_t RCC_MJPEG_Config_Disable(void)
{
    RCC_PeriphClockCmd(APBPeriph_JPEG, APBPeriph_JPEG_CLOCK, DISABLE);
    //Function enable
    // DBG_DIRECT("BIT28 reg_0x210 = 0x%x", *((uint32_t*) 0x40000210));
    //clock enable
    // DBG_DIRECT("BIT26_17 reg_0x23c = 0x%x", *((uint32_t*) 0x4000023C));
    return 0;
}



uint32_t CODA_Test_read(uint32_t addr)
{
    uint32_t data = 0;
    uint32_t *reg_addr;

    reg_addr = (uint32_t *)(addr);
    // data = *(uint32_t *)reg_addr;
    data = jdi_read_register(addr);
    DBG_DIRECT("JPEG rd 0x%03x: 0x%08x\n", addr & 0XFFF, data);
    return data;
}
uint32_t CODA_Test_write(uint32_t addr, uint32_t val)
{
    CODA_Test_read(addr);

    uint32_t *reg_addr = (uint32_t *)(addr);
    // *reg_addr = val;
    jdi_write_register(addr, val);
    DBG_DIRECT("JPEG wr 0x%03x: 0x%08x\n", addr & 0XFFF, val);

    CODA_Test_read(addr);
    return 0;
}

// -------------------------------------------------------------------------

#define NUM_FRAME_BUF           MAX_FRAME
#define EXTRA_FRAME_BUFFER_NUM  0
#define MAX_ROT_BUF_NUM         1

#ifdef JPEG_DECODER_EN
static int CODA_Decode_Test(DecConfigParam *param)
{
    JpgDecHandle handle     = {0};
    JpgDecOpenParam decOP       = {0};
    JpgDecInitialInfo initialInfo = {0};
    JpgDecOutputInfo outputInfo = {0};
    JpgDecParam decParam    = {0};
    JpgRet ret = JPG_RET_SUCCESS;
    FrameBuffer frameBuf[NUM_FRAME_BUF];
    jpu_buffer_t vbStream    = {0};
    BufInfo bufInfo     = {0};
    FRAME_BUF *pFrame[NUM_FRAME_BUF];
    FRAME_BUF *pDispFrame = NULL;
    Uint32 bsSize = 0, framebufSize = 0, framebufWidth = 0, framebufHeight = 0, framebufStride = 0,
           framebufFormat = FORMAT_420;
    int dispWidth = 0, dispHeight = 0;
    int i = 0, frameIdx = 0, ppIdx = 0, saveIdx = 0, totalNumofErrMbs = 0, streameos = 0, dispImage = 0;
    int key = 0,  suc = 1;
    Uint8 *pFileBuf = NULL;
    Uint8 *pYuv  =  NULL;
    void *fpYuv  =  NULL;
    int needFrameBufCount = 0, regFrameBufCount = 0;
    int rotEnable = 0;
    int int_reason = 0;
    int instIdx;
    int partPosIdx = 0;
    int partBufIdx = 0;
    int partMaxIdx = 0;
    int partialHeight = 0;
    Uint32 product_version;

    DecConfigParam decConfig;


    memcpy(&decConfig, param, sizeof(DecConfigParam));
    memset(&pFrame, 0x00, sizeof(FRAME_BUF *)*NUM_FRAME_BUF);
    memset(&frameBuf, 0x00, sizeof(FrameBuffer)*NUM_FRAME_BUF);

    instIdx = decConfig.instNum;
    if (decConfig.usePartialMode && decConfig.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : partial and ROI mode can not be worked\n");
        goto ERR_DEC_INIT;
    }
    if (decConfig.packedFormat && decConfig.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : packed mode and ROI mode can not be worked\n");
        goto ERR_DEC_INIT;
    }
    if ((decConfig.iHorScaleMode || decConfig.iVerScaleMode) && decConfig.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : Scaler mode and ROI mode can not be worked\n");
        goto ERR_DEC_INIT;
    }
    if (decConfig.useRot && decConfig.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : Rotator mode and ROI mode can not be worked\n");
        goto ERR_DEC_INIT;
    }

    // store file or display
    dispImage = 1;


    // bitstream
    bsSize = sizeof(pic) / sizeof(pic[0]);
    pFileBuf = pic;
    // pFileBuf = malloc(bsSize);
    if (!pFileBuf)
    {
        goto ERR_DEC_INIT;
    }

    // memcpy(pFileBuf, pic, bsSize);

    // source image buffer
    bufInfo.buf = pFileBuf;
    bufInfo.size = bsSize;
    bufInfo.point = 0;

    ret = JPU_Init();
    if (ret != JPG_RET_SUCCESS &&
        ret != JPG_RET_CALLED_BEFORE)
    {
        suc = 0;
        DBG_DIRECT("JPU_Init failed Error code is 0x%x \n", ret);
        goto ERR_DEC_INIT;
    }
    DBG_DIRECT("JPU_Init Done\n");

    // Open an instance and get initial information for decoding.
    vbStream.size = ((bsSize + 1023) >> 10 << 10);

    // vbStream.size = STREAM_BUF_SIZE;
    if (jdi_allocate_dma_memory(&vbStream) < 0)
    {
        DBG_DIRECT("fail to allocate bitstream buffer\n");
        goto ERR_DEC_INIT;
    }
//  DBG_DIRECT("jdi_allocate_dma_memory Done\n");

    decOP.streamEndian = decConfig.StreamEndian;
    decOP.frameEndian = decConfig.FrameEndian;
    decOP.chromaInterleave = (CbCrInterLeave)decConfig.chromaInterleave;
    decOP.bitstreamBuffer = vbStream.phys_addr;
    decOP.bitstreamBufferSize = vbStream.size;
    decOP.pBitStream = (BYTE *)vbStream.virt_addr; // set virtual address mapped of physical address
    decOP.packedFormat = decConfig.packedFormat;
    decOP.roiEnable = decConfig.roiEnable;
    decOP.roiOffsetX = decConfig.roiOffsetX;
    decOP.roiOffsetY = decConfig.roiOffsetY;
    decOP.roiWidth = decConfig.roiWidth;
    decOP.roiHeight = decConfig.roiHeight;

    ret = JPU_DecOpen(&handle, &decOP);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecOpen failed Error code is 0x%x \n", ret);
        goto ERR_DEC_INIT;
    }
//  DBG_DIRECT("JPU_DecOpen Done\n");

    JPU_GetVersionInfo(&product_version);
//  DBG_DIRECT("Product Version is 0x%x \n", product_version);

    //JPU_DecGiveCommand(handle, ENABLE_LOGGING, NULL);

    if (decConfig.useRot)
    {
        rotEnable = 1;
    }
    else
    {
        rotEnable = 0;
    }


    ret = WriteJpgBsBufHelper(handle, &bufInfo, decOP.bitstreamBuffer,
                              decOP.bitstreamBuffer + decOP.bitstreamBufferSize, 0, 0, &streameos, decOP.streamEndian);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("WriteBsBufHelper failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }
//  DBG_DIRECT("WriteJpgBsBufHelper Done\n");

    ret = JPU_DecGetInitialInfo(handle, &initialInfo);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGetInitialInfo failed Error code is 0x%x, inst=%d \n", ret, instIdx);
        goto ERR_DEC_OPEN;
    }


    if (decConfig.usePartialMode)
    {
        // disable Rotator, Scaler
        rotEnable = 0;
        decConfig.iHorScaleMode = 0;
        decConfig.iVerScaleMode = 0;
        partialHeight = (initialInfo.sourceFormat == FORMAT_420 ||
                         initialInfo.sourceFormat == FORMAT_224) ? 16 : 8;

        partMaxIdx  = ((initialInfo.picHeight + 15) & ~15) / partialHeight;
        if (partMaxIdx < decConfig.partialBufNum)
        {
            decConfig.partialBufNum = partMaxIdx;
        }
    }

    if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_422)
    {
        framebufWidth = ((initialInfo.picWidth + 15) / 16) * 16;
    }
    else
    {
        framebufWidth = ((initialInfo.picWidth + 7) / 8) * 8;
    }

    if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_224)
    {
        framebufHeight = ((initialInfo.picHeight + 15) / 16) * 16;
    }
    else
    {
        framebufHeight = ((initialInfo.picHeight + 7) / 8) * 8;
    }

    if (decConfig.roiEnable)
    {
        framebufWidth  = initialInfo.roiFrameWidth ;
        framebufHeight = initialInfo.roiFrameHeight;
    }


    // scaler constraint when conformance test is disable
    if (framebufWidth < 128 || framebufHeight < 128)
    {
        if (decConfig.iHorScaleMode || decConfig.iVerScaleMode)
        {
            DBG_DIRECT("Invalid operation mode : Not supported resolution with Scaler, width=%d, height=%d\n",
                       framebufWidth, framebufHeight);
        }
        decConfig.iHorScaleMode = 0;
        decConfig.iVerScaleMode = 0;
    }


//  DBG_DIRECT("* Dec InitialInfo =>\n instance #%d, \n minframeBuffercount: %u\n ", instIdx, initialInfo.minFrameBufferCount);
//  DBG_DIRECT("picWidth: %u\n picHeight: %u\n roiWidth: %u\n rouHeight: %u\n ", initialInfo.picWidth, initialInfo.picHeight, initialInfo.roiFrameWidth, initialInfo.roiFrameHeight);

    if (decConfig.usePartialMode)
    {
        DBG_DIRECT("Partial Mode Enable\n ");
        DBG_DIRECT("Num of Buffer for Partial : %d\n ", decConfig.partialBufNum);
        DBG_DIRECT("Num of Line for Partial   : %d\n ", partialHeight);
    }

    framebufFormat = initialInfo.sourceFormat;
    DBG_DIRECT("framebufFormat %d framebufHeight %d", framebufFormat, framebufHeight);

    if (decConfig.iHorScaleMode || decConfig.iVerScaleMode)
    {
        framebufHeight = ((framebufHeight + 1) / 2) * 2;
        framebufWidth = ((framebufWidth + 1) / 2) * 2;
    }

    framebufWidth  >>= decConfig.iHorScaleMode;
    framebufHeight >>= decConfig.iVerScaleMode;

    dispWidth = (decConfig.rotAngle == 90 ||
                 decConfig.rotAngle == 270) ? framebufHeight : framebufWidth;
    dispHeight = (decConfig.rotAngle == 90 ||
                  decConfig.rotAngle == 270) ? framebufWidth : framebufHeight;
    JPU_DecGiveCommand(handle, SET_JPG_DISP_WIDTH, &dispWidth);


    if (decConfig.rotAngle == 90 || decConfig.rotAngle == 270)
    {
        framebufStride = framebufHeight;
        framebufHeight = framebufWidth;
        framebufFormat = (framebufFormat == FORMAT_422) ? FORMAT_224 : (framebufFormat == FORMAT_224) ?
                         FORMAT_422 : framebufFormat;
    }
    else
    {
        framebufStride = framebufWidth;
    }

    if (decConfig.iHorScaleMode || decConfig.iVerScaleMode)
    {
        framebufStride = ((framebufStride + 15) / 16) * 16;
    }

    if (decOP.packedFormat >= PACKED_FORMAT_422_YUYV && decOP.packedFormat <= PACKED_FORMAT_422_VYUY)
    {
        framebufWidth = framebufWidth * 2;
        framebufStride = framebufStride * 2;
        framebufFormat = FORMAT_422;
        if (decConfig.rotAngle == 90 || decConfig.rotAngle == 270)
        {
            framebufFormat = FORMAT_224;
        }

    }
    else if (decOP.packedFormat == PACKED_FORMAT_444)
    {
        framebufWidth = framebufWidth * 3;
        framebufStride = framebufStride * 3;
        framebufFormat = FORMAT_444;
    }

    if (decConfig.rotAngle == 90 || decConfig.rotAngle == 270)
    {
        param->picWidth = framebufHeight ;
        param->picHeight = framebufWidth ;
    }
    else
    {
        param->picWidth = framebufWidth ;
        param->picHeight = framebufHeight ;
    }

    DBG_DIRECT("framebufFormat %d", framebufFormat);

    framebufSize = GetFrameBufSize(framebufFormat, initialInfo.picWidth, initialInfo.picHeight);
//  DBG_DIRECT("framebuffer stride: %d,  width: %d, height = %d\n", framebufStride, framebufWidth, framebufHeight);
//  DBG_DIRECT("framebuffer format: %d, framebuffer size = %d, packed format = %d\n", framebufFormat, framebufSize, decOP.packedFormat);
    DBG_DIRECT("framebufSize %d frame width: %d, stride : %d, height = %d\n", framebufSize,
               framebufWidth, framebufStride, framebufHeight);


    //Allocate frame buffer
    regFrameBufCount = initialInfo.minFrameBufferCount + EXTRA_FRAME_BUFFER_NUM;


    if (decConfig.usePartialMode)
    {
        if (decConfig.partialBufNum > 4)
        {
            decConfig.partialBufNum = 4;
        }

        regFrameBufCount *= decConfig.partialBufNum;
    }

    needFrameBufCount = regFrameBufCount;

    if (AllocateFrameBuffer(instIdx, framebufFormat, framebufStride, framebufHeight, needFrameBufCount,
                            (decOP.packedFormat >= PACKED_FORMAT_422_YUYV)) == 0)
    {
        DBG_DIRECT("fail to AllocateFrameBuffer\n");
        goto ERR_DEC_OPEN;
    }
//  DBG_DIRECT("AllocateFrameBuffer DONE\n");

    JpgEnterLock();
    for (i = 0; i < needFrameBufCount; ++i)
    {
        pFrame[i] = GetFrameBuffer(instIdx, i);
        frameBuf[i].bufY = pFrame[i]->vbY.phys_addr;
        frameBuf[i].bufCb = pFrame[i]->vbCb.phys_addr;
        if (decOP.chromaInterleave == CBCR_SEPARATED)
        {
            frameBuf[i].bufCr = pFrame[i]->vbCr.phys_addr;
        }
        if (dispImage)
        {
            ClearFrameBuffer(instIdx, i);
//          DBG_DIRECT(".");
        }
    }
    JpgLeaveLock();


    // sw_mixer_close(instIdx);
    // sw_mixer_open(instIdx, dispWidth, dispHeight);

//  pYuv = malloc(framebufSize);
    // pYuv = jpg_malloc(framebufSize);
    pYuv = jpeg_heap_get_mem();
    if (!pYuv)
    {
        DBG_DIRECT("Fail to allocation memory for display buffer\n");
        goto ERR_DEC_OPEN;
    }

    ret = JPU_DecGiveCommand(handle, SET_JPG_USE_PARTIAL_MODE,  &(decConfig.usePartialMode));
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGiveCommand[SET_JPG_USE_PARTIAL_MODE] failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }
    ret = JPU_DecGiveCommand(handle, SET_JPG_PARTIAL_FRAME_NUM, &(decConfig.partialBufNum));
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGiveCommand[SET_JPG_PARTIAL_FRAME_NUM] failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }
    ret = JPU_DecGiveCommand(handle, SET_JPG_PARTIAL_LINE_NUM,  &(partialHeight));
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGiveCommand[SET_JPG_PARTIAL_LINE_NUM] failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }

    // Register frame buffers requested by the decoder.
    ret = JPU_DecRegisterFrameBuffer(handle, frameBuf, regFrameBufCount, framebufStride);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecRegisterFrameBuffer failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }
    ppIdx = 0;

//  DBG_DIRECT("Dec Start : Press enter key to show menu.\n" );
//  DBG_DIRECT("          : Press space key to stop.\n" );
    DBG_DIRECT("framebufFormat %d dispHeight %d", framebufFormat, dispHeight);
    DBG_DIRECT("decConfig.outNum 0x%x 0x%x %d  frameIdx  0x%x %d ", &decConfig, &decConfig.outNum,
               decConfig.outNum, &frameIdx, frameIdx);
    while (1)
    {

        if (rotEnable)
        {
            JPU_DecGiveCommand(handle, SET_JPG_ROTATION_ANGLE, &(decConfig.rotAngle));
            JPU_DecGiveCommand(handle, SET_JPG_MIRROR_DIRECTION, &(decConfig.mirDir));
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_OUTPUT, &frameBuf[ppIdx]);
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_STRIDE, &framebufStride);

            JPU_DecGiveCommand(handle, ENABLE_JPG_ROTATION, 0);
            JPU_DecGiveCommand(handle, ENABLE_JPG_MIRRORING, 0);
        }

        JPU_DecGiveCommand(handle, SET_JPG_SCALE_HOR,  &(decConfig.iHorScaleMode));
        JPU_DecGiveCommand(handle, SET_JPG_SCALE_VER,  &(decConfig.iVerScaleMode));

        if (decConfig.usePartialMode)
        {
            partPosIdx = 0;
            partBufIdx = 0;
            outputInfo.decodingSuccess = 0;
            JPU_SWReset();
        }

        // Start decoding a frame.
        ret = JPU_DecStartOneFrame(handle, &decParam);
        if (ret != JPG_RET_SUCCESS && ret != JPG_RET_EOS)
        {
            if (ret == JPG_RET_BIT_EMPTY)
            {
                ret = WriteJpgBsBufHelper(handle, &bufInfo, decOP.bitstreamBuffer,
                                          decOP.bitstreamBuffer + decOP.bitstreamBufferSize, STREAM_FILL_SIZE, 0, &streameos,
                                          decOP.streamEndian);
                if (ret != JPG_RET_SUCCESS)
                {
                    DBG_DIRECT("WriteBsBufHelper failed Error code is 0x%x \n", ret);
                    goto ERR_DEC_OPEN;
                }
                continue;
            }

            DBG_DIRECT("JPU_DecStartOneFrame failed Error code is 0x%x \n", ret);
            goto ERR_DEC_OPEN;
        }
        if (ret == JPG_RET_EOS)
        {
            goto JPU_END_OF_STREAM;
        }

        while (1)
        {
            int_reason = JPU_WaitInterrupt(JPU_INTERRUPT_TIMEOUT_MS);
            if (int_reason == -1)
            {
                DBG_DIRECT("Error : timeout happened\n");
                JPU_SWReset();
                break;
            }


            if (decConfig.usePartialMode && (int_reason & 0xf0))
            {
                partBufIdx = ((partPosIdx) % decConfig.partialBufNum);

                if ((1 << partBufIdx) & ((int_reason & 0xf0) >> 4))
                {
                    DBG_DIRECT("DECODED : PARTIAL BUFFER IDX %d / POS %d / MAX POS %d / INT_REASON=0x%x\n", partBufIdx,
                               partPosIdx + 1, partMaxIdx, int_reason);

                    if (dispImage)
                    {
                        pDispFrame = FindFrameBuffer(instIdx, frameBuf[partBufIdx].bufY);
#ifdef CNM_FPGA_PLATFORM
                        SetMixerDecOutFrame(pDispFrame, framebufStride, partialHeight);
#endif
                    }
                    else
                    {
                        saveIdx = partBufIdx;
                        if (!SaveYuvPartialImageHelperFormat(fpYuv, pYuv,
                                                             frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                                             dispWidth, dispHeight, partialHeight, framebufStride, decConfig.chromaInterleave, framebufFormat,
                                                             decOP.frameEndian, partPosIdx, frameIdx,
                                                             decOP.packedFormat))
                        {
                            goto ERR_DEC_OPEN;
                        }

                        // sw_mixer_draw(instIdx, 0, 0, dispWidth, partialHeight, framebufFormat, decOP.packedFormat, decOP.chromaInterleave, pYuv);
                    }

                    partPosIdx++;
                    JPU_ClrStatus((1 << (INT_JPU_PARIAL_BUF0_EMPTY + partBufIdx)));
                    // next part frame
                    continue;
                }
                else
                {
                    DBG_DIRECT("Invalid partial interrupt : expected reason =0x%x, actual reason=0x%x \n",
                               (1 << partBufIdx), ((int_reason & 0xF0) >> 4));
                    goto ERR_DEC_OPEN;
                }
            }
            if (int_reason & (1 << INT_JPU_DONE) ||
                int_reason & (1 << INT_JPU_ERROR)) // Must catch PIC_DONE interrupt before catching EMPTY interrupt
            {
//              DBG_DIRECT("INT_JPU_DONE\n");
                // Do no clear INT_JPU_DONE and INT_JPU_ERROR interrupt. these will be cleared in JPU_DecGetOutputInfo.
                break;
            }

            if (int_reason & (1 << INT_JPU_BIT_BUF_EMPTY))
            {
                DBG_DIRECT("INT_JPU_BIT_BUF_EMPTY \n");
                ret = WriteJpgBsBufHelper(handle, &bufInfo, decOP.bitstreamBuffer,
                                          decOP.bitstreamBuffer + decOP.bitstreamBufferSize, STREAM_FILL_SIZE, 0, &streameos,
                                          decOP.streamEndian);
                if (ret != JPG_RET_SUCCESS)
                {
                    DBG_DIRECT("WriteBsBufHelper failed Error code is 0x%x \n", ret);
                    goto ERR_DEC_OPEN;
                }
                JPU_ClrStatus((1 << INT_JPU_BIT_BUF_EMPTY));
            }

            if (int_reason & (1 << INT_JPU_BIT_BUF_STOP))
            {
                DBG_DIRECT("INT_JPU_BIT_BUF_STOP \n");
                ret = JPU_DecCompleteStop(handle);
                if (ret != JPG_RET_SUCCESS)
                {
                    DBG_DIRECT("JPU_DecCompleteStop failed Error code is 0x%x \n", ret);
                    goto ERR_DEC_OPEN;
                }
                JPU_ClrStatus((1 << INT_JPU_BIT_BUF_STOP));
                break;
            }

            if (int_reason & (1 << INT_JPU_PARIAL_OVERFLOW))
            {
                DBG_DIRECT("INT_JPU_PARIAL_OVERFLOW \n");
                JPU_ClrStatus((1 << INT_JPU_PARIAL_OVERFLOW));
            }


        }

JPU_END_OF_STREAM:
//      DBG_DIRECT("\nEnter JPU_END_OF_STREAM. \n");
        DBG_DIRECT("framebufFormat 0x%x %d dispHeight 0x%x %d", &framebufFormat, framebufFormat,
                   &dispHeight, dispHeight);
        ret = JPU_DecGetOutputInfo(handle, &outputInfo);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("JPU_DecGetOutputInfo failed Error code is 0x%x \n", ret);
            goto ERR_DEC_OPEN;
        }

#ifdef MJPEG_ERROR_CONCEAL
        if (outputInfo.numOfErrMBs)
        {
            int errRstIdx, errPosX, errPosY;
            errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
            errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
            errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
            DBG_DIRECT("Error restart Idx : %d, MCU x:%d, y:%d, in Frame : %d \n", errRstIdx, errPosX, errPosY,
                       frameIdx);
            continue;
        }
#endif

        if (outputInfo.decodingSuccess == 0)
        {
            DBG_DIRECT("JPU_DecGetOutputInfo decode fail framdIdx %d \n", frameIdx);
        }


        // DBG_DIRECT("#%d:%d, indexFrameDisplay %d || consumedByte %d || ppIdx %d || frameStart=0x%x || ecsStart=0x%x || rdPtr=0x%x || wrPtr=0x%x || pos=%d\n",
        //  instIdx, frameIdx, outputInfo.indexFrameDisplay, outputInfo.consumedByte, ppIdx, outputInfo.bytePosFrameStart, outputInfo.bytePosFrameStart+outputInfo.ecsPtr, JpuReadReg(MJPEG_BBC_RD_PTR_REG), JpuReadReg(MJPEG_BBC_WR_PTR_REG), JpuReadReg(MJPEG_BBC_CUR_POS_REG));

        if (outputInfo.indexFrameDisplay == -1)
        {
            break;
        }

        // YUV Dump Done when partial buffer is all displayed.
        int_reason = JPU_GetStatus();
        if (decConfig.usePartialMode && !(int_reason & 0xF0))
        {
            goto SKIP_BUF_DUMP;
        }
        // indexFrameDisplay points to the frame buffer, among ones registered, which holds
        // the output of the decoder.
        dispImage = 0;
        if (dispImage)
        {
#ifdef CNM_FPGA_PLATFORM
            if (frameIdx)
            {
                WaitMixerInt();
            }
#endif
            if (!rotEnable)
            {
                int a = 0;
                saveIdx = outputInfo.indexFrameDisplay;
                extern int getFramBuffyuv(PhysicalAddress addrY, PhysicalAddress addrU, PhysicalAddress addrV);
                a = getFramBuffyuv(frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr);
                pDispFrame = FindFrameBuffer(instIdx, frameBuf[saveIdx].bufY);
#ifdef CNM_FPGA_PLATFORM
                SetMixerDecOutFrame(pDispFrame, outputInfo.decPicWidth, outputInfo.decPicHeight);
#endif
            }
            else
            {
                pDispFrame = FindFrameBuffer(instIdx, frameBuf[ppIdx].bufY);
#ifdef CNM_FPGA_PLATFORM
                SetMixerDecOutFrame(pDispFrame,
                                    (decConfig.rotAngle == 90 ||
                                     decConfig.rotAngle == 270) ? outputInfo.decPicHeight : outputInfo.decPicWidth,
                                    (decConfig.rotAngle == 90 ||
                                     decConfig.rotAngle == 270) ? outputInfo.decPicWidth : outputInfo.decPicHeight);
#endif

                ppIdx = (ppIdx - regFrameBufCount + 1) % MAX_ROT_BUF_NUM;

            }
        }
        else // store image
        {
            DBG_DIRECT("rotEnable %d ", rotEnable);
            if (!rotEnable)
            {
                saveIdx = outputInfo.indexFrameDisplay;
                DBG_DIRECT("framebufFormat %d dispHeight %d", framebufFormat, dispHeight);
                if (!SaveYuvImageHelperFormat(fpYuv, pYuv,
                                              frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                              dispWidth, dispHeight, framebufStride,
                                              decConfig.chromaInterleave, framebufFormat, decOP.frameEndian,
                                              decOP.packedFormat))
                {
                    goto ERR_DEC_OPEN;
                }
            }
            else
            {
                saveIdx = ppIdx;

                if (!SaveYuvImageHelperFormat(fpYuv, pYuv,
                                              frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                              dispWidth, dispHeight,
                                              framebufStride, decConfig.chromaInterleave, framebufFormat, decOP.frameEndian,
                                              decOP.packedFormat))
                {
                    goto ERR_DEC_OPEN;
                }
                ppIdx = (ppIdx - regFrameBufCount + 1) % MAX_ROT_BUF_NUM;
            }
            // sw_mixer_draw(instIdx, 0, 0, dispWidth, dispHeight, framebufFormat, decOP.packedFormat, decOP.chromaInterleave,  pYuv);
        }

SKIP_BUF_DUMP:
//      DBG_DIRECT("\nEnter SKIP_BUF_DUMP. \n");
        if (outputInfo.numOfErrMBs)
        {
            int errRstIdx, errPosX, errPosY;
            errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
            errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
            errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
            // DBG_DIRECT("Error restart Idx : %d, MCU x:%d, y:%d, in Frame : %d \n", errRstIdx, errPosX, errPosY, frameIdx);
        }
        frameIdx++;
        decConfig.outNum = 1;
        DBG_DIRECT("decConfig.outNum 0x%x 0x%x %d  frameIdx  0x%x %d ", &decConfig, &(decConfig.outNum),
                   decConfig.outNum, &frameIdx, frameIdx);
        if (decConfig.outNum && (frameIdx == decConfig.outNum))
        {
            break;
        }

    }
    if (totalNumofErrMbs)
    {
        suc = 0;
        // DBG_DIRECT("Total Num of Error MBs : %d\n", totalNumofErrMbs);
    }

ERR_DEC_OPEN:
    // Now that we are done with decoding, close the open instance.
    // DBG_DIRECT("\nEnter Dec End. \n");
    ret = JPU_DecClose(handle);

    // DBG_DIRECT("\nDec End. Tot Frame %d\n", frameIdx);




ERR_DEC_INIT:
//  DBG_DIRECT("\nEnter DEC_INIT. \n");
    // FreeFrameBuffer(instIdx);

    // jdi_free_dma_memory(&vbStream);


    if (pYuv)
    {
        jpg_free(pYuv);
    }

    // if (pFileBuf)
    //  free(pFileBuf);

    //sw_mixer_close(instIdx);
    JPU_DeInit();

    return suc;

}

#endif

uint32_t CODA_Test(uint8_t cmd)
{
    int ret = 0;

    RCC_MJPEG_Config_Enable();
    switch (cmd)
    {
    case 1:
        CODA_Test_read(MJPEG_OP_INFO_REG);
        break;
    case 2:
        CODA_Test_write(MJPEG_OP_INFO_REG, 0x02);
        break;
    case 3: // decode
        {
#ifdef JPEG_DECODER_EN
            DecConfigParam  decConfig;
            memset((void *)&decConfig, 0x00, sizeof(DecConfigParam));

            // decConfig.bitstreamFileName = %s;
            decConfig.roiEnable = 0;
            if (decConfig.roiEnable)
            {
                // printf("Enter ROI offsetX:");
                decConfig.roiOffsetX = 0;
                decConfig.roiOffsetY = 0;
                decConfig.roiWidth = 100;
                decConfig.roiHeight = 100;
            }
            // Packed stream format output [0](PLANAR) [1](YUYV) [2](UYVY) [3](YVYU) [4](VYUY) [5](YUV_444 PACKED)
            decConfig.packedFormat = 4;
            // Chroma format type [0](SEPARATED CHROMA) [1](CBCR INTERLEAVED) [2](CRCB INTERLEAVED)
            decConfig.chromaInterleave = 0;
            decConfig.StreamEndian = JPU_STREAM_ENDIAN;
            decConfig.FrameEndian = JPU_FRAME_ENDIAN;

            if (!decConfig.roiEnable && !decConfig.packedFormat)
            {
                // partial Mode(0: OFF 1: ON);
                decConfig.usePartialMode = 0;

                if (decConfig.usePartialMode)
                {
                    // Num of Frame Buffer[ 2 ~ 4 ] ;
                    decConfig.partialBufNum = 2;
                }
            }

            if (!decConfig.usePartialMode)
            {
                // rotation angle in degrees(0, 90, 180, 270);
                decConfig.rotAngle = 0;
                if (decConfig.rotAngle != 0 && decConfig.rotAngle != 90 && decConfig.rotAngle != 180 &&
                    decConfig.rotAngle != 270)
                {
                    DBG_DIRECT("Invalid rotation angle.\n");
                    break;
                }
                // mirror direction(0-no mirror, 1-vertical, 2-horizontal, 3-both);
                decConfig.mirDir = 0;
                if (decConfig.mirDir != 0 && decConfig.mirDir != 1 && decConfig.mirDir != 2 &&
                    decConfig.mirDir != 3)
                {
                    DBG_DIRECT("Invalid mirror direction.\n");
                    break;
                }
                if (decConfig.rotAngle != 0 || decConfig.mirDir != 0)
                {
                    decConfig.useRot = 1;
                }
            }
            // Store image?
//            decConfig.yuvFileName[0] = '\0';
            // Number of Images that you want to decode(0: decode continue, -1: loop);
            decConfig.outNum = 1;
            ret = CODA_Decode_Test(&decConfig);

            if (!ret)
            {
                DBG_DIRECT("\nFailed to DecodeTest\n");
            }

#endif
        }
        break;

    case 4: // encoder
        {
        }
        break;
    }


    return 0;
}
