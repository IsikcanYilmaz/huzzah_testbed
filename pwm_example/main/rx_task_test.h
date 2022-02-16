
#include "queue.h"

#define RX_QUEUE_LEN 16

extern QueueHandle_t RxTaskQueue;

bool Rx_TaskInit(void);
void Rx_Task(void *params);
