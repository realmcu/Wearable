#include "trace.h"
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "app_task.h"

#define APP_TASK_PRIORITY               0   /* Task priorities. */
#define APP_TASK_STACK_SIZE             512 * 4

#define MAX_NUMBER_OF_GAP_MESSAGE       0x20
#define MAX_NUMBER_OF_IO_MESSAGE        0x20
#define MAX_NUMBER_OF_EVENT_MESSAGE     (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)


QueueHandle_t evt_queue_handle;
QueueHandle_t io_queue_handle;
TaskHandle_t app_task_handle;
TimerHandle_t xTestTimerHandle = NULL;
uint32_t timers = 0;

bool app_send_msg_to_apptask(uint32_t *p_send)
{
    uint32_t event = EVENT_IO_TO_APP;

    if (xQueueGenericSend(io_queue_handle, (void *)p_send, (TickType_t)0, queueSEND_TO_BACK) != pdPASS)
    {
        DBG_DIRECT("send io msg fail");
        return false;
    }

    if (xQueueGenericSend(evt_queue_handle, (void *)&event, (TickType_t)0, queueSEND_TO_BACK) != pdPASS)
    {
        DBG_DIRECT("send event msg fail");
        return false;
    }
    return true;
}

void test_timer_cb(TimerHandle_t xTimer)
{
    timers ++;
    DBG_DIRECT("Test Timer timeout %u times", timers);

    uint32_t send_val = 100;   //test value
    app_send_msg_to_apptask(&send_val);
}

void sw_timer_init(void)
{
    uint32_t timer_id;
    DBG_DIRECT("sw_timer_init");
    xTestTimerHandle = xTimerCreate("Test Timer", 100, pdTRUE, &timer_id, test_timer_cb);
    DBG_DIRECT("Test Timer handle 0x%x", (uint32_t)xTestTimerHandle);
    if (xTestTimerHandle)
    {
        xTimerStart(xTestTimerHandle, 0);
        DBG_DIRECT("Start auto reload Test Timer: Period 1s");
    }
}

void app_main_task(void *pvParameters)
{
    DBG_DIRECT("app task");
    uint32_t event, uIoReceivedMsg;

    evt_queue_handle = xQueueCreate(MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint32_t));
    io_queue_handle = xQueueCreate(MAX_NUMBER_OF_IO_MESSAGE, sizeof(uint32_t));

    for (;;)
    {
        // Task code goes here.
        if (pdPASS == xQueueReceive(evt_queue_handle, &event, 0xFFFFFFFF))
        {
            DBG_DIRECT("app task: recv event type %u", event);
            if (event == EVENT_IO_TO_APP)
            {
                if (pdPASS == xQueueReceive(io_queue_handle, &uIoReceivedMsg, 0))
                {
                    DBG_DIRECT("app task: recv io msg 0x%x", uIoReceivedMsg);
                }
            }
        }
    }
}

// Function that creates a task.
void task_init(void)
{
    static uint8_t ucParameterToPass;

    // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
    // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
    // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
    // the new task attempts to access it.
    xTaskCreate(app_main_task, "app", APP_TASK_STACK_SIZE, &ucParameterToPass, APP_TASK_PRIORITY,
                &app_task_handle);

}

