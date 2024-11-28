//#include "time_adapter.h"
#include "trace.h"
#include "rtl876x.h"
#include "rtl876x_pinmux.h"
//#include "rtl876x_aon.h"
#include "rtl876x_rcc.h"
#include "string.h"

#include "jpeg_coda.h"
#include "regdefine.h"
#include "jpuconfig.h"
#include "jpuapi.h"
#include "mixer.h"
#include "jpuhelper_rt.h"
#include "jpuapifunc_rt.h"
#include "jpeg_heap.h"

// #include "pic_hex.txt"
// #include "pic_hex_112_144.txt"  // 420
// #include "pic_hex_16_32.txt"
// #include "pic_hex_184_96.txt"    // 444
// #include "pic_hex_400_184_96.txt"   // 400
// #include "pic_hex_422_192_96.txt"  // 422
// #include "pic_hex_444_184_96.txt"  // 444

// #include "pic_hex_420_192_96.txt"  // 420


// #include "pic_hex_444_184x96_95.txt"  // 444 95
// #include "pic_hex_440_192x96_95.txt"   // 440 95
#include "decode_test.txt"




#define BLOCK_ALIGN(x,n) (((x + (n) - 1) / (n)) * (n))


// #define SRC_FLASH


uint8_t f_img_index = 4;
struct Dec_ret
{
    int format;
    int w;
    int h;
    int pix;
    int clk;
    int speed;
    int rate;
};

struct Dec_ret dec_ret[44];

void coda_dec_auto(void)
{
    for (uint32_t i = 0; i < 44; i++)
    {
        f_img_index = i;
        CODA_Test(3);
    }

}

void coda_dec_res_log(void)
{
    for (uint32_t i = 0; i < 44; i++)
    {
        DBG_DIRECT("\n[%d] format %d pix %dx%d=%d clk %d speed %d r= %d\n", (i + 3) / 4, \
                   dec_ret[i].format, \
                   dec_ret[i].w, dec_ret[i].h, \
                   dec_ret[i].pix, \
                   dec_ret[i].clk, \
                   dec_ret[i].speed, dec_ret[i].rate);

    }

}


//PCC config functin
uint32_t coda_init(void)
{
    DBG_DIRECT("MJPEG RCC ENABLE");
    RCC_PeriphClockCmd(APBPeriph_JPEG, APBPeriph_JPEG_CLOCK, ENABLE);
    //Function enable
    // DBG_DIRECT("BIT28 reg_0x210 = 0x%x", *((uint32_t*) 0x40000210));
    //clock enable
    // DBG_DIRECT("BIT16_17 reg_0x23c = 0x%x", *((uint32_t*) 0x4000023C));
    return 0;
}
uint32_t coda_deinit(void)
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
    DBG_DIRECT("JPEG rd 0x%03x: 0x%x %d\n", addr & 0XFFF, data, data);
    return data;
}
uint32_t CODA_Test_write(uint32_t addr, uint32_t val)
{
    CODA_Test_read(addr);

    uint32_t *reg_addr = (uint32_t *)(addr);
    // *reg_addr = val;
    jdi_write_register(addr, val);
    DBG_DIRECT("JPEG wr 0x%03x: 0x%x\n", addr & 0XFFF, val);

    CODA_Test_read(addr);
    return 0;
}

// -------------------------------------------------------------------------

#define NUM_FRAME_BUF           MAX_FRAME
#define EXTRA_FRAME_BUFFER_NUM  0
#define MAX_ROT_BUF_NUM         1

static DecConfigParam jdec_config;

#if 0
static int coda_decode_partial(void)
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
    uint32_t bsSize = 0, framebufSize = 0, framebufWidth = 0, framebufHeight = 0, framebufStride = 0,
             framebufFormat = FORMAT_420;
    int dispWidth = 0, dispHeight = 0;
    int i = 0, frameIdx = 0, ppIdx = 0, saveIdx = 0, totalNumofErrMbs = 0, streameos = 0, dispImage = 0;
    int suc = 1;
    uint8_t *pFileBuf = NULL;
    uint8_t *pYuv  =  NULL;
    void *fpYuv  =  NULL;
    int needFrameBufCount = 0, regFrameBufCount = 0;
    int rotEnable = 0;
    int int_reason = 0;
    int instIdx;
    int partPosIdx = 0;
    int partBufIdx = 0;
    int partMaxIdx = 0;
    int partialHeight = 0;

    memset(&pFrame, 0x00, sizeof(FRAME_BUF *)*NUM_FRAME_BUF);
    memset(&frameBuf, 0x00, sizeof(FrameBuffer)*NUM_FRAME_BUF);

    instIdx = jdec_config.instNum;

    // store file or display
    dispImage = 1;

    // bitstream
    bsSize = sizeof(pic) / sizeof(pic[0]);
    pFileBuf = pic;
    if (!pFileBuf)
    {
        goto ERR_DEC_INIT;
    }

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
    DBG_DIRECT("jdi_allocate_dma_memory Done\n");

    decOP.streamEndian = jdec_config.StreamEndian;
    decOP.frameEndian = jdec_config.FrameEndian;
    decOP.chromaInterleave = (CbCrInterLeave)jdec_config.chromaInterleave;
    decOP.bitstreamBuffer = vbStream.phys_addr;
    decOP.bitstreamBufferSize = vbStream.size;
    decOP.pBitStream = (BYTE *)vbStream.virt_addr; // set virtual address mapped of physical address
    decOP.packedFormat = jdec_config.packedFormat;

    decOP.roiEnable = jdec_config.roiEnable;
    decOP.roiOffsetX = jdec_config.roiOffsetX;
    decOP.roiOffsetY = jdec_config.roiOffsetY;
    decOP.roiWidth = jdec_config.roiWidth;
    decOP.roiHeight = jdec_config.roiHeight;

    ret = JPU_DecOpen(&handle, &decOP);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecOpen failed Error code is 0x%x \n", ret);
        goto ERR_DEC_INIT;
    }
    DBG_DIRECT("JPU_DecOpen Done\n");

    if (jdec_config.useRot)
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


    if (jdec_config.usePartialMode)
    {
        // disable Rotator, Scaler
        rotEnable = 0;
        jdec_config.iHorScaleMode = 0;
        jdec_config.iVerScaleMode = 0;
        partialHeight = (initialInfo.sourceFormat == FORMAT_420 ||
                         initialInfo.sourceFormat == FORMAT_224) ? 16 : 8;

        partMaxIdx  = ((initialInfo.picHeight + 15) & ~15) / partialHeight;
        if (partMaxIdx < jdec_config.partialBufNum)
        {
            jdec_config.partialBufNum = partMaxIdx;
        }
        DBG_DIRECT("Num of Buffer for Partial : %d\n ", jdec_config.partialBufNum);
        DBG_DIRECT("Num of Line for Partial   : %d\n ", partialHeight);
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

    if (jdec_config.roiEnable)
    {
        framebufWidth  = initialInfo.roiFrameWidth ;
        framebufHeight = initialInfo.roiFrameHeight;
    }


    // scaler constraint when conformance test is disable
    if (framebufWidth < 128 || framebufHeight < 128)
    {
        if (jdec_config.iHorScaleMode || jdec_config.iVerScaleMode)
        {
            DBG_DIRECT("Invalid operation mode : Not supported resolution with Scaler, width=%d, height=%d\n",
                       framebufWidth, framebufHeight);
        }
        jdec_config.iHorScaleMode = 0;
        jdec_config.iVerScaleMode = 0;
    }

    framebufFormat = initialInfo.sourceFormat;

    if (jdec_config.iHorScaleMode || jdec_config.iVerScaleMode)
    {
        framebufHeight = ((framebufHeight + 1) / 2) * 2;
        framebufWidth = ((framebufWidth + 1) / 2) * 2;
    }

    framebufWidth  >>= jdec_config.iHorScaleMode;
    framebufHeight >>= jdec_config.iVerScaleMode;

    dispWidth = (jdec_config.rotAngle == 90 ||
                 jdec_config.rotAngle == 270) ? framebufHeight : framebufWidth;
    dispHeight = (jdec_config.rotAngle == 90 ||
                  jdec_config.rotAngle == 270) ? framebufWidth : framebufHeight;
    JPU_DecGiveCommand(handle, SET_JPG_DISP_WIDTH, &dispWidth);


    if (jdec_config.rotAngle == 90 || jdec_config.rotAngle == 270)
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

    if (jdec_config.iHorScaleMode || jdec_config.iVerScaleMode)
    {
        framebufStride = ((framebufStride + 15) / 16) * 16;
    }

    if (decOP.packedFormat >= PACKED_FORMAT_422_YUYV && decOP.packedFormat <= PACKED_FORMAT_422_VYUY)
    {
        framebufWidth = framebufWidth * 2;
        framebufStride = framebufStride * 2;
        framebufFormat = FORMAT_422;
        if (jdec_config.rotAngle == 90 || jdec_config.rotAngle == 270)
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

    DBG_DIRECT("framebufFormat %d", framebufFormat);

    framebufSize = GetFrameBufSize(framebufFormat, initialInfo.picWidth, initialInfo.picHeight);
//  DBG_DIRECT("framebuffer stride: %d,  width: %d, height = %d\n", framebufStride, framebufWidth, framebufHeight);
//  DBG_DIRECT("framebuffer format: %d, framebuffer size = %d, packed format = %d\n", framebufFormat, framebufSize, decOP.packedFormat);
    DBG_DIRECT("framebufSize %d frame width: %d, stride : %d, height = %d\n", framebufSize,
               framebufWidth, framebufStride, framebufHeight);


    //Allocate frame buffer
    regFrameBufCount = initialInfo.minFrameBufferCount + EXTRA_FRAME_BUFFER_NUM;


    if (jdec_config.usePartialMode)
    {
        if (jdec_config.partialBufNum > 4)
        {
            jdec_config.partialBufNum = 4;
        }

        regFrameBufCount *= jdec_config.partialBufNum;
    }

    needFrameBufCount = regFrameBufCount;

    if (AllocateFrameBuffer(instIdx, framebufFormat, framebufStride, framebufHeight, needFrameBufCount,
                            (decOP.packedFormat >= PACKED_FORMAT_422_YUYV)) == 0)
    {
        DBG_DIRECT("fail to AllocateFrameBuffer\n");
        goto ERR_DEC_OPEN;
    }
    DBG_DIRECT("AllocateFrameBuffer DONE\n");

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

    pYuv = jpeg_heap_get_mem();

    ret = JPU_DecGiveCommand(handle, SET_JPG_USE_PARTIAL_MODE,  &(jdec_config.usePartialMode));
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGiveCommand[SET_JPG_USE_PARTIAL_MODE] failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }
    ret = JPU_DecGiveCommand(handle, SET_JPG_PARTIAL_FRAME_NUM, &(jdec_config.partialBufNum));
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

    while (1)
    {
        if (rotEnable)
        {
            JPU_DecGiveCommand(handle, SET_JPG_ROTATION_ANGLE, &(jdec_config.rotAngle));
            JPU_DecGiveCommand(handle, SET_JPG_MIRROR_DIRECTION, &(jdec_config.mirDir));
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_OUTPUT, &frameBuf[ppIdx]);
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_STRIDE, &framebufStride);

            JPU_DecGiveCommand(handle, ENABLE_JPG_ROTATION, 0);
            JPU_DecGiveCommand(handle, ENABLE_JPG_MIRRORING, 0);
        }

        JPU_DecGiveCommand(handle, SET_JPG_SCALE_HOR,  &(jdec_config.iHorScaleMode));
        JPU_DecGiveCommand(handle, SET_JPG_SCALE_VER,  &(jdec_config.iVerScaleMode));

        if (jdec_config.usePartialMode)
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


            if (jdec_config.usePartialMode && (int_reason & 0xf0))
            {
                partBufIdx = ((partPosIdx) % jdec_config.partialBufNum);

                if ((1 << partBufIdx) & ((int_reason & 0xf0) >> 4))
                {
                    DBG_DIRECT("DECODED : PARTIAL BUFFER IDX %d / POS %d / MAX POS %d / INT_REASON=0x%x\n", partBufIdx,
                               partPosIdx + 1, partMaxIdx, int_reason);

                    if (dispImage)
                    {
                        pDispFrame = FindFrameBuffer(instIdx, frameBuf[partBufIdx].bufY);
                    }
                    else
                    {
                        saveIdx = partBufIdx;
                        if (!SaveYuvPartialImageHelperFormat(fpYuv, pYuv,
                                                             frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                                                             dispWidth, dispHeight, partialHeight, framebufStride, jdec_config.chromaInterleave, framebufFormat,
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
        // DBG_DIRECT("framebufFormat 0x%x %d dispHeight 0x%x %d", &framebufFormat, framebufFormat,
        //            &dispHeight, dispHeight);
        ret = JPU_DecGetOutputInfo(handle, &outputInfo);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("JPU_DecGetOutputInfo failed Error code is 0x%x \n", ret);
            goto ERR_DEC_OPEN;
        }

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
        if (jdec_config.usePartialMode && !(int_reason & 0xF0))
        {
            goto SKIP_BUF_DUMP;
        }
        // indexFrameDisplay points to the frame buffer, among ones registered, which holds
        // the output of the decoder.
        /*
        dispImage = 0;
        if (dispImage)
        {
            if (!rotEnable)
            {
                int a = 0;
                saveIdx = outputInfo.indexFrameDisplay;
                extern int getFramBuffyuv(PhysicalAddress addrY, PhysicalAddress addrU, PhysicalAddress addrV);
                a = getFramBuffyuv(frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr);
                pDispFrame = FindFrameBuffer(instIdx, frameBuf[saveIdx].bufY);
            }
            else
            {
                pDispFrame = FindFrameBuffer(instIdx, frameBuf[ppIdx].bufY);

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
                                              jdec_config.chromaInterleave, framebufFormat, decOP.frameEndian,
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
                                              framebufStride, jdec_config.chromaInterleave, framebufFormat, decOP.frameEndian,
                                              decOP.packedFormat))
                {
                    goto ERR_DEC_OPEN;
                }
                ppIdx = (ppIdx - regFrameBufCount + 1) % MAX_ROT_BUF_NUM;
            }
            // sw_mixer_draw(instIdx, 0, 0, dispWidth, dispHeight, framebufFormat, decOP.packedFormat, decOP.chromaInterleave,  pYuv);
        }
        */

SKIP_BUF_DUMP:
        DBG_DIRECT("\nEnter SKIP_BUF_DUMP. \n");
        if (outputInfo.numOfErrMBs)
        {
            int errRstIdx, errPosX, errPosY;
            errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
            errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
            errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
            // DBG_DIRECT("Error restart Idx : %d, MCU x:%d, y:%d, in Frame : %d \n", errRstIdx, errPosX, errPosY, frameIdx);
        }
        frameIdx++;
//        jdec_config.outNum = 1;
        DBG_DIRECT("jdec_config.outNum 0x%x 0x%x %d  frameIdx  0x%x %d ", &jdec_config,
                   &(jdec_config.outNum),
                   jdec_config.outNum, &frameIdx, frameIdx);
        if (jdec_config.outNum && (frameIdx == jdec_config.outNum))
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
    DBG_DIRECT("\nEnter Dec End. \n");
    ret = JPU_DecClose(handle);

    // DBG_DIRECT("\nDec End. Tot Frame %d\n", frameIdx);

ERR_DEC_INIT:
    DBG_DIRECT("\nEnter ERR_DEC_INIT. \n");
    // FreeFrameBuffer(instIdx);

    // jdi_free_dma_memory(&vbStream);

    //sw_mixer_close(instIdx);
    JPU_DeInit();

    return suc;

}

#endif








static int coda_decode(uint8_t *jpg_addr, uint64_t jpg_sz)
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
    uint32_t framebufSize = 0, framebufWidth = 0, framebufHeight = 0, framebufStride = 0,
             framebufFormat = FORMAT_420;
    int dispWidth = 0, dispHeight = 0;
    int i = 0, frameIdx = 0, ppIdx = 0, saveIdx = 0, streameos = 0;
    int suc = 1;
    int needFrameBufCount = 0, regFrameBufCount = 0;
    int rotEnable = 0;
    int int_reason = 0;
    int instIdx;
    int partPosIdx = 0;
    int partBufIdx = 0;
    int partMaxIdx = 0;
    int partialHeight = 0;
    uint8_t *pYuv  =  NULL;

    DBG_DIRECT("%s %d\n", __FUNCTION__, __LINE__);
    memset(&pFrame, 0x00, sizeof(FRAME_BUF *)*NUM_FRAME_BUF);
    memset(&frameBuf, 0x00, sizeof(FrameBuffer)*NUM_FRAME_BUF);

    instIdx = jdec_config.instNum;

    // source image buffer
    bufInfo.buf = jpg_addr;
    bufInfo.size = jpg_sz;

    ret = JPU_Init();
    if (ret != JPG_RET_SUCCESS &&
        ret != JPG_RET_CALLED_BEFORE)
    {
        suc = 0;
        DBG_DIRECT("JPU_Init failed Error code is 0x%x \n", ret);
        // goto ERR_DEC_INIT;
    }
    DBG_DIRECT("JPU_Init Done\n");


    // Open an instance and get initial information for decoding.
    vbStream.size = ((bufInfo.size + 1023) >> 10 << 10);
    jdi_register_extern_memory(&vbStream, bufInfo.buf, bufInfo.size);
    DBG_DIRECT("jdi_register_extern_memory Done\n");
#if 0
    else
    {
        if (jdi_allocate_dma_memory(&vbStream) < 0)
        {
            DBG_DIRECT("fail to allocate bitstream buffer\n");
            goto ERR_DEC_INIT;
        }
        DBG_DIRECT("jdi_allocate_dma_memory Done\n");
    }
#endif

    decOP.streamEndian = jdec_config.StreamEndian;
    decOP.frameEndian = jdec_config.FrameEndian;
    decOP.chromaInterleave = (CbCrInterLeave)jdec_config.chromaInterleave;
    decOP.bitstreamBuffer = vbStream.phys_addr;
    decOP.bitstreamBufferSize = vbStream.size;
    decOP.pBitStream = (BYTE *)vbStream.virt_addr; // set virtual address mapped of physical address
    decOP.packedFormat = jdec_config.packedFormat;

    decOP.roiEnable = jdec_config.roiEnable;
    decOP.roiOffsetX = jdec_config.roiOffsetX;
    decOP.roiOffsetY = jdec_config.roiOffsetY;
    decOP.roiWidth = jdec_config.roiWidth;
    decOP.roiHeight = jdec_config.roiHeight;


    ret = JPU_DecOpen(&handle, &decOP);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecOpen failed Error code is 0x%x \n", ret);
        goto ERR_DEC_INIT;
    }
    DBG_DIRECT("JPU_DecOpen Done\n");

    // load img
    ret = SyncJpgBsBufHelper(handle, &bufInfo, decOP.bitstreamBuffer,
                             decOP.bitstreamBuffer + decOP.bitstreamBufferSize, 0, 0, &streameos, decOP.streamEndian);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("SyncJpgBsBufHelper failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }
    DBG_DIRECT("SyncJpgBsBufHelper Done\n");
    DBG_DIRECT("No need to load src\n");

#if 0
    else
    {
        ret = WriteJpgBsBufHelper(handle, &bufInfo, decOP.bitstreamBuffer,
                                  decOP.bitstreamBuffer + decOP.bitstreamBufferSize, 0, 0, &streameos, decOP.streamEndian);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("WriteBsBufHelper failed Error code is 0x%x \n", ret);
            goto ERR_DEC_OPEN;
        }
        DBG_DIRECT("WriteJpgBsBufHelper Done\n");
    }
#endif

    DBG_DIRECT("\nbs  0x%x\n", *(volatile unsigned long *)(decOP.bitstreamBuffer));
    ret = JPU_DecGetInitialInfo(handle, &initialInfo);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGetInitialInfo failed Error code is 0x%x, inst=%d \n", ret, instIdx);
        goto ERR_DEC_OPEN;
    }
    DBG_DIRECT("JPU_DecGetInitialInfo Done\n");

    if (jdec_config.usePartialMode)
    {
        // disable Rotator, Scaler
        rotEnable = 0;
        jdec_config.iHorScaleMode = 0;
        jdec_config.iVerScaleMode = 0;
        partialHeight = (initialInfo.sourceFormat == FORMAT_420 ||
                         initialInfo.sourceFormat == FORMAT_224) ? 16 : 8;

        partMaxIdx  = ((initialInfo.picHeight + 15) & ~15) / partialHeight;
        if (partMaxIdx < jdec_config.partialBufNum)
        {
            jdec_config.partialBufNum = partMaxIdx;
        }
        DBG_DIRECT("Num of Buffer for Partial : %d\n ", jdec_config.partialBufNum);
        DBG_DIRECT("Num of Line for Partial   : %d\n ", partialHeight);
    }



    // roi
    if (jdec_config.roiEnable)
    {
        framebufWidth  = initialInfo.roiFrameWidth ;
        framebufHeight = initialInfo.roiFrameHeight;
    }
    else
    {
        if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_422)
        {
            framebufWidth = BLOCK_ALIGN(initialInfo.picWidth, 16);
        }
        else
        {
            framebufWidth = BLOCK_ALIGN(initialInfo.picWidth, 8);
        }

        if (initialInfo.sourceFormat == FORMAT_420 || initialInfo.sourceFormat == FORMAT_224)
        {
            framebufHeight = BLOCK_ALIGN(initialInfo.picHeight, 16);
        }
        else
        {
            framebufHeight = BLOCK_ALIGN(initialInfo.picHeight, 8);
        }


        // scaler constraint when conformance test is disable
        if (framebufWidth < 128 || framebufHeight < 128)
        {
            if (jdec_config.iHorScaleMode || jdec_config.iVerScaleMode)
            {
                DBG_DIRECT("Invalid operation mode : Not supported resolution with Scaler, width=%d, height=%d\n",
                           framebufWidth, framebufHeight);
            }
            jdec_config.iHorScaleMode = 0;
            jdec_config.iVerScaleMode = 0;
        }

        if (jdec_config.iHorScaleMode || jdec_config.iVerScaleMode)
        {
            framebufHeight = ((framebufHeight + 1) / 2) * 2;
            framebufWidth = ((framebufWidth + 1) / 2) * 2;
        }

        framebufWidth  >>= jdec_config.iHorScaleMode;
        framebufHeight >>= jdec_config.iVerScaleMode;
    }

    // rotate
    if (jdec_config.rotAngle == 90 || jdec_config.rotAngle == 270)
    {
        dispWidth = framebufHeight;
        dispHeight = framebufWidth;
        framebufStride = framebufHeight;
        framebufHeight = framebufWidth;
        framebufFormat = (framebufFormat == FORMAT_422) ? FORMAT_224 : (framebufFormat == FORMAT_224) ?
                         FORMAT_422 : framebufFormat;
    }
    else
    {
        dispWidth = framebufWidth;
        dispHeight = framebufHeight;
        framebufStride = framebufWidth;
    }
    JPU_DecGiveCommand(handle, SET_JPG_DISP_WIDTH, &dispWidth);

    if (jdec_config.iHorScaleMode || jdec_config.iVerScaleMode)
    {
        framebufStride = BLOCK_ALIGN(framebufStride, 16);
    }

    DBG_DIRECT(" initialInfo.sourceFormat %d", initialInfo.sourceFormat);
    framebufFormat = initialInfo.sourceFormat;
    if (decOP.packedFormat >= PACKED_FORMAT_422_YUYV && decOP.packedFormat <= PACKED_FORMAT_422_VYUY)
    {
        if (jdec_config.useWrapper)
        {
            if (jdec_config.rgbType == JPG_RGB888)
            {
                framebufWidth = framebufWidth * 3;
                framebufStride = framebufStride * 3;
            }
            else if (jdec_config.rgbType == JPG_ARGB8888)
            {
                framebufWidth = framebufWidth * 4;
                framebufStride = framebufStride * 4;
            }
            else if (jdec_config.rgbType == JPG_RGB565)
            {
                framebufWidth = framebufWidth * 2;
                framebufStride = framebufStride * 2;
            }
        }
        else
        {
            framebufWidth = framebufWidth * 2;
            framebufStride = framebufStride * 2;
        }

        framebufFormat = FORMAT_422;
        if (jdec_config.rotAngle == 90 || jdec_config.rotAngle == 270)
        {
            framebufFormat = FORMAT_224;
        }

    }
    else if (decOP.packedFormat == PACKED_FORMAT_444)
    {
        if (jdec_config.useWrapper)
        {
            if (jdec_config.rgbType == JPG_RGB888)
            {
                framebufWidth = framebufWidth * 3;
                framebufStride = framebufStride * 3;
            }
            else if (jdec_config.rgbType == JPG_ARGB8888)
            {
                framebufWidth = framebufWidth * 4;
                framebufStride = framebufStride * 4;
            }
            else if (jdec_config.rgbType == JPG_RGB565)
            {
                framebufWidth = framebufWidth * 2;
                framebufStride = framebufStride * 2;
            }
        }
        else
        {
            framebufWidth = framebufWidth * 3;
            framebufStride = framebufStride * 3;
        }
        framebufFormat = FORMAT_444;
    }

    DBG_DIRECT("framebufFormat %d initialInfo.sourceFormat %d", framebufFormat,
               initialInfo.sourceFormat);
    // framebufSize = GetFrameBufSize(framebufFormat, initialInfo.picWidth, initialInfo.picHeight);
    DBG_DIRECT("framebufSize %d frame width: %d, stride : %d, height = %d\n", framebufSize,
               framebufWidth, framebufStride, framebufHeight);


    //Allocate frame buffer
    regFrameBufCount = initialInfo.minFrameBufferCount + EXTRA_FRAME_BUFFER_NUM;
    if (jdec_config.usePartialMode)
    {
        regFrameBufCount *= jdec_config.partialBufNum;
    }
    needFrameBufCount = regFrameBufCount;

    if (AllocateFrameBuffer(instIdx, framebufFormat, framebufStride, framebufHeight, needFrameBufCount,
                            (decOP.packedFormat >= PACKED_FORMAT_422_YUYV)) == 0)
    {
        DBG_DIRECT("fail to AllocateFrameBuffer\n");
        goto ERR_DEC_OPEN;
    }
    DBG_DIRECT("AllocateFrameBuffer DONE\n");

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
    }
    JpgLeaveLock();

    // pYuv = jpg_malloc(framebufSize);
    ret = JPU_DecGiveCommand(handle, SET_JPG_USE_PARTIAL_MODE,  &(jdec_config.usePartialMode));
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_DecGiveCommand[SET_JPG_USE_PARTIAL_MODE] failed Error code is 0x%x \n", ret);
        goto ERR_DEC_OPEN;
    }

    ret = JPU_DecGiveCommand(handle, SET_JPG_PARTIAL_FRAME_NUM, &(jdec_config.partialBufNum));
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


    {
        // wrapper
        uint32_t reg =  jdec_config.rgbType | ((jdec_config.rgbType == 0x0 ? jdec_config.opacity : 0x0) <<
                                               8);
        DBG_DIRECT("wrapper %d, type: %d opacity: 0x%x, 0x%x \n", jdec_config.useWrapper,
                   jdec_config.rgbType, jdec_config.opacity, reg);
        CODA_Test_write(REG_MJPEG_WRAP_CFG_REG, (jdec_config.useWrapper ? 0x01 : 0x00));
        // CODA_Test_write(MJPEG_WRAPPER_ENABLE_REG, 0x00);
        CODA_Test_write(REG_MJPEG_RGB_PF_REG, reg);
    }

    uint32_t decode_cnt_clk = 0;
    while (1)
    {
        if (jdec_config.useRot)
        {
            JPU_DecGiveCommand(handle, SET_JPG_ROTATION_ANGLE, &(jdec_config.rotAngle));
            JPU_DecGiveCommand(handle, SET_JPG_MIRROR_DIRECTION, &(jdec_config.mirDir));
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_OUTPUT, &frameBuf[ppIdx]);
            JPU_DecGiveCommand(handle, SET_JPG_ROTATOR_STRIDE, &framebufStride);

            JPU_DecGiveCommand(handle, ENABLE_JPG_ROTATION, 0);
            JPU_DecGiveCommand(handle, ENABLE_JPG_MIRRORING, 0);
        }

        JPU_DecGiveCommand(handle, SET_JPG_SCALE_HOR,  &(jdec_config.iHorScaleMode));
        JPU_DecGiveCommand(handle, SET_JPG_SCALE_VER,  &(jdec_config.iVerScaleMode));

        if (jdec_config.usePartialMode)
        {
            partPosIdx = 0;
            partBufIdx = 0;
            outputInfo.decodingSuccess = 0;
            JPU_SWReset();
        }
        DBG_DIRECT("%s %d\n", __FUNCTION__, __LINE__);
        ret = JPU_DecStartOneFrame(handle, &decParam);
        DBG_DIRECT("%s %d ret %d\n", __FUNCTION__, __LINE__, ret);
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
            DBG_DIRECT("%s %d\n", __FUNCTION__, __LINE__);
            int_reason = JPU_WaitInterrupt(JPU_INTERRUPT_TIMEOUT_MS);
            DBG_DIRECT("%s %d int_reason 0x%x\n", __FUNCTION__, __LINE__, int_reason);
            // check wrapper done flg
            if (jdec_config.useWrapper)
            {
                uint16_t cnt_wait = 0;
                while (1)
                {
                    if (jdi_read_register(REG_MJPEG_WRAP_CFG_DONE))
                    {
                        DBG_DIRECT("REG_MJPEG_WRAP_CFG_DONE\n");
                        break;
                    }

                    DBG_DIRECT("REG_MJPEG_WRAP_CFG_DONE  Waiting...\n");
                    cnt_wait++;
                    // if (cnt_wait > 50)
                    // {
                    //     CODA_Test_write(REG_MJPEG_WRAP_CFG_REG, (false ? 0x01 : 0x00));
                    // }
                }
            }

            if (int_reason == -1)
            {
                DBG_DIRECT("Error : timeout happened\n");
                JPU_SWReset();
                break;
            }


            if (jdec_config.usePartialMode && (int_reason & 0xf0))
            {
                partBufIdx = ((partPosIdx) % jdec_config.partialBufNum);

                if ((1 << partBufIdx) & ((int_reason & 0xf0) >> 4))
                {
                    DBG_DIRECT("DECODED : PARTIAL BUFFER IDX %d / POS %d / MAX POS %d / INT_REASON=0x%x\n", partBufIdx,
                               partPosIdx + 1, partMaxIdx, int_reason);

                    // if (dispImage)
                    // {
                    //     pDispFrame = FindFrameBuffer(instIdx, frameBuf[partBufIdx].bufY);
                    // }
                    // else
                    {
                        saveIdx = partBufIdx;
                        // if (!SaveYuvPartialImageHelperFormat(NULL, pYuv,
                        //                                      frameBuf[saveIdx].bufY, frameBuf[saveIdx].bufCb, frameBuf[saveIdx].bufCr,
                        //                                      dispWidth, dispHeight, partialHeight, framebufStride, jdec_config.chromaInterleave, framebufFormat,
                        //                                      decOP.frameEndian, partPosIdx, frameIdx,
                        //                                      decOP.packedFormat))
                        // {
                        //     goto ERR_DEC_OPEN;
                        // }

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
                // Do no clear INT_JPU_DONE and INT_JPU_ERROR interrupt. these will be cleared in JPU_DecGetOutputInfo.
                // Pad_Config(P2_1, PAD_SW_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
                DBG_DIRECT("INT_JPU_DONE \n");
                {
                    decode_cnt_clk = CODA_Test_read(MJPEG_APB_CYCLE_CNT_REG);
                }
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
        // DBG_DIRECT("framebufFormat 0x%x %d dispHeight 0x%x %d", &framebufFormat, framebufFormat,
        //            &dispHeight, dispHeight);
        ret = JPU_DecGetOutputInfo(handle, &outputInfo);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("JPU_DecGetOutputInfo failed Error code is 0x%x \n", ret);
            goto ERR_DEC_OPEN;
        }
        if ((outputInfo.decodingSuccess == 0) || (outputInfo.indexFrameDisplay == -1))
        {
            DBG_DIRECT("JPU_DecGetOutputInfo decode fail %d %d\n", outputInfo.decodingSuccess,
                       outputInfo.indexFrameDisplay);
            if (outputInfo.numOfErrMBs)
            {
                int errRstIdx, errPosX, errPosY;
                errRstIdx = (outputInfo.numOfErrMBs & 0x0F000000) >> 24;
                errPosX = (outputInfo.numOfErrMBs & 0x00FFF000) >> 12;
                errPosY = (outputInfo.numOfErrMBs & 0x00000FFF);
                DBG_DIRECT("Error restart Idx : %d, MCU x:%d, y:%d\n", errRstIdx, errPosX, errPosY);
            }
            goto ERR_DEC_OPEN;
            break;
        }

        // indexFrameDisplay points to the frame buffer, among ones registered, which holds
        // the output of the decoder.
        // YUV2RGB here
        int_reason = JPU_GetStatus();
        if (jdec_config.usePartialMode && !(int_reason & 0xF0))
        {
            goto SKIP_BUF_DUMP;
        }
SKIP_BUF_DUMP:
        frameIdx++;
        if (jdec_config.outNum && (frameIdx == jdec_config.outNum))
        {
            break;
        }
        break;
    }


    // Now that we are done with decoding, close the open instance.
    // Unit: M pix/sec x 100  (200 M Hz)
    float speed = initialInfo.picWidth * initialInfo.picHeight * 20000.f / decode_cnt_clk;
    float speed_vendor = 0;
    uint32_t format_n;
    if (initialInfo.sourceFormat == FORMAT_400)
    {
        speed_vendor = 180;
        format_n = 400;
    }
    else if (initialInfo.sourceFormat == FORMAT_444)
    {
        speed_vendor = 100;
        format_n = 444;
    }
    else if (initialInfo.sourceFormat == FORMAT_420)
    {
        speed_vendor = 230;
        format_n = 420;
    }
    else if (initialInfo.sourceFormat == FORMAT_422)
    {
        speed_vendor = 160;
        format_n = 422;
    }
    float rate = speed / speed_vendor;
    // 0: FORMAT_420, 1:FORMAT_422, 3:FORMAT_444, 4:FORMAT_400
    DBG_DIRECT("\nformat %d pix %dx%d=%d clk %d speed %f. r= %f\n", format_n,
               initialInfo.picWidth, initialInfo.picHeight, \
               initialInfo.picWidth * initialInfo.picHeight, \
               decode_cnt_clk, \
               speed, rate);

#ifdef SRC_FLASH
    dec_ret[f_img_index].format = format_n;
    dec_ret[f_img_index].w = initialInfo.picWidth;
    dec_ret[f_img_index].h = initialInfo.picHeight;
    dec_ret[f_img_index].pix = initialInfo.picWidth * initialInfo.picHeight;
    dec_ret[f_img_index].clk = decode_cnt_clk;
    dec_ret[f_img_index].speed = speed;
    dec_ret[f_img_index].rate  = rate;
#endif
//    DBG_DIRECT("\n decoded addr: 0x%x 0x%x, %d\n", );

ERR_DEC_OPEN:
    ret = JPU_DecClose(handle);

ERR_DEC_INIT:
    // DBG_DIRECT("\nEnter ERR_DEC_INIT. \n");

    // yuv/RGB buffer
    FreeFrameBuffer(instIdx);

    // jpg buffer
    if (jdec_config.loc_src == JPG_SRC_HEAP)
    {
        jdi_free_dma_memory(&vbStream);
    }

    //sw_mixer_close(instIdx);
    JPU_DeInit();

    // while (1);
    return suc;
}



JpgRet coda_prepare(DecConfigSetting *jdc)
{
    memset(&jdec_config, 0, sizeof(DecConfigParam));

    // default config: not export to user for now
    {
        // Chroma format type [0](SEPARATED CHROMA) [1](CBCR INTERLEAVED) [2](CRCB INTERLEAVED)
        jdec_config.chromaInterleave = 0;
        jdec_config.StreamEndian = JPU_STREAM_ENDIAN;
        jdec_config.FrameEndian = JPU_FRAME_ENDIAN;

        // partial Mode(0: OFF 1: ON);
        jdec_config.usePartialMode = 0;
        // Num of Frame Buffer[ 2 ~ 4 ] ;
        jdec_config.partialBufNum = 4;

        //// Can NOT do rotation and mirror, when partial mode enable.
        // rotation angle in degrees(0, 90, 180, 270);
        jdec_config.rotAngle = 0;
        // mirror direction(0-no mirror, 1-vertical, 2-horizontal, 3-both);
        jdec_config.mirDir = 0;
        // scalerMode: 0-no scaler, 1-1/2, 2-1/4, 3-1/8
        jdec_config.iHorScaleMode = 0;
        jdec_config.iVerScaleMode = 0;

        // Number of Images that you want to decode(0: decode continue, -1: loop);
        jdec_config.outNum = 1;
    }





    jdec_config.roiEnable = jdc->roiEnable;
    if (jdec_config.roiEnable)
    {
        jdec_config.roiOffsetX = jdc->roiOffsetX;
        jdec_config.roiOffsetY = jdc->roiOffsetY;
        jdec_config.roiWidth  = jdc->roiWidth;
        jdec_config.roiHeight = jdc->roiHeight;
        DBG_DIRECT("roiWidth %d, roiHeight %d \n", jdec_config.roiWidth, jdec_config.roiHeight);
        DBG_DIRECT("roiOffsetX %d, roiOffsetY %d \n", jdec_config.roiOffsetX, jdec_config.roiOffsetY);
    }

    // Packed stream format output [0](PLANAR) [1](YUYV) [2](UYVY) [3](YVYU) [4](VYUY) [5](YUV_444 PACKED)
    jdec_config.packedFormat = jdc->packedFormat;
    // Chroma format type [0](SEPARATED CHROMA) [1](CBCR INTERLEAVED) [2](CRCB INTERLEAVED)
    // jdec_config.chromaInterleave = jdc->chromaInterleave;
    // jdec_config.StreamEndian = jdc->StreamEndian;
    // jdec_config.FrameEndian = jdc->FrameEndian;


    if (!jdec_config.roiEnable && !jdec_config.packedFormat)
    {
        // partial Mode(0: OFF 1: ON);
        // jdec_config.usePartialMode = jdc->usePartialMode;
        if (jdec_config.usePartialMode)
        {
            // Num of Frame Buffer[ 2 ~ 4 ] ;
            // jdec_config.partialBufNum = jdc->partialBufNum;
            if (jdec_config.partialBufNum > 4)
            {
                jdec_config.partialBufNum = 4;
            }
        }
    }

    if (!jdec_config.usePartialMode)
    {
        // rotation angle in degrees(0, 90, 180, 270);
        // jdec_config.rotAngle = jdc->rotAngle;
        if (jdec_config.rotAngle != 0 && jdec_config.rotAngle != 90 && jdec_config.rotAngle != 180 &&
            jdec_config.rotAngle != 270)
        {
            DBG_DIRECT("Invalid rotation angle.\n");
            return JPG_RET_INVALID_PARAM;
        }
        // mirror direction(0-no mirror, 1-vertical, 2-horizontal, 3-both);
        // jdec_config.mirDir = jdc->mirDir;
        if (jdec_config.mirDir != 0 && jdec_config.mirDir != 1 && jdec_config.mirDir != 2 &&
            jdec_config.mirDir != 3)
        {
            DBG_DIRECT("Invalid mirror direction.\n");
            return JPG_RET_INVALID_PARAM;
        }
        if (jdec_config.rotAngle != 0 || jdec_config.mirDir != 0)
        {
            jdec_config.useRot = 1;
        }
    }

    // jdec_config.iHorScaleMode = jdc->iHorScaleMode;
    // jdec_config.iVerScaleMode = jdc->iVerScaleMode;


    // Number of Images that you want to decode(0: decode continue, -1: loop);
    jdec_config.outNum = 1;

    jdec_config.loc_src =  jdc->loc_src;

    // Wrapper enable: 0-OFF, 1-ON
    jdec_config.useWrapper = jdc->useWrapper;
    //  0-JPG_ARGB8888, 1-JPG_RGB888, 2-JPG_RGB565
    jdec_config.rgbType = jdc->rgbType;
    if (jdec_config.rgbType == 0)
    {
        jdec_config.opacity = jdc->opacity;
    }


    if (jdec_config.usePartialMode && jdec_config.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : partial and ROI mode can not be worked\n");
        return JPG_RET_INVALID_PARAM;
    }
    if (jdec_config.packedFormat && jdec_config.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : packed mode and ROI mode can not be worked\n");
        // return JPG_RET_INVALID_PARAM;
    }
    if ((jdec_config.iHorScaleMode || jdec_config.iVerScaleMode) && jdec_config.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : Scaler mode and ROI mode can not be worked\n");
        return JPG_RET_INVALID_PARAM;
    }
    if (jdec_config.useRot && jdec_config.roiEnable)
    {
        DBG_DIRECT("Invalid operation mode : Rotator mode and ROI mode can not be worked\n");
        return JPG_RET_INVALID_PARAM;
    }
    if ((jdec_config.iHorScaleMode || jdec_config.iVerScaleMode) && (jdec_config.rotAngle ||
                                                                     jdec_config.mirDir))
    {
        DBG_DIRECT("Invalid operation mode : Scaler mode and Rotator mode can not be worked\n");
        return JPG_RET_INVALID_PARAM;
    }

    return JPG_RET_SUCCESS;
}




uint32_t CODA_Test(uint8_t cmd)
{
    int ret = 0;

    switch (cmd)
    {
    case 1:
        CODA_Test_read(MJPEG_OP_INFO_REG);
        break;
    case 2:
        CODA_Test_write(MJPEG_OP_INFO_REG, 0x02);
        break;
    case 3: // decoder
        {
            DecConfigSetting  decConfig;
            memset((void *)&decConfig, 0x00, sizeof(DecConfigSetting));

            // export config
            {
                // Packed stream format output [0](PLANAR) [1](YUYV) [2](UYVY) [3](YVYU) [4](VYUY) [5](YUV_444 PACKED)
                decConfig.packedFormat = 1;

                decConfig.roiEnable = 0;
                {
                    decConfig.roiOffsetX = 0;
                    decConfig.roiOffsetY = 16;
                    decConfig.roiWidth  = 96;
                    decConfig.roiHeight = 88;
                }

                // Wrapper enable: 0-OFF, 1-ON
                decConfig.useWrapper = 1;
                //  0-JPG_ARGB8888, 1-JPG_RGB888, 2-JPG_RGB565
                decConfig.rgbType = 2;
                // ARGB: A(8 bit)
                decConfig.opacity = 0xa5;
            }



            // test differnt mem only
            // jpg data location: 0-JPG_SRC_RAM, 1-JPG_SRC_FLASH, 2-JPG_SRC_PSRAM, 3-JPG_SRC_HEAP
            decConfig.loc_src = 3;

            uint8_t *jpg_addr = NULL; // should align to 8
            uint64_t jpg_sz = 0;
            {
#ifdef SRC_FLASH
                DBG_DIRECT("%s 0x%x\n", f_img[f_img_index].name, f_img[f_img_index].addr);
                // bufInfo.size = 20 * 1024 + 1;
                jpg_addr = f_img[f_img_index].addr;
                jpg_sz = f_img[f_img_index].sz;
#else
                jpg_addr = pic;
                jpg_sz = sizeof(pic) / sizeof(pic[0]);
#endif

                // copy img from flash to heap (should align to 8)
                if (decConfig.loc_src == JPG_SRC_HEAP)
                {
                    uint8_t *tmp = (uint8_t *)jpg_malloc_align(jpg_sz, 8);
                    memcpy(tmp, jpg_addr, jpg_sz);
                    jpg_addr = tmp;
                }
                else if (decConfig.loc_src == JPG_SRC_PSRAM)
                {
                    // // cp to psram
                    // DBG_DIRECT("memcpy 0x%x->0x%x, %d\n", bufInfo.buf, (void*)0x22000000, bufInfo.size);
                    // memcpy((void*)0x22000000, bufInfo.buf, bufInfo.size);
                    // bufInfo.buf = (void*)0x22000000;
                    // log_hex((void*)bufInfo.buf, 100);
                }
                else if (decConfig.loc_src == JPG_SRC_RAM)// copy to ram
                {
                    // data ram: 0x20200000   psram: 0x22000000
                    // cp to data ram
                    // DBG_DIRECT("memcpy 0x%x->0x%x, %d\n", bufInfo.buf, 0x20200100, bufInfo.size);
                    // memcpy((void *)0x20200100, bufInfo.buf, bufInfo.size);
                    // log_hex((void *)0x20200100, 100);
                    // bufInfo.buf = (void*)0x20200100;
                }
                else {} // flash

            }



            ret = coda_prepare(&decConfig);
            if (ret != JPG_RET_SUCCESS)
            {
                DBG_DIRECT("\nCODA: Failed to prepare %d\n", ret);
                break;
            }

            DBG_DIRECT("\nCODA: Decode start \n");
            ret = coda_decode(jpg_addr, jpg_sz);
            if (!ret)
            {
                DBG_DIRECT("\nCODA: Failed to Decode %d\n", ret);
            }
            DBG_DIRECT("\nCODA: Decode Done \n");


        }
        break;
    case 4: // encoder
        CODA_encoder_Test(4);
        break;
    }
    return 0;
}
