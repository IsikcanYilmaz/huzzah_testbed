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

uint16_t count = 0;

void Tx_Task(void *params)
{
  printf("TX Task created!!!\n");
  while(1) 
  {
    /*uint16_t *rand_num_ptr = malloc(sizeof(uint16_t));*/
    uint16_t rand_num = (uint16_t) rand();
    /**rand_num_ptr = (uint16_t) rand();*/
    /*printf("TX Task generated random number 0x%x = %d\n", (unsigned int) rand_num_ptr, *rand_num_ptr);*/
    printf("TX Task generated random number 0x%x = %d\n", (unsigned int) &rand_num, rand_num);
    BaseType_t xstatus = xQueueSendToBack(RxTaskQueue, &rand_num, 0);
    printf("TX Task xstatus == pdPASS %d\n", xstatus == pdPASS);
    count++;
    if (count == 4){
      count = 0;
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}
