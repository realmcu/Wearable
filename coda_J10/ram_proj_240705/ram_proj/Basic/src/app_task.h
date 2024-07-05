#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"


#define MAX_NUMBER_OF_GAP_MESSAGE       0x20
#define MAX_NUMBER_OF_IO_MESSAGE        0x20
#define MAX_NUMBER_OF_EVENT_MESSAGE     (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE)


extern QueueHandle_t evt_queue_handle;
extern QueueHandle_t io_queue_handle;
extern TaskHandle_t app_task_handle;

/**  @brief Event type definitions.*/
typedef enum
{
    EVENT_GAP_MSG = 0x01,  /**< message from gap layer for stack */
    EVENT_IO_TO_APP = 0x02, /**< message from IO to user application */
} T_EVENT_TYPE;

bool app_send_msg_to_apptask(uint32_t *p_send);

void task_init(void);

void sw_timer_init(void);


