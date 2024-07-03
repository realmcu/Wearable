#ifndef ___JPEG_DECODER__H__
#define ___JPEG_DECODER__H__


#include "trace.h"
#include "rtl876x.h"
#include "jpuapi.h"








uint32_t coda_init(void);
uint32_t coda_deinit(void);

JpgRet coda_prepare(DecConfigParam *jdc);
JpgRet coda_decode(void);

uint32_t CODA_Test(uint8_t cmd);


#endif