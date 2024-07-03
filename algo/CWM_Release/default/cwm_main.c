#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cwm_lib.h"
#include "trace.h"

int CWM_OS_dbgOutput(const char *format)
{
    DBG_DIRECT("%s", format);
    return 0;
}

int CWM_OS_dbgPrintf(const char *format, ...)
{
    va_list argList;
    int size = 0;
    static char tBuffer[256];

    va_start(argList, format);
    size = vsnprintf(tBuffer, sizeof(tBuffer), format, argList);
    va_end(argList);

    //strcat(tBuffer, "\n");
    CWM_OS_dbgOutput(tBuffer);
    return size;
}

uint64_t CWM_OS_GetTimeNs(void)
{
    uint64_t timeNow_us;

    timeNow_us = 0;

    return timeNow_us * 1000;
}

void CWM_AP_SensorListen(pSensorEVT_t sensorEVT)
{
    switch (sensorEVT->sensorType)
    {
    case 0:
        CWM_OS_dbgPrintf("acc: x=%.3f, y=%.3f, z=%.3f\n",
                         sensorEVT->fData[0],
                         sensorEVT->fData[1],
                         sensorEVT->fData[2]
                        );
        break;

    case 1:
        CWM_OS_dbgPrintf("gyro: x=%.3f, y=%.3f, z=%.3f\n",
                         sensorEVT->fData[0],
                         sensorEVT->fData[1],
                         sensorEVT->fData[2]
                        );
        break;

    default:
        CWM_OS_dbgPrintf("IDX[%d]: %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", sensorEVT->sensorType,
                         sensorEVT->fData[0],
                         sensorEVT->fData[1],
                         sensorEVT->fData[2],
                         sensorEVT->fData[3],
                         sensorEVT->fData[4],
                         sensorEVT->fData[5],
                         sensorEVT->fData[6],
                         sensorEVT->fData[7]
                        );
    }
}

int cwm_main(int argc, char *argv[])
{
    int ret, loopIndex;
    SettingControl_t scl;

    //get lib version information
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    CWM_SettingControl(SCL_GET_LIB_INFO, &scl);
    CWM_OS_dbgPrintf("version:%d.%d.%d.%d product:%d model:%d\n", scl.iData[1], scl.iData[2],
                     scl.iData[3], scl.iData[4], scl.iData[5], scl.iData[6]);

    os_api api = {0};
    api.dbgOutput    = CWM_OS_dbgOutput;
    //api.GetTimeNs    = CWM_OS_GetTimeNs;

    CWM_LibPreInit(&api);

    //set mcu_chip information, must call this before CWM_LibPostInit()
    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 2; // 0: mcu_auto_detect 2: skip_mcu_auto_detect
    CWM_SettingControl(SCL_CHIP_VENDOR_CONFIG, &scl);

    CWM_LibPostInit(CWM_AP_SensorListen);

    //get chip information
    char chipInfo[64];

    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[1] = 1;
    scl.iData[2] = (int)chipInfo;
    scl.iData[3] = sizeof(chipInfo);
    scl.iData[4] = 0;
    scl.iData[5] = 0;
    scl.iData[6] = 0;
    CWM_SettingControl(SCL_GET_CHIP_INFO, &scl);
    CWM_OS_dbgPrintf("have_security = %d.%d ret_buff_size = %d  chipInfo = %s\n", scl.iData[5],
                     scl.iData[6], scl.iData[4], chipInfo);
    CWM_OS_dbgPrintf("chip_settings = %d, %d, %d\n", scl.iData[9], scl.iData[10], scl.iData[11]);
    CWM_OS_dbgPrintf("secFileId = %08X\n", scl.iData[7]);

    memset(&scl, 0, sizeof(scl));
    scl.iData[0] = 1;
    scl.iData[3] = 3;
    scl.iData[4] = 15 + 64;
    //scl.iData[6] = 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384;
    //CWM_SettingControl(SCL_LOG, &scl);

    CWM_Sensor_Enable(0); //IDX_ACCEL
    CWM_Sensor_Enable(1); //IDX_GYRO

    /*
        CWM_Sensor_Disable(0);
        CWM_Sensor_Disable(1);
    */

    for (loopIndex = 0; loopIndex < 101; loopIndex ++)
    {
        int dt_us;

        dt_us = 20000;

        CustomSensorData csd;

        memset(&csd, 0, sizeof(CustomSensorData));
        csd.sensorType = CUSTOM_ACC;
        csd.fData[0] = 0;
        csd.fData[1] = 0;
        csd.fData[2] = 9.80665;
        CWM_CustomSensorInput(&csd);

        memset(&csd, 0, sizeof(CustomSensorData));
        csd.sensorType = CUSTOM_GYRO;
        csd.fData[0] = 0;
        csd.fData[1] = 0;
        csd.fData[2] = -3.1415926;
        CWM_CustomSensorInput(&csd);

        ret = CWM_process2(dt_us);
        if (ret < 0)
        {
            CWM_OS_dbgPrintf("FATAL ERROR! ret=%d\n", ret);
        }
    }

    return 0;
}

