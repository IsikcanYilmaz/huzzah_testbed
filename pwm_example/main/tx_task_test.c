#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "queue.h"
#include "rx_task_test.h"

#include <stdlib.h>
#include <time.h>

bool Tx_TaskInit(void)
{
  /*srand(time(0));*/
  return true;
}

void Tx_Task(void *params)
{
  printf("TX Task created!!!\n");
  while(1) 
  {
    uint32_t *rand_num_ptr = (uint32_t) pvPortMalloc(sizeof(uint32_t));
    *rand_num_ptr = 1;
    printf("TX Task generated random number 0x%x\n", (unsigned int) rand_num_ptr);
    vPortFree(rand_num_ptr);
    BaseType_t xstatus = xQueueSendToBack(RxTaskQueue, rand_num_ptr, 0);
    /*uint32_t heap_size32 = heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT);*/
    /*uint32_t heap_size8 = heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT);*/
    /*printf("TX Task heap size 32 %d 8 %d\n", heap_size32, heap_size8);*/
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
