#ifndef ___JPEG_CODA__H__
#define ___JPEG_CODA__H__


#include "trace.h"
#include "rtl876x.h"
#include "jpuapi.h"








uint32_t coda_init(void);
uint32_t coda_deinit(void);

JpgRet coda_prepare(DecConfigSetting *jdc);
// JpgRet coda_decode(void);
uint32_t CODA_Test_read(uint32_t addr);
uint32_t CODA_Test_write(uint32_t addr, uint32_t val);

uint32_t CODA_Test(uint8_t cmd);
uint32_t CODA_encoder_Test(uint8_t cmd);

#endif