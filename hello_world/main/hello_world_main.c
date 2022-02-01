/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
   */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_task.h"
#include "driver/uart.h"

#include "tasktest.h"

#define BUF_SIZE (1024)

void app_main()
{
  // Configure parameters of an UART driver,
  // communication pins and install the driver
  uart_config_t uart_config = {
    .baud_rate = 115200,             
    .data_bits = UART_DATA_8_BITS,  
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_NUM_0, &uart_config);
  uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);

  printf("Hello world!\n");

  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
      chip_info.cores);

  printf("silicon revision %d, ", chip_info.revision);

  printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  for (int i = 3; i >= 0; i--) {
    printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  /*printf("Restarting now.\n");*/
  fflush(stdout);
  /*esp_restart();*/
  printf("Creating tasks\n");

  /*assert(xTaskCreate(taskTest, "uiT", ESP_TASK_MAIN_STACK, NULL, configMAX_PRIORITIES, NULL) == pdPASS);*/
  for (;;) {}
}
