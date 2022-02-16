/* pwm example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_task.h"

#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "driver/pwm.h"
#include "driver/uart.h"

#include "uart_task.h"
#include "pwm_task.h"
#include "cmd_shell_task.h"
#include "rx_task_test.h"
#include "tx_task_test.h"

static const char *TAG = "pwm_example";

void app_main()
{
  Uart_TaskInit();
  Pwm_TaskInit();
  CmdShell_TaskInit();
  Rx_TaskInit();
  Tx_TaskInit();
  
  assert(xTaskCreate(Uart_Task, "uiT", ESP_TASK_MAIN_STACK, NULL, configMAX_PRIORITIES, NULL) == pdPASS);
  assert(xTaskCreate(CmdShell_Task, "uiT", ESP_TASK_MAIN_STACK, NULL, configMAX_PRIORITIES, NULL) == pdPASS);
  assert(xTaskCreate(Pwm_Task, "uiT", ESP_TASK_MAIN_STACK, NULL, configMAX_PRIORITIES, NULL) == pdPASS);
  assert(xTaskCreate(Rx_Task, "uiT", ESP_TASK_MAIN_STACK, NULL, configMAX_PRIORITIES, NULL) == pdPASS);
  assert(xTaskCreate(Tx_Task, "uiT", ESP_TASK_MAIN_STACK, NULL, configMAX_PRIORITIES, NULL) == pdPASS);

  while(1)
  {
    vTaskDelay(1000);
  }
}

