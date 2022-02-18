#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rx_task_test.h"

QueueHandle_t RxTaskQueue = NULL;

const TickType_t xTicksToWait = pdMS_TO_TICKS(1000);

bool Rx_TaskInit(void)
{
  bool ret = true;
  RxTaskQueue = xQueueCreate(RX_QUEUE_LEN, sizeof(uint16_t));
  if (RxTaskQueue == NULL)
  {
    printf("RX Task error while creating queue!\n");
    ret = false;
  }
  return ret;
}

void Rx_Task(void *params)
{
  printf("RX Task created!!!\n");
  while(1) 
  {
    uint16_t rx_rand_num = NULL;
    BaseType_t xstatus = xQueueReceive(RxTaskQueue, &rx_rand_num, xTicksToWait);
    if (xstatus == pdPASS)
    {
      printf("RX Task received rand num %d\n", rx_rand_num);
    }
    else
    {
      printf("RX Task xstatus not pdPass\n");
    }
  }
}
