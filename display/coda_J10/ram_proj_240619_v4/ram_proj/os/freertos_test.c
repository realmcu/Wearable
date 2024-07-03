#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "trace.h"

static xQueueHandle xQueue;

static void vSenderTask(void *pvParameters)
{
    long lVauleToSend;
    portBASE_TYPE xStatus;

    lVauleToSend = (long) pvParameters;

    for (;;)
    {
        xStatus = xQueueSendToBack(xQueue, &lVauleToSend, 0);
        if (xStatus != pdPASS)
        {
            //error
            configASSERT(0);
        }
        else
        {
            DBG_DIRECT("send %d", lVauleToSend);
        }

        taskYIELD();
    }
}

static void vReceiverTask(void *pvParameters)
{
    long lReceivedValue;
    portBASE_TYPE xStatus;
    const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

    for (;;)
    {
        xStatus = xQueueReceive(xQueue, &lReceivedValue, xTicksToWait);
        if (xStatus == pdPASS)
        {
            //print received data
            DBG_DIRECT("receive data: %d", lReceivedValue);
        }
        else
        {
            //error
            configASSERT(0);
        }

    }

}

void test_freertos(void)
{
    //Samples of print log
#if 0
    {
        uint8_t event = 0x15;
        uint8_t result = 1;
        char pcName[] = "lower stack";
        uint8_t keyLength = 10;
        uint8_t pkey[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
        char pBDAddress[6] = {0xe0, 0x00, 0x00, 0x01, 0x02, 0x03};

        DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_FORMAT, MODULE_APP, LEVEL_INFO, "receive event 0x%x, data: %d",
                   2, event, result);
        DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_BINARY, MODULE_APP, LEVEL_INFO, "The Key is: ", 2, keyLength,
                   pkey);
        DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_DOWN_MESSAGE, MODULE_APP, LEVEL_INFO, "DownStream is: ", 1,
                   "Hello");
        DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_UP_MESSAGE, MODULE_APP, LEVEL_INFO, "UpStream is: ", 1,
                   "World");

        DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_FORMAT, MODULE_UPPERSTACK, LEVEL_INFO,
                   "BTMAN: ACTIVATION complete, bd_addr %s status 0x%x, activeApp(%p), activeLink(%p)",
                   4,
                   TRACE_BDADDR(pBDAddress),
                   800,
                   0x20001000,
                   0x20001200
                  );

        DBG_BUFFER(TYPE_BUMBLEBEE3, SUBTYPE_FORMAT, MODULE_UPPERSTACK, LEVEL_INFO,
                   "btsm: LTK:%s keySize:%d",
                   2,
                   TRACE_STRING("ABCDEF"),
                   6
                  );

        DBG_SNOOP(TYPE_BUMBLEBEE3, SUBTYPE_DOWN_SNOOP, MODULE_APP, LEVEL_INFO, 10, (uint8_t *)"1234567890");
    }
#endif

    xQueue = xQueueCreate(5, sizeof(long));
    if (xQueue != NULL)
    {
        xTaskCreate(vSenderTask, "Sender1", 1000, (void *)100, 1, NULL);
        xTaskCreate(vSenderTask, "Sender2", 1000, (void *)200, 1, NULL);

        xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, NULL);
    }
}
