#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/uart.h"

#include "uart_task.h"
#include "cmd_shell_task.h"

static const char *TAG = "uart";

static QueueHandle_t uart0_queue;

bool Uart_TaskInit(void) // To be called by the app_main()
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
  uart_param_config(UART_NUM, &uart_config);

  // Install UART driver, and get the queue.
  uart_driver_install(UART_NUM, RX_BUF_SIZE * 2, TX_BUF_SIZE * 2, QUEUE_SIZE, &uart0_queue, 0);

  return true;
}

void Uart_Task(void *params)
{
  printf("UART Task created!!!\n");
  uart_event_t event;
  uint8_t *dtmp = (uint8_t *)malloc(RX_BUF_SIZE);
  while(1) 
  {
    if (xQueueReceive(uart0_queue, (void *)&event, (portTickType)portMAX_DELAY))
    {
      switch (event.type)
      {
        case UART_DATA: /*!< UART data event*/
          {
            ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
            uart_read_bytes(UART_NUM, dtmp, event.size, portMAX_DELAY);
            CmdShell_AppendToCommandBuf((char *)dtmp, event.size);
            break;
          }
        case UART_BUFFER_FULL: /*!< UART RX buffer full event*/
          {
            ESP_LOGI(TAG, "ring buffer full");
            uart_flush_input(UART_NUM);
            xQueueReset(uart0_queue);
            break;
          }
        case UART_FIFO_OVF: /*!< UART FIFO overflow event*/
          {
            ESP_LOGI(TAG, "hw fifo overflow");
            uart_flush_input(UART_NUM);
            xQueueReset(uart0_queue);
            break;
          }
        case UART_FRAME_ERR: /*!< UART RX frame error event*/
          {
            ESP_LOGI(TAG, "uart frame error");
            break;
          }
        case UART_PARITY_ERR: /*!< UART RX parity event*/
          {
            ESP_LOGI(TAG, "uart parity error");
            break;
          }
        default:
          {
            ESP_LOGI(TAG, "uart event type: %d", event.type);
            break;
          }
      }
    }
  }
  free(dtmp);
  dtmp = NULL;
  vTaskDelete(NULL);
}
