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


// yuv
// #include "pic_p400_184_96.txt"
// #include "pic_p420_192_96.txt"
// #include "pic_p422_192_96.txt"
// #include "pic_p444_184_96.txt"
// #include "pic_yuv_184_96.txt"
// #include "pic_yuyv_112_144.txt"
// #include "pic_yuyv_192_96_test.txt"


// rgb
// #include "pic_rgb565_184_96.txt"
// #include "pic_rgb888_184_96.txt"
// #include "pic_rgb888_192_96.txt"
// #include "pic_argb8888_184_96.txt"
// #include "pic_argb8888_192_96.txt"
// #include "pic_argb8888_184_48.txt"
// #include "pic_argb8888_192_48.txt"
#include "encode_test.txt"


// #define AUTO_ENCODER_TEST

#ifdef  AUTO_ENCODER_TEST
uint8_t *pic_yuv = (uint8_t *)0x0220A000;
#endif

uint8_t f_img_index_en = 0;
struct Enc_ret
{
    int format;
    int w;
    int h;
    int pix;
    int clk;
    int speed;
    int rate;
};

struct Enc_ret enc_ret[22];

void coda_enc_auto(void)
{
    for (uint32_t i = 0; i < 22; i++)
    {
        f_img_index_en = i;
        CODA_encoder_Test(4);
    }

}

void coda_enc_res_log(void)
{
    for (uint32_t i = 0; i < 22; i++)
    {
        DBG_DIRECT("\n[%d] format %d pix %dx%d=%d clk %d speed %d r= %d\n", i, \
                   enc_ret[i].format, \
                   enc_ret[i].w, enc_ret[i].h, \
                   enc_ret[i].pix, \
                   enc_ret[i].clk, \
                   enc_ret[i].speed, enc_ret[i].rate);

    }

}




#define NUM_FRAME_BUF           MAX_FRAME
#define ENC_SRC_BUF_NUM         1

int CODA_Encode_Test(EncConfigParam *param)
{

    JpgEncHandle        handle      = { 0 };
    JpgEncOpenParam encOP       = { 0 };
    JpgEncParam     encParam    = { 0 };
    JpgEncInitialInfo   initialInfo = { 0 };
    JpgEncOutputInfo    outputInfo  = { 0 };
    JpgEncParamSet encHeaderParam = {0};
    jpu_buffer_t    vbStream     = {0};
    FRAME_BUF      *pFrame[NUM_FRAME_BUF];
    FrameBuffer     frameBuf[NUM_FRAME_BUF];
    JpgRet          ret = JPG_RET_SUCCESS;
    int             i = 0, srcFrameIdx = 0, frameIdx = 0;
    int             srcFrameFormat = 0;
    int             framebufStride = 0, framebufWidth = 0, framebufHeight = 0, framebufFormat = 0;
    int             suc = 1, key = 0;
    uint8_t            *pYuv    = NULL;
    FILE           *fpYuv   = NULL;
    FILE           *fpSrcYuv = NULL;
    FILE           *fpBitstrm   = NULL;
    int             int_reason = 0;
    int             instIdx;
    int             needFrameBufCount;

    int             partPosIdx = 0;
    int             partBufIdx = 0;
    int             partMaxIdx;

    EncConfigParam encConfig;

    param->usePartialMode = 0;
    encConfig = *param;

    memset(&pFrame[0], 0x00, sizeof(FRAME_BUF *)*NUM_FRAME_BUF);
    memset(&frameBuf[0], 0x00, sizeof(FrameBuffer)*NUM_FRAME_BUF);


    instIdx = encConfig.instNum;


    // bitstream jpg
    ret = JPU_Init();
    if (ret != JPG_RET_SUCCESS &&
        ret != JPG_RET_CALLED_BEFORE)
    {
        DBG_DIRECT("JPU_Init failed Error code is 0x%x \n", ret);
        goto ERR_ENC_INIT;
    }
    // vbStream.size = STREAM_BUF_SIZE;
    vbStream.size = 40 * 1024;  // 35 encode output jpg size(bitstream buffer)
    extern int jdi_allocate_vb_memory_en(jpu_buffer_t *vb);
    if (jdi_allocate_dma_memory(&vbStream) < 0)
        // if (jdi_allocate_vb_memory_en(&vbStream) < 0)
    {
        DBG_DIRECT("fail to allocate bitstream buffer\n");
        goto ERR_ENC_INIT;
    }

    // hfm table config
    ret = getJpgEncOpenParamDefault(&encOP, &encConfig);

    if (ret == 0)
    {
        goto ERR_ENC_INIT;
    }

    // quality set quant table
    JpgEncSetupTables(&encOP, encConfig.encQualityPercentage);

    encOP.streamEndian = encConfig.StreamEndian;
    encOP.frameEndian = encConfig.FrameEndian;
    encOP.chromaInterleave = (CbCrInterLeave)encConfig.chromaInterleave;
    encOP.bitstreamBuffer = vbStream.phys_addr;
    encOP.bitstreamBufferSize = vbStream.size;
    encOP.packedFormat = (PackedOutputFormat)encConfig.packedFormat;
    if (encOP.packedFormat)
    {
        if (encOP.sourceFormat == FORMAT_420 || encOP.sourceFormat == FORMAT_400) // mjpgChromaFormat
        {
            DBG_DIRECT("Invalid operation mode : In case of using packed mode. sourceFormat must be FORMAT_444\n");
            // goto ERR_ENC_INIT;
        }

    }

    srcFrameFormat = encOP.sourceFormat;

    if (encConfig.rotAngle == 90 || encConfig.rotAngle == 270)
    {
        framebufFormat = (srcFrameFormat == FORMAT_422) ? FORMAT_224 : (srcFrameFormat == FORMAT_224) ?
                         FORMAT_422 : srcFrameFormat;
    }
    else
    {
        framebufFormat = srcFrameFormat;
    }

    // srcFrameFormat means that it is original source image format.
    // framebufFormat means that it is converted image format.

    if (encConfig.usePartialMode)
    {
        // Rotator must be disable when partial mode is enable
        encConfig.rotAngle = 0;
        encConfig.mirDir = 0;

        encConfig.partialHeight = (framebufFormat == FORMAT_420 || framebufFormat == FORMAT_224) ? 16 : 8;
        partMaxIdx  = ((encOP.picHeight + 15) & ~15) / encConfig.partialHeight;
        // constraint
        if (partMaxIdx < encConfig.partialBufNum)
        {
            encConfig.partialBufNum = partMaxIdx;
        }

        DBG_DIRECT("Partial Mode Enable\n ");
        DBG_DIRECT(" - Num of line for partial: %d\n ", encConfig.partialHeight);

    }

    DBG_DIRECT("pic_w: %d pic_h: %d \n", encOP.picWidth, encOP.picHeight);
    if (framebufFormat == FORMAT_420 || framebufFormat == FORMAT_422)
    {
        framebufWidth = (((encOP.picWidth + 15) / 16) * 16);
    }
    else
    {
        framebufWidth = (((encOP.picWidth + 7) / 8) * 8);
    }

    if (framebufFormat == FORMAT_420 || framebufFormat == FORMAT_224)
    {
        framebufHeight = (((encOP.picHeight + 15) / 16) * 16);
    }
    else
    {
        framebufHeight = (((encOP.picHeight + 7) / 8) * 8);
    }

    DBG_DIRECT("framebufWidth: %d \n", framebufWidth);
    framebufStride = framebufWidth;

    if (encOP.packedFormat >= PACKED_FORMAT_422_YUYV && encOP.packedFormat <= PACKED_FORMAT_422_VYUY)
    {
        if (encConfig.useWrapper)
        {
            if (encConfig.rgbType == JPG_RGB888)
            {
                framebufStride = framebufStride * 3;
            }
            else if (encConfig.rgbType == JPG_ARGB8888)
            {
                framebufStride = framebufStride * 4;
            }
            else if (encConfig.rgbType == JPG_RGB565)
            {
                framebufStride = framebufStride * 2;
            }
        }
        else
        {
            framebufStride = framebufStride * 3;
        }

        framebufFormat = FORMAT_422;
        if (encConfig.rotAngle == 90 || encConfig.rotAngle == 270)
        {
            framebufFormat = FORMAT_224;
        }
    }
    else if (encOP.packedFormat == PACKED_FORMAT_444)
    {
        if (encConfig.useWrapper)
        {
            if (encConfig.rgbType == JPG_RGB888)
            {
                framebufStride = framebufStride * 3;
            }
            else if (encConfig.rgbType == JPG_ARGB8888)
            {
                framebufStride = framebufStride * 4;
            }
            else if (encConfig.rgbType == JPG_RGB565)
            {
                framebufStride = framebufStride * 3;
            }
        }
        else
        {
            framebufStride = framebufStride * 3;
        }
        framebufFormat = FORMAT_444;
    }
    DBG_DIRECT("framebufStride: %d \n", framebufStride);

    // Open an instance and get initial information for encoding.
    ret = JPU_EncOpen(&handle, &encOP);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_EncOpen failed Error code is 0x%x \n", ret);
        goto ERR_ENC_INIT;
    }

    if (encConfig.useRot)
    {
        JPU_EncGiveCommand(handle, ENABLE_JPG_ROTATION, 0);
        JPU_EncGiveCommand(handle, ENABLE_JPG_MIRRORING, 0);
        JPU_EncGiveCommand(handle, SET_JPG_ROTATION_ANGLE, &encConfig.rotAngle);
        JPU_EncGiveCommand(handle, SET_JPG_MIRROR_DIRECTION, &encConfig.mirDir);
    }

    JPU_EncGiveCommand(handle, SET_JPG_USE_PARTIAL_MODE,  &encConfig.usePartialMode);
    JPU_EncGiveCommand(handle, SET_JPG_PARTIAL_FRAME_NUM, &encConfig.partialBufNum);
    JPU_EncGiveCommand(handle, SET_JPG_PARTIAL_LINE_NUM,  &encConfig.partialHeight);
    JPU_EncGiveCommand(handle, SET_JPG_USE_STUFFING_BYTE_FF, &encConfig.bEnStuffByte);

    JPU_EncSetWrPtr(handle, encOP.bitstreamBuffer, 1);

    //
    ret = JPU_EncGetInitialInfo(handle, &initialInfo);
    if (ret != JPG_RET_SUCCESS)
    {
        DBG_DIRECT("JPU_EncGetInitialInfo failed Error code is 0x%x \n", ret);
        goto ERR_ENC_OPEN;
    }


    DBG_DIRECT("pic_w: %d stride: %d h: %d, srcFrameFormat %d \n", framebufWidth, framebufStride,
               framebufHeight, srcFrameFormat);
    if (encConfig.usePartialMode)
    {
        needFrameBufCount  = encConfig.partialBufNum;
        partMaxIdx = ((encOP.picHeight + 15) & ~15) / encConfig.partialHeight;

        // Initialize frame buffers for encoding and source frame
        if (!AllocateFrameBuffer(instIdx, srcFrameFormat, framebufStride, encConfig.partialHeight,
                                 needFrameBufCount, encOP.packedFormat))
        {
            JpgLeaveLock();
            goto ERR_ENC_OPEN;
        }
    }
    else
    {
        needFrameBufCount = ENC_SRC_BUF_NUM;

        // Initialize frame buffers for encoding and source frame
        uint32_t src_size = 0;
        if (pic_yuv < 0x10000000)
        {
            src_size = sizeof(pic_yuv) / sizeof(pic_yuv[0]);
        }
        else
        {
            src_size = f_img_en[f_img_index_en].sz;
        }
        extern int RegisterFrameBuffer(int instIdx, int format, int width, int height, int frameBufNum,
                                       int pack, void *addr);
        switch (encConfig.loc_src)
        {
        case JPG_SRC_RAM:
            {
                memcpy(JDATA_RAM_ADDR, pic_yuv, src_size);
                if (!RegisterFrameBuffer(instIdx, srcFrameFormat, framebufStride, framebufHeight, needFrameBufCount,
                                         encOP.packedFormat, JDATA_RAM_ADDR))
                {
                    JpgLeaveLock();
                    goto ERR_ENC_OPEN;
                }

                break;
            case JPG_SRC_FLASH:
                {
                    if (!RegisterFrameBuffer(instIdx, srcFrameFormat, framebufStride, framebufHeight, needFrameBufCount,
                                             encOP.packedFormat, pic_yuv))
                    {
                        JpgLeaveLock();
                        goto ERR_ENC_OPEN;
                    }

                }
                break;
            case JPG_SRC_PSRAM:
                {
                    memcpy(JPSRAM_ADDR, pic_yuv, src_size);
                    if (!RegisterFrameBuffer(instIdx, srcFrameFormat, framebufStride, framebufHeight, needFrameBufCount,
                                             encOP.packedFormat, JPSRAM_ADDR))
                    {
                        JpgLeaveLock();
                        goto ERR_ENC_OPEN;
                    }
                }
                break;
            case JPG_SRC_HEAP:
                {
                    if (!AllocateFrameBuffer(instIdx, srcFrameFormat, framebufStride, framebufHeight, needFrameBufCount,
                                             encOP.packedFormat))
                    {
                        JpgLeaveLock();
                        goto ERR_ENC_OPEN;
                    }
                }
            }
            break;

        }
    }
    JpgEnterLock();
    for (i = 0; i < needFrameBufCount; ++i)
    {
        pFrame[i] = GetFrameBuffer(instIdx, i);

        frameBuf[i].stride = pFrame[i]->strideY;
        frameBuf[i].bufY  = pFrame[i]->vbY.phys_addr;
        frameBuf[i].bufCb = pFrame[i]->vbCb.phys_addr;
        frameBuf[i].bufCr = pFrame[i]->vbCr.phys_addr;
        ClearFrameBuffer(instIdx, i);
    }

    JpgLeaveLock();

    DBG_DIRECT("framebuffer stride = %d, width = %d, height = %d\n", framebufStride, framebufWidth,
               framebufHeight);
    DBG_DIRECT("framebuffer format = %d, srcFrameFormat : %d, packed format = %d, Interleave = %d\n",
               framebufFormat, srcFrameFormat, encOP.packedFormat, encOP.chromaInterleave);

    DBG_DIRECT("%d encParam.sourceFrame->stride  %d", __LINE__, frameBuf[srcFrameIdx].stride);
    uint32_t encode_cnt_clk = 0;
    while (1)
    {
        srcFrameIdx = (frameIdx % ENC_SRC_BUF_NUM);

        // Write picture header
        if (encConfig.encHeaderMode == ENC_HEADER_MODE_NORMAL)
        {
            encHeaderParam.size = STREAM_BUF_SIZE;
            encHeaderParam.headerMode =
                ENC_HEADER_MODE_NORMAL;         //Encoder header disable/enable control. Annex:A 1.2.3 item 13
            encHeaderParam.quantMode =
                JPG_TBL_NORMAL; //JPG_TBL_MERGE  // Merge quantization table. Annex:A 1.2.3 item 7
            encHeaderParam.huffMode  =
                JPG_TBL_NORMAL; // JPG_TBL_MERGE //Merge huffman table. Annex:A 1.2.3 item 6
            encHeaderParam.disableAPPMarker = 0;                        //Remove APPn. Annex:A item 11

            if (encHeaderParam.headerMode == ENC_HEADER_MODE_NORMAL)
            {
                //make picture header
                JPU_EncGiveCommand(handle, ENC_JPG_GET_HEADER,
                                   &encHeaderParam); // return exact header size int endHeaderparam.siz;

            }
        }


        // Partial Buffer Control
        if (encConfig.usePartialMode)
        {
            partBufIdx = 0;
            partPosIdx = 0;
            JPU_SWReset();
            // Get a yuv frame
            // if( !LoadYuvPartialImageHelperFormat( fpYuv, pYuv,
            //     frameBuf[partBufIdx].bufY,
            //     frameBuf[partBufIdx].bufCb,
            //     frameBuf[partBufIdx].bufCr,
            //     encOP.picWidth,
            //     encOP.picHeight,
            //     encConfig.partialHeight,
            //     framebufStride, encOP.chromaInterleave, srcFrameFormat, encOP.frameEndian, partPosIdx, frameIdx, encConfig.packedFormat) )
            //     break;   // must break to read last bitstream buffer

        }
        else
        {
            // if( !LoadYuvImageHelperFormat( pic_yuv, pYuv,
            //  frameBuf[srcFrameIdx].bufY,
            //  frameBuf[srcFrameIdx].bufCb,
            //  frameBuf[srcFrameIdx].bufCr,
            //  encOP.picWidth,
            //  encOP.picHeight,
            //  framebufStride, encOP.chromaInterleave, srcFrameFormat, encOP.frameEndian, encConfig.packedFormat) )
            //  break;  // must break to read last bit-stream buffer

            // Load source one picture image to encode to SDRAM frame buffer.
            DBG_DIRECT("%d encParam.sourceFrame->stride  %d", __LINE__, frameBuf[srcFrameIdx].stride);
            if (!LoadYuvImageHelperFormat(NULL, pic_yuv,
                                          frameBuf[srcFrameIdx].bufY,
                                          frameBuf[srcFrameIdx].bufCb,
                                          frameBuf[srcFrameIdx].bufCr,
                                          encOP.picWidth,
                                          encOP.picHeight,
                                          framebufStride, encOP.chromaInterleave, srcFrameFormat, encOP.frameEndian, encConfig.packedFormat, \
                                          encConfig.useWrapper,
                                          encConfig.rgbType))
            {
                break;    // must break to read last bit-stream buffer
            }
            DBG_DIRECT("%d encParam.sourceFrame->stride  %d", __LINE__, frameBuf[srcFrameIdx].stride);
        }


        if (encConfig.usePartialMode)
        {
            // need more frame buffer when partial mode is enable
            encParam.sourceFrame = frameBuf;
        }
        else
        {
            encParam.sourceFrame = &frameBuf[srcFrameIdx];
        }
        DBG_DIRECT("%d encParam.sourceFrame->stride  %d", __LINE__, encParam.sourceFrame->stride);

        {
            {
                // wrapper
                CODA_Test_write(REG_MJPEG_WRAP_CFG_REG, encConfig.useWrapper ? 0x01 : 0x00);
                // CODA_Test_write(MJPEG_WRAPPER_ENABLE_REG, 0x00);
                CODA_Test_write(REG_MJPEG_RGB_PF_REG, encConfig.rgbType);
            }
            CODA_Test_read(MJPEG_APB_CYCLE_CNT_REG);
        }
        // Start encoding a frame.
        ret = JPU_EncStartOneFrame(handle, &encParam);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("JPU_EncStartOneFrame failed Error code is 0x%x \n", ret);
            goto ERR_ENC_OPEN;
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

            // goto loading partial buffer routine when partial buffer is used.
            if (encConfig.usePartialMode && (int_reason >> 4))
            {
                DBG_DIRECT("ENCODE : PARTIAL BUFFER IDX %d / POS %d / MAX POS %d / INT_REASON=0x%x\n", partBufIdx,
                           partPosIdx + 1, partMaxIdx, int_reason);

                if ((1 << partBufIdx) & ((int_reason & 0xf0) >> 4))
                {
                    JPU_ClrStatus((1 << (INT_JPU_PARIAL_BUF0_EMPTY + partBufIdx)));

                    if (int_reason & (1 <<
                                      INT_JPU_DONE)) // Must catch PIC_DONE interrupt before catching EMPTY interrupt
                    {
                        // Do no clear INT_JPU_DONE these will be cleared in JPU_EncGetOutputInfo.
                        break;
                    }
                    partPosIdx ++;
                    partBufIdx = (partPosIdx % encConfig.partialBufNum);
                    // Get a yuv frame
                    LoadYuvPartialImageHelperFormat(fpYuv, pYuv,
                                                    frameBuf[partBufIdx].bufY,
                                                    frameBuf[partBufIdx].bufCb,
                                                    frameBuf[partBufIdx].bufCr,
                                                    encOP.picWidth,
                                                    encOP.picHeight,
                                                    encConfig.partialHeight,
                                                    framebufStride, encOP.chromaInterleave, srcFrameFormat, encOP.frameEndian, partPosIdx, frameIdx,
                                                    encConfig.packedFormat, \
                                                    encConfig.useWrapper,
                                                    encConfig.rgbType);

                    JPU_EncGiveCommand(handle, SET_JPG_ENCODE_NEXT_LINE, NULL);

                    continue;
                }
                else
                {
                    DBG_DIRECT("Invalid partial interrupt : expected reason =0x%x, actual reason=0x%x \n",
                               (1 << partBufIdx), ((int_reason & 0xf0) >> 4));
                    goto ERR_ENC_OPEN;
                }
            }

            if (int_reason & (1 <<
                              INT_JPU_DONE)) // Must catch PIC_DONE interrupt before catching EMPTY interrupt
            {
                // Do no clear INT_JPU_DONE these will be cleared in JPU_EncGetOutputInfo.
                {
                    encode_cnt_clk = CODA_Test_read(MJPEG_APB_CYCLE_CNT_REG);
                }
                break;
            }

            if (int_reason & (1 << INT_JPU_BIT_BUF_EMPTY))
            {
                ret = ReadJpgBsBufHelper(handle, fpBitstrm, encOP.bitstreamBuffer,
                                         encOP.bitstreamBuffer + encOP.bitstreamBufferSize, 0, encOP.streamEndian);
                if (ret != JPG_RET_SUCCESS)
                {
                    DBG_DIRECT("ReadBsRingBufHelper failed Error code is 0x%x \n", ret);
                    goto ERR_ENC_OPEN;
                }
                JPU_ClrStatus((1 << INT_JPU_BIT_BUF_EMPTY));
            }

            // expect this interrupt after stop is enabled.
            if (int_reason & (1 << INT_JPU_BIT_BUF_STOP))
            {
                ret = JPU_EncCompleteStop(handle);
                if (ret != JPG_RET_SUCCESS)
                {
                    DBG_DIRECT("JPU_EncCompleteStop failed Error code is 0x%x \n", ret);
                    goto ERR_ENC_OPEN;
                }
                JPU_ClrStatus((1 << INT_JPU_BIT_BUF_STOP));
                break;
            }

            if (int_reason & (1 << INT_JPU_PARIAL_OVERFLOW))
            {
                JPU_ClrStatus((1 << INT_JPU_PARIAL_OVERFLOW));
            }
        }

        ret = JPU_EncGetOutputInfo(handle, &outputInfo);
        if (ret != JPG_RET_SUCCESS)
        {
            DBG_DIRECT("JPU_EncGetOutputInfo failed Error code is 0x%x \n", ret);
            goto ERR_ENC_OPEN;
        }


        // ReadJpgBsBufHelper(handle, fpBitstrm, encOP.bitstreamBuffer,
        //                    encOP.bitstreamBuffer + encOP.bitstreamBufferSize, encHeaderParam.size, encOP.streamEndian);

        DBG_DIRECT("Enc: %d:%d, S= 0x%x, 0x%x size %d\n", instIdx, frameIdx,
                   outputInfo.bitstreamBuffer, \
                   outputInfo.bitstreamBuffer + outputInfo.bitstreamSize, \
                   outputInfo.bitstreamSize);

        uint32_t cnt_pix = encConfig.picWidth * encConfig.picHeight;
        float speed = cnt_pix * 20000.f / encode_cnt_clk;
        DBG_DIRECT("Encoded Q=%d, cnt_clk=%d, pix= %d speed(x100)= %f\n", encConfig.encQualityPercentage,
                   encode_cnt_clk, cnt_pix, speed);
        DBG_DIRECT("compress RGB565=%d, JPG=%d, r(x100)= %f per\n", cnt_pix * 2, outputInfo.bitstreamSize,
                   outputInfo.bitstreamSize * 100 * 100.f / (cnt_pix * 2));

        float rate ;
        if (encConfig.packedFormat == PACKED_FORMAT_444)
        {
            rate = speed / 100;
            DBG_DIRECT("performance rate %f per\n", rate);
        }
        else
        {
            // 422
            rate = speed / 155;
            DBG_DIRECT("performance rate %f per\n", rate);
        }

#ifdef  AUTO_ENCODER_TEST
        enc_ret[f_img_index_en].format = (f_img_index_en % 2) ? 444 : 422;
        enc_ret[f_img_index_en].w = encConfig.picWidth;
        enc_ret[f_img_index_en].h = encConfig.picHeight;
        enc_ret[f_img_index_en].pix = cnt_pix;
        enc_ret[f_img_index_en].clk = encode_cnt_clk;
        enc_ret[f_img_index_en].speed = speed;
        enc_ret[f_img_index_en].rate  = rate;
#endif


        frameIdx++;
        if (encConfig.outNum && (frameIdx == encConfig.outNum))
        {
            break;
        }

        // DBG_DIRECT("Encoded instIdx=%d, frameIdx=%d\n", instIdx, frameIdx);
    }


ERR_ENC_OPEN:
    // Now that we are done with encoding, close the open instance.
    ret = JPU_EncClose(handle);
//      log_hex((void *)0x202068a0, 200);
    if (ret == JPG_RET_FRAME_NOT_COMPLETE)
    {
        JPU_EncGetOutputInfo(handle, &outputInfo);
        JPU_EncClose(handle);
    }

    DBG_DIRECT("\nEnc End. Tot Frame %d\n", frameIdx);

ERR_ENC_INIT:

    // rgb/yuv
    if (encConfig.loc_src == JPG_SRC_HEAP)
    {
        FreeFrameBuffer(instIdx);
    }
    else
    {
        extern void CleanFrameBuffer(int instIdx);
        CleanFrameBuffer(instIdx);
    }

    // jpg img
    jdi_free_dma_memory(&vbStream);


    if (pYuv)
    {
        jpg_free(pYuv);
    }
    sw_mixer_close(instIdx);

    JPU_DeInit();
    return suc;
}



JpgRet coda_encoder_prepare(DecConfigParam *jdc)
{
    // memset(&jdec_config, 0, sizeof(DecConfigParam));

    // jdec_config.roiEnable = jdc->roiEnable;
    // if (jdec_config.roiEnable)
    // {
    //     jdec_config.roiOffsetX = jdc->roiOffsetX;
    //     jdec_config.roiOffsetY = jdc->roiOffsetY;
    //     jdec_config.roiWidth  = jdc->roiWidth;
    //     jdec_config.roiHeight = jdc->roiHeight;
    // }

    // // Packed stream format output [0](PLANAR) [1](YUYV) [2](UYVY) [3](YVYU) [4](VYUY) [5](YUV_444 PACKED)
    // jdec_config.packedFormat = jdc->packedFormat;
    // // Chroma format type [0](SEPARATED CHROMA) [1](CBCR INTERLEAVED) [2](CRCB INTERLEAVED)
    // jdec_config.chromaInterleave = jdc->chromaInterleave;
    // jdec_config.StreamEndian = jdc->StreamEndian;
    // jdec_config.FrameEndian = jdc->FrameEndian;


    // if (!jdec_config.roiEnable && !jdec_config.packedFormat)
    // {
    //     // partial Mode(0: OFF 1: ON);
    //     jdec_config.usePartialMode = jdc->usePartialMode;
    //     if (jdec_config.usePartialMode)
    //     {
    //         // Num of Frame Buffer[ 2 ~ 4 ] ;
    //         jdec_config.partialBufNum = jdc->partialBufNum;
    //     }
    // }

    // if (!jdec_config.usePartialMode)
    // {
    //     // rotation angle in degrees(0, 90, 180, 270);
    //     jdec_config.rotAngle = jdc->rotAngle;
    //     if (jdec_config.rotAngle != 0 && jdec_config.rotAngle != 90 && jdec_config.rotAngle != 180 &&
    //         jdec_config.rotAngle != 270)
    //     {
    //         DBG_DIRECT("Invalid rotation angle.\n");
    //         return JPG_RET_INVALID_PARAM;
    //     }
    //     // mirror direction(0-no mirror, 1-vertical, 2-horizontal, 3-both);
    //     jdec_config.mirDir = jdc->mirDir;
    //     if (jdec_config.mirDir != 0 && jdec_config.mirDir != 1 && jdec_config.mirDir != 2 &&
    //         jdec_config.mirDir != 3)
    //     {
    //         DBG_DIRECT("Invalid mirror direction.\n");
    //         return JPG_RET_INVALID_PARAM;
    //     }
    //     if (jdec_config.rotAngle != 0 || jdec_config.mirDir != 0)
    //     {
    //         jdec_config.useRot = 1;
    //     }
    // }

    // jdec_config.iHorScaleMode = jdc->iHorScaleMode;
    // jdec_config.iVerScaleMode = jdc->iVerScaleMode;


    // // Number of Images that you want to decode(0: decode continue, -1: loop);
    // jdec_config.outNum = 1;

    // jdec_config.loc_src =  jdc->loc_src;

    // // Wrapper enable: 0-OFF, 1-ON
    // jdec_config.useWrapper = jdc->useWrapper;
    // //  0-JPG_ARGB8888, 1-JPG_RGB888, 2-JPG_RGB565
    // jdec_config.rgbType = jdc->rgbType;


    // if (jdec_config.usePartialMode && jdec_config.roiEnable)
    // {
    //     DBG_DIRECT("Invalid operation mode : partial and ROI mode can not be worked\n");
    //     return JPG_RET_INVALID_PARAM;
    // }
    // if (jdec_config.packedFormat && jdec_config.roiEnable)
    // {
    //     DBG_DIRECT("Invalid operation mode : packed mode and ROI mode can not be worked\n");
    //     return JPG_RET_INVALID_PARAM;
    // }
    // if ((jdec_config.iHorScaleMode || jdec_config.iVerScaleMode) && jdec_config.roiEnable)
    // {
    //     DBG_DIRECT("Invalid operation mode : Scaler mode and ROI mode can not be worked\n");
    //     return JPG_RET_INVALID_PARAM;
    // }
    // if (jdec_config.useRot && jdec_config.roiEnable)
    // {
    //     DBG_DIRECT("Invalid operation mode : Rotator mode and ROI mode can not be worked\n");
    //     return JPG_RET_INVALID_PARAM;
    // }
    // if ((jdec_config.iHorScaleMode || jdec_config.iVerScaleMode) && (jdec_config.rotAngle||jdec_config.mirDir))
    // {
    //     DBG_DIRECT("Invalid operation mode : Scaler mode and Rotator mode can not be worked\n");
    //     return JPG_RET_INVALID_PARAM;
    // }

    return JPG_RET_SUCCESS;
}




uint32_t CODA_encoder_Test(uint8_t cmd)
{
    int ret = 0;

    switch (cmd)
    {
    case 1:

        break;
    case 2:

        break;
    case 4: // encoder
        {
            EncConfigParam  encConfig;
            memset(&encConfig, 0x00, sizeof(EncConfigParam));

            encConfig.StreamEndian = JPU_STREAM_ENDIAN;
            encConfig.FrameEndian = JPU_FRAME_ENDIAN;
            // packed format and interleace is mutually exclusive
            encConfig.chromaInterleave = JPU_CHROMA_INTERLEAVE;

            encConfig.bEnStuffByte = JPU_STUFFING_BYTE_FF;
            // Enter ENC CFG file name [0 if manual]: ";
            // encConfig.cfgFileName[0] = 0;

            // // picture width
            // encConfig.picWidth = 112;
            // // picture height
            // encConfig.picHeight = 144;

            // picture width
            // encConfig.picWidth = 192;
            // encConfig.picWidth = 184;
            // picture height
            // encConfig.picHeight = 96;
            // encConfig.picHeight = 48;


            // test
            // encConfig.picWidth = 112;  //
            // encConfig.picHeight = 144;  //

            // encConfig.picWidth = 144;  //
            // encConfig.picWidth = 136;  //
            // encConfig.picWidth = 96;  //
            // encConfig.picWidth = 168;  //
            encConfig.picWidth = 176;  //
            // encConfig.picWidth = 152;  //
            // encConfig.picWidth = 160;  //

            // encConfig.picHeight = 136;  //
            // encConfig.picHeight = 112;  //
            // encConfig.picHeight = 104;  //
            // encConfig.picHeight = 168;  //
            encConfig.picHeight = 96;  //
            // encConfig.picHeight = 176;  //
            // encConfig.picHeight = 80;  //

            // bitstream file name
            // encConfig.bitstreamFileName[0] = 0;

            // MJPG
            // Jpeg Encoder Quality Percentage [0] not use / [1%% ~ 100%%]
            encConfig.encQualityPercentage = 100; // 100 80 50
            if (encConfig.encQualityPercentage == 0)
            {
                // Huffman Table file name(0 : use pre-defined table in Ref-S/W)
                // encConfig.huffFileName[0] = 0;
                // Q Matrix Table file name(0 : use pre-defined table in Ref-S/W)
                // encConfig.qMatFileName[0] = 0;
            }

            // support: packed  422 -> 420, 422, 444;  444 -> 422
            // YUV format
            // packed 422/444 only
            // Frame Format [0](PLANAR) [1](YUYV) [2](UYVY) [3](YVYU) [4](VYUY) [5](YUV_444 PACKED)
            encConfig.packedFormat = 1;
            // encConfig.packedFormat = 5;
            // encConfig.packedFormat = 0;

            // JPG format
            // Source Chroma Format 0 (4:2:0) / 1 (4:2:2) / 2 (2:2:4 4:2:2 rotated) / 3 (4:4:4) / 4 (4:0:0)
            // encConfig.mjpgChromaFormat = 1;
            // encConfig.mjpgChromaFormat = 3;
            encConfig.mjpgChromaFormat = 0;

            // Wrapper enable: 0-OFF, 1-ON
            encConfig.useWrapper = 1;
            //  0-JPG_ARGB8888, 1-JPG_RGB888, 2-JPG_RGB565
            encConfig.rgbType = 2;
            // rgb/yuv data location set: 0-JPG_SRC_RAM, 1-JPG_SRC_FLASH, 2-JPG_SRC_PSRAM, 3-JPG_SRC_HEAP
            encConfig.loc_src = 3;
            // // rgb/yuv data type: 0-array, 1- flash bin
            // encConfig.src_type = 1;

#ifdef  AUTO_ENCODER_TEST
            {
                // f_img_index_en = 1;
                DBG_DIRECT("%d %s 0x%x %d B", f_img_index_en, f_img_en[f_img_index_en].name,
                           f_img_en[f_img_index_en].addr,  f_img_en[f_img_index_en].sz);
                pic_yuv = f_img_en[f_img_index_en].addr;
                encConfig.picWidth = f_img_en[f_img_index_en].w;
                encConfig.picHeight = f_img_en[f_img_index_en].h;

                if (f_img_index_en % 2) // 444
                {
                    encConfig.packedFormat = 5;
                    encConfig.mjpgChromaFormat = 3;
                }
                else
                {
                    encConfig.packedFormat = 1;
                    encConfig.mjpgChromaFormat = 1;
                }


            }

#endif


            // Number of Images that you want to encode(0: encode continue);
            encConfig.outNum = 1;

            // partial Mode(0: OFF 1: ON)
            encConfig.usePartialMode = 0;

            if (encConfig.usePartialMode)
            {
                // Num of Frame Buffer[ 2 ~ 4 ];
                encConfig.partialBufNum = 4;
            }


            if (encConfig.usePartialMode == 0)
            {
                // Rotation parameter
                // rotation angle in degrees(0, 90, 180, 270)
                encConfig.rotAngle = 0;
                if (encConfig.rotAngle != 0 && encConfig.rotAngle != 90 && encConfig.rotAngle != 180 &&
                    encConfig.rotAngle != 270)
                {
                    DBG_DIRECT("Invalid rotation angle.\n");
                    break;
                }
                // Flip parameter
                // mirror direction(0-no mirror, 1-vertical, 2-horizontal, 3-both)
                encConfig.mirDir = 0;
                if (encConfig.mirDir != 0 && encConfig.mirDir != 1 && encConfig.mirDir != 2 &&
                    encConfig.mirDir != 3)
                {
                    DBG_DIRECT("Invalid mirror direction.\n");
                    break;
                }
            }

            if (encConfig.rotAngle != 0 || encConfig.mirDir != 0)
            {
                encConfig.useRot = 1;
            }


RUN_LAST_ENC_CMD:

            // DumpConfig("enc_cmd", &encConfig, sizeof(encConfig), DUMP_WRITE);
            ret = CODA_Encode_Test(&encConfig);
            if (!ret)
            {
                DBG_DIRECT("\nFailed to EncodeTest\n");
            }

        }
        break;
    }
    return 0;
}
