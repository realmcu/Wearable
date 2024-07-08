//------------------------------------------------------------------------------
// File: JpuApi.h
//
// Copyright (c) 2006~2011, Chips & Media.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef JPUAPI_H_INCLUDED
#define JPUAPI_H_INCLUDED

#include "jpuconfig.h"
#include "jdi.h"

//------------------------------------------------------------------------------
// common struct and definition
//------------------------------------------------------------------------------


typedef enum
{
    ENABLE_JPG_ROTATION,
    DISABLE_JPG_ROTATION,
    ENABLE_JPG_MIRRORING,
    DISABLE_JPG_MIRRORING,
    SET_JPG_MIRROR_DIRECTION,
    SET_JPG_ROTATION_ANGLE,
    SET_JPG_ROTATOR_OUTPUT,
    SET_JPG_ROTATOR_STRIDE,
    SET_JPG_SCALE_HOR,
    SET_JPG_SCALE_VER,
    SET_JPG_USE_PARTIAL_MODE,
    SET_JPG_PARTIAL_FRAME_NUM,
    SET_JPG_PARTIAL_LINE_NUM,
    SET_JPG_ENCODE_NEXT_LINE,
    SET_JPG_USE_STUFFING_BYTE_FF,
    ENC_JPG_GET_HEADER,
    ENABLE_LOGGING,
    DISABLE_LOGGING,
    SET_JPG_DISP_WIDTH,
    JPG_CMD_END
} JpgCommand;



typedef enum
{
    JPG_RET_SUCCESS,
    JPG_RET_FAILURE,
    JPG_RET_BIT_EMPTY,
    JPG_RET_EOS,
    JPG_RET_INVALID_HANDLE,
    JPG_RET_INVALID_PARAM,
    JPG_RET_INVALID_COMMAND,
    JPG_RET_ROTATOR_OUTPUT_NOT_SET,
    JPG_RET_ROTATOR_STRIDE_NOT_SET,
    JPG_RET_FRAME_NOT_COMPLETE,
    JPG_RET_INVALID_FRAME_BUFFER,
    JPG_RET_INSUFFICIENT_FRAME_BUFFERS,
    JPG_RET_INVALID_STRIDE,
    JPG_RET_WRONG_CALL_SEQUENCE,
    JPG_RET_CALLED_BEFORE,
    JPG_RET_NOT_INITIALIZED,
    JPG_RET_NOT_SUPPORTED_FORMAT
} JpgRet;

typedef enum
{
    MIRDIR_NONE,
    MIRDIR_VER,
    MIRDIR_HOR,
    MIRDIR_HOR_VER
} JpgMirrorDirection;

typedef enum
{
    FORMAT_420 = 0,
    FORMAT_422 = 1,
    FORMAT_224 = 2,
    FORMAT_444 = 3,
    FORMAT_400 = 4
} FrameFormat;


typedef enum
{
    CBCR_ORDER_NORMAL,
    CBCR_ORDER_REVERSED
} CbCrOrder;


typedef enum
{
    CBCR_SEPARATED = 0,
    CBCR_INTERLEAVE
    ,
    CRCB_INTERLEAVE
} CbCrInterLeave;

typedef enum
{
    PACKED_FORMAT_NONE,
    PACKED_FORMAT_422_YUYV,
    PACKED_FORMAT_422_UYVY,
    PACKED_FORMAT_422_YVYU,
    PACKED_FORMAT_422_VYUY,
    PACKED_FORMAT_444
} PackedOutputFormat;


typedef enum
{
    INT_JPU_DONE = 0,
    INT_JPU_ERROR = 1,
    INT_JPU_BIT_BUF_EMPTY = 2,
    INT_JPU_BIT_BUF_FULL = 2,
    INT_JPU_PARIAL_OVERFLOW = 3,
    INT_JPU_PARIAL_BUF0_EMPTY = 4,
    INT_JPU_PARIAL_BUF1_EMPTY,
    INT_JPU_PARIAL_BUF2_EMPTY,
    INT_JPU_PARIAL_BUF3_EMPTY
    ,
    INT_JPU_BIT_BUF_STOP
} InterruptJpu;

typedef enum
{
    JPG_TBL_NORMAL,
    JPG_TBL_MERGE
} JpgTableMode;

typedef enum
{
    ENC_HEADER_MODE_NORMAL,
    ENC_HEADER_MODE_SOS_ONLY
} JpgEncHeaderMode;

typedef struct
{
    PhysicalAddress bufY;
    PhysicalAddress bufCb;
    PhysicalAddress bufCr;
    int stride;
} FrameBuffer;


struct JpgInst;

//------------------------------------------------------------------------------
// decode struct and definition
//------------------------------------------------------------------------------

typedef struct JpgInst JpgDecInst;
typedef JpgDecInst *JpgDecHandle;

typedef struct
{
    PhysicalAddress bitstreamBuffer;
    int bitstreamBufferSize;
    BYTE *pBitStream;
    int streamEndian;
    int frameEndian;
    CbCrInterLeave chromaInterleave;
    PackedOutputFormat packedFormat;
    int roiEnable;
    int roiOffsetX;
    int roiOffsetY;
    int roiWidth;
    int roiHeight;

} JpgDecOpenParam;

typedef struct
{
    int picWidth;
    int picHeight;
    int minFrameBufferCount;
    int sourceFormat;
    int ecsPtr;
    int roiFrameWidth;
    int roiFrameHeight;
    int roiFrameOffsetX;
    int roiFrameOffsetY;
    int roiMCUSize;
    int colorComponents;
} JpgDecInitialInfo;


typedef struct
{
    int scaleDownRatioWidth;
    int scaleDownRatioHeight;
} JpgDecParam;


typedef struct
{
    int indexFrameDisplay;
    int numOfErrMBs;
    int decodingSuccess;
    int decPicHeight;
    int decPicWidth;
    int consumedByte;
    int bytePosFrameStart;
    int ecsPtr;
    int frameCycle;
} JpgDecOutputInfo;

typedef struct
{
    unsigned char *buf;
    int size;
    int point;
    int count;
    int fillendbs;
} BufInfo;

typedef struct
{
    int outNum;
    int rotAngle;
    int mirDir;
    int useRot;

    int checkeos;
    int instNum;
    int StreamEndian;
    int FrameEndian;
    int chromaInterleave;
    int iHorScaleMode;
    int iVerScaleMode;

    //ROI
    int roiEnable;
    int roiWidth;
    int roiHeight;
    int roiOffsetX;
    int roiOffsetY;
    int roiWidthInMcu;
    int roiHeightInMcu;
    int roiOffsetXInMcu;
    int roiOffsetYInMcu;

    //packed
    int packedFormat;
    int picSizeProbing;
    int picWidth;
    int picHeight;
    int picFormat;

    int usePartialMode;
    int partialBufNum;

    int partialHeight;
    int filePlay;

} DecConfigParam;


#ifdef __cplusplus
extern "C" {
#endif


int         JPU_IsBusy();
uint32_t      JPU_GetStatus();
void        JPU_ClrStatus(uint32_t val);
uint32_t      JPU_IsInit(void);
uint32_t      JPU_WaitInterrupt(int timeout);

JpgRet      JPU_Init();
void        JPU_DeInit();
int         JPU_GetOpenInstanceNum();
JpgRet     JPU_GetVersionInfo(uint32_t *versionInfo);

// function for decode
JpgRet JPU_DecOpen(JpgDecHandle *, JpgDecOpenParam *);
JpgRet JPU_DecClose(JpgDecHandle);
JpgRet JPU_DecGetInitialInfo(
    JpgDecHandle handle,
    JpgDecInitialInfo *info);

JpgRet JPU_DecSetRdPtr(
    JpgDecHandle handle,
    PhysicalAddress addr,
    int updateWrPtr);           // it's useful for ring buffer mode.

JpgRet JPU_DecSetRdPtrEx(
    JpgDecHandle handle,
    PhysicalAddress addr,
    int updateWrPtr);           // to set new decoding start ptr (useful for line buffer mode(chunk base decoding). (offset is always 0). it's useful for the chunk base decoding

JpgRet JPU_DecRegisterFrameBuffer(
    JpgDecHandle handle,
    FrameBuffer *bufArray,
    int num,
    int stride);
JpgRet JPU_DecGetBitstreamBuffer(
    JpgDecHandle handle,
    PhysicalAddress *prdPrt,
    PhysicalAddress *pwrPtr,
    int *size);
JpgRet JPU_DecUpdateBitstreamBuffer(
    JpgDecHandle handle,
    int size);
JpgRet JPU_HWReset();
JpgRet JPU_SWReset();
JpgRet JPU_DecStartOneFrame(
    JpgDecHandle handle,
    JpgDecParam *param);
JpgRet JPU_DecGetOutputInfo(
    JpgDecHandle handle,
    JpgDecOutputInfo *info);
JpgRet JPU_DecIssueStop(
    JpgDecHandle handle);
JpgRet JPU_DecCompleteStop(
    JpgDecHandle handle);
JpgRet JPU_DecGiveCommand(
    JpgDecHandle handle,
    JpgCommand cmd,
    void *parameter);

#ifdef __cplusplus
}
#endif

#endif
